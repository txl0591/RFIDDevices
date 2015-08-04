/*********************************************************
  Copyright (C), 2013-2016
  File name:	ReadRFID.c
  Author:   	
  Version:   	1.0
  Date: 		13.11.23
  Description:  读卡模块
  History:            
*********************************************************/
#include "Include.h"

#define _RFID_MAX_LEN_  		18
#define _RFID_WAIT_TIME_		2000
#define _RFID_WORK_MODE_		ISO14443_A

/*************************************************
  Function:		ReadRegister
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
static INT8U ReadRegister(INT8U Address)
{
	INT8U ucAddr = ((Address<<1)&0x7E)|0x80;
	return SPIReadByte(ucAddr);
}

/*************************************************
  Function:		WriteRegister
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
static void WriteRegister(INT8U Address, INT8U value)
{  
    INT8U ucAddr = ((Address<<1)&0x7E);
	SPIWriteByte(ucAddr, value);
}   

/*************************************************
  Function:		ClearRegisterBit
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
static void ClearRegisterBit(INT8U reg, INT8U mask)  
{
    char tmp = 0x00;
    tmp = ReadRegister(reg);
    WriteRegister(reg, tmp & ~mask);  
} 

/*************************************************
  Function:		MaskRegisterBit
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
static void MaskRegisterBit(INT8U reg,INT8U mask)  
{
    char tmp = 0x00;
    tmp = ReadRegister(reg);
    WriteRegister(reg,tmp | mask); 
}

/*************************************************
  Function:		CalulateCRC
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
static void CalulateCRC(INT8U *pIndata, INT8U len, INT8U *pOutData)
{
    INT8U i,n;
    ClearRegisterBit(DivIrqReg,0x04);
    WriteRegister(CommandReg,PCD_IDLE);
    MaskRegisterBit(FIFOLevelReg,0x80);
    for (i = 0; i < len; i++)
    {   
    	WriteRegister(FIFODataReg, *(pIndata+i));   
	}

	WriteRegister(CommandReg, PCD_CALCCRC);
    i = 0xFF;
    do 
    {
        n = ReadRegister(DivIrqReg);
        i--;
    }
    while ((i != 0) && !(n&0x04));
	
    pOutData[0] = ReadRegister(CRCResultRegL);
    pOutData[1] = ReadRegister(CRCResultRegM);
}

/*************************************************
  Function:		RFIDReadCom
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
static int RFIDReadCom(INT8U Command, INT8U *pInData, 
                 INT8U InLenByte,INT8U *pOutData, 
                 INT16U *pOutLenBit)
{
    int status = MI_ERR;
    INT8U irqEn   = 0x00;
    INT8U waitFor = 0x00;
    INT8U lastBits;
    INT8U n;
    INT16U i;
    switch (Command)
    {
        case PCD_AUTHENT:
			irqEn   = 0x12;
			waitFor = 0x10;
			break;
		case PCD_TRANSCEIVE:
			irqEn   = 0x77;
			waitFor = 0x30;
			break;
		default:
			break;
    }
   
    WriteRegister(ComIEnReg,irqEn|0x80);
    ClearRegisterBit(ComIrqReg,0x80);
    WriteRegister(CommandReg,PCD_IDLE);
    MaskRegisterBit(FIFOLevelReg,0x80);
    
    for (i = 0; i < InLenByte; i++)
    {   
		WriteRegister(FIFODataReg, pInData[i]);    
	}
    WriteRegister(CommandReg, Command);
   
    if (Command == PCD_TRANSCEIVE)
    {    
		MaskRegisterBit(BitFramingReg,0x80);  
	}
    
	i = _RFID_WAIT_TIME_;
    do 
    {
        n = ReadRegister(ComIrqReg);
        i--;
    }
    while ((i != 0) && !(n&0x01) && !(n&waitFor));
    ClearRegisterBit(BitFramingReg,0x80);

    if (i != 0)
    {    
        if(!(ReadRegister(ErrorReg)&0x1B))
        {
            status = MI_OK;
            if (n & irqEn & 0x01)
            {
            	status = MI_NOTAGERR;   
			}

			if (Command == PCD_TRANSCEIVE)
            {
               	n = ReadRegister(FIFOLevelReg);
              	lastBits = ReadRegister(ControlReg) & 0x07;
                if (lastBits)
                {   
					*pOutLenBit = (n-1)*8 + lastBits;   
				}
                else
                {   
					*pOutLenBit = n*8;   
				}
                if (n == 0)
                {   
					n = 1;    
				}
                if (n > _RFID_MAX_LEN_)
                {   
					n = _RFID_MAX_LEN_;   
				}
                for (i = 0; i < n; i++)
                {   
					pOutData[i] = ReadRegister(FIFODataReg);    
				}
            }
        }
        else
        {   
			status = MI_ERR;   
		}
        
    }
   
    MaskRegisterBit(ControlReg,0x80); 
    WriteRegister(CommandReg,PCD_IDLE); 
    return status;
}
                      
/*************************************************
  Function:		RFIDFIndCard
  Description:  
  Input:		
  	1.req_code	 0x52 = 寻感应区内所有符合14443A标准的卡
  				 0x26 = 寻未进入休眠状态的卡
  Output:		
  	1.pTagType
  				 0x4400 = Mifare_UltraLight
                 0x0400 = Mifare_One(S50)
                 0x0200 = Mifare_One(S70)
                 0x0800 = Mifare_Pro(X)
                 0x4403 = Mifare_DESFire
  Return:		
  Others:
*************************************************/
int RFIDFIndCard(INT8U req_code,INT8U *pTagType)
{
	int status;  
	INT16U unLen;
	INT8U ucComMF522Buf[_RFID_MAX_LEN_]; 

	ClearRegisterBit(Status2Reg,0x08);
	WriteRegister(BitFramingReg,0x07);
	MaskRegisterBit(TxControlReg,0x03);
 
	ucComMF522Buf[0] = req_code;

	status = RFIDReadCom(PCD_TRANSCEIVE,ucComMF522Buf,1,ucComMF522Buf,&unLen);

	if ((status == MI_OK) && (unLen == 0x10))
	{    
		*pTagType     = ucComMF522Buf[0];
		*(pTagType+1) = ucComMF522Buf[1];
	}
	else
	{   
		status = MI_ERR;   
	}
   
	return status;
}

/*************************************************
  Function:		RFIDCollision
  Description:  RFID 防冲撞
  Input:		
  	1.	pSnr	CardID
  Output:		
  Return:		
  Others:
*************************************************/
int RFIDCollision(INT8U *pSnr)
{
    int status;
    INT8U i,snr_check=0;
    INT16U unLen;
    INT8U ucComMF522Buf[_RFID_MAX_LEN_]; 
    
    ClearRegisterBit(Status2Reg,0x08);
    WriteRegister(BitFramingReg,0x00);
    ClearRegisterBit(CollReg,0x80);
 
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x20;

    status = RFIDReadCom(PCD_TRANSCEIVE,ucComMF522Buf,2,ucComMF522Buf,&unLen);

    if (status == MI_OK)
	{
		for (i=0; i<4; i++)
		{   
			*(pSnr+i)  = ucComMF522Buf[i];
			snr_check ^= ucComMF522Buf[i];
		}
		if (snr_check != ucComMF522Buf[i])
   		{   
			status = MI_ERR;    
		}
    }
    
    MaskRegisterBit(CollReg,0x80);
    return status;
}

/*************************************************
  Function:		RFIDSelectCard
  Description:  RFID 选卡
  Input:		
  	1.	CardID	
  Output:		
  Return:		
  Others:
*************************************************/
int RFIDSelectCard(INT8U *CardId)
{
    int status;
    INT8U i;
    INT16U unLen;
    INT8U ucComMF522Buf[_RFID_MAX_LEN_]; 
    
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x70;
    ucComMF522Buf[6] = 0;
    for (i = 0; i < 4; i++)
    {
    	ucComMF522Buf[i+2] = *(CardId+i);
    	ucComMF522Buf[6]  ^= *(CardId+i);
    }
    CalulateCRC(ucComMF522Buf,7,&ucComMF522Buf[7]);
  
    ClearRegisterBit(Status2Reg,0x08);

    status = RFIDReadCom(PCD_TRANSCEIVE,ucComMF522Buf,9,ucComMF522Buf,&unLen);
    
    if ((status == MI_OK) && (unLen == 0x18))
    {   
		status = MI_OK;  
	}
    else
    {   
		status = MI_ERR;    
	}

    return status;
}

/*************************************************
  Function:		RFIDSelectCard
  Description:  RFID 命令卡片进入休眠状态
  Input:		
  	1.	CardID	
  Output:		
  Return:		
  Others:
*************************************************/
int RFIDCardIdle(void)
{
    int status;
    INT16U unLen;
    INT8U ucComMF522Buf[_RFID_MAX_LEN_]; 

    ucComMF522Buf[0] = PICC_HALT;
    ucComMF522Buf[1] = 0;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = RFIDReadCom(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    return status;
}


/*************************************************
  Function:		RFIDCheckCardPwd
  Description:  RFID 验证卡片密码
  Input:		
  	1.	auth_mode	0x60 = 验证A密钥  0x61 = 验证B密钥 
  	2.	Block		块地址
  	3.	Pwd			密码
  	4.	CardId		卡片序列号
  Output:		
  Return:		
  Others:
*************************************************/
int RFIDCheckCardPwd(INT8U auth_mode,INT8U Block,INT8U *Pwd,INT8U *CardId)
{
    int status;
    INT16U unLen;
    INT8U ucComMF522Buf[_RFID_MAX_LEN_]; 

    ucComMF522Buf[0] = auth_mode;
    ucComMF522Buf[1] = Block;
	memcpy(&ucComMF522Buf[2], Pwd, 6); 
	memcpy(&ucComMF522Buf[8], CardId, 6); 
    
    status = RFIDReadCom(PCD_AUTHENT,ucComMF522Buf,12,ucComMF522Buf,&unLen);
    if ((status != MI_OK) || (!(ReadRegister(Status2Reg) & 0x08)))
    {   
		status = MI_ERR;   
	}
    
    return status;
}

/*************************************************
  Function:		RFIDReadBlock
  Description:  RFID 读取一块数据
  Input:		
  	1.	addr	
  	2.	pData	
  Output:		
  Return:		
  Others:
*************************************************/
int RFIDReadBlock(INT8U addr,INT8U *pData)
{
    int status;
    INT16U unLen;
    INT8U ucComMF522Buf[_RFID_MAX_LEN_]; 

    ucComMF522Buf[0] = PICC_READ;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
   
    status = RFIDReadCom(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
    if ((status == MI_OK) && (unLen == 0x90))
   	{   
		memcpy(pData, ucComMF522Buf, 16);   
	}
    else
    {   
		status = MI_ERR;   
	}
    
    return status;
}

/*************************************************
  Function:		RFIDWriteBlock
  Description:  RFID 写一块数据
  Input:		
  	1.	addr	
  	2.	pData	
  Output:		
  Return:		
  Others:
*************************************************/  
int RFIDWriteBlock(INT8U addr,INT8U *pData)
{
    int status;
    INT16U unLen;
    INT8U ucComMF522Buf[_RFID_MAX_LEN_]; 
    
    ucComMF522Buf[0] = PICC_WRITE;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = RFIDReadCom(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   
		status = MI_ERR;   
	}
        
    if (status == MI_OK)
    {
        memcpy(ucComMF522Buf, pData, 16);
        CalulateCRC(ucComMF522Buf,16,&ucComMF522Buf[16]);

        status = RFIDReadCom(PCD_TRANSCEIVE,ucComMF522Buf,18,ucComMF522Buf,&unLen);
        if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
        {   
			status = MI_ERR;   
		}
    }
    
    return status;
}

/*************************************************
  Function:		RFIDAntennaOn
  Description:  RFID 打开天线
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/  
void RFIDAntennaOn(void)
{
    INT8U i;
    i = ReadRegister(TxControlReg);
    if (!(i & 0x03))
    {
        MaskRegisterBit(TxControlReg, 0x03);
    }
}

/*************************************************
  Function:		RFIDAntennaOff
  Description:  RFID 关闭天线
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/  
void RFIDAntennaOff(void)
{
	ClearRegisterBit(TxControlReg, 0x03);
}

/*************************************************
  Function:		RFIDReset
  Description:  RFID 复位
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/  
int RFIDReset(void)
{
	SET_RFIDRST;
    delay_ms(1);
	CLR_RFIDRST;
    delay_ms(1);
	SET_RFIDRST;
    delay_ms(1);
    WriteRegister(CommandReg,PCD_RESETPHASE);
    delay_ms(1);
    
    WriteRegister(ModeReg,0x3D);           
    WriteRegister(TReloadRegL,30);           
    WriteRegister(TReloadRegH,0);
    WriteRegister(TModeReg,0x8D);
    WriteRegister(TPrescalerReg,0x3E);
	WriteRegister(TxAutoReg,0x40);
    return MI_OK;
}

/*************************************************
  Function:		RFIDConfigMode
  Description:  RFID 配置
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/  
int RFIDConfigMode(INT8U type)
{
	int ret = MI_ERR;
	
	switch (type)
	{
		case ISO14443_A:
	        ClearRegisterBit(Status2Reg,0x08);
	        WriteRegister(ModeReg,0x3D);
	        WriteRegister(RxSelReg,0x86);
	        WriteRegister(RFCfgReg,0x7F);
	   	    WriteRegister(TReloadRegL,30);
		    WriteRegister(TReloadRegH,0);
	        WriteRegister(TModeReg,0x8D);
		    WriteRegister(TPrescalerReg,0x3E);
		    delay_ns(1000);
	        RFIDAntennaOn();
			ret = MI_OK;
			break;

		default:
			break;
	}

	return ret;
}

/*************************************************
  Function:		RFIDInit
  Description:  RFID 初始化
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/  
void RFIDInit(void)
{
	RFIDReset();
	RFIDAntennaOff();  
	RFIDAntennaOn();
	RFIDConfigMode(ISO14443_A);
	RFIDAntennaOff();
}

