/******************************************************************************
  * @file           bt_gap_connection.c
  * @author         leon
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
#include "bt_config.h"
#include "bt_gap_internal.h"
#include "bt_gap_classic.h"
#include "bt_callback_manager.h"
#include "bt_gap_classic.h"

bool bt_gap_connection_cmp_by_address(const bt_linknode_t *node, const void *data)
{
    bt_gap_classic_connection_t *classic_con = NULL;
	bt_bd_addr_t *address;
    BT_ASSERT(node && data);	
	if (node->next == NULL) {
		return true;
	}
	address = (bt_bd_addr_t *)data;
	classic_con = (bt_gap_classic_connection_t *)node->next;
	if (0 == bt_memcmp((void *)&classic_con->address, (void *)address, sizeof(bt_bd_addr_t))) {
		return true;
	} else {
		return false;
	}
}

bt_gap_classic_connection_t *bt_gap_find_connection_by_address(bt_bd_addr_t address)
{
	bt_gap_classic_connection_t *classic_con = NULL;
	bt_linknode_t *pre = NULL;
	pre = bt_linknode_travel_node(&blue_angel.bt_gap_classic_connection, bt_gap_connection_cmp_by_address, (const void *)&address);
	if (pre != NULL) {
		classic_con = (bt_gap_classic_connection_t *)pre->next;
	}
	return classic_con;
}

bt_status_t bt_gap_connection_handler(bool is_timeout, uint32_t timer_id, uint32_t data, const void *param)
{
	bt_status_t status = BT_STATUS_FAIL;
	bt_hci_cmd_t hci_cmd = {0};
	bt_hci_accept_connection_request_t accept_con_request = {0};
	if (is_timeout) {
		BT_GAP_LOG_ERROR("[BT_GAP][CONN] timeout, timer_id = 0x%x\r\n", timer_id);
		BT_ASSERT(0 && "timeout");
		return status;
	}
	switch (timer_id) {
		case BT_HCI_TIMER_ID_TYPE_B(BT_HCI_EVT_CONNECTION_REQUEST):
			if (BT_ACL_CONNECTION == BT_HCI_GET_EVT_PARAM(param, bt_hci_connection_request_t)->link_type) {
				if (NULL == bt_gap_find_connection_by_address(BT_HCI_GET_EVT_PARAM(param, bt_hci_connection_request_t)->address)) {
					bt_memcpy((void *)&accept_con_request.address, (void *)&BT_HCI_GET_EVT_PARAM(param, bt_hci_connection_request_t)->address, sizeof(bt_bd_addr_t));
					accept_con_request.role = BT_ROLE_SLAVE;
					hci_cmd.cmd_code = BT_HCI_CMD_ACCEPT_CONNECTION_REQUEST;
					hci_cmd.length = sizeof(bt_hci_accept_connection_request_t);
					hci_cmd.param = (void *)&accept_con_request;
					status = bt_hci_cmd_send(hci_cmd, 0, BT_HCI_CMD_TIMEOUT, NULL);
				}
			}
			break;
		case BT_HCI_TIMER_ID_TYPE_B(BT_HCI_EVT_CONNECTION_COMPLETE):
			break;
		default:
			break;
	}
	return status;
}
