/******************************************************************************
  * @file           bt_platform.c
  * @author         Leonard-y.He(1027901566@qq.com)
  * @version        V0.1
  * @date           2021-05-19
  * @brief
******************************************************************************/
#include "bt_platform.h"
#include "bt_os_layer_api.h"
#include "bt_timer.h"
#include "bt_driver.h"
#include "bt_hci.h"

typedef struct {
	uint32_t event;
	uint16_t data_length;
	void *data;
} bt_task_event_notify_t;

static uint32_t bt_task_mutex = 0;
static uint32_t bt_task_semaphore = 0;
static bt_task_event_notify_t notify = {0};
void bt_task_take_semaphore()
{
	if (bt_os_layer_is_isr_active()) {
		bt_os_layer_take_semaphore_from_isr(bt_task_semaphore);
	} else {
		bt_os_layer_take_semaphore(bt_task_semaphore, portMAX_DELAY);
	}
}

static void bt_task_interrupt_trigger()
{
	BT_ASSERT(bt_task_semaphore);
	if (bt_os_layer_is_isr_active()) {
		bt_os_layer_give_semaphore_from_isr(bt_task_semaphore);
	} else {
		bt_os_layer_give_semaphore(bt_task_semaphore);
	}
}

void bt_task_event_handler()
{
	uint32_t event = 0;
	bt_os_layer_disable_interrupt();
	event = notify.event;
	notify.event = 0;
	bt_os_layer_enable_interrupt();

	bt_os_layer_take_mutex(bt_task_mutex);
	if (event & BT_TASK_EVENT_TIMER_EXPIRED) {
		bt_timer_check_timeout_handler();
	}
	if (event & BT_TASK_EVENT_RX) {
		bt_driver_rx(notify.data_length);
		bt_hci_packet_process();
	}
	if (event & BT_TASK_EVENT_TX) {

	}
	if (event & BT_TASK_EVENT_OOM) {

	}
	bt_os_layer_give_mutex(bt_task_mutex);
}

void bt_task_event_notify(uint32_t event, uint16_t data_length, void *data)
{
	bt_os_layer_disable_interrupt();
	notify.event |= event;
	notify.data_length |= data_length;
	notify.data = data;
	bt_os_layer_enable_interrupt();
	bt_task_interrupt_trigger();
}

static void bt_timer_timeout_callback()
{
	bt_task_event_notify(BT_TASK_EVENT_TIMER_EXPIRED, 0, NULL);
}

void bt_init()
{
	bt_os_layer_init_timer();
	bt_os_layer_register_timer_callback(bt_timer_timeout_callback);
	if (bt_task_mutex == 0) {
		bt_task_mutex = bt_os_layer_create_mutex();
		BT_ASSERT(bt_task_mutex);
	}
	if (bt_task_semaphore == 0) {
		bt_task_semaphore = bt_os_layer_create_semaphore();
		BT_ASSERT(bt_task_semaphore);
	}
	//bt_driver_init();
}

void bt_deinit()
{
	bt_os_layer_deinit_timer();
	if (bt_task_mutex) {
		bt_os_layer_delete_mutex(bt_task_mutex);
		bt_task_mutex = 0;
	}
	if (bt_task_semaphore) {
		bt_os_layer_delete_semaphore(bt_task_semaphore);
		bt_task_semaphore = 0;
	}
}

void bt_timer_start_timer(uint32_t timer_length)
{
	bt_os_layer_start_timer(timer_length);
}

void bt_timer_stop_timer()
{
	bt_os_layer_stop_timer();
}

uint32_t bt_timer_get_current_tick()
{
	return bt_os_layer_get_system_tick();
}

void bt_memset(void *des, uint8_t value, uint32_t length)
{
	memset(des, value, length);
}

void bt_memcpy(void *des, void *src, uint32_t length)
{
	memcpy(des, src, length);
}

bool bt_memcmp(void *des, void *src, uint32_t length)
{
	return memcmp(des, src, length);
}

