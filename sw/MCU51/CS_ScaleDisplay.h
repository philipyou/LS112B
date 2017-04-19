#ifndef __ScaleDisplay_H
#define __ScaleDisplay_H

#define	CS_ScaleDisplay_L_kg		0	
#define	CS_ScaleDisplay_L_lb		1
#define	CS_ScaleDisplay_L_gongjin	2
#define	CS_ScaleDisplay_L_jin		3
#define	CS_ScaleDisplay_L_err		4


#define	CS_Lcd_Buf_Bit0				0x01
#define	CS_Lcd_Buf_Bit1				0x02
#define	CS_Lcd_Buf_Bit2				0x04
#define	CS_Lcd_Buf_Bit3				0x08
#define	CS_Lcd_Buf_Bit4				0x10
#define	CS_Lcd_Buf_Bit5				0x20
#define	CS_Lcd_Buf_Bit6				0x40
#define	CS_Lcd_Buf_Bit7				0x80



//=========================================
//数码管的A~G段和显示缓存的位对应关系
//=========================================
//  __	    A		      0			
// |  |	F	  B 	  5	   1 	
//  __	    G		      6		
// |  |	E	  C	  4      2		
//  __  	    D		      3		
//-----------------------------------------------------
#define	S_A					CS_Lcd_Buf_Bit0
#define	S_B					CS_Lcd_Buf_Bit1
#define	S_C					CS_Lcd_Buf_Bit2
#define	S_D					CS_Lcd_Buf_Bit7
#define	S_E					CS_Lcd_Buf_Bit6
#define	S_F					CS_Lcd_Buf_Bit4
#define	S_G					CS_Lcd_Buf_Bit5

#define   S_ST_DOT			CS_Lcd_Buf_Bit3				
#define	S_DOT				CS_Lcd_Buf_Bit3
#define   S_DAT				CS_Lcd_Buf_Bit2
#define   S_BLE				CS_Lcd_Buf_Bit1
#define	S_KG				CS_Lcd_Buf_Bit4
#define	S_LB				CS_Lcd_Buf_Bit5
#define	S_GONG				CS_Lcd_Buf_Bit6
#define	S_JIN				CS_Lcd_Buf_Bit7


#define	CS_Lcd_Buf_SIGN	CS_Lcd_Send_Data[0]	
#define	CS_Lcd_Buf_DOT1	CS_Lcd_Send_Data[2]		//S_ST_DOT
#define	CS_Lcd_Buf_DOT2	CS_Lcd_Send_Data[3]		//S_DOT
#define	CS_Lcd_Buf_Unit		CS_Lcd_Send_Data[4]	


//=========================================
//	数字符号编码表
//=========================================
//  __		A			
// |  | 	  F	      B 	
//  __		G		
// |  |      E         C		
//  __  		D
//-----------------------------------------------------

#define	Lcdch0		S_A+S_B+S_C+S_D+S_E+S_F
#define	Lcdch1		S_B+S_C
#define	Lcdch2   	S_A+S_B+S_D+S_E+S_G
#define	Lcdch3   	S_A+S_B+S_C+S_D+S_G
#define	Lcdch4   	S_B+S_C+S_F+S_G
#define	Lcdch5   	S_A+S_C+S_D+S_F+S_G
#define	Lcdch6   	S_A+S_C+S_D+S_E+S_F+S_G
#define	Lcdch7   	S_A+S_B+S_C
#define	Lcdch8   	S_A+S_B+S_C+S_D+S_E+S_F+S_G
#define	Lcdch9   	S_A+S_B+S_C+S_D+S_F+S_G
	
#define	LcdchA   	S_A+S_B+S_C+S_E+S_F+S_G
#define	Lcdchb   	S_C+S_D+S_E+S_F+S_G
#define	LcdchC   	S_A+S_D+S_E+S_F
#define	Lcdchd   	S_B+S_C+S_D+S_E+S_G
#define	LcdchE   	S_A+S_D+S_E+S_F+S_G
#define	LcdchF   	S_A+S_E+S_F+S_G

#define	LcdchL		S_D+S_E+S_F
#define	Lcdcho		S_C+S_D+S_E+S_G
#define	LcdchP		S_A+S_B+S_E+S_F+S_G
#define	Lcdchn		S_C+S_E+S_G
#define	Lcdchr		S_E+S_G
#define	Lcdnull		0
#define	Lcdup		S_A					//上划线

#define	LcdchA_Num		10
#define	Lcdchb_Num		11
#define	LcdchC_Num		12
#define	Lcdchd_Num		13
#define	LcdchE_Num		14
#define	LcdchF_Num		15
#define	LcdchL_Num		16
#define	Lcdcho_Num		17
#define	LcdchP_Num		18
#define	Lcdchn_Num		19
#define	Lcdchr_Num		20
#define	Lcdnull_Num		21
#define   Lcdup_Num		22

extern u8_t xdata CS_Lcd_Send_Data[6];

void CS_ScaleDisplay_Debug();


void CS_ScaleDisplay();
void CS_ScaleDisplay_LcdBuf_rShift4();



 
#define	showhigh4					0x40	//显示缓存有5位，显示只有4位
											//选择显示高四位的标志，否则默认低四位
#define	ShowNoDot					0x20	//没有小数点(用于控制高位如果是0时不显示的位数)
			
#define	DisplayCal1					0x81
#define	DisplayCal2					0x82
#define	DisplayCal3					0x83
#define	DisplayCal4					0x84
#define	DisplayOverLoad				0x85
#define	DisplayLowBat				0x86
#define	DisplayZeroErr				0x87

#define	DisplayData					ShowNoDot+1		
#define	Displaykg					showhigh4+1
#define	Displaylb					showhigh4+2
#define	Displayjin					showhigh4+3
#define	Displaygongjin				showhigh4+4

void CS_ScaleDisplay_FreshBuf(u8_t option,u16_t showdata);
void CS_ScaleDisplay_HexToBcd(u16_t HexData,u8_t* buf);
u8_t CS_ScaleDisplay_LookUpTable(u8_t NumData);


void CS_ScaleDisplay_LcdShowNull(void);
void CS_ScaleDisplay_LcdShowAll(void);
void CS_ScaleDisplay_LcdBackup(void);
void CS_ScaleDisplay_LcdRestore(void);
void CS_ScaleDisplay_SteadyFlash(void);



#endif


