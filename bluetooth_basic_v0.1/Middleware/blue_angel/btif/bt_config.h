/******************************************************************************
  * @file           bt_config.h
  * @author         leon
  * @version        V0.1
  * @date           2021-05-17
  * @brief
******************************************************************************/
#ifndef __BT_CONFIG__
#define __BT_CONFIG__

#define BT_BR_BLE_COMBINATION	    0

#if BT_BR_BLE_COMBINATION
	#define BT_BLE_ONLY				0
#else
	#define BT_BLE_ONLY				1
#endif

#define PROFILE_DID_ENABLE 			1
#define PROFILE_HFP_ENABLE 			1
#define PROFILE_SPP_ENABLE 			1
#define PROFILE_PBAP_ENABLE 		0
#define PROFILE_A2DP_ENABLE 		1
#define PROFILE_AVRCP_ENABLE 		1
#define PROFILE_HID_ENABLE 			0

/*System log config*/
#define BT_TIMER_LOG_LEVEL	BT_SYS_LOG_INFO
#define BT_MEMORY_LOG_LEVEL	BT_SYS_LOG_INFO
#define BT_DRIVER_LOG_LEVEL	BT_SYS_LOG_INFO
#define BT_HCI_LOG_LEVEL	BT_SYS_LOG_INFO
#define BT_GAP_LOG_LEVEL	BT_SYS_LOG_INFO
#define BT_L2CAP_LOG_LEVEL	BT_SYS_LOG_INFO
#define BT_ATT_LOG_LEVEL	BT_SYS_LOG_INFO
#define BT_GATT_LOG_LEVEL	BT_SYS_LOG_INFO

/*HCI LOG Config*/
#define BT_HCI_LOG_ENABLE		  1
#define BT_HCI_LOG_MIX_SYSTEM_LOG 0

/*BT CUNIT*/
#define BT_CUNIT_ENABLE			  1

/**/
#define BT_LOCAL_NAME	"leonard's bt demo"


/***********   DID profile config ********************************************************************************************/
#define DID_VENDOR_ID_VALUE 0xffff
#define DID_PRODUCT_ID_VALUE 0xffff
#define DID_VERSION_ID_VALUE 0x0100
#define DID_PRIMARY_RECORD_VALUE 0x01

#define DID_VENDOR_ID_SOURCE_BLUETOOTH 0x0001
#define DIDS_VENDOR_ID_SOURCE_USB       0x0002
#define DID_VENDOR_ID_SOURCE_VALUE DID_VENDOR_ID_SOURCE_BLUETOOTH
/******************************************************************************************************************************/
#endif//__BT_CONFIG__
