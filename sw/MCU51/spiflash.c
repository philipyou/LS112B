#include "YC_Define.h"
#include "hwreg.h"
#include "YC_SYS.h"
#include "YC_Utilities.h"
#include "YC_Peripheral.h"
#include "btreg.h"
#include "BT_IPC.h"
#include "system.h"
#include "spiflash.h"

#if 0
void SPIFlashInit(void)
{
	REG_SPID_CTRL = 0x40;
	REG_SPID_DELAY = 0;
	REG_GPIO_SELECT(1)  &=~0x03;
	REG_GPIO_SELECT(1) |= 0x02;
}
#endif

u8_t SPIFlashReadSR(void)
{
	u8_t status;
	status = 0xff;
	REG_SPID_RXLEN = ESWAP(1);
	REG_SPID_TXLEN = ESWAP(1);
	*SF_TX_BUF = SF_READ_STATUS_REG;
	REG_SPID_TXADDR = ESWAP((u16_t)SF_TX_BUF);
	REG_SPID_RXADDR = ESWAP((u16_t)(&status));
	REG_MISC_CTRL = SPID_START;
	while((REG_DMA_STATUS&(1<<3)) == 0);		//wait_spid_done
	//YC_UARTSendData(&status, 1);
	return status;
}
void SPIFlashWriteEnable(void)
{
	REG_SPID_RXLEN = ESWAP(0);
	REG_SPID_TXLEN = ESWAP(1);
	*SF_TX_BUF = SF_WRITE_ENABLE;
	REG_SPID_TXADDR = ESWAP((u16_t)SF_TX_BUF);
	REG_MISC_CTRL = SPID_START;
	while((REG_DMA_STATUS&(1<<3)) == 0);
}
void SPIFlashWaitBusy(void)
{
	while((SPIFlashReadSR()&0x01) == 0x01) {	//wait for BUSY
		YC_Delay1ms(1);
	}
}
void SPIFlashEraseSector(u32_t addr)
{
	SPIFlashWriteEnable();
	SPIFlashWaitBusy();
	*(SF_TX_BUF) = SF_SECTOR_ERASE;
	*(SF_TX_BUF+1) = (addr>>16);
	*(SF_TX_BUF+2) = (addr>>8);
	*(SF_TX_BUF+3) = (addr&0xff);
	REG_SPID_TXLEN = ESWAP(4);
	REG_SPID_TXADDR = ESWAP((u16_t)SF_TX_BUF);
	REG_SPID_RXLEN = ESWAP(0);
	REG_MISC_CTRL = SPID_START;
	while((REG_DMA_STATUS&(1<<3)) == 0);		//wait_spid_done
	SPIFlashWaitBusy();
}
void SPIFlashWrite(u8_t *buf, u32_t addr, u8_t len)
{
	u8_t i;
	SPIFlashWriteEnable();
	SPIFlashWaitBusy();
	for(i=0;i<len;i++) {
		*(SF_TX_BUF+4+i) = buf[i];
	}
	*(SF_TX_BUF) = SF_PAGE_PROGRAM;
	*(SF_TX_BUF+1) = (addr>>16);
	*(SF_TX_BUF+2) = (addr>>8);
	*(SF_TX_BUF+3) = (addr&0xff);
	REG_SPID_TXLEN = ESWAP(len+4);
	REG_SPID_TXADDR = ESWAP((u16_t)SF_TX_BUF);
	REG_SPID_RXLEN = ESWAP(0);
	REG_MISC_CTRL = SPID_START;
	while((REG_DMA_STATUS&(1<<3)) == 0);		//wait_spid_done
	SPIFlashWaitBusy();
}
void SPIFlashRead(u8_t *buf, u32_t addr, u8_t len)
{
	REG_SPID_RXLEN = ESWAP(len);
	REG_SPID_TXLEN = ESWAP(4);
	*SF_TX_BUF = SF_READ_DATA;
	*(SF_TX_BUF+1) = (addr>>16);
	*(SF_TX_BUF+2) = (addr>>8);
	*(SF_TX_BUF+3) = (addr&0xff);
	REG_SPID_TXADDR = ESWAP((u16_t)SF_TX_BUF);
	REG_SPID_RXADDR = ESWAP((u16_t)buf);
	REG_SPID_CTRL  |=  (1<<6);
	REG_MISC_CTRL = SPID_START;
	//spi_read_flash_wait
	while(REG_DMA_STATUS&(1<<7));	//waiting for spid write
	while((REG_DMA_STATUS&(1<<3)) == 0);		//wait_spid_done
	return;
}


