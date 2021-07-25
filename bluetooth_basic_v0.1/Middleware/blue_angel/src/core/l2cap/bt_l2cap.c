/******************************************************************************
  * @file           bt_l2cap.c
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
#include "bt_l2cap_internal.h"
#include "bt_l2cap.h"

static bt_status_t bt_l2cap_sig_channel_handler(bt_l2cap_sig_packet_t *sig_packet);
static bt_status_t bt_l2cap_dynamic_channel_handler(bt_l2cap_packet_t *l2cap_packet);

bt_status_t bt_l2cap_rx_process(bt_hci_spec_packet_t *hci_packet)
{
	bt_l2cap_packet_t *l2cap_packet;
	bt_status_t status;
	BT_ASSERT(hci_packet);
	l2cap_packet = (bt_l2cap_packet_t *)((uint8_t *)hci_packet + BT_HCI_ACL_HEADER_SIZE);
	switch (l2cap_packet->pdu_hdr.cid) {
		case BT_L2CAP_NULL_CID:
			BT_ASSERT(0);
			break;
		case BT_L2CAP_SIG_CID:
			status = bt_l2cap_sig_channel_handler((bt_l2cap_sig_packet_t *)l2cap_packet);
			break;
		default:
			if (l2cap_packet->pdu_hdr.cid >= BT_L2CAP_DYNAMIC_CID_START && 
				l2cap_packet->pdu_hdr.cid <= BT_L2CAP_DYNAMIC_CID_END) {
					status = bt_l2cap_dynamic_channel_handler(l2cap_packet);
				}
			break;
	}
	return status;
}

static bt_status_t bt_l2cap_sig_channel_handler(bt_l2cap_sig_packet_t *sig_packet)
{
	bt_status_t status;
	switch (sig_packet->sig_hdr.code) {
		case BT_L2CAP_COMMAND_REJ:
			break;
		case BT_L2CAP_CONN_REQ:
			break;
		case BT_L2CAP_CONN_RSP:
			break;
		case BT_L2CAP_INFO_REQ:
			break;
		case BT_L2CAP_INFO_RSP:
			break;
		case BT_L2CAP_CREATE_CHANNEL_REQ:
			break;
		case BT_L2CAP_CREATE_CHANNEL_RSP:
			break;
		default:
			break;
	}
	return status;
}

static bt_status_t bt_l2cap_dynamic_channel_handler(bt_l2cap_packet_t *l2cap_packet)
{
	bt_status_t status;
	return status;
}

#if 0 
int L2CAP_Send_SIG_Con_Req(uint8_t ident,uint16_t psm,uint16_t scid)
{
	L2CAP_Con_Req PDU;
	PDU.com_hdr.sig_hdr.cid = L2CAP_SIG_CH;
	PDU.com_hdr.sig_hdr.len = sizeof(L2CAP_Con_Req) - sizeof(L2CAP_PDU_HDR_Format);
 
	PDU.com_hdr.code = L2CAP_CONN_REQ;
	PDU.com_hdr.ident = ident;
	PDU.com_hdr.len = sizeof(L2CAP_Con_Req) - sizeof(L2CAP_SIG_HDR_Format);
 
	PDU.psm = psm;
	PDU.scid = scid;
	L2CAP_Send_PDU((uint8_t *)&PDU,sizeof(L2CAP_Con_Req));
}
int L2CAP_Send_SIG_Conf_Req(uint8_t ident,uint16_t dcid,uint16_t flags,uint8_t *conf_opt,uint8_t opt_len)
{
	L2CAP_Conf_Req *PDU = (L2CAP_Conf_Req *)malloc(sizeof(L2CAP_Conf_Req) + opt_len);
	(PDU->com_hdr).sig_hdr.cid = L2CAP_SIG_CH;
	(PDU->com_hdr).sig_hdr.len = sizeof(L2CAP_Conf_Req) - sizeof(L2CAP_PDU_HDR_Format) + opt_len;
	
	(PDU->com_hdr).code = L2CAP_CONF_REQ;
	(PDU->com_hdr).ident = ident;
	(PDU->com_hdr).len = sizeof(L2CAP_Conf_Req) - sizeof(L2CAP_SIG_HDR_Format) + opt_len;
	
	PDU->dst_cid = dcid;
	PDU->flags = flags;
	memcpy(PDU->data,conf_opt,opt_len);
 
	L2CAP_Send_PDU(PDU,sizeof(L2CAP_Conf_Req) + opt_len);
}
 
int L2CAP_Send_Upper_Layer_data(uint16_t cid,uint8_t *data,uint16_t length)
{
	L2CAP_Upper_Layer_data_Format *PDU = (L2CAP_Upper_Layer_data_Format *)malloc(sizeof(L2CAP_Upper_Layer_data_Format) + length);
	(PDU->com_hdr).cid = cid;
	(PDU->com_hdr).len = sizeof(L2CAP_Upper_Layer_data_Format) + length;
 
	memcpy(PDU->data,data,length);	
	L2CAP_Send_PDU(PDU,sizeof(L2CAP_Upper_Layer_data_Format) + length);
}
 
int L2CAP_Send_SIG_Discon_Req(uint8_t ident,uint16_t dcid,uint16_t scid)
{
	L2CAP_Discon_req PDU;
	PDU.com_hdr.sig_hdr.cid = L2CAP_SIG_CH;
	PDU.com_hdr.sig_hdr.len = sizeof(L2CAP_Discon_req) - sizeof(L2CAP_PDU_HDR_Format);
	
	PDU.com_hdr.code = L2CAP_DISCONN_REQ;
	PDU.com_hdr.ident = ident;
	PDU.com_hdr.len = sizeof(L2CAP_Con_Req) - sizeof(L2CAP_SIG_HDR_Format);
	
	PDU.dcid = dcid;
	PDU.scid = scid;
	L2CAP_Send_PDU((uint8_t *)&PDU,sizeof(L2CAP_Con_Req));
}
int L2CAP_Send_PDU(uint8_t *PDU,uint32_t length)
{
	int index = 0;
	BT_L2CAP_DEBUG((" >> Send L2CAP REQ PDU:"));
	for(index = 0; index < length; index++)
	{
		BT_L2CAP_DEBUG(("0x%02x ",PDU[index]));
	}
	BT_L2CAP_DEBUG(("\n"));
}
 
 
int L2CAP_Parse_PDU(uint8_t *PDU,uint32_t length)
{
	L2CAP_PDU_HDR_Format *TMP_PDU = (L2CAP_PDU_HDR_Format *)PDU;
	if((TMP_PDU->cid) == L2CAP_SIG_CH)
	{
		uint8_t tmp_code;
		L2CAP_SIG_HDR_Format *pdu = (L2CAP_SIG_HDR_Format *)PDU;
		BT_L2CAP_DEBUG(("SIG PDU\n"));		
		tmp_code = pdu->code;
		switch(tmp_code)
		{
			case L2CAP_CONN_RSP:
			{
				BT_L2CAP_DEBUG(("L2CAP_CONN_RSP\n"));
				L2CAP_Parse_SIG_Con_Rsp_PDU(PDU,length);
				break;
			}
			case L2CAP_CONF_RSP:
			{
				BT_L2CAP_DEBUG(("L2CAP_CONF_RSP\n"));
				L2CAP_Parse_SIG_Conf_Rsp_PDU(PDU,length);
				break;
			}
			case L2CAP_DISCONN_RSP:
			{
				BT_L2CAP_DEBUG(("L2CAP_DISCONN_RSP\n"));
				L2CAP_Parse_SIG_Discon_Rsp_PDU(PDU,length);
				break;
			}
			default:
			{
				break;
			}
		}
		
	}
	else
	{
		//TODO
	}
}
 
int L2CAP_Parse_SIG_Con_Rsp_PDU(uint8_t *PDU,uint32_t length)
{
	L2CAP_Con_Rsp *tmp_pdu = (L2CAP_Con_Rsp *)PDU;
	if(tmp_pdu->result == L2CAP_CR_SUCCESS)
	{
		BT_L2CAP_DEBUG(("connect successful\n"));
	}
	else if(tmp_pdu->result == L2CAP_CR_PEND)
	{
		BT_L2CAP_DEBUG(("connect pending\n"));
	}
}
 
int L2CAP_Parse_SIG_Conf_Rsp_PDU(uint8_t *PDU,uint32_t length)
{
	L2CAP_Conf_Rsp *tmp_pdu = (L2CAP_Conf_Rsp *)PDU;
	if(tmp_pdu->result == L2CAP_CONF_SUCCESS)
	{
		BT_L2CAP_DEBUG(("config successful\n"));
	}
	else if(tmp_pdu->result == L2CAP_CONF_UNACCEPT)
	{
		BT_L2CAP_DEBUG(("config unaccept\n"));
	}
}
 
int L2CAP_Parse_SIG_Discon_Rsp_PDU(uint8_t *PDU,uint32_t length)
{
	
}
#endif
