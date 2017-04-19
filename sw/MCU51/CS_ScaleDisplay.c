#include "YC_Define.h"
#include "YC_Peripheral.h"
#include "CS_Scale_Proc.h"
#include "CS_CommTo1186.h"
#include "CS_ScaleDisplay.h"


#include "retation_variable.h"

u8_t xdata CS_lcd_showbuf[5];
u8_t xdata CS_Lcd_Send_Data[6];
u8_t xdata CS_Lcd_Send_Data_BAK[6];

void CS_ScaleDisplay()
{
	u32_t weight_temp;
	
	if(R_Display_Err==CS_ScaleDisplay_L_err)	//提示信息显示
		{
		if(CS_Scale_JudgeOverLoad(GetIfOverLoad)== true)	
			CS_ScaleDisplay_FreshBuf(DisplayOverLoad,0);			
		if(CS_Scale_JudgeLowBat(GetIfLowBat)== true)
			CS_ScaleDisplay_FreshBuf(DisplayLowBat,0);	
		if(CS_Scale_ZeroProc(GetIfZeroErr,0)==true)
			CS_ScaleDisplay_FreshBuf(DisplayZeroErr,0);
		return;
		}
	if(R_Selet_Unit == CS_ScaleDisplay_L_kg)
		{
		R_UnitWeight.kg = R_UnitWeight.origin;
		CS_ScaleDisplay_FreshBuf(Displaykg,R_UnitWeight.kg);
		}
	if(R_Selet_Unit == CS_ScaleDisplay_L_gongjin)
		{
		R_UnitWeight.kg = R_UnitWeight.origin;
		CS_ScaleDisplay_FreshBuf(Displaygongjin,R_UnitWeight.kg);
		}
	if(R_Selet_Unit == CS_ScaleDisplay_L_jin)
		{
		R_UnitWeight.jin = R_UnitWeight.origin *2;
		CS_ScaleDisplay_FreshBuf(Displayjin,R_UnitWeight.jin);
		}				
       if(R_Selet_Unit == CS_ScaleDisplay_L_lb )
       	{
		weight_temp   = (u32_t) R_UnitWeight.origin *22046;
		R_UnitWeight.lb = weight_temp / 10000;
		CS_ScaleDisplay_FreshBuf(Displaylb,R_UnitWeight.lb);
       	}		
}





void CS_ScaleDisplay_Debug()
{
	u8_t buf[5];
	CS_ScaleDisplay_HexToBcd(R_Debug_temp,buf);
		
	CS_lcd_showbuf[0] = CS_ScaleDisplay_LookUpTable(buf[1]);
	CS_lcd_showbuf[1] = CS_ScaleDisplay_LookUpTable(buf[2]);
	CS_lcd_showbuf[2] = CS_ScaleDisplay_LookUpTable(buf[3]);
	CS_lcd_showbuf[3] = CS_ScaleDisplay_LookUpTable(buf[4]);	
		
	CS_ScaleDisplay_LcdBuf_rShift4();

	CS_CommTo1186_SendCmd(CS_CommTo1186_LcdDisplay,CS_Lcd_Send_Data);

}



void CS_ScaleDisplay_FreshBuf(u8_t option,u16_t showdata)
{
	static	u8_t	data_buf[5];

	//////////////////////////////////////////
	//数字部分赋值
	if(option==DisplayOverLoad)
	{
	data_buf[0]=Lcdnull_Num;
	data_buf[1]=Lcdup_Num;
	data_buf[2]=Lcdup_Num;
	data_buf[3]=Lcdup_Num;
	data_buf[4]=Lcdup_Num;
	}

	if(option==DisplayLowBat)
	{
	data_buf[0]=0;
	data_buf[1]=Lcdnull_Num;
	data_buf[2]=LcdchL_Num;
	data_buf[3]=Lcdcho_Num;
	data_buf[4]=Lcdnull_Num;
	}

	if(option==DisplayZeroErr)
	{
	data_buf[0]=Lcdnull_Num;
	data_buf[1]=Lcdnull_Num;
	data_buf[2]=LcdchC_Num;
	data_buf[3]=Lcdnull_Num;
	data_buf[4]=Lcdnull_Num;
	}
	
	if(option==DisplayCal1||option==DisplayCal2||option==DisplayCal3||option==DisplayCal4)
	{
	data_buf[0]=0;
	data_buf[1]=LcdchC_Num;
	data_buf[2]=LcdchA_Num;
	data_buf[3]=LcdchL_Num;
		if(option==DisplayCal1)
			data_buf[4]=1;
		if(option==DisplayCal2)
			data_buf[4]=2;
		if(option==DisplayCal3)
			data_buf[4]=3;
		if(option==DisplayCal4)
			data_buf[4]=4;
	}

	if(option==DisplayData||option==Displaykg||option==Displaylb||option==Displayjin||option==Displaygongjin)
	{
	CS_ScaleDisplay_HexToBcd(showdata,data_buf);
	
	if(option&showhigh4)		//显示高4位
		{
		data_buf[4]=data_buf[3];
		data_buf[3]=data_buf[2];
		data_buf[2]=data_buf[1];
		data_buf[1]=data_buf[0];
		data_buf[0]=0;
		}

		if(data_buf[0]==0 && data_buf[1]==0 )	
			data_buf[1]=Lcdnull_Num;
		if(data_buf[1]==Lcdnull_Num && data_buf[2]==0 )	
			data_buf[2]=Lcdnull_Num;

		if(option&ShowNoDot)	//没有小数点还要判断多一位数字为0时是否不显示
			{
			if(data_buf[2]==Lcdnull_Num && data_buf[3]==0 )	
				data_buf[3]=Lcdnull_Num;
			}
	}
	
	CS_lcd_showbuf[0] = CS_ScaleDisplay_LookUpTable(data_buf[1]);
	CS_lcd_showbuf[1] = CS_ScaleDisplay_LookUpTable(data_buf[2]);
	CS_lcd_showbuf[2] = CS_ScaleDisplay_LookUpTable(data_buf[3]);
	CS_lcd_showbuf[3] = CS_ScaleDisplay_LookUpTable(data_buf[4]);
			
	CS_ScaleDisplay_LcdBuf_rShift4();	//显示缓存跟实际液晶的逻辑对应有出入还要微调

										//显示缓存数字还有符号部分的位置

	//////////////////////////////////////////
	//符号部分赋值
	CS_Lcd_Send_Data[4] &=0x0f;			//清单位符号缓存	
	CS_Lcd_Buf_DOT2	 &= ~S_DOT;			//清显示小数点

	
	if(option==DisplayData)				//最高位用蓝牙标志辅助显示
		{	
		CS_Lcd_Buf_SIGN &=~S_BLE;
		if(data_buf[0]!=0)
		CS_Lcd_Buf_SIGN |=S_BLE;
		}	
	
	if(option==Displaykg)
		{
		CS_Lcd_Buf_DOT2	|= S_DOT;
		CS_Lcd_Buf_Unit	|= S_KG;							
		}	

	if(option==Displaylb)
		{
		CS_Lcd_Buf_DOT2	|= S_DOT;
		CS_Lcd_Buf_Unit	|= S_LB;							
		}
	if(option==Displayjin)
		{
		CS_Lcd_Buf_DOT2	|= S_DOT;
		CS_Lcd_Buf_Unit	|= S_JIN;							
		}
	
	if(option==Displaygongjin)
		{
		CS_Lcd_Buf_DOT2	|= S_DOT;
		CS_Lcd_Buf_Unit	|= S_GONG | S_JIN;							
		}

	if(bleConnStatus==BLE_STATUS_CONN)
		CS_Lcd_Buf_SIGN |=S_BLE;
	else
		CS_Lcd_Buf_SIGN &=~S_BLE;
	
}



/*
      _	  _	   _	    _
ble | |     | |	  | |	   | |	
dat  _	  _	   _	    _		kg
     | |     | |	  | |	   | |	lb
      _	 _    .   _	 .  _		公斤

      2	 3	   4	    5

		SEG1	SEG2	SEG3	SEG4	SEG5	SEG6	SEG7	SEG8	SEG9	SEG10		
COM1			2F		2A		3F		3A		4F		4A		5F		5A		KG
COM2	BLE		2G		2B		3G		3B		4G		4B		5G		5B		LB
COM3	DAT		2E		2C		3E		3C		4E		4C		5E		5C		公
COM4			2D				3D		.00		4D		.0		5D				斤

  __		   A
  
|     |     F      B
  __            G
  
|     |     E      C
  __            D

#define	S_A		CS_Lcd_Buf_Bit0
#define	S_B		CS_Lcd_Buf_Bit1
#define	S_C		CS_Lcd_Buf_Bit2

#define	S_F		CS_Lcd_Buf_Bit4
#define	S_G		CS_Lcd_Buf_Bit5
#define	S_E		CS_Lcd_Buf_Bit6
#define	S_D		CS_Lcd_Buf_Bit7
*/


void CS_ScaleDisplay_LcdBuf_rShift4()
{
	CS_lcd_showbuf[4] = (CS_lcd_showbuf[4] & 0xf0) | (CS_lcd_showbuf[3] & 0x0f);
	CS_lcd_showbuf[3] = (CS_lcd_showbuf[3] & 0xf0) | (CS_lcd_showbuf[2] & 0x0f);
	CS_lcd_showbuf[2] = (CS_lcd_showbuf[2] & 0xf0) | (CS_lcd_showbuf[1] & 0x0f);
	CS_lcd_showbuf[1] = (CS_lcd_showbuf[1] & 0xf0) | (CS_lcd_showbuf[0] & 0x0f);
	CS_lcd_showbuf[0] = CS_lcd_showbuf[0] & 0xf0;

	//清显示缓存数字部分
	CS_Lcd_Send_Data[0]  &= 0x0f;
	CS_Lcd_Send_Data[1]  &= 0x08;
	CS_Lcd_Send_Data[2]  &= 0x08;
	CS_Lcd_Send_Data[3]  &= 0x08;
	CS_Lcd_Send_Data[4]  &= 0xf8;

	//显示缓存数字部分重新赋值
	CS_Lcd_Send_Data[0]  |= CS_lcd_showbuf[0];
	CS_Lcd_Send_Data[1]  |= CS_lcd_showbuf[1];
	CS_Lcd_Send_Data[2]  |= CS_lcd_showbuf[2];
	CS_Lcd_Send_Data[3]  |= CS_lcd_showbuf[3];
	CS_Lcd_Send_Data[4]  |= CS_lcd_showbuf[4];
		
}


void CS_ScaleDisplay_HexToBcd(u16_t HexData,u8_t* buf)
{
	u8_t  i;
	for(i=0;i<5;i++)
		{
		buf[4-i] =  HexData%10;
		HexData = HexData /10;
		}	
}


const u8_t CS_Lcd_Table[] = {
	Lcdch0,Lcdch1,Lcdch2,Lcdch3,Lcdch4,	
	Lcdch5,Lcdch6,Lcdch7,Lcdch8,Lcdch9,	
	LcdchA,Lcdchb,LcdchC,Lcdchd,LcdchE,	
	LcdchF,LcdchL,Lcdcho,LcdchP,Lcdchn,
	Lcdchr,Lcdnull,Lcdup
};


u8_t CS_ScaleDisplay_LookUpTable(u8_t NumData)
{
	u8_t	 * data_ptr;
	data_ptr = CS_Lcd_Table;
	return 	*(data_ptr + NumData);
}




void CS_ScaleDisplay_LcdShowNull(void)
{
	CS_Lcd_Send_Data[0] = 0x00;
	CS_Lcd_Send_Data[1] = 0x00;
	CS_Lcd_Send_Data[2] = 0x00;
	CS_Lcd_Send_Data[3] = 0x00;
	CS_Lcd_Send_Data[4] = 0x00;
}

void CS_ScaleDisplay_LcdShowNoDigit(void)
{
	CS_Lcd_Send_Data[0] &= 0x0f;
	CS_Lcd_Send_Data[1] &= 0x00;
	CS_Lcd_Send_Data[2] &= 0x08;
	CS_Lcd_Send_Data[3] &= 0x08;
	CS_Lcd_Send_Data[4] &= 0xf0;
}

void CS_ScaleDisplay_LcdShowAll(void)
{
	CS_Lcd_Send_Data[0] = 0xFF;
	CS_Lcd_Send_Data[1] = 0xFF;
	CS_Lcd_Send_Data[2] = 0xFF;
	CS_Lcd_Send_Data[3] = 0xFF;
	CS_Lcd_Send_Data[4] = 0xFF;
}


void CS_ScaleDisplay_LcdBackup(void)
{
	CS_Lcd_Send_Data_BAK[0] = CS_Lcd_Send_Data[0];
	CS_Lcd_Send_Data_BAK[1] = CS_Lcd_Send_Data[1];
	CS_Lcd_Send_Data_BAK[2] = CS_Lcd_Send_Data[2];
	CS_Lcd_Send_Data_BAK[3] = CS_Lcd_Send_Data[3];
	CS_Lcd_Send_Data_BAK[4] = CS_Lcd_Send_Data[4];	
}


void CS_ScaleDisplay_LcdRestore(void)
{
	CS_Lcd_Send_Data[0] = CS_Lcd_Send_Data_BAK[0];
	CS_Lcd_Send_Data[1] = CS_Lcd_Send_Data_BAK[1];
	CS_Lcd_Send_Data[2] = CS_Lcd_Send_Data_BAK[2];
	CS_Lcd_Send_Data[3] = CS_Lcd_Send_Data_BAK[3];
	CS_Lcd_Send_Data[4] = CS_Lcd_Send_Data_BAK[4];
}


void CS_ScaleDisplay_SteadyFlash(void)
{	 

	u8_t xdata R_Weight_FlashTime;

	
	if(CS_SoftTimer(GetIfTimeOut) == false)
		{
		R_Weight_FlashTime = CS_SoftTimer(GetSoftTimerTime);
		   if(R_Weight_FlashTime<30) 
		   	CS_ScaleDisplay_LcdBackup();
		    else if(R_Weight_FlashTime<60) 
				CS_ScaleDisplay_LcdShowNoDigit();
		    		else if(R_Weight_FlashTime<90)
					CS_ScaleDisplay_LcdRestore();
		     			else if(R_Weight_FlashTime<120) 
						CS_ScaleDisplay_LcdShowNoDigit();
		   			 	else if(R_Weight_FlashTime<150)
							CS_ScaleDisplay_LcdRestore();		
		}
}








