#ifndef __SPIFLASH_H
#define __SPIFLASH_H

#define SF_WRITE_ENABLE				0x06 
#define SF_WRITE_DISABLE			0x04 
#define SF_READ_STATUS_REG			0x05 
#define SF_WRITE_STATUS_REG		0x01 
#define SF_READ_DATA				0x03 
#define SF_FAST_READ_DATA			0x0B 
#define SF_FAST_READ_DUAL			0x3B 
#define SF_PAGE_PROGRAM			0x02 
#define SF_BLOCK_ERASE				0xD8 
#define SF_SECTOR_ERASE			0x20 
#define SF_CHIP_ERASE				0xC7 
#define SF_POWERDOWN				0xB9 
#define SF_RELEASE_POWERDOWN		0xAB 
#define SF_DEV_ID					0xAB 
#define SF_MANUFACT_ID				0x90 
#define SF_JEDEC_ID					0x9F 

#define SF_TX_BUF					mem_eir			//size is 128 bytes
#define SF_RX_BUF					(mem_eir+128)	//size is 128 bytes
#define SPID_START 					0x02

void SPIFlashInit(void);
void SPIFlashEraseSector(u32_t addr);
void SPIFlashRead(u8_t *buf, u32_t addr, u8_t len);
void SPIFlashWrite(u8_t *buf, u32_t addr, u8_t len);
#endif


