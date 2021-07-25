/******************************************************************************
  * @file           bt_l2cap.h
  * @author         leon
  * @version        V0.1
  * @date           2021-05-19
  * @brief
******************************************************************************/
#ifndef __BT_L2CAP_H__
#define __BT_L2CAP_H__
#include "bt_type.h"
#include "bt_hci_spec.h"
#include "bt_timer.h"

#define BT_L2CAP_LOG_INFO(format, ...)               \
    {									             \
        if (BT_L2CAP_LOG_LEVEL >= BT_SYS_LOG_INFO)   \
            BT_SYSTEM_LOG(format,##__VA_ARGS__);     \
    }											     \

#define BT_L2CAP_LOG_DEBUG(format, ...)              \
    {									             \
        if (BT_L2CAP_LOG_LEVEL >= BT_SYS_LOG_DEBUG)  \
            BT_SYSTEM_LOG(format,##__VA_ARGS__);     \
    }

#define BT_L2CAP_LOG_WARNING(format, ...)            \
    {									             \
        if (BT_L2CAP_LOG_LEVEL >= BT_SYS_LOG_WARNING)\
            BT_SYSTEM_LOG(format,##__VA_ARGS__);     \
    }

#define BT_L2CAP_LOG_ERROR(format, ...)              \
    {									             \
        if (BT_L2CAP_LOG_LEVEL >= BT_SYS_LOG_ERROR)  \
            BT_SYSTEM_LOG(format,##__VA_ARGS__);     \
    }

/* L2CAP defaults */
#define BT_L2CAP_DEFAULT_MTU			       672
#define BT_L2CAP_DEFAULT_FLUSH_TO		       0xFFFF

/* UPPER LAYER PSM : https://www.bluetooth.com/specifications/assigned-numbers/ */
#define BT_SDP_PSM 				    	       0x0001
typedef uint16_t bt_l2cap_psm_t;

/* L2CAP command codes */
#define BT_L2CAP_COMMAND_REJ	    		   0x01
#define BT_L2CAP_CONN_REQ		    		   0x02
#define BT_L2CAP_CONN_RSP		    		   0x03
#define BT_L2CAP_CONFIG_REQ		    		   0x04
#define BT_L2CAP_CONFIG_RSP		    		   0x05
#define BT_L2CAP_DISCONN_REQ	    		   0x06
#define BT_L2CAP_DISCONN_RSP	    		   0x07
#define BT_L2CAP_ECHO_REQ		    		   0x08
#define BT_L2CAP_ECHO_RSP		    		   0x09
#define BT_L2CAP_INFO_REQ		    		   0x0A
#define BT_L2CAP_INFO_RSP		    		   0x0B
#define BT_L2CAP_CREATE_CHANNEL_REQ 		   0x0C
#define BT_L2CAP_CREATE_CHANNEL_RSP 		   0x0D
#define BT_L2CAP_MOVE_CHANNEL_REQ   		   0x0E
#define BT_L2CAP_MOVE_CHANNEL_RSP   		   0x0F
#define BT_L2CAP_MOVE_CHANNEL_CONFIRMATION_REQ 0x10
#define BT_L2CAP_MOVE_CHANNEL_CONFIRMATION_RSP 0x11
#define BT_L2CAP_CONN_PARAM_UPDATE_REQ 		   0x12
#define BT_L2CAP_CONN_PARAM_UPDATE_RSP 		   0x13
#define BT_L2CAP_LE_CREDIT_BASED_CONN_REQ      0x14
#define BT_L2CAP_LE_CREDIT_BASED_CONN_RSP      0x15
#define BT_L2CAP_FLOW_CONTROL_CREDIT_IND   	   0x16
#define BT_L2CAP_CREDIT_BASED_CONN_REQ 		   0x17
#define BT_L2CAP_CREDIT_BASED_CONN_RSP         0x18
#define BT_L2CAP_CREDIT_BASED_RECONFIGURE_REQ  0x19
#define BT_L2CAP_CREDIT_BASED_RECONFIGURE_RSP  0x1A
typedef uint8_t bt_l2cap_command_code_t;

/* Permanent channel identifiers */
#define BT_L2CAP_NULL_CID 		 			   0x0000
#define BT_L2CAP_SIG_CID 		 			   0x0001
#define BT_L2CAP_CONNLESS_CID 	 			   0x0002
#define BT_L2CAP_AMP_MANAGER_CID 			   0x0003
#define BT_L2CAP_ATT_CID 		 			   0x0004
#define BT_L2CAP_L2_SIG_CID 	 			   0x0005
#define BT_L2CAP_SM_CID 		 			   0x0006
#define BT_L2CAP_BREDR_SM_CID    			   0x0007
#define BT_L2CAP_DYNAMIC_CID_START         	   0x0040
#define BT_L2CAP_DYNAMIC_CID_END	           0xFFFF

/* connect result */
#define BT_L2CAP_CR_SUCCESS	        		   0x0000
#define BT_L2CAP_CR_PEND		    		   0x0001
#define BT_L2CAP_CR_NOSUP_PSM	    		   0x0002
#define BT_L2CAP_CR_SEC_BLOCK	    		   0x0003
#define BT_L2CAP_CR_NO_RSR		    		   0x0004
typedef uint16_t bt_l2cap_connect_result_t;

/* config result */
#define BT_L2CAP_CONF_SUCCESS	               0x0000
#define BT_L2CAP_CONF_UNACCEPT	               0x0001
#define BT_L2CAP_CONF_REJECT	               0x0002
#define BT_L2CAP_CONF_UNKNOWN	               0x0003
typedef uint16_t bt_l2cap_config_result_t;

/* config option */
#define BT_L2CAP_CONF_MTU		               0x01
#define BT_L2CAP_CONF_FLUSH_TO	               0x02
#define BT_L2CAP_CONF_QOS		               0x03
#define BT_L2CAP_CONF_RFC		               0x04
#define BT_L2CAP_CONF_RFC_MODE	               0x04
typedef uint8_t bt_l2cap_config_option_t;


#define BT_L2CAP_PDU_HEADER_SIZE	sizeof(bt_l2cap_pdu_header_t)

/* L2CAP structures hdr*/
typedef struct {
    uint16_t	len;
    uint16_t	cid;
} BT_PACKED bt_l2cap_pdu_header_t;

typedef struct {
    bt_l2cap_pdu_header_t pdu_hdr;
    bt_l2cap_command_code_t	code;
    uint8_t		ident; //用于标示command 的发送序列，response 必须跟request 相同
    uint16_t	len;
} BT_PACKED bt_l2cap_sig_header_t;

/* l2cap structures detail*/
typedef struct {
    bt_l2cap_sig_header_t sig_hdr;
    bt_l2cap_psm_t	psm;
    uint16_t	scid; //本地发起：本地的CID 远端发起：远端的CID
} BT_PACKED bt_l2cap_con_req_t;


#define BT_L2CAP_CON_RSP_RESULT_SUCCESS							0x0000
#define BT_L2CAP_CON_RSP_RESULT_PENDING							0x0001
#define BT_L2CAP_CON_RSP_RESULT_REFUSED_PSM_NOT_SUPPORT			0x0002
#define BT_L2CAP_CON_RSP_RESULT_REFUSED_SECURITY_BLOCK			0x0003
#define BT_L2CAP_CON_RSP_RESULT_REFUSED_NO_RESOURCES_AVAILABLE	0x0004
#define BT_L2CAP_CON_RSP_RESULT_REFUSED_INVALID_SCID			0x0006
#define BT_L2CAP_CON_RSP_RESULT_REFUSED_SCID_ALREADY_ALLOCATED	0x0007
typedef uint16_t bt_l2cap_con_rsp_result_t;

#define BT_L2CAP_CON_RSP_STATUS_NO_FURTHRE_INFO_AVAILABLE		0x0000
#define BT_L2CAP_CON_RSP_STATUS_AUTHENTICATION_PENDING			0x0001
#define BT_L2CAP_CON_RSP_STATUS_AUTHORIZATION_PENDING			0x0002
typedef uint16_t bt_l2cap_con_rsp_status_t;

typedef struct {
    bt_l2cap_sig_header_t sig_hdr;
    uint16_t	dst_cid; //可以认为是发送response 的本地CID
    uint16_t	src_cid; //发起L2CAP_CONNECTION_REQ 的CID
    bt_l2cap_con_rsp_result_t	result;
    bt_l2cap_con_rsp_status_t	status; //only valid when result = BT_L2CAP_CON_RSP_RESULT_PENDING
} BT_PACKED bt_l2cap_con_rsp_t;

typedef struct {
    bt_l2cap_sig_header_t sig_hdr;
    uint16_t	dst_cid;
    uint16_t	flags;
    bt_data_t	data;
} BT_PACKED bt_l2cap_config_req_t;

typedef struct {
    bt_l2cap_sig_header_t sig_hdr;
    uint16_t	src_cid;
    uint16_t	flags;
    uint16_t	result;
    bt_data_t	data;
} BT_PACKED bt_l2cap_config_rsp_t;

typedef struct {
    bt_l2cap_pdu_header_t pdu_hdr;
    bt_data_t	data;
} BT_PACKED bt_l2cap_upper_layer_data_t;

typedef struct {
    bt_l2cap_sig_header_t com_hdr;
    uint16_t	dcid;
    uint16_t	scid;
} BT_PACKED bt_l2cap_discon_req_t;

typedef struct {
    bt_l2cap_sig_header_t com_hdr;
    uint16_t	dcid;
    uint16_t	scid;
} BT_PACKED bt_l2cap_discon_rsp_t;

#define BT_L2CAP_REJECT_COMMAND_NOT_UNDERSTAND 0x0000
#define BT_L2CAP_REJECT_SIG_MTU_EXCEEDED	   0x0001
#define BT_L2CAP_REJECT_INVALID_CID_IN_REQUEST 0x0002
typedef uint16_t bt_l2cap_reject_reason_t;

typedef struct {
    bt_l2cap_sig_header_t com_hdr;
    bt_l2cap_reject_reason_t reason;
    bt_data_t	data;
} BT_PACKED bt_l2cap_command_reject_rsp_t;

/* Extended features mask bits
*/
#define BT_L2CAP_EXTFEA_FC             0x00000001    /* Flow Control Mode   (Not Supported)    */
#define BT_L2CAP_EXTFEA_RTRANS         0x00000002    /* Retransmission Mode (Not Supported)    */
#define BT_L2CAP_EXTFEA_QOS            0x00000004
#define BT_L2CAP_EXTFEA_ENH_RETRANS    0x00000008    /* Enhanced retransmission mode           */
#define BT_L2CAP_EXTFEA_STREAM_MODE    0x00000010    /* Streaming Mode                         */
#define BT_L2CAP_EXTFEA_NO_CRC         0x00000020    /* Optional FCS (if set No FCS desired)   */
#define BT_L2CAP_EXTFEA_EXT_FLOW_SPEC  0x00000040    /* Extended flow spec                     */
#define BT_L2CAP_EXTFEA_FIXED_CHNLS    0x00000080    /* Fixed channels                         */
#define BT_L2CAP_EXTFEA_EXT_WINDOW     0x00000100    /* Extended Window Size                   */
#define BT_L2CAP_EXTFEA_UCD_RECEPTION  0x00000200    /* Unicast Connectionless Data Reception  */
#define BT_L2CAP_EXTFEA_ENH_CREDIT_BASE_FC 0x00000400 /* Enhanced Credit Based Flow Control Mode */
#define BT_L2CAP_EXTFEA_SUPPORTED_MASK (BT_L2CAP_EXTFEA_ENH_RETRANS |\
                                        BT_L2CAP_EXTFEA_NO_CRC      |\
                                        BT_L2CAP_EXTFEA_FIXED_CHNLS |\
                                        BT_L2CAP_EXTFEA_FIXED_CHNLS |\
                                        BT_L2CAP_EXTFEA_UCD_RECEPTION)


/* Info type */
#define BT_L2CAP_CONLESS_MTU          0x0001
#define BT_L2CAP_EXFEATURE_SUPPORT    0x0002
#define BT_L2CAP_FIXED_CHNL_SUPPORT   0x0003
typedef uint16_t bt_l2cap_info_type_t;

typedef struct {
    bt_l2cap_sig_header_t sig_hdr;
    bt_l2cap_info_type_t info_type;
} BT_PACKED bt_l2cap_info_req_t;

typedef struct {
    bt_l2cap_sig_header_t sig_hdr;
    bt_l2cap_info_type_t info_type;
} BT_PACKED bt_l2cap_info_rsp_t;

typedef struct {
    bt_l2cap_sig_header_t sig_hdr;
    bt_data_t	data;
} BT_PACKED bt_l2cap_sig_packet_t;

typedef struct {
    bt_l2cap_pdu_header_t pdu_hdr;
    bt_data_t	data;
} BT_PACKED bt_l2cap_packet_t;

/* API */
/* send pdu */
bt_status_t bt_l2cap_send_sig_con_req(uint8_t ident, uint16_t psm, uint16_t scid);
bt_status_t bt_l2cap_send_sig_conf_req(uint8_t ident, uint16_t dcid, uint16_t flags, uint8_t *conf_opt, uint8_t opt_len);
bt_status_t bt_l2cap_send_upper_layer_data(uint16_t cid, uint8_t *data, uint16_t length);

bt_status_t bt_l2cap_send_sig_discon_req(uint8_t ident, uint16_t dcid, uint16_t scid);
bt_status_t bt_l2cap_send_pdu(uint8_t *pdu, uint32_t length);

/* reve pdu */
bt_status_t bt_l2cap_parse_pdu(uint8_t *pdu, uint32_t length);
bt_status_t bt_l2cap_parse_sig_con_rsp_pdu(uint8_t *pdu, uint32_t length);
bt_status_t bt_l2cap_parse_sig_conf_rsp_pdu(uint8_t *pdu, uint32_t length);
bt_status_t bt_l2cap_parse_sig_discon_rsp_pdu(uint8_t *pdu, uint32_t length);

bt_status_t bt_l2cap_rx_process(bt_hci_spec_packet_t *hci_packet);

#endif//__BT_L2CAP_H__
