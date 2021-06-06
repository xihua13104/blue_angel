/******************************************************************************
  * @file           bt_inknode.c
  * @author         leon
  * @version        V0.0.1
  * @date           2020-12-05
  * @brief
******************************************************************************/

#include "bt_linknode.h"

void bt_linknode_insert_node(bt_linknode_t *head, bt_linknode_t *node_to_insert, bt_linknode_position_t position)
{
    bt_linknode_t *tmp = head;
    BT_ASSERT(head);
    if (position == BT_NODE_TAIL) {
        while (tmp->next != NULL) {
            tmp = tmp->next;
        }
    }
    node_to_insert->next = tmp->next;
    tmp->next = node_to_insert;
}

bt_linknode_t *bt_linknode_delete_node(bt_linknode_t *head, bt_linknode_position_t position)
{
    bt_linknode_t *tmp = head;
    bt_linknode_t *node_to_delete = tmp->next;
    BT_ASSERT(head);
    if (node_to_delete != NULL) {
        if (position == BT_NODE_TAIL) {
            while (node_to_delete->next != NULL) {
                tmp = node_to_delete;
                node_to_delete = node_to_delete->next;
            }
        }
        tmp->next = node_to_delete->next;
        node_to_delete->next = NULL;
    }
    return node_to_delete;
}

bool bt_linknode_cmp_backward(const bt_linknode_t *node, const void *data)
{
    return (node->next == (const bt_linknode_t *)data);
}

/*遍历整个链表，如果找到目标node(data),则返回其前驱节点*/
bt_linknode_t *bt_linknode_travel_node(bt_linknode_t *head, bt_linknode_cmp_t cmp_func, const void *data)
{
    bt_linknode_t *tmp = head;
    while (tmp) {
        if (cmp_func(tmp, data)) {
            return tmp;
        }
        tmp = tmp->next;
        /*这条链表可能是循环链表*/
        if (tmp == head) {
            return NULL;
        }
    }
    return NULL;
}
