/******************************************************************************
  * @file           bt_hci_spec.h
  * @author         Leonard-y.He(1027901556@qq.com)
  * @version        V0.1
  * @date           2021-05-19
  * @brief
******************************************************************************/
#ifndef __BT_HCI_SPEC_H__
#define __BT_HCI_SPEC_H__
#include "bt_type.h"
#include "bt_linknode.h"

#define BT_UART_CMD 0x01
#define	BT_UART_ACL 0x02
#define	BT_UART_EVT 0x04
typedef uint8_t bt_hci_spec_packet_indicator_t;

#define BT_HCI_CMD_HEADER_SIZE	(sizeof(bt_hci_spec_packet_indicator_t) + sizeof(bt_hci_spec_cmd_t) - 1) //4
#define BT_HCI_ACL_HEADER_SIZE	(sizeof(bt_hci_spec_packet_indicator_t) + sizeof(bt_hci_spec_acl_t) - 1) //5
#define BT_HCI_EVT_HEADER_SIZE	(sizeof(bt_hci_spec_packet_indicator_t) + sizeof(bt_hci_spec_evt_t) - 1) //3
#define BT_HCI_CMD_SIZE(p) 		(*((uint16_t *)((uint8_t *)p + 3)) + BT_HCI_CMD_HEADER_SIZE)
#define BT_HCI_ACL_SIZE(p) 		(*((uint16_t *)((uint8_t *)p + 3)) + BT_HCI_ACL_HEADER_SIZE)
#define BT_HCI_EVT_SIZE(p) 		(*((uint8_t *)((uint8_t *)p + 2)) + BT_HCI_EVT_HEADER_SIZE)

typedef struct {
	uint16_t op_code;
	uint8_t length;
	uint8_t data[1];
} BT_PACKED bt_hci_spec_cmd_t;

typedef struct {
	uint16_t handle;
	uint16_t length;
	uint8_t data[1];
} BT_PACKED bt_hci_spec_acl_t;

typedef struct {
	uint8_t evt_code;
	uint8_t length;
	uint8_t data[1];
} BT_PACKED bt_hci_spec_evt_t;

typedef struct {
	bt_hci_spec_packet_indicator_t indicator;
	union {
		bt_hci_spec_cmd_t cmd;
		bt_hci_spec_acl_t acl;
		bt_hci_spec_evt_t evt;
	} BT_PACKED value;
} BT_PACKED bt_hci_spec_packet_t;

typedef struct {
	bt_linknode_t node;
} BT_PACKED bt_hci_packet_header_t;

typedef struct {
	bt_hci_packet_header_t header;
	bt_hci_spec_packet_t hci_spec_packet;
} BT_PACKED bt_hci_packet_t;

#endif//__BT_HCI_SPEC_H__
