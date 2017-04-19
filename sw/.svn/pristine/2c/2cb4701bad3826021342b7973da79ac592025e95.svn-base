#ifndef _HWREG_H_
#define _HWREG_H_


#define HWRITED(reg, dword)	do{HWRITE(reg, (dword) & 0xff); \
							HWRITE(reg + 1, (dword) >> 8 & 0xff); \
							HWRITE(reg + 2, (dword) >> 16 & 0xff); \
							HWRITE(reg + 3, (dword) >> 24 & 0xff); \
							}while(0)

#define HREADD(reg)			((u32_t)HREAD(reg)  | (u32_t)HREAD(reg + 1) << 8 | (u32_t)HREAD(reg + 2) << 16 | (u32_t)HREAD(reg + 3) << 24)

#define ESWAP(word)			((word) >> 8 & 0xff | (word) << 8 & 0xff00)

#define HREADW(reg)			(*((volatile unsigned int xdata *)(reg)))
#define HWRITEW(mem, word)	HREADW(mem) = ESWAP(word)
#define HREADWS(reg)			ESWAP(HREADW(reg))

#define HREAD(reg)			(*(volatile unsigned char xdata *)(reg))
#define HWRITE(reg, value)		*((volatile unsigned char xdata *)(reg)) = value

#define SETBIT(reg, value)		HWRITE(reg,( (HREAD(reg)) | (value)))
#define CLRBIT(reg, value)		HWRITE(reg, (HREAD(reg) & (~(value))))
#define READBIT(reg, value)		((HREAD(reg)) & (value))
#define BIT(x)					(1 << (x))


#define PWM_EN			0x8084

#define GPIO_OE(X)                0x8070 + X
#define GPIO_OUT(X)                0x8074 + X
#define GPIO_PUP(X)                0x8078 + X
#define GPIO_PDN(X)                0x807c + X
#define GPIO_SEL(X)            0x8080 + X
#define GPIO_KEY(X)            0x8083 + X

#define GPIO_IN(X)                  0x811c + X


#define PWM_PCOUNT(X) (0x80a0 + X*4)
#define PWM_NCOUNT(X) (0x80a2 + X*4)

#define CLOCK_SEL			0x8044
#define CLOCK_SEL_12M		9

#define LPM_EXENOE		0x813e

// math cordic
#define CORDIC_TYPE    	0x8053
#define CORDIC_A     		0x80a8
#define CORDIC_B     		0x80ac
#define PERF_STATUS		0x812d
#define CORDIC_RESULT     	0x8120

#define YC_TYPE_SIN		0
#define YC_TYPE_ATAN	1
#define YC_TYPE_MULT	2
#define YC_TYPE_DIV		3
#define YC_TYPE_ASIN	         4
#define YC_TYPE_SQRT	5
#define YC_TYPE_COS		6
#define YC_TYPE_TAN		7

// DMA UART
#define UART_BAUD     0x8052
#define UART_RSADDR   0x8054
#define UART_READDR   0x8056 
#define UART_RRPTR    0x8058
#define UART_TSADDR   0x805a 
#define UART_TEADDR   0x805c 
#define UART_TWPTR    0x805e
#define UART_TRPTRP   0x8060 
#define UART_CTRL     0x8062 
#define CLKN	0x8100

#define UART_STATUS  0x810c
#define UART_RBAUD   0x810d 
#define UART_TXITEMS 0x810e 
#define UART_TRPTR   0x8110 
#define UART_RXITEMS 0x8112 
#define UART_RDPTR   0x8114 
#define UART_RWPTR   0x8116 

#define KSCN_KSCTRL                 	0x8063
#define KSCN_RCTC                    	0x8064
#define KSCN_ROW_MASK                	0x806f

#define KSCN_EVENT_BUF           	0x811a
#define KSCN_EVENT_NUM           	0x811b


#define  CPUB_ICE_CTRL (*(volatile unsigned char xdata *)(0x8280))

#define CPUB_ICE_BK0_LO			0x8205
#define CPUB_ICE_BK0_HI			0x8206

#define IIC_ADDR_START		0x8300
#define IIC_PRER_LOW			0x8300
#define IIC_SLAVE_ADDR		0x8301
#define IIC_CTR				0x8302
#define IIC_CR				0x8303
#define IIC_RXR				0x8304
#define IIC_TXR				0x8305
#define IIC_SR				0x8306
#define IIC_FIFO				0x8307

#define USB_CONFIG		0x8c00
#define USB_INT_MASK(x)	(0x8c01 + x)
#define USB_TRG			0x8c10
#define USB_STALL		0x8c11
#define USB_CLEAR		0x8c12
#define USB_EP(x)			(0x8c18 + x)
#define USB_EP_LEN(x)		(0x8c20 + x)


#define USB_STATUS		0x8c26
#define USB_FIFO_EMPTY	0x8c27

sfr DFIFO0     = 0x94;
sfr DFIFO1     = 0x95;
sfr DFIFO2     = 0x96;


#define PWM_CLOCK           5 
#define REG_CLK_OFF(n)		(*(volatile unsigned char xdata *)(0x8050 + n))

#define REG_PWM_ENABLE   	(*(volatile unsigned char xdata *)0x8085)
#define REG_PWM_CLK_SEL 	(*(volatile unsigned char xdata *)0x8042)
#define REG_PWM_PCOUNT(n) 	(*(volatile unsigned char xdata *)(0x80a0 + n*4))
#define REG_PWM_NCOUNT(n) 	(*(volatile unsigned char xdata *)(0x80a2 + n*4))


#define REG_UART_CLK_SEL	(*(volatile unsigned char xdata *)0x8043)
#define REG_UART_BAUD		(*(volatile unsigned short xdata *)0x8052)
#define REG_UART_RX_ADDR_S	(*(volatile unsigned short xdata *)0x8054)
#define REG_UART_RX_ADDR_E 	(*(volatile unsigned short xdata *)0x8056)
#define REG_UART_RX_PTR_R	(*(volatile unsigned short xdata *)0x8058)
#define REG_UART_TX_ADDR_S	(*(volatile unsigned short xdata *)0x805a)
#define REG_UART_TX_ADDR_E	(*(volatile unsigned short xdata *)0x805c)
#define REG_UART_TX_PTR_W	(*(volatile unsigned short xdata *)0x805e)
#define REG_UART_TX_PTRP_R	(*(volatile unsigned short xdata *)0x8060)
#define REG_UART_CTRL		(*(volatile unsigned char xdata *)0x8062)

//read only
#define REG_UART_STATUS		(*(volatile unsigned char xdata *)0x810c)
#define REG_UART_DET_BAUD	(*(volatile unsigned char xdata *)0x810d)
#define REG_UART_TX_ITEMS	(*(volatile unsigned short xdata *)0x810e)
#define REG_UART_TX_PTR_R	(*(volatile unsigned short xdata *)0x8110)
#define REG_UART_RX_ITEMS	(*(volatile unsigned short xdata *)0x8112)
#define REG_UART_RX_DPTR_R	(*(volatile unsigned short xdata *)0x8114)
#define REG_UART_RX_PTR_W	(*(volatile unsigned short xdata *)0x8116)

#define REG_GPIO_SELECT(n)	(*(volatile unsigned char xdata *)(0x8080 + n))

#define REG_GPIO_OE(n) 	(*(volatile unsigned char xdata *)(0x8070 + n))
#define REG_GPIO_OUT(n)	(*(volatile unsigned char xdata *)(0x8074 + n))
#define REG_GPIO_PUP(n) 	(*(volatile unsigned char xdata *)(0x8078 + n))
#define REG_GPIO_PDN(n)	(*(volatile unsigned char xdata *)(0x807c + n))
#define REG_GPIO_SEL(n) 	(*(volatile unsigned char xdata *)(0x8080 + n))
#define REG_GPIO_KEY(n) 	(*(volatile unsigned char xdata *)(0x8083 + n))

#define REG_GPIO_IN(n)	(*(volatile unsigned char xdata *)(0x811c + n))	//n=0,1,2,3

#define REG_MISC_CTRL			(*(volatile unsigned char xdata *)(0x8006))


#define REG_SPID_CTRL  			(*(volatile unsigned char xdata *)(0x8086))
#define REG_SPID_DELAY  		(*(volatile unsigned char xdata *)(0x8087))
#define REG_SPID_TXADDR			(*(volatile unsigned short xdata *)(0x808a))
#define REG_SPID_RXADDR			(*(volatile unsigned short xdata *)(0x808c))
#define REG_SPID_TXLEN			(*(volatile unsigned short xdata *)(0x8088))
#define REG_SPID_RXLEN			(*(volatile unsigned short xdata *)(0x808e))

#define REG_IICD_CTRL			(*(volatile unsigned char xdata *)(0x8090))
#define REG_IICD_SCL_LOW		(*(volatile unsigned char xdata *)(0x8091))
#define REG_IICD_SCL_HIGH		(*(volatile unsigned char xdata *)(0x8092))
#define REG_IICD_START_SETUP		(*(volatile unsigned char xdata *)(0x8093))
#define REG_IICD_START_HOLD		(*(volatile unsigned char xdata *)(0x8094))
#define REG_IICD_STOP_SETUP		(*(volatile unsigned char xdata *)(0x8095))
#define REG_IICD_DATA_SETUP		(*(volatile unsigned char xdata *)(0x8096))
#define REG_IICD_DATA_HOLD		(*(volatile unsigned char xdata *)(0x8097))
#define REG_IICD_TXLEN			(*(volatile unsigned short xdata *)(0x8098))
#define REG_IICD_TXADDR			(*(volatile unsigned short xdata *)(0x809a))
#define REG_IICD_RXADDR			(*(volatile unsigned short xdata *)(0x809c))
#define REG_IICD_RXLEN			(*(volatile unsigned short xdata *)(0x809e))
#define REG_CLKN					(*(volatile unsigned long xdata *)(0x8100))
#define REG_DMA_STATUS			(*(volatile unsigned char xdata *)(0x812c))

#define REG_RFEN_ADC			(*(volatile unsigned char xdata *)(0x8906))
#define REG_ADC_IN				(*(volatile unsigned short xdata *)(0x812e))
#define REG_RF_ADC_GC			(*(volatile unsigned char xdata *)(0x8972))
#define REG_RFEN_CHGPUMP		(*(volatile unsigned char xdata *)(0x8973))

#endif
