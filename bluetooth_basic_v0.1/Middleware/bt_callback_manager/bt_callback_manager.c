/******************************************************************************
  * @file           bt_callback_manager.c
  * @author         leon
  * @version        V0.1
  * @date           2021-05-16
  * @brief
******************************************************************************/
#include "bt_callback_manager.h"

typedef struct {
	bool in_use;
	uint32_t mask;
	void *callback;
} bt_callback_manager_block_t;

static bt_callback_manager_block_t callback_table[BT_CALLBACK_TYPE_MAX] = {0};

bt_status_t bt_callback_manager_register_callback(bt_callback_type_t type, bt_callback_module_mask_t module_mask, void *callback)
{
	bt_status_t status = BT_STATUS_FAIL;
	uint32_t i = 0;
	switch type {
		case BT_CALLBACK_TYPE_APP_EVENT£º
			for (i = 0; i < BT_CALLBACK_TYPE_MAX; i++) {
				if (!callback_table[i].in_use && callback_table[i].callback == NULL && callback_table[i].mask == 0 && callback) {
					callback_table.in_use = true;
					callback_table.mask = module_mask;
					callback_table.callback = callback;
					status = BT_STATUS_SUCCESS;
					break;
				}
			}
			break;
		default:
			break;
	}

	return status;
}

bt_status_t bt_callback_manager_deregister_callback(bt_callback_type_t type, bt_callback_module_mask_t module_mask, void *callback)
{
	bt_status_t status = BT_STATUS_FAIL;
	uint32_t i = 0;
	switch type {
		case BT_CALLBACK_TYPE_APP_EVENT£º
			for (i = 0; i < BT_CALLBACK_TYPE_MAX; i++) {
				if (callback_table[i].in_use && callback_table[i].callback && callback) {
					callback_table.in_use = false;
					callback_table.mask = 0;
					callback_table.callback = NULL;
					status = BT_STATUS_SUCCESS;
					break;
				}
			}
			break;
		default:
			break;
	}

	return status;

}

bt_status_t bt_app_event_callback(bt_msg_type_t msg, bt_status_t status, void *buf)
{
	bt_status_t status = BT_STATUS_FAIL;
	uint32_t i = 0;	
	uint32_t module_mask = BT_MODULE_MASK(msg);
	switch type {
		case BT_CALLBACK_TYPE_APP_EVENT£º
			for (i = 0; i < BT_CALLBACK_TYPE_MAX; i++) {
				if (callback_table[i].in_use && callback_table[i].callback && callback_table[i].mask & module_mask) {
					status = callback_table.callback(msg, status, buf)
					break;
				}
			}
			break;
		default:
			break;
	}

	return status;

}

