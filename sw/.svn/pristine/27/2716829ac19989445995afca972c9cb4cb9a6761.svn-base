#include <string.h>
#include "YC_Define.h"
#include "hwreg.h"
#include "YC_Utilities.h"
#include "YC_Peripheral.h"
//#include "btreg.h"
//#include "YC_Sys.h"
#include "Reg320.h"
#include <intrins.h>
#ifdef YC_PERIPHERAL_LED
u8_t xdata YC_LedBlinkTimerID;
LOCAL YC_LedBlink_t xdata YC_LedBlink;
#endif

#ifdef YC_PERIPHERAL_EEPROM
LOCAL volatile u8_t xdata YC_IICDTbuf[4]  _at_ mem_iicd_tbuf;
#endif


#ifdef YC_PERIPHERAL_EEPROM
void YC_WaitIICDone(void);
#endif
void YC_LedOffCallback(void);

#ifdef YC_PERIPHERAL_ADC
/*===============ADC=============================*/

u16_t YC_ReadADC(YC_ADCChannel_e channel)
{
	u16_t x2;
	REG_RF_ADC_GC = 0xaa;
	REG_RFEN_ADC=0x7c;
	REG_GPIO_SEL(0) = 1<<channel;
	REG_RFEN_CHGPUMP &= 0xf8;
	REG_RFEN_CHGPUMP |= channel;
	_nop_();
	_nop_();
	_nop_();
	REG_RFEN_ADC=0x78;
	_nop_();
	_nop_();
	_nop_();
	x2=ESWAP(REG_ADC_IN)&0x03ff;
	REG_RFEN_ADC=0x7c;
	return x2;
}

#endif

/*===============SPI FLASH======================================================================*/





/*===============EEPROM======================================================================*/
#ifdef YC_PERIPHERAL_EEPROM


void YC_EepromRead(u16_t addr, u16_t len, u8_t* pbuff)
{
	u16_t mem_src =  addr;
	REG_IICD_TXLEN = ESWAP(4);
	YC_IICDTbuf[0] = 0xa0;
	*(u16_t*)(YC_IICDTbuf+1) = mem_src;
	YC_IICDTbuf[3] = 0xa1;
	REG_IICD_TXADDR = ESWAP((u16_t)&YC_IICDTbuf);
	REG_IICD_RXLEN = ESWAP(len);
	REG_IICD_RXADDR = ESWAP((u16_t)pbuff);
	REG_IICD_CTRL = 2;
	if(!(REG_IICD_RXADDR&(1<<7)))
	{
		REG_IICD_CTRL |= 1;
	}
	REG_MISC_CTRL |= 1 << 2;	//start iic dma
	YC_WaitIICDone();	
}

LOCAL u16_t min(u16_t num1,u16_t num2)
{
	if(num1<num2)
		return num1;
	return num2;
}

void YC_EepromWrite(u16_t addr, u16_t len, u8_t* pbuff)
{
	u16_t current_len;
	while(len)
	{
		current_len =  (addr|(EEP_PAZE_SIZE-1)) +1 - addr;
		
		current_len = min (current_len,len);
	//	current_len = (current_len<len)?current_len:len;
		len -= current_len;
		YC_Delay10ms(1);
		YC_IICWrite(addr,current_len,pbuff);
		pbuff += current_len;
		addr  += current_len;
	};
}


LOCAL void YC_WaitIICDone(void)
{
	while(!(REG_DMA_STATUS&(1<<5)));
}
/*
void YC_IICRead(u8_t dev_addr, u16_t len, u8_t* pbuff)
{
	REG_IICD_TXLEN = ESWAP(1);
	REG_IICD_TXADDR = ESWAP((u16_t)&dev_addr);
	REG_IICD_RXLEN = ESWAP(len);
	REG_IICD_RXADDR = ESWAP((u16_t)pbuff);
	REG_IICD_CTRL = 1;
	REG_MISC_CTRL |= 1 << 2;	//start iic dma
	YC_WaitIICDone();	
}
*/
void YC_IICWrite(u16_t dev_addr,u16_t len, u8_t* pbuff)
{
	byte ptemp_addr[MAX_IICD_WRITE_LENGTH];
	ptemp_addr[0] = 0xa0;
	xmemcpy(ptemp_addr+3, pbuff, len);
	(u16_t)(ptemp_addr[1]) = dev_addr;
	
//	YC_Delay10ms(1);

	REG_IICD_TXLEN = ESWAP(len+3);
	REG_IICD_TXADDR = ESWAP((u16_t)ptemp_addr);
	REG_IICD_RXLEN = 0;
	REG_IICD_CTRL = 1;

	REG_MISC_CTRL |= 1 << 2;	//start iic dma
	YC_WaitIICDone();	
}
#endif
/*===================GPIO========================================================================*/

bool YC_GPIOGetInputStatus(u8_t gpio_num)
{
	u8_t queue, group;

	queue = (gpio_num & 7);
	group = (gpio_num>>3 & 3);

	return (REG_GPIO_IN(group) & (1<<queue));
}

#if 0
void YC_GPIOSetBit(u8_t gpio_num, char *reg_base, bool bit_val)
{
	u8_t queue, group;

	queue = (gpio_num & 7);
	group = gpio_num >> 3 & 3;

	if(bit_val)
	{
		*(reg_base + group) |= 1<<queue;
	}
	else
	{
		*(reg_base + group) &= ~(1<<queue);
	}
}

void YC_GPIOSetPullup(u8_t gpio_num, bool bit_val)
{
	YC_GPIOSetBit(gpio_num, &REG_GPIO_PUP(0), bit_val);	
}

void YC_GPIOSetPulldown(u8_t gpio_num, bool bit_val)
{
	YC_GPIOSetBit(gpio_num, &REG_GPIO_PDN(0), bit_val);	
}

void YC_GPIOSetOe(u8_t gpio_num, bool bit_val)
{
	YC_GPIOSetBit(gpio_num, &REG_GPIO_OE(0), bit_val);	
}

void YC_GPIOSetOut(u8_t gpio_num, bool bit_val)
{
	YC_GPIOSetBit(gpio_num, &REG_GPIO_OUT(0), bit_val);	
}

void YC_GPIOSet(u8_t gpio_num, bool pup_bit_val ,bool  pdn_bit_val , bool oe_bit_val , bool out_bit_val)
{
	
	YC_GPIOSetBit(gpio_num,&REG_GPIO_PUP(0),pup_bit_val);
	
	YC_GPIOSetBit(gpio_num,&REG_GPIO_PDN(0),pdn_bit_val);

	YC_GPIOSetBit(gpio_num,&REG_GPIO_OE(0),oe_bit_val);

	YC_GPIOSetBit(gpio_num,&REG_GPIO_OUT(0),out_bit_val);
}
#endif

/*
void YC_GPIOSetHighWakeup(u8_t gpio_num)
{
	u8_t queue, group;

	queue = (gpio_num & 7);
	group = (gpio_num>>3 & 3);

	*(YC_GPIOWakeupL+group) &= ~(1<<queue);
	*(YC_GPIOWakeupH+group) |= 1<<queue;	//high wakeup
}



void YC_GPIOSetLowWakeup(u8_t gpio_num)
{
	u8_t queue, group;

	queue = (gpio_num & 7);
	group = (gpio_num>>3 & 3);

	*(YC_GPIOWakeupL+group) |= 1<<queue;	//low wakeup
	*(YC_GPIOWakeupH+group) &= ~(1<<queue);
}


void YC_GPIOClearWakeup(u8_t gpio_num)
{
	u8_t queue, group;

	queue = (gpio_num & 7);
	group = (gpio_num>>3 & 3);
	
	*(YC_GPIOWakeupH+group) &= ~(1<<queue);
	*(YC_GPIOWakeupL+group) &= ~(1<<queue);
}

*/
/*==============LED======================================================================*/
#ifdef YC_PERIPHERAL_LED
void YC_LedOn(u8_t gpio_num, bool active_state)
{
	YC_GPIOSetBit(gpio_num, &REG_GPIO_OE(0), 1);
	YC_GPIOSetBit(gpio_num, &REG_GPIO_OUT(0), active_state);
}

void YC_LedOff(u8_t gpio_num, bool active_state)
{
	YC_GPIOSetBit(gpio_num, &REG_GPIO_OE(0), 1);
	YC_GPIOSetBit(gpio_num, &REG_GPIO_OUT(0), !active_state);
}

void YC_LedOnCallback(void)
{
	YC_LedOn(YC_LedBlink.gpio_num, YC_LedBlink.active_state);
	YC_UtilTimerStartMs(YC_LedBlink.light_time, YC_LedOffCallback, &YC_LedBlinkTimerID);

}

void YC_LedOffCallback(void)
{
	YC_LedOff(YC_LedBlink.gpio_num, YC_LedBlink.active_state);
	YC_UtilTimerStartMs(YC_LedBlink.dark_time, YC_LedOnCallback, &YC_LedBlinkTimerID);

}

void YC_LedStartBlink(bool active, u8_t gpio, u16_t on_time_ms, u16_t off_time_ms)
{
	YC_LedBlink.active_state = active;
	YC_LedBlink.gpio_num = gpio;
	YC_LedBlink.light_time = on_time_ms;
	YC_LedBlink.dark_time = off_time_ms;
	YC_LedOnCallback();
}

void YC_LedStopBlink(void)
{
	YC_LedOff(YC_LedBlink.gpio_num, YC_LedBlink.active_state);
	YC_UtilTimerStop(YC_LedBlinkTimerID);
}
#endif

/*==============UART======================================================================*/
#ifdef YC_PERIPHERAL_UART
#if 0
void YC_UARTInit(YC_UARTPARAM_t* param)
{
	u16_t baud;
	u32_t BaseBaud = 48000000;
	REG_GPIO_SELECT(1) |= BIT(2);		//set GPIO[7:6] as UART pin

	if(param->baud <=12000)
	{
		REG_UART_CLK_SEL &= 0xfe;	//use 24Mhz crystal
		BaseBaud = BaseBaud/2;
	}
	else
		REG_UART_CLK_SEL |= 1;	//use 48Mhz DPLL

	//RX buffer and R/W pointer setting
	REG_UART_RX_ADDR_S = ESWAP((short)param->pRxBuff);
	REG_UART_RX_ADDR_E = ESWAP((short)param->pRxBuff + param->rxBuffLen - 1);
	REG_UART_RX_PTR_R = REG_UART_RX_ADDR_S;

	//TX buffer and R/W pointer setting
	REG_UART_TX_ADDR_S = ESWAP((short)param->pTxBuff);
	REG_UART_TX_ADDR_E = ESWAP((short)param->pTxBuff + param->txBuffLen - 1);
	REG_UART_TX_PTRP_R = REG_UART_TX_ADDR_S;
	REG_UART_TX_PTR_W = REG_UART_TX_ADDR_S;

	//baud rate setting
	baud = ESWAP(REG_UART_BAUD);
	baud &= 0xf000;
	baud |=((BaseBaud/param->baud)&0x0fff);
	REG_UART_BAUD = ESWAP(baud);

	REG_CLK_OFF(1) &= ~ BIT(7);		//turn on UART's clock
	REG_UART_CTRL = param->uartCtrl;			//UART control
}

u16_t YC_UARTReciveData(u8_t* pbuff, u16_t len)
{
	int i = 0;
	u8_t xdata* rrptr = (u8_t *)ESWAP(REG_UART_RX_PTR_R);

	if(!ESWAP(REG_UART_RX_ITEMS))
	{
		return false;
	}

	for(i=0;i<len;i++)
	{
		memcpy(pbuff++, rrptr++, 1);

		if((short)rrptr == ESWAP(REG_UART_RX_ADDR_E) + 1)		//rrptr reach the end of buffer
		{
			rrptr = (u8_t *)ESWAP(REG_UART_RX_ADDR_S);
		}

		if((short)rrptr == ESWAP(REG_UART_RX_PTR_W) )	//read all data already
		{
			REG_UART_RX_PTR_R = ESWAP((short)rrptr);
			return (i+1);
		}
	}

	REG_UART_RX_PTR_R = ESWAP((short)rrptr);
	return i;
}
#endif
void YC_UARTSendData(u8_t* pbuff, u16_t len)
{
	int i;
	u8_t *twptr = (u8_t *)ESWAP(REG_UART_TX_PTR_W);

	for(i=0;i<len;i++)
	{
		xmemcpy(twptr++, pbuff++, 1);

		if(((short)twptr-1)==ESWAP(REG_UART_TX_ADDR_E))	//twptr reach the end of buffer
		{
			REG_UART_TX_PTR_W = REG_UART_TX_ADDR_E;
			while(ESWAP(REG_UART_TX_ITEMS));
			
			twptr = (u8_t *)ESWAP(REG_UART_TX_ADDR_S);	//reset twptr
		}	
	}	

	REG_UART_TX_PTR_W = ESWAP((short)twptr);
	//YC_UARTWaitSendData();
}

void YC_UARTWaitSendData(void)
{
	while(ESWAP(REG_UART_TX_ITEMS));
}

u16_t YC_UARTReciveDataExpected(u8_t* pbuff, u16_t len)
{
	int i = 0;
	u8_t xdata* rrptr = (u8_t *)ESWAP(REG_UART_RX_PTR_R);

	if(ESWAP(REG_UART_RX_ITEMS)<len)
	{
		return false;
	}

	for(i=0;i<len;i++)
	{
		memcpy(pbuff++, rrptr++, 1);

		if((short)rrptr == ESWAP(REG_UART_RX_ADDR_E) + 1)		//rrptr reach the end of buffer
		{
			rrptr = (u8_t *)ESWAP(REG_UART_RX_ADDR_S);
		}

		if((short)rrptr == ESWAP(REG_UART_RX_PTR_W) )	//read all data already
		{
			REG_UART_RX_PTR_R = ESWAP((short)rrptr);
			return (i+1);
		}
	}

	REG_UART_RX_PTR_R = ESWAP((short)rrptr);
	
	return len;
}

void YC_UARTClearBuffer(void)
{
	REG_UART_RX_PTR_R = REG_UART_RX_PTR_W;
	//REG_UART_TX_PTR_W = REG_UART_TX_PTR_R;
}
#endif
 /*==============PWM======================================================================*/
#ifdef YC_PERIPHERAL_PWM
void YC_PWMInitClock()
{
	REG_PWM_CLK_SEL &= ~(0x11 << 6);
	REG_PWM_CLK_SEL |= 1 << 6;				//select PWM clock: system clock undivided 48Mhz
	REG_CLK_OFF(1) &= ~(1 << PWM_CLOCK);	//turn on PWM clock
}

void YC_PWMCloseClock()
{	
	REG_CLK_OFF(1) |= 1 << PWM_CLOCK;	//turn off PWM clock	
}

/*
void YC_PWMConfig(YC_PWMChanel_e PWMChSel, u32_t PWM_Freq, u32_t Duty)
{
	u32_t pwm_count_max = 48000000 / PWM_Freq;
	u16_t PWM_pcnt = 0;
	u16_t PWM_ncnt = 0;
	PWM_pcnt = pwm_count_max * Duty / 100;
	PWM_ncnt = pwm_count_max - PWM_pcnt;	

	switch(PWMChSel){
	case PWM_CH0:
		REG_PWM_PCOUNT(0) = PWM_pcnt & 0xff;
		*(&REG_PWM_PCOUNT(0)+1) = PWM_pcnt >> 8 & 0xff;		//set positive pulse clock count
		REG_PWM_NCOUNT(0) = PWM_ncnt & 0xff;				
		*(&REG_PWM_NCOUNT(0)+1) = PWM_ncnt >> 8 & 0xff;		//set npositive pulse clock count
	break;
	case PWM_CH1:
		REG_PWM_PCOUNT(1) = PWM_pcnt & 0xff;
		*(&REG_PWM_PCOUNT(1)+1) = PWM_pcnt >> 8 & 0xff;	    //set positive pulse clock count
		REG_PWM_NCOUNT(1) = PWM_ncnt & 0xff;
		*(&REG_PWM_NCOUNT(1)+1) = PWM_ncnt >> 8 & 0xff;	    //set npositive pulse clock count
		break;
	default:
		break;
	}
}
*/

void YC_PWMConfig(YC_PWMChanel_e PWMChSel, u16_t PWM_pcnt, u16_t PWM_ncnt)
{
	switch(PWMChSel){
	case PWM_CH0:
		REG_PWM_PCOUNT(0) = PWM_pcnt & 0xff;
		*(&REG_PWM_PCOUNT(0)+1) = PWM_pcnt >> 8 & 0xff;		//set positive pulse clock count
		REG_PWM_NCOUNT(0) = PWM_ncnt & 0xff;				
		*(&REG_PWM_NCOUNT(0)+1) = PWM_ncnt >> 8 & 0xff;		//set npositive pulse clock count
		break;
	case PWM_CH1:
		REG_PWM_PCOUNT(1) = PWM_pcnt & 0xff;
		*(&REG_PWM_PCOUNT(1)+1) = PWM_pcnt >> 8 & 0xff;	    //set positive pulse clock count
		REG_PWM_NCOUNT(1) = PWM_ncnt & 0xff;
		*(&REG_PWM_NCOUNT(1)+1) = PWM_ncnt >> 8 & 0xff;	    //set npositive pulse clock count
		break;
	default:
		break;
	}		
}

void YC_PWMEnableChanel(YC_PWMChanel_e PWMChSel)
{
  	switch(PWMChSel){
	case PWM_CH0:
		REG_PWM_ENABLE |= 1 << 6;
		break;
	case PWM_CH1:
		REG_PWM_ENABLE |= 1 << 7;
			break;
	default:
		break;
	}
}

void YC_PWMDisableChanel(YC_PWMChanel_e PWMChSel)
{
  	switch(PWMChSel){
	case PWM_CH0:
		REG_PWM_ENABLE &= ~(1 << 6);
		break;
	case PWM_CH1:
		REG_PWM_ENABLE &= ~(1 << 7);
			break;
	default:
		break;
	}
}

void YC_PWMOpenChanel(YC_PWMChanel_e PWMChSel, u8_t gpio)
{
	u8_t lebit = 7;
	switch(PWMChSel){
	case PWM_CH0:
		switch(gpio){									        //open PWM0 output pin
			case 4:
				REG_GPIO_SELECT(1) |= 1 << 6;
				break;
			case 3:
				lebit--;
			case 6:
				lebit--;
			case 7:
				lebit--;
			case 18:
				lebit--;
			case 19:
				lebit--;
			case 20:
				lebit--;
			case 21:
				lebit--;
			case 22:
				REG_GPIO_SELECT(2) |= 1 << lebit;		  
				break;
			default:
				break;
		}
		break;
	case PWM_CH1:
		REG_GPIO_SELECT(1) |= 1 << 7;					        //open PWM1 output pin
		break;
	default:
		break;
	}
}

void YC_PWMCloseChanel(u8_t gpio)
{
	u8_t lebit = 7;

	switch(gpio){
		case 5:
			REG_GPIO_SELECT(1) &= ~(1 << 7);					//close PWM1 output pin
			break;			
		case 4:													//close PWM0 output pin
			REG_GPIO_SELECT(1) &= ~(1 << 6);
			break;
		case 3:
			lebit--;
		case 6:
			lebit--;
		case 7:
			lebit--;
		case 18:
			lebit--;
		case 19:
			lebit--;
		case 20:
			lebit--;
		case 21:
			lebit--;
		case 22:
			REG_GPIO_SELECT(2) &= ~(1 << lebit);				//close PWM0 output pin
			break;
		default:
			break;
	}
}
#endif

 /*==============TIME======================================================================*/
#ifdef YC_PERIPHERAL_TIME
 void YC_Time0Init(void)
 {
 	TMOD = (TMOD&0xf0) |0x01;
 	TH0 = (65536-60000)/256;
 	TL0 = (65536-60000)%256;
 	ET0 = 1;
 	EA = 1;
 }

 void YC_Time1Init(void)
 {
 	TMOD = (TMOD&0x0f) |0x10;
 	TH1 = (65536-60000)/256;
 	TL1 = (65536-60000)%256;
 	ET1 = 1;
 	EA = 1;
 }

void YC_TimeStart(YC_TimeChanel_e time_num)
{
	switch(time_num){
		case Time0:
			TR0 = 1;
			break;
		case Time1:
			TR1 = 1;
			break;
		case Time2:
			break;
		default:
			break;
	}
}

void YC_TimeClose(YC_TimeChanel_e time_num)
{
	switch(time_num){
		case Time0:
			TR0 = 0;
			ET0 = 0;
			break;
		case Time1:
			TR1 = 0;
			ET1 = 0;
			break;
		case Time2:
			break;
		default:
			break;
	}
}
#endif

/*==============MATH======================================================================*/
#ifdef YC_PERIPHERAL_MATH
static void YC_Wait_Cordic()	{while(!(HREAD(PERF_STATUS) & 1 << 2));}

double YC_Cordica(double x, byte type)
{
	byte i, t[4];
	u32_t a = 0;
	if(type == YC_TYPE_ASIN)
		a = (u32_t)(x*YC_GAIN);
	else
		a = (u32_t)(x*YC_MAG);
		
	REG_CLK_OFF(0) &= ~(1 << 5);

	if(type == YC_TYPE_TAN)
		HWRITE(CORDIC_TYPE, YC_TYPE_SIN << 4);
	else
		HWRITE(CORDIC_TYPE, type << 4);
	*(u32_t*)t = a;
	for(i = 0;i < 4;i++)
		HWRITE(CORDIC_A + i, t[3 - i]);
	YC_Wait_Cordic();
	if(type == YC_TYPE_TAN) {
		HWRITE(CORDIC_TYPE, type << 4);
		HWRITE(CORDIC_A + 3, 0);
		YC_Wait_Cordic();
	}
	for(i = 0;i < 4;i++)
		t[3 - i] = HREAD(CORDIC_RESULT + i);

	REG_CLK_OFF(0) |= 1 << 5;	
	if(type == YC_TYPE_ASIN)
		return (double)(*(u32_t*)t)/YC_GAIN;
	else
		return (double)(*(u32_t*)t)/YC_MAG;
} 

double YC_Cordicab(double x, double y, byte type)
{
	byte i, t[4];
	u32_t a = (u32_t)(x * YC_MAG);
	u32_t b = (u32_t)(y * YC_MAG);
	REG_CLK_OFF(0) &= ~(1 << 5);
	
	HWRITE(CORDIC_TYPE, type << 4);
	*(u32_t*)t = b;
	for(i = 0;i < 4;i++)
		HWRITE(CORDIC_B + i, t[3 - i]);
	*(u32_t*)t = a;
	for(i = 0;i < 4;i++)
		HWRITE(CORDIC_A + i, t[3 - i]);
	YC_Wait_Cordic();
	for(i = 0;i < 4;i++)
		t[3 - i] = HREAD(CORDIC_RESULT + i);

	REG_CLK_OFF(0) |= 1 << 5;	
	return (double)(*(u32_t*)t)/YC_MAG;
}

double YC_Arcsin(double x)
{
	if (x<0.0761718750)
		return x;
	else if ((x>0.1992187500) & (x<0.1997070313))
		return YC_CompensateArcsin(x,0.1992187500,0.1997070313,0.2005606256,0.2010589199);		
	else if ((x>0.3034667969) & (x<0.3122558594))
		return YC_CompensateArcsin(x,0.3034667969,0.3122558594,0.3083289318,0.3175667024);	
	else if ((x>0.4267578125) & (x<0.4282226563))
		return YC_CompensateArcsin(x,0.4267578125,0.4282226563,0.4409046923,0.4425250592);	
	else if ((x>0.4831542969) & (x<0.4836425781))
		return YC_CompensateArcsin(x,0.4831542969,0.4836425781,0.5042538530,0.5048116334);	
	else if ((x>0.5312500000) & (x<0.5366210938))
		return YC_CompensateArcsin(x,0.5312500000,0.5366210938,0.5600753062,0.5664277055);	
	else if ((x>0.5876464844) & (x<0.5881347656))
		return YC_CompensateArcsin(x,0.5876464844,0.5881347656,0.6281470148,0.6287506208);	

	else if ((x>0.6071777344) & (x<=0.6267089844))
		return YC_CompensateArcsin(x,0.6071777344,0.6267089844,0.6525037913,0.6773227088);	
	else if ((x>0.6267089844) & (x<=0.6462402344))
		return YC_CompensateArcsin(x,0.6267089844,0.6462402344,0.6773227088,0.7026473527);
	else if ((x>0.6462402344) & (x<=0.6657714844))
		return YC_CompensateArcsin(x,0.6462402344,0.6657714844,0.7026473527,0.7285272872);
	else if ((x>0.6657714844) & (x < 0.6853027344))
		return YC_CompensateArcsin(x,0.6657714844,0.6853027344,0.7285272872,0.7550193140);

		
	else if ((x>0.7287597656) & (x<0.7292480469))
		return YC_CompensateArcsin(x,0.7287597656,0.7292480469,0.8165090300,0.8172223602) ;	
	else if ((x>0.7500000000) & (x<0.7504882813))
		return YC_CompensateArcsin(x,0.7500000000,0.7504882813,0.8480620790,0.8488006001);	
	else if ((x>0.7683105469) & (x<0.7705078125))
		return YC_CompensateArcsin(x,0.7683105469,0.7705078125,0.8761975022,0.8796374240);	
	else if ((x>0.8081054688) & (x<0.8088378906))
		return YC_CompensateArcsin(x,0.8081054688,0.8088378906,0.9409286670,0.9421731504);	
	else if ((x>0.8264160156) & (x<0.8269042969))
		return YC_CompensateArcsin(x,0.8264160156,0.8269042969,0.9727124357,0.9735801806) ;	
	else if ((x>0.8349609375) & (x<0.8435058594))
		return YC_CompensateArcsin(x,0.8349609375,0.8435058594,0.9880617986,1.0037772973) ;	
	else if ((x>0.8593750000) & (x<0.8598632813))
		return YC_CompensateArcsin(x,0.8593750000,0.8598632813,1.0340461496,1.0350018114);
	else if ((x>0.8745117188) & (x<0.8754882813))
		return YC_CompensateArcsin(x,0.8745117188,0.8754882813,1.0644281445,1.0664453271) ;
	else if ((x>0.8896484375) & (x<0.8901367188))
		return YC_CompensateArcsin(x,0.8896484375,0.8901367188,1.0965747116,1.0976451051);
	else if ((x>0.9013671875) & (x<0.9038085938))
		return YC_CompensateArcsin(x,0.9013671875,0.9038085938,1.1229162861,1.1285874144);

	else if ((x>0.9106445313) & (x<=0.9167480469))
		return YC_CompensateArcsin(x,0.9106445313,0.9167480469,1.1448412785,1.1598621394);
	else if ((x>0.9167480469) & (x<=0.9228515625))
		return YC_CompensateArcsin(x,0.9167480469,0.9228515625,1.1598621394,1.1754196810);
	else if ((x>0.9228515625) & (x<=0.9289550781))
		return YC_CompensateArcsin(x,0.9228515625,0.9289550781,1.1754196810,1.1915801278);
	else if ((x>0.9289550781) & (x<=0.9350585938))
		return YC_CompensateArcsin(x,0.9289550781,0.9350585938,1.1915801278,1.2084243173);
	else if ((x>0.9350585938) & (x<=0.9392089844))
		return YC_CompensateArcsin(x,0.9350585938,0.9392089844,1.2084243173,1.2203191539);
	else if ((x>0.9392089844) & (x<=0.9433593750))
		return YC_CompensateArcsin(x,0.9392089844,0.9433593750,1.2203191539,1.2326142913);
	else if ((x>0.9433593750) & (x<0.9475097656))
		return YC_CompensateArcsin(x,0.9433593750,0.9475097656,1.2326142913,1.2453551596);

		
	else if ((x>0.9565429688) & (x<0.9570312500))
		return YC_CompensateArcsin(x,0.9565429688,0.9570312500,1.2749062027,1.2765853674) ;
	else if ((x>0.9648437500) & (x<0.9655761719))
		return YC_CompensateArcsin(x,0.9648437500,0.9655761719,1.3048482231,1.3076493663);
	else if ((x>0.9772949219) & (x<0.9799804688))
		return YC_CompensateArcsin(x,0.9772949219,0.9799804688,1.3572944661,1.3703633600);

	else if ((x>0.9865722656) & (x<=0.9885253906))
		return YC_CompensateArcsin(x,0.9865722656,0.9885253906,1.4067360140,1.4191610986);
	else if ((x>0.9885253906) & (x<=0.9904785156))
		return YC_CompensateArcsin(x,0.9885253906,0.9904785156,1.4191610986,1.4326903339);
	else if ((x>0.9904785156) & (x<=0.9924316406))
		return YC_CompensateArcsin(x,0.9904785156,0.9924316406,1.4326903339,1.4476872259);
	else if ((x>0.9924316406) & (x<=0.9943847656))
		return YC_CompensateArcsin(x,0.9924316406,0.9943847656,1.4476872259,1.4647727690);
	else if ((x>0.9943847656) & (x<=0.9963378906))
		return YC_CompensateArcsin(x,0.9943847656,0.9963378906,1.4647727690,1.4851885382);
	else if ((x>0.9963378906) & (x<=0.9982910156))
		return YC_CompensateArcsin(x,0.9963378906,0.9982910156,1.4851885382,1.5123246008);
	else if ((x>0.9982910156) & (x<=1.0000000000))
		return YC_CompensateArcsin(x,0.9982910156,1.0000000000,1.5123246008,1.5707963268);

	else
		return YC_Cordica(x,YC_TYPE_ASIN);
}

static double YC_CompensateArcsin(double parameter,double c1,double c2,double a1, double a2)
{	
	return a1 + (parameter -c1) *(a2 -a1) /(c2 -c1);
}

#endif
