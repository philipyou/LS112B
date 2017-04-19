#ifndef _YC_SYSTEM_H_
#define _YC_SYSTEM_H_
#include "YC_Define.h"

//the total timer numbers, max 8
#define YC_UTIL_TIMER_NUM 8
typedef void (*p_TimerCallback) (u8_t timer_id );
//timer structure
typedef struct YC_UtilTimer{
	u8_t timer_flages; 	//flags to control open or close the timer;
	p_TimerCallback callback[YC_UTIL_TIMER_NUM];//callback functions when timeout
	u32_t timer_end[YC_UTIL_TIMER_NUM];
	u32_t last_timer;//
}YC_UtilTimer_t;

void YC_UtilTimerInit(void);

/*==============start timer (seconds unit)
input:
	time_in_second:Cycle size(seconds unit)
	callback:when time out ,the call back function
	timer_id:timers ID
no output
====================*/
//bool YC_UtilTimerStart(u16_t time_in_second, p_TimerCallback callback, pxbyte timer_id);
/*==============start timer (millisecond unit)
input:
	time_in_ms:Cycle size(millisecond unit)
	callback:when time out ,the call back function
	timer_id:timers ID
no output
====================*/
bool YC_UtilTimerStartMs(u16_t time_in_ms, p_TimerCallback callback, pxbyte timer_id);
/*==============stop timer
input:
	timer_id:timers ID
no output
====================*/
bool YC_UtilTimerStop( u8_t timer_id);

void YC_UtilTimerProcess(void) small;




#endif
