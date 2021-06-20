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

#define BT_GAP_CMD_TIMEOUT_ASSERT	0x01
#define BT_GAP_CMD_TIMEOUT_RETRY	0x02
#define BT_GAP_CMD_TIMEOUT_IGNORE	0x04

typedef struct {
	uint8_t attribute;
	bt_hci_cmd_t hci_cmd;
} bt_gap_init_cmd_t;

bt_blue_angel_t blue_angel;

/*************************************************************************************************
HCI command  PACKETS Format:
	opcode 16 bit
	para total len 8 bit
	para 0
**************************************************************************************************/
/*************************************************************************************************
BCCMD PACKETS Format:
           |	  type   |	length     |	seqno   |		varid	|	status   |	payload	    |
 uint 16   |	    1    |       2     |      3     |       4       |     5      |        6~    |
**************************************************************************************************/
/*************************************************************************************************
BCCMD PS PACKETS Format:
           |	  header  |	key     |	length   |	stores        |		ps value  	  |
 uint 16   |	    1-5   |    6    |      7     |       8        |      9~           |
**************************************************************************************************/
#if 0
uint8_t csr8x11_initscript[] =
{
    //  Set PSKEY_DEEP_SLEEP_STATE never deep sleep
    0x13, 0xc2, 0x02, 0x00, 0x09, 0x00, 0x02, 0x00, 0x03, 0x70, 0x00, 0x00, 0x29, 0x02, 0x01, 0x00, 0x08, 0x00, 0x00, 0x00,
    //  Set ANA_Freq to 26MHz
    0x13, 0xc2, 0x02, 0x00, 0x09, 0x00, 0x03, 0x00, 0x03, 0x70, 0x00, 0x00, 0xfe, 0x01, 0x01, 0x00, 0x08, 0x00, 0x90, 0x65,
    //  Set CSR_PSKEY_ANA_FTRIM 0x24 for csr8811
    0x13, 0xc2, 0x02, 0x00, 0x09, 0x00, 0x04, 0x00, 0x03, 0x70, 0x00, 0x00, 0xf6, 0x01, 0x01, 0x00, 0x08, 0x00, 0x24, 0x00,
    // Set CSR_PSKEY_DEFAULT_TRANSMIT_POWER 0x4
    0x13, 0xc2, 0x02, 0x00, 0x09, 0x00, 0x05, 0x00, 0x03, 0x70, 0x00, 0x00, 0x21, 0x00, 0x01, 0x00, 0x08, 0x00, 0x04, 0x00,
    // Set CSR_PSKEY_MAXIMUM_TRANSMIT_POWER 0x4
    0x13, 0xc2, 0x02, 0x00, 0x09, 0x00, 0x06, 0x00, 0x03, 0x70, 0x00, 0x00, 0x17, 0x00, 0x01, 0x00, 0x08, 0x00, 0x04, 0x00,
    // Set CSR_PSKEY_BLE_DEFAULT_TRANSMIT_POWER 0x4
    0x13, 0xc2, 0x02, 0x00, 0x09, 0x00, 0x07, 0x00, 0x03, 0x70, 0x00, 0x00, 0xc8, 0x22, 0x01, 0x00, 0x08, 0x00, 0x04, 0x00,
    // Set CSR_PSKEY_BDADDR
    0x19, 0xc2, 0x02, 0x00, 0x0c, 0x00, 0x08, 0x00, 0x03, 0x70, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x08, 0x00, 0x20, 0x00, 0x99, 0x1a, 0x86, 0x00, 0x1d, 0x00,
    // Set CSR_PSKEY_PCM_CONFIG32
    0x15, 0xc2, 0x02, 0x00, 0x0a, 0x00, 0x09, 0x00, 0x03, 0x70, 0x00, 0x00, 0xb3, 0x01, 0x02, 0x00, 0x08, 0x00, 0x80, 0x08, 0x80, 0x18,
    // Set CSR_PSKEY_PCM_FORMAT 0x60
    0x13, 0xc2, 0x02, 0x00, 0x09, 0x00, 0x0a, 0x00, 0x03, 0x70, 0x00, 0x00, 0xb6, 0x01, 0x01, 0x00, 0x08, 0x00, 0x60, 0x00,
    // Set CSR_PSKEY_USER_LOW_JITTER_MODE
    0x13, 0xc2, 0x02, 0x00, 0x09, 0x00, 0x0b, 0x00, 0x03, 0x70, 0x00, 0x00, 0xc9, 0x23, 0x01, 0x00, 0x08, 0x00, 0x01, 0x00,
    //  Set HCI_NOP_DISABLE
    0x13, 0xc2, 0x02, 0x00, 0x09, 0x00, 0x0c, 0x00, 0x03, 0x70, 0x00, 0x00, 0xf2, 0x00, 0x01, 0x00, 0x08, 0x00, 0x01, 0x00,
    // Set UART baudrate to 921600
    0x15, 0xc2, 0x02, 0x00, 0x0a, 0x00, 0x0d, 0x00, 0x03, 0x70, 0x00, 0x00, 0xea, 0x01, 0x02, 0x00, 0x08, 0x00,0x0e,0x00,0x00,0x10,/*0x1b, 0x00, 0x40, 0x77,*/
    //  WarmReset
    0x13, 0xc2, 0x02, 0x00, 0x09, 0x00, 0x0e, 0x00, 0x02, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
#endif

static uint8_t bt_gap_init_table_index = 0;
static uint8_t vendor_set_pskey_deep_sleep_state[] = {0xc2, 0x02, 0x00, 0x09, 0x00, 0x02, 0x00, 0x03, 0x70, 0x00, 0x00, 0x29, 0x02, 0x01, 0x00, 0x08, 0x00, 0x00, 0x00}; //never deep sleep
static uint8_t vendor_set_ana_freq_26mhz[] = {0xc2, 0x02, 0x00, 0x09, 0x00, 0x03, 0x00, 0x03, 0x70, 0x00, 0x00, 0xfe, 0x01, 0x01, 0x00, 0x08, 0x00, 0x90, 0x65};	
static uint8_t vendor_set_csr_pskey_ana_ftrim[] = {0xc2, 0x02, 0x00, 0x09, 0x00, 0x04, 0x00, 0x03, 0x70, 0x00, 0x00, 0xf6, 0x01, 0x01, 0x00, 0x08, 0x00, 0x24, 0x00};
static uint8_t vendor_set_csr_pskey_default_transmit_power[] = {0xc2, 0x02, 0x00, 0x09, 0x00, 0x05, 0x00, 0x03, 0x70, 0x00, 0x00, 0x21, 0x00, 0x01, 0x00, 0x08, 0x00, 0x04, 0x00};
static uint8_t vendor_set_csr_pskey_max_transmit_power[] = {0xc2, 0x02, 0x00, 0x09, 0x00, 0x06, 0x00, 0x03, 0x70, 0x00, 0x00, 0x17, 0x00, 0x01, 0x00, 0x08, 0x00, 0x04, 0x00};
static uint8_t vendor_set_csr_pskey_ble_default_transmit_power[] = {0xc2, 0x02, 0x00, 0x09, 0x00, 0x07, 0x00, 0x03, 0x70, 0x00, 0x00, 0xc8, 0x22, 0x01, 0x00, 0x08, 0x00, 0x04, 0x00};
static uint8_t vendor_set_csr_pskey_bd_addr[] = {0xc2, 0x02, 0x00, 0x0c, 0x00, 0x08, 0x00, 0x03, 0x70, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x08, 0x00, 0x20, 0x00, 0x99, 0x1a, 0x86, 0x00, 0x1d, 0x00};
static uint8_t vendor_set_csr_pskey_pcm_config[] = {0xc2, 0x02, 0x00, 0x0a, 0x00, 0x09, 0x00, 0x03, 0x70, 0x00, 0x00, 0xb3, 0x01, 0x02, 0x00, 0x08, 0x00, 0x80, 0x08, 0x80, 0x18};
static uint8_t vendor_set_csr_pskey_pcm_formt[] = {0xc2, 0x02, 0x00, 0x09, 0x00, 0x0a, 0x00, 0x03, 0x70, 0x00, 0x00, 0xb6, 0x01, 0x01, 0x00, 0x08, 0x00, 0x60, 0x00};
static uint8_t vendor_set_csr_pskey_user_low_jitter_mode[] = {0xc2, 0x02, 0x00, 0x09, 0x00, 0x0b, 0x00, 0x03, 0x70, 0x00, 0x00, 0xc9, 0x23, 0x01, 0x00, 0x08, 0x00, 0x01, 0x00};
static uint8_t vendor_set_hci_nop_disable[] = {0xc2, 0x02, 0x00, 0x09, 0x00, 0x0c, 0x00, 0x03, 0x70, 0x00, 0x00, 0xf2, 0x00, 0x01, 0x00, 0x08, 0x00, 0x01, 0x00};
static uint8_t vendor_set_uart_buad_921600[] = {0xc2, 0x02, 0x00, 0x0a, 0x00, 0x0d, 0x00, 0x03, 0x70, 0x00, 0x00, 0xea, 0x01, 0x02, 0x00, 0x08, 0x00,0x0e,0x00,0x00,0x10};
static uint8_t vendor_set_warm_reset[] = {0xc2, 0x02, 0x00, 0x09, 0x00, 0x0e, 0x00, 0x02, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static char *bt_gap_local_name = BT_LOCAL_NAME;
static uint8_t class_of_device[] = {0x08, 0x04, 0x20};
static uint16_t page_timeout = 0x4000; /* value*0.625ms */
static uint8_t event_mask[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F};
static uint8_t ssp_enable = 1;
static uint8_t inquiry_mode = 2;
static uint8_t le_host_support[] = {0x01, 0x00};
static bt_gap_write_eir_t bt_gap_eir;
static bt_gap_scan_mode_t bt_scan_mode = BT_GAP_SCAN_MODE_GENERAL_ACCESSABLE;

extern void bt_build_eir_data(uint8_t *eir_data, uint8_t lenght);

static bt_gap_init_cmd_t bt_gap_init_cmd_table[] = {
	{BT_GAP_CMD_TIMEOUT_RETRY,  {BT_HCI_CMD_RESET, 0, NULL}},
	{BT_GAP_CMD_TIMEOUT_ASSERT, {BT_HCI_CMD_VENDOR_CSR8X11, sizeof(vendor_set_pskey_deep_sleep_state), vendor_set_pskey_deep_sleep_state}},
	{BT_GAP_CMD_TIMEOUT_ASSERT, {BT_HCI_CMD_VENDOR_CSR8X11, sizeof(vendor_set_ana_freq_26mhz), vendor_set_ana_freq_26mhz}},
	{BT_GAP_CMD_TIMEOUT_ASSERT, {BT_HCI_CMD_VENDOR_CSR8X11, sizeof(vendor_set_csr_pskey_ana_ftrim), vendor_set_csr_pskey_ana_ftrim}},
	{BT_GAP_CMD_TIMEOUT_ASSERT, {BT_HCI_CMD_VENDOR_CSR8X11, sizeof(vendor_set_csr_pskey_default_transmit_power), vendor_set_csr_pskey_default_transmit_power}},
	{BT_GAP_CMD_TIMEOUT_ASSERT, {BT_HCI_CMD_VENDOR_CSR8X11, sizeof(vendor_set_csr_pskey_max_transmit_power), vendor_set_csr_pskey_max_transmit_power}},
	{BT_GAP_CMD_TIMEOUT_ASSERT, {BT_HCI_CMD_VENDOR_CSR8X11, sizeof(vendor_set_csr_pskey_ble_default_transmit_power), vendor_set_csr_pskey_ble_default_transmit_power}},
	{BT_GAP_CMD_TIMEOUT_ASSERT, {BT_HCI_CMD_VENDOR_CSR8X11, sizeof(vendor_set_csr_pskey_bd_addr), vendor_set_csr_pskey_bd_addr}},
	{BT_GAP_CMD_TIMEOUT_ASSERT, {BT_HCI_CMD_VENDOR_CSR8X11, sizeof(vendor_set_csr_pskey_pcm_config), vendor_set_csr_pskey_pcm_config}},	
	{BT_GAP_CMD_TIMEOUT_ASSERT, {BT_HCI_CMD_VENDOR_CSR8X11, sizeof(vendor_set_csr_pskey_pcm_formt), vendor_set_csr_pskey_pcm_formt}},
	{BT_GAP_CMD_TIMEOUT_ASSERT, {BT_HCI_CMD_VENDOR_CSR8X11, sizeof(vendor_set_csr_pskey_user_low_jitter_mode), vendor_set_csr_pskey_user_low_jitter_mode}},
	{BT_GAP_CMD_TIMEOUT_ASSERT, {BT_HCI_CMD_VENDOR_CSR8X11, sizeof(vendor_set_hci_nop_disable), vendor_set_hci_nop_disable}},
	{BT_GAP_CMD_TIMEOUT_ASSERT, {BT_HCI_CMD_VENDOR_CSR8X11, sizeof(vendor_set_uart_buad_921600), vendor_set_uart_buad_921600}},
	{BT_GAP_CMD_TIMEOUT_IGNORE, {BT_HCI_CMD_VENDOR_CSR8X11, sizeof(vendor_set_warm_reset), vendor_set_warm_reset}},
	{BT_GAP_CMD_TIMEOUT_RETRY,  {BT_HCI_CMD_RESET, 0, NULL}},	
	{BT_GAP_CMD_TIMEOUT_ASSERT, {BT_HCI_CMD_READ_BUFFER_SIZE, 0, NULL}},
	{BT_GAP_CMD_TIMEOUT_ASSERT, {BT_HCI_CMD_READ_LOCAL_VERSION_INFO, 0, NULL}},
	{BT_GAP_CMD_TIMEOUT_ASSERT, {BT_HCI_CMD_READ_BD_ADDR, 0, NULL}},
	{BT_GAP_CMD_TIMEOUT_ASSERT, {BT_HCI_CMD_CHANGE_LOCAL_NAME, 0, NULL}},
	{BT_GAP_CMD_TIMEOUT_ASSERT, {BT_HCI_CMD_WRITE_CLASS_OF_DEVICE, sizeof(class_of_device), class_of_device}},
	{BT_GAP_CMD_TIMEOUT_ASSERT, {BT_HCI_CMD_WRITE_PAGE_TIMEOUT, sizeof(page_timeout), &page_timeout}},
	{BT_GAP_CMD_TIMEOUT_ASSERT, {BT_HCI_CMD_SET_EVENT_MASK, sizeof(event_mask), event_mask}},
	{BT_GAP_CMD_TIMEOUT_ASSERT, {BT_HCI_CMD_WRITE_SIMPLE_PAIRING_MODE, sizeof(ssp_enable), &ssp_enable}},
	{BT_GAP_CMD_TIMEOUT_ASSERT, {BT_HCI_CMD_WRITE_INQUIRY_MODE, sizeof(inquiry_mode), &inquiry_mode}},
	{BT_GAP_CMD_TIMEOUT_ASSERT, {BT_HCI_CMD_WRITE_LE_SUPPORT, sizeof(le_host_support), le_host_support}},
	{BT_GAP_CMD_TIMEOUT_ASSERT, {BT_HCI_CMD_WRITE_EXT_INQ_RESPONSE, 0, NULL}},
	{BT_GAP_CMD_TIMEOUT_ASSERT, {BT_HCI_CMD_WRITE_SCAN_ENABLE, sizeof(bt_scan_mode), &bt_scan_mode}},
};

static bt_gap_init_cmd_t *bt_gap_init_cmd_pop()
{
    bt_gap_init_cmd_t *init_cmd = NULL;
	if (bt_gap_init_table_index == sizeof(bt_gap_init_cmd_table)/sizeof(bt_gap_init_cmd_t)) {
		return NULL;
	}
	init_cmd = &bt_gap_init_cmd_table[bt_gap_init_table_index++];
	switch (init_cmd->hci_cmd.cmd_code) {
		case BT_HCI_CMD_CHANGE_LOCAL_NAME:
			init_cmd->hci_cmd.length = strlen(bt_gap_local_name);
			init_cmd->hci_cmd.param = (void *)bt_gap_local_name;
			break;
		case BT_HCI_CMD_WRITE_EXT_INQ_RESPONSE:
			bt_gap_eir.fec_required = 1;
			bt_build_eir_data(bt_gap_eir.eir, sizeof(bt_gap_eir.eir));
			init_cmd->hci_cmd.length = sizeof(bt_gap_eir);
			init_cmd->hci_cmd.param = (void *)&bt_gap_eir;
			break;
		default:
			break;
	}
	return init_cmd;
}

bt_status_t bt_gap_init_process(bool is_timeout, uint32_t timer_id, uint32_t data, const void *param)
{
	bt_status_t status = BT_STATUS_SUCCESS;
	bt_gap_init_cmd_t *init_cmd = NULL;

	if (is_timeout) {
		init_cmd = (bt_gap_init_cmd_t *)data;
		BT_GAP_LOG_ERROR("[BT_GAP_COMMON] init cmd timeout, timer_id = 0x%x, init_table_index = %d\r\n,", timer_id, bt_gap_init_table_index);
		if (init_cmd->attribute & BT_GAP_CMD_TIMEOUT_ASSERT) {
			BT_ASSERT(0);
		} else if (init_cmd->attribute & BT_GAP_CMD_TIMEOUT_RETRY) {
			return bt_hci_cmd_send(init_cmd->hci_cmd, (uint32_t)init_cmd, 100, bt_gap_init_process);
		}
	}
	switch (timer_id) {
		case BT_HCI_TIMER_ID_TYPE_A(BT_HCI_CMD_READ_BD_ADDR):
			bt_memcpy(&blue_angel.local_public_addr, &BT_HCI_GET_EVT_PARAM(param, bt_hci_command_complete_t)->data, sizeof(bt_bd_addr_t));
			break;
		case BT_HCI_TIMER_ID_TYPE_A(BT_HCI_CMD_READ_BUFFER_SIZE):
			break;
		default:
			break;
	}
		
	init_cmd = bt_gap_init_cmd_pop();
	if (init_cmd != NULL) {
		status = bt_hci_cmd_send(init_cmd->hci_cmd, (uint32_t)init_cmd, 2000, bt_gap_init_process);
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
