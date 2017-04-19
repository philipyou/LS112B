#ifndef _YC_UTILITIES_H_
#define _YC_UTILITIES_H_
#include "YC_Define.h"
/*==============Set a breakpoint function
no input:
no output
====================*/
void YC_Ice_break();
void xmemset(pxbyte addr, u8_t value, u8_t len) small;
/*==============xmem copy function
input:
	src:the address of source
	dest:the address of destination in xpdata
	len:the length that you want copy array
no output
====================*/
void xmemcpy(pxbyte dest, u8_t* src, u8_t len) small;

/*==============data copy function
input:
	src:the address of source
	dest:the address of destination in xpdata
	len:the length that you want copy array
no output
====================*/
void dmemcpy(u8_t *dest, u8_t* src, u8_t len) small;

void cmemcpy(pxbyte dest, const u8_t* src, u8_t len) small;

//return 1 if not empty
bool YC_MemCheckEmpty(u8_t *mem, u8_t len);
/*============Delay a certain number of milliseconds; On the basis of 1 millisecond
input:
	Time-delay number of one milliseconds
no output
====================*/
void YC_Delay1ms(u8_t Time);
/*============Delay a certain number of milliseconds; On the basis of 10 millisecond
input:
	Time-delay number of ten milliseconds
no output
====================*/
void YC_Delay10ms(u8_t Time);
/*============A certain number of seconds of delay;On the basis of 1 second
input:
	Time-delay number of seconds
no output
====================*/
void YC_Delay1S(u8_t Time);


#endif
