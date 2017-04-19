#include "YC_Define.h"
#include "YC_Peripheral.h"
#include "system.h"
#include "CS_Scale_Proc.h"
#include "CS_CommTo1186.h"
#include "CS_ScaleDisplay.h"
#include "btreg.h"
#include "BT_IPC.h"
#include "lxProtocol.h"

#include "retation_variable.h"
#include "YC_Utilities.h"
#include "hwreg.h"



bool	B_Debug_Test;				//调试模式
u16_t xdata R_Debug_temp;		//调试用

u8_t   xdata R_Selet_Unit;			//单位选择状态
u8_t	  xdata R_Scale_state;		//秤的状态
u8_t	  xdata R_Display_Err;		//显示错误信息
	

//全局变量--------------------
u16_t xdata  R_AD_Original;		//原始AD值
u8_t	  xdata  R_1186sys_state;		//1186系统状态
u8_t   xdata  B_Weight_AdOk;		//AD准备好标志


u8_t xdata  R_1186_RTC[4];			//1186的RTC时间


u16_t xdata R_Weight_Mem;		//记忆重量
u16_t xdata R_Weight_Lock;		//锁定重量
CS_Weight_T  xdata R_UnitWeight;	//重量值



//////////////////////////////////////////////系统参数

#define	CS_Scale_FiterX					0x05		//滑动滤波范围5个AD内码
#define	CS_Scale_MaxWeight				15300		//最大称量重量153.00kg
#define	CS_Scale_MinLockWeight			250			//最小锁定重量2.50kg
#define	CS_Scale_RetLockWeightInc		1000		//加重解锁重量10kg
#define	CS_Scale_RetLockWeightDec		300			//减重解锁重量3kg
#define	CS_Scale_SameWeightRange		20			//判断是同一个重量的阈值20个AD内码
#define	CS_Scale_TimeOut_Zero			15			//零位超时关机时间15s
#define	CS_Scale_TimeOut_Lock			15			//锁定超时关机时间15s
#define	CS_Scale_TimeOut_Unstable		15			//不稳定关机时间15s
#define	CS_Scale_TimeOut_OverLoad		3			//超重关机时间3s
#define	CS_Scale_TimeOut_LowBat		3			//低电关机时间3s
#define	SameZeroRange					10			//认为是同一个零点的误差范围
#define	ScaleWeightMin					10			//秤体重量左边界值
#define	ScaleWeightMax					15			//秤体重量右边界值
#define	ScaleStartMemoryWeight			600			//秤开始记忆重量范围
#define	CS_CaliStateData_Up				6000			//标定数据有效值上限(对应AD内码)
#define	CS_CaliStateData_Down			1000			//标定数据有效值下限(对应AD内码)
#define   CS_PowerOnAD_Steadytimes		10			//稳定开机稳定次数
#define   CS_SmallWeight_Steadytimes		6			//判断稳定的次数
#define   CS_BigWeight_Steadytimes		6			//判断稳定的次数
#define   CS_Cali_Steadytimes				12			//标定时判断稳定的次数
#define	CS_MemoryRange				25			//记忆重量范围


//////////////////////////////////////////////内部函数声明

void CS_Scale_WeightProc(void);


#define	GetDeltaAD					0
#define	GetADDirection				1
u16_t CS_Scale_GetDeltaAD(u8_t option);


#define	ScaleAdcFilterRun			0
#define	ScaleAdcFilterReset			1
#define	ScaleAdcFilterGetFilTimes	2
u16_t CS_Scale_AdcFilter(u8_t option,u16_t R_AD_BUF);


void CS_Scale_GetWeight(u16_t R_AD_BUF);


#define	JudgeSteadyRun				0
#define	ResetSteady					1
#define	GetIfSmallSteady			2
#define	GetIfHeavySteady			3
u8_t CS_Scale_JudgeSteady(u8_t option);



#define	LockProcRun					0
#define	SetDowningLock				1
#define	GetIfLock					2
u8_t CS_Scale_LockProc(u8_t option);


#define	SteadyProcReset				0
#define	SteadyProcRun				1
void CS_Scale_SteadyProc(u8_t option);


void CS_Scale_ReturnLock(void);


void CS_Scale_StandbyProc(u8_t B_Reset);


void CS_Scale_PowerOnProc(bool B_Reset);

/*
R_Debug_temp=1234;
CS_ScaleDisplay_Debug();
while(1);

*/

//////////////////////////////////////////主流程

void CS_Scale_Proc(void)
{
	if(R_Scale_state == CS_Scale_state_init)
		{	
		CS_Scale_PowerOnProc(0);
		}
	if(R_Scale_state == CS_Scale_state_standby)
		{
		CS_Scale_StandbyProc(0);		
		}
		
	if(B_Weight_AdOk == true)
		{		
		B_Weight_AdOk = false;	
		
		if(CS_Scale_ZeroProc(GetPowerOnZeroState,0)== false)	//还没上电取零点先取零点
			{
			CS_Scale_ZeroProc(FindPowerOnZero,0);
			}
		else
			{
				
			if(R_Scale_state == CS_Scale_state_weighting)
				{
				CS_Scale_WeightProc();
				CS_ScaleDisplay();
				}
			if(R_Scale_state == CS_Scale_state_locking)
				{					
				CS_ScaleDisplay_SteadyFlash();
				CS_Scale_SteadyProc(SteadyProcRun);
				}
			if(R_Scale_state == CS_Scale_state_caling)
				{
				CS_Scale_CaliProc(CaliProcRun);
				}	
			
			CS_Scale_TimeOutProc(0);
			}
		}
}	



//////////////////////////////////////////开机流程



void CS_Scale_PowerOn(void)
{
	
	if(R_Scale_state == CS_Scale_state_init)
		{
		B_Weight_AdOk = false;
		R_Weight_Com_Coo.pre =CS_CommTo1186_Null;	
		R_Weight_Com_Coo.now = CS_CommTo1186_Null;	
		//R_Scale_state =	CS_Scale_state_weighting;
		R_Selet_Unit = CS_ScaleDisplay_L_kg;

		R_Debug_temp = 0;
		B_Debug_Test =false;
		
		CS_Scale_CaliProc(CaliProcSetCali);		
		CS_Scale_ZeroProc(ZeroProcReset,0);	
		
		CS_Scale_TimeOutProc(CS_Reset_AllTime);
		CS_Scale_PowerOnProc(1);
		}
}






//////////////////////////////////////////正常称重流程

void CS_Scale_WeightProc(void)
{
	u16_t	R_AD_BUF;
	
	R_AD_BUF = CS_Scale_GetDeltaAD(GetDeltaAD);
	R_AD_BUF = CS_Scale_AdcFilter(ScaleAdcFilterRun,R_AD_BUF);
	CS_Scale_GetWeight(R_AD_BUF);
	CS_Scale_ZeroProc(RunningZero,0);
	CS_Scale_LockProc(LockProcRun);
	CS_Scale_JudgeOverLoad(0);
	CS_Scale_JudgeLowBat(0);
	CS_Scale_JudgeSteady(JudgeSteadyRun);
	
}


//当前AD和零点AD的差值
u16_t CS_Scale_GetDeltaAD(u8_t option)
{
	static       u8_t xdata B_Weight_Pos;				//正负AD变化标志
	u16_t	ad_temp;
	u16_t      R_AD_Zero;
	R_AD_Zero = CS_Scale_ZeroProc(GetRunningZero,0);
	if(R_AD_Original>R_AD_Zero)
		{
		ad_temp = R_AD_Original - R_AD_Zero;
		B_Weight_Pos = true;

		}
	else
		{
		ad_temp = R_AD_Zero - R_AD_Original;
		B_Weight_Pos = false;		
		}

	
	if(option == GetADDirection)
		{
		return	B_Weight_Pos;
		}	
	return ad_temp;
}



//滑动平均滤波
u16_t CS_Scale_AdcFilter(u8_t option,u16_t R_AD_BUF)
{
	u32_t	ad_temp;
	static	u16_t xdata R_AD_FilTimes;
	static	u16_t xdata R_AD_BUF_OLD;
	static	u16_t xdata R_AD_F1;
	static	u16_t xdata R_AD_F2;
	static	u16_t xdata R_AD_F3;
	static	u16_t xdata R_AD_F4;
	static	u16_t xdata R_AD_F5;
	static	u16_t xdata R_AD_F6;
	static	u16_t xdata R_AD_F7;
	static	u16_t xdata R_AD_F8;	
		
	if(option==ScaleAdcFilterGetFilTimes)
		{
		return  R_AD_FilTimes;
		}
	
	
	if(R_AD_BUF >R_AD_BUF_OLD)
		ad_temp = R_AD_BUF - R_AD_BUF_OLD;
	else
		ad_temp = R_AD_BUF_OLD - R_AD_BUF;

	R_AD_BUF_OLD	=	R_AD_BUF;
	
	if((ad_temp > CS_Scale_FiterX) ||(option==ScaleAdcFilterReset))	
		{						
		R_AD_FilTimes = 0;
		R_AD_F1 	    = 0;
		R_AD_F2 	    = 0;
		R_AD_F3 	    = 0;
		R_AD_F4 	    = 0;
		R_AD_F5 	    = 0;
		R_AD_F6 	    = 0;
		R_AD_F7 	    = 0;
		R_AD_F8 	    = 0;
		}
	else
		{
		if(R_AD_FilTimes < 200 )
		R_AD_FilTimes ++;	

			if(R_AD_FilTimes<=1)
				R_AD_F1 = R_AD_BUF;	
			if(R_AD_FilTimes<=2)
				R_AD_F2 = R_AD_BUF;
			if(R_AD_FilTimes<=3)
				R_AD_F3 = R_AD_BUF;
			if(R_AD_FilTimes<=4)
				R_AD_F4 = R_AD_BUF;
			if(R_AD_FilTimes<=5)
				R_AD_F5 = R_AD_BUF;
			if(R_AD_FilTimes<=6)
				R_AD_F6 = R_AD_BUF;
			if(R_AD_FilTimes<=7)
				R_AD_F7 = R_AD_BUF;
			if(R_AD_FilTimes<=8)
				R_AD_F8 = R_AD_BUF;
			if(R_AD_FilTimes>8)
				{
				R_AD_F1 = R_AD_F2;
				R_AD_F2 = R_AD_F3;
				R_AD_F3 = R_AD_F4;
				R_AD_F4 = R_AD_F5;
				R_AD_F5 = R_AD_F6;
				R_AD_F6 = R_AD_F7;
				R_AD_F7 = R_AD_F8;
				R_AD_F8 = R_AD_BUF;
				}

		ad_temp	= R_AD_F1;
		ad_temp	+= R_AD_F2;
		ad_temp	+= R_AD_F3;
		ad_temp	+= R_AD_F4;
		ad_temp	+= R_AD_F5;
		ad_temp	+= R_AD_F6;
		ad_temp	+= R_AD_F7;
		ad_temp	+= R_AD_F8;

		R_AD_BUF = ad_temp / 8 ;
		R_AD_BUF_OLD	=	R_AD_BUF;
		}
	return	R_AD_BUF;
}




void CS_Scale_GetWeight(u16_t R_AD_BUF)
{
	u32_t	ad_temp;
	u16_t	ad_cal;
	u16_t	CaliDot1,CaliDot2,CaliDot3;
	
	CaliDot1 =CS_Scale_CaliProc(CaliProcGetCaliDot1);
	CaliDot2 =CS_Scale_CaliProc(CaliProcGetCaliDot2);
	CaliDot3 =CS_Scale_CaliProc(CaliProcGetCaliDot3);
	
	if(R_AD_BUF <= CaliDot1 )
		{
		ad_temp = (u32_t)R_AD_BUF  * 5000;	
		R_UnitWeight.origin = ad_temp / CaliDot1;
		return;
		}
	ad_cal = R_AD_BUF - CaliDot1;

	if(ad_cal <=CaliDot2)
		{
		ad_temp = (u32_t)ad_cal  * 5000;	
		R_UnitWeight.origin = ad_temp / CaliDot2 +5000 ;
		return;
		}
	
	ad_cal  = R_AD_BUF - CaliDot1 - CaliDot2 ;	
	ad_temp = (u32_t)ad_cal  * 5000;	
	R_UnitWeight.origin = ad_temp /CaliDot3 +10000 ;

	
}


void CS_Scale_ChangeUnit()
{
	switch(R_Selet_Unit)
		{
		case CS_ScaleDisplay_L_kg:
			R_Selet_Unit=CS_ScaleDisplay_L_lb;
			break;
		case CS_ScaleDisplay_L_lb:
			R_Selet_Unit=CS_ScaleDisplay_L_jin;
			break;
		case CS_ScaleDisplay_L_jin:
			R_Selet_Unit=CS_ScaleDisplay_L_gongjin;
			break;
		case CS_ScaleDisplay_L_gongjin:
			R_Selet_Unit=CS_ScaleDisplay_L_kg;
			break;
		default :
			R_Selet_Unit=CS_ScaleDisplay_L_kg;
			break;
		}
}



u16_t  CS_Scale_ZeroProc(u8_t option,u16_t setdata)
{
	static	u8_t	   xdata R_Weight_GetZeroTimeOut = 0;	
	static	u16_t  xdata R_AD_PowerOnZero;	//上电零点AD值
	static	u16_t  xdata R_AD_Zero;			//零点AD值
	static	u8_t    xdata B_Weight_Zero;		//处于零点标志
	static	u8_t    xdata B_Weight_ZeroOk;	//上电获取完零点标志
	static	u8_t	   xdata B_LockDownCheck;	//下秤后第一次稳定追零
	static	u8_t	   xdata B_WeightZeroErr;		//零点错误标志
	
	u16_t	R_AdFilTimes;
	u16_t	R_AD_BUF;
	u16_t	weight_temp;
	u16_t	R_Open_Weight;
	u8_t		B_Weight_Pos;

	
	
	if(option==ZeroProcReset)
		{
		B_Weight_ZeroOk = 0;
		return 0;		
		}
	if(option==SetPowerOnZero)
		{
		R_AD_PowerOnZero = setdata;
		return 0;		
		}
	if(option==SetRunningZero)
		{
		R_AD_Zero = setdata;
		return 0;		
		}
	if(option==ClrLockDownCheckFlag)
		{
		B_LockDownCheck=false;
		return 0;
		}
	
	if(option==FindPowerOnZero)
	{
		
	
		if(R_Weight_GetZeroTimeOut <60)
		{
		R_Weight_GetZeroTimeOut ++;
		
		R_AD_BUF = R_AD_Original;
		R_AD_BUF=CS_Scale_AdcFilter(ScaleAdcFilterRun,R_AD_BUF);
		R_AdFilTimes = CS_Scale_AdcFilter(ScaleAdcFilterGetFilTimes,R_AD_BUF);
		
			if(R_AdFilTimes >=CS_PowerOnAD_Steadytimes)
				{
				R_AD_Zero	= R_AD_BUF;
				R_AD_PowerOnZero = R_AD_BUF;
				B_Weight_ZeroOk = true;
				}
		}
		else
		{
		R_AD_Zero	= 4000;
		R_AD_PowerOnZero = 4000;
		B_Weight_ZeroOk = true;
		}

	}

	if(option==RunningZero)
	{
	B_Weight_Zero = false;		
	//3kg以下才追零包括负重量
	B_Weight_Pos = CS_Scale_GetDeltaAD(GetADDirection);		//获取AD方向
	R_Open_Weight = CS_Scale_CaliProc(CaliProcGetOpenWeight);	//获取开机重量值
	
	if(R_UnitWeight.origin <= 300 ||B_Weight_Pos ==false)	
		B_Weight_Zero = true;

		if(R_UnitWeight.origin <= 200 || B_Weight_Pos==false)
			{		
				if(CS_Scale_JudgeSteady(GetIfSmallSteady)==true)
					{

					CS_Scale_JudgeSteady(ResetSteady);
					
					//非锁定状态正常追零
					if(CS_Scale_LockProc(GetIfLock)==false)	
						{
						if(R_AD_Original >= R_AD_PowerOnZero)
							{
							weight_temp = R_AD_Original - R_AD_PowerOnZero;
							if(weight_temp < R_Open_Weight)
								R_AD_Zero = R_AD_Original;
							}
						else
							R_AD_Zero = R_AD_Original;
						}
					//锁定状态下秤第一次追零做防提起处理
					else			
						{
						if(B_LockDownCheck==false)
							{
							if(R_AD_Original>R_AD_Zero)
								weight_temp = R_AD_Original - R_AD_Zero;
							else
								weight_temp = R_AD_Zero - R_AD_Original;

							//跟锁定前的零点做比较不等于一个秤体重量的处理
							//if(weight_temp<ScaleWeightMin&&weight_temp>ScaleWeightMax)
								//{
								R_AD_Zero = R_AD_Original;	//更新零点
								if(weight_temp>SameZeroRange)
									{
									//跟称重前不是一个零点报零点错并解锁
									B_WeightZeroErr=true;
									R_Display_Err=CS_ScaleDisplay_L_err;
									CS_Scale_LockProc(LockProcUnLock);
									}
								B_LockDownCheck=true;
								//}

							}						
						}
					
					}
			}
	return	0;		
	}

	if(option==GetPowerOnZero)
		return	R_AD_PowerOnZero;
	if(option==GetPowerOnZeroState)
		return	B_Weight_ZeroOk;
	if(option==GetRunningZero)
		return	R_AD_Zero;
	if(option==GetZeroState)
		return	B_Weight_Zero;
	if(option==GetIfZeroErr)
		return   B_WeightZeroErr;
	
	return 0;	
}



#define	LockProcZeroLock			1
#define	LockProcZeroUnLock			0xfe
#define	LockProcDownLock			2
#define	LockProcDownUnLock		0xfd
#define	LockProcLock			LockProcZeroLock+LockProcDownLock
#define	LockProcUnLock				3

u8_t CS_Scale_LockProc(u8_t option)
{
	static u8_t xdata B_Weight_Downing_Lock;		//下秤过程有重物在上面锁住
	
	if(option==SetDowningLock)
		{
		B_Weight_Downing_Lock =LockProcLock;
		return 0;
		}

	if(option==GetIfLock)
		{
		return B_Weight_Downing_Lock;
		}
	if(option==LockProcUnLock)
		{
		B_Weight_Downing_Lock =false;
		return 0;
		}
	
	if(CS_Scale_ZeroProc(GetZeroState,0)==true)		//处于零位
		{
		B_Weight_Downing_Lock &=LockProcDownUnLock ;
		
		if(B_Weight_Downing_Lock&LockProcZeroLock)
		R_UnitWeight.origin = R_Weight_Lock;
		}
	else
		{
		if(B_Weight_Downing_Lock&LockProcDownLock)
			R_UnitWeight.origin = R_Weight_Lock;
		else
			B_Weight_Downing_Lock &= LockProcZeroUnLock;
		}

	return 0;
}




//该部分工作移交给1186处理，不做误开机处理
/*
void CS_Scale_AvoidFalseBoot(void)
{
	static u8_t xdata R_Weight_ZeroTimes;
	static u8_t xdata B_Weight_PowerOn;		//开机标志
	static u8_t xdata B_Weight_AutoOpen;		//自动上秤标志
	
	if(R_UnitWeight.origin == 0)
		{
		if(R_Weight_ZeroTimes<255)
			R_Weight_ZeroTimes++;
		}	
	else
		{
		R_Weight_ZeroTimes = 0;
		}

	
	if(R_Weight_ZeroTimes == 1)
		{
		if(B_Weight_AutoOpen)
		B_Weight_PowerOn = false;
		else
		B_Weight_PowerOn = true;	
		}
	else
		{
		B_Weight_PowerOn = true;
		B_Weight_AutoOpen = false;
		}
}	
*/


#define	CS_Scale_LowBatBit	0x02
#define	GetIfLowBat			1

u8_t CS_Scale_JudgeLowBat(u8_t option)
{
	static	u8_t xdata B_Weight_LowBat;			//低电标志
	
	if(R_1186sys_state&CS_Scale_LowBatBit==true)
		{
		B_Weight_LowBat = true;
		R_Display_Err=CS_ScaleDisplay_L_err;
		}
	else
		B_Weight_LowBat = false;

	if(option==GetIfLowBat)
		return B_Weight_LowBat;
	
		return 0;	
}


u8_t CS_Scale_JudgeOverLoad(u8_t option)
{

	static	u8_t	  xdata R_Weight_OverLoadTimes;
	static	u8_t  xdata  B_Weight_OverLoad;			//超载标志
	if(option==ResetOverLoad)
		{
		R_Weight_OverLoadTimes = 0;
		B_Weight_OverLoad = false;
		return 0;
		}
	
	if(R_UnitWeight.origin > CS_Scale_MaxWeight)
		R_Weight_OverLoadTimes++;
	if(R_Weight_OverLoadTimes >=2)
		{
		R_Weight_OverLoadTimes = 2;
		B_Weight_OverLoad = true;
		R_Display_Err=CS_ScaleDisplay_L_err;
		}
	else
		B_Weight_OverLoad = false;

	if(option==GetIfOverLoad)
		return B_Weight_OverLoad;
	
		return 0;
}




u8_t CS_Scale_JudgeSteady(u8_t option)
{

	u16_t	weight_temp;
	u16_t	R_Weight_Temp;
	static	u16_t xdata R_Weight_kg_old;
	static	u8_t	  xdata R_Weight_SameTimes;
	static	u8_t xdata B_Weight_SmallSteady;		//小重量稳定标志
	static	u8_t xdata B_Weight_HeavySteady;	//大重量稳定标志
	
	if(option==ResetSteady)
		{
		R_Weight_SameTimes = 0;
		B_Weight_SmallSteady = false;
		B_Weight_HeavySteady = false;
		return 0;
		}	

	if(option==GetIfSmallSteady)
		return B_Weight_SmallSteady;

	if(option==GetIfHeavySteady)
		return B_Weight_HeavySteady;
	
	if(CS_Scale_LockProc(GetIfLock)==false)
		{
	
		if(R_UnitWeight.origin > R_Weight_kg_old)
			weight_temp = R_UnitWeight.origin -R_Weight_kg_old;
		else
			weight_temp = R_Weight_kg_old -R_UnitWeight.origin;
		
		if(weight_temp <  CS_Scale_SameWeightRange)
			{
			if(R_Weight_SameTimes<255)
				R_Weight_SameTimes++;
			}
		else
			R_Weight_SameTimes = 0;
	
		
		R_Weight_kg_old = R_UnitWeight.origin;

		B_Weight_SmallSteady = false;
		B_Weight_HeavySteady = false;
		
		if(R_UnitWeight.origin < CS_Scale_MinLockWeight)
			{
			if(R_Weight_SameTimes>CS_SmallWeight_Steadytimes)
				B_Weight_SmallSteady = true;		
			}
		else
			{			
			if(R_Weight_SameTimes>CS_BigWeight_Steadytimes)
				{
				B_Weight_HeavySteady = true;	

				if(R_UnitWeight.origin>R_Weight_Mem)
				R_Weight_Temp = R_UnitWeight.origin -R_Weight_Mem;
				else
				R_Weight_Temp = R_Weight_Mem - R_UnitWeight.origin;

				//在记忆范围内记忆
				if(R_UnitWeight.origin > ScaleStartMemoryWeight)
					{
					if(R_Weight_Temp<CS_MemoryRange)
					R_UnitWeight.origin = R_Weight_Mem;
					}
			
				R_Weight_Lock = R_UnitWeight.origin;
				R_Weight_Mem = R_UnitWeight.origin;
				
				R_Scale_state = CS_Scale_state_locking;
				CS_SoftTimer(ResetSoftTimer);		//软件定时器开始用于锁定闪显示计时
				CS_Scale_SteadyProc(SteadyProcReset);
				CS_Scale_TimeOutProc(CS_Reset_AllTime);
				}
			}
		}
	return 0;
}




///////////////////////////////////锁定流程



void CS_Scale_SteadyProc(u8_t option)
{
	u16_t	R_AD_BUF;	

	static	u8_t xdata B_Debug_Send;	
	static 	u8_t xdata ReadUTC;
	UserMeasureData SendData;

	if(option==SteadyProcReset)
		{
		B_Debug_Send=false;
		return;
		}
	
	if(CS_SoftTimer(GetIfTimeOut) == true)	//锁定闪现时间到
		{
		if(CS_Scale_JudgeSteady(GetIfHeavySteady) == true)
			{
			
			CS_Scale_JudgeSteady(ResetSteady);
		
			CS_Scale_ZeroProc(ClrLockDownCheckFlag,0);
			ReadUTC = false;	//初始化读ADC
			}
		else
			{
			
			/*
			if(wechatStatus == WECHAT_ST_OK&&B_Debug_Send==false)
				{
				sendMeasureData();
				B_Debug_Send=true;
				}
			*/		
			if(B_Debug_Send==false)
				{
				
				if(ReadUTC==false)
					{
					ReadUTC=true;
					CS_Scale_1186ComSend(CS_CommTo1186_ReadTime);	
					}
				
				if(CS_If1186ComSucess()==true)
					{
					
					
					ReadUTC=false;
					
					SendData.UTCTime[0]=R_1186_RTC[0];
					SendData.UTCTime[1]=R_1186_RTC[1];	
					SendData.UTCTime[2]=R_1186_RTC[2];	
					SendData.UTCTime[3]=R_1186_RTC[3];	
					SendData.weight=ESWAP(R_UnitWeight.origin);
					QueuePush(&SendData);
					B_Debug_Send=true;

					CS_Scale_1186ComSend(CS_CommTo1186_ReadAd);
			
					//CS_SoftTimer(ResetSoftTimer);	
					
					}
				
				}
			else
				{
				R_AD_BUF = CS_Scale_GetDeltaAD(GetDeltaAD);
				R_AD_BUF = CS_Scale_AdcFilter(ScaleAdcFilterRun,R_AD_BUF);
				CS_Scale_GetWeight(R_AD_BUF);				
				CS_Scale_ReturnLock();
				}
			}
		}

			/*
			if(CS_SoftTimer(GetSoftTimerTime)<200)
				{
			R_Debug_temp=SendData.UTCTime;
			CS_ScaleDisplay_Debug();			
			return;
				}
			*/	
			/* B_Debug_Test =true;
			if(B_Debug_Test)
				{
				R_Debug_temp=888;
				CS_ScaleDisplay_Debug();
				while(1);
				}
			*/
}



void CS_Scale_ReturnLock(void)
{
	u16_t	weight_temp;
	
	if( R_UnitWeight.origin > R_Weight_Lock)
		{
		weight_temp = R_UnitWeight.origin -R_Weight_Lock;
		if(weight_temp >= CS_Scale_RetLockWeightInc)
			{
			R_Scale_state = CS_Scale_state_weighting;
			CS_Scale_TimeOutProc(CS_Reset_AllTime);
			}
		}
	else
		{
		weight_temp = R_Weight_Lock -R_UnitWeight.origin;
		if(weight_temp >= CS_Scale_RetLockWeightDec)
			{
			
			R_Scale_state = CS_Scale_state_weighting;
			CS_Scale_LockProc(SetDowningLock);
			CS_Scale_TimeOutProc(CS_Reset_AllTime);
			
			}
		}

}

#define	StartClearMemWeightTimer	1
#define	MemWeightTimerInc			2
#define	MemWeightTimer			30

void CS_MemoryWeightClear_Timer(u8_t option)
{
	static u8_t xdata timecount; 				//计时变量
	static u8_t xdata B_Timeout;

	if(option==StartClearMemWeightTimer)
		{
		timecount=0;
		B_Timeout = false;
		}

	if(option==MemWeightTimerInc)
		{
		if(timecount<255 ) 
		timecount++;
		}
	
	if(B_Timeout == false)
		{
		if(timecount>SoftTimerTimeOut)
			{
			B_Timeout = true;
			R_Weight_Mem = 0;	//时间到清记忆重量
			}
		}
}


///////////////////////////////////标定流程
#define	CS_CaliState_GetZero			0
#define	CS_CaliState_ShowCal1			1
#define	CS_CaliState_GetCal1			2
#define	CS_CaliState_ShowCal2			3
#define	CS_CaliState_GetCal2			4
#define	CS_CaliState_ShowCal3			5
#define	CS_CaliState_GetCal3			6
#define	CS_CaliState_ShowCal4			7
#define	CS_CaliState_SaveData			8
#define	CS_CaliState_ShowPass			9
#define	CS_CaliState_ShowErr			10

u16_t CS_Scale_CaliProc(u8_t option)
{
	static	u16_t xdata R_AD_CaliDot1;		//第一个标定点AD值
	static	u16_t xdata R_AD_CaliDot2;		//第二个标定点AD值
	static	u16_t xdata R_AD_CaliDot3;		//第三个标定点AD值
	static	u8_t	  xdata R_CaliState;			//标定状态
	static	u16_t xdata R_OpenWeight_Ad;	//开机重量AD值
	
	u16_t	R_AdFilTimes;	
	u16_t	weight_temp;
	u16_t	R_AD_BUF;
	u16_t	R_AD_PowerOnZero;

	if(option==CaliProcReset)
		{
		R_CaliState=CS_CaliState_GetZero;
		return	0;
		}
	if(option==CaliProcSetCali)
		{
/*
		*(mem_sensor_buf )= 12;
		*(mem_sensor_buf+1) = 12;
		*(mem_sensor_buf+2 )= 12;
		*(mem_sensor_buf+3) = 12;
		*(mem_sensor_buf+4 )= 12;
		*(mem_sensor_buf+5) = 12;
		SpiFlashOperator(CS_CMD_WRITE_FLASH);
		//YC_Delay10ms(10);
		
		*(mem_sensor_buf )= 0;
		*(mem_sensor_buf+1) = 0;
		*(mem_sensor_buf+2 )= 0;
		*(mem_sensor_buf+3) = 0;
		*(mem_sensor_buf+4 )= 0;
		*(mem_sensor_buf+5) = 0;
		
		
		SpiFlashOperator( CS_CMD_READ_FLASH);
		
		
		
		R_Debug_temp=(u16_t) *(mem_sensor_buf+2 ) ;
		CS_ScaleDisplay_Debug();
		while(1);
		*/
		//YC_Delay1S(1);
		
		R_AD_BUF = *(mem_sensor_buf );
		R_AD_CaliDot1 =R_AD_BUF<<8 |  (*(mem_sensor_buf+1));
		R_AD_BUF = *(mem_sensor_buf+2 );
		R_AD_CaliDot2 =R_AD_BUF<<8 |  (*(mem_sensor_buf+3));
		R_AD_BUF = *(mem_sensor_buf+4 );
		R_AD_CaliDot3 =R_AD_BUF<<8 |  (*(mem_sensor_buf+5));	

		

		if(R_AD_CaliDot1<CS_CaliStateData_Down || R_AD_CaliDot1>CS_CaliStateData_Up)
			if(R_AD_CaliDot2<CS_CaliStateData_Down||R_AD_CaliDot2>CS_CaliStateData_Up)
				if(R_AD_CaliDot3<CS_CaliStateData_Down||R_AD_CaliDot3>CS_CaliStateData_Up)
					{
					R_AD_CaliDot1	= 6000;
					R_AD_CaliDot2	= 5000;
					R_AD_CaliDot3	= 4000;	
					}	
		
		
		R_OpenWeight_Ad	= R_AD_CaliDot1/10;
		return	0;		
		}
	if(option==CaliProcSetOpenWeight)
		{
		R_OpenWeight_Ad	= R_AD_CaliDot1/10;
		return	0;
		}
	
	if(option==CaliProcGetOpenWeight)
		return R_OpenWeight_Ad;
	if(option==CaliProcGetCaliDot1)
		return R_AD_CaliDot1;
	if(option==CaliProcGetCaliDot2)
		return R_AD_CaliDot2;
	if(option==CaliProcGetCaliDot3)
		return R_AD_CaliDot3;
	
	R_AD_BUF = R_AD_Original;
	R_AD_BUF=CS_Scale_AdcFilter(ScaleAdcFilterRun,R_AD_BUF);
	R_AdFilTimes = CS_Scale_AdcFilter(ScaleAdcFilterGetFilTimes,R_AD_BUF);
	
	

	//数据稳定才处理
		
	if(R_CaliState==CS_CaliState_GetZero) 
		{	

			R_AD_CaliDot1 = R_AD_BUF;	//暂存零点用于显示
			if(R_AdFilTimes>=CS_Cali_Steadytimes)
				{
			CS_Scale_ZeroProc(SetPowerOnZero,R_AD_BUF);	
			CS_Scale_AdcFilter(ScaleAdcFilterReset,R_AD_BUF);
			CS_SoftTimer(ResetSoftTimer);
			R_CaliState = CS_CaliState_ShowCal1;
				}
		CS_ScaleDisplay_FreshBuf(DisplayData,R_AD_BUF);
		}
	
	if(R_CaliState == CS_CaliState_ShowCal1)
		{
		if(CS_SoftTimer(GetSoftTimerTime)>200) 	
			R_CaliState = CS_CaliState_GetCal1;
		CS_ScaleDisplay_FreshBuf(DisplayCal1,0);
		}

	if(R_CaliState==CS_CaliState_GetCal1)
		{	
			R_AD_PowerOnZero =CS_Scale_ZeroProc(GetPowerOnZero,0);	
			R_AD_CaliDot1 = R_AD_BUF - R_AD_PowerOnZero;
				
			if((R_AD_CaliDot1 < R_OpenWeight_Ad)||(R_AD_BUF < R_AD_PowerOnZero))
				{
				if(R_AdFilTimes>=CS_Cali_Steadytimes)
					{
					CS_Scale_ZeroProc(SetPowerOnZero,R_AD_BUF);	
					CS_Scale_AdcFilter(ScaleAdcFilterReset,R_AD_BUF);
					}
				R_AD_CaliDot1 = 0;
				}
			else
				{
				if((R_AD_CaliDot1> 750)&&(R_AdFilTimes>=CS_Cali_Steadytimes))
					{				
					CS_SoftTimer(ResetSoftTimer);
					CS_Scale_AdcFilter(ScaleAdcFilterReset,R_AD_BUF);
					R_CaliState = CS_CaliState_ShowCal2;
					}
				}
			CS_ScaleDisplay_FreshBuf(DisplayData,R_AD_CaliDot1);
			
		}

	if(R_CaliState == CS_CaliState_ShowCal2)
		{
		if(CS_SoftTimer(GetSoftTimerTime)>200) 	
			R_CaliState = CS_CaliState_GetCal2;
		CS_ScaleDisplay_FreshBuf(DisplayCal2,0);
		}
	
	if(R_CaliState==CS_CaliState_GetCal2)
		{		
			R_AD_PowerOnZero =CS_Scale_ZeroProc(GetPowerOnZero,0);		
			weight_temp = R_AD_BUF - R_AD_PowerOnZero;
					
			if((weight_temp < R_AD_CaliDot1)||(R_AD_BUF < R_AD_PowerOnZero))
				{
				R_AD_CaliDot2 = 0;
				}
			else
				{
				R_AD_CaliDot2 = weight_temp -R_AD_CaliDot1;
			
				if((R_AD_CaliDot2> 750)&& (R_AdFilTimes>=CS_Cali_Steadytimes))
					{
					CS_Scale_AdcFilter(ScaleAdcFilterReset,R_AD_BUF);
					CS_SoftTimer(ResetSoftTimer);
					R_CaliState = CS_CaliState_ShowCal3;
					}
				}
			CS_ScaleDisplay_FreshBuf(DisplayData,R_AD_CaliDot2);
		}

	if(R_CaliState == CS_CaliState_ShowCal3)
		{
		if(CS_SoftTimer(GetSoftTimerTime)>200) 	
			R_CaliState = CS_CaliState_GetCal3;
		CS_ScaleDisplay_FreshBuf(DisplayCal3,0);
		}
	
	if(R_CaliState==CS_CaliState_GetCal3)
		{		
			R_AD_PowerOnZero =CS_Scale_ZeroProc(GetPowerOnZero,0);		
			weight_temp = R_AD_BUF - R_AD_PowerOnZero;
					
			if((weight_temp < R_AD_CaliDot1)||(R_AD_BUF < R_AD_PowerOnZero))
				{
				R_AD_CaliDot3 = 0;
				}
			else
				{
				weight_temp = weight_temp -R_AD_CaliDot1;

				if(weight_temp < R_AD_CaliDot2)
					{
					R_AD_CaliDot3 = 0;
					
					}
				else
					{
				R_AD_CaliDot3 = weight_temp -R_AD_CaliDot2;
			
				if((R_AD_CaliDot3> 750)&& (R_AdFilTimes>=CS_Cali_Steadytimes))
					{
					CS_SoftTimer(ResetSoftTimer);
					R_CaliState = CS_CaliState_ShowCal4;
					}
					}
				}
			CS_ScaleDisplay_FreshBuf(DisplayData,R_AD_CaliDot3);
		}
	
	if(R_CaliState == CS_CaliState_ShowCal4)
		{
		if(CS_SoftTimer(GetSoftTimerTime)>200) 
			R_CaliState = CS_CaliState_SaveData;
		CS_ScaleDisplay_FreshBuf(DisplayCal4,0);
		}
	
	if(R_CaliState==CS_CaliState_SaveData)
		{
		
		R_AD_PowerOnZero =CS_Scale_ZeroProc(GetPowerOnZero,0);
		CS_Scale_ZeroProc(SetRunningZero,R_AD_PowerOnZero);
		R_Scale_state = CS_Scale_state_weighting;
		R_OpenWeight_Ad	=  R_AD_CaliDot1/10;

		/*
		R_Debug_temp=R_AD_CaliDot2;
		CS_ScaleDisplay_Debug();
		while(1);
		*/
		
		*(mem_sensor_buf )= HI_UINT16(R_AD_CaliDot1);
		*(mem_sensor_buf+1) = LO_UINT16(R_AD_CaliDot1);
		*(mem_sensor_buf+2 )= HI_UINT16(R_AD_CaliDot2);
		*(mem_sensor_buf+3) = LO_UINT16(R_AD_CaliDot2);
		*(mem_sensor_buf+4 )= HI_UINT16(R_AD_CaliDot3);
		*(mem_sensor_buf+5) = LO_UINT16(R_AD_CaliDot3);
		SpiFlashOperator(CS_CMD_WRITE_FLASH);

		

		}
	return 0;
	
}



///////////////////////////////////待机流程

#define	CS_Standby_S1_ToSleep				0
#define	CS_Standby_S2_WakeUp				1
#define	CS_Standby_S3_xxxx				2



void CS_Scale_StandbyProc(u8_t B_Reset)
{
	static u8_t xdata comm_doing;
	static u8_t xdata StandbyState;

	if(B_Reset==1)
		{
		StandbyState=CS_Standby_S1_ToSleep;
		comm_doing=false;
		return;
		}

	if(B_Reset==2)
		{
		StandbyState=CS_Standby_S2_WakeUp;
		comm_doing=false;
		return;
		}
	
	switch(StandbyState)
	{
	case CS_Standby_S1_ToSleep:
		if(comm_doing==false)
			{
			comm_doing=true;

			CS_Scale_1186ComSend(CS_CommTo1186_SetSleepMode);
			//sys_state = SYS_ST_IDLE;			//系统进入睡眠
			}
		if(CS_If1186ComSucess()==true)
			{
			comm_doing=false;
			StandbyState =  CS_Standby_S2_WakeUp;

			btCmdBuf.len = 0x01;
			btCmdBuf.DATA[0] = CS_CMD_SLEEP_REQ;
			BT_CmdFifoIn(&btCmdBuf);

			//btCmdBuf.len = 2;
			//btCmdBuf.DATA[0] = 0x20;
			//btCmdBuf.DATA[1] = 0;
			//BT_CmdFifoIn(&btCmdBuf);
			
			 
			sys_state = SYS_ST_IDLE;  //SYS_ST_SLEEP;			//系统进入睡眠
					
			}	
		break;
	case CS_Standby_S2_WakeUp:
		if(comm_doing==false)
			{
			comm_doing=true;
			CS_Scale_1186ComSend(CS_CommTo1186_ReadAdZero);
			}
		if(CS_If1186ComSucess()==true)
			{
			comm_doing=false;
			StandbyState =  CS_Standby_S1_ToSleep;	
			sys_state = SYS_ST_RUN;

			/*
			while(1)
			{
			R_Debug_temp = R_AD_Zero;
			CS_ScaleDisplay_Debug();
			}
			*/
			//清除异常状态
			CS_Scale_JudgeOverLoad(ResetOverLoad);
			CS_Scale_TimeOutProc(CS_Reset_AllTime);
			R_Display_Err = 0;
			
			R_Scale_state = CS_Scale_state_weighting;

			CS_Scale_1186ComSend(CS_CommTo1186_ReadAd);
			
			}
		break;
	default:
		StandbyState = CS_Standby_S1_ToSleep;
		break;
	}
	
	//R_Debug_temp = 456;

}


///////////////////////////////////开机流程

#define	CS_PowerOn_State1_Reset				0
#define	CS_PowerOn_State2_ReadOtp				1
#define	CS_PowerOn_State3_ReadVersion			2
#define	CS_PowerOn_State4_ReadTime			3
#define	CS_PowerOn_State5_SetOpenWeight		4
#define	CS_PowerOn_State6_ShowOpenPic		5
#define	CS_PowerOn_State7_ShowOpenPicWait	6


void CS_Scale_PowerOnProc(bool B_Reset)
{
	static u8_t xdata comm_doing;
	static u8_t xdata PowerOnState;
	
	
	if(B_Reset)
		{
		PowerOnState=CS_PowerOn_State1_Reset;
		comm_doing=false;
		return;
		}
	
	//-----------S1
	if(PowerOnState==CS_PowerOn_State1_Reset)
		{
		if(comm_doing==false)
			{
			comm_doing=true;
			CS_Scale_1186ComSend(CS_CommTo1186_Reset);
			}
		else
			{
			comm_doing=false;
			PowerOnState	= CS_PowerOn_State2_ReadOtp;
			}		
		}
	//-----------S2
	if(PowerOnState==CS_PowerOn_State2_ReadOtp)
		{
		if(comm_doing==false)
			{
			comm_doing=true;
			CS_Scale_1186ComSend(CS_CommTo1186_ReadOtp);
			
			}
		if(CS_If1186ComSucess()==true)
			{
			comm_doing=false;
			PowerOnState =  CS_PowerOn_State3_ReadVersion;
			}
		}
	//-----------S3
	if(PowerOnState==CS_PowerOn_State3_ReadVersion)
		{
		PowerOnState =  CS_PowerOn_State4_ReadTime;		
		}
	//-----------S4
	if(PowerOnState==CS_PowerOn_State4_ReadTime)
		{
		if(comm_doing==false)
			{
			comm_doing=true;
			CS_Scale_1186ComSend(CS_CommTo1186_ReadTime);
			}
		if(CS_If1186ComSucess()==true)
			{
			comm_doing=false;
			PowerOnState =  CS_PowerOn_State5_SetOpenWeight;	
			}
		}
	//-----------S5
	if(PowerOnState==CS_PowerOn_State5_SetOpenWeight)
		{
		if(comm_doing==false)
			{
			comm_doing=true;
			CS_Scale_1186ComSend(CS_CommTo1186_SetOpenWeight);
			}
		if(CS_If1186ComSucess()==true)
			{
			comm_doing=false;
			PowerOnState =  CS_PowerOn_State6_ShowOpenPic;
			}
		}
	//-----------S6
	if(PowerOnState==CS_PowerOn_State6_ShowOpenPic)
		{
		if(comm_doing==false)
			{
			comm_doing=true;	
			CS_ScaleDisplay_FreshBuf(DisplayData,8888);
			CS_Scale_1186ComSend(CS_CommTo1186_LcdDisplay);
			}
		if(CS_If1186ComSucess()==true)
			{	
			comm_doing=false;
			PowerOnState =	CS_PowerOn_State7_ShowOpenPicWait;
			CS_SoftTimer(ResetSoftTimer);
			}		
		}
	//-----------S7
	if(PowerOnState==CS_PowerOn_State7_ShowOpenPicWait)
		{
		if(CS_SoftTimer(GetSoftTimerTime)>200)
			{
			R_Scale_state = CS_Scale_state_weighting;
			CS_Scale_1186ComSend(CS_CommTo1186_ReadAd);
			}
		}

}




u8_t CS_SoftTimer(u8_t option)
{
	static u8_t xdata timecount; 				//计时变量
	static u8_t xdata B_Timeout;

	if(option==ResetSoftTimer)
		{
		timecount=0;
		B_Timeout = false;
		}

	if(option==GetSoftTimerTime)
		return	timecount;

	if(option==SoftTimerInc)
		{
		if(timecount<255 ) 
		timecount++;
		}
	
	
	if(timecount>SoftTimerTimeOut)
		B_Timeout = true;

	if(option==GetIfTimeOut)
		return	B_Timeout;
	
	return 0;

}





///////////////////////////////////超时处理


typedef struct _CS_TimeOut_T{
	u8_t zero; 	
	u8_t lock;
	u8_t unstable;
	u8_t overload;
	u8_t lowbat;
}CS_TimeOut_T;



void CS_Scale_TimeOutProc(u8_t option)
{
	static	CS_TimeOut_T xdata R_TimeOut;
	static	u8_t xdata B_TimeBase_1s;		//1s时基

	if(option==CS_SetTimeBase1S)
		{
		B_TimeBase_1s = true;
		return;
		}
	//超时计数器复位
	if(option)
	{
	if(option==CS_Reset_ZeroTime)
		R_TimeOut.zero=0;
	if(option==CS_Reset_LockTime)
		R_TimeOut.lock=0;
	if(option==CS_Reset_UnstableTime)
		R_TimeOut.unstable=0;
	if(option==CS_Reset_OverLoadTime)
		R_TimeOut.overload=0;
	if(option==CS_Reset_LowBatTime)
		R_TimeOut.lowbat=0;
	if(option==CS_Reset_AllTime)
		{
		R_TimeOut.zero=0;
		R_TimeOut.lock=0;
		R_TimeOut.unstable=0;
		R_TimeOut.overload=0;
		R_TimeOut.lowbat=0;
		}
	return;
	}
		
	if(B_TimeBase_1s == true)
	{
		B_TimeBase_1s = false;

		//低电超时处理
		if(CS_Scale_JudgeLowBat(GetIfLowBat) == true)
		{
			R_TimeOut.lowbat++;

			if(R_TimeOut.lowbat>=CS_Scale_TimeOut_LowBat)
			{
				R_Scale_state = CS_Scale_state_standby;
				CS_Scale_StandbyProc(1);
				R_TimeOut.lowbat=0;
			}
		return;			
		}

		//超载超时处理
		if(CS_Scale_JudgeOverLoad(GetIfOverLoad)== true)
		{
			R_TimeOut.overload++;

			if(R_TimeOut.overload>=CS_Scale_TimeOut_OverLoad)
			{
				R_Scale_state = CS_Scale_state_standby;
				CS_Scale_StandbyProc(1);
				R_TimeOut.overload=0;
			}
		return;	

		}
		
		
		//称重模式下的超时处理
		if(R_Scale_state == CS_Scale_state_weighting)
		{
		
		if(CS_Scale_ZeroProc(GetZeroState,0) == true)
			{
			R_TimeOut.unstable=0;
			R_TimeOut.zero++;
			}
		else
			{
			R_TimeOut.zero=0;
			R_TimeOut.unstable++;
			}

		if(R_TimeOut.zero >=CS_Scale_TimeOut_Zero||R_TimeOut.unstable>=CS_Scale_TimeOut_Unstable)
			{
				//R_Debug_temp=R_TimeOut.unstable;
				//CS_ScaleDisplay_Debug();
				//while(1);
				
				R_Scale_state = CS_Scale_state_standby;
				CS_Scale_StandbyProc(1);
				R_TimeOut.zero=0;
				R_TimeOut.unstable=0;
			}
		return;

		}
		

		//锁定模式下的超时处理
		if(R_Scale_state == CS_Scale_state_locking)
		{
			R_TimeOut.unstable=0;
			R_TimeOut.lock++;
			
			if(R_TimeOut.lock>=CS_Scale_TimeOut_Lock)
			{
				
				R_Scale_state = CS_Scale_state_standby;
				CS_Scale_StandbyProc(1);
				R_TimeOut.lock=0;
			}
		return;
		}		
		

	}

}











