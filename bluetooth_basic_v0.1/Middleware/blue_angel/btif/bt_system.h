/******************************************************************************
  * @file           bt_system.h
  * @author         leon
  * @version        V0.1
  * @date           2021-05-17
  * @brief
******************************************************************************/
#ifndef __BT_SYSTEM__
#define __BT_SYSTEM__
#include "stdint.h"

typedef enum {
    BT_MEMORY_TX = 0,
    BT_MEMORY_RX = 1
} bt_memory_type_t;

typedef enum {
    BT_FIXED_MM_TIMER = 0,
    BT_FIXED_MM_MAX
} bt_fixed_memory_type_t;

#define BT_TIMER_BLOCK_SIZE (28)

void bt_memory_init(bt_memory_type_t type, uint8_t *buf, uint32_t size);

void bt_fixed_memory_init(bt_fixed_memory_type_t type, uint8_t *buf, uint32_t size);
#endif//__BT_SYSTEM__
