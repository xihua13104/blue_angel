/******************************************************************************
  * @file           bt_common.h
  * @author         leon
  * @version        V0.0.1
  * @date           2020-12-05
  * @brief          bt common header file
******************************************************************************/

#ifndef COMMON_H_H_H
#define COMMON_H_H_H
#include "bt_config.h"
#include "stdint.h"
#include "bt_os_layer_api.h"
#include "bt_memory.h"
#include "bt_hci_spec.h"

#define BT_SHARE_BUFFER_LOCK()   taskENTER_CRITICAL()
#define BT_SHARE_BUFFER_UNLOCK() taskEXIT_CRITICAL()

#define BT_ALLOCATE_HCI_PACKET_WITH_NODE(memory_type, size) (bt_memory_allocate_packet(memory_type, sizeof(bt_hci_packet_header_t) + size) + sizeof(bt_hci_packet_header_t))
#define BT_FREE_HCI_PACKET_WITH_NODE(memory_type, p)  		(bt_memory_free_packet(memory_type, (uint8_t *)p - sizeof(bt_hci_packet_header_t)))

#define BT_GET_NODE_FROM_HCI_SPEC_PACKET(p)    ((bt_linknode_t *)((uint8_t *)p - sizeof(bt_hci_packet_header_t)))
#define BT_GET_HCI_SPEC_PACKET_FROM_NODE(node) ((bt_hci_spec_packet_t *)((uint8_t *)node + sizeof(bt_hci_packet_header_t)))

#define BT_RX_QUEUE  (&p_bt_mm_cb->rx_queue)
#define BT_CMD_QUEUE (&p_bt_mm_cb->cmd_queue)

#define BT_PUSH_NODE_TO_QUEUE(q, node, position) bt_linknode_insert_node(q, node, position)
#define BT_POP_NODE_FROM_QUEUE(q, position)      bt_linknode_delete_node(q, position)

#define BT_PUSH_NODE_TO_RX_QUEUE_FRONT(node)     BT_PUSH_NODE_TO_QUEUE(BT_RX_QUEUE, node, BT_NODE_FRONT)
#define BT_PUSH_NODE_TO_RX_QUEUE_TAIL(node)      BT_PUSH_NODE_TO_QUEUE(BT_RX_QUEUE, node, BT_NODE_TAIL)
#define BT_POP_NODE_FROM_RX_QUEUE_FRONT()		 BT_POP_NODE_FROM_QUEUE(BT_RX_QUEUE, BT_NODE_FRONT)
#define BT_POP_NODE_FROM_RX_QUEUE_TAIL()		 BT_POP_NODE_FROM_QUEUE(BT_RX_QUEUE, BT_NODE_TAIL)

#define BT_QUEUE_IS_EMPTY(q) 					 (q->next == NULL)

#endif

