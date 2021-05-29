/******************************************************************************
  * @file           bt_linknode.h
  * @author         Leonard-y.He(1027901556@qq.com)
  * @version        V0.0.1
  * @date           2020-12-05
  * @brief
******************************************************************************/
#ifndef __BT_LINKNODE_H__
#define __BT_LINKNODE_H__
#include "bt_type.h"

#define BT_NODE_FRONT 0x01
#define BT_NODE_TAIL  0x02
typedef uint8_t bt_linknode_position_t;

typedef struct _link_node {
	struct _link_node *next;
} bt_linknode_t;

typedef bool (*bt_linknode_cmp_t)(const bt_linknode_t *node, const void *data);

void bt_linknode_insert_node(bt_linknode_t *head, bt_linknode_t *node_to_insert, bt_linknode_position_t position);

bt_linknode_t *bt_linknode_delete_node(bt_linknode_t *head, bt_linknode_position_t position);

bool bt_linknode_cmp_backward(const bt_linknode_t *node, const void *data);

bt_linknode_t *bt_linknode_travel_node(bt_linknode_t *head, bt_linknode_cmp_t cmp_func, const void *data);
#endif //__BT_LINKNODE_H__


