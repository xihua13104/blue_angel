/******************************************************************************
  * @file           bt_gap_classic.h
  * @author         leon
  * @version        V0.1
  * @date           2021-05-19
  * @brief
******************************************************************************/
#ifndef __BT_CLASSIC_H__
#define __BT_CLASSIC_H__
#include "bt_type.h"
#include "bt_hci_spec.h"
#include "bt_timer.h"


#define BT_GAP_SCAN_MODE_NOT_ACCESSIABLE 	0
#define BT_GAP_SCAN_MODE_DISCOVERABLE_ONLY	1
#define BT_GAP_SCAN_MODE_CONNECTABLE_ONLY	2
#define BT_GAP_SCAN_MODE_GENERAL_ACCESSABLE	3
typedef uint8_t bt_gap_scan_mode_t;

#endif//__BT_CLASSIC_H__
