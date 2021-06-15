/******************************************************************************
  * @file           bt_init.c
  * @author         leon
  * @version        V0.1
  * @date           2021-05-19
  * @brief
******************************************************************************/
#include "bt_system.h"
#include "bt_init.h"

__align(4) static uint8_t bt_timer_fixed_memory[BT_TIMER_BLOCK_SIZE * 10] = {0};
__align(4) static uint8_t bt_tx_buf[1024 *4] = {0};
__align(4) static uint8_t bt_rx_buf[1024 *4] = {0};

void bt_init()
{
	bt_memory_init(BT_MEMORY_TX, bt_tx_buf, sizeof(bt_tx_buf));
	bt_memory_init(BT_MEMORY_RX, bt_rx_buf, sizeof(bt_rx_buf));
	
	bt_fixed_memory_init(BT_FIXED_MM_TIMER, bt_timer_fixed_memory, sizeof(bt_timer_fixed_memory));
}




