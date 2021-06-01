/******************************************************************************
  * @file           bt_platform.h
  * @author         Leonard-y.He(1027901556@qq.com)
  * @version        V0.1
  * @date           2021-05-17
  * @brief
******************************************************************************/

#ifndef __BT_PLATFORM_H
#define __BT_PLATFORM_H

#include "stm32f10x_conf.h"
#include "core_cmfunc.h"
#include "cunit_interface.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#define BT_ASSERT(expr) assert_param(expr)

#define BT_TASK_EVENT_TIMER_EXPIRED	0x01
#define BT_TASK_EVENT_RX			0x02
#define BT_TASK_EVENT_TX			0x04
#define BT_TASK_EVENT_OOM			0x08

void bt_timer_start_timer(uint32_t timer_length);

void bt_timer_stop_timer(void);

uint32_t bt_timer_get_current_tick(void);

void bt_memset(void *des, uint8_t value, uint32_t length);

void bt_memcpy(void *des, void *src, uint32_t length);

bool bt_memcmp(void *des, void *src, uint32_t length);

void bt_task_take_semaphore(void);

void bt_task_event_handler(void);

void bt_task_event_notify(uint32_t event, uint16_t data_length, void *data);

void bt_init(void);

void bt_deinit(void);
#endif
