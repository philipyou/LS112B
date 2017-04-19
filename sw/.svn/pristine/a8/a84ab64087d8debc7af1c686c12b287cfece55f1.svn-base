#include <reg320.h>
#include <string.h>
#include "hwreg.h"
#include "YC_Define.h"
#include "YC_Utilities.h"
#include "YC_Peripheral.h"
#include "YC_sys.h"
#include "system.h"
#include "btreg.h"
#include "BT_IPC.h"
//#include "wechat.h"
//#include "spiflash.h"
#include "CS_Scale_Proc.h"
#include "CS_ScaleDisplay.h"
#include "CS_ScaleKey.h"
#include "CS_CommTo1186.h"
#include "retation_variable.h"

#include "lxProtocol.h"
void main()
{
	R_Test++;
	systemInit();
	while(1) {
		BTControlProcess();
		lxOptProcess();
		//WechatProcess();
		SysStatusProcess();
		if(sys_state == SYS_ST_RUN) {
			YC_UtilTimerProcess();		
			CS_Scale_1186Com();		//1186通信处理
			CS_Scale_Proc();
		}
	}
}


