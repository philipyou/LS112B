#ifndef __SYSTEM_H
#define __SYSTEM_H
#include "YC_Define.h"

#define SYS_ST_IDLE		1	//system idle state, MCU can enter LPM mode, scale is not working, bluetooth is working
#define SYS_ST_RUN		2	//system run state, MCU can't enter LPM mode, scale is working
#define SYS_ST_SLEEP	3	//system sleep state, MCU is in LPM mode or Hibernate mode, scale and bluetooth is not working

extern	u8_t sys_state;

void systemInit(void);
void BTControlProcess(void);
u8_t generateChecksum(u8_t *buf,u8_t len);
void SysStatusProcess(void) ;

#endif
