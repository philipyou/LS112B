#include "YC_Define.h"
#include "lxProtocol.h"
#include "retation_variable.h"
#include "BT_IPC.h"
#include "YC_Utilities.h"
#include "BTReg.h"
#include "spiflash.h"
#include "crc32.h"
#include "CS_ScaleDisplay.h"

static void lxSettingDeviceRespone(u16_t cmd, u8_t state);
static void sendHistoryData(void);
static void delUserData(void);

static UserMeasureData* QueuePop(void);

static void bigLittleEndianSwap(u8_t *buf, u16_t len)
{
	u16_t i, size;
	u8_t temp;
	
	size = len -1;
	for(i=0; i<len/2; i++)
	{
		temp = buf[i];
		buf[i] = buf[size -i];
		buf[size -i] = temp;
	}
}

static void lxSendRecvDataACK(u16_t handle, u8_t state)
{
	if(notifyValueUUIDA625 != NOTIFY_ENABLE)
	{
		return;
	}

	btCmdBuf.len = 0x06;
	btCmdBuf.DATA[0] = CS_CMD_LE_TX_DAT;
	btCmdBuf.DATA[1] = LO_UINT16(handle);
	btCmdBuf.DATA[2] = HI_UINT16(handle);
	btCmdBuf.DATA[3] = ACK_FRAME;
	btCmdBuf.DATA[4] = RECV_ACK_FRAME_LEN;
	btCmdBuf.DATA[5] = state;
	
	BT_CmdFifoIn(&btCmdBuf);
	YC_Delay10ms(10);
}

void lxRecvDataFromApp(BT_EVENTFIFO_t *event)
{
	//data[0]:total length, data[1~2]:handle
	//data[3]:frame control,data[4]:length,data[5~N]:content
	u8_t frameControl = event->DATA[3];
	u16_t cmd;
	
	bigLittleEndianSwap(&(event->DATA[5]), event->DATA[4]);
	cmd = event->DATA[5] | (event->DATA[6] << 8);
	
	//numTotalFrames == 1
	if(frameControl == FRAME_CONTROL_SINGLE)
	{
		//send ack
		lxSendRecvDataACK(HANDLE_UUIDA625_NOTIFY, RECV_ACK_SUCCESS);

		//copy data
		xmemcpy(LX_RECV_BUF, &(event->DATA[5]), event->DATA[4]);
		
		switch(cmd)
		{
			case CMD_REGISTRATION_REQ:
				changeLxOptStatus(OPT_REGISTER_REQ);
				break;
			case CMD_BIND_REQ:
				changeLxOptStatus(OPT_BIND_REQ);
				break;
			case CMD_LOGIN_RSP:
				changeLxOptStatus(OPT_LOGIN_RSP);
				break;
			case CMD_INIT_RSP:
				changeLxOptStatus(OPT_INIT_RSP);
				break;
				
			case CMD_TIME_SETTING_REQ:
				changeLxOptStatus(OPT_TIME_SETTING_REQ);
				break;
			case CMD_UNIT_SETTING_REQ:
				changeLxOptStatus(OPT_UNIT_SETTING_REQ);
				break;
			case CMD_DATA_CLEAR_SETTING_REQ:
				changeLxOptStatus(OPT_DATA_DEL_SETTING_REQ);
				break;
				
			case CMD_SYNC_DATA_REQ:
				changeLxOptStatus(OPT_SYNC_DATA_REQ);
				break;
		}
		
	}
	//ACK:numTotalFrames == 0
	else if(frameControl == FRAME_CONTROL_ACK)
	{
		if(event->DATA[5] == RECV_ACK_SUCCESS)
		{
			if(lxOptStatus == OPT_REGISTER_RSP_WAIT_ACK)
			{
				SpiFlashOperator(CS_CMD_WRITE_FLASH);
			}
			
			lxOptStatus = OPT_IDLE;
		}
		else
		{
			if(lxOptStatus == OPT_REGISTER_RSP_WAIT_ACK)
			{
				xmemset(mem_register_buf , 0, 10);
				//update adv flag
				*(mem_le_adv_data + 13) = 0;
			}
			lxOptStatus = OPT_IDLE;
		}
	}
}

void changeLxOptStatus(u8_t opt)
{
	if(lxOptStatus == OPT_IDLE)
	{
		lxOptStatus = opt;
	}
}

void lxOptProcess(void)
{
	u8_t i;

	btCmdBuf.DATA[0] = CS_CMD_LE_TX_DAT;
	btCmdBuf.DATA[1] = LO_UINT16(HANDLE_UUIDA621_NOTIFY);
	btCmdBuf.DATA[2] = HI_UINT16(HANDLE_UUIDA621_NOTIFY);
	switch(lxOptStatus)
	{
#if 1
		case OPT_REGISTER_REQ:
			if(*(LX_RECV_BUF + 8) == DEVICE_ID_UNREGISTERED ||
				*(LX_RECV_BUF + 8) == DEVICE_ID_REGISTERED)
			{
				//save deviceID
				dmemcpy(deviceInfo.deviceID, LX_RECV_BUF + 2, DEVICE_ID_LENGTH);
				//register success
				deviceInfo.registerFlag = DEVICE_REGISTERED;
				//update adv flag
				*(mem_le_adv_data + 13) = DEVICE_REGISTERED;
			}
			else// if(*(LX_RECV_BUF + 8) == DEVICE_ID_ILLEGALED)
			{
				deviceInfo.registerFlag = DEVICE_UNREGISTERED;
			}
			
			btCmdBuf.len = 0x08;
			btCmdBuf.DATA[3] = TOTAL_FRAMES_SINGLE << 4;
			btCmdBuf.DATA[4] = REGISTER_RSP_DATA_LEN;
			btCmdBuf.DATA[5] = LO_UINT16(CMD_REGISTER_RSP);
			btCmdBuf.DATA[6] = HI_UINT16(CMD_REGISTER_RSP);
			btCmdBuf.DATA[7] = deviceInfo.registerFlag;
			bigLittleEndianSwap(&(btCmdBuf.DATA[5]), REGISTER_RSP_DATA_LEN);
			BT_CmdFifoIn(&btCmdBuf);
			lxOptStatus = OPT_REGISTER_RSP_WAIT_ACK;
			break;
			
		case OPT_BIND_REQ:
			//bind success
			if(*(LX_RECV_BUF + 2) >= 1 && *(LX_RECV_BUF + 2) <= 4 && *(LX_RECV_BUF + 3) == 1)
			{
				//save user number
				deviceInfo.userNum = *(LX_RECV_BUF + 2);
				//lcd display result
				deviceInfo.bindFlag = BIND_SUCCESS;
			}
			else
			{
				deviceInfo.bindFlag =  BIND_FAIL;
			}
			
			btCmdBuf.len = 0x08;
			btCmdBuf.DATA[0] = CS_CMD_LE_TX_DAT;
			btCmdBuf.DATA[1] = LO_UINT16(HANDLE_UUIDA621_NOTIFY);
			btCmdBuf.DATA[2] = HI_UINT16(HANDLE_UUIDA621_NOTIFY);
			btCmdBuf.DATA[3] = TOTAL_FRAMES_SINGLE << 4;
			btCmdBuf.DATA[4] = BIND_RSP_DATA_LEN;
			btCmdBuf.DATA[5] = LO_UINT16(CMD_BIND_RSP);
			btCmdBuf.DATA[6] = HI_UINT16(CMD_BIND_RSP);
			btCmdBuf.DATA[7] = deviceInfo.bindFlag;

			bigLittleEndianSwap(&(btCmdBuf.DATA[5]), BIND_RSP_DATA_LEN);
			BT_CmdFifoIn(&btCmdBuf);
			lxOptStatus = OPT_BIND_RSP_WAIT_ACK;
			break;
		
		case OPT_LOGIN_REQ:
			btCmdBuf.len = 0x0f;
			btCmdBuf.DATA[3] = TOTAL_FRAMES_SINGLE << 4;
			btCmdBuf.DATA[4] = LOGIN_REQ_DATA_LEN;
			btCmdBuf.DATA[5] = LO_UINT16(CMD_LOGIN_REQ);
			btCmdBuf.DATA[6] = HI_UINT16(CMD_LOGIN_REQ);
			//Verification code:6bytes deviceID XOR MAC
			for(i=0;i<6;i++)
			{
				btCmdBuf.DATA[7+i] = deviceInfo.deviceID[i] ^ (*(mem_le_mac+i));
			}
			
			//user number:1bytes
			btCmdBuf.DATA[13] = deviceInfo.userNum;
			//battery:1bytes
			if(CS_Scale_JudgeLowBat(GetIfLowBat) == true)
				btCmdBuf.DATA[14] = 10;
			else
				btCmdBuf.DATA[14] = 100;

			bigLittleEndianSwap(&(btCmdBuf.DATA[5]), LOGIN_REQ_DATA_LEN);
			BT_CmdFifoIn(&btCmdBuf);
			YC_Delay10ms(10);
			lxOptStatus = OPT_LOGIN_REQ_WAIT_ACK;
			break;
		case OPT_LOGIN_RSP:
			*(mem_register_buf + 2) = *(LX_RECV_BUF + 2);
			if(*(mem_register_buf + 2) == LOGIN_SUCCESS)
			{
				//Verification code:6bytes deviceID XOR MAC
				for(i=0;i<6;i++)
				{
					if(*(LX_RECV_BUF + 3+i) != *(mem_register_buf + 4 + i) ^ (*(mem_le_mac+i)))
					{
						//disconnected
						btCmdBuf.len = 0x01;
						//btCmdBuf.DATA[0] = CS_CMD_LE_DISC_REQ;
						//BT_CmdFifoIn(&btCmdBuf);
						//return;
					}
				}
				//login success:sync time
				lxOptStatus = OPT_INIT_REQ;
			}
			else
			{
				lxOptStatus = OPT_IDLE;
				//disconnected
				btCmdBuf.len = 0x01;
				btCmdBuf.DATA[0] = CS_CMD_LE_DISC_REQ;
				BT_CmdFifoIn(&btCmdBuf);
				return;
			}
			break;
			
		case OPT_INIT_REQ:
			btCmdBuf.len = 0x08;
			btCmdBuf.DATA[0] = CS_CMD_LE_TX_DAT;
			btCmdBuf.DATA[1] = LO_UINT16(HANDLE_UUIDA621_NOTIFY);
			btCmdBuf.DATA[2] = HI_UINT16(HANDLE_UUIDA621_NOTIFY);
			btCmdBuf.DATA[3] = TOTAL_FRAMES_SINGLE << 4;
			btCmdBuf.DATA[4] = INIT_REQ_DATA_LEN;
			btCmdBuf.DATA[5] = LO_UINT16(CMD_INIT_REQ);
			btCmdBuf.DATA[6] = HI_UINT16(CMD_INIT_REQ);
			btCmdBuf.DATA[7] = 0x08;
			
			bigLittleEndianSwap(&(btCmdBuf.DATA[5]), INIT_REQ_DATA_LEN);
			BT_CmdFifoIn(&btCmdBuf);
			lxOptStatus = OPT_INIT_REQ_WAIT_ACK;
			break;
		case OPT_INIT_RSP:
			if(*(LX_RECV_BUF + 2) == 0x08)
			{
				//save UTC time
				dmemcpy((pxbyte)&UTCTime, LX_RECV_BUF + 3, 4);
			}
			lxOptStatus = OPT_IDLE;
			break;
			
		case OPT_TIME_SETTING_REQ:
			if(*(LX_RECV_BUF+2) &0x01 == 1)
			{
				dmemcpy((pxbyte)&UTCTime, LX_RECV_BUF + 3, 4);
			}
			/*
			if(*(LX_RECV_BUF+2) &0x02 == 1)
			{

			}
			*/
			lxSettingDeviceRespone(CMD_TIME_SETTING_REQ, DEVICE_RSP_SUCCESS);
			lxOptStatus = OPT_TIME_SETTING_RSP_WAIT_ACK;
			break;
		
		case OPT_UNIT_SETTING_REQ:
			//save
			if(*(LX_RECV_BUF + 2) <= 3)
			{
				R_Selet_Unit = *(LX_RECV_BUF + 2);
				lxSettingDeviceRespone(CMD_UNIT_SETTING_REQ, DEVICE_RSP_SUCCESS);
			}
			else
			{
				lxSettingDeviceRespone(CMD_UNIT_SETTING_REQ, DEVICE_RSP_FAIL);
			}
			lxOptStatus = OPT_UNIT_SETTING_RSP_WAIT_ACK;
			break;

		case OPT_DATA_DEL_SETTING_REQ:
			if(*(LX_RECV_BUF + 3)&0x01 == 1)
			{
				delUserData();
			}
			if(*(LX_RECV_BUF + 3)&0x04 == 1)
			{
				UTCTime = 0;
				R_Selet_Unit = CS_ScaleDisplay_L_kg;
			}
			if(*(LX_RECV_BUF + 3)&0x08 == 1)
			{
				QueueInit();
			}
			
			lxSettingDeviceRespone(CMD_DATA_CLEAR_SETTING_REQ, DEVICE_RSP_SUCCESS);
			lxOptStatus = OPT_DATA_DEL_SETTING_RSP_WAIT_ACK;
			break;
#endif
		case OPT_SYNC_DATA_REQ:
			//forbidden
			if(*(LX_RECV_BUF + 3) == 0)
			{
				lxOptStatus = OPT_IDLE;
			}
			else
			{
				lxOptStatus = OPT_SYNC_DATA_RSP;
			}
			break;
		case OPT_SYNC_DATA_RSP:
			//send measuredata
			sendHistoryData();
			lxOptStatus = OPT_SYNC_DATA_RSP_WAIT_ACK;
			break;
		/*
		case LX_OPT_STATUS_RESEND:
			BT_CmdFifoIn(&btCmdBuf);
			lxOptStatus = OPT_IDLE;
			break;
			*/
	}
}

static void lxSettingDeviceRespone(u16_t cmd, u8_t state)
{
	btCmdBuf.len = DEVICE_RSP_DATA_LEN + 5;
	btCmdBuf.DATA[0] = CS_CMD_LE_TX_DAT;
	btCmdBuf.DATA[1] = LO_UINT16(HANDLE_UUIDA621_NOTIFY);
	btCmdBuf.DATA[2] = HI_UINT16(HANDLE_UUIDA621_NOTIFY);
	btCmdBuf.DATA[3] = TOTAL_FRAMES_SINGLE << 4;
	btCmdBuf.DATA[4] = DEVICE_RSP_DATA_LEN;
	btCmdBuf.DATA[5] = LO_UINT16(CMD_DEVICE_RSP);
	btCmdBuf.DATA[6] = HI_UINT16(CMD_DEVICE_RSP);
	btCmdBuf.DATA[7] = LO_UINT16(cmd);
	btCmdBuf.DATA[8] = HI_UINT16(cmd);
	btCmdBuf.DATA[9] = state;

	bigLittleEndianSwap(&(btCmdBuf.DATA[5]), DEVICE_RSP_DATA_LEN);
	BT_CmdFifoIn(&btCmdBuf);
	YC_Delay10ms(10);
}

static void delUserData(void)
{
	//(mem_register_buf) = DEVICE_UNREGISTERED;
	//*(mem_le_adv_data + 13) = *(mem_register_buf);
	xmemset((u8_t *)&deviceInfo, 0, sizeof(DeviceInfo_t));
	QueueInit();
	*(mem_le_adv_data + 13) = DEVICE_UNREGISTERED;
	UTCTime = 0;
	SpiFlashOperator(CS_CMD_WRITE_FLASH);
}

#define SYNC_DATA_HDR_LEN	0x08
#define WEIGHT_UTC_FLAG		0x05
static void sendHistoryData(void)
{
	static u8_t total_len = 0;
	UserMeasureData *temp = NULL;
	u32_t crc32;
	u8_t i=0;
	u8_t pkgCnt; 
	u8_t size;
	/*
	UserMeasureData temp1;
	temp1.UTCTime[0] = 0x12;
	temp1.UTCTime[0] = 0x34;
	temp1.UTCTime[0] = 0x56;
	temp1.UTCTime[0] = 0x78;
	temp1.weight = 0xabba;
	QueuePush(&temp1);
	QueuePush(&temp1);
	*/
	
	size = historyUserData.size;
	pkgCnt = (8 + historyUserData.size * 6 + 4)/18 + (((8 + historyUserData.size * 6 + 4)%18)>0?1:0);
	total_len = 8 + historyUserData.size * 6 + 4;
	
	btCmdBuf.DATA[0] = CS_CMD_LE_TX_DAT;
	btCmdBuf.DATA[1] = LO_UINT16(HANDLE_UUIDA621_NOTIFY);
	btCmdBuf.DATA[2] = HI_UINT16(HANDLE_UUIDA621_NOTIFY);

	*(LX_SEND_BUF+0) = LO_UINT16(CMD_SYNC_DATA_RSP);
	*(LX_SEND_BUF+1) = HI_UINT16(CMD_SYNC_DATA_RSP);
	*(LX_SEND_BUF+2) = pkgCnt - 1;
	*(LX_SEND_BUF+3) = 0;
	*(LX_SEND_BUF+4) = WEIGHT_UTC_FLAG;
	*(LX_SEND_BUF+5) = 0;
	*(LX_SEND_BUF+6) = 0;
	*(LX_SEND_BUF+7) = 0;
	if(pkgCnt > 1)
	{
		for(i=0;i<size;i++)
		{
			temp = QueuePop();
			dmemcpy(LX_SEND_BUF + (sizeof(UserMeasureData) * i) + 8, (u8_t *)temp, sizeof(UserMeasureData));
		}
		crc32 = cal_crc32_multiple(0, LX_SEND_BUF, total_len - 4);
		dmemcpy(LX_SEND_BUF + (sizeof(UserMeasureData) * i) + 8, (u8_t *)&crc32, sizeof(crc32));
		bigLittleEndianSwap(LX_SEND_BUF, total_len);
		
		for(i=0;i<pkgCnt;i++)
		{
			btCmdBuf.DATA[3] = (pkgCnt << 4) + i;
			if(i == pkgCnt -1)
			{
				btCmdBuf.DATA[4] = total_len;
			}
			else
			{
				btCmdBuf.DATA[4] = 20 - 2;
				total_len -= 18;
				
			}
			btCmdBuf.DATA[7] = pkgCnt - i - 1;
			btCmdBuf.len = 3 + 2 + btCmdBuf.DATA[4] ;
			dmemcpy(&btCmdBuf.DATA[5], (u8_t *)(LX_SEND_BUF+i*18), btCmdBuf.DATA[4] );
			BT_CmdFifoIn(&btCmdBuf);
			YC_Delay10ms(12);
		}
	}
	else
	{
		btCmdBuf.len = 3 + 2 + SYNC_DATA_HDR_LEN + sizeof(UserMeasureData);
		btCmdBuf.DATA[3] = TOTAL_FRAMES_SINGLE << 4;
		btCmdBuf.DATA[4] = SYNC_DATA_HDR_LEN + sizeof(UserMeasureData);
		temp = QueuePop();
		if(temp != NULL)
		{
			dmemcpy(LX_SEND_BUF + 8, (u8_t *)temp, sizeof(UserMeasureData));
		}
		else
		{
			xmemset(LX_SEND_BUF, 0, sizeof(UserMeasureData));
		}
		dmemcpy(&(btCmdBuf.DATA[5]), LX_SEND_BUF, btCmdBuf.DATA[4]);
		bigLittleEndianSwap(&(btCmdBuf.DATA[5]), btCmdBuf.DATA[4]);
		BT_CmdFifoIn(&btCmdBuf);
	}
}

void SpiFlashOperator(u8_t opt)
{
#if 1
	if(CS_CMD_READ_FLASH == opt)
	{
		SPIFlashRead(mem_spiflash_buf, SPIFLASH_DATA_ADDR, SPIFLASH_READ_LENGTH);
	}
	else if(CS_CMD_WRITE_FLASH == opt)
	{
		SPIFlashEraseSector(SPIFLASH_DATA_ADDR);
		SPIFlashWrite(mem_spiflash_buf, SPIFLASH_DATA_ADDR, SPIFLASH_READ_LENGTH);
	}
#else
	btCmdBuf.len = 1;
	btCmdBuf.DATA[0] = opt;
	BT_CmdFifoIn(&btCmdBuf);
	YC_Delay10ms(10);
#endif
}




/**************************************************************************/
#define QUEUE_IS_EMPTY	(historyUserData.size <= 0)		
#define QUEUE_IS_FULL	(historyUserData.size >= MAX_SIZE)

void QueueInit(void)
{
	historyUserData.head = 0;
	historyUserData.tail = 0;
	historyUserData.size = 0;
}

static UserMeasureData* QueuePop(void)
{
	UserMeasureData *temp = NULL;
	
	if(QUEUE_IS_EMPTY)
		return NULL;

	temp = &(historyUserData.measureData[historyUserData.head]);
	historyUserData.head = (historyUserData.head + 1)%MAX_SIZE;
	historyUserData.size--;

	return temp;
}

void QueuePush(UserMeasureData *userData)
{
	if(QUEUE_IS_FULL)
		QueuePop();
	
	dmemcpy((u8_t *)&(historyUserData.measureData[historyUserData.tail]), (u8_t *)userData, sizeof(UserMeasureData));
	historyUserData.tail = (historyUserData.tail + 1)%MAX_SIZE;
	historyUserData.size++;
}




