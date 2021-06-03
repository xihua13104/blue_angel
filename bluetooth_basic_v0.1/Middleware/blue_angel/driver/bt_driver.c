#include "bt_driver.h"
#include "bt_config.h"
#include "bt_os_layer_api.h"
#include "bsp.h"
#include "bt_log.h"
#include "bt_common.h"
#include "bt_hci_spec.h"

static bool bt_driver_is_inited = false;
static bt_driver_rx_state_t rx_state = BT_DRIVER_WAIT_4_INDICATOR;
static uint8_t buffer[6] = {0};
static uint16_t buffer_index = 0;
static bt_hci_spec_packet_t *g_packet = (bt_hci_spec_packet_t *)buffer;
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
	uint16_t length = 0;

	if (buffer_index < sizeof(buffer)) {
		buffer[buffer_index++] = data;	
	}

	switch (rx_state) {
		case BT_DRIVER_WAIT_4_INDICATOR: {
			if (data == BT_UART_ACL || data == BT_UART_EVT) {
				rx_state = BT_DRIVER_WAIT_4_HEADER;			
			}
			break;
		}
		case BT_DRIVER_WAIT_4_HEADER: {
			if (g_packet->indicator == BT_UART_ACL && buffer_index >= BT_HCI_ACL_HEADER_SIZE) {
				rx_state = BT_DRIVER_WAIT_4_PAYLOAD;
			} else if (g_packet->indicator == BT_UART_EVT && buffer_index >= BT_HCI_EVT_HEADER_SIZE) {
				rx_state = BT_DRIVER_WAIT_4_PAYLOAD;
			}
			break;
		}
		case BT_DRIVER_WAIT_4_PAYLOAD: {
			payload_index++;
			if (g_packet->indicator == BT_UART_ACL && payload_index >= g_packet->value.acl.length) {
				rx_state = BT_DRIVER_RX_COMPLETE;
			} else if (g_packet->indicator == BT_UART_EVT && payload_index >= g_packet->value.evt.length) {
				rx_state = BT_DRIVER_RX_COMPLETE;
			} else {
				break;
			}
		}	
		case BT_DRIVER_RX_COMPLETE: {
			if (g_packet->indicator == BT_UART_ACL) {
				length = BT_HCI_ACL_SIZE(g_packet);		
			} else if (g_packet->indicator == BT_UART_EVT) {
				length = BT_HCI_EVT_SIZE(g_packet);
			}
			bt_task_event_notify(BT_TASK_EVENT_RX, length, NULL);
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
	BT_FREE_HCI_PACKET_WITH_NODE(BT_MEMORY_TX, buf);
}

void bt_driver_rx(uint16_t length)
{
	bt_hci_spec_packet_t *hci_spec_packet = NULL;
	uint16_t acture_length = 0;
	hci_spec_packet = (bt_hci_spec_packet_t *)BT_ALLOCATE_HCI_PACKET_WITH_NODE(BT_MEMORY_RX, length);
	BT_ASSERT(hci_spec_packet != NULL);
	acture_length = comGetBuf(COM2, (uint8_t *)hci_spec_packet, length);
	BT_ASSERT(acture_length == length);
	BT_PUSH_NODE_TO_RX_QUEUE_TAIL(BT_GET_NODE_FROM_HCI_SPEC_PACKET(hci_spec_packet));
}
