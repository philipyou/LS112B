#ifndef __RETATION_VARIABLE_H
#define __RETATION_VARIABLE_H
#include "YC_Define.h"
#include "cs_scalekey.h"
#include "bt_ipc.h"
#include "cs_commto1186.h"
#include "cs_scale_proc.h"
#include "yc_sys.h"
#include "lxProtocol.h"

//ipc variable
extern volatile u16_t xdata BT_CmdRptr;
extern volatile u16_t xdata BT_CmdWptr;
extern volatile u16_t xdata BT_EventRptr;
extern volatile u16_t xdata BT_EventWptr;

//bt variable
extern u8_t xdata sensor_data[6];
extern DeviceInfo_t xdata deviceInfo;
extern HistoryList_t xdata historyUserData;
extern u8_t sys_state;	
extern u8_t xdata notifyValueUUIDA621;
extern u8_t xdata notifyValueUUIDA625;
extern u8_t xdata lxOptStatus;
extern u8_t xdata bleConnStatus;
/*
extern u8_t xdata prev_wechatStatus;
extern u8_t xdata wechatStatus;
extern u16_t xdata wechatSendLen;
extern u16_t xdata wechatRcvLen;
extern u16_t xdata wechatRcvTotalLen;
extern u16_t xdata wechatRcvCmd;
extern u8_t xdata wechatRcvProtoalbufBaseResponseErr;
extern u16_t xdata wechatSeq;
*/
extern BT_CMDFIFO_t btCmdBuf;
extern u8_t xdata handleIndicationConfirm;
//extern u8_t xdata userData[49];			//byte 0 is length
extern s32_t xdata UTCTime;

extern u8_t xdata R_Test;
#endif

