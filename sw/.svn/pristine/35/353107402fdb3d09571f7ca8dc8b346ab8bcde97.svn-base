#ifndef __WECHAT_H
#define __WECHAT_H

#define WECHAT_RCV_BUF				mem_module_uuid_list
#define WECHAT_TX_BUF				(mem_module_uuid_list+128)

#define REQ_VENDOR_INFO						0xC0
#define REQ_SEND_DATA							0xC3
#define REQ_SEND_WR_CODE_OK					0xC8
#define REQ_SEND_PUSH_USER_INFO_CONFIRM		0xCB
#define REQ_SEND_DEV_INFO						0xCC

#define RES_REQUEST_VENDOR_INFO				0xC0
#define RES_SEND_DATA							0xC3
#define RES_SEND_WR_CODE_OK					0xC8
#define RES_SEND_PUSH_USER_INFO_CONFIRM		0xCB
#define RES_SEND_DEV_INFO						0xCC

#define  PUSH_USER_INFO							0xD1

#define WECHAT_ST_IDLE								0x00
#define WECHAT_ST_AUTH_REQ						0x11
#define WECHAT_ST_AUTH_REQ_WAIT					0x21
#define WECHAT_ST_AUTH_RESPONSE_WAIT			0x31
#define WECHAT_ST_INIT_REQ							0x12
#define WECHAT_ST_INIT_REQ_WAIT					0x22
#define WECHAT_ST_INIT_RESPONSE_WAIT				0x32
#define WECHAT_ST_MANUFACTURE_REQ				0x13
#define WECHAT_ST_MANUFACTURE_REQ_WAIT			0x23
#define WECHAT_ST_MANUFACTURE_RESPONSE_WAIT	0x33
#define WECHAT_ST_SENDDATA_REQ					0x14
#define WECHAT_ST_SENDDATA_REQ_WAIT				0x24
#define WECHAT_ST_SENDDATA_RESPONSE_WAIT		0x34


#define WECHAT_ST_OK								0x40
#define WECHAT_ST_FAIL								0x50

#define WECHAT_ECI_REQ_AUTH					10001
#define WECHAT_ECI_REQ_SENDDATA				10002
#define WECHAT_ECI_REQ_INIT						10003
#define WECHAT_ECI_RESP_AUTH					20001
#define WECHAT_ECI_RESP_SENDDATA				20002
#define WECHAT_ECI_RESP_INIT					20003
#define WECHAT_ECI_PUSH_RECVDATA				30001
#define WECHAT_ECI_PUSH_SWITCHVIEW			30002
#define WECHAT_ECI_PUSH_SWITCHBACKGROUD	30003
#define WECHAT_ERR_DECODE						29999

#define PROTOBUF_VARINT_TYPE					0
#define PROTOBUF_LEN_DELIMITED_TYPE			2

extern u8_t xdata prev_wechatStatus;
extern u8_t xdata wechatStatus;
extern u16_t xdata wechatSendLen;
extern u16_t xdata wechatRcvLen;
extern u16_t xdata wechatRcvTotalLen;
extern u16_t xdata wechatRcvCmd;
extern u8_t xdata wechatRcvProtoalbufBaseResponseErr;
extern u16_t xdata wechatSeq;
extern BT_CMDFIFO_t btCmdBuf;
extern u8_t xdata handleIndicationConfirm;
extern u8_t xdata userData[49];			//byte 0 is length
extern s32_t xdata UTCTime;

bool asciiToHex(u8_t ascii[2],  u8_t *hex);
bool hexToAscii(u8_t hex, u8_t ascii[2]);
u8_t decodeProtobuf(u8_t xdata*protobuf, u8_t protobufLen, u8_t decodeIndex,u8_t xdata *bufout, u8_t xdata *bufoutLen);
void WechatProcess(void);
void sendMeasureData(void);

#endif


