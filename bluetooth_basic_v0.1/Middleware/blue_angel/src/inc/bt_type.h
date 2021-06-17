/******************************************************************************
  * @file           bt_type.h
  * @author         leon
  * @version        V0.1
  * @date           2021-05-16
  * @brief          bt type define for internal use
******************************************************************************/
#ifndef __BT_TYPE_H__
#define __BT_TYPE_H__
#include "bt_platform.h"

#define BT_MODULE_OFFSET			27

#define BT_MODULE_GENERAL			(0x00 << BT_MODULE_OFFSET)
#define BT_MODULE_MEMORY			(0x01 << BT_MODULE_OFFSET)
#define BT_MODULE_TIMER     		(0x02 << BT_MODULE_OFFSET)
#define BT_MODULE_DRIVER    		(0x03 << BT_MODULE_OFFSET)
#define BT_MODULE_HCI				(0x04 << BT_MODULE_OFFSET)
#define BT_MODULE_GAP				(0x05 << BT_MODULE_OFFSET)
#define BT_MODULE_L2CAP				(0x06 << BT_MODULE_OFFSET)
#define BT_MODULE_ATT				(0x07 << BT_MODULE_OFFSET)
#define BT_MODULE_GATT				(0x08 << BT_MODULE_OFFSET)
#define BT_MODULE_SM				(0x09 << BT_MODULE_OFFSET)
#define BT_MODULE_SYSTEM			(0x0A << BT_MODULE_OFFSET)
#define BT_MODULE_CUSTOM			(0x1E << BT_MODULE_OFFSET)
#define BT_MODULE_ERROR				(0x1F << BT_MODULE_OFFSET)

#define BT_STATUS_SUCCESS			(BT_MODULE_GENERAL | (1 << 0))
#define BT_STATUS_FAIL				(BT_MODULE_GENERAL | (1 << 1))
#define BT_STATUS_PENDING			(BT_MODULE_GENERAL | (1 << 2))
#define BT_STATUS_BUSY				(BT_MODULE_GENERAL | (1 << 3))
#define BT_STATUS_TIMEOUT			(BT_MODULE_GENERAL | (1 << 4))
#define BT_STATUS_OUT_OF_MEMORY 	(BT_MODULE_GENERAL | (1 << 5))

#define BT_STATUS_TIMER_NOT_FOUND   (BT_MODULE_TIMER   | (1 << 0))
#define BT_STATUS_TIMER_EXIST		(BT_MODULE_TIMER   | (1 << 1))
typedef uint32_t bt_status_t;

typedef uint32_t bt_msg_type_t;

#define BT_PACKED __attribute__ ((packed))

#define BD_ADDR_LENGTH	6

typedef struct {
	uint8_t addr[BD_ADDR_LENGTH];
} BT_PACKED bt_bd_addr_t;



#endif //__BT_TYPE_H__


