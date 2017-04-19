#ifndef _LX_PROTOCOL_H_
#define _LX_PROTOCOL_H_

#include "YC_Define.h"
#include "BT_IPC.h"
#include "BTReg.h"

#define SPIFLASH_DATA_ADDR			0xa000
#define SPIFLASH_READ_LENGTH		0x4c
#define SENSOR_CALIBRATION_ADDR	0xa000
#define REGISTER_DATA_ADDR			0xb000
#define SCALE_DATA_ADDR			0xc000


#define LX_RECV_BUF				mem_module_uuid_list
#define LX_SEND_BUF				(mem_module_uuid_list+50)

#define HI_UINT8(a) 	(((a) >> 4) & 0xF)
#define LO_UINT8(a) 	((a) & 0xF)
#define HI_UINT16(a) 	(((a) >> 8) & 0xFF)
#define LO_UINT16(a) 	((a) & 0xFF)


#define DEVICE_REGISTERED						0x01
#define DEVICE_UNREGISTERED					0x00

#define FRAME_CONTROL_SINGLE					0x10
#define FRAME_CONTROL_ACK						0x00

#define TOTAL_FRAMES_SINGLE					1
#define ACK_FRAME								0
#define TOTAL_FRAMES_MAX						15
#define SUBFRAMES_ZERO							0

#define RECV_ACK_FRAME_LEN					1
#define RECV_ACK_SUCCESS						1
#define RECV_ACK_FAIL							2

#define CMD_REGISTRATION_REQ			0x0001
#define REGISTRATION_REQ_DATA_LEN		0x09
#define DEVICE_ID_UNREGISTERED			0x01
#define DEVICE_ID_REGISTERED			0x02
#define DEVICE_ID_ILLEGALED				0x03

#define CMD_REGISTER_RSP				0x0002
#define REGISTER_RSP_DATA_LEN			0x03
#define DEVICE_ID_ASSIGNED_SUCCESS	0x01
#define DEVICE_ID_ASSIGNED_FAIL			0x02
#define DEVICE_ID_LENGTH				0x06

#define CMD_BIND_REQ					0x0003
#define BIND_REQUEST_DATA_LEN			0x04

#define CMD_BIND_RSP					0x0004
#define BIND_RSP_DATA_LEN				0x03
#define BIND_SUCCESS					0x01
#define BIND_FAIL							0x02

#define CMD_LOGIN_REQ					0x0007
#define LOGIN_REQ_DATA_LEN				0x0a

#define CMD_LOGIN_RSP					0x0008
#define LOGIN_RSP_DATA_LEN				0x09
#define LOGIN_SUCCESS					0x01
#define LOGIN_FAIL						0x02

#define CMD_INIT_REQ						0x0009
#define INIT_REQ_DATA_LEN				0x03

#define CMD_INIT_RSP						0x000a
#define INIT_RSP_DATA_LEN				0x07



#define CMD_DEVICE_RSP					0x1000
#define DEVICE_RSP_DATA_LEN			0x05
#define DEVICE_RSP_SUCCESS				0x01
#define DEVICE_RSP_FAIL					0x02

#define CMD_USER_INFO_SETTING_REQ		0x1001
#define CMD_TIME_SETTING_REQ			0x1002
#define CMD_TARGET_SETTING_REQ		0x1003
#define CMD_UNIT_SETTING_REQ			0x1004
#define CMD_DATA_CLEAR_SETTING_REQ	0x1005
#define USER_INFO_RESPONSE_DATA_LEN	0x10

#define CMD_SYNC_DATA_REQ	0x4801
#define CMD_SYNC_DATA_RSP	0x4802


#define OPT_IDLE						0x00

#define OPT_REGISTER_REQ			0x10
#define OPT_REGISTER_RSP_WAIT_ACK	0x11

#define OPT_BIND_REQ				0x13
#define OPT_BIND_RSP_WAIT_ACK		0x14

#define OPT_LOGIN_REQ				0x16
#define OPT_LOGIN_REQ_WAIT_ACK		0x17
#define OPT_LOGIN_RSP				0x18
#define OPT_LOGIN_RSP_WAIT_ACK		0x19

#define OPT_INIT_REQ					0x20
#define OPT_INIT_REQ_WAIT_ACK		0x21
#define OPT_INIT_RSP					0x22
#define OPT_INIT_RSP_WAIT_ACK		0x23

#define OPT_TIME_SETTING_REQ		0x25
#define OPT_TIME_SETTING_RSP_WAIT_ACK	0x26

#define OPT_UNIT_SETTING_REQ		0x28
#define OPT_UNIT_SETTING_RSP_WAIT_ACK	0x29

#define OPT_DATA_DEL_SETTING_REQ	0x31
#define OPT_DATA_DEL_SETTING_RSP_WAIT_ACK	0x32

#define OPT_SYNC_DATA_REQ			0x35
#define OPT_SYNC_DATA_RSP			0x36
#define OPT_SYNC_DATA_RSP_WAIT_ACK	0x37



typedef struct{
	u8_t registerFlag;
	u8_t bindFlag;
	u8_t loginFlag;
	u8_t userNum;
	u8_t deviceID[6];
}DeviceInfo_t;

typedef struct{
	u16_t weight;
	u8_t UTCTime[4];
}UserMeasureData;


#define MAX_SIZE		(10)
typedef struct{
	u8_t head;
	u8_t tail;
	u8_t size;
	UserMeasureData measureData[MAX_SIZE];
}HistoryList_t;

extern void lxRecvDataFromApp(BT_EVENTFIFO_t *event);
extern void lxOptProcess(void);
void changeLxOptStatus(u8_t opt);
extern void QueuePush(UserMeasureData *userData);

extern void SpiFlashOperator(u8_t operator);


extern void QueueInit(void);
extern DeviceInfo_t xdata *pDeviceInfo;
#endif //_LX_PROTOCOL_H_
