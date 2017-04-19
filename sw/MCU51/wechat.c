#include "YC_Define.h"
#include "hwreg.h"
#include "YC_SYS.h"
#include "YC_Utilities.h"
#include "YC_Peripheral.h"
#include "btreg.h"
#include "BT_IPC.h"
#include "system.h"
#include "wechat.h"
#include "CS_Scale_Proc.h"
#include "retation_variable.h"

const u8_t AuthRequestPack[] = {
	0xFE, 	//bMagicNumber
	0x01, 	//bVer
	0x00, 0x1C, 	//nLength
	0x27, 0x11, 	//10001,ECI_req_auth
	0x00, 0x01, 	//nSeq
	0x0A, 0x00, 	//BaseRequest
	//0x12, 0x10, 0x7E, 0x65, 0xA0, 0x8D, 0x26, 0x4D, 0x9E, 0xA7, 0xCC, 0xC9, 0x83, 0x57, 0xA0, 0x54, 0x69, 0x6D,	//Md5DeviceTypeAndDeviceId
	0x18, 0x83, 0x80, 0x04, 	//ProtoVersion,65539
	0x20, 0x01, 	//AuthProto = 1
	0x28, 0x02, 	//AuthMethod = EAM_macNoEncrypt
	
	0x32, 0x00,	//AesSign = null
	0x3A//, 0x06, 0x00,0x00,0x00,0x00,0x00,0x00	//MAC address should be added to the end
};

/*const u8_t AuthResponsePack[]= {
	0xFE, 	//bMagicNumber
	0x01, 	//bVer
	0x00, 0x0E, 	//nLength
	0x4E, 0x21, 	//20001,ECI_resp_auth
	0x00, 0x01, 	//nSeq
	0x0A, 0x02, 	//BaseResponse
		0x08, 0x00, 	//ErrorCode=0x00
	0x12, 0x00 	//AesSessionKey = NULL
};*/

const u8_t InitRequestPack[] = {
	0xFE, 	//bMagicNumber
	0x01, 	//bVer
	0x00, 0x0D, 	//nLength
	0x27, 0x13, 	//10003,ECI_req_init
	0x00, 0x02, 	//nSeq
	0x0A, 0x00, 	//BaseRequest
	0x12, 0x01, 0x7F	//RespFieldFilter = 7F 
};

/*const u8_t InitResponsePack[] = {
	0xFE, 	//bMagicNumber
	0x01, 	//bVer
	0x00, 0x0C, 	//nLength
	0x4E, 0x23, 	//20003,ECI_resp_init
	0x00, 0x02, 	//nSeq
	0x0A, 0x02, 	//BaseResponse
		0x08, 0x00,	//ErrorCode=0x00
};*/

const u8_t sendDataRequestPack[] = {
	0xFE, 	//bMagicNumber
	0x01,	//bVer
	0x00, 0x36,	//nLength
	0x27, 0x12,	//10002,ECI_req_sendData
	0x00, 0x03,	//nSeq
	0x0A, 0x00, 	//BaseRequest
	0x12, 
		/*0x2A, 
			0x43, 0x43, 
			0x36, 0x43, 0x31, 0x30, 0x37, 0x38, 0x42, 0x45, 0x37, 0x43,  0x30, 0x38, 	//mac
			0x33, 0x32, 0x33, 0x30, 0x33, 0x37, 0x32, 0x30, 0x34, 0x32, 	//model number
			0x34, 0x31, 0x33, 0x30, 0x33, 0x32, 0x33, 0x33,  //sw version
			0x34, 0x31, 0x33, 0x30, 0x33, 0x30, 0x33, 0x32, //hw version
			0x30, 0x38	//timezone*/
		   /*0x2A, 0x43, 0x43, 0x36, 0x43, 0x31, 0x30, 0x37, 0x38, 0x42, 0x45, 0x37, 0x43, 0x30, 0x38, 0x33, 0x32, 
			      0x33, 0x30, 0x33, 0x37, 0x32, 0x30, 0x34, 0x32, 0x34, 0x31, 0x33, 0x30, 0x33, 0x32, 0x33, 0x33, 
			      0x34, 0x31, 0x33, 0x30, 0x33, 0x30, 0x33, 0x32, 0x30, 0x38 */
};

const u8_t modelParameter[] = {
			0x33, 0x32, 0x33, 0x30, 0x33, 0x37, 0x32, 0x30, 0x34, 0x32, 	//model number
			0x34, 0x31, 0x33, 0x30, 0x33, 0x32, 0x33, 0x33,  //sw version
			0x34, 0x31, 0x33, 0x30, 0x33, 0x30, 0x33, 0x32, //hw version
			0x30, 0x38	//timezone
			};

bool asciiToHex(u8_t *ascii,  u8_t *hex)
{
	u8_t result, i;
	result = 0;
	for(i=0;i<2;i++) {
		if(ascii[i] >='0' && ascii[i]<='9') {		
			result = ascii[i]  - '0';
		}
		else if(ascii[i] >= 'A' && ascii[i]<= 'F') {
			result = ascii[i]  - 'A' + 0x0a;
		}
		else if(ascii[i] >= 'a' && ascii[i]<='f') {
			result = ascii[i]  - 'a' + 0x0a;
		}
		else {
			return false;
		}
		result <<=4;
	}
	*hex = result;
	return true;
}

bool hexToAscii(u8_t hex, u8_t *ascii)
{
	u8_t  temp;
	temp = hex>>4;
	ascii[0] =(temp>9)?(temp-0x0A +'A'):(temp+'0');
	temp = hex&0x0F;
	ascii[1] =(temp>9)?(temp-0x0A +'A'):(temp+'0');
	return true;
}

u8_t decodeProtobuf(u8_t xdata*protobuf, u8_t protobufLen, u8_t decodeIndex,u8_t xdata *bufout, u8_t xdata *bufoutLen)
{
	u8_t curIndex,type;
	u8_t  pos = 0;
	u8_t offset = 0;
	u8_t len;
	u32_t dat;
	//u8_t buf[4];
	while(pos<protobufLen) {
		type = (*(protobuf+pos))&0x07;
		if(((*(protobuf+pos))&0x80) != 0) {
			curIndex = (((*(protobuf+pos))&0x78)>>3);
			curIndex += (*(protobuf+pos+1)<<4);
			pos+=2;
		}
		else {
			curIndex = *(protobuf+pos)>>3;
			pos++;
		}
		offset = 0;
		len = 0;
		dat = 0;
		switch(type) {
			case PROTOBUF_VARINT_TYPE:
				while((*(protobuf+pos+offset) &0x80) != 0) {
					dat+=((u32_t)(*(protobuf+pos+offset) &0x7F)<<(offset*7));
					offset++;
				}
				dat+=((u32_t)(*(protobuf+pos+offset) &0x7F)<<(offset*7));
				len = offset+1;
				//buf[0] = 0xa1;
				//buf[1] = pos;
				//buf[2] = len;
				//YC_UARTSendData(buf,3);
				if(curIndex == decodeIndex) {	//find the decodeIndex
					if(bufout) {
						xmemcpy(bufout,(u8_t *)&dat,4);
					}
					if(bufoutLen) {
						*bufoutLen  = 4;
					}
					return pos;
				}
				
				break;
			case PROTOBUF_LEN_DELIMITED_TYPE:
				len = *(protobuf+pos)+1;
				//buf[0] = 0xa2;
				//buf[1] = pos;
				//buf[2] = len;
				//YC_UARTSendData(buf,3);
				if(curIndex == decodeIndex) {	//find the decodeIndex
					if(bufout) {
						xmemcpy(bufout,protobuf+pos+1,len-1);
					}
					if(bufoutLen) {
						*bufoutLen  = len-1;
					}
					return pos;
				}
				break;
			default:
				//buf[0] = 0xa0;
				//buf[1] = pos;
				//YC_UARTSendData(buf,2);
				return 0;
		}
		if(curIndex != decodeIndex) {	//don't find, goto next index
			pos += len;
		}
	}
	return 0;
}

void sendMeasureData(void)
{
	u8_t i;
	wechatStatus = WECHAT_ST_SENDDATA_REQ;
	*(WECHAT_TX_BUF) = 0xC3;	//CMD
	*(WECHAT_TX_BUF+1) = 0xFE;	//weight
	*(WECHAT_TX_BUF+2) = 0x00;
	*(WECHAT_TX_BUF+3) = (u8_t)(R_UnitWeight.origin>>8);		//0x13;
	*(WECHAT_TX_BUF+4) = (u8_t)(R_UnitWeight.origin&0xff);		//0xC3;
	*(WECHAT_TX_BUF+5) = (u8_t)(UTCTime>>24);	//UTC time
	*(WECHAT_TX_BUF+6) = (u8_t)(UTCTime>>16);
	*(WECHAT_TX_BUF+7) = (u8_t)(UTCTime>>8);
	*(WECHAT_TX_BUF+8) = (u8_t)(UTCTime&0xff);
	*(WECHAT_TX_BUF+9) = 0xFF;	//5kHz impedence
	*(WECHAT_TX_BUF+10) = 0x00;
	*(WECHAT_TX_BUF+11) = 0x15;
	*(WECHAT_TX_BUF+12) = 0x77;
	*(WECHAT_TX_BUF+13) = 0xFF;	//50kHz impedence
	*(WECHAT_TX_BUF+14) = 0x00;
	*(WECHAT_TX_BUF+15) = 0x15;
	*(WECHAT_TX_BUF+16) = 0x77;
	*(WECHAT_TX_BUF+17) = 0x01;	//user ID
	*(WECHAT_TX_BUF+18) = 0x01;	//measurement status
	for(i=0; i<19; i++) {
		userData[0] = 38;
		hexToAscii(*(WECHAT_TX_BUF+i), userData+(i<<1)+1);
	}
}

void WechatProcess(void)
{
	u8_t len,len2,i;
	const u8_t *ptrHead;
	u8_t ptrHeadLen;
	u8_t ptrDataLen;
	//u8_t buf[4];
	/*if(prev_wechatStatus != wechatStatus) {
		prev_wechatStatus = wechatStatus;
		buf[0] = 0xa0;
		buf[1] = wechatStatus;
		YC_UARTSendData(buf, 2);
	}*/
	switch(wechatStatus) {
		case WECHAT_ST_AUTH_REQ:
		case WECHAT_ST_AUTH_REQ_WAIT:
		case WECHAT_ST_AUTH_RESPONSE_WAIT:
			ptrHead = AuthRequestPack;
			ptrHeadLen = sizeof(AuthRequestPack);
			userData[0] = 6;
			ptrDataLen = userData[0]+1;	
			for(i=0;i<6;i++) {
				userData[1+i] = mem_le_lap[5-i]; 
			}
			
			break;
		case WECHAT_ST_INIT_REQ:
		case WECHAT_ST_INIT_REQ_WAIT:
		case WECHAT_ST_INIT_RESPONSE_WAIT:
			ptrHead = InitRequestPack;
			ptrHeadLen = sizeof(InitRequestPack);
			ptrDataLen = 0;
			break;
		case WECHAT_ST_MANUFACTURE_REQ:
		case WECHAT_ST_MANUFACTURE_REQ_WAIT:
		case WECHAT_ST_MANUFACTURE_RESPONSE_WAIT:
			ptrHead = sendDataRequestPack;
			ptrHeadLen = sizeof(sendDataRequestPack);
			userData[0] = 14+sizeof(modelParameter);
			ptrDataLen = userData[0]+1;	
			hexToAscii(0xCC,&userData[1]);
			for(i=0;i<6;i++) {
				hexToAscii(mem_le_lap[5-i], &userData[3+2*i]);
			}
			cmemcpy(userData+15,modelParameter,sizeof(modelParameter));			
			break;
		case WECHAT_ST_SENDDATA_REQ:
		case WECHAT_ST_SENDDATA_REQ_WAIT:
		case WECHAT_ST_SENDDATA_RESPONSE_WAIT:
			ptrHead = sendDataRequestPack;
			ptrHeadLen = sizeof(sendDataRequestPack);
			ptrDataLen = userData[0]+1;		
			break;
	}
	if(wechatStatus > 0x10 && wechatStatus <0x20) {	//send data
		if(wechatSendLen >= ptrHeadLen+ptrDataLen) {	//send finish

		}
		else {
			xmemset(btCmdBuf.DATA,0,sizeof(btCmdBuf.DATA));	//clear
			btCmdBuf.len = 23;
			if(wechatSendLen >=ptrHeadLen) {
				len = 0;
			}
			else if(wechatSendLen + 20 > ptrHeadLen) {	//the end of the head
				len = ptrHeadLen - wechatSendLen;
			}
			else {
				len = 20;				
			}
			cmemcpy(&btCmdBuf.DATA[3] ,ptrHead+wechatSendLen,len);
			wechatSendLen += len;
			if(ptrDataLen >0 && len <20) {
				 if(wechatSendLen + (20-len) > ptrHeadLen+ptrDataLen) {	//the end of the head
					len2 = ptrHeadLen+ptrDataLen - wechatSendLen;
				}
				else {
					len2 = 20-len;
				}
				cmemcpy(&btCmdBuf.DATA[3+len] ,userData+(wechatSendLen-ptrHeadLen),len2);
				wechatSendLen += len2;
			}
			if(wechatSendLen <= 20) {	//the first frame
				btCmdBuf.DATA[5] = (ptrHeadLen+ptrDataLen)>>8;		//length
				btCmdBuf.DATA[6] = (ptrHeadLen+ptrDataLen)&0xff;
				btCmdBuf.DATA[9] = (wechatSeq>>8);
				btCmdBuf.DATA[10] = (wechatSeq&0xFF);
				//buf[0] = 0xa5;
				//buf[1] = btCmdBuf.DATA[9];
				//buf[2] = btCmdBuf.DATA[10];
				//YC_UARTSendData(buf, 3);	
			}
			
			btCmdBuf.DATA[0] = CS_CMD_LE_TX_DAT;
			btCmdBuf.DATA[1] = HANDLE_WECHAT_INDICATE;
			btCmdBuf.DATA[2] = 0x00;
			BT_CmdFifoIn(&btCmdBuf);
			if(wechatStatus == WECHAT_ST_SENDDATA_REQ) {
				//buf[0] = 0xa1;
				//YC_UARTSendData(btCmdBuf.DATA, btCmdBuf.len);		
			}
		}
		wechatStatus += 0x10;
	}
	else if(wechatStatus > 0x20 && wechatStatus <0x30) {	//wait confirm
		if(handleIndicationConfirm) {
			handleIndicationConfirm = 0;
			if(wechatSendLen >= ptrHeadLen+ptrDataLen) {	//send finish
				wechatSendLen = 0;
				wechatStatus += 0x10;
				wechatRcvLen = 0;
				wechatRcvTotalLen = 0xff;
			}
			else {
				wechatStatus -= 0x10;
			}
		}
	}
	else if(wechatStatus > 0x30 && wechatStatus <0x40) {	//wait response
		if(wechatRcvLen >=wechatRcvTotalLen) {
			if((wechatRcvCmd&0xFFFC) ==0x4E20 && wechatRcvProtoalbufBaseResponseErr == 0) {	//OK
				wechatSeq++;
				switch(wechatStatus) {
					case WECHAT_ST_AUTH_RESPONSE_WAIT:
						wechatStatus = WECHAT_ST_INIT_REQ;
						break;
					case WECHAT_ST_INIT_RESPONSE_WAIT:
						wechatStatus = WECHAT_ST_MANUFACTURE_REQ;
						decodeProtobuf((WECHAT_RCV_BUF+8),wechatRcvTotalLen-8,15,(u8_t xdata *)&UTCTime,NULL);
						break;
					case WECHAT_ST_MANUFACTURE_RESPONSE_WAIT:
					case WECHAT_ST_SENDDATA_RESPONSE_WAIT:
						wechatStatus = WECHAT_ST_OK;
						break;
				}
				//YC_UARTSendData(WECHAT_RCV_BUF,wechatRcvLen);
			}
			else {
				wechatStatus = WECHAT_ST_FAIL;
			}
			wechatSendLen = 0;
			wechatRcvLen = 0;
		}
	}	
}


