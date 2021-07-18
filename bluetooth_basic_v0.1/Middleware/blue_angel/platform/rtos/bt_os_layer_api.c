/******************************************************************************
  * @file           bt_os_layer_api.c
  * @author         leon
  * @version        V0.1
  * @date           2021-05-19
  * @brief
******************************************************************************/
#include "bt_os_layer_api.h"
#include "FreeRTOS.h"
#include "timers.h"
#include "task.h"
#include "semphr.h"

static TimerHandle_t bt_rtos_timer = NULL;
static bt_os_layer_timer_expire_callback_t bt_os_expire_cb = NULL;

static void bt_os_layer_timer_timeout_callback(TimerHandle_t xTimer)
{
    if (bt_os_expire_cb) {
        bt_os_expire_cb();
    }
}

void bt_os_layer_register_timer_callback(bt_os_layer_timer_expire_callback_t cb)
{
    BT_ASSERT(cb);
    bt_os_expire_cb = cb;
}

void bt_os_layer_init_timer()
{
    if (bt_rtos_timer == NULL) {
        bt_rtos_timer = xTimerCreate("BT_Timer", 0xffff, pdFALSE, NULL, bt_os_layer_timer_timeout_callback);
        BT_ASSERT(bt_rtos_timer && "create timer fail");
    }
}

void bt_os_layer_deinit_timer()
{
    if (bt_rtos_timer) {
        xTimerDelete(bt_rtos_timer, 0);
        bt_rtos_timer = NULL;
    }
}

bool bt_os_layer_is_timer_active()
{
    BT_ASSERT(bt_rtos_timer);
    if (xTimerIsTimerActive(bt_rtos_timer) != pdFALSE) {
        return true;
    } else {
        return false;
    }
}

void bt_os_layer_start_timer(uint32_t ms)
{
    uint32_t time = ms / portTICK_PERIOD_MS;
    BT_ASSERT(bt_rtos_timer);
    if (bt_os_layer_is_timer_active()) {
        bt_os_layer_stop_timer();
    }
    BT_ASSERT(pdFAIL != xTimerChangePeriod(bt_rtos_timer, time, 0));
    BT_ASSERT(pdFAIL != xTimerReset(bt_rtos_timer, 0));
}

void bt_os_layer_stop_timer()
{
    BT_ASSERT(bt_rtos_timer);
    xTimerStop(bt_rtos_timer, 0);
}

uint32_t bt_os_layer_get_system_tick()
{
    return xTaskGetTickCount();
}

void bt_os_layer_disable_interrupt()
{
    //taskENTER_CRITICAL();
    __set_PRIMASK(1);
}

void bt_os_layer_enable_interrupt()
{
    //taskEXIT_CRITICAL();
    __set_PRIMASK(0);
}

uint32_t bt_os_layer_create_mutex()
{
    return (uint32_t)xSemaphoreCreateRecursiveMutex();
}

void bt_os_layer_delete_mutex(uint32_t mutex_id)
{
    vSemaphoreDelete((SemaphoreHandle_t)mutex_id);
}

void bt_os_layer_take_mutex(uint32_t mutex_id)
{
    if (xTaskGetSchedulerState() == taskSCHEDULER_NOT_STARTED) {
        return;
    }
    xSemaphoreTakeRecursive((SemaphoreHandle_t)mutex_id, portMAX_DELAY);
}

void bt_os_layer_give_mutex(uint32_t mutex_id)
{
    if (xTaskGetSchedulerState() == taskSCHEDULER_NOT_STARTED) {
        return;
    }
    xSemaphoreGiveRecursive((SemaphoreHandle_t)mutex_id);
}

uint32_t bt_os_layer_create_semaphore()
{
    return (uint32_t)xSemaphoreCreateBinary();
}

void bt_os_layer_delete_semaphore(uint32_t semaphore)
{
    vSemaphoreDelete((SemaphoreHandle_t)semaphore);
}

void bt_os_layer_take_semaphore(uint32_t semaphore, uint32_t block_time)
{
    if (xTaskGetSchedulerState() == taskSCHEDULER_NOT_STARTED) {
        return;
    }
    xSemaphoreTake((SemaphoreHandle_t)semaphore, block_time);
}

void bt_os_layer_take_semaphore_from_isr(uint32_t semaphore)
{
    BaseType_t pxHigherPriorityTaskWoken;
    if (xTaskGetSchedulerState() == taskSCHEDULER_NOT_STARTED) {
        return;
    }
    xSemaphoreTakeFromISR((SemaphoreHandle_t)semaphore, &pxHigherPriorityTaskWoken);
}

void bt_os_layer_give_semaphore(uint32_t semaphore)
{
    if (xTaskGetSchedulerState() == taskSCHEDULER_NOT_STARTED) {
        return;
    }
    xSemaphoreGive((SemaphoreHandle_t)semaphore);
}

void bt_os_layer_give_semaphore_from_isr(uint32_t semaphore)
{
    BaseType_t pxHigherPriorityTaskWoken;
    if (xTaskGetSchedulerState() == taskSCHEDULER_NOT_STARTED) {
        return;
    }
    xSemaphoreGiveFromISR((SemaphoreHandle_t)semaphore, &pxHigherPriorityTaskWoken);
}

uint32_t bt_os_layer_create_queue(uint32_t queue_length, uint32_t item_size)
{
	return (uint32_t)xQueueCreate(queue_length, item_size);
}

void bt_os_layer_delete_queue(uint32_t queue_handle)
{
	vQueueDelete((QueueHandle_t)queue_handle);
}

void bt_os_layer_queue_send(uint32_t queue_handle, const void * const item_to_queue, uint32_t block_time)
{
    if (xTaskGetSchedulerState() == taskSCHEDULER_NOT_STARTED) {
        return;
    }
	xQueueSend((QueueHandle_t)queue_handle, item_to_queue, block_time);
}

void bt_os_layer_queue_send_from_isr(uint32_t queue_handle, const void * const item_to_queue)
{
    BaseType_t pxHigherPriorityTaskWoken;
    if (xTaskGetSchedulerState() == taskSCHEDULER_NOT_STARTED) {
        return;
    }
	xQueueSendFromISR((QueueHandle_t)queue_handle, item_to_queue, &pxHigherPriorityTaskWoken);
}

void bt_os_layer_queue_receive(uint32_t queue_handle, void * const buffer, uint32_t block_time)
{
    if (xTaskGetSchedulerState() == taskSCHEDULER_NOT_STARTED) {
        return;
    }
	xQueueReceive((QueueHandle_t)queue_handle, buffer, block_time);
}

void bt_os_layer_queue_receive_from_isr(uint32_t queue_handle, void * const buffer)
{
    BaseType_t pxHigherPriorityTaskWoken;
    if (xTaskGetSchedulerState() == taskSCHEDULER_NOT_STARTED) {
        return;
    }
	xQueueReceiveFromISR((QueueHandle_t)queue_handle, buffer, &pxHigherPriorityTaskWoken);
}

bool bt_os_layer_is_isr_active()
{
    /*
    	CM3 : 9:0 VECTACTIVE R 0
    	当前活动的ISR编号，该位段指出当前运行中的ISR
    	是哪个中断的（提供异常序号），包括NMI和硬
    	fault。如果多个异常共享一个服务例程，该例程可根据
    	本位段的值来判定是哪一个异常的响应导致它的执行。把
    	本位段的值减去16,就得到了外中断的编号，并可以用此
    	编号来操作外中断相关的使能/除能等寄存器。
    */
#define SBC_ICSR         		  (*((volatile uint32_t *)0xE000ED04))
#define SBC_ICSR_VECTACTIVE_MASK  0x000003FF
#define EXCEPTION_NUMBER		  ((SBC_ICSR & SBC_ICSR_VECTACTIVE_MASK))
    int32_t exception = EXCEPTION_NUMBER;
    if (0 == exception) {
        return false;
    } else {
        return true;
    }
}

bool bt_os_layer_is_os_scheduler_start()
{
    return taskSCHEDULER_RUNNING == xTaskGetSchedulerState();
}
