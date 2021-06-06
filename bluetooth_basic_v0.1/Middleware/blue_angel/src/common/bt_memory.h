/******************************************************************************
  * @file           bt_memory.h
  * @author         leon
  * @version        V0.1
  * @date           2021-05-19
  * @brief          bt memory dynamic pool header file
******************************************************************************/
#ifndef __BT_MEMORY_H__
#define __BT_MEMORY_H__
#include "bt_type.h"
#include "bt_linknode.h"
#include "bt_system.h"

#define BT_MM_HEADER_SIZE  (sizeof(bt_mm_header_t))
#define BT_MM_FOOTER_SIZE  (4)
#define BT_MM_FOOTER 	   (0xABCDABCD)

#define MEMORY_ALIGNMENT   (4)
#define MEMORY_ALIGN_SIZE(size) (((size) + MEMORY_ALIGNMENT - 1) & ~(MEMORY_ALIGNMENT-1))

#define BT_MM_SIZE_MASK    (0x3FFFFFFF)
#define BT_MM_STATE_MASK   (0xC0000000)
#define BT_MM_SIZE_OFFSET  (0)
#define BT_MM_STATE_OFFSET (30)
#define BT_MM_STATE_FREE   (0U)
#define BT_MM_STATE_USING  (1U)

#define BT_MM_SET_STATE(info, state) ((info) = (((info) & BT_MM_SIZE_MASK) | ((state)<<BT_MM_STATE_OFFSET)))
#define BT_MM_GET_STATE(info)        ((info) >> BT_MM_STATE_OFFSET)
#define BT_MM_SET_SIZE(info, size)	 ((info) = (((info) & BT_MM_STATE_MASK) | ((size) & BT_MM_SIZE_MASK)))
#define BT_MM_GET_SIZE(info)		 ((info) & BT_MM_SIZE_MASK)
#define BT_MM_SET_INFO(info, state, size) ((info) = ((((state) << BT_MM_STATE_OFFSET) & BT_MM_STATE_MASK) | ((size) & BT_MM_SIZE_MASK)))

/**********************
bit31 & bit30 indicats the memory state
00->free
01->using
01->reserve for future
11->reserve for future

bit29-bit0 indicats the memory block size
**********************/
typedef struct {
    uint32_t info;
} bt_mm_header_t;

typedef struct {
    bt_mm_header_t *start_mm_h[2];
    bt_mm_header_t *search_mm_h[2];
    uint32_t mm_poll_size[2];
    bt_linknode_t rx_queue;
    bt_linknode_t cmd_queue;
    bt_linknode_t acl_queue;
} bt_mm_poll_ctrl_block_t;

extern bt_mm_poll_ctrl_block_t *p_bt_mm_cb;

void bt_memory_init(bt_memory_type_t type, uint8_t *buf, uint32_t size);

uint8_t *bt_memory_allocate_packet(bt_memory_type_t type, uint32_t size);

void bt_memory_free_packet(bt_memory_type_t type, uint8_t *ptr);

void bt_fixed_memory_init(bt_fixed_memory_type_t type, uint8_t *buf, uint32_t size);

uint8_t *bt_fixed_memory_allocate(bt_fixed_memory_type_t type);

void bt_fixed_memory_free(bt_fixed_memory_type_t type, uint8_t *buf);

#endif //__BT_MEMORY_H__


