/******************************************************************************
  * @file           bt_callback_manager.h
  * @author         leon
  * @version        V0.1
  * @date           2021-05-16
  * @brief
******************************************************************************/

#ifndef __BT_CALLBACK_MANAGER_H__
#define __BT_CALLBACK_MANAGER_H__
#include "bt_type.h"

#define BT_CALLBACK_TYPE_APP_EVENT	0
#define BT_CALLBACK_TYPE_MAX		31
typedef uint32_t bt_callback_type_t;

#define BT_MODULE_MASK(module)		(1 << (module >> BT_MODULE_OFFSET))
#define MODULE_MASK_GENERAL BT_MODULE_MASK(BT_MODULE_GENERAL)
#define MODULE_MASK_MEMORY  BT_MODULE_MASK(BT_MODULE_MEMORY)
#define MODULE_MASK_TIMER   BT_MODULE_MASK(BT_MODULE_TIMER)
#define MODULE_MASK_DRIVER  BT_MODULE_MASK(BT_MODULE_DRIVER)
#define MODULE_MASK_HCI     BT_MODULE_MASK(BT_MODULE_HCI)
#define MODULE_MASK_GAP     BT_MODULE_MASK(BT_MODULE_GAP)
#define MODULE_MASK_L2CAP   BT_MODULE_MASK(BT_MODULE_L2CAP)
#define MODULE_MASK_ATT     BT_MODULE_MASK(BT_MODULE_ATT)
#define MODULE_MASK_GATT    BT_MODULE_MASK(BT_MODULE_GATT)
#define MODULE_MASK_SM      BT_MODULE_MASK(BT_MODULE_SM)
#define MODULE_MASK_SYSTEM  BT_MODULE_MASK(BT_MODULE_SYSTEM)
#define MODULE_MASK_CUSTOM  BT_MODULE_MASK(BT_MODULE_CUSTOM)
#define MODULE_MASK_ERROR   BT_MODULE_MASK(BT_MODULE_ERROR)
typedef uint32_t bt_callback_module_mask_t;

typedef bt_status_t (*bt_callback_manager_cb_t)(bt_msg_type_t msg, bt_status_t status, void *buf);

bt_status_t bt_callback_manager_register_callback(bt_callback_type_t type, bt_callback_module_mask_t module_mask, bt_callback_manager_cb_t callback);

bt_status_t bt_callback_manager_deregister_callback(bt_callback_type_t type, bt_callback_module_mask_t module_mask, bt_callback_manager_cb_t callback);

void bt_app_event_callback(bt_msg_type_t msg, bt_status_t status, void *buf);
#endif //__BT_CALLBACK_MANAGER_H__


