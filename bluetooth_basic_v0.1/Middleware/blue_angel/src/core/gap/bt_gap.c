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

static uint16_t bt_gap_init_table_index = 0;

static bt_hci_cmd_t bt_gap_init_table[] = {
	{BT_HCI_CMD_RESET, 0, NULL},
	{BT_HCI_CMD_READ_BUFFER_SIZE, 0, NULL},
	{BT_HCI_CMD_READ_LOCAL_VERSION_INFO, 0, NULL},
	{BT_HCI_CMD_READ_BD_ADDR, 0, NULL},
	{BT_HCI_CMD_CHANGE_LOCAL_NAME, 0, NULL},
};

bt_status_t bt_gap_init_process(bool is_timeout, uint32_t timer_id, uint32_t data, const void *param)
{
	return BT_STATUS_SUCCESS;
}

bt_status_t bt_power_on()
{
	return BT_STATUS_SUCCESS;
}

bt_status_t bt_power_off()
{
	return BT_STATUS_SUCCESS;
}

bt_status_t bt_power_reset()
{
	return BT_STATUS_SUCCESS;
}
