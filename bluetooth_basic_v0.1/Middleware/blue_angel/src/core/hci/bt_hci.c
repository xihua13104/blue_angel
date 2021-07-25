/******************************************************************************
  * @file           bt_hci.c
  * @author         leon
  * @version        V0.1
  * @date           2021-05-19
  * @brief
******************************************************************************/
#include "bt_hci.h"
#include "bt_hci_spec.h"
#include "bt_common.h"
#include "bt_log.h"
#include "bt_platform.h"
#include "bt_driver.h"
#include "bt_gap_internal.h"
#include "bt_l2cap.h"

bt_status_t bt_hci_cmd_send(bt_hci_cmd_t cmd, uint32_t data, uint32_t timeout, bt_hci_timeout_callback_t callback)
{
    bt_status_t status = BT_STATUS_SUCCESS;
    bt_hci_spec_packet_t *packet = NULL;
    uint16_t length = cmd.length + BT_HCI_CMD_HEADER_SIZE;
    uint32_t timer_id = cmd.cmd_code | BT_MODULE_HCI | BT_HCI_TIMER_MASK_A;

    BT_HCI_LOG_INFO("[BT_HCI] cmd send, cmd_code = 0x%x \r\n", cmd.cmd_code);
    packet = (bt_hci_spec_packet_t *)BT_ALLOCATE_HCI_PACKET_WITH_NODE(BT_MEMORY_TX, length);
    if (packet == NULL) {
        BT_HCI_LOG_ERROR("[BT_HCI] cmd send fail, OOM \r\n");
        return BT_STATUS_OUT_OF_MEMORY;
    }
    packet->indicator = BT_UART_CMD;
    packet->value.cmd.cmd_code = cmd.cmd_code;
    packet->value.cmd.length = cmd.length;
    if (cmd.length > 0) {
        bt_memcpy((void *)&packet->value.cmd.data[0], cmd.param, cmd.length);
    }
    if (callback) {
        if (bt_timer_find(timer_id)) {
            BT_HCI_LOG_ERROR("[BT_HCI] cmd send fail, timer already exist\r\n");
            BT_FREE_HCI_PACKET_WITH_NODE(BT_MEMORY_TX, packet);
            return BT_STATUS_TIMER_EXIST;
        }
        status = bt_timer_start(timer_id, timeout, data, callback);
        BT_ASSERT(status == BT_STATUS_SUCCESS);
    }
    bt_hci_log(0, (uint8_t *)packet, BT_HCI_CMD_SIZE(packet));
    bt_driver_send_data_to_controller((uint8_t *)packet, BT_HCI_CMD_SIZE(packet));
    BT_FREE_HCI_PACKET_WITH_NODE(BT_MEMORY_TX, packet);
    return status;
}

bt_status_t bt_hci_evt_handler(bt_hci_spec_packet_t *packet)
{
    bt_status_t status = BT_STATUS_SUCCESS;
    bt_hci_evt_code_t evt_code = packet->value.evt.evt_code;
    uint32_t timer_id = BT_MODULE_HCI | BT_HCI_TIMER_MASK_A;
    switch (evt_code) {
        case BT_HCI_EVT_COMMAND_COMPLETE:
            timer_id |= BT_HCI_GET_EVT_PARAM(packet, bt_hci_command_complete_t)->cmd_code;
            break;
        case BT_HCI_EVT_COMMAND_STATUS:
            status = BT_HCI_GET_EVT_PARAM(packet, bt_hci_command_status_t)->status;
            return status;
        case BT_HCI_EVT_NUMBER_OF_COMPLETED_PACKETS:
            break;
        case BT_HCI_EVT_LE_META:
            break;
        case BT_HCI_EVT_VENDOR_SPEC:
            timer_id |= BT_HCI_CMD_VENDOR_CSR8X11;
            break;
        default:
            timer_id |= BT_HCI_TIMER_MASK_B | evt_code;
            break;
    }
    status = bt_timer_cancel_and_callback(timer_id, (void *)packet);
    if (BT_STATUS_TIMER_NOT_FOUND == status) {
        status = bt_gap_evt_handler(timer_id, (void *)packet);
    }
    return status;
}

bt_status_t bt_hci_acl_handler(bt_hci_spec_packet_t *packet)
{
    return bt_l2cap_rx_process(packet);
}

void bt_hci_packet_process()
{
    bt_hci_spec_packet_t *packet = NULL;

    while (!BT_QUEUE_IS_EMPTY(BT_RX_QUEUE)) {
        packet = (bt_hci_spec_packet_t *)BT_GET_HCI_SPEC_PACKET_FROM_NODE(BT_POP_NODE_FROM_RX_QUEUE_FRONT());
        if (packet->indicator == BT_UART_ACL) {
            bt_hci_log(1, (uint8_t *)packet, BT_HCI_ACL_SIZE(packet));
            bt_hci_acl_handler(packet);
        } else if (packet->indicator == BT_UART_EVT) {
            bt_hci_log(1, (uint8_t *)packet, BT_HCI_EVT_SIZE(packet));
            bt_hci_evt_handler(packet);
        }
        BT_FREE_HCI_PACKET_WITH_NODE(BT_MEMORY_RX, packet);
    }
}

const char *bt_hci_get_error_code(uint8_t error_code)
{
    switch (error_code) {
        case BT_HCI_ERR_CODE_SUCCESS:
            return ("Success");
        case BT_HCI_ERR_CODE_UNKNOWN_COMMAND:
            return ("Unknown HCI Command");
        case BT_HCI_ERR_CODE_NO_CONNECTION:
            return ("No Connection");
        case BT_HCI_ERR_CODE_HW_FAILURE:
            return ("Hardware Failure");
        case BT_HCI_ERR_CODE_PAGE_TIMEOUT:
            return ("Page Timeout");
        case BT_HCI_ERR_CODE_AUTHENTICATION_FAILURE:
            return ("Authentication Failure");
        case BT_HCI_ERR_CODE_KEY_MISSING:
            return ("Key Missing");
        case BT_HCI_ERR_CODE_MEMORY_FULL:
            return ("Memory Full");
        case BT_HCI_ERR_CODE_CONN_TIMEOUT:
            return ("Connection Timeout");
        case BT_HCI_ERR_CODE_MAX_NUMBER_OF_CONNECTIONS:
            return ("Max Number Of Connections");
        case BT_HCI_ERR_CODE_MAX_NUMBER_OF_SCO_CONNECTIONS_TO_DEVICE:
            return ("Max Number Of SCO Connections To A Device");
        case BT_HCI_ERR_CODE_ACL_CONNECTION_EXISTS:
            return ("ACL connection already exists");
        case BT_HCI_ERR_CODE_COMMAND_DISSALLOWED:
            return ("Command Disallowed");
        case BT_HCI_ERR_CODE_HOST_REJECTED_DUE_TO_LIMITED_RESOURCES:
            return ("Host Rejected due to limited resources");
        case BT_HCI_ERR_CODE_HOST_REJECTED_DUE_TO_SECURITY_REASONS:
            return ("Host Rejected due to security reasons");
        case BT_HCI_ERR_CODE_HOST_REJECTED_DUE_TO_REMOTE_DEVICE_ONLY_PERSONAL_SERVICE:
            return ("Host Rejected due to remote device is only a personal device");
        case BT_HCI_ERR_CODE_HOST_TIMEOUT:
            return ("Host Timeout");
        case BT_HCI_ERR_CODE_UNSUPPORTED_FEATURE_OR_PARAMETER_VALUE:
            return ("Unsupported Feature or Parameter Value");
        case BT_HCI_ERR_CODE_INVALID_COMMAND_PARAMETERS:
            return ("Invalid HCI Command Parameters");
        case BT_HCI_ERR_CODE_OTHER_END_TERMINATED_CONN_USER_ENDED:
            return ("Other End Terminated Connection: User Ended Connection");
        case BT_HCI_ERR_CODE_OTHER_END_TERMINATED_CONN_LOW_RESOURCES:
            return ("Other End Terminated Connection: Low Resources");
        case BT_HCI_ERR_CODE_OTHER_END_TERMINATED_CONN_ABOUT_TO_POWER_OFF:
            return ("Other End Terminated Connection: About to Power Off");
        case BT_HCI_ERR_CODE_CONN_TERMINATED_BY_LOCAL_HOST:
            return ("Connection Terminated by Local Host");
        case BT_HCI_ERR_CODE_REPETED_ATTEMPTS:
            return ("Repeated Attempts");
        case BT_HCI_ERR_CODE_PAIRING_NOT_ALLOWED:
            return ("Pairing Not Allowed");
        case BT_HCI_ERR_CODE_UNKNOWN_LMP_PDU:
            return ("Unknown LMP PDU");
        case BT_HCI_ERR_CODE_UNSUPPORTED_REMOTE_FEATURE:
            return ("Unsupported Remote Feature");
        case BT_HCI_ERR_CODE_SCO_OFFSET_REJECTED:
            return ("SCO Offset Rejected");
        case BT_HCI_ERR_CODE_SCO_INTERVAL_REJECTED:
            return ("SCO Interval Rejected");
        case BT_HCI_ERR_CODE_SCO_AIR_MODE_REJECTED:
            return ("SCO Air Mode Rejected");
        case BT_HCI_ERR_CODE_INVALID_LMP_PARAMETERS:
            return ("Invalid LMP Parameters");
        case BT_HCI_ERR_CODE_UNSPECIFIED_ERROR:
            return ("Unspecified Error");
        case BT_HCI_ERR_CODE_UNSUPPORTED_LMP_PARAMETER_VALUE:
            return ("Unsupported LMP Parameter Value");
        case BT_HCI_ERR_CODE_ROLE_CHANGE_NOT_ALLOWED:
            return ("Role Change Not Allowed");
        case BT_HCI_ERR_CODE_LMP_RESPONSE_TIMEOUT:
            return ("LMP Response Timeout");
        case BT_HCI_ERR_CODE_LMP_ERROR_TRANSACTION_COLLISION:
            return ("LMP Error Transaction Collision");
        case BT_HCI_ERR_CODE_LMP_PDU_NOT_ALLOWED:
            return ("LMP PDU Not Allowed");
        case BT_HCI_ERR_CODE_ENCRYPTION_MODE_NOT_ACCEPTABLE:
            return ("Encryption Mode Not Acceptable");
        case BT_HCI_ERR_CODE_UNIT_KEY_USED:
            return ("Unit Key Used");
        case BT_HCI_ERR_CODE_QOS_NOT_SUPPORTED:
            return ("QoS is Not Supported");
        case BT_HCI_ERR_CODE_INSTANT_PASSED:
            return ("Instant Passed");
        case BT_HCI_ERR_CODE_PAIRING_UNIT_KEY_NOT_SUPPORTED:
            return ("Pairing with Unit Key Not Supported");
        default:
            return ("Error code unknown");
    }
}

