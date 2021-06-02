/******************************************************************************
  * @file           bt_hci.h
  * @author         Leonard-y.He(1027901556@qq.com)
  * @version        V0.1
  * @date           2021-05-19
  * @brief
******************************************************************************/
#ifndef __BT_HCI_H__
#define __BT_HCI_H__
#include "bt_type.h"
#include "bt_hci_spec.h"
#include "bt_timer.h"

#define BT_HCI_LOG_INFO(format, ...)              \
{									            \
	if (BT_HCI_LOG_LEVEL >= BT_SYS_LOG_INFO) \
		BT_SYSTEM_LOG(format,##__VA_ARGS__);                  \
}											    \

#define BT_HCI_LOG_DEBUG(format, ...)              \
{									            \
	if (BT_HCI_LOG_LEVEL >= BT_SYS_LOG_DEBUG)\
		BT_SYSTEM_LOG(format,##__VA_ARGS__);                  \
}	

#define BT_HCI_LOG_WARNING(format, ...)              \
{									            \
	if (BT_HCI_LOG_LEVEL >= BT_SYS_LOG_WARNING) \
		BT_SYSTEM_LOG(format,##__VA_ARGS__);                  \
}	

#define BT_HCI_LOG_ERROR(format, ...)              \
{									            \
	if (BT_HCI_LOG_LEVEL >= BT_SYS_LOG_ERROR)\
		BT_SYSTEM_LOG(format,##__VA_ARGS__);                  \
}

#define BT_HCI_TIMER_MASK_A	BT_MODULE_HCI
#define BT_HCI_TIMEOUT_LENGTH	500

typedef struct {
	bt_hci_cmd_code_t cmd_code;
	uint8_t length;
	void *param;
} bt_hci_cmd_t;

typedef bt_timer_timeout_callback_t bt_hci_timeout_callback_t;

bt_status_t bt_hci_cmd_send(bt_hci_cmd_t cmd, uint32_t data, bt_hci_timeout_callback_t callback);

void bt_hci_packet_process(void);

#endif//__BT_HCI_H__
