#include "system.h"
#include "YC_Define.h"
#include "hwreg.h"
#include "YC_SYS.h"
#include "YC_Utilities.h"
#include "YC_Peripheral.h"
#include "btreg.h"
#include "BT_IPC.h"
//#include "wechat.h"
#include "CS_Scale_Proc.h"
#include "CS_CommTo1186.h"
#include "CS_ScaleDisplay.h"
#include "retation_variable.h"

#include "spiflash.h"
#include "lxProtocol.h"

u8_t xdata SoftTimer_1STimerID;
u8_t xdata SoftTimer_10msTimerID;
#if 0
void SoftTimer_1SCallback(void)
{
	u8_t buf[10] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x00};
	//YC_UARTSendData(buf, 10);
	YC_UtilTimerStart(1, SoftTimer_1SCallback, &SoftTimer_1STimerID);
	//add user function
	
	UTCTime++;

	/*
	R_Debug_temp = 1234;
	CS_ScaleDisplay_Debug();
	
	*/
	
/*
	SPIFlashInit();
	SPIFlashEraseSector(0x8000);
	SPIFlashWrite(buf,0x8000,10);
	SPIFlashRead(SF_RX_BUF,0x8000,10);	
	*/
}
#endif
void SoftTimer_10mSCallback(void)
{
	static u8_t count;
	YC_UtilTimerStartMs(10, SoftTimer_10mSCallback, &SoftTimer_10msTimerID);
	//add user function
	if(uartTimeoutCnt<255) {
		uartTimeoutCnt++;
	}


	if(count<100)
		count++;
	else
		{
		count=0;
		CS_Scale_TimeOutProc(CS_SetTimeBase1S);
		CS_MemoryWeightClear_Timer(MemWeightTimerInc);
		}
		
	CS_SoftTimer(SoftTimerInc);

	CS_KeyScan();

}

void systemInit(void)
{
	LPM_DISABLE();
	if(*mem_poweron_flag == CS_POWERON_FLAG		//poweron or reset
	/*|| *mem_poweron_flag == CS_IOWAKE_FLAG*/) {		//iowake from sleep
		sys_state = SYS_ST_RUN;
		R_Scale_state = CS_Scale_state_init;

		QueueInit();
		//read spiflash data,save to buffer
		SpiFlashOperator(CS_CMD_READ_FLASH);
		bleConnStatus = BLE_STATUS_DISCONN;
		if(deviceInfo.registerFlag != DEVICE_REGISTERED)
		{
			xmemset(mem_register_buf, 0, sizeof(deviceInfo));
		}
		*(mem_le_adv_data + 13) = deviceInfo.registerFlag;
			
		YC_UtilTimerInit();
		//YC_UtilTimerStart(1, SoftTimer_1SCallback, &SoftTimer_1STimerID);
		YC_UtilTimerStartMs(10, SoftTimer_10mSCallback, &SoftTimer_10msTimerID);
		CS_Scale_PowerOn();	
		
	}
	else {	//wake from LPM mode
		if(YC_GPIOGetInputStatus(5) == 0 ||	//GPIO5, key is  down	
		YC_GPIOGetInputStatus(27) == 0) {	//GPIO27, scale wakeup
			sys_state = SYS_ST_RUN;
			R_Scale_state = CS_Scale_state_standby;
			YC_UtilTimerInit();
			//YC_UtilTimerStart(1, SoftTimer_1SCallback, &SoftTimer_1STimerID);
			YC_UtilTimerStartMs(10, SoftTimer_10mSCallback, &SoftTimer_10msTimerID);
			//CS_Scale_PowerOn();	

			//R_Debug_temp=R_Scale_state;
			//CS_ScaleDisplay_Debug();
			//while(1);
		}
		else {	//remote bluetooth wakeup
			sys_state = SYS_ST_IDLE;
		}
	}
	*mem_poweron_flag = CS_NULL_FLAG;
}

u8_t generateChecksum(u8_t *buf,u8_t len)
{
	u8_t result;
	result = 0;
	while(len--) {
		result^=*buf++;
	}
	return result;
}

void BTControlProcess(void)
{
	BT_EVENTFIFO_t event;
	//normal process
	BT_EventFifoOut(&event);
	if(event.len != 0)
	{
		//YC_UARTSendData(event.DATA, event.len);
		switch(event.DATA[0]) {
			case CS_NOTIFY_POWERON:
				break;
			case CS_NOTIFY_DISC:		//bluetooth disconnect
				notifyValueUUIDA621 = NOTIFY_INDICATE_DISABLE;
				notifyValueUUIDA625 = NOTIFY_INDICATE_DISABLE;
				bleConnStatus= BLE_STATUS_DISCONN;
				break;
			case CS_NOTIFY_CONN:	//bluetooth connected
				lxOptStatus = OPT_IDLE;
				bleConnStatus= BLE_STATUS_CONN;
				break;
			case CS_NOTIFY_DATA:	//receive data
				lxRecvDataFromApp(&event);
				break; 
			case CS_NOTIFY_OPENHANDLE:
				if(event.DATA[1] == HANDLE_UUIDA621_NOTIFY)
				{
					notifyValueUUIDA621 = event.DATA[3];
				}
				else if(event.DATA[1] == HANDLE_UUIDA625_NOTIFY)
				{
					notifyValueUUIDA625 = event.DATA[3];
				}

				if(*mem_register_buf == DEVICE_REGISTERED &&
					notifyValueUUIDA621 == NOTIFY_ENABLE &&
					notifyValueUUIDA625 == NOTIFY_ENABLE)
				{
					lxOptStatus = OPT_LOGIN_REQ;
				}
				break;
			case CS_NOTIFY_READ_REQ:
				break;
			case CS_NOTIFY_INDICATE_CONFIRM:
				handleIndicationConfirm = 1;
				break;
		}
	}
}


void SysStatusProcess(void) 
{
	if( sys_state == SYS_ST_IDLE) {	//can enter LPM
		if(bleConnStatus ==BLE_STATUS_DISCONN)
		{
			YC_UARTWaitSendData();
			YC_UARTClearBuffer();
		    	LPM_ENABLE();

			btCmdBuf.len = 0x01;
			btCmdBuf.DATA[0] = CS_CMD_SLEEP_REQ;
			BT_CmdFifoIn(&btCmdBuf);

				
		}
	}
	else if( sys_state == SYS_ST_SLEEP) {//can enter LPM
		YC_UARTWaitSendData();
		YC_UARTClearBuffer();
	    	LPM_ENABLE();
	} 
	else {
		LPM_DISABLE();
	}
}

