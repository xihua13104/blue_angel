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

/*System log config*/
#define BT_TIMER_LOG_LEVEL	BT_SYS_LOG_INFO
#define BT_MEMORY_LOG_LEVEL	BT_SYS_LOG_INFO
#define BT_DRIVER_LOG_LEVEL	BT_SYS_LOG_INFO
#define BT_HCI_LOG_LEVEL	BT_SYS_LOG_INFO
#define BT_L2CAP_LOG_LEVEL	BT_SYS_LOG_INFO
#define BT_ATT_LOG_LEVEL	BT_SYS_LOG_INFO
#define BT_GATT_LOG_LEVEL	BT_SYS_LOG_INFO

/*HCI LOG Config*/
#define BT_HCI_LOG_MIX_SYSTEM_LOG 0

#endif//__BT_CONFIG__
