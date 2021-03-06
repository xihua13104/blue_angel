/******************************************************************************
  * @file           bt_os_layer_api.h
  * @author         leon
  * @version        V0.1
  * @date           2021-05-17
  * @brief
******************************************************************************/

#ifndef __BT_OS_LAYER_API_H
#define __BT_OS_LAYER_API_H
#include "bt_platform.h"
#include "FreeRTOS.h"
#include "task.h"

typedef void (*bt_os_layer_timer_expire_callback_t)(void);

void bt_os_layer_register_timer_callback(bt_os_layer_timer_expire_callback_t cb);

void bt_os_layer_init_timer(void);

void bt_os_layer_deinit_timer(void);

bool bt_os_layer_is_timer_active(void);

void bt_os_layer_start_timer(uint32_t ms);

void bt_os_layer_stop_timer(void);

uint32_t bt_os_layer_get_system_tick(void);

void bt_os_layer_disable_interrupt(void);

void bt_os_layer_enable_interrupt(void);

uint32_t bt_os_layer_create_mutex(void);

void bt_os_layer_delete_mutex(uint32_t mutex_id);

void bt_os_layer_take_mutex(uint32_t mutex_id);

void bt_os_layer_give_mutex(uint32_t mutex_id);

uint32_t bt_os_layer_create_semaphore(void);

void bt_os_layer_delete_semaphore(uint32_t semaphore);

void bt_os_layer_take_semaphore(uint32_t semaphore, uint32_t block_time);

void bt_os_layer_take_semaphore_from_isr(uint32_t semaphore);

void bt_os_layer_give_semaphore(uint32_t semaphore);

void bt_os_layer_give_semaphore_from_isr(uint32_t semaphore);

uint32_t bt_os_layer_create_queue(uint32_t queue_length, uint32_t item_size);

void bt_os_layer_delete_queue(uint32_t queue_handle);

void bt_os_layer_queue_send(uint32_t queue_handle, const void * const item_to_queue, uint32_t block_time);

void bt_os_layer_queue_send_from_isr(uint32_t queue_handle, const void * const item_to_queue);

void bt_os_layer_queue_receive(uint32_t queue_handle, void * const buffer, uint32_t block_time);

void bt_os_layer_queue_receive_from_isr(uint32_t queue_handle, void * const buffer);

bool bt_os_layer_is_isr_active(void);

bool bt_os_layer_is_os_scheduler_start(void);

uint8_t *bt_os_layer_malloc(uint32_t size);

void bt_os_layer_free(uint8_t *ptr);

#endif//__BT_OS_LAYER_API_H

