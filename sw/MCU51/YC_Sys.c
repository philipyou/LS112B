#include "YC_Sys.h"
#include "YC_Define.h"
#include "hwreg.h"

#define YC_CLK_N (HREADD(CLKN))

YC_UtilTimer_t xdata YC_UtilTimer;
void YC_UtilTimerInit(void)
{
	u8_t i;
	
	YC_UtilTimer.timer_flages = 0;
	YC_UtilTimer.last_timer = YC_CLK_N;
	for(i = 0; i < YC_UTIL_TIMER_NUM; i++){
		YC_UtilTimer.callback[i] = 0;
		YC_UtilTimer.timer_end[i] = 0;
	}
	return;
}
#if 0
bool YC_UtilTimerStart(u16_t time_in_second, p_TimerCallback callback, pxbyte timer_id)
{
	u8_t i = 0;
	
	//do we have enough timers?
	if(YC_UtilTimer.timer_flages == 0xff){
		return false;
	}
	while(i < YC_UTIL_TIMER_NUM){
		if(!((YC_UtilTimer.timer_flages >> i) & 1)){
			if(timer_id)
				*timer_id = i;
			break;
		}
		++i;
	}
	YC_UtilTimer.timer_flages |= 1 << i;
	YC_UtilTimer.callback[i] = callback;
	YC_UtilTimer.timer_end[i] =YC_CLK_N + (u32_t)time_in_second * 3200;
//	while(1);
	return true;
}
#endif
bool YC_UtilTimerStartMs(u16_t time_in_ms, p_TimerCallback callback, pxbyte timer_id)
{
	u8_t i = 0;
	
	//do we have enough timers?
	if(YC_UtilTimer.timer_flages == 0xff){
		return false;
	}
	while(i < YC_UTIL_TIMER_NUM){
		if(!((YC_UtilTimer.timer_flages >> i) & 1)){
			if(timer_id)
				*timer_id = i;
			break;
		}
		++i;
	}
	YC_UtilTimer.timer_flages |= 1 << i;
	YC_UtilTimer.callback[i] = callback;
	YC_UtilTimer.timer_end[i] = YC_CLK_N + ((u32_t)time_in_ms << 4)/5;
	return true;
}

bool YC_UtilTimerStop( u8_t timer_id) large
{
	if((timer_id > 8) ||!((YC_UtilTimer.timer_flages >> timer_id) & 1))
		return false;
	YC_UtilTimer.timer_flages &= ~( 1<< timer_id);
	YC_UtilTimer.callback[timer_id] = 0;
	YC_UtilTimer.timer_end[timer_id] = 0;
	return true;
}

void YC_UtilTimerProcess(void) small
{
	u8_t i;
#if 0
	if(YC_UtilTimer.last_timer > (YC_CLK_N + 0x1000)){
		for(i = 0; i < YC_UTIL_TIMER_NUM; ++i){
			if((YC_UtilTimer.timer_flages >> i) & 1){
				YC_UtilTimer.timer_end[i] -= YC_UtilTimer.last_timer;
			}
		}
	}
	YC_UtilTimer.last_timer = YC_CLK_N;
	for(i = 0; i < YC_UTIL_TIMER_NUM; ++i){
		if((YC_UtilTimer.timer_flages >> i) & 1){
			if(YC_UtilTimer.timer_end[i] < YC_CLK_N){
				YC_UtilTimer.timer_flages &=~( 1<< i); // clear the flag
				YC_UtilTimer.callback[i](i);			
			}
		}
	}
#else
	YC_UtilTimer.last_timer = YC_CLK_N;
	for(i = 0; i < YC_UTIL_TIMER_NUM; ++i){
		if((YC_UtilTimer.timer_flages >> i) & 1){
			if((YC_UtilTimer.timer_end[i] < YC_UtilTimer.last_timer) || 
			   (YC_UtilTimer.timer_end[i]  - YC_UtilTimer.last_timer > 0x1000)){
				YC_UtilTimer.timer_flages &=~( 1<< i); // clear the flag
				YC_UtilTimer.callback[i](i);			
			}
		}
	}
#endif
	return;
}



