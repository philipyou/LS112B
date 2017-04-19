#include "YC_Utilities.h"
#include "hwreg.h"

void YC_Ice_break()
{
	CPUB_ICE_CTRL = 1;
	return;
}

void xmemset(pxbyte addr, u8_t value, u8_t len) small
{
	while(len--)
		*addr++ = value;
	return;
}
void xmemcpy(pxbyte dest, u8_t* src, u8_t len) small
{
	while(len--)
		*dest++ = *src++;
	return;
}
void dmemcpy(u8_t *dest, u8_t* src, u8_t len) small
{
	while(len--)
		*dest++ = *src++;
	return;
}
void cmemcpy(pxbyte dest, const u8_t* src, u8_t len) small
{
	while(len--)
		*dest++ = (u8_t)(*src++);
	return;
}
bool YC_MemCheckEmpty(u8_t *mem, u8_t len)
{
	int i;
	for(i=0;i<len;i++)
	{
		if(mem[i])
		{
			break;
		}
		if(i==(len-1)&&mem[i]==0)
		{
			return 0;
		}
	}
	return 1;
}

void YC_Delay1ms(u8_t Time)
{
	u16_t j; 
	for(;Time>0;Time--)
	{
		for(j=0;j<1160;j++);
	}
}

void YC_Delay10ms(u8_t Time)
{
	u16_t j; 
	for(;Time>0;Time--)
	{
		for(j=0;j<11900;j++);
	}
}

void YC_Delay1S(u8_t Time)
{
	u16_t j,k;
	for(;Time>0;Time--)
	{
		for(j=0;j<1000;j++)
		{
			for(k=0;k<1150;k++);
		}
	}
}


