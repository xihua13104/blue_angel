/******************************************************************************
  * @file           bt_hci.h
  * @author         leon
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

#define BT_HCI_CMD_TIMEOUT_SPECIAL	100
#define BT_HCI_CMD_TIMEOUT			1000	
	
#define BT_HCI_TIMER_MASK_A		0x00000000	/*host主动发hci cmd而产生的event*/
#define BT_HCI_TIMER_MASK_B		0x00010000	/*controller主动上报的event，不是由host发送hci            cmd产生的evt */

#define BT_HCI_TIMER_ID_TYPE_A(cmd_code) (cmd_code | BT_MODULE_HCI | BT_HCI_TIMER_MASK_A)
#define BT_HCI_TIMER_ID_TYPE_B(evt_code) (evt_code | BT_MODULE_HCI | BT_HCI_TIMER_MASK_B)
	
typedef struct {
    bt_hci_cmd_code_t cmd_code;
    uint8_t length;
    void *param;
} bt_hci_cmd_t;

typedef struct {
	uint8_t cmd_credit;
	uint16_t acl_credit;
	uint16_t acl_length;
	bool is_cmd_flow_controled;
	bool is_acl_flow_controled;
} bt_hci_flow_control_t;
	
typedef bt_timer_timeout_callback_t bt_hci_timeout_callback_t;

bt_status_t bt_hci_cmd_send(bt_hci_cmd_t cmd, uint32_t data, uint32_t timeout, bt_hci_timeout_callback_t callback);

void bt_hci_packet_process(void);

#endif//__BT_HCI_H__
