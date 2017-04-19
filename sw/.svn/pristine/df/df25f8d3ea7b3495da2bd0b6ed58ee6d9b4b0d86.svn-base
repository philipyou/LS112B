#ifndef _CRC32_H
#define _CRC32_H

#include "YC_Define.h"

#define CRC32_START_VAL	(0x00000000)
#define CRC32_COMPLETE(crc_val)			(crc_val ^ 0xFFFFFFFF)

extern u32_t cal_crc32_multiple(u32_t crc, u8_t *buffer, u32_t size);
extern u32_t crc32(u8_t *buffer, u32_t size);
extern u8_t cs(u8_t *buffer,u8_t size);

#endif
