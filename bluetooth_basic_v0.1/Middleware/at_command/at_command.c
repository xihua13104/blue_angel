/******************************************************************************
  * @file           at_command.c
  * @author         leon
  * @version        V0.1
  * @date           2021-05-16
  * @brief
******************************************************************************/
#include "at_command.h"
#include "FreeRTOS.h"
#include "timers.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "timers.h"
#include "bt_os_layer_api.h"
#include "bsp_uart_fifo.h"
#include "at_command_bt.h"
static at_command_state_t g_state = AT_COMMAND_STATE_IDLE;
static at_command_parse_state_t g_parse_state = AT_COMMAND_WAIT_4_HEADER_A;
static QueueHandle_t at_command_queue_handle = NULL;
static TimerHandle_t at_command_timer_handle = NULL;
static at_command_queue_message_t g_message = {0};
static uint8_t g_parse_index = 0;
static at_command_item_t at_cmd_item_table[AT_COMMAND_ITEM_MAX_COUNT] = {
    {"AT+BTSYSIT", at_command_bt_system_it_handler},
};

static at_command_status_t at_command_timer_stop(void);

static void at_command_timeout_callback(TimerHandle_t handle)
{
    if (handle != NULL && handle == at_command_timer_handle) {
        if (AT_COMAND_STATE_PARSING == g_state) {
            g_parse_state = AT_COMMAND_WAIT_4_HEADER_A;
            g_parse_index = 0;
            memset((void *)&g_message, 0, sizeof(at_command_queue_message_t));
            comClearRxFifo(COM1);
            AT_COMMAND_LOG_INFO("[AT_COMMAND] error\r\n");
        } else if (AT_COMMAND_STATE_EXECUTING == g_state) {
            AT_COMMAND_LOG_INFO("[AT_COMMAND] execute timeout\r\n");
        }
    }
}

static at_command_status_t at_command_timer_stop()
{
    if (bt_os_layer_is_isr_active()) {
        if (pdFALSE == xTimerStopFromISR(at_command_timer_handle, 0)) {
			return AT_COMMAND_FAIL;
		}
    } else {
        if (pdFALSE == xTimerStop(at_command_timer_handle, 0)) {
			return AT_COMMAND_FAIL;
		}
    }
	return AT_COMMAND_OK;
}

static at_command_status_t at_command_timer_start(uint32_t timer_length)
{
    uint32_t time = timer_length / portTICK_PERIOD_MS;
    if (bt_os_layer_is_isr_active()) {
        /*if (xTimerIsTimerActive(at_command_timer_handle) != pdFALSE) {
            return;
        }*/
        if (pdFALSE == xTimerChangePeriodFromISR(at_command_timer_handle, time, 0)) {
			return AT_COMMAND_FAIL;
		}
        if (pdFALSE == xTimerResetFromISR(at_command_timer_handle, 0)) {
			return AT_COMMAND_FAIL;
		}
    } else {
        /*if (xTimerIsTimerActive(at_command_timer_handle) != pdFALSE) {
        	return;
        }**/
        if (pdFALSE == xTimerChangePeriod(at_command_timer_handle, time, 0)) {
			return AT_COMMAND_FAIL;
		}
        if (pdFALSE == xTimerReset(at_command_timer_handle, 0)) {
			return AT_COMMAND_FAIL;
		}
    }
	return AT_COMMAND_OK;
}

at_command_status_t at_command_init(void)
{
    AT_COMMAND_LOG_INFO("[AT_COMMAND] at_command_init\r\n");
    at_command_queue_handle = xQueueCreate(AT_COMMAND_QUEUE_LENGTH, sizeof(at_command_queue_message_t));
    if (at_command_queue_handle == NULL) {
        return AT_COMMAND_FAIL;
    }
    if (at_command_timer_handle == NULL) {
        at_command_timer_handle = xTimerCreate("AT_CMD_Timer", 0xffff, pdFALSE, NULL, at_command_timeout_callback);
        if (at_command_timer_handle == NULL) {
            return AT_COMMAND_FAIL;
        }
    }
    return AT_COMMAND_OK;
}

at_command_status_t at_command_deinit(void)
{
    AT_COMMAND_LOG_INFO("[AT_COMMAND] at_command_deinit\r\n");
    if (at_command_queue_handle) {
        vQueueDelete(at_command_queue_handle);
        at_command_queue_handle = NULL;
    }
    if (at_command_timer_handle) {
        xTimerDelete(at_command_timer_handle, 0);
        at_command_timer_handle = NULL;
    }
	g_state = AT_COMMAND_STATE_IDLE;

    return AT_COMMAND_OK;
}

at_command_status_t at_command_register_handler(at_command_item_t *item)
{
    uint8_t i = 0;
    if (NULL == item || NULL == item->name || NULL == item->handler) {
        return AT_COMMAND_INVALID_PARAM;
    }

    for (i = 0; i < AT_COMMAND_ITEM_MAX_COUNT; i++) {
        if (at_cmd_item_table[i].handler == NULL) {
            at_cmd_item_table[i].handler = item->handler;
			at_cmd_item_table[i].name = item->name;
            return AT_COMMAND_OK;;
        }
    }
    return AT_COMMAND_FAIL;
}

at_command_status_t at_command_deregister_handler(at_command_item_t *item)
{
    uint8_t i = 0;
    if (NULL == item || NULL == item->name || NULL == item->handler) {
        return AT_COMMAND_INVALID_PARAM;
    }
    for (i = 0; i < AT_COMMAND_ITEM_MAX_COUNT; i++) {
        if (at_cmd_item_table[i].handler == item->handler &&
                0 == strncmp((const char *)at_cmd_item_table[i].name, item->name, strlen(item->name))) {
            at_cmd_item_table[i].handler = NULL;
			at_cmd_item_table[i].name = NULL;
            return AT_COMMAND_OK;
        }
    }
    return AT_COMMAND_FAIL;
}


void vTaskATCommand(void *pvParameters)
{
    at_command_queue_message_t queue_message = {0};
    char *ptr = NULL;
    uint8_t i = 0, acture_length = 0;
    while (1) {
        if (pdTRUE == xQueueReceive(at_command_queue_handle, (void *)&queue_message, portMAX_DELAY)) {
            g_state = AT_COMMAND_STATE_EXECUTING;
            ptr = (char *)pvPortMalloc(queue_message.at_cmd_tatal_length);
            AT_COMMAND_ASSERT(ptr);
			memset(ptr, 0, queue_message.at_cmd_tatal_length);
            acture_length = comGetBuf(COM1, (uint8_t *)ptr, queue_message.at_cmd_tatal_length);
            AT_COMMAND_ASSERT(acture_length == queue_message.at_cmd_tatal_length);
			AT_COMMAND_LOG_INFO("[AT_COMMAND] %s", ptr);
            for (i = 0; i < AT_COMMAND_ITEM_MAX_COUNT; i++) {
                if (0 == strncmp(ptr, at_cmd_item_table[i].name, queue_message.at_cmd_name_length)) {
                    if (at_cmd_item_table[i].handler) {
                        at_cmd_item_table[i].handler((void *)(ptr + queue_message.at_cmd_param_offset), queue_message.at_cmd_param_length);
                    }
                    break;
                }
            }
			if (ptr) {
				vPortFree(ptr);
				ptr = NULL;
			}
        }
        memset((void *)&queue_message, 0, sizeof(at_command_queue_message_t));
    }
}

void at_command_send_response(at_command_response_t *response)
{
	if (response == NULL) {
		return;
	}
	AT_COMMAND_LOG_INFO("[AT_COMMAND] %s\r\n", response->buffer);
}

void at_command_parsing(uint8_t data)
{
    BaseType_t pxHigherPriorityTaskWoken;
	at_command_status_t status = AT_COMMAND_OK;

    switch (g_parse_state) {
        case AT_COMMAND_WAIT_4_HEADER_A: {  //'A'
            if (data == AT_COMMAND_HEADER_A) {
                status = at_command_timer_start(AT_COMMAND_PARSE_TIMEOUT_LENGTH);
                g_state = AT_COMAND_STATE_PARSING;
                g_parse_index++;
                g_parse_state = AT_COMMAND_WAIT_4_HEADER_T;
            }
            break;
        }
        case AT_COMMAND_WAIT_4_HEADER_T: { //'T'
            if (data == AT_COMMAND_HEADER_T) {
                g_parse_index++;
                g_parse_state = AT_COMMAND_WAIT_4_HEADER_PLUS;
            }
            break;
        }
        case AT_COMMAND_WAIT_4_HEADER_PLUS: { //'+'
            if (data == AT_COMMAND_HEADER_PLUS) {
                g_parse_index++;
                g_parse_state = AT_COMMAND_WAIT_4_NAME;
            }
            break;
        }
        case AT_COMMAND_WAIT_4_NAME: {
            g_parse_index++;
            if (data == AT_COMMAND_EQUAL) { //AT+BTPW=1\r\n
                g_parse_state = AT_COMMAND_WAIT_4_PARAM;
                g_message.at_cmd_name_length = g_parse_index - 1;
                g_message.at_cmd_param_offset = g_parse_index;
                g_message.mode = AT_COMMAND_EXECUTION;
            } else if (data == AT_COMMAND_TAIL_R) { //AT+PWF\r\n
                g_parse_state = AT_COMMAND_WAIT_4_TAIL_N;
                g_message.at_cmd_name_length = g_parse_index - 1;
                g_message.mode = AT_COMMAND_ACTIVE;
            }
            break;
        }
        case AT_COMMAND_WAIT_4_PARAM: {
            g_parse_index++;
            if (data == AT_COMMAND_TAIL_R) {
                g_parse_state = AT_COMMAND_WAIT_4_TAIL_N;
            }
            break;
        }
        case AT_COMMAND_WAIT_4_TAIL_R: { //\r
            if (data == AT_COMMAND_TAIL_R) {
                g_parse_index++;
                g_parse_state = AT_COMMAND_WAIT_4_TAIL_N;
            }
            break;
        }
        case AT_COMMAND_WAIT_4_TAIL_N: { //Ω‚ŒˆÕÍ≥…,clear context
            if (data == AT_COMMAND_TAIL_N) {
                g_parse_index++;
                g_message.at_cmd_tatal_length = g_parse_index;
                if (AT_COMMAND_EXECUTION == g_message.mode) {
                    g_message.at_cmd_param_length = g_parse_index - g_message.at_cmd_param_offset - AT_COMMAND_TAIL_LENGTH;
                }
                at_command_timer_stop();
                xQueueSendFromISR(at_command_queue_handle, (const void *)&g_message, &pxHigherPriorityTaskWoken);
                g_parse_state = AT_COMMAND_WAIT_4_HEADER_A;
                g_parse_index = 0;
                memset((void *)&g_message, 0, sizeof(at_command_queue_message_t));
                g_state = AT_COMAND_STATE_PARSE_DONE;
            }
            break;
        }
        default: {
            break;
        }
    }
}
