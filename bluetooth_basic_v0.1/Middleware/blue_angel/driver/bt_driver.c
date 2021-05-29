#include "bt_driver.h"
#include "bt_config.h"
#include "bt_os_layer_api.h"
#include "bsp.h"
#include "bt_log.h"

static bool bt_driver_is_inited = false;
static bt_driver_rx_state_t rx_state = BT_DRIVER_WAIT_4_INDICATOR;
static uint32_t bt_driver_mutex = 0;

void bt_driver_share_buffer_lock()
{
	BT_ASSERT(bt_driver_mutex);
	bt_os_layer_take_mutex(bt_driver_mutex);
}

void bt_driver_share_buffer_unlock()
{
	BT_ASSERT(bt_driver_mutex);
	bt_os_layer_give_mutex(bt_driver_mutex);
}

void bt_driver_hardware_reset_controller()
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(BT_CONTROLLER_HARDWARE_RESET_PIN_PERIPH, ENABLE);
    GPIO_InitStructure.GPIO_Pin = BT_CONTROLLER_HARDWARE_RESET_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(BT_CONTROLLER_HARDWARE_RESET_PIN_GROUP, &GPIO_InitStructure);
	
    GPIO_WriteBit(BT_CONTROLLER_HARDWARE_RESET_PIN_GROUP, BT_CONTROLLER_HARDWARE_RESET_PIN, Bit_RESET);
    vTaskDelay(200);
    GPIO_WriteBit(BT_CONTROLLER_HARDWARE_RESET_PIN_GROUP, BT_CONTROLLER_HARDWARE_RESET_PIN, Bit_SET);
}

void bt_driver_init()
{
	if (!bt_driver_is_inited) {
		//bsp_InitUart();
		bt_driver_hardware_reset_controller();
		if (!bt_driver_mutex) {
			bt_driver_mutex = bt_os_layer_create_mutex();
			BT_ASSERT(bt_driver_mutex);
		}
		bt_driver_is_inited = true;
		BT_DRIVER_LOG_INFO("[BT_DRIVER] bt_driver init\r\n");
	}
}

void bt_driver_deinit()
{
	bt_driver_hardware_reset_controller();
	if (bt_driver_mutex) {
		bt_os_layer_delete_mutex(bt_driver_mutex);
		bt_driver_mutex = 0;
	}
	bt_driver_is_inited = false;
	rx_state = BT_DRIVER_WAIT_4_INDICATOR;
	BT_DRIVER_LOG_INFO("[BT_DRIVER] bt_driver deinit\r\n");
}

void bt_driver_recieve_data_from_controller(uint8_t data)
{
	BT_DRIVER_LOG_INFO("[BT_DRIVER] bt_driver rx from controller = %d\r\n", data);
}

void bt_driver_send_data_to_controller(uint8_t *buf, uint16_t buf_size)
{
	comSendBuf(COM2, buf, buf_size);
}


