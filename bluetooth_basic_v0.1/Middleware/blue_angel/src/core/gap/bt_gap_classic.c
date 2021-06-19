/******************************************************************************
  * @file           bt_gap_le.c
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
#include "bt_gap_classic.h"

bt_status_t bt_gap_write_eir(uint8_t *eir_data, uint8_t length)
{
	bt_gap_write_eir_t write_eir;
	bt_hci_cmd_t hci_cmd = {BT_HCI_CMD_WRITE_EXT_INQ_RESPONSE, 0, NULL};
	write_eir.fec_required = 1;
	bt_memcpy(write_eir.eir, eir_data, length);
	hci_cmd.length = sizeof(bt_gap_write_eir_t);
	hci_cmd.param = (void *)&write_eir;
	return bt_hci_cmd_send(hci_cmd, 0, 5000, NULL);
}
