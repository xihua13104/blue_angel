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
#include "bt_platform.h"
#include "bt_driver.h"

bt_status_t bt_hci_cmd_send(bt_hci_cmd_t cmd, uint32_t data, bt_hci_timeout_callback_t callback)
{
	bt_status_t status = BT_STATUS_SUCCESS;
	bt_hci_spec_packet_t *packet = NULL;
	uint16_t length = cmd.length + BT_HCI_CMD_HEADER_SIZE;
	uint32_t timer_id = cmd.cmd_code | BT_MODULE_HCI;

	BT_HCI_LOG_INFO("[BT_HCI] cmd send, cmd_code = 0x%x \r\n", cmd.cmd_code);
	packet = (bt_hci_spec_packet_t *)BT_ALLOCATE_HCI_PACKET_WITH_NODE(BT_MEMORY_TX, length);
	if (packet == NULL) {
		BT_HCI_LOG_ERROR("[BT_HCI] cmd send fail, OOM \r\n");
		return BT_STATUS_OUT_OF_MEMORY;
	}
	packet->indicator = BT_UART_CMD;
	packet->value.cmd.cmd_code = cmd.cmd_code;
	packet->value.cmd.length = cmd.length;
	if (cmd.length > 0) {
		bt_memcpy((void *)&packet->value.cmd.data[0], cmd.param, cmd.length);	
	}
	if (callback) {
		if (bt_timer_find(timer_id)) {
			BT_HCI_LOG_ERROR("[BT_HCI] cmd send fail, timer already exist\r\n");
			BT_FREE_HCI_PACKET_WITH_NODE(BT_MEMORY_TX, packet);
			return BT_STATUS_TIMER_EXIST;
		}
		status = bt_timer_start(timer_id, BT_HCI_TIMEOUT_LENGTH, data, callback);
		BT_ASSERT(status == BT_STATUS_SUCCESS);
	}
	bt_driver_send_data_to_controller((uint8_t *)packet, BT_HCI_CMD_SIZE(packet));
	bt_hci_log(0, (uint8_t *)packet, BT_HCI_CMD_SIZE(packet));
	return status;
}

bt_status_t bt_hci_evt_handler(bt_hci_spec_packet_t *packet)
{
	bt_hci_evt_code_t evt_code = packet->value.evt.evt_code;
	uint32_t timer_id = BT_MODULE_HCI;
	switch (evt_code) {
		case 0x0E:
			timer_id |= *(bt_hci_cmd_code_t *)((uint8_t *)packet + 4);
			break;
		default:
			break;
	}
	return bt_timer_cancel_and_callback(timer_id, (void *)packet);
}

bt_status_t bt_hci_acl_handler(bt_hci_spec_packet_t *packet)
{
	return 0;
}

void bt_hci_packet_process()
{
	bt_hci_spec_packet_t *packet = NULL;
	
	while (!BT_QUEUE_IS_EMPTY(BT_RX_QUEUE)) {
		packet = (bt_hci_spec_packet_t *)BT_GET_HCI_SPEC_PACKET_FROM_NODE(BT_POP_NODE_FROM_RX_QUEUE_FRONT());
		if (packet->indicator == BT_UART_ACL) {
			bt_hci_log(1, (uint8_t *)packet, BT_HCI_ACL_SIZE(packet));
			bt_hci_acl_handler(packet);
		} else if (packet->indicator == BT_UART_EVT) {
			bt_hci_log(1, (uint8_t *)packet, BT_HCI_EVT_SIZE(packet));
			bt_hci_evt_handler(packet);
		}
		BT_FREE_HCI_PACKET_WITH_NODE(BT_MEMORY_RX, packet);
	}
}
