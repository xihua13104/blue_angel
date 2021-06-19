/******************************************************************************
  * @file           bt_init.c
  * @author         leon
  * @version        V0.1
  * @date           2021-05-19
  * @brief
******************************************************************************/
#include "bt_system.h"
#include "bt_init.h"
#include "bt_gap_classic.h"
#include "bt_config.h"

__align(4) static uint8_t bt_timer_fixed_memory[BT_TIMER_BLOCK_SIZE * 10] = {0};
__align(4) static uint8_t bt_tx_buf[1024 *4] = {0};
__align(4) static uint8_t bt_rx_buf[1024 *4] = {0};

void bt_init()
{
	bt_memory_init(BT_MEMORY_TX, bt_tx_buf, sizeof(bt_tx_buf));
	bt_memory_init(BT_MEMORY_RX, bt_rx_buf, sizeof(bt_rx_buf));
	
	bt_fixed_memory_init(BT_FIXED_MM_TIMER, bt_timer_fixed_memory, sizeof(bt_timer_fixed_memory));
}

void bt_build_eir_data(uint8_t *eir_data, uint8_t lenght)
{
    uint8_t data_pos = 0;
    uint8_t len = 0;
#if 1
    /* local name */
    len = strlen(BT_LOCAL_NAME);
    eir_data[data_pos++] = len + 1;
    eir_data[data_pos++] = BT_DT_COMPLETE_LOCAL_NAME;
    memcpy(eir_data+data_pos,BT_LOCAL_NAME,strlen(BT_LOCAL_NAME));
    data_pos += strlen(BT_LOCAL_NAME);
#endif
    /* 16 bit UUID */
    len = 1;
#if PROFILE_DID_ENABLE
    len += 2;
#endif
#if PROFILE_HFP_ENABLE
    len += 2;
#endif
#if PROFILE_SPP_ENABLE
    len += 2;
#endif
#if PROFILE_A2DP_ENABLE
    len += 2;
#endif
#if PROFILE_AVRCP_ENABLE
    len += 2;
#endif
#if PROFILE_HID_ENABLE
	len += 2;
#endif
    eir_data[data_pos++] = len;
    eir_data[data_pos++] = BT_DT_COMPLETE_LIST_OF_16_BIT_SERVICE_CLASS_UUIDS;
#if PROFILE_DID_ENABLE
    eir_data[data_pos++] = BT_SERVICE_CLASS_PNP_INFORMATION & 0xff;
    eir_data[data_pos++] = (BT_SERVICE_CLASS_PNP_INFORMATION>>8) & 0xff;
#endif
#if PROFILE_HFP_ENABLE
    eir_data[data_pos++] = BT_SERVICE_CLASS_HANDSFREE & 0xff;
    eir_data[data_pos++] = (BT_SERVICE_CLASS_HANDSFREE>>8) & 0xff;
#endif
#if PROFILE_SPP_ENABLE
    eir_data[data_pos++] = BT_SERVICE_CLASS_SERIAL_PORT & 0xff;
    eir_data[data_pos++] = (BT_SERVICE_CLASS_SERIAL_PORT>>8) & 0xff;
#endif
#if PROFILE_A2DP_ENABLE
    eir_data[data_pos++] = BT_SERVICE_CLASS_AUDIO_SINK & 0xff;
    eir_data[data_pos++] = (BT_SERVICE_CLASS_AUDIO_SINK>>8) & 0xff;
#endif
#if PROFILE_AVRCP_ENABLE
    eir_data[data_pos++] = BT_SERVICE_CLASS_AV_REMOTE_CONTROL & 0xff;
    eir_data[data_pos++] = (BT_SERVICE_CLASS_AV_REMOTE_CONTROL>>8) & 0xff;
#endif
#if PROFILE_HID_ENABLE
	eir_data[data_pos++] = BT_SERVICE_CLASS_HUMAN_INTERFACE_DEVICE_SERVICE & 0xff;
	eir_data[data_pos++] = (BT_SERVICE_CLASS_HUMAN_INTERFACE_DEVICE_SERVICE>>8) & 0xff;
#endif
    /* Device ID */
#if PROFILE_DID_ENABLE
    eir_data[data_pos++] = 9;
    eir_data[data_pos++] = BT_DT_DEVICE_ID;
    eir_data[data_pos++] = DID_VENDOR_ID_SOURCE_VALUE & 0xff;
    eir_data[data_pos++] = (DID_VENDOR_ID_SOURCE_VALUE>>8) & 0xff;
    eir_data[data_pos++] = DID_VENDOR_ID_VALUE & 0xff;
    eir_data[data_pos++] = (DID_VENDOR_ID_VALUE>>8) & 0xff;
    eir_data[data_pos++] = DID_PRODUCT_ID_VALUE & 0xff;
    eir_data[data_pos++] = (DID_PRODUCT_ID_VALUE>>8) & 0xff;
    eir_data[data_pos++] = DID_VERSION_ID_VALUE & 0xff;
    eir_data[data_pos++] = (DID_VERSION_ID_VALUE>>8) & 0xff;
#endif
}


