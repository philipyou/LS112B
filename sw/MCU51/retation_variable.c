#include "hwreg.h"
#include "btreg.h"
#include "retation_variable.h"

#define RETATION_BASE 0x4C00

//ipc variable
volatile u16_t xdata BT_CmdRptr _at_  mem_cmd_rptr;
volatile u16_t xdata BT_CmdWptr _at_ mem_cmd_wptr;
volatile u16_t xdata BT_EventRptr _at_ mem_event_rptr;
volatile u16_t xdata BT_EventWptr _at_ mem_event_wptr;

//bt variable
//spiflash_data:6bytes sensor data; 10bytes register data; 60:userdata
u8_t xdata sensor_data[6] _at_(RETATION_BASE+0);
DeviceInfo_t xdata deviceInfo _at_(RETATION_BASE+6);
HistoryList_t xdata historyUserData _at_(RETATION_BASE+16);
u8_t sys_state;
u8_t xdata notifyValueUUIDA621 _at_ (RETATION_BASE+89);
u8_t xdata notifyValueUUIDA625 _at_ (RETATION_BASE+90);
u8_t xdata bleConnStatus _at_(RETATION_BASE+91);
u8_t xdata lxOptStatus _at_ (RETATION_BASE+92);
u8_t xdata handleIndicationConfirm _at_ (RETATION_BASE+93);
s32_t xdata UTCTime _at_ (RETATION_BASE+94);
u8_t xdata R_Test _at_ (RETATION_BASE+98);

/*
u8_t xdata prev_wechatStatus _at_ (RETATION_BASE+1);
u8_t xdata wechatStatus _at_ (RETATION_BASE+2);
u16_t xdata wechatSendLen _at_ (RETATION_BASE+3);
u16_t xdata wechatRcvLen _at_ (RETATION_BASE+5);
u16_t xdata wechatRcvTotalLen _at_ (RETATION_BASE+7);
u16_t xdata wechatRcvCmd _at_ (RETATION_BASE+9);
u8_t xdata wechatRcvProtoalbufBaseResponseErr _at_ (RETATION_BASE+11);
u16_t xdata wechatSeq _at_ (RETATION_BASE+12);
u8_t xdata userData[49] _at_ (RETATION_BASE+55);			//byte 0 is length
*/

BT_CMDFIFO_t btCmdBuf _at_ (RETATION_BASE+99);









