#include "bt_driver.h"
#include "bt_config.h"
#include "bt_os_layer_api.h"
#include "bsp.h"
#include "bt_log.h"
#include "bt_common.h"
#include "bt_hci_spec.h"

typedef struct {
	bt_hci_spec_packet_indicator_t ind;
	union {
		struct {
			uint16_t handle;
			uint16_t length;
			uint16_t l2cap_channel;
			uint16_t l2cap_length;
		} BT_PACKED hci_acl;
		struct {
			uint8_t evt_code;
			uint8_t length;		
		} BT_PACKED hci_evt;
	} BT_PACKED hci_header;
} BT_PACKED bt_driver_rx_t;

static bool bt_driver_is_inited = false;
static bt_driver_rx_state_t rx_state = BT_DRIVER_WAIT_4_INDICATOR;
static uint8_t buffer[sizeof(bt_driver_rx_t)] = {0};
static uint16_t buffer_index = 0;
static bt_driver_rx_t *rx_struct = (bt_driver_rx_t *)buffer;
static uint16_t payload_index = 0;

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
		bt_driver_hardware_reset_controller();
		bt_driver_is_inited = true;
		BT_DRIVER_LOG_INFO("[BT_DRIVER] bt_driver init\r\n");
	}
}

void bt_driver_deinit()
{
	bt_driver_hardware_reset_controller();
	bt_driver_is_inited = false;
	BT_DRIVER_LOG_INFO("[BT_DRIVER] bt_driver deinit\r\n");
}

void bt_driver_recieve_data_from_controller(uint8_t data)
{
	bt_hci_packet_t *packet = NULL;
	if (buffer_index < sizeof(buffer)) {
		buffer[buffer_index++] = data;	
	}

	switch (rx_state) {
		case BT_DRIVER_WAIT_4_INDICATOR: {
			rx_state = BT_DRIVER_WAIT_4_HEADER;
			break;
		}
		case BT_DRIVER_WAIT_4_HEADER: {
			if (rx_struct->ind == BT_UART_ACL && buffer_index >= BT_HCI_ACL_HEADER_SIZE) {
				rx_state = BT_DRIVER_WAIT_4_PAYLOAD;
			} else if (rx_struct->ind == BT_UART_EVT && buffer_index >= BT_HCI_EVT_HEADER_SIZE) {
				rx_state = BT_DRIVER_WAIT_4_PAYLOAD;
			}
			break;
		}
		case BT_DRIVER_WAIT_4_PAYLOAD: {
			payload_index++;
			if (rx_struct->ind == BT_UART_ACL && payload_index >= rx_struct->hci_header.hci_acl.length) {
				rx_state = BT_DRIVER_RX_COMPLETE;
			} else if (rx_struct->ind == BT_UART_EVT && payload_index >= rx_struct->hci_header.hci_evt.length) {
				rx_state = BT_DRIVER_RX_COMPLETE;
			}
			break;
		}	
		case BT_DRIVER_RX_COMPLETE: {
			if (rx_struct->ind == BT_UART_ACL) {
				//packet = BT_ALLOCATE_HCI_PACKET_WITH_NODE(BT_MEMORY_RX, );
			} else if (rx_struct->ind == BT_UART_EVT) {
			
			}
			bt_memset(buffer, 0, sizeof(buffer));
			rx_state = BT_DRIVER_WAIT_4_INDICATOR;
			buffer_index = 0;
			payload_index = 0;
			break;
		}
		default : {
			break;
		}
	}
}

void bt_driver_send_data_to_controller(uint8_t *buf, uint16_t buf_size)
{
	comSendBuf(COM2, buf, buf_size);
}


