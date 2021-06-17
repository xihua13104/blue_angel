/******************************************************************************
  * @file           bt_gap.c
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

bt_blue_angel_t blue_angel;

static uint8_t bt_gap_init_table_index = 0;
static bt_gap_scan_mode_t bt_scan_mode = BT_GAP_SCAN_MODE_GENERAL_ACCESSABLE;
static char *bt_gap_local_name = BT_LOCAL_NAME;

static bt_hci_cmd_t bt_gap_init_table[] = {
	{BT_HCI_CMD_RESET, 0, NULL},
	{BT_HCI_CMD_READ_BUFFER_SIZE, 0, NULL},
	{BT_HCI_CMD_READ_LOCAL_VERSION_INFO, 0, NULL},
	{BT_HCI_CMD_READ_BD_ADDR, 0, NULL},
	{BT_HCI_CMD_CHANGE_LOCAL_NAME, 0, NULL},
	{BT_HCI_CMD_WRITE_SCAN_ENABLE, 0, NULL},
	//{BT_HCI_CMD_WRITE_EXT_INQ_RESPONSE, 0, NULL}
};

static bt_hci_cmd_t *bt_gap_init_cmd_pop()
{
    bt_hci_cmd_t *cmd = NULL;
	if (bt_gap_init_table_index == sizeof(bt_gap_init_table)/sizeof(bt_hci_cmd_t)) {
		return NULL;
	}
	cmd = &bt_gap_init_table[bt_gap_init_table_index++];
	switch (cmd->cmd_code) {
		case BT_HCI_CMD_CHANGE_LOCAL_NAME:
			cmd->length = strlen(bt_gap_local_name);
			cmd->param = (void *)bt_gap_local_name;
			break;
		case BT_HCI_CMD_WRITE_SCAN_ENABLE:
			cmd->length = sizeof(bt_scan_mode);
			cmd->param = (void *)&bt_scan_mode;
			break;
	}
	return cmd;
}

bt_status_t bt_gap_init_process(bool is_timeout, uint32_t timer_id, uint32_t data, const void *param)
{
	bt_status_t status = BT_STATUS_SUCCESS;
	bt_hci_cmd_t *cmd = NULL;
	uint32_t need_retry = data;

	if (is_timeout) {
		BT_GAP_LOG_ERROR("[BT_GAP_COMMON] init cmd timeout, timer_id=0x%x\r\n", timer_id);
		if (need_retry && timer_id == BT_HCI_TIMER_ID_TYPE_A(BT_HCI_CMD_RESET)) {
			bt_gap_init_table_index = 0;
			cmd = bt_gap_init_cmd_pop();
			return bt_hci_cmd_send(*cmd, 1, 100, bt_gap_init_process);
		}
		BT_ASSERT(0);
	}
	
	switch (timer_id) {
		case BT_HCI_TIMER_ID_TYPE_A(BT_HCI_CMD_READ_BD_ADDR):
			bt_memcpy(&blue_angel.local_public_addr, ((bt_hci_command_complete_t *)((uint8_t *)param + BT_HCI_EVT_HEADER_SIZE))->data, sizeof(bt_bd_addr_t));
			break;
		case BT_HCI_TIMER_ID_TYPE_A(BT_HCI_CMD_READ_BUFFER_SIZE):
			break;
		default:
			break;
	}
		
	cmd = bt_gap_init_cmd_pop();
	if (cmd != NULL) {
		if (cmd->cmd_code == BT_HCI_CMD_RESET) {
			status = bt_hci_cmd_send(*cmd, 1, 100, bt_gap_init_process);
		} else {
			status = bt_hci_cmd_send(*cmd, 0, 5000, bt_gap_init_process);
		}
	} else {
		blue_angel.power_status = BT_POWER_ON;
		bt_app_event_callback(0,0,0);
	}
	return status;
}

bt_status_t bt_power_on()
{
	bt_memset(&blue_angel, 0, sizeof(blue_angel));
	blue_angel.power_status = BT_POWER_SWITCHING_ON;

	return bt_gap_init_process(false, BT_GAP_INVALID_TIMER_ID, 0, NULL);
	
}

bt_status_t bt_power_off()
{
	return BT_STATUS_SUCCESS;
}

bt_status_t bt_power_reset()
{
	return BT_STATUS_SUCCESS;
}
