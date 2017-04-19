#ifndef __CS_CommTo1186_H
#define __CS_CommTo1186_H

#define	CS_CommTo1186_Null				0
#define	CS_CommTo1186_Reset				0xc5
#define	CS_CommTo1186_ReadAd			0x80
#define	CS_CommTo1186_ReadVersion		0x81
#define	CS_CommTo1186_ReadTime			0x82
#define	CS_CommTo1186_ReadRam			0x83
#define	CS_CommTo1186_ReadOtp			0x84
#define	CS_CommTo1186_ReadAdZero		0x85
#define	CS_CommTo1186_SetSleepMode		0xA0
#define	CS_CommTo1186_SetOpenWeight		0xA1
#define	CS_CommTo1186_SetTime			0xA2
#define	CS_CommTo1186_SetRam			0xA3
#define	CS_CommTo1186_LcdDisplay			0xE0

#define	CS_CommTo1186_ReadAdStandby				1
#define	CS_CommTo1186_ReadVersionStandby		2
#define	CS_CommTo1186_ReadTimeStandby			3
#define	CS_CommTo1186_ReadRamStandby			4
#define	CS_CommTo1186_ReadOtpStandby			5
#define	CS_CommTo1186_ReadAdZeroStandby		6
#define	CS_CommTo1186_SetSleepModeStandby		7
#define	CS_CommTo1186_SetOpenWeightStandby		8
#define	CS_CommTo1186_SetTimeStandby			9
#define	CS_CommTo1186_SetRamStandby				10
#define	CS_CommTo1186_LcdDisplayStandby			11
			
#define 	UART_EVENT_OK						0x10

#define	CS_CommTo1186_TimeOut			21



typedef struct _CS_ComState{
	u8_t	now; 	
	u8_t	pre;
	u8_t sucess;
}CS_ComState;

extern CS_ComState	xdata R_Weight_Com_Coo;	//协助通信处理的变量
extern u8_t uartTimeoutCnt;	//timeunit is 10ms


void CS_Scale_1186Com(void);
void CS_Scale_1186ComSend(u8_t com_comand);
u8_t CS_If1186ComSucess(void);

void CS_1186Com_ReadMacAdress_Proc(void);
void CS_1186Com_ReadAdc_Proc(void);
void CS_1186Com_SetLcd_Proc(void);
void CS_1186Com_ReadTime_Proc(void);
void CS_1186Com_SetSleepMode_Proc(void);
void CS_1186Com_SetOpenWeight_Proc(void);
void CS_1186Com_ReadAdZero_Proc(void);
void CS_1186Com_Reset_Proc(void);


void CS_CommTo1186_SendCmd(u8_t cmd_code,u8_t* databuf);



#endif


