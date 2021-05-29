/******************************************************************************
  * @file           bt_driver.h
  * @author         Leonard-y.He(1027901556@qq.com)
  * @version        V0.1
  * @date           2021-05-17
  * @brief
******************************************************************************/
#ifndef __BT_DRIVER__
#define __BT_DRIVER__
#include "bt_type.h"

#define BT_DRIVER_LOG_INFO(format, ...)              \
{									            \
	if (BT_DRIVER_LOG_LEVEL >= BT_SYS_LOG_INFO) \
		BT_SYSTEM_LOG(format,##__VA_ARGS__);                  \
}											    \

#define BT_DRIVER_LOG_DEBUG(format, ...)              \
{									            \
	if (BT_DRIVER_LOG_LEVEL >= BT_SYS_LOG_DEBUG)\
		BT_SYSTEM_LOG(format,##__VA_ARGS__);                  \
}	

#define BT_DRIVER_LOG_WARNING(format, ...)              \
{									            \
	if (BT_DRIVER_LOG_LEVEL >= BT_SYS_LOG_WARNING) \
		BT_SYSTEM_LOG(format,##__VA_ARGS__);                  \
}	

#define BT_DRIVER_LOG_ERROR(format, ...)              \
{									            \
	if (BT_DRIVER_LOG_LEVEL >= BT_SYS_LOG_ERROR)\
		BT_SYSTEM_LOG(format,##__VA_ARGS__);                  \
}

typedef enum {
	BT_DRIVER_WAIT_4_INDICATOR,
	BT_DRIVER_WAIT_4_HEADER,
	BT_DRIVER_WAIT_4_PAYLOAD,
	BT_DRIVER_RX_COMPLETE
} bt_driver_rx_state_t;

#define BT_CONTROLLER_HARDWARE_RESET_PIN_PERIPH	RCC_APB2Periph_GPIOA
#define BT_CONTROLLER_HARDWARE_RESET_PIN_GROUP	GPIOA	
#define BT_CONTROLLER_HARDWARE_RESET_PIN 		GPIO_Pin_7	

void bt_driver_share_buffer_lock(void);

void bt_driver_share_buffer_unlock(void);

void bt_driver_hardware_reset_controller(void);

void bt_driver_init(void);

void bt_driver_deinit(void);

void bt_driver_recieve_data_from_controller(uint8_t data);

void bt_driver_send_data_to_controller(uint8_t *buf, uint16_t buf_size);
#endif//__BT_DRIVER__
