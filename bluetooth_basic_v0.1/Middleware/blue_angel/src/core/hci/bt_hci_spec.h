/******************************************************************************
  * @file           bt_hci_spec.h
  * @author         leon
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
#define BT_HCI_CMD_SIZE(p) 		(*((uint8_t *)((uint8_t *)p + 3)) + BT_HCI_CMD_HEADER_SIZE)
#define BT_HCI_ACL_SIZE(p) 		(*((uint16_t *)((uint8_t *)p + 3)) + BT_HCI_ACL_HEADER_SIZE)
#define BT_HCI_EVT_SIZE(p) 		(*((uint8_t *)((uint8_t *)p + 2)) + BT_HCI_EVT_HEADER_SIZE)


/* Group occupies high 6 bits of the HCI command rest is opcode itself */
#define BT_HCI_OGF(p)  (uint8_t)((0xFC00 & (p)) >> 10)
#define BT_HCI_OCF(p)  ( 0x3FF & (p))

/* Opcode Group Field (OGF) values */
#define BT_HCI_NOP_OGF 			    0x00
#define BT_HCI_OGF_LINK_CONTROL 	0x01/* Link Control Commands */
#define BT_HCI_OGF_LINK_POLICY 		0x02/* Link Policy Commands */
#define BT_HCI_OGF_HOST_C_N_BB      0x03/* Host Controller & Baseband Commands */
#define BT_HCI_OGF_INFO_PARAM       0x04/* Informational Parameters */
#define BT_HCI_OGF_STATUS_PARAM     0x05/* Status Parameters */
#define BT_HCI_OGF_TESTING          0x06/* Testing Commands */
#define BT_HCI_OGF_LE	            0x08/* BLE cmd */
#define BT_HCI_OGF_VENDOR           0x3F/* vendor Commands */

/*
**  Definitions for HCI group
*/
#define BT_HCI_GRP_LINK_CTL_CMDS        (0x01 << 10)/* 0x0400 */
#define BT_HCI_GRP_LINK_PLY_CMDS        (0x02 << 10)/* 0x0800 */
#define BT_HCI_GRP_HOST_CONT_BB_CMDS    (0x03 << 10)/* 0x0C00 */
#define BT_HCI_GRP_INFO_PARA            (0x04 << 10)/* 0x1000 */
#define BT_HCI_GRP_STATUS_PARA          (0x05 << 10)/* 0x1400 */
#define BT_HCI_GRP_TEST_CMDS            (0x06 << 10)/* 0x1800 */
#define BT_HCI_GRP_BLE_CMDS             (0x08 << 10)/* 0x1C00 */
#define BT_HCI_GRP_VENDOR_SPECIFIC      (0x3F << 10)/* 0xFC00 */

typedef uint16_t bt_hci_cmd_code_t;

/*Link Control Commands (OGF = 0x01)*/
#define BT_HCI_CMD_INQUIRY									(0x0001 | BT_HCI_GRP_LINK_CTL_CMDS)//Command used to enter Inquiry mode where it discovers other Bluetooth devices.
#define BT_HCI_CMD_INQUIRY_CANCEL							(0x0002 | BT_HCI_GRP_LINK_CTL_CMDS)//Command to cancel the Inquiry mode in which the Bluetooth device is in.
#define BT_HCI_CMD_PERIODIC_INQUIRY_MODE					(0x0003 | BT_HCI_GRP_LINK_CTL_CMDS)//Command to set the device to enter Inquiry modes periodically according to the time interval set.
#define BT_HCI_CMD_EXIT_PERIODIC_INQUIRY_MODE				(0x0004 | BT_HCI_GRP_LINK_CTL_CMDS)//Command to exit the periodic Inquiry mode
#define BT_HCI_CMD_CREATE_CONNECTION						(0x0005 | BT_HCI_GRP_LINK_CTL_CMDS)//Command to create an ACL connection to the device specified by the BD_ADDR in the parameters.
#define BT_HCI_CMD_DISCONNECT								(0x0006 | BT_HCI_GRP_LINK_CTL_CMDS)//Command to terminate the existing connection to a device
#define BT_HCI_CMD_ADD_SCO_CONNECTION						(0x0007 | BT_HCI_GRP_LINK_CTL_CMDS)//Create an SCO connection defined by the connection handle parameters.
#define BT_HCI_CMD_ACCEPT_CONNECTION_REQUEST				(0x0009 | BT_HCI_GRP_LINK_CTL_CMDS)//Command to accept a new connection request
#define BT_HCI_CMD_REJECT_CONNECTION_REQUEST				(0x000A | BT_HCI_GRP_LINK_CTL_CMDS)//Command to reject a new connection request
#define BT_HCI_CMD_LINK_KEY_REQUEST_REPLY					(0x000B | BT_HCI_GRP_LINK_CTL_CMDS)//Reply command to a link key request event sent from controller to the host
#define BT_HCI_CMD_LINK_KEY_REQUEST_NEGATIVE_REPLY			(0x000C | BT_HCI_GRP_LINK_CTL_CMDS)//Reply command to a link key request event from the controller to the host if there is no link key associated with the connection.
#define BT_HCI_CMD_PIN_CODE_REQUEST_REPLY					(0x000D | BT_HCI_GRP_LINK_CTL_CMDS)//Reply command to a PIN code request event sent from a controller to the host.
#define BT_HCI_CMD_PIN_CODE_REQUEST_NEGATIVE_REPLY			(0x000E | BT_HCI_GRP_LINK_CTL_CMDS)//Reply command to a PIN code request event sent from the controller to the host if there is no PIN associated with the connection.
#define BT_HCI_CMD_CHANGE_CONNECTION_PACKET_TYPE			(0x000F | BT_HCI_GRP_LINK_CTL_CMDS)//Command to change the type of packets to be sent for an existing connection.
#define BT_HCI_CMD_AUTHENTICATION_REQUESTED					(0x0011 | BT_HCI_GRP_LINK_CTL_CMDS)//Command to establish authentication between two devices specified by the connection handle.
#define BT_HCI_CMD_SET_CONNECTION_ENCRYPTION				(0x0013 | BT_HCI_GRP_LINK_CTL_CMDS)//Command to enable or disable the link level encryption.
#define BT_HCI_CMD_CHANGE_CONNECTION_LINK_KEY				(0x0015 | BT_HCI_GRP_LINK_CTL_CMDS)//Command to force the change of a link key to a new one between two connected devices.
#define BT_HCI_CMD_MASTER_LINK_KEY							(0x0017 | BT_HCI_GRP_LINK_CTL_CMDS)//Command to force two devices to use the master's link key temporarily.
#define BT_HCI_CMD_REMOTE_NAME_REQUEST						(0x0019 | BT_HCI_GRP_LINK_CTL_CMDS)//Command to determine the user friendly name of the connected device.
#define BT_HCI_CMD_READ_REMOTE_SUPPORTED_FEATURES			(0x001B | BT_HCI_GRP_LINK_CTL_CMDS)//Command to determine the features supported by the connected device.
#define BT_HCI_CMD_READ_REMOTE_VERSION_INFORMATION			(0x001D | BT_HCI_GRP_LINK_CTL_CMDS)//Command to determine the version information of the connected device.
#define BT_HCI_CMD_READ_CLOCK_OFFSET						(0x001F | BT_HCI_GRP_LINK_CTL_CMDS)//Command to read the clock offset of the remote device.

/*HCI Policy Command (OGF=(0x02)*/
#define BT_HCI_CMD_HOLD_MODE								(0x0001 | BT_HCI_GRP_LINK_PLY_CMDS)//Command to place the current or remote device into the Hold mode state.
#define BT_HCI_CMD_SNIFF_MODE								(0x0003 | BT_HCI_GRP_LINK_PLY_CMDS)//Command to place the current or remote device into the Sniff mode state.
#define BT_HCI_CMD_EXIT_SNIFF_MODE							(0x0004 | BT_HCI_GRP_LINK_PLY_CMDS)//Command to exit the current or remote device from the Sniff mode state.
#define BT_HCI_CMD_PARK_MODE								(0x0005 | BT_HCI_GRP_LINK_PLY_CMDS)//Command to place the current or remote device into the Park mode state.
#define BT_HCI_CMD_EXIT_PARK_MODE							(0x0006 | BT_HCI_GRP_LINK_PLY_CMDS)//Command to exit the current or remote device from the Park mode state.
#define BT_HCI_CMD_QOS_SETUP								(0x0007 | BT_HCI_GRP_LINK_PLY_CMDS)//Command to setup the Quality of Service parameters of the device.
#define BT_HCI_CMD_ROLE_DISCOVERY							(0x0009 | BT_HCI_GRP_LINK_PLY_CMDS)//Command to determine the role of the device for a particular connection.
#define BT_HCI_CMD_SWITCH_ROLE								(0x000B | BT_HCI_GRP_LINK_PLY_CMDS)//Command to allow the device to switch roles for a particular connection.
#define BT_HCI_CMD_READ_LINK_POLICY_SETTINGS				(0x000C | BT_HCI_GRP_LINK_PLY_CMDS)//Command to determine the link policy that the LM can use to establish connections.
#define BT_HCI_CMD_WRITE_LINK_POLICY_SETTINGS				(0x000D | BT_HCI_GRP_LINK_PLY_CMDS)//Command to set the link policy that the LM can use for a particular connection.

/*Host Controller and Baseband Commands (OGF=0x03)*/
#define BT_HCI_CMD_SET_EVENT_MASK							(0x0001 | BT_HCI_GRP_HOST_CONT_BB_CMDS)//Command to set which events are generated by the HCI for the host.
#define BT_HCI_CMD_RESET									(0x0003 | BT_HCI_GRP_HOST_CONT_BB_CMDS)//Command to reset the host controller, link manager and the radio module.
#define BT_HCI_CMD_SET_EVENT_FILTER							(0x0005 | BT_HCI_GRP_HOST_CONT_BB_CMDS)//Command used by host to set the different types of event filters that the host needs to receive.
#define BT_HCI_CMD_FLUSH									(0x0008 | BT_HCI_GRP_HOST_CONT_BB_CMDS)//Command used to flush all pending data packets for transmission for a particular connection handle.
#define BT_HCI_CMD_READ_PIN_TYPE							(0x0009 | BT_HCI_GRP_HOST_CONT_BB_CMDS)//Command used by host to determine if the link manager assumes that the host requires a variable PIN type or fixed PIN code. PIN is used during pairing.
#define BT_HCI_CMD_WRITE_PIN_TYPE							(0x000A | BT_HCI_GRP_HOST_CONT_BB_CMDS)//Command used by host to write to the host controller on the PIN type supported by the host.
#define BT_HCI_CMD_CREATE_NEW_UNIT_KEY						(0x000B | BT_HCI_GRP_HOST_CONT_BB_CMDS)//Command used to create a new unit key.
#define BT_HCI_CMD_READ_STORED_LINK_KEY						(0x000D | BT_HCI_GRP_HOST_CONT_BB_CMDS)//Command to read the link key stored in the host controller.
#define BT_HCI_CMD_WRITE_STORED_LINK_KEY					(0x0011 | BT_HCI_GRP_HOST_CONT_BB_CMDS)//Command to write the link key to the host controller.
#define BT_HCI_CMD_DELETE_STORED_LINK_KEY					(0x0012 | BT_HCI_GRP_HOST_CONT_BB_CMDS)//Command to delete a stored link key in the host controller.
#define BT_HCI_CMD_CHANGE_LOCAL_NAME						(0x0013 | BT_HCI_GRP_HOST_CONT_BB_CMDS)//Command to modify the user friendly name of the device.
#define BT_HCI_CMD_READ_LOCAL_NAME							(0x0014 | BT_HCI_GRP_HOST_CONT_BB_CMDS)//Command to read the user friendly name of the device.
#define BT_HCI_CMD_READ_CONNECTION_ACCEPT_TIMEOUT			(0x0015 | BT_HCI_GRP_HOST_CONT_BB_CMDS)//Command to determine the timeout session before the host denies and rejects a new connection request.
#define BT_HCI_CMD_WRITE_CONNECTION_ACCEPT_TIMEOUT			(0x0016 | BT_HCI_GRP_HOST_CONT_BB_CMDS)//Command to set the timeout session before a device can deny or reject a connection request.
#define BT_HCI_CMD_READ_PAGE_TIMEOUT						(0x0017 | BT_HCI_GRP_HOST_CONT_BB_CMDS)//Command to read the timeout value where a device will wait for a connection acceptance before sending a connection failure is returned.
#define BT_HCI_CMD_WRITE_PAGE_TIMEOUT						(0x0018 | BT_HCI_GRP_HOST_CONT_BB_CMDS)//Command to write the timeout value where a device will wait for a connection acceptance before sending a connection failure is returned.
#define BT_HCI_CMD_READ_SCAN_ENABLE							(0x0019 | BT_HCI_GRP_HOST_CONT_BB_CMDS)//Command to read the status of the Scan_Enable configuration.
#define BT_HCI_CMD_WRITE_SCAN_ENABLE						(0x001A | BT_HCI_GRP_HOST_CONT_BB_CMDS)//Command to set the status of the Scan_Enable configuration.
#define BT_HCI_CMD_READ_PAGE_SCAN_ACTIVITY					(0x001B | BT_HCI_GRP_HOST_CONT_BB_CMDS)//Command to read the value of the Page_Scan_Interval and Page_Scan_Window configurations.
#define BT_HCI_CMD_WRITE_PAGE_SCAN_ACTIVITY					(0x001C | BT_HCI_GRP_HOST_CONT_BB_CMDS)//Command to write the value of the Page_Scan_Interval and Page_Scan_Window configurations.
#define BT_HCI_CMD_READ_INQUIRY_SCAN_ACTIVITY				(0x001D | BT_HCI_GRP_HOST_CONT_BB_CMDS)//Command to read the value of the Inquiry_Scan_Interval and Inquiry_Scan_Window configurations.
#define BT_HCI_CMD_WRITE_INQUIRY_SCAN_ACTIVITY				(0x001E | BT_HCI_GRP_HOST_CONT_BB_CMDS)//Command to set the value of the Inquiry_Scan_Interval and Inquiry_Scan_Window configurations.
#define BT_HCI_CMD_READ_AUTHENTICATION_ENABLE				(0x001F | BT_HCI_GRP_HOST_CONT_BB_CMDS)//Command to read the Authentication_Enable parameter.
#define BT_HCI_CMD_WRITE_AUTHENTICATION_ENABLE				(0x0020 | BT_HCI_GRP_HOST_CONT_BB_CMDS)//Command to set the Authentication_Enable parameter.
#define BT_HCI_CMD_READ_ENCRYPTION_MODE						(0x0021 | BT_HCI_GRP_HOST_CONT_BB_CMDS)//Command to read the Encryption_Mode parameter.
#define BT_HCI_CMD_WRITE_ENCRYPTION_MODE					(0x0022 | BT_HCI_GRP_HOST_CONT_BB_CMDS)//Command to write the Encryption_Mode parameter.
#define BT_HCI_CMD_READ_CLASS_OF_DEVICE						(0x0023 | BT_HCI_GRP_HOST_CONT_BB_CMDS)//Command to read the Class_Of_Device parameter.
#define BT_HCI_CMD_WRITE_CLASS_OF_DEVICE					(0x0024 | BT_HCI_GRP_HOST_CONT_BB_CMDS)//Command to set the Class_Of_Device parameter.
#define BT_HCI_CMD_READ_VOICE_SETTING						(0x0025 | BT_HCI_GRP_HOST_CONT_BB_CMDS)//Command to read the Voice_Setting parameter. Used for voice connections.
#define BT_HCI_CMD_WRITE_VOICE_SETTING						(0x0026 | BT_HCI_GRP_HOST_CONT_BB_CMDS)//Command to set the Voice_Setting parameter. Used for voice connections.
#define BT_HCI_CMD_READ_AUTOMATIC_FLUSH_TIMEOUT				(0x0027 | BT_HCI_GRP_HOST_CONT_BB_CMDS)//Command to read the Flush_Timeout parameter. Used for ACL connections only.
#define BT_HCI_CMD_WRITE_AUTOMATIC_FLUSH_TIMEOUT			(0x0028 | BT_HCI_GRP_HOST_CONT_BB_CMDS)//Command to set the Flush_Timeout parameter. Used for ACL connections only.
#define BT_HCI_CMD_READ_NUM_BROADCAST_RETRANSMISSIONS		(0x0029 | BT_HCI_GRP_HOST_CONT_BB_CMDS)//Command to read the number of time a broadcast message is retransmitted.
#define BT_HCI_CMD_WRITE_NUM_BROADCAST_RETRANSMISSIONS		(0x002A | BT_HCI_GRP_HOST_CONT_BB_CMDS)//Command to set the number of time a broadcast message is retransmitted.
#define BT_HCI_CMD_READ_HOLD_MODE_ACTIVITY					(0x002B | BT_HCI_GRP_HOST_CONT_BB_CMDS)//Command to set the Hold_Mode activity to instruct the device to perform an activity during hold mode.
#define BT_HCI_CMD_WRITE_HOLD_MODE_ACTIVITY					(0x002C | BT_HCI_GRP_HOST_CONT_BB_CMDS)//Command to set the Hold_Mode_Activity parameter.
#define BT_HCI_CMD_READ_TRANSMIT_POWER_LEVEL				(0x002D | BT_HCI_GRP_HOST_CONT_BB_CMDS)//Command to read the power level required for transmission for a connection handle.
#define BT_HCI_CMD_READ_SCO_FLOW_CONTROL_ENABLE				(0x002E | BT_HCI_GRP_HOST_CONT_BB_CMDS)//Command to check the current status of the flow control for the SCO connection.
#define BT_HCI_CMD_WRITE_SCO_FLOW_CONTROL_ENABLE			(0x002F | BT_HCI_GRP_HOST_CONT_BB_CMDS)//Command to set the status of the flow control for a connection handle.
#define BT_HCI_CMD_SET_HOST_CONTROLLER_TO_HOST_FLOW_CONTROL	(0x0031 | BT_HCI_GRP_HOST_CONT_BB_CMDS)//Command to set the flow control from the host controller to host in on or off state.
#define BT_HCI_CMD_HOST_BUFFER_SIZE							(0x0033 | BT_HCI_GRP_HOST_CONT_BB_CMDS)//Command set by host to inform the host controller of the buffer size of the host for ACL and SCO connections.
#define BT_HCI_CMD_HOST_NUMBER_OF_COMPLETED_PACKETS			(0x0035 | BT_HCI_GRP_HOST_CONT_BB_CMDS)//Command set from host to host controller when it is ready to receive more data packets.
#define BT_HCI_CMD_READ_LINK_SUPERVISION_TIMEOUT			(0x0036 | BT_HCI_GRP_HOST_CONT_BB_CMDS)//Command to read the timeout for monitoring link losses.
#define BT_HCI_CMD_WRITE_LINK_SUPERVISION_TIMEOUT			(0x0037 | BT_HCI_GRP_HOST_CONT_BB_CMDS)//Command to set the timeout for monitoring link losses.
#define BT_HCI_CMD_READ_NUMBER_OF_SUPPORTED_IAC				(0x0038 | BT_HCI_GRP_HOST_CONT_BB_CMDS)//Command to read the number of IACs that the device can listen on during Inquiry access.
#define BT_HCI_CMD_READ_CURRENT_IAC_LAP						(0x0039 | BT_HCI_GRP_HOST_CONT_BB_CMDS)//Command to read the LAP for the current IAC.
#define BT_HCI_CMD_WRITE_CURRENT_IAC_LAP					(0x003A | BT_HCI_GRP_HOST_CONT_BB_CMDS)//Command to set the LAP for the current IAC.
#define BT_HCI_CMD_READ_PAGE_SCAN_PERIOD_MODE				(0x003B | BT_HCI_GRP_HOST_CONT_BB_CMDS)//Command to read the timeout session of a page scan.
#define BT_HCI_CMD_WRITE_PAGE_SCAN_PERIOD_MODE				(0x003C | BT_HCI_GRP_HOST_CONT_BB_CMDS)//Command to set the timeout session of a page scan.
#define BT_HCI_CMD_READ_PAGE_SCAN_MODE						(0x003D | BT_HCI_GRP_HOST_CONT_BB_CMDS)//Command to read the default Page scan mode.
#define BT_HCI_CMD_WRITE_PAGE_SCAN_MODE						(0x003E | BT_HCI_GRP_HOST_CONT_BB_CMDS)//Command to set the default page scan mode.

#define BT_HCI_CMD_SET_AFH_CHANNELS            				(0x003F | BT_HCI_GRP_HOST_CONT_BB_CMDS)
#define BT_HCI_CMD_READ_INQSCAN_TYPE           				(0x0042 | BT_HCI_GRP_HOST_CONT_BB_CMDS)
#define BT_HCI_CMD_WRITE_INQSCAN_TYPE          				(0x0043 | BT_HCI_GRP_HOST_CONT_BB_CMDS)
#define BT_HCI_CMD_READ_INQUIRY_MODE           				(0x0044 | BT_HCI_GRP_HOST_CONT_BB_CMDS)
#define BT_HCI_CMD_WRITE_INQUIRY_MODE          				(0x0045 | BT_HCI_GRP_HOST_CONT_BB_CMDS)
#define BT_HCI_CMD_READ_PAGESCAN_TYPE          				(0x0046 | BT_HCI_GRP_HOST_CONT_BB_CMDS)
#define BT_HCI_CMD_WRITE_PAGESCAN_TYPE         				(0x0047 | BT_HCI_GRP_HOST_CONT_BB_CMDS)
#define BT_HCI_CMD_READ_AFH_ASSESSMENT_MODE    				(0x0048 | BT_HCI_GRP_HOST_CONT_BB_CMDS)
#define BT_HCI_CMD_WRITE_AFH_ASSESSMENT_MODE   				(0x0049 | BT_HCI_GRP_HOST_CONT_BB_CMDS)
#define BT_HCI_CMD_READ_EXT_INQ_RESPONSE       				(0x0051 | BT_HCI_GRP_HOST_CONT_BB_CMDS)
#define BT_HCI_CMD_WRITE_EXT_INQ_RESPONSE      				(0x0052 | BT_HCI_GRP_HOST_CONT_BB_CMDS)
#define BT_HCI_CMD_REFRESH_ENCRYPTION_KEY      				(0x0053 | BT_HCI_GRP_HOST_CONT_BB_CMDS)
#define BT_HCI_CMD_READ_SIMPLE_PAIRING_MODE    				(0x0055 | BT_HCI_GRP_HOST_CONT_BB_CMDS)
#define BT_HCI_CMD_WRITE_SIMPLE_PAIRING_MODE   				(0x0056 | BT_HCI_GRP_HOST_CONT_BB_CMDS)
#define BT_HCI_CMD_READ_LOCAL_OOB_DATA         				(0x0057 | BT_HCI_GRP_HOST_CONT_BB_CMDS)
#define BT_HCI_CMD_READ_INQ_TX_POWER_LEVEL     				(0x0058 | BT_HCI_GRP_HOST_CONT_BB_CMDS)
#define BT_HCI_CMD_WRITE_INQ_TX_POWER_LEVEL    				(0x0059 | BT_HCI_GRP_HOST_CONT_BB_CMDS)
#define BT_HCI_CMD_READ_ERRONEOUS_DATA_RPT     				(0x005A | BT_HCI_GRP_HOST_CONT_BB_CMDS)
#define BT_HCI_CMD_WRITE_ERRONEOUS_DATA_RPT    				(0x005B | BT_HCI_GRP_HOST_CONT_BB_CMDS)
#define BT_HCI_CMD_ENHANCED_FLUSH              				(0x005F | BT_HCI_GRP_HOST_CONT_BB_CMDS)
#define BT_HCI_CMD_SEND_KEYPRESS_NOTIF         				(0x0060 | BT_HCI_GRP_HOST_CONT_BB_CMDS)
#define BT_HCI_CMD_READ_LOGICAL_LINK_ACCEPT_TIMEOUT  		(0x0061 | BT_HCI_GRP_HOST_CONT_BB_CMDS)
#define BT_HCI_CMD_WRITE_LOGICAL_LINK_ACCEPT_TIMEOUT 		(0x0062 | BT_HCI_GRP_HOST_CONT_BB_CMDS)
#define BT_HCI_CMD_SET_EVENT_MASK_PAGE_2             		(0x0063 | BT_HCI_GRP_HOST_CONT_BB_CMDS)
#define BT_HCI_CMD_READ_LOCATION_DATA                		(0x0064 | BT_HCI_GRP_HOST_CONT_BB_CMDS)
#define BT_HCI_CMD_WRITE_LOCATION_DATA               		(0x0065 | BT_HCI_GRP_HOST_CONT_BB_CMDS)
#define BT_HCI_CMD_READ_FLOW_CONTROL_MODE            		(0x0066 | BT_HCI_GRP_HOST_CONT_BB_CMDS)
#define BT_HCI_CMD_WRITE_FLOW_CONTROL_MODE           		(0x0067 | BT_HCI_GRP_HOST_CONT_BB_CMDS)
#define BT_HCI_CMD_READ_BE_FLUSH_TOUT                		(0x0069 | BT_HCI_GRP_HOST_CONT_BB_CMDS)
#define BT_HCI_CMD_WRITE_BE_FLUSH_TOUT               		(0x006A | BT_HCI_GRP_HOST_CONT_BB_CMDS)
#define BT_HCI_CMD_SHORT_RANGE_MODE                  		(0x006B | BT_HCI_GRP_HOST_CONT_BB_CMDS) /* 802.11 only */
#define BT_HCI_CMD_READ_LE_SUPPORT 					 		(0x006C | BT_HCI_GRP_HOST_CONT_BB_CMDS)
#define BT_HCI_CMD_WRITE_LE_SUPPORT 				 		(0x006D | BT_HCI_GRP_HOST_CONT_BB_CMDS)

/* Commands of HCI_GRP_INFO_PARA group 4*/
#define BT_HCI_CMD_READ_LOCAL_VERSION_INFO     				(0x0001 | BT_HCI_GRP_INFO_PARA)
#define BT_HCI_CMD_READ_LOCAL_SUPPORTED_CMDS   				(0x0002 | BT_HCI_GRP_INFO_PARA)
#define BT_HCI_CMD_READ_LOCAL_FEATURES         				(0x0003 | BT_HCI_GRP_INFO_PARA)
#define BT_HCI_CMD_READ_LOCAL_EXT_FEATURES     				(0x0004 | BT_HCI_GRP_INFO_PARA)
#define BT_HCI_CMD_READ_BUFFER_SIZE            				(0x0005 | BT_HCI_GRP_INFO_PARA)
#define BT_HCI_CMD_READ_COUNTRY_CODE           				(0x0007 | BT_HCI_GRP_INFO_PARA)
#define BT_HCI_CMD_READ_BD_ADDR                				(0x0009 | BT_HCI_GRP_INFO_PARA)
#define BT_HCI_CMD_READ_DATA_BLOCK_SIZE        				(0x000A | BT_HCI_GRP_INFO_PARA)

/* Commands of HCI_GRP_STATUS_PARA group 5*/
#define BT_HCI_CMD_READ_FAILED_CONTACT_COUNT   				(0x0001 | BT_HCI_GRP_STATUS_PARA)
#define BT_HCI_CMD_RESET_FAILED_CONTACT_COUNT  				(0x0002 | BT_HCI_GRP_STATUS_PARA)
#define BT_HCI_CMD_GET_LINK_QUALITY            				(0x0003 | BT_HCI_GRP_STATUS_PARA)
#define BT_HCI_CMD_READ_RSSI                   				(0x0005 | BT_HCI_GRP_STATUS_PARA)
#define BT_HCI_CMD_READ_AFH_CH_MAP             				(0x0006 | BT_HCI_GRP_STATUS_PARA)
#define BT_HCI_CMD_READ_CLOCK                  				(0x0007 | BT_HCI_GRP_STATUS_PARA)
#define BT_HCI_CMD_READ_ENCR_KEY_SIZE          				(0x0008 | BT_HCI_GRP_STATUS_PARA)
#define BT_HCI_CMD_READ_LOCAL_AMP_INFO         				(0x0009 | BT_HCI_GRP_STATUS_PARA)
#define BT_HCI_CMD_READ_LOCAL_AMP_ASSOC        				(0x000A | BT_HCI_GRP_STATUS_PARA)
#define BT_HCI_CMD_WRITE_REMOTE_AMP_ASSOC      				(0x000B | BT_HCI_GRP_STATUS_PARA)

/* Commands of HCI_GRP_TEST_CMDS group 6*/
#define BT_HCI_CMD_READ_LOOPBACK_MODE          				(0x0001 | BT_HCI_GRP_TEST_CMDS)
#define BT_HCI_CMD_WRITE_LOOPBACK_MODE         				(0x0002 | BT_HCI_GRP_TEST_CMDS)
#define BT_HCI_CMD_ENABLE_DEV_UNDER_TEST_MODE  				(0x0003 | BT_HCI_GRP_TEST_CMDS)
#define BT_HCI_CMD_WRITE_SIMP_PAIR_DEBUG_MODE  				(0x0004 | BT_HCI_GRP_TEST_CMDS)
#define BT_HCI_CMD_ENABLE_AMP_RCVR_REPORTS     				(0x0007 | BT_HCI_GRP_TEST_CMDS)
#define BT_HCI_CMD_AMP_TEST_END                				(0x0008 | BT_HCI_GRP_TEST_CMDS)
#define BT_HCI_CMD_AMP_TEST                    				(0x0009 | BT_HCI_GRP_TEST_CMDS)

/* Commands of BLE Controller setup and configuration 8*/
#define BT_HCI_CMD_BLE_SET_EVENT_MASK          				(0x0001 | BT_HCI_GRP_BLE_CMDS)
#define BT_HCI_CMD_BLE_READ_BUFFER_SIZE        				(0x0002 | BT_HCI_GRP_BLE_CMDS)
#define BT_HCI_CMD_BLE_READ_LOCAL_SPT_FEAT     				(0x0003 | BT_HCI_GRP_BLE_CMDS)
#define BT_HCI_CMD_BLE_WRITE_LOCAL_SPT_FEAT    				(0x0004 | BT_HCI_GRP_BLE_CMDS)
#define BT_HCI_CMD_BLE_WRITE_RANDOM_ADDR       				(0x0005 | BT_HCI_GRP_BLE_CMDS)
#define BT_HCI_CMD_BLE_WRITE_ADV_PARAMS        				(0x0006 | BT_HCI_GRP_BLE_CMDS)
#define BT_HCI_CMD_BLE_READ_ADV_CHNL_TX_POWER  				(0x0007 | BT_HCI_GRP_BLE_CMDS)
#define BT_HCI_CMD_BLE_WRITE_ADV_DATA          				(0x0008 | BT_HCI_GRP_BLE_CMDS)
#define BT_HCI_CMD_BLE_WRITE_SCAN_RSP_DATA     				(0x0009 | BT_HCI_GRP_BLE_CMDS)
#define BT_HCI_CMD_BLE_WRITE_ADV_ENABLE        				(0x000A | BT_HCI_GRP_BLE_CMDS)
#define BT_HCI_CMD_BLE_WRITE_SCAN_PARAMS       				(0x000B | BT_HCI_GRP_BLE_CMDS)
#define BT_HCI_CMD_BLE_WRITE_SCAN_ENABLE       				(0x000C | BT_HCI_GRP_BLE_CMDS)
#define BT_HCI_CMD_BLE_CREATE_LL_CONN          				(0x000D | BT_HCI_GRP_BLE_CMDS)
#define BT_HCI_CMD_BLE_CREATE_CONN_CANCEL      				(0x000E | BT_HCI_GRP_BLE_CMDS)
#define BT_HCI_CMD_BLE_READ_WHITE_LIST_SIZE    				(0x000F | BT_HCI_GRP_BLE_CMDS)
#define BT_HCI_CMD_BLE_CLEAR_WHITE_LIST        				(0x0010 | BT_HCI_GRP_BLE_CMDS)
#define BT_HCI_CMD_BLE_ADD_WHITE_LIST          				(0x0011 | BT_HCI_GRP_BLE_CMDS)
#define BT_HCI_CMD_BLE_REMOVE_WHITE_LIST       				(0x0012 | BT_HCI_GRP_BLE_CMDS)
#define BT_HCI_CMD_BLE_UPD_LL_CONN_PARAMS      				(0x0013 | BT_HCI_GRP_BLE_CMDS)
#define BT_HCI_CMD_BLE_SET_HOST_CHNL_CLASS     				(0x0014 | BT_HCI_GRP_BLE_CMDS)
#define BT_HCI_CMD_BLE_READ_CHNL_MAP           				(0x0015 | BT_HCI_GRP_BLE_CMDS)
#define BT_HCI_CMD_BLE_READ_REMOTE_FEAT        				(0x0016 | BT_HCI_GRP_BLE_CMDS)
#define BT_HCI_CMD_BLE_ENCRYPT                 				(0x0017 | BT_HCI_GRP_BLE_CMDS)
#define BT_HCI_CMD_BLE_RAND                    				(0x0018 | BT_HCI_GRP_BLE_CMDS)
#define BT_HCI_CMD_BLE_START_ENC               				(0x0019 | BT_HCI_GRP_BLE_CMDS)
#define BT_HCI_CMD_BLE_LTK_REQ_REPLY           				(0x001A | BT_HCI_GRP_BLE_CMDS)
#define BT_HCI_CMD_BLE_LTK_REQ_NEG_REPLY       				(0x001B | BT_HCI_GRP_BLE_CMDS)
#define BT_HCI_CMD_BLE_READ_SUPPORTED_STATES   				(0x001C | BT_HCI_GRP_BLE_CMDS)

/*Vendor cmd for CSR8x11*/
#define BT_HCI_CMD_VENDOR_CSR8X11							0xFC00

typedef uint8_t  bt_hci_evt_code_t;

#define	BT_HCI_EVT_INQUIRY_COMPLETE							0x01	//Indicates the Inquiry has finished.
#define	BT_HCI_EVT_INQUIRY_RESULT							0x02	//Indicates that Bluetooth device(s) have responded for the inquiry.
#define	BT_HCI_EVT_CONNECTION_COMPLETE						0x03	//Indicates to both hosts that the new connection has been formed.
#define	BT_HCI_EVT_CONNECTION_REQUEST						0x04    //Indicates that a new connection is trying to be established.
#define	BT_HCI_EVT_DISCONNECTION_COMPLETE					0x05	//Occurs when a connection has been disconnected.
#define	BT_HCI_EVT_AUTHENTICATION_COMPLETE					0x06	//Occurs when an authentication has been completed.
#define	BT_HCI_EVT_REMOTE_NAME_REQUEST_COMPLETE				0x07	//Indicates that the request for the remote name has been completed.
#define	BT_HCI_EVT_ENCRYPTION_CHANGE						0x08	//Indicates that a change in the encryption has been completed.
#define	BT_HCI_EVT_CHANGE_CONNECTION_LINK_KEY_COMPLETE		0x09	//Indicates that the change in the link key has been completed.
#define	BT_HCI_EVT_MASTER_LINK_KEY_COMPLETE					0x0A	//Indicates that the change in the temporary link key or semi permanent link key on the master device is complete.
#define	BT_HCI_EVT_READ_REMOTE_SUPPORTED_FEATURES_COMPLETE	0x0B    //Indicates that the reading of the supported features on the remote device is complete.
#define	BT_HCI_EVT_READ_REMOTE_VERSION_COMPLETE				0x0C	//Indicates that the version number on the remote device has been read and completed.
#define	BT_HCI_EVT_Q0S_SETUP_COMPLETE						0x0D	//Indicates that the Quality of Service setup has been complete.
#define	BT_HCI_EVT_COMMAND_COMPLETE							0x0E	//Used by controller to send status and event parameters to the host for the particular command.
#define	BT_HCI_EVT_COMMAND_STATUS							0x0F	//Indicates that the command has been received and is being processed in the host controller.
#define	BT_HCI_EVT_HARDWARE_ERROR							0x10	//Indicates a hardware failure of the Bluetooth device.
#define	BT_HCI_EVT_FLUSH_OCCURED							0x11	//Indicates that the data has been flushed for a particular connection.
#define	BT_HCI_EVT_ROLE_CHANGE								0x12	//Indicates that the current bluetooth role for a connection has been changed.
#define	BT_HCI_EVT_NUMBER_OF_COMPLETED_PACKETS				0x13	//Indicates to the host the number of data packets sent compared to the last time the same event was sent.
#define	BT_HCI_EVT_MODE_CHANGE								0x14	//Indicates the change in mode from hold, sniff, park or active to another mode.
#define	BT_HCI_EVT_RETURN_LINK_KEYS							0x15	//Used to return stored link keys after a Read_Stored_Link_Key command was issued.
#define	BT_HCI_EVT_PIN_CODE_REQUEST							0x16	//Indicates the a PIN code is required for a new connection.
#define	BT_HCI_EVT_LINK_KEY_REQUEST							0x17	//Indicates that a link key is required for the connection.
#define	BT_HCI_EVT_LINK_KEY_NOTIFICATION					0x18	//Indicates to the host that a new link key has been created
#define BT_HCI_EVT_LOOPBACK 								0x19
#define BT_HCI_EVT_DATA_BUFFER_OVERFLOW 					0x1A
#define BT_HCI_EVT_MAX_SLOTS_CHANGE 						0x1B
#define BT_HCI_EVT_READ_CLK_OFFSET_COMPLETE 				0x1C
#define BT_HCI_EVT_CONN_PACK_TYPE_CHANGE 					0x1D
#define BT_HCI_EVT_QOS_VIOLATION 							0x1E
#define BT_HCI_EVT_PS_REPETITION_MODE_CHANGE	 			0x20
#define BT_HCI_EVT_FLOW_SPEC_COMPLETE 						0x21
#define BT_HCI_EVT_INQ_RESULT_RSSI 							0x22
#define BT_HCI_EVT_READ_REMOTE_EXT_FEATURE_COMPLETE 		0x23
#define BT_HCI_EVT_SCO_CONNECTION_COMPLETE 					0x2C
#define BT_HCI_EVT_SCO_CONN_CHANGE 							0x2D
#define BT_HCI_EVT_SNIFF_SUBRATING 						    0x2E
#define BT_HCI_EVT_EXT_INQ_RESULT 							0x2F
#define BT_HCI_EVT_ENC_KEY_REFRESH_COMPLETE 				0x30
#define BT_HCI_EVT_IO_CAP_REQ 								0x31
#define BT_HCI_EVT_IO_CAP_RESPONSE 							0x32
#define BT_HCI_EVT_USER_CONF_REQ 							0x33
#define BT_HCI_EVT_USER_PASSKEY_REQ 						0x34
#define BT_HCI_EVT_REMOTE_OOB_DATA_REQ 						0x35
#define BT_HCI_EVT_SP_COMPLETE 								0x36
#define BT_HCI_EVT_LINK_SUPERVISION_TIMEOUT_CHANGE 			0x38
#define BT_HCI_EVT_EFLUSH_COMPLETE 							0x39
#define BT_HCI_EVT_USER_PASSKEY_NOTIFICATION 				0x3B
#define BT_HCI_EVT_KEYPRESS_NOTIFICATION 					0x3C
#define BT_HCI_EVT_REMOTE_HOST_SUPPORT_FEATURE_NOTIFICATION 0x3D
#define BT_HCI_EVT_LE_META 									0x3E
#define BT_HCI_EVT_VENDOR_SPEC 								0xFF

/* LE META SUBEVENT */
#define BT_HCI_SUBEVENT_LE_CONN_COMPLETE                	0x01
#define BT_HCI_SUBEVENT_LE_ADV_REPORT                 		0x02
#define BT_HCI_SUBEVENT_LE_CONNE_UPDATE_COMPLETE         	0x03
#define BT_HCI_SUBEVENT_LE_READ_REMOTE_USED_FEATURES_COMPLETE 0x04
#define BT_HCI_SUBEVENT_LE_LONG_TERM_KEY_REQUEST            0x05
#define BT_HCI_SUBEVENT_LE_REMOTE_CONN_PARAM_REQUEST 		0x06
#define BT_HCI_SUBEVENT_LE_DATA_LENGTH_CHANGE 				0x07
#define BT_HCI_SUBEVENT_LE_READ_LOCAL_P256_PUBLIC_KEY_COMPLETE 0x08
#define BT_HCI_SUBEVENT_LE_GENERATE_DHKEY_COMPLETE          0x09
#define BT_HCI_SUBEVENT_LE_ENHANCED_CONN_COMPLETE       	0x0A
#define BT_HCI_SUBEVENT_LE_DIRECT_AD_REPORT          		0x0B

/* Success code */
#define BT_HCI_ERR_CODE_SUCCESS                                                  0x00
/* Possible error codes */
#define BT_HCI_ERR_CODE_UNKNOWN_COMMAND                                      	 0x01
#define BT_HCI_ERR_CODE_NO_CONNECTION                                            0x02
#define BT_HCI_ERR_CODE_HW_FAILURE                                               0x03
#define BT_HCI_ERR_CODE_PAGE_TIMEOUT                                             0x04
#define BT_HCI_ERR_CODE_AUTHENTICATION_FAILURE                                   0x05
#define BT_HCI_ERR_CODE_KEY_MISSING                                              0x06
#define BT_HCI_ERR_CODE_MEMORY_FULL                                              0x07
#define BT_HCI_ERR_CODE_CONN_TIMEOUT                                             0x08
#define BT_HCI_ERR_CODE_MAX_NUMBER_OF_CONNECTIONS                                0x09
#define BT_HCI_ERR_CODE_MAX_NUMBER_OF_SCO_CONNECTIONS_TO_DEVICE                  0x0A
#define BT_HCI_ERR_CODE_ACL_CONNECTION_EXISTS                                    0x0B
#define BT_HCI_ERR_CODE_COMMAND_DISSALLOWED                                      0x0C
#define BT_HCI_ERR_CODE_HOST_REJECTED_DUE_TO_LIMITED_RESOURCES                   0x0D
#define BT_HCI_ERR_CODE_HOST_REJECTED_DUE_TO_SECURITY_REASONS                    0x0E
#define BT_HCI_ERR_CODE_HOST_REJECTED_DUE_TO_REMOTE_DEVICE_ONLY_PERSONAL_SERVICE 0x0F
#define BT_HCI_ERR_CODE_HOST_TIMEOUT                                             0x10
#define BT_HCI_ERR_CODE_UNSUPPORTED_FEATURE_OR_PARAMETER_VALUE                   0x11
#define BT_HCI_ERR_CODE_INVALID_COMMAND_PARAMETERS                               0x12
#define BT_HCI_ERR_CODE_OTHER_END_TERMINATED_CONN_USER_ENDED                     0x13
#define BT_HCI_ERR_CODE_OTHER_END_TERMINATED_CONN_LOW_RESOURCES                  0x14
#define BT_HCI_ERR_CODE_OTHER_END_TERMINATED_CONN_ABOUT_TO_POWER_OFF             0x15
#define BT_HCI_ERR_CODE_CONN_TERMINATED_BY_LOCAL_HOST                            0x16
#define BT_HCI_ERR_CODE_REPETED_ATTEMPTS                                         0x17
#define BT_HCI_ERR_CODE_PAIRING_NOT_ALLOWED                                      0x18
#define BT_HCI_ERR_CODE_UNKNOWN_LMP_PDU                                          0x19
#define BT_HCI_ERR_CODE_UNSUPPORTED_REMOTE_FEATURE                               0x1A
#define BT_HCI_ERR_CODE_SCO_OFFSET_REJECTED                                      0x1B
#define BT_HCI_ERR_CODE_SCO_INTERVAL_REJECTED                                    0x1C
#define BT_HCI_ERR_CODE_SCO_AIR_MODE_REJECTED                                    0x1D
#define BT_HCI_ERR_CODE_INVALID_LMP_PARAMETERS                                   0x1E
#define BT_HCI_ERR_CODE_UNSPECIFIED_ERROR                                        0x1F
#define BT_HCI_ERR_CODE_UNSUPPORTED_LMP_PARAMETER_VALUE                          0x20
#define BT_HCI_ERR_CODE_ROLE_CHANGE_NOT_ALLOWED                                  0x21
#define BT_HCI_ERR_CODE_LMP_RESPONSE_TIMEOUT                                     0x22
#define BT_HCI_ERR_CODE_LMP_ERROR_TRANSACTION_COLLISION                          0x23
#define BT_HCI_ERR_CODE_LMP_PDU_NOT_ALLOWED                                      0x24
#define BT_HCI_ERR_CODE_ENCRYPTION_MODE_NOT_ACCEPTABLE                           0x25
#define BT_HCI_ERR_CODE_UNIT_KEY_USED                                            0x26
#define BT_HCI_ERR_CODE_QOS_NOT_SUPPORTED                                        0x27
#define BT_HCI_ERR_CODE_INSTANT_PASSED                                           0x28
#define BT_HCI_ERR_CODE_PAIRING_UNIT_KEY_NOT_SUPPORTED                           0x29
typedef uint8_t bt_hci_err_code_t;


#define BT_SCO_CONNECTION  0x00
#define BT_ACL_CONNECTION  0x01
#define BT_ESCO_CONNECTION 0x02
typedef uint8_t bt_hci_connection_link_type_t;

/*BT_HCI_EVT_CONNECTION_REQUEST*/
typedef struct {
    bt_bd_addr_t address;
    uint32_t cod: 24;
    uint32_t link_type;
} BT_PACKED bt_hci_connection_request_t;

#define BT_ROLE_MASTER  0x00
#define BT_ROLE_SLAVE   0x01
typedef uint8_t bt_role_t;

/*BT_HCI_CMD_ACCEPT_CONNECTION_REQUEST*/
typedef struct {
    bt_bd_addr_t address;
    bt_role_t role;
} BT_PACKED bt_hci_accept_connection_request_t;

#define BT_HCI_LAP_GIAC	0x009E8B33
#define BT_HCI_LAP_LIAC	0x00000000
typedef uint32_t bt_hci_lap_t;

typedef struct {
	uint16_t acl_packet_length;
	uint8_t sco_packet_length;
	uint16_t acl_credit;
	uint16_t sco_credit;
} BT_PACKED bt_hci_read_buffer_size_evt_t;

#define BT_HCI_GET_EVT_PARAM(packet, _evt_struct_)		((_evt_struct_ *)((uint8_t *)packet + BT_HCI_EVT_HEADER_SIZE))
#define BT_HCI_GET_CMD_COMPLETE_EVT_PARAM(cmd_complete) ((uint8_t *)&(cmd_complete->data))
#define BT_HCI_GET_NOCP_EVT_PARAM(nocp)					(0)

typedef struct {
    bt_hci_err_code_t status;
    uint8_t num_hci_command_packet;
    bt_hci_cmd_code_t cmd_code;
} BT_PACKED bt_hci_command_status_t;

typedef struct {
    uint8_t lenght;
} BT_PACKED bt_hci_number_of_complete_packet_t;

typedef struct {
    uint8_t num_hci_command_packet;
    bt_hci_cmd_code_t cmd_code;
    bt_hci_err_code_t status;
    bt_data_t data;
} BT_PACKED bt_hci_command_complete_t;

typedef uint16_t bt_handle_t;

typedef struct {
    bt_hci_cmd_code_t cmd_code;
    uint8_t length;
    bt_data_t data;
} BT_PACKED bt_hci_spec_cmd_t;

typedef struct {
    bt_handle_t handle;
    uint16_t length;
    bt_data_t data;
} BT_PACKED bt_hci_spec_acl_t;

typedef struct {
    bt_hci_evt_code_t evt_code;
    uint8_t length;
    bt_data_t data;
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
