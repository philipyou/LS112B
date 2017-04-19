#include "BT_IPC.h"
#include "YC_Utilities.h"
#include "hwreg.h"
#include "btreg.h"
#include "retation_variable.h"

bool BT_CmdFifoIn(BT_CMDFIFO_t *pFIFO)
{
	u16_t new_wprt,new_cmd_len,i;
	if( pFIFO->len ==0 )
		return false;
	new_cmd_len = BT_GetCmdFifoDepth()+pFIFO->len+2;
	if(new_cmd_len<=BT_CMD_BUF_LEN)
	{
		new_wprt = ESWAP(BT_CmdWptr);
		HWRITE(new_wprt++,CS_PREFIX_CMD);
		if(new_wprt == (mem_cmd_buf_end + 1))
		{
			new_wprt = mem_cmd_buf;
		}
		HWRITE(new_wprt++,pFIFO->len);
		if(new_wprt == (mem_cmd_buf_end + 1))
		{
			new_wprt = mem_cmd_buf;
		}
		for(i=0;i<pFIFO->len;i++)
		{
			HWRITE(new_wprt++,pFIFO->DATA[i]);
			if(new_wprt == (mem_cmd_buf_end + 1))
			{
				new_wprt = mem_cmd_buf;
			}
		}
		BT_CmdWptr = ESWAP(new_wprt);
		return true;
	}
	else
	{
		return false;
	}
}

void BT_EventFifoOut(BT_EVENTFIFO_t * event)
{
	u16_t new_rprt,i;
	u8_t event_prefix;
	if(BT_GetEvtFifoDepth()>2)
	{//event is not empty
		new_rprt = ESWAP(BT_EventRptr);
		event_prefix = HREAD(new_rprt++);
		if(event_prefix == CS_PREFIX_EVENT) {
			event->len = HREAD(new_rprt++);
			if( new_rprt == (mem_event_buf_end+1) )
			{
				new_rprt = mem_event_buf;
			}
			for(i=0;i<event->len;i++)
			{
				event->DATA[i] = HREAD(new_rprt++);
				if( new_rprt == (mem_event_buf_end+1) )
				{
					new_rprt = mem_event_buf;
				}
			}
		}
		else {
			event->len = 0;
		}
		BT_EventRptr = ESWAP(new_rprt);
	}
	else
	{
		event->len = 0;
	}
}


u8_t BT_GetCmdFifoDepth(void)
{
	u16_t wptr,rptr,len;
	wptr = ESWAP(BT_CmdWptr);
	rptr = ESWAP(BT_CmdRptr);
	len = (wptr + BT_CMD_BUF_LEN - rptr)&(BT_CMD_BUF_LEN-1);
	return len&0xff;
}

u8_t BT_GetEvtFifoDepth(void)
{
	u16_t wptr,rptr,len;
	wptr = ESWAP(BT_EventWptr);
	rptr = ESWAP(BT_EventRptr);
	len = (wptr + BT_EVT_BUF_LEN - rptr)&(BT_EVT_BUF_LEN-1);
	return len&0xff;
}

bool BT_CmdFifoEmpty(void)
{
	if(BT_GetCmdFifoDepth()==0)
	{
		return true;
	}
	return false;
}

bool BT_CmdFifoFull(BT_CMDFIFO_t *pFIFO)
{
	if((BT_GetCmdFifoDepth()+pFIFO->len+1) > BT_CMD_BUF_LEN)
		return true;
	return false;
}
