/******************************************************************************
  * @file           bt_test.c
  * @author         leon
  * @version        V0.1
  * @date           2021-05-19
  * @brief
******************************************************************************/
#include "bt_test.h"
#include "bt_os_layer_api.h"
#include "cunit_interface.h"
#include "bt_linknode.h"
#include "bt_memory.h"
#include "bt_timer.h"
#include "bt_hci.h"

#define BT_TIMER_BLOCK_SIZE (MEMORY_ALIGN_SIZE(sizeof(bt_timer_t)) + BT_MM_HEADER_SIZE + BT_MM_FOOTER_SIZE)
__align(4) static uint8_t bt_timer_fixed_memory[BT_TIMER_BLOCK_SIZE * 5] = {0};
__align(4) static uint8_t bt_tx_buf[1024 *4] = {0};
__align(4) static uint8_t bt_rx_buf[1024 *4] = {0};

void bt_linknode_test(void) 
{ 
	bt_linknode_t head;
	uint32_t timer_id = 0xFFFF;
	bt_timer_t timer = {
		NULL,
		0xFFFF,
		0x1234,
		0x100,
		NULL
		};
	bt_timer_t timer1 = {
		NULL,
		0xFFFE,
		0x1233,
		0x101,
		NULL
		};
	bt_linknode_t *tmp_node = NULL;
	bt_timer_t *tmp_timer = NULL;
	bt_linknode_insert_node(&head, (bt_linknode_t *)&timer, BT_NODE_FRONT);
	bt_linknode_insert_node(&head, (bt_linknode_t *)&timer1, BT_NODE_FRONT);	
	/*tmp_node = bt_linknode_delete_node(&head, BT_NODE_FRONT);
	CU_ASSERT(tmp_node == (bt_linknode_t *)&timer); 
	tmp_timer = (bt_timer_t *)(tmp_node);
	CU_ASSERT(tmp_timer->next == NULL);
	CU_ASSERT(tmp_timer->timer_id == 0xFFFF);
	CU_ASSERT(tmp_timer->data == 0x1234);
	CU_ASSERT(tmp_timer->time_ms == 0x100);
	CU_ASSERT(tmp_timer->timeout_cb == NULL);
	CU_ASSERT(head.next == NULL);*/
	tmp_node = bt_linknode_travel_node(&head, bt_timer_cmp_by_timerid, &timer_id);
	tmp_timer = (bt_timer_t *)(tmp_node->next);
	CU_ASSERT(tmp_timer == &timer);
	CU_ASSERT(tmp_timer->timer_id == 0xFFFF);
	CU_ASSERT(tmp_timer->data == 0x1234);
	CU_ASSERT(tmp_timer->time_ms == 0x100);
	CU_ASSERT(tmp_timer->timeout_cb == NULL);

	timer_id = 0xFFFE;
	tmp_node = bt_linknode_travel_node(&head, bt_timer_cmp_by_timerid, &timer_id);
	tmp_timer = (bt_timer_t *)(tmp_node->next);
	CU_ASSERT(tmp_timer == &timer1);
	CU_ASSERT(tmp_timer->timer_id == 0xFFFE);
	CU_ASSERT(tmp_timer->data == 0x1233);
	CU_ASSERT(tmp_timer->time_ms == 0x101);
	CU_ASSERT(tmp_timer->timeout_cb == NULL);
}

void bt_memory_test(void) 
{ 
	bt_timer_t *timer;
	uint8_t *tx_buf, *rx_buf;
	bt_fixed_memory_init(BT_FIXED_MM_TIMER, bt_timer_fixed_memory, sizeof(bt_timer_fixed_memory));
	timer = (bt_timer_t *)bt_fixed_memory_allocate(BT_FIXED_MM_TIMER); 
	CU_ASSERT(timer != NULL);
	bt_fixed_memory_free(BT_FIXED_MM_TIMER, (uint8_t *)timer);
	
	timer = (bt_timer_t *)bt_fixed_memory_allocate(BT_FIXED_MM_TIMER); 
	CU_ASSERT(timer != NULL);
	bt_fixed_memory_free(BT_FIXED_MM_TIMER, (uint8_t *)timer);
	
	timer = (bt_timer_t *)bt_fixed_memory_allocate(BT_FIXED_MM_TIMER); 
	CU_ASSERT(timer != NULL);
	bt_fixed_memory_free(BT_FIXED_MM_TIMER, (uint8_t *)timer);	

	timer = (bt_timer_t *)bt_fixed_memory_allocate(BT_FIXED_MM_TIMER); 
	CU_ASSERT(timer != NULL);
	bt_fixed_memory_free(BT_FIXED_MM_TIMER, (uint8_t *)timer);	

	timer = (bt_timer_t *)bt_fixed_memory_allocate(BT_FIXED_MM_TIMER); 
	CU_ASSERT(timer != NULL);
	bt_fixed_memory_free(BT_FIXED_MM_TIMER, (uint8_t *)timer);

	timer = (bt_timer_t *)bt_fixed_memory_allocate(BT_FIXED_MM_TIMER); 
	CU_ASSERT(timer != NULL);
	bt_fixed_memory_free(BT_FIXED_MM_TIMER, (uint8_t *)timer);
	//bt_fixed_memory_free(BT_FIXED_MM_TIMER, (uint8_t *)timer);
	
	bt_memory_init(BT_MEMORY_TX, bt_tx_buf, sizeof(bt_tx_buf));
	bt_memory_init(BT_MEMORY_RX, bt_rx_buf, sizeof(bt_rx_buf));
	
	tx_buf = bt_memory_allocate_packet(BT_MEMORY_TX, 8);
	rx_buf = bt_memory_allocate_packet(BT_MEMORY_RX, 16);
	bt_memory_free_packet(BT_MEMORY_TX, tx_buf);
	bt_memory_free_packet(BT_MEMORY_RX, rx_buf);
	//bt_memory_free_packet(BT_MEMORY_TX, tx_buf);
	//bt_memory_free_packet(BT_MEMORY_RX, rx_buf);
}

uint32_t start_tick, end_tick;
//typedef bt_status_t (*bt_timer_timeout_callback_t)(bool is_timeout, uint32_t timer_id, uint32_t data, const void *param);
bt_status_t timeout(bool is_timeout, uint32_t timer_id, uint32_t data, const void *param)
{
	CU_ASSERT(is_timeout);
	CU_ASSERT(timer_id == (BT_MODULE_GAP | 0x0C03));
	end_tick = bt_os_layer_get_system_tick();
	printf("time_length0 = %d, is_timeout = %d\r\n", end_tick-start_tick, is_timeout);
	//bt_timer_stop_all_timer();
	//CU_ASSERT(BT_STATUS_SUCCESS == bt_timer_cancel_and_callback(timer_id+1, NULL));
	//CU_ASSERT(BT_STATUS_SUCCESS == bt_timer_cancel_and_callback(timer_id+2, NULL));
	//bt_timer_stop_all_timer();
	return BT_STATUS_SUCCESS;
}

bt_status_t timeout1(bool is_timeout, uint32_t timer_id, uint32_t data, const void *param)
{
	CU_ASSERT(is_timeout);
	CU_ASSERT(timer_id == ((BT_MODULE_GAP | 0x0C03) + 1));
	end_tick = bt_os_layer_get_system_tick();
	printf("time_length1 = %d, is_timeout = %d\r\n", end_tick-start_tick, is_timeout);
	return BT_STATUS_SUCCESS;
}

bt_status_t timeout2(bool is_timeout, uint32_t timer_id, uint32_t data, const void *param)
{
	CU_ASSERT(is_timeout);
	CU_ASSERT(timer_id == ((BT_MODULE_GAP | 0x0C03) + 2));
	end_tick = bt_os_layer_get_system_tick();
	printf("time_length2 = %d, is_timeout = %d\r\n", end_tick-start_tick, is_timeout);
	return BT_STATUS_SUCCESS;
}

void bt_timer_test(void) 
{ 
	uint32_t timer_id = (BT_MODULE_GAP | 0x0C03);
	start_tick = bt_os_layer_get_system_tick();
	CU_ASSERT(BT_STATUS_SUCCESS == bt_timer_start(timer_id, 100, 0, timeout));
	CU_ASSERT(BT_STATUS_SUCCESS == bt_timer_start(timer_id+1, 200, 0, timeout1));
	CU_ASSERT(BT_STATUS_SUCCESS == bt_timer_start(timer_id+2, 300, 0, timeout2));
}

bt_status_t bt_hci_test_timeout_callback(bool is_timeout, uint32_t timer_id, uint32_t data, const void *param);
bt_status_t bt_hci_test_timeout_callback(bool is_timeout, uint32_t timer_id, uint32_t data, const void *param)
{
	bt_status_t status = BT_STATUS_SUCCESS;
	bt_hci_cmd_t cmd;
	cmd.cmd_code = BT_HCI_CMD_RESET;
	cmd.length = 0;
	cmd.param = NULL;
	if (is_timeout) {
		status = bt_hci_cmd_send(cmd, 0, 500, bt_hci_test_timeout_callback);
	} else {
		cmd.cmd_code = BT_HCI_CMD_READ_BD_ADDR;
		//status = bt_hci_cmd_send(cmd, 0, bt_hci_test_timeout_callback);
	}
	return status;
}

void bt_hci_test(void)
{
	bt_status_t status = BT_STATUS_SUCCESS;
	bt_hci_cmd_t cmd;
	cmd.cmd_code = BT_HCI_CMD_RESET;
	cmd.length = 0;
	cmd.param = NULL;
	status = bt_hci_cmd_send(cmd, 0, 500, bt_hci_test_timeout_callback);
	BT_ASSERT(status == BT_STATUS_SUCCESS);
}
