#ifndef __CS_Scale_Proc_H
#define __CS_Scale_Proc_H
#include "YC_Define.h"



#define	CS_Scale_state_init				0
#define	CS_Scale_state_weighting		1
#define	CS_Scale_state_standby			2
#define	CS_Scale_state_caling			3
#define	CS_Scale_state_locking			4


typedef struct _CS_Weight_T{
	u16_t origin; 	
	u16_t kg; 	
	u16_t lb;
	u16_t st;
	u16_t jin;
}CS_Weight_T;


extern	bool	B_Debug_Test;
extern	u16_t xdata R_Debug_temp;		//调试用

extern	u8_t   xdata R_Selet_Unit;
extern	u8_t	  xdata R_Scale_state;
extern	u8_t	 xdata  R_1186sys_state;		//1186系统状态
extern	u8_t	  xdata R_Display_Err;		//显示错误信息


//外部共享变量--------------------
extern	u16_t xdata R_AD_Original;
extern	u8_t xdata B_Weight_AdOk;		//AD准备好标志
extern	u8_t xdata  R_1186_RTC[4];		//1186的RTC时间

extern	CS_Weight_T  xdata R_UnitWeight;

extern	u8_t xdata B_Weight_LowBat;		//低电标志





void CS_Scale_Proc(void);

void CS_Scale_PowerOn(void);

void CS_Scale_ChangeUnit();

//---------------------------------

#define	GetPowerOnZero				0
#define	GetRunningZero				1
#define	GetPowerOnZeroState		2
#define	GetZeroState				3
#define	ZeroProcReset				4
#define	SetPowerOnZero				5
#define	SetRunningZero				6
#define	RunningZero					7
#define	ClrLockDownCheckFlag		8
#define	GetIfZeroErr					9
#define	FindPowerOnZero			10
u16_t CS_Scale_ZeroProc(u8_t option,u16_t setdata);



#define	GetIfLowBat			1
u8_t CS_Scale_JudgeLowBat(u8_t option);

#define	GetIfOverLoad				1
#define	ResetOverLoad				2
u8_t CS_Scale_JudgeOverLoad(u8_t option);



#define	LockProcUnLock				3
u8_t CS_Scale_LockProc(u8_t option);


#define	CaliProcRun					0
#define	CaliProcSetCali				1
#define	CaliProcGetCaliDot1			2
#define	CaliProcGetCaliDot2			3
#define	CaliProcGetCaliDot3			4
#define	CaliProcReset				5
#define	CaliProcSetOpenWeight		6
#define	CaliProcGetOpenWeight		7
u16_t CS_Scale_CaliProc(u8_t option);


//超时处理函数
#define	CS_Reset_ZeroTime			1
#define	CS_Reset_LockTime			2
#define	CS_Reset_UnstableTime		3
#define	CS_Reset_OverLoadTime		4
#define	CS_Reset_LowBatTime		5
#define	CS_Reset_AllTime			6
#define	CS_SetTimeBase1S			7
void CS_Scale_TimeOutProc(u8_t option);


#define	SoftTimerTimeOut			150
#define	SoftTimerInc				0
#define	GetSoftTimerTime			1
#define	ResetSoftTimer				2
#define	GetIfTimeOut				3
u8_t CS_SoftTimer(u8_t option);



#define	StartClearMemWeightTimer	1
#define	MemWeightTimerInc			2
#define	MemWeightTimer			30
void CS_MemoryWeightClear_Timer(u8_t option);

#endif

