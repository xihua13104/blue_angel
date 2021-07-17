/******************************************************************************
  * @file           bt_gap_classic.h
  * @author         leon
  * @version        V0.1
  * @date           2021-05-19
  * @brief
******************************************************************************/
#ifndef __BT_CLASSIC_H__
#define __BT_CLASSIC_H__
#include "bt_type.h"
#include "bt_hci_spec.h"
#include "bt_timer.h"

typedef struct {
	uint8_t fec_required;
	uint8_t eir[240];
} BT_PACKED bt_gap_write_eir_t;

#define BT_GAP_SCAN_MODE_NOT_ACCESSIABLE 	0
#define BT_GAP_SCAN_MODE_DISCOVERABLE_ONLY	1
#define BT_GAP_SCAN_MODE_CONNECTABLE_ONLY	2
#define BT_GAP_SCAN_MODE_GENERAL_ACCESSABLE	3
typedef uint8_t bt_gap_scan_mode_t;

typedef struct {
	bt_linknode_t next;
	bt_bd_addr_t address;
	bt_hci_connection_link_type_t link_type;
} bt_gap_classic_connection_t;

#define BT_DT_FLAGS											     0x01
#define BT_DT_INCOMPLETE_LIST_OF_16_BIT_SERVICE_CLASS_UUIDS      0x02
#define BT_DT_COMPLETE_LIST_OF_16_BIT_SERVICE_CLASS_UUIDS        0x03
#define BT_DT_INCOMPLETE_LIST_OF_32_BIT_SERVICE_CLASS_UUIDS      0x04
#define BT_DT_COMPLETE_LIST_OF_32_BIT_SERVICE_CLASS_UUIDS        0x05
#define BT_DT_INCOMPLETE_LIST_OF_128_BIT_SERVICE_CLASS_UUIDS     0x06
#define BT_DT_COMPLETE_LIST_OF_128_BIT_SERVICE_CLASS_UUIDS       0x07
#define BT_DT_SHORTENED_LOCAL_NAME                               0x08
#define BT_DT_COMPLETE_LOCAL_NAME                                0x09
#define BT_DT_TX_POWER_LEVEL                                     0x0A
#define BT_DT_CLASS_OF_DEVICE                                    0x0D
#define BT_DT_SIMPLE_PAIRING_HASH_C                              0x0E
#define BT_DT_SIMPLE_PAIRING_HASH_C_192                          0x0E
#define BT_DT_SIMPLE_PAIRING_RANDOMIZER_R                        0x0F
#define BT_DT_SIMPLE_PAIRING_RANDOMIZER_R_192                    0x0F
#define BT_DT_DEVICE_ID                                          0x10
#define BT_DT_SECURITY_MANAGER_TK_VALUE                          0x10
#define BT_DT_SECURITY_MANAGER_OUT_OF_BAND_FLAGS                 0x11
#define BT_DT_SLAVE_CONNECTION_INTERVAL_RANGE                    0x12
#define BT_DT_LIST_OF_16_BIT_SERVICE_SOLICITATION_UUIDS          0x14
#define BT_DT_LIST_OF_32_BIT_SERVICE_SOLICITATION_UUIDS          0x1F
#define BT_DT_LIST_OF_128_BIT_SERVICE_SOLICITATION_UUIDS         0x15
#define BT_DT_SERVICE_DATA                                       0x16
#define BT_DT_SERVICE_DATA_16_BIT_UUID                           0x16
#define BT_DT_SERVICE_DATA_32_BIT_UUID                           0x20
#define BT_DT_SERVICE_DATA_128_BIT_UUID                          0x21
#define BT_DT_LE_SECURE_CONNECTIONS_CONFIRMATION_VALUE           0x22
#define BT_DT_LE_SECURE_CONNECTIONS_RANDOM_VALUE                 0x23
#define BT_DT_URI                                                0x24
#define BT_DT_INDOOR_POSITIONING                                 0x25
#define BT_DT_TRANSPORT_DISCOVERY_DATA                           0x26
#define BT_DT_PUBLIC_TARGET_ADDRESS                              0x17
#define BT_DT_RANDOM_TARGET_ADDRESS                              0x18
#define BT_DT_APPEARANCE                                         0x19
#define BT_DT_ADVERTISING_INTERVAL                               0x1A
#define BT_DT_LE_BLUETOOTH_DEVICE_ADDRESS                        0x1B
#define BT_DT_LE_ROLE                                            0x1C
#define BT_DT_SIMPLE_PAIRING_HASH_C_256                          0x1D
#define BT_DT_SIMPLE_PAIRING_RANDOMIZER_R_256                    0x1E
#define BT_DT_3D_INFORMATION_DATA                                0x3D
#define BT_DT_MANUFACTURER_SPECIFIC_DATA                         0xFF

#endif//__BT_CLASSIC_H__
