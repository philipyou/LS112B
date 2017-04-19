#ifndef _YC_DEFINE_H_
#define _YC_DEFINE_H_

#define PUBLIC 
#define LOCAL static

typedef unsigned char byte;
typedef unsigned char data dbyte;
typedef unsigned char idata ibyte;
typedef unsigned char xdata xbyte;
typedef unsigned int uint;
typedef data unsigned int dint;
typedef unsigned char xdata * pxbyte;
typedef unsigned char xdata * data pxdbyte;
typedef unsigned char idata * pdbyte;
typedef unsigned   char    u8_t;
typedef signed     char    s8_t;
typedef unsigned   short   u16_t;
typedef signed     short   s16_t;
typedef unsigned   long    u32_t;
typedef signed     long    s32_t;
typedef bit bool;

#define true 1
#define false 0
#ifndef NULL
#define NULL (0)
#endif

#endif

