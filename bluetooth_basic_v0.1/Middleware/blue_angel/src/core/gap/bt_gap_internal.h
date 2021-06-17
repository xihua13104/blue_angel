/******************************************************************************
  * @file           bt_gap_internal.h
  * @author         leon
  * @version        V0.1
  * @date           2021-05-19
  * @brief
******************************************************************************/
#ifndef __BT_GAP_INTERNAL_H__
#define __BT_GAP_INTERNAL_H__
#include "bt_type.h"
#include "bt_hci_spec.h"
#include "bt_timer.h"

#define BT_GAP_LOG_INFO(format, ...)              \
    {									            \
        if (BT_GAP_LOG_LEVEL >= BT_SYS_LOG_INFO) \
            BT_SYSTEM_LOG(format,##__VA_ARGS__);                  \
    }											    \

#define BT_GAP_LOG_DEBUG(format, ...)              \
    {									            \
        if (BT_GAP_LOG_LEVEL >= BT_SYS_LOG_DEBUG)\
            BT_SYSTEM_LOG(format,##__VA_ARGS__);                  \
    }

#define BT_GAP_LOG_WARNING(format, ...)              \
    {									            \
        if (BT_GAP_LOG_LEVEL >= BT_SYS_LOG_WARNING) \
            BT_SYSTEM_LOG(format,##__VA_ARGS__);                  \
    }

#define BT_GAP_LOG_ERROR(format, ...)              \
    {									            \
        if (BT_GAP_LOG_LEVEL >= BT_SYS_LOG_ERROR)\
            BT_SYSTEM_LOG(format,##__VA_ARGS__);                  \
    }


#define BT_GAP_INVALID_TIMER_ID	(BT_MODULE_GAP|0xFF)

#define BT_POWER_OFF			0
#define BT_POWER_ON				1
#define BT_POWER_SWITCHING_ON	2
#define BT_POWER_SWITCHING_OFF  3
typedef uint8_t bt_power_status_t;

typedef struct {
	bt_power_status_t power_status;
	bt_bd_addr_t local_public_addr;
	bt_bd_addr_t random_addr;
	struct {
		uint8_t cmd_credit;
		uint16_t acl_credit;
		uint16_t acl_length;
	} bt_hci_flow_control;
} bt_blue_angel_t;

#endif//__BT_GAP_INTERNAL_H__
