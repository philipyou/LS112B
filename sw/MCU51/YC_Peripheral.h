#ifndef _YC_PERIPHERAL_H_
#define _YC_PERIPHERAL_H_
#include "YC_Define.h"
//#include "Bluetooth.h"

//#define YC_PERIPHERAL_TIME 
//#define YC_PERIPHERAL_PWM 
//#define YC_PERIPHERAL_MATH 
//#define YC_PERIPHERAL_LED
#define YC_PERIPHERAL_UART
//#define YC_PERIPHERAL_ADC
//#define YC_PERIPHERAL_EEPROM
#define YC_PERIPHERAL_SPIFLASH


#define GPIO_ACTIVE_BIT	(1<<7)

#define LED_STOP 0
#define LED_START 1
#define LED_DARK_TIME 2
#define LED_LIGHT_TIME 3

#define EEP_PAZE_SIZE  16

#define MAX_IICD_WRITE_LENGTH 19
#define MAX_EEPROM_WRITE_LENGTH (MAX_IICD_WRITE_LENGTH -3)

#ifdef YC_PERIPHERAL_MATH
#define YC_MAG				                           65536
#define YC_GAIN	                                                      1073741824
#define YC_PI                                                                3.1415926
#define YC_Sin(x)                                                        YC_Cordica(x,YC_TYPE_SIN)                       //x(radian):the parameter of sin
#define YC_Arctan(x)                                                  YC_Cordica(x,YC_TYPE_ATAN)                  //x(radian):the parameter of atan
#define YC_Sqrt(x)                                                       YC_Cordica(x,YC_TYPE_SQRT)                 //x:the parameter of sqrt  
#define YC_Cos(x)                                                       YC_Cordica(x,YC_TYPE_COS)                   //x(radian):the parameter of cos
#define YC_Tan(x)                                                        YC_Cordica(x,YC_TYPE_TAN)                    //x(radian):the parameter of tan
#define YC_Mult(x,y)                                                   YC_Cordicab(x,y,YC_TYPE_MULT)            //x:the parameterA of mult,y:the parameterB of mult
#define YC_Div(x,y)                                                     YC_Cordicab(x,y,YC_TYPE_DIV)                //x:the parameterA of div,y:the parameterB of div
#endif

#ifdef YC_PERIPHERAL_EEPROM
typedef struct YC_Eeprom{
	u8_t device_addr;
	u16_t size;
	u16_t page_size;
}YC_Eeprom_t;
#endif

typedef struct YC_LedBlink{
	u8_t active_state;
	u8_t gpio_num;
	u16_t light_time;
	u16_t dark_time;
}YC_LedBlink_t;


#ifdef YC_PERIPHERAL_UART

typedef struct YC_UARTPARAM{
	u8_t uartCtrl;
	u8_t *pRxBuff;
	u8_t *pTxBuff;
	u16_t rxBuffLen;
	u16_t txBuffLen;
	u32_t baud;//uart baud 
} YC_UARTPARAM_t;

typedef enum YC_UartCTRLBit{
	UART_Enable=0,//Enable UART
	UART_Parity=2,//1: odd parity; 0: even parity
	UART_DataLen=3,//UART Data length 1: 9 bits; 0: 8 bits
	UART_EnableFlowControl=4,//1: enable RTS/CTS flow control
	UART_CardProtcol=5,//1: enable 7816 smart card protocol 
	UART_HalfDuplex=6,//:enable half duplex single line UART
	UART_BaudSet=7//1: Use baud settings 0:auto detect baud rate
}YC_UartCTRLBit_e;

#define UART_CONTROLSET ((1<<UART_Enable)|(1<<UART_BaudSet))

#endif

#ifdef YC_PERIPHERAL_PWM
//PWM Chanel
typedef enum YS_PWMChanel {
	PWM_CH0 = 0,
	PWM_CH1 = 1,
} YC_PWMChanel_e;
#endif

#ifdef YC_PERIPHERAL_TIME
//Time Chanel
typedef enum YS_TimeChanel {
	Time0 = 0,
	Time1 = 1,
	Time2 = 2,
} YC_TimeChanel_e;
#endif

#ifdef YC_PERIPHERAL_ADC
//ADC Chanel
typedef enum YC_ADCChannel {
	ADC_Channel1_GPIO5 = 0,
	ADC_Channel2_GPIO6 = 1,
	ADC_Channel3_GPIO7 = 2,
	ADC_Channel4_GPIO18 = 3,
	ADC_Channel5_GPIO19 = 4,
	ADC_Channel6_GPIO20 = 5,
	ADC_Channel7_GPIO21 = 6,
	ADC_Channel8_GPIO22 = 7
} YC_ADCChannel_e;
#endif

#ifdef YC_PERIPHERAL_ADC

u16_t YC_ReadADC(YC_ADCChannel_e channel);

#endif

#ifdef YC_PERIPHERAL_EEPROM
/*==============read eeprom
input:
	addr:eeprom device addresss
	len:eeprom size
	pbuff:read eeprom beginning at the address
no output
====================*/
void YC_EepromRead(u16_t addr, u16_t len, u8_t* pbuff);
/*==============write eeprom
input:
	addr:eeprom device addresss
	len:eeprom size
	pbuff:write eeprom beginning at the address
no output
====================*/
void YC_EepromWrite(u16_t addr,u16_t len, u8_t* pbuff);
/*==============read IIC value
input:
	dev_addr:device address
	len:the that you want to read
	pbuff:read IIC beginning at the address
no output
====================*/
//void YC_IICRead(u8_t dev_addr, u16_t len, u8_t* pbuff);
/*==============write IIC value
input:
	dev_addr:device address
	len:the that you want to read
	pbuff:write IIC beginning at the address
no output
====================*/
void YC_IICWrite(u16_t dev_addr,u16_t len, u8_t* pbuff);
/*==============being set GPIO pull up
input:
	gpio_num:wanting to set GPIO_PUP  number 
	bit_val:wanting to set GPIO value.one(1) is mean GPIO status is pull up
no output
====================*/
#endif
void YC_GPIOSetPullup(u8_t gpio_num, bool bit_val);
/*==============being set GPIO pull down
input:
	gpio_num:wanting to set GPIO_PDN  number 
	bit_val:wanting to set GPIO value.one(1) is mean GPIO status is pull down
no output
====================*/
void YC_GPIOSetPulldown(u8_t gpio_num, bool bit_val);
/*==============being set GPIO is output or input
input:
	gpio_num:wanting to set GPIO  number:
	bit_val:wanting to set GPIO_OE value.one(1) is mean GPIO status is output.zero(0) is mean GPIO status is input
no output
====================*/
void YC_GPIOSetOe(u8_t gpio_num, bool bit_val);
/*==============being set GPIO level 
input:
	gpio_num:wanting to set GPIO  number:
	bit_val:wanting to set GPIO_OUT value.one(1) is mean GPIO status is High.zero(0) is mean GPIO status is low
no output
====================*/
void YC_GPIOSetOut(u8_t gpio_num, bool bit_val);
/*==============being set GPIO
input:
	gpio_num:wanting to set GPIO  number
	pup_bit_val:wanting to set GPIO_PUP value.one(1) is mean GPIO status is pull up
	pdn_bit_val:wanting to set GPIO_PDN value.one(1) is mean GPIO status is pull down
	oe_bit_val:wanting to set GPIO_OE value.one(1) is mean GPIO status is output.zero(0) is mean GPIO status is input
	out_bit_val:wanting to set GPIO_OUT value.one(1) is mean GPIO status is High.zero(0) is mean GPIO status is low
no output
====================*/
void YC_GPIOSet(u8_t gpio_num, bool pup_bit_val ,bool  pdn_bit_val , bool oe_bit_val , bool out_bit_val);
/*==============get GPIO input status
input:
	gpio_num:gpio_num want to get
output:
	GPIO input value.if output is 0,GPIO input is low.if putput is 1,GPIO input is high
====================*/
bool YC_GPIOGetInputStatus(u8_t gpio_num);
/*==============Setting GPIO wake-high
input:
	gpio_num:wanting to wake up GPIO number 
no output
====================*/
//void YC_GPIOSetHighWakeup(u8_t gpio_num);
/*==============Setting GPIO wake-low
input:
	gpio_num:wanting to wake up GPIO number 
no output
====================*/
//void YC_GPIOSetLowWakeup(u8_t gpio_num);
/*==============clearing GPIO wake up
input:
	gpio_num:wanting to clear wake up GPIO number 
no output
====================*/
//void YC_GPIOClearWakeup(u8_t gpio_num);
/*==============Lit LED light
input:
	gpio_num:the GPIO number which connects LED light
	active_state:Let the LED light electric level.1 is high level let LED light,0 is low level let LED 
no output
====================*/
void YC_LedOn(u8_t gpio_num, bool active_state);
/*==============Lit LED slake
input:
	gpio_num:the GPIO number which connects LED light
	active_state:Let the LED light electric level.1 is high level let LED light,0 is low level let LED 
no output
====================*/
void YC_LedOff(u8_t gpio_num, bool active_state);
/*==============LED start blink
input:
	active:Let the LED light electric level.1 is high level let LED light,0 is low level let LED 
	gpio:the GPIO number which connects LED light
	on_time_ms:LED light time
	off_time_ms:LED dark time
no output
====================*/
void YC_LedStartBlink(bool active, u8_t gpio, u16_t on_time_ms, u16_t off_time_ms);
/*==============LED stop blink
no input:
no output
====================*/
void YC_LedStopBlink(void);
/*==============initialized UART 
input:
	param:UART parameter Structure
no output
====================*/
#ifdef YC_PERIPHERAL_UART
void YC_UARTInit(YC_UARTPARAM_t* param);
/*==============received UART data
input:
	pbuff:Receiving data beginning at the address
	len:The desired length of the received data
output:
	The actual length of the received data
====================*/
u16_t YC_UARTReciveData(u8_t* pbuff, u16_t len);
/*==============received UART data; the funtion will wait len data then return 
input:
	pbuff:Receiving data beginning at the address
	len:The desired length of the received data
output:
	length of the received data of flase
====================*/
u16_t YC_UARTReciveDataExpected(u8_t* pbuff, u16_t len);

/*==============send UART data
input:
	pbuff:sending data beginning at the address
	len:The desired length of the send data
output:
====================*/
void YC_UARTSendData(u8_t* pbuff, u16_t len);
void YC_UARTWaitSendData(void);
/*=============receive UART data=================
input:
	pbuff:Destination address
	len:length of data that expected
output:
	len
================================================*/
u16_t YC_UARTReciveDataExpected(u8_t* pbuff, u16_t len);

/*=============clear uart buffer=============*/
void YC_UARTClearBuffer(void);
#endif

#ifdef YC_PERIPHERAL_PWM
/*==============initialized PWM Clock
no input
no output
====================*/
void YC_PWMInitClock();

/*==============Close PWM Clock
no input
no output
====================*/
void YC_PWMCloseClock();

/*==============configurate PWM 
input:
	PWM_ch_sel:pwm channel select
		PWM_Freq:PWM channel frequency
	Duty:Duty ratio
no output
pwm_count_max = 48M/PWM_Freq = PWM_pcnt+PWM_ncnt
Duty ratio = (PWM_pcnt/(PWM_pcnt+PWM_ncnt))*100%
====================*/
/*
void YC_PWMConfig(YC_PWMChanel_e PWMChSel, u32_t PWM_Freq, u32_t Duty);
*/

/*==============configurate PWM 
input:
	PWM_ch_sel:pwm channel select
	PWM_pcnt:PWM channel positive pulse clock count
	PWM_ncnt:PWM channel npositive pulse clock count
no output
PWM channel frequency = (48M/(PWM_pcnt+PWM_ncnt))Hz
Duty ratio = (PWM_pcnt/(PWM_pcnt+PWM_ncnt))*100%
====================*/
void YC_PWMConfig(YC_PWMChanel_e PWMChSel, u16_t PWM_pcnt, u16_t PWM_ncnt);

/*==============Enable PWM Chanel
input:
	PWM_ch_sel:pwm channel select
no output
====================*/
void YC_PWMEnableChanel(YC_PWMChanel_e PWMChSel);

/*==============Disable PWM Chanel
input:
	PWM_ch_sel:pwm channel select
no output
====================*/
void YC_PWMDisableChanel(YC_PWMChanel_e PWMChSel);

/*==============Open PWM output pin
input:
	PWM_ch_sel:pwm channel select
	gpio:PWM channel IO select
no output
PWMChSel:2组PWM通道的选择，有效值为PWM_CH0,PWM_CH1分别对应PWM0,PWM1的选择。
gpio: 每组PWM通道引脚的选择，如果PWM通道选择PWM_CH0，则gpio有效值为3,4,6,7,18,19,20,21,22
                                                                                如果PWM通道选择PWM_CH1，则gpio有效值为5
====================*/
void YC_PWMOpenChanel(YC_PWMChanel_e PWMChSel, u8_t gpio);

/*==============Close PWM output pin
input:
	gpio:PWM channel IO select
no output
====================*/
void YC_PWMCloseChanel(u8_t gpio);
#endif

#ifdef YC_PERIPHERAL_TIME
/*==============initialized Time0
no input
no output
time=1/48M*4*60000s=5ms
====================*/
 void YC_Time0Init(void);

/*==============initialized Time1
no input
no output
time=1/48M*4*60000s=5ms
====================*/
 void YC_Time1Init(void);

/*==============Start TimeChanel
input:
	time_num:Time channel select
no output
====================*/
 void YC_TimeStart(YC_TimeChanel_e time_num);

/*==============Close TimeChanel
input:
	time_num:Time channel select
no output
====================*/
 void YC_TimeClose(YC_TimeChanel_e time_num);
#endif

#ifdef YC_PERIPHERAL_MATH
/*==============wait cordic finish
no input
no output
====================*/
static void wait_cordic();

/*==============cordic with one parameter
input:
	a:parameterA
	type:cordic mode
output:
	 the result of cordic
====================*/
double YC_Cordica(double x, byte type);

/*==============cordic with two parameter
input:
	a:parameterA
	b:parameterB
	type:cordic mode
output:
	 the result of cordic
====================*/
double YC_Cordicab(double x, double y, byte type);

/*==============arcsin
input:
	x(radian):the parameter of arcsin
output:
	 the result of arcsin
====================*/
double YC_Arcsin(double x);

static double YC_CompensateArcsin(double parameter,double c1,double c2,double a1, double a2);
#endif

#endif
