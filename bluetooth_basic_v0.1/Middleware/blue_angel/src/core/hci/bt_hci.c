/******************************************************************************
  * @file           bt_hci.c
  * @author         Leonard-y.He(1027901556@qq.com)
  * @version        V0.1
  * @date           2021-05-19
  * @brief
******************************************************************************/
#include "bt_hci.h"
#include "bt_hci_spec.h"
#include "bt_common.h"
#include "bt_log.h"

void bt_hci_packet_process()
{
	bt_hci_spec_packet_t *packet = NULL;
	
	while (!BT_QUEUE_IS_EMPTY(BT_RX_QUEUE)) {
		packet = (bt_hci_spec_packet_t *)BT_GET_HCI_SPEC_PACKET_FROM_NODE(BT_POP_NODE_FROM_RX_QUEUE_FRONT());
		if (packet->indicator == BT_UART_ACL) {
			bt_hci_log(1, (uint8_t *)packet, BT_HCI_ACL_SIZE(packet));			
		} else if (packet->indicator == BT_UART_EVT) {
			bt_hci_log(1, (uint8_t *)packet, BT_HCI_EVT_SIZE(packet));		
		}
		BT_FREE_HCI_PACKET_WITH_NODE(BT_MEMORY_RX, BT_GET_NODE_FROM_HCI_SPEC_PACKET(packet));
	}
}
