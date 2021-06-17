/******************************************************************************
  * @file           bt_log.c
  * @author         leon
  * @version        V0.1
  * @date           2021-05-19
  * @brief
******************************************************************************/
#include "bt_log.h"
#include "bsp.h"
#include "bt_memory.h"
#include "bt_hci_spec.h"
#include "bt_config.h"

void bt_hci_log(uint8_t in_out, uint8_t *log, uint16_t log_length)
{
#if BT_HCI_LOG_ENABLE
#if BT_HCI_LOG_MIX_SYSTEM_LOG
    return;
#endif
    bt_hci_log_type_t type;
    uint16_t data_tatal_length = 0;
    uint16_t index = 0, i = 0;
    uint8_t *buf = NULL;
    uint8_t check_sum = 0;

    if (in_out == 0) {
        if (log[0] == BT_UART_CMD) {
            type = BT_HCI_LOG_CMD;
        } else if (log[0] == BT_UART_ACL) {
            type = BT_HCI_LOG_ACL_OUT;
        }
    } else if (in_out == 1) {
        if (log[0] == BT_UART_ACL) {
            type = BT_HCI_LOG_ACL_IN;
        } else if (log[0] == BT_UART_EVT) {
            type = BT_HCI_LOG_EVT;
        }
    }
    log++;
    log_length--;
    data_tatal_length = BT_HCI_LOG_HEADER_LEGNTH + log_length + 1;//1:check sum
    buf = (uint8_t *)bt_memory_allocate_packet(BT_MEMORY_TX, data_tatal_length);
    BT_ASSERT(buf);

    buf[index++] = 0xF5;
    buf[index++] = 0x5A;
    buf[index++] = type;
    buf[index++] = log_length & 0xFF;
    buf[index++] = (log_length >> 8) & 0xFF;
    for (i = 0; i < log_length; index++, i++) {
        buf[index] = log[i];
    }
    for (i = 0; i < data_tatal_length - 1; i++) {
        check_sum += buf[i];
    }
    buf[index] = check_sum;

    comSendBuf(COM3, buf, data_tatal_length);

    bt_memory_free_packet(BT_MEMORY_TX, buf);
#endif
}
