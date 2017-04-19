#ifndef __BT_IPC_H
#define __BT_IPC_H
#include "YC_Define.h"

#define BLE_STATUS_CONN					0x01
#define BLE_STATUS_DISCONN					0x00

#define NOTIFY_INDICATE_DISABLE		0x00
#define NOTIFY_ENABLE				0x01
#define INDICATE_ENABLE				0x02
/*lexin UUID handle*/
#define HANDLE_UUIDA620_INDICATE	0x0019
#define HANDLE_UUIDA621_NOTIFY		0x001c
#define HANDLE_UUIDA622_WO			0x001f
#define HANDLE_UUIDA623_WI			0x0021
#define HANDLE_UUIDA624_WO			0x0023
#define HANDLE_UUIDA625_NOTIFY		0x0025
#define HANDLE_UUIDA640_READ		0x0028
#define HANDLE_UUIDA641_READ		0x002a



#define HANDLE_WECHAT_WRITE		0x001f
#define HANDLE_WECHAT_INDICATE	0x0021
#define HANDLE_WECHAT_READ		0x0024

#define CS_PREFIX_CMD	0xC5
#define CS_PREFIX_EVENT	0xC6

#define CS_CMD_SLEEP_REQ					0x20
#define CS_CMD_ADV_INTERV_SET_REQ			0x22
#define CS_CMD_CONN_INTERV_SET_REQ		0x23
#define CS_CMD_TXPOWER_SET_REQ			0x28
#define CS_CMD_LE_DISC_REQ					0x29
#define CS_CMD_LE_TX_DAT					0x30
#define CS_CMD_LE_WR_DAT					0x32

#define CS_CMD_READ_FLASH					0x40
#define CS_CMD_WRITE_FLASH					0x41



#define CS_NOTIFY_POWERON			0x10	//poweron
#define CS_NOTIFY_DISC				0x11 	//data:disc reason						ok
#define CS_NOTIFY_CONN				0x12 	//no data								ok
#define CS_NOTIFY_DATA				0x13 	//data:handle(2 Byte)+data					ok
#define CS_NOTIFY_OPENHANDLE		0x14 	//data:handle+enableFlag					ok
#define CS_NOTIFY_READ_REQ			0x15 		//data:handle						ok
#define CS_NOTIFY_INDICATE_CONFIRM	0x18 	//indication confirm						ok

#define BT_CMD_BUF_LEN 0x80
#define BT_CMD_PAYLOAD_LEN 0x15
#define BT_CMD_LEN (BT_CMD_PAYLOAD_LEN+3)

#define BT_EVT_BUF_LEN 0x80
#define BT_EVT_PAYLOAD_LEN 0X15
#define BT_EVENT_LEN (BT_EVT_PAYLOAD_LEN+3)

typedef struct BT_CMDFIFO{
	u8_t len;
	u8_t DATA[BT_CMD_LEN];
}BT_CMDFIFO_t;

typedef struct BT_EVENTFIFO{
	u8_t len;
	u8_t DATA[BT_EVENT_LEN];
}BT_EVENTFIFO_t;


bool BT_CmdFifoIn(BT_CMDFIFO_t *pFIFO);
void BT_EventFifoOut(BT_EVENTFIFO_t * event);
u8_t BT_GetCmdFifoDepth(void);
u8_t BT_GetEvtFifoDepth(void);
bool BT_CmdFifoEmpty(void);
bool BT_CmdFifoFull(BT_CMDFIFO_t *pFIFO);

#endif
