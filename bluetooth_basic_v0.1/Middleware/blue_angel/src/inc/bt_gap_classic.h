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

#define BT_DT_FLAGS												 0x01
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

/* Service Classes */ 																			
#define BT_SERVICE_CLASS_SERVICE_DISCOVERY_SERVER                           0x1000 /* Bluetooth Core Specification                             */
#define BT_SERVICE_CLASS_BROWSE_GROUP_DESCRIPTOR                            0x1001 /* Bluetooth Core Specification                             */
#define BT_SERVICE_CLASS_SERIAL_PORT                                        0x1101 /* Serial Port Profile (SPP) NOTE: The example SDP record in SPP v1.0 does not include a BluetoothProfileDescriptorList attribute, but some implementations may also use this UUID for the Profile Identifier.  */
#define BT_SERVICE_CLASS_LAN_ACCESS_USING_PPP                               0x1102 /* LAN Access Profile [DEPRECATED] NOTE: Used as both Service Class Identifier and Profile Identifier.                  */
#define BT_SERVICE_CLASS_DIALUP_NETWORKING                                  0x1103 /* Dial-up Networking Profile (DUN) NOTE: Used as both Service Class Identifier and Profile Identifier.                 */
#define BT_SERVICE_CLASS_IR_MC_SYNC                                         0x1104 /* Synchronization Profile (SYNC) NOTE: Used as both Service Class Identifier and Profile Identifier.                   */
#define BT_SERVICE_CLASS_OBEX_OBJECT_PUSH                                   0x1105 /* Object Push Profile (OPP) NOTE: Used as both Service Class Identifier and Profile.                                   */
#define BT_SERVICE_CLASS_OBEX_FILE_TRANSFER                                 0x1106 /* File Transfer Profile (FTP) NOTE: Used as both Service Class Identifier and Profile Identifier.                      */
#define BT_SERVICE_CLASS_IR_MC_SYNC_COMMAND                                 0x1107 /* Synchronization Profile (SYNC)                                                                                       */
#define BT_SERVICE_CLASS_HEADSET                                            0x1108 /* Headset Profile (HSP) NOTE: Used as both Service Class Identifier and Profile Identifier.                            */
#define BT_SERVICE_CLASS_CORDLESS_TELEPHONY                                 0x1109 /* Cordless Telephony Profile (CTP) NOTE: Used as both Service Class Identifier and Profile Identifier. [DEPRECATED]    */
#define BT_SERVICE_CLASS_AUDIO_SOURCE                                       0x110A /* Advanced Audio Distribution Profile (A2DP)                                                                           */
#define BT_SERVICE_CLASS_AUDIO_SINK                                         0x110B /* Advanced Audio Distribution Profile (A2DP)                                                                           */
#define BT_SERVICE_CLASS_AV_REMOTE_CONTROL_TARGET                           0x110C /* Audio/Video Remote Control Profile (AVRCP)                                                                           */
#define BT_SERVICE_CLASS_ADVANCED_AUDIO_DISTRIBUTION                        0x110D /* Advanced Audio Distribution Profile (A2DP)                                                                           */
#define BT_SERVICE_CLASS_AV_REMOTE_CONTROL                                  0x110E /* Audio/Video Remote Control Profile (AVRCP) NOTE: Used as both Service Class Identifier and Profile Identifier.       */
#define BT_SERVICE_CLASS_AV_REMOTE_CONTROL_CONTROLLER                       0x110F /* Audio/Video Remote Control Profile (AVRCP) NOTE: The AVRCP specification v1.3 and later require that 0x110E also be included in the ServiceClassIDList before 0x110F for backwards compatibility. */
#define BT_SERVICE_CLASS_INTERCOM                                           0x1110 /* Intercom Profile (ICP) NOTE: Used as both Service Class Identifier and Profile Identifier. [DEPRECATED]                    */
#define BT_SERVICE_CLASS_FAX                                                0x1111 /* Fax Profile (FAX) NOTE: Used as both Service Class Identifier and Profile Identifier. [DEPRECATED]                         */
#define BT_SERVICE_CLASS_HEADSET_AUDIO_GATEWAY_AG                           0x1112 /* Headset Profile (HSP)                                                                                                      */
#define BT_SERVICE_CLASS_WAP                                                0x1113 /* Interoperability Requirements for Bluetooth technology as a WAP, Bluetooth SIG [DEPRECATED]                                */
#define BT_SERVICE_CLASS_WAP_CLIENT                                         0x1114 /* Interoperability Requirements for Bluetooth technology as a WAP, Bluetooth SIG [DEPRECATED]                                */
#define BT_SERVICE_CLASS_PANU                                               0x1115 /* Personal Area Networking Profile (PAN) NOTE: Used as both Service Class Identifier and Profile Identifier for PANU role.   */
#define BT_SERVICE_CLASS_NAP                                                0x1116 /* Personal Area Networking Profile (PAN) NOTE: Used as both Service Class Identifier and Profile Identifier for NAP role.    */
#define BT_SERVICE_CLASS_GN                                                 0x1117 /* Personal Area Networking Profile (PAN) NOTE: Used as both Service Class Identifier and Profile Identifier for GN role.     */
#define BT_SERVICE_CLASS_DIRECT_PRINTING                                    0x1118 /* Basic Printing Profile (BPP)                                                                                               */
#define BT_SERVICE_CLASS_REFERENCE_PRINTING                                 0x1119 /* See Basic Printing Profile (BPP)                                                                                           */
#define BT_SERVICE_CLASS_BASIC_IMAGING_PROFILE                              0x111A /* Basic Imaging Profile (BIP)                                                                                                */
#define BT_SERVICE_CLASS_IMAGING_RESPONDER                                  0x111B /* Basic Imaging Profile (BIP)                                                                                                */
#define BT_SERVICE_CLASS_IMAGING_AUTOMATIC_ARCHIVE                          0x111C /* Basic Imaging Profile (BIP)                                                                                                */
#define BT_SERVICE_CLASS_IMAGING_REFERENCED_OBJECTS                         0x111D /* Basic Imaging Profile (BIP)                                                                                                */
#define BT_SERVICE_CLASS_HANDSFREE                                          0x111E /* Hands-Free Profile (HFP) NOTE: Used as both Service Class Identifier and Profile Identifier.                               */
#define BT_SERVICE_CLASS_HANDSFREE_AUDIO_GATEWAY                            0x111F /* Hands-free Profile (HFP)                                                                                                   */
#define BT_SERVICE_CLASS_DIRECT_PRINTING_REFERENCE_OBJECTS_SERVICE          0x1120 /* Basic Printing Profile (BPP)                                                                                               */
#define BT_SERVICE_CLASS_REFLECTED_UI                                       0x1121 /* Basic Printing Profile (BPP)                                                                                               */
#define BT_SERVICE_CLASS_BASIC_PRINTING                                     0x1122 /* Basic Printing Profile (BPP)                                                                                               */
#define BT_SERVICE_CLASS_PRINTING_STATUS                                    0x1123 /* Basic Printing Profile (BPP)                                                                                               */
#define BT_SERVICE_CLASS_HUMAN_INTERFACE_DEVICE_SERVICE                     0x1124 /* Human Interface Device (HID) NOTE: Used as both Service Class Identifier and Profile Identifier.                           */
#define BT_SERVICE_CLASS_HARDCOPY_CABLE_REPLACEMENT                         0x1125 /* Hardcopy Cable Replacement Profile (HCRP)                                                                                  */
#define BT_SERVICE_CLASS_HCR_PRINT                                          0x1126 /* Hardcopy Cable Replacement Profile (HCRP)                                                                                  */
#define BT_SERVICE_CLASS_HCR_SCAN                                           0x1127 /* Hardcopy Cable Replacement Profile (HCRP)                                                                                  */
#define BT_SERVICE_CLASS_COMMON_ISDN_ACCESS                                 0x1128 /* Common ISDN Access Profile (CIP) NOTE: Used as both Service Class Identifier and Profile Identifier. [DEPRECATED]          */
#define BT_SERVICE_CLASS_SIM_ACCESS                                         0x112D /* SIM Access Profile (SAP) NOTE: Used as both Service Class Identifier and Profile Identifier.                               */
#define BT_SERVICE_CLASS_PHONEBOOK_ACCESS_PCE                               0x112E /* Phonebook Access Profile (PBAP)                                                                                            */
#define BT_SERVICE_CLASS_PHONEBOOK_ACCESS_PSE                               0x112F /* Phonebook Access Profile (PBAP)                                                                                            */
#define BT_SERVICE_CLASS_PHONEBOOK_ACCESS                                   0x1130 /* Phonebook Access Profile (PBAP)                                                                                            */
#define BT_SERVICE_CLASS_HEADSET_HS                                         0x1131 /* Headset Profile (HSP) NOTE: See erratum #3507. 0x1108 and 0x1203 should also be included in the ServiceClassIDList before 0x1131 for backwards compatibility. */
#define BT_SERVICE_CLASS_MESSAGE_ACCESS_SERVER                              0x1132 /* Message Access Profile (MAP)                                                                   */
#define BT_SERVICE_CLASS_MESSAGE_NOTIFICATION_SERVER                        0x1133 /* Message Access Profile (MAP)                                                                   */
#define BT_SERVICE_CLASS_MESSAGE_ACCESS_PROFILE                             0x1134 /* Message Access Profile (MAP)                                                                   */
#define BT_SERVICE_CLASS_GNSS                                               0x1135 /* Global Navigation Satellite System Profile (GNSS)                                              */
#define BT_SERVICE_CLASS_GNSS_SERVER                                        0x1136 /* Global Navigation Satellite System Profile (GNSS)                                              */
#define BT_SERVICE_CLASS_3D_DISPLAY                                         0x1137 /* 3D Synchronization Profile (3DSP)                                                              */
#define BT_SERVICE_CLASS_3D_GLASSES                                         0x1138 /* 3D Synchronization Profile (3DSP)                                                              */
#define BT_SERVICE_CLASS_3D_SYNCHRONIZATION                                 0x1139 /* 3D Synchronization Profile (3DSP)                                                              */
#define BT_SERVICE_CLASS_MPS_PROFILE_UUID                                   0x113A /* Multi-Profile Specification (MPS)                                                              */
#define BT_SERVICE_CLASS_MPS_SC_UUID                                        0x113B /* Multi-Profile Specification (MPS)                                                              */
#define BT_SERVICE_CLASS_CTN_ACCESS_SERVICE                                 0x113C /* Calendar, Task, andNotes (CTN)Profile                                                          */
#define BT_SERVICE_CLASS_CTN_NOTIFICATION_SERVICE                           0x113D /* CalendarTasksandNotes (CTN)Profile                                                             */
#define BT_SERVICE_CLASS_CTN_PROFILE                                        0x113E /* CalendarTasksandNotes (CTN)Profile                                                             */
#define BT_SERVICE_CLASS_PNP_INFORMATION                                    0x1200 /* Device Identification (DID) NOTE: Used as both Service Class Identifier and Profile Identifier.*/
#define BT_SERVICE_CLASS_GENERIC_NETWORKING                                 0x1201 /* N/A                                                                                            */
#define BT_SERVICE_CLASS_GENERIC_FILE_TRANSFER                              0x1202 /* N/A                                                                                            */
#define BT_SERVICE_CLASS_GENERIC_AUDIO                                      0x1203 /* N/A                                                                                            */
#define BT_SERVICE_CLASS_GENERIC_TELEPHONY                                  0x1204 /* N/A                                                                                            */
#define BT_SERVICE_CLASS_UPNP_SERVICE                                       0x1205 /* Enhanced Service Discovery Profile (ESDP) [DEPRECATED]                                         */
#define BT_SERVICE_CLASS_UPNP_IP_SERVICE                                    0x1206 /* Enhanced Service Discovery Profile (ESDP) [DEPRECATED]                                         */
#define BT_SERVICE_CLASS_ESDP_UPNP_IP_PAN                                   0x1300 /* Enhanced Service Discovery Profile (ESDP) [DEPRECATED]                                         */
#define BT_SERVICE_CLASS_ESDP_UPNP_IP_LAP                                   0x1301 /* Enhanced Service Discovery Profile (ESDP)[DEPRECATED]                                          */
#define BT_SERVICE_CLASS_ESDP_UPNP_L2CAP                                    0x1302 /* Enhanced Service Discovery Profile (ESDP)[DEPRECATED]                                          */
#define BT_SERVICE_CLASS_VIDEO_SOURCE                                       0x1303 /* Video Distribution Profile (VDP)                                                               */
#define BT_SERVICE_CLASS_VIDEO_SINK                                         0x1304 /* Video Distribution Profile (VDP)                                                               */
#define BT_SERVICE_CLASS_VIDEO_DISTRIBUTION                                 0x1305 /* Video Distribution Profile (VDP)                                                               */
#define BT_SERVICE_CLASS_HDP                                                0x1400 /* Health Device Profile                                                                          */
#define BT_SERVICE_CLASS_HDP_SOURCE                                         0x1401 /* Health Device Profile (HDP)                                                                    */
#define BT_SERVICE_CLASS_HDP_SINK                                           0x1402 /* Health Device Profile (HDP)  																	 */

#endif//__BT_CLASSIC_H__
