/******************************************************************************
  * @file           bt_task.c
  * @author         leon
  * @version        V0.1
  * @date           2021-05-19
  * @brief
******************************************************************************/
#include "bt_platform.h"
#include "bt_test.h"
#include "bt_driver.h"
#include "bt_log.h"
#include "bt_config.h"

void vBT_Task(void *pvParameters)
{
    bt_task_init();
#if BT_CUNIT_ENABLE
    /*BT_ASSERT(CUNIT_SUCCESS == cunit_add_test_case("bt_linknode_test", bt_linknode_test));
    BT_ASSERT(CUNIT_SUCCESS == cunit_add_test_case("bt_memory_test", bt_memory_test));
    BT_ASSERT(CUNIT_SUCCESS == cunit_add_test_case("bt_timer_test", bt_timer_test));
    BT_ASSERT(CUNIT_SUCCESS == cunit_add_test_case("bt_hci_test", bt_hci_test));
	BT_ASSERT(CUNIT_SUCCESS == cunit_add_test_case("bt_gap_test", bt_gap_test));
    BT_ASSERT(cunit_test_start() == CUNIT_SUCCESS);*/
#endif
    while (1) {
        bt_task_take_semaphore();
        bt_task_event_handler();
    }
}


