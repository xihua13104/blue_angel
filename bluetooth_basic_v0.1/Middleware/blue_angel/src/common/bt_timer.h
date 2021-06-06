/******************************************************************************
  * @file           bt_timer.h
  * @author         leon
  * @version        V0.1
  * @date           2021-05-19
  * @brief
******************************************************************************/
#ifndef __BT_TIMER_H__
#define __BT_TIMER_H__
#include "bt_type.h"
#include "bt_linknode.h"

typedef bt_status_t (*bt_timer_timeout_callback_t)(bool is_timeout, uint32_t timer_id, uint32_t data, const void *param);

typedef struct _bt_timer {
    struct _bt_timer *next;
    uint32_t timer_id;
    uint32_t data;
    uint32_t time_ms;
    bt_timer_timeout_callback_t timeout_cb;
} bt_timer_t;

bt_status_t bt_timer_start(uint32_t timer_id, uint32_t timer_length, uint32_t data, bt_timer_timeout_callback_t cb);

bt_status_t bt_timer_cancel(uint32_t timer_id);

bt_timer_t *bt_timer_find(uint32_t timer_id);

bt_status_t bt_timer_cancel_and_callback(uint32_t timer_id, const void *param);

void bt_timer_stop_all_timer(void);

void bt_timer_check_timeout_handler(void);

bool bt_timer_cmp_by_timerid(const bt_linknode_t *node, const void *data);

bool bt_timer_cmp_by_timer_duration(const bt_linknode_t *node, const void *data);
#endif //__BT_TIMER_H__
