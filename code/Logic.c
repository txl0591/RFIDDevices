/*********************************************************
  Copyright (C), 2013-2016
  File name:	LogicProc.c
  Author:   	
  Version:   	1.0
  Date: 		13.11.23
  Description:  
  History:            
*********************************************************/
#include "Include.h"

static INT8U mVersion = UARTFRAME_VERSION;
static INT8U mMasterCtl = FRAME_ADDR_NONE;
static INT8U mRFIDPwd[ICCARD_COUNT][6] =
{
	{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},
	{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},
	{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},
	{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},
	{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},	
	{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},
	{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},
	{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},
	{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},
	{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},
	{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},
	{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},
	{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},
	{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},
	{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},
	{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},
};

static INT8U mRFIDCrc[16] = {0x2A,0x61,0x66,0x5E,0x66,0x39,0x6F,0x32,0x77,0x35,0x66,0x76,0x46,0x31,0x23,0x51};
/*************************************************
  Function:		LogicRFIDGetPwd
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
static void LogicRFIDGetPwd(INT8U Key, INT8U* CardKey)
{
	memcpy(CardKey, &mRFIDPwd[Key][0], 6);
}


/*************************************************
  Function:		LogicRFIDGetDefaultPwd
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
static void LogicRFIDGetDefaultPwd(INT8U* CardKey)
{
	int i;
	for(i = 0; i < 6; i++)
	{
		CardKey[i]= 0xFF;
	}
}


/*************************************************
  Function:		LogicRFIDReadly
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
static INT8U LogicRFIDReadly(INT8U* CardNum)
{
	INT8U CardType[2];
	INT8U Num[6];
	INT8U ret = ECHO_OK;
	if(MI_OK != RFIDFIndCard(PICC_REQIDL, &CardType))
	{
		if(MI_OK != RFIDFIndCard(PICC_REQIDL, &CardType))
		{
			return ECHO_ERR;
		}
	}

	if(MI_OK != RFIDCollision(&Num))
	{
		return ECHO_ERR_COLLISION;
	}

	if(MI_OK != RFIDSelectCard(&Num))
	{
		return ECHO_ERR;
	}

	memcpy(CardNum,Num,6);
		
	return ECHO_OK;
}

/*************************************************
  Function:		LogicRFIDCrc
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
static INT8U LogicRFIDCrc(INT8U PwdMode, INT8U* CardNum, INT8U* CardType)
{
	INT8U Type, i,ret = ECHO_OK;
	INT8U CardBuf[48];
	INT8U CardKey[6];
	Type = RFID_CARD_NONE;
	LogicRFIDGetPwd(1,CardKey);
	for(i = 0; i < 3; i++)
	{
		if(MI_OK != RFIDCheckCardPwd(PwdMode, 4+i, CardKey, CardNum))
		{
			ret = ECHO_ERR_PASSWD;
			break;
		}

		if(MI_OK != RFIDReadBlock(4+i,CardBuf+i*16))
		{
			ret = ECHO_ERR_READDATA;
			break;
		}
	}

	if(ret == ECHO_OK)
	{
		for (i = 0; i < 16; i++)
		{
			if(CardBuf[i] != mRFIDCrc[i])
			{
				ret = ECHO_ERR_CHECK_CARD;
				break;
			}
		}

		if(CardBuf[16] == 0x00 && CardBuf[17] == 0x00)
		{
			Type = RFID_CARD_INIT;
		}

		if(CardBuf[16] == 0x01)
		{
			Type = RFID_CARD_USER;
		}

		if(CardBuf[17] == 0x01)
		{
			if(RFID_CARD_USER == Type)
			{
				Type = RFID_CARD_ALL;
			}
			else
			{
				Type = RFID_CARD_SYS;	
			}
		}
		*CardType = Type;
		
	}

	return ret;
}


/*************************************************
  Function:		LogicRFIDReadBlk
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
static void LogicRFIDReadBlk(INT8U Id, INT8U* Data)
{
	INT8U Encry = Data[0];
	INT8U Blk = Data[1];
	INT8U PwdMode = Data[2];
	INT8U PwdDefault = Data[3];
	INT8U CardBuf[56];
	INT8U CardKey[6];	
	INT8U CardNum[6];
	INT8U Len = 0;
	INT8U i,ret = ECHO_OK;
	INT8U CardType = RFID_CARD_NONE;
	#if _UART_DATA_ENY_ 
		
	#endif
	ret = LogicRFIDReadly(CardNum);

	if(ECHO_OK == ret)
	{	
		if(PwdDefault == 0)
		{
			LogicRFIDGetPwd(Blk,CardKey);
		}
		else
		{
			LogicRFIDGetDefaultPwd(CardKey);
		}
		for(i = 0; i < 3; i++)
		{
			if(MI_OK != RFIDCheckCardPwd(PwdMode, Blk*4+i, CardKey, CardNum))
			{
				ret = ECHO_ERR_PASSWD;
				break;
			}
	
			if(MI_OK != RFIDReadBlock(Blk*4+i,CardBuf+i*16+7))
			{
				ret = ECHO_ERR_READDATA;
				break;
			}
		}
	}

	CardBuf[0] = ret;
	CardBuf[1] = CardNum[0];
	CardBuf[2] = CardNum[1];
	CardBuf[3] = CardNum[2];
	CardBuf[4] = CardNum[3];
	CardBuf[5] = CardNum[4];
	CardBuf[6] = CardNum[5];	
	
	if(ECHO_OK == ret)
	{
		Len = 55;
	}
	else
	{	
		Len = 1;
	}

	#if (_LCD_MODULE_ != _LCD_NONE_)
	LCDLogECHO(ret, TEXT_LEFT);
	#endif
	AckCommand(RFID_READ_BLOCK,Id,CardBuf, Len);
}


/*************************************************
  Function:		LogicRFIDReadMultBlk
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
static void LogicRFIDReadMultBlk(INT8U Id, INT8U* Data)
{
	INT8U Encry = Data[0];
	INT8U Blk = Data[1];
	INT8U BlkLen = Data[2];
	INT8U PwdMode = Data[3];
	INT8U PwdDefault = Data[4];
	INT8U CardBuf[56];
	INT8U CardKey[6];	
	INT8U CardNum[6];
	INT8U Len = 0;
	INT8U j,i,ret = ECHO_OK;
	INT8U CardType = RFID_CARD_NONE;
	#if _UART_DATA_ENY_ 
		
	#endif
	ret = LogicRFIDReadly(CardNum);
	if(ECHO_OK == ret)
	{
		ret = LogicRFIDCrc(PwdMode,CardNum,&CardType);
		if(ECHO_OK == ret)
		{
			CardBuf[0] = ret;
			CardBuf[1] = READ_MULT_START;
			CardBuf[2] = CardNum[0];
			CardBuf[3] = CardNum[1];
			CardBuf[4] = CardNum[2];
			CardBuf[5] = CardNum[3];
			CardBuf[6] = CardNum[4];
			CardBuf[7] = CardNum[5];
			CardBuf[8] = CardType;
			CardBuf[9] = PwdDefault;
			AckCommandDirect(RFID_READ_MULT_BLOCK, Id, CardBuf, 10);
			
			for(j = 0; j < BlkLen; j++)
			{
				if(PwdDefault == 0)
				{
					LogicRFIDGetPwd(Blk,CardKey);
				}
				else
				{
					LogicRFIDGetDefaultPwd(CardKey);
				}
				
				for(i = 0; i < 3; i++)
				{
					if(MI_OK != RFIDCheckCardPwd(PwdMode, Blk*4+i, CardKey, CardNum))
					{
						ret = ECHO_ERR_PASSWD;
						break;
					}
			
					if(MI_OK != RFIDReadBlock(Blk*4+i,CardBuf+2))
					{
						ret = ECHO_ERR_READDATA;
						break;
					}

					if(ECHO_OK == ret)
					{
						CardBuf[0] = ret;
						CardBuf[1] = READ_MULT_CENTER;
						AckCommandDirect(RFID_READ_MULT_BLOCK, Id, CardBuf, 18);
					}
				}
				Blk += 1;
			}
		}
	}

	CardBuf[0] = ret;
	CardBuf[1] = READ_MULT_END;
	CardBuf[2] = CardNum[0];
	CardBuf[3] = CardNum[1];
	CardBuf[4] = CardNum[2];
	CardBuf[5] = CardNum[3];
	CardBuf[6] = CardNum[4];
	CardBuf[7] = CardNum[5];
	CardBuf[8] = CardType;
	Len = 9;

	#if (_LCD_MODULE_ != _LCD_NONE_)
	LCDLogECHO(ret, TEXT_LEFT);
	#endif
	AckCommand(RFID_READ_MULT_BLOCK,Id,CardBuf, Len);
}

/*************************************************
  Function:		LogicRFIDWriteBlk
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
static INT8U LogicRFIDWriteBlk(INT8U Id, INT8U* Data, INT8U len)
{
	INT8U Encry = Data[0];
	INT8U Blk = Data[1];
	INT8U PwdMode = Data[2];
	INT8U *CardBuf = &Data[3];
	INT8U CardKey[6];	
	INT8U CardNum[6];
	INT8U Len = 0;
	INT8U WriteLen = len-3;
	INT8U Write = 0;
	INT8U i,ret = ECHO_OK;
	INT8U tmp[16];
	INT8U DefaultKey = 0;
	INT8U DefaultKeyBuf[16] = 
	{
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xff,0x07,0x80,0x69,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	
	};
	#if _UART_DATA_ENY_ 
		
	#endif
	ret = LogicRFIDReadly(CardNum);
	
	if(ECHO_OK == ret)
	{
		i = 0;
		LogicRFIDGetPwd(Blk,CardKey);		
		while(WriteLen > 0)
		{	
			if(DefaultKey == 0)
			{
				if(MI_OK != RFIDCheckCardPwd(PwdMode, Blk*4+i, CardKey, CardNum))
				{
					if(MI_OK != LogicRFIDReadly(CardNum))
					{
						ret = ECHO_ERR;
						break;
					}
					if(MI_OK != RFIDCheckCardPwd(PwdMode, Blk*4+i, DefaultKeyBuf, CardNum))
					{
						ret = ECHO_ERR_PASSWD;
						break;
					}
					else
					{
						DefaultKey = 1;
					}
				}
			}
			else
			{
				if(MI_OK != RFIDCheckCardPwd(PwdMode, Blk*4+i, DefaultKeyBuf, CardNum))
				{
					ret = ECHO_ERR_PASSWD;
					break;
				}
				else
				{
					DefaultKey = 1;
				}
			}
		
			
			if(WriteLen >= 16)
			{
				if(MI_OK != RFIDWriteBlock(Blk*4+i,CardBuf+i*16))
				{
					ret = ECHO_ERR_WRITEDATA;
					break;
				}
			}
			else
			{
				if(MI_OK != RFIDReadBlock(Blk*4+i,tmp))
				{
					ret = ECHO_ERR_WRITEDATA;
					break;
				}

				memcpy(tmp, CardBuf+i*16, WriteLen);
				if(MI_OK != RFIDWriteBlock(Blk*4+i,tmp))
				{
					ret = ECHO_ERR_WRITEDATA;
					break;
				}
				
			}
			if(WriteLen > 16)
			{
				WriteLen -= 16;
				Write += 16;
			}
			else
			{
				Write += WriteLen;
				WriteLen = 0;
			}

			if(DefaultKey == 1)
			{
				memcpy(DefaultKeyBuf, CardKey, 6);
				if(MI_OK != RFIDWriteBlock(Blk*4+3,DefaultKeyBuf))
				{
					ret = ECHO_ERR_WRITEPWD;
					break;
				}
				else
				{
					DefaultKey = 0;
				}
			}
			
			i++;
		}
	}

	CardBuf[0] = ret;
	CardBuf[1] = Write;
	Len = 2;

	#if (_LCD_MODULE_ != _LCD_NONE_)
	LCDLogECHO(ret, TEXT_LEFT);
	#endif
	AckCommand(RFID_WRITE_BLOCK,Id,CardBuf, Len);
	
	return ret;
}

/*************************************************
  Function:		LogicRFIDWriteMultBlk
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
static INT8U LogicRFIDWriteMultBlk(INT8U Id, INT8U* Data, INT8U len)
{
	INT8U Encry = Data[0];
	INT8U Blk = Data[1];
	INT8U BlkIndex = Data[2];
	INT8U PwdMode = Data[3];
	INT8U *CardBuf = &Data[4];
	INT8U CardKey[6];	
	INT8U CardNum[6];
	INT8U Len = 0;
	INT8U WriteLen = len-4;
	INT8U Write = 0;
	INT8U ret = ECHO_OK;
	INT8U tmp[16];
	INT8U DefaultKey = 0;
	INT8U DefaultKeyBuf[16] = 
	{
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xff,0x07,0x80,0x69,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	
	};
	#if _UART_DATA_ENY_ 	
			
	#endif
	ret = LogicRFIDReadly(CardNum);
	
	if(ECHO_OK == ret)
	{
		LogicRFIDGetPwd(Blk,CardKey);
		if(MI_OK != RFIDCheckCardPwd(PwdMode, Blk*4+BlkIndex, CardKey, CardNum))
		{
			if(MI_OK != LogicRFIDReadly(CardNum))
			{
				ret = ECHO_ERR;
			}
			else
			{
				
				if(MI_OK != RFIDCheckCardPwd(PwdMode, Blk*4+BlkIndex, DefaultKeyBuf, CardNum))
				{
					ret = ECHO_ERR_PASSWD;
				}
				else
				{
					DefaultKey = 1;
				}
			}
		}
		
		if(ret == ECHO_OK)
		{
			if(WriteLen >= 16)
			{
				if(MI_OK != RFIDWriteBlock(Blk*4+BlkIndex,CardBuf))
				{
					ret = ECHO_ERR_WRITEDATA;
				}
			}
			else
			{
				if(MI_OK != RFIDReadBlock(Blk*4+BlkIndex,tmp))
				{
					ret = ECHO_ERR_WRITEDATA;
				}

				memcpy(tmp, CardBuf, WriteLen);
				if(MI_OK != RFIDWriteBlock(Blk*4+BlkIndex,tmp))
				{
					ret = ECHO_ERR_WRITEDATA;
				}
				
			}

			if(DefaultKey == 1)
			{
				memcpy(DefaultKeyBuf, CardKey, 6);
				if(MI_OK != RFIDWriteBlock(Blk*4+3,DefaultKeyBuf))
				{
					ret = ECHO_ERR_WRITEPWD;
				}
			}
		}
		
	}

	CardBuf[0] = ret;
	Len = 1;

#if (_LCD_MODULE_ != _LCD_NONE_)
	LCDLogECHO(ret, TEXT_LEFT);
#endif
	AckCommand(RFID_WRITE_MULT_BLOCK,Id,CardBuf, Len);
	
	return ret;
}


/*************************************************
  Function:		LogicRFIDWritePwd
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
static INT8U LogicRFIDWritePwd(INT8U Id, INT8U* Data)
{
	INT8U Encry = Data[0];
	INT8U Blk = Data[1];
	INT8U BlkLen = Data[2];
	INT8U PwdMode = Data[3];
	INT8U CardKey[7];	
	INT8U CardNum[6];
	INT8U Len = 0;
	INT8U ret = ECHO_OK;
	INT8U i;
	INT8U CardBuf[16] = 
	{
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xff,0x07,0x80,0x69,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	
	};
	INT8U DefaultKeyBuf[16] = 
	{
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xff,0x07,0x80,0x69,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	
	};
	#if _UART_DATA_ENY_ 
		
	#endif
	ret = LogicRFIDReadly(CardNum);
	
	if(ECHO_OK == ret)
	{
		for (i = 0; i < BlkLen; i++)
		{
			if(MI_OK == RFIDCheckCardPwd(PwdMode, Blk*4+3, DefaultKeyBuf, CardNum))
			{ 	
				if(RFID_PWD_A == PwdMode)
				{
					LogicRFIDGetPwd(Blk,CardKey);
					memcpy(CardBuf,CardKey,6);
				}
				else
				{
					LogicRFIDGetPwd(Blk,CardKey);
					memcpy(CardBuf+10,CardKey,6);
				}
				if(MI_OK != RFIDWriteBlock(Blk*4+3,CardBuf))
				{
					ret = ECHO_ERR_WRITEPWD;
					break;
				}
			}
			Blk++;
		} 
	}
	
	DefaultKeyBuf[0] = ret;
	DefaultKeyBuf[1] = Blk;
	Len = 2;
	#if (_LCD_MODULE_ != _LCD_NONE_)
	LCDLogECHO(ret, TEXT_LEFT);
	#endif
	AckCommand(RFID_SET_PASSWD,Id,DefaultKeyBuf, Len);
	
	return ret;
}

/*************************************************
  Function:		LogicRFIDClearPwd
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
static INT8U LogicRFIDClearPwd(INT8U Id, INT8U* Data)
{
	INT8U Encry = Data[0];
	INT8U Blk = Data[1];
	INT8U BlkLen = Data[2];
	INT8U PwdMode = Data[3];
	INT8U CardKey[7];	
	INT8U CardNum[6];
	INT8U Len = 0;
	INT8U ret = ECHO_OK;
	INT8U j;
	INT8U DefaultKeyBuf[16] = 
	{
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xff,0x07,0x80,0x69,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	
	};
	#if _UART_DATA_ENY_ 
		
	#endif
	ret = LogicRFIDReadly(CardNum);
		
	if(ECHO_OK == ret)
	{
		for(j = 0; j < BlkLen; j++)
		{
			if(MI_OK != RFIDCheckCardPwd(PwdMode, Blk*4+3, DefaultKeyBuf, CardNum))
			{
				LogicRFIDReadly(CardNum);
				LogicRFIDGetPwd(Blk,CardKey);
				if(MI_OK == RFIDCheckCardPwd(PwdMode, Blk*4+3, CardKey, CardNum))
				{
					if(MI_OK != RFIDWriteBlock(Blk*4+3,DefaultKeyBuf))
					{
						ret = ECHO_ERR_WRITEPWD;
						break;
					}			
				}
				else
				{
					ret = ECHO_ERR_PASSWD;
					break;
				}
				
			}	  			
			Blk++;
		}
	}

	DefaultKeyBuf[0] = ret;
	DefaultKeyBuf[1] = Blk;
	Len = 2;

	#if (_LCD_MODULE_ != _LCD_NONE_)
	LCDLogECHO(ret, TEXT_LEFT);
	#endif
	AckCommand(RFID_CLR_PASSWD,Id,DefaultKeyBuf, Len);
	
	return ret;
}

/*************************************************
  Function:		LogicRFIDBeep
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
static void LogicRFIDBeep(INT8U Id,INT8U* Data)
{
	INT8U type = Data[0];
	if(RFID_BEEP_OK == type)
	{
		SetBeepIndex(_HW_BEEP_OK_);
	}
	else
	{
		SetBeepIndex(_HW_BEEP_ERR_);
	}
	type = ECHO_OK;
	AckCommand(RFID_BEEP,Id, &type, 1);
}

/*************************************************
  Function:		LogicRFIDGetVersion
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
static void LogicRFIDGetVersion(INT8U Id)
{
	INT8U buf[4];
	
	buf[0] = ECHO_OK;
	buf[1] = _HW_VERSION_;
	buf[2] = _SW_VERSION_;
	buf[3] = StorageReadByte(ADDRESS_PWD_VERSION);
	#if (_LCD_MODULE_ != _LCD_NONE_)
	LCDLogECHO(ECHO_OK, TEXT_LEFT);
	#endif
	AckCommand(RFID_GET_VERSION,Id,buf, 4);
}

/*************************************************
  Function:		LogicRFIDSetAntena
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
static void LogicRFIDSetAntena(INT8U Id, INT8U* Data)
{
	INT8U buf = ECHO_OK;
	if (Data[0] == RFID_ANTENNA_ON)
	{
		RFIDAntennaOn();
	}
	else
	{
		RFIDAntennaOff();
	}

	#if (_LCD_MODULE_ != _LCD_NONE_)
	LCDLogECHO(ECHO_OK, TEXT_LEFT);
	#endif
	AckCommand(RFID_ANTENNA,Id, &buf, 1);
}


/*************************************************
  Function:		LogicRFIDGetCrc
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
static void LogicRFIDGetCrc(INT8U Id, INT8U* Data)
{
	INT8U Encry = Data[0];
	INT8U PwdMode = Data[1];
	INT8U CardNum[6];
	INT8U Len = 0;
	INT8U buf[8];
	INT8U ret;
	INT8U CardType = RFID_CARD_NONE;
	CardNum[0] = 0x00;
	CardNum[1] = 0x00;
	CardNum[2] = 0x00;
	CardNum[3] = 0x00;
	CardNum[4] = 0x00;
	CardNum[5] = 0x00;
	ret = LogicRFIDReadly(CardNum);
	if(ECHO_OK == ret)
	{
		ret = LogicRFIDCrc(PwdMode,CardNum,&CardType);
	}

	buf[0] = ret;
	buf[1] = CardNum[0];
	buf[2] = CardNum[1];
	buf[3] = CardNum[2];
	buf[4] = CardNum[3];
	buf[5] = CardNum[4];
	buf[6] = CardNum[5];
	buf[7] = CardType;
	Len = 8;
	AckCommand(RFID_GET_CRCBLK,Id, buf, Len);
}

/*************************************************
  Function:		LogicRFIDGetCrc
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
static void LogicRFIDSetCrc(INT8U Id, INT8U* Data, INT8U nLen)
{
	INT8U Encry = Data[0];
	INT8U *CardBuf = &Data[1];
	INT8U Len = nLen-1;
	INT8U ret = ECHO_OK;
	INT8U CardNum;
	INT8U i;

	if(Len > 16)
	{
		Len = 16;
	}

	for(i = 0; i < Len; i++)
	{
		StorageWriteByte(ADDRESS_CHECK+i,CardBuf[i]);
	}

	for(i = 0; i < Len; i++)
	{
		CardNum = StorageReadByte(ADDRESS_CHECK+i);
		if (CardNum != *(CardBuf+i))
		{
			ret = ECHO_ERR;
			break;
		}
	}
		
	AckCommand(RFID_SET_CRCBLK,Id, &ret, 1);
}

/*************************************************
  Function:		LogicRFIDGetCardID
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
static void LogicRFIDGetCardID(INT8U Id)
{
	INT8U ret = ECHO_ERR;
	INT8U buf[7];
	INT8U CardNum[6];

	CardNum[0] = 0x00;
	CardNum[1] = 0x00;
	CardNum[2] = 0x00;
	CardNum[3] = 0x00;
	CardNum[4] = 0x00;
	CardNum[5] = 0x00;

	
	ret = LogicRFIDReadly(CardNum);
	
	buf[0] = ret;
	buf[1] = CardNum[0];
	buf[2] = CardNum[1];
	buf[3] = CardNum[2];
	buf[4] = CardNum[3];
	buf[5] = CardNum[4];
	buf[6] = CardNum[5];

	#if (_LCD_MODULE_ != _LCD_NONE_)
	LCDLogECHO(ECHO_OK, TEXT_LEFT);
	#endif
	AckCommand(RFID_READ_CARDID,Id,buf, 7);
}


/*************************************************
  Function:		LogicGetFrameVersion
  Description:  获取协议版本
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
INT8U LogicGetFrameVersion(void)
{
	return mVersion;
}

/*************************************************
  Function:		LogicSetFrameVersion
  Description:  获取协议版本
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void LogicSetFrameVersion(INT8U Version)
{
	mVersion = Version;
}

/*************************************************
  Function:		LogicGetMasterCtl
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
INT8U LogicGetMasterCtl(void)
{
	return mMasterCtl;
}

/*************************************************
  Function:		LogicGetMasterCtl
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void LogicSetMasterCtl(INT8U Ctl)
{
	mMasterCtl = Ctl;
}

/*************************************************
  Function:		PasswordInit
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
static void PasswordInit(void)
{
	int i;
	INT8U CardKey[6];
	for(i = 0; i < ICCARD_COUNT; i++)
	{
		StorageRead(ADDRESS_PASSWD(i),CardKey,6);
		memcpy(&mRFIDPwd[i][0], CardKey, 6);
	}

	StorageRead(ADDRESS_CHECK,mRFIDCrc,16);	
}

/*************************************************
  Function:		OtherInit
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
static void OtherInit(void)
{
	INT8U ret = StorageReadByte(ADDRESS_UART_VERSION);
	LogicSetFrameVersion(ret);
}


/*************************************************
  Function:		LogicRFIDRecover
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
static void LogicRFIDRecover(INT8U Id)
{
	INT8U type = ECHO_OK;
	StorageRecover();
	PasswordInit();
	OtherInit();
	AckCommand(RFID_RECOVER_SYS,Id, &type, 1);
}


/*************************************************
  Function:		LogicDistribute
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void LogicDistribute(INT8U Cmd, INT8U NeedAck, INT8U Id, INT8U* Data, INT8U len)
{
	INT8U mNeedAck = NeedAck;
	INT8U mlen = len;
	
	switch (Cmd)
	{
		case RFID_SET_PASSWD:
			LogicRFIDWritePwd(Id,Data);
			break;
			
		case RFID_CLR_PASSWD:
			LogicRFIDClearPwd(Id,Data);
			break;

		case RFID_GET_CRCBLK:
			LogicRFIDGetCrc(Id,Data);
			break;

		case RFID_SET_CRCBLK:
			LogicRFIDSetCrc(Id,Data,len);
			break;

		case RFID_WRITE_BLOCK:
			LogicRFIDWriteBlk(Id,Data,len);
			break;
			
		case RFID_READ_BLOCK:
			LogicRFIDReadBlk(Id,Data);
			break;

		case RFID_WRITE_MULT_BLOCK:
			LogicRFIDWriteMultBlk(Id,Data,len);
			break;

		case RFID_READ_MULT_BLOCK:
			LogicRFIDReadMultBlk(Id,Data);
			break;

		case RFID_GET_VERSION:
			LogicRFIDGetVersion(Id);
			break;
			
		case RFID_ANTENNA:
			LogicRFIDSetAntena(Id, Data);
			break;

		case RFID_BEEP:
			LogicRFIDBeep(Id,Data);
			break;

		case RFID_RECOVER_SYS:
			LogicRFIDRecover(Id);
			break;

		case RFID_POWER_SYNC:
			LogicSetMasterCtl(Data[0]);
			break;	

		case RFID_READ_CARDID:
			LogicRFIDGetCardID(Id);
			break;

		default:
			break;
	}
}

/*************************************************
  Function:		LogicResponsion
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void LogicResponsion(INT8U Cmd, INT8U NeedAck, INT8U Id, INT8U* Data, INT8U len)
{
	INT8U nNeedAck = NeedAck;
	INT8U nId = Id;
	INT8U* nData = Data;
	INT8U nlen = len;
		
	switch (Cmd)
	{
		case RFID_SET_PASSWD:
			break;
			
		case RFID_CLR_PASSWD:
			break;

		case RFID_WRITE_BLOCK:
			break;
			
		case RFID_READ_BLOCK:
			break;

		case RFID_GET_VERSION:
			break;
			
		case RFID_POWER_SYNC:
			LogicSetMasterCtl(Data[0]);
			break;

		case RFID_RECOVER_SYS:
			break;

		default:
			break;
	}
}

/*************************************************
  Function:		LogicProc
  Description:  命令解析
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void LogicProc(void)
{
	INT8U FrameType;
	INT8U FrameAck;
	INT8U FrameDst;
	INT8U FrameCmd;
	INT8U FrameID;
	
	PUART_FRAME Frame = (PUART_FRAME)UartGetFrame();
	
	if (NULL != Frame && Frame->State == FRAMEBUF_OK)
	{
		FrameDst = Frame->Buf[FTRAME_INDEX_DST]; 
		FrameID = Frame->Buf[FTRAME_INDEX_ID];
		
		if(Frame->Len < FRAME_MIN_SIZE)
		{
			#if (_LCD_MODULE_ != _LCD_NONE_)
			LCDLogD(LOG_DATA_SIZE, SIZEOF(LOG_DATA_SIZE), TEXT_LEFT);	
			#endif
		}

		if(FrameDst != FRAME_ADDR_RFID)
		{
			#if (_LCD_MODULE_ != _LCD_NONE_)
			LCDLogD(LOG_DATA_ADDR, SIZEOF(LOG_DATA_ADDR), TEXT_LEFT);	
			#endif
		}
		
		if (Frame->Len > FRAME_MIN_SIZE && FrameDst == FRAME_ADDR_RFID)
		{
			FrameType = Frame->Buf[FTRAME_INDEX_CTRL] & FRAME_ACK;
			FrameAck = Frame->Buf[FTRAME_INDEX_CTRL] & FRAME_NONEED_ACK;
			FrameCmd = Frame->Buf[FTRAME_INDEX_CMD];
			if (FrameType == FRAME_SEND)
			{
				LogicDistribute(FrameCmd, FrameAck, FrameID, &Frame->Buf[FTRAME_INDEX_DATA], Frame->Buf[FTRAME_INDEX_LEN]-3);
			}
			else
			{
				LogicResponsion(FrameCmd, FrameAck, FrameID, &Frame->Buf[FTRAME_INDEX_DATA], Frame->Buf[FTRAME_INDEX_LEN]-3);
			}
		}
		Frame->State = FRAMEBUF_NONE;
	}
}

/*************************************************
  Function:		LogicInit
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void LogicInit(void)
{
	HarewareInit();
	StorageInit();
	PasswordInit();
	OtherInit();
	SendCommandAck(RFID_POWER_SYNC, NULL, 0);
	SetBeepIndex(_HW_BEEP_OK_);
}

