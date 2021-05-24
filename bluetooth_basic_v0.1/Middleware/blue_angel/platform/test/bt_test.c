/******************************************************************************
  * @file           bt_test.c
  * @author         Leonard-y.He(1027901566@qq.com)
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

#define BT_TIMER_BLOCK_SIZE (MEMORY_ALIGN_SIZE(sizeof(bt_timer_t)) + BT_MM_HEADER_SIZE + BT_MM_FOOTER_SIZE)
__align(4) uint8_t bt_timer_fixed_memory[BT_TIMER_BLOCK_SIZE * 5] = {0};

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
	bt_fixed_memory_init(BT_FIXED_MM_TIMER, bt_timer_fixed_memory, sizeof(bt_timer_fixed_memory));
	timer = (bt_timer_t *)bt_fixed_memory_allocate(BT_FIXED_MM_TIMER); 
	CU_ASSERT(timer != NULL);
	timer = (bt_timer_t *)bt_fixed_memory_allocate(BT_FIXED_MM_TIMER); 
	CU_ASSERT(timer != NULL);
	timer = (bt_timer_t *)bt_fixed_memory_allocate(BT_FIXED_MM_TIMER); 
	CU_ASSERT(timer != NULL);
	timer = (bt_timer_t *)bt_fixed_memory_allocate(BT_FIXED_MM_TIMER); 
	CU_ASSERT(timer != NULL);
	timer = (bt_timer_t *)bt_fixed_memory_allocate(BT_FIXED_MM_TIMER); 
	CU_ASSERT(timer != NULL);
	timer = (bt_timer_t *)bt_fixed_memory_allocate(BT_FIXED_MM_TIMER); 
	CU_ASSERT(timer != NULL);
	bt_fixed_memory_free(BT_FIXED_MM_TIMER, (uint8_t *)timer);
	//bt_fixed_memory_free(BT_FIXED_MM_TIMER, (uint8_t *)timer);
}

void bt_timer_test(void) 
{ 
	CU_ASSERT_STRING_EQUAL_FATAL("abc", "123"); 
}
