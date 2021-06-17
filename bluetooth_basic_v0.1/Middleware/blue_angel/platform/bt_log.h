/******************************************************************************
  * @file           bt_log.h
  * @author         leon
  * @version        V0.1
  * @date           2021-05-19
  * @brief
******************************************************************************/
#ifndef __BT_LOG_H__
#define __BT_LOG_H__
#include "bt_type.h"

#define BT_SYSTEM_LOG printf

typedef enum {
    BT_SYS_LOG_OFF,
    BT_SYS_LOG_ERROR,
    BT_SYS_LOG_WARNING,
    BT_SYS_LOG_DEBUG,
    BT_SYS_LOG_INFO,
} bt_system_log_level_t;

typedef enum {
    BT_HCI_LOG_CMD     = 0x01,
    BT_HCI_LOG_ACL_OUT = 0x02,
    BT_HCI_LOG_ACL_IN  = 0x04,
    BT_HCI_LOG_EVT     = 0x08
} bt_hci_log_type_t;

#define BT_HCI_LOG_HEADER_LEGNTH 5 //header + direction + sizeof(log_length)

void bt_hci_log(uint8_t in_out, uint8_t *log, uint16_t log_length);
#endif //__BT_LOG_H__
