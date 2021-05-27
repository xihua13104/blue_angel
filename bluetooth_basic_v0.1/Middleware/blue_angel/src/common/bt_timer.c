/******************************************************************************
  * @file           bt_timer.c
  * @author         Leonard-y.He(1027901566@qq.com)
  * @version        V0.1
  * @date           2021-05-19
  * @brief
******************************************************************************/
#include "bt_timer.h"
#include "bt_memory.h"
#include "bt_os_layer_api.h"

typedef struct {
	uint32_t current_tick;
	uint32_t duration;
} bt_timer_duration_t;

static bt_linknode_t bt_timer_list = {NULL};

bool bt_timer_cmp_by_timerid(const bt_linknode_t *node, const void *data)
{
	bt_timer_t *timer = NULL;
	uint32_t timer_id = 0;
	BT_ASSERT(node && data);
	
	timer = (bt_timer_t *)node->next;
	if (timer == NULL) {
		return true;
	}
	timer_id = *(uint32_t *)data;
	return timer->timer_id == timer_id;
}

bool bt_timer_cmp_by_timer_duration(const bt_linknode_t *node, const void *data)
{
	bt_timer_duration_t *duration = NULL;
	bt_timer_t *timer = NULL;
	int32_t is_timeout = 0;
	int32_t diff = 0;
	BT_ASSERT(node && data);

	duration = (bt_timer_duration_t *)data;
	timer = (bt_timer_t *)(node->next);
	if (timer == NULL) {
		return true;
	}

	is_timeout = timer->time_ms - duration->current_tick;
	diff = timer->time_ms - duration->current_tick - duration->duration;
	return ((is_timeout > 0) && (diff > 0));
}

bt_status_t bt_timer_start(uint32_t timer_id, uint32_t timer_length, uint32_t data, bt_timer_timeout_callback_t cb)
{
	bt_timer_t *timer = NULL;
	bt_timer_duration_t duration = {0};
	bt_linknode_t *pre = NULL;
	BT_ASSERT(timer_length > 0);
	if (NULL != bt_timer_find(timer_id)) {
		return BT_STATUS_FAIL;
	}

	timer = (bt_timer_t *)bt_fixed_memory_allocate(BT_FIXED_MM_TIMER);
	if (timer == NULL) {
		return BT_STATUS_OUT_OF_MEMORY;
	}
	duration.current_tick = bt_os_layer_get_system_tick();
	duration.duration = timer_length;
	timer->timer_id = timer_id;
	timer->data = data;
	timer->timeout_cb = cb;
	timer->time_ms = duration.current_tick + duration.duration;

	pre = bt_linknode_travel_node(&bt_timer_list, bt_timer_cmp_by_timer_duration, (void *)&duration);
	if (pre != NULL) {
		bt_linknode_insert_node(pre, (bt_linknode_t *)timer, BT_NODE_FRONT);
		if (pre == &bt_timer_list) {
			/*update for the first timer*/
			bt_timer_stop_timer();
			bt_timer_start_timer(timer_length);
		}
		return BT_STATUS_SUCCESS;
	}

	return BT_STATUS_FAIL;
}

bt_status_t bt_timer_cancel(uint32_t timer_id)
{
	bt_timer_t *timer = NULL;
	uint32_t current_tick = 0;
	int32_t diff = 0;
	bt_linknode_t *pre = bt_linknode_travel_node(&bt_timer_list, bt_timer_cmp_by_timerid, (const void *)&timer_id);
	if (pre == NULL) {
		return BT_STATUS_TIMER_NOT_FOUND;
	}
	timer = (bt_timer_t *)bt_linknode_delete_node(pre, BT_NODE_FRONT);
	/*删除的是第一个timer node*/
	if (pre == &bt_timer_list) {
		bt_timer_stop_timer();
		if (bt_timer_list.next) {
			current_tick = bt_timer_get_current_tick();
			diff = ((bt_timer_t *)(bt_timer_list.next))->time_ms - current_tick;
			if (diff < 0) {
				diff = 0;
			}
			bt_timer_start_timer(diff);
		}
	}
	bt_fixed_memory_free(BT_FIXED_MM_TIMER, (uint8_t *)timer);
	
	return BT_STATUS_SUCCESS;
}

bt_timer_t *bt_timer_find(uint32_t timer_id)
{
	bt_linknode_t *pre = NULL;
	pre = bt_linknode_travel_node(&bt_timer_list, bt_timer_cmp_by_timerid, (const void *)&timer_id);
	if (pre) {
		return (bt_timer_t *)(pre->next);
	}
	return NULL;
}

bt_status_t bt_timer_cancel_and_callback(uint32_t timer_id, const void *param)
{
	bt_status_t status = BT_STATUS_SUCCESS;
	bt_timer_t tmp = {0};
	bt_timer_t *timer = NULL;
	bt_linknode_t *pre = bt_linknode_travel_node(&bt_timer_list, bt_timer_cmp_by_timerid, (const void *)&timer_id);
	if (pre == NULL) {
		return BT_STATUS_TIMER_NOT_FOUND;
	}
	timer = (bt_timer_t *)(pre->next);
	bt_memcpy(&tmp, timer, sizeof(bt_timer_t));
	bt_timer_cancel(timer_id);
	if (tmp.timeout_cb) {
		status = tmp.timeout_cb(false, timer_id, tmp.data, param);
	}
	return status;
}

void bt_timer_stop_all_timer()
{
	bt_linknode_t *tmp= NULL;
	bt_timer_stop_timer();

	while (bt_timer_list.next != NULL) {
		tmp = bt_linknode_delete_node(&bt_timer_list, BT_NODE_FRONT);
		bt_fixed_memory_free(BT_FIXED_MM_TIMER, (uint8_t *)tmp);
	}
}

void bt_timer_check_timeout_handler()
{
	bt_timer_t *timer = NULL;
	uint32_t current_tick = bt_timer_get_current_tick();
	int32_t diff = 0;
	bt_linknode_t expired_node_header = {NULL};
	bt_linknode_t *tmp = NULL;
	bt_timer_t local_timer_buf = {0};

	tmp = &bt_timer_list;
	while (tmp != NULL) {
		timer = (bt_timer_t *)(tmp->next);
		if (timer != NULL && timer->time_ms <= current_tick) {
			bt_linknode_insert_node(&expired_node_header, bt_linknode_delete_node(tmp, BT_NODE_FRONT), BT_NODE_TAIL);
			tmp = &bt_timer_list;
		} else {
			tmp = tmp->next;	
		}
	}
	/*没有timer到期*/
	if (expired_node_header.next == NULL) {
		return;
	}

	while (expired_node_header.next != NULL) {
		timer = (bt_timer_t *)(expired_node_header.next);
		bt_memcpy(&local_timer_buf, timer, sizeof(bt_timer_t));
		if (local_timer_buf.timeout_cb) {
			local_timer_buf.timeout_cb(true, local_timer_buf.timer_id, local_timer_buf.data, NULL);
		}
		bt_linknode_delete_node(&expired_node_header, BT_NODE_FRONT);
		bt_fixed_memory_free(BT_FIXED_MM_TIMER, (uint8_t *)timer);
	}
	/*到期的是第一个timer，则需要更新timer list*/
	if (bt_timer_list.next) {
		bt_timer_stop_timer();
		current_tick = bt_timer_get_current_tick();
		diff = ((bt_timer_t *)(bt_timer_list.next))->time_ms - current_tick;
		if (diff < 0) {
			diff = 0;
		}
		bt_timer_start_timer(diff);
	}
}

