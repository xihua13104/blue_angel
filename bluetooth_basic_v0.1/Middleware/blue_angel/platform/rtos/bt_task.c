/******************************************************************************
  * @file           bt_task.c
  * @author         Leonard-y.He(1027901566@qq.com)
  * @version        V0.1
  * @date           2021-05-19
  * @brief
******************************************************************************/
#include "bt_platform.h"
#include "bt_test.h"

void vBT_Task(void *pvParameters)
{
	cunit_status_t cunit_status;

	//BT_ASSERT(cunit_status == cunit_add_test_case("bt_linknode_test", bt_linknode_test));
	BT_ASSERT(cunit_status == cunit_add_test_case("bt_memory_test", bt_memory_test));
	//BT_ASSERT(cunit_status == cunit_add_test_case("bt_timer_test", bt_timer_test));
    while(1)
    {
		cunit_status = cunit_test_start();
		BT_ASSERT(cunit_status == CUNIT_SUCCESS);
		bt_task_take_semaphore();
		bt_task_event_handler();
	}
}


