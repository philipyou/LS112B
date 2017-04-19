#include "YC_Define.h"
#include "YC_Peripheral.h"
#include "system.h"
#include "CS_CommTo1186.h"
#include "CS_Scale_Proc.h"
#include "CS_ScaleDisplay.h"
#include "CS_ScaleKey.h"


#define KEY_SHORT_PUSH_CNT		3
#define KEY_LONG_PUSH_CNT			50
#define KEY_SHORT_PRESS_TIME		15
#define KEY_DOWN					0
#define KEY_UP						1


void CS_KeyScan(void)
{
	u8_t  keyState;
	static u8_t xdata keyState_prev=1;
	static u8_t xdata keyStateCnt;
	static u8_t xdata KeyPressDeal=0;
	static u8_t xdata KeyLongPressDeal=0;
	static u8_t xdata ShortPressCount=0;
	static u8_t xdata PressTimesCount=0;
	static u8_t xdata Press;
	
	
	keyState = YC_GPIOGetInputStatus(5);	//GPIO5	

	if(ShortPressCount<255)		//短按计时
		ShortPressCount++;
	
	if(keyState ==KEY_UP)
		{
		keyState_prev = KEY_UP;	
		keyStateCnt = 0; 

		if(KeyLongPressDeal==1)
			Press = false;
		
		KeyPressDeal=0;
		KeyLongPressDeal=0;
		
		if(Press == true)
			{
			Press = false;
						
			//短按弹起处理
			if(ShortPressCount <KEY_SHORT_PRESS_TIME)
				PressTimesCount++;
			else
				{
				PressTimesCount = 0;
				CS_Scale_ChangeUnit();	
				}
			ShortPressCount =0;	
			}
		}
	else
		{
		if(keyState_prev == KEY_DOWN)
			{	
			if(keyStateCnt<255)
				keyStateCnt++;
			 if(keyStateCnt > KEY_SHORT_PUSH_CNT)
			 	{
			 	Press =true;
				if(KeyPressDeal==0)
					{
					KeyPressDeal=1;
					//按键刚按下的处理
					if(ShortPressCount > KEY_SHORT_PRESS_TIME)
						PressTimesCount = 0;
					ShortPressCount=0;
					}
			 	}
			 if(keyStateCnt > KEY_LONG_PUSH_CNT)
			 	{
				if(KeyLongPressDeal==0)
					{
					//按键刚长按下的处理	
					KeyLongPressDeal=1;					
					if(PressTimesCount==3)
						{
						R_Scale_state = CS_Scale_state_caling;	
						CS_Scale_CaliProc(CaliProcReset);
						Press = false;
						keyStateCnt=0;
						}
					PressTimesCount = 0;
					ShortPressCount =0;
					}
			 	}
			}
		else
			{
			keyState_prev = KEY_DOWN;
			Press = false;
			}
		}
}








































