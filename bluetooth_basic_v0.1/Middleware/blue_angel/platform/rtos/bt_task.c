/******************************************************************************
  * @file           bt_task.c
  * @author         Leonard-y.He(1027901566@qq.com)
  * @version        V0.1
  * @date           2021-05-19
  * @brief
******************************************************************************/
#include "bt_platform.h"
#include "bt_test.h"
#include "bt_driver.h"
#include "bt_log.h"

void vBT_Task(void *pvParameters)
{
	cunit_status_t cunit_status;
	uint32_t i = 0;
	uint8_t reset[] = {0x03, 0x0c, 0x00};
	BT_ASSERT(CUNIT_SUCCESS == cunit_add_test_case("bt_linknode_test", bt_linknode_test));
	BT_ASSERT(CUNIT_SUCCESS == cunit_add_test_case("bt_memory_test", bt_memory_test));
	BT_ASSERT(CUNIT_SUCCESS == cunit_add_test_case("bt_timer_test", bt_timer_test));
	cunit_status = cunit_test_start();
	BT_ASSERT(cunit_status == CUNIT_SUCCESS);
	bt_driver_init();
	for (i = 0; i < 2; i ++) {
		bt_driver_send_data_to_controller(reset, sizeof(reset));
		bt_hci_log(BT_HCI_LOG_CMD, reset, sizeof(reset));
	}	
    while(1)
    {
		bt_task_take_semaphore();
		bt_task_event_handler();
	}
}


