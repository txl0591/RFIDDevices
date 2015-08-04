/*********************************************************
  Copyright (C), 2013-2016
  File name:	Uart.c
  Author:   	
  Version:   	1.0
  Date: 		13.11.23
  Description:  
  History:                   
    1. Date:
       Author:
       Modification:
    2. ...
*********************************************************/
#include "Include.h"
#include "Hardware.h"
#include "Lcd.h"
#define _UART_RECV_OVERTIME_		10

static INT8U data mUartID = 0;
static UART_FRAME mUartRecvBuf[UART_RECV_MAX];
static INT8U mUartRecvIndex = 0;
static INT8U mUartStartRecv = UART_RECV_STOP;
static INT8U mUartNeedRecv = 0;
static UART_FRAME xdata mUartSendBuf;
static INT8U mUartRecvTick = 0;
static INT8U mUartLogID = UART_LOG_NONE;
static INT8U Recv = 0;
/*************************************************
  Function:		UartSwitchBuf
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
static void UartSwitchBuf(void)
{
	mUartRecvIndex++;
	if(mUartRecvIndex >=  UART_RECV_MAX)
	{
		mUartRecvIndex = 0;
	}
}

/*************************************************
  Function:		UartInterruptReceiveCom1
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void UartInterruptReceiveCom1(void) interrupt 4 using 1
{
	INT8U ReByte;
	if(RI == 1)
	{
		RI = 0;
		ReByte = SBUF;
		if (mUartStartRecv == UART_RECV_STOP)
		{
			if(ReByte == 0xAA && mUartRecvBuf[mUartRecvIndex].State == FRAMEBUF_NONE)
			{
				mUartStartRecv = UART_RECV_HEAD;
				mUartRecvBuf[mUartRecvIndex].State = FRAMEBUF_RECV;
				mUartRecvBuf[mUartRecvIndex].Len = 0;
				mUartRecvBuf[mUartRecvIndex].Buf[mUartRecvBuf[mUartRecvIndex].Len] = 0xAA;
				mUartRecvBuf[mUartRecvIndex].Len++;
				mUartNeedRecv = 5;
				mUartRecvTick = 1;
			}
		}
		else
		{
			if (mUartStartRecv == UART_RECV_HEAD)
			{
				mUartRecvBuf[mUartRecvIndex].Buf[mUartRecvBuf[mUartRecvIndex].Len] = ReByte;
				mUartNeedRecv--;
				if(mUartNeedRecv == 0)
				{
					mUartNeedRecv = mUartRecvBuf[mUartRecvIndex].Buf[mUartRecvBuf[mUartRecvIndex].Len];
					mUartStartRecv = UART_RECV_DATA;
				}	
				mUartRecvBuf[mUartRecvIndex].Len++;	
			}
			else
			{
				mUartRecvBuf[mUartRecvIndex].Buf[mUartRecvBuf[mUartRecvIndex].Len] = ReByte;
				mUartRecvBuf[mUartRecvIndex].Len += 1;
				mUartNeedRecv--;		
				if(mUartNeedRecv == 0)
				{
					mUartRecvBuf[mUartRecvIndex].State = FRAMEBUF_OK;	
					UartSwitchBuf();
					mUartStartRecv = UART_RECV_STOP;
					mUartRecvTick = 0;
				}
			}
		}
	}
}

/*************************************************
  Function:		UartInterruptReceiveCom2
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void UartInterruptReceiveCom2(void) interrupt 8 using 1
{
	INT8U ReByte;
	if(S2CON&S2R1)
	{
		S2CON &= ~S2R1;
		ReByte = S2BUF;
		if (mUartStartRecv == UART_RECV_STOP)
		{
			if(ReByte == 0xAA && mUartRecvBuf[mUartRecvIndex].State == FRAMEBUF_NONE)
			{
				mUartStartRecv = UART_RECV_HEAD;
				mUartRecvBuf[mUartRecvIndex].State = FRAMEBUF_RECV;
				mUartRecvBuf[mUartRecvIndex].Len = 0;
				mUartRecvBuf[mUartRecvIndex].Buf[mUartRecvBuf[mUartRecvIndex].Len] = 0xAA;
				mUartRecvBuf[mUartRecvIndex].Len++;
				mUartNeedRecv = 5;
				mUartRecvTick = 1;
			}
		}
		else
		{
			if (mUartStartRecv == UART_RECV_HEAD)
			{
				mUartRecvBuf[mUartRecvIndex].Buf[mUartRecvBuf[mUartRecvIndex].Len] = ReByte;
				mUartNeedRecv--;
				if(mUartNeedRecv == 0)
				{
					mUartNeedRecv = mUartRecvBuf[mUartRecvIndex].Buf[mUartRecvBuf[mUartRecvIndex].Len];
					mUartStartRecv = UART_RECV_DATA;
				}	
				mUartRecvBuf[mUartRecvIndex].Len++;	
			}
			else
			{
				mUartRecvBuf[mUartRecvIndex].Buf[mUartRecvBuf[mUartRecvIndex].Len] = ReByte;
				mUartRecvBuf[mUartRecvIndex].Len += 1;
				mUartNeedRecv--;		
				if(mUartNeedRecv == 0)
				{
					mUartRecvBuf[mUartRecvIndex].State = FRAMEBUF_OK;	
					UartSwitchBuf();
					mUartStartRecv = UART_RECV_STOP;
					mUartRecvTick = 0;
				}
			}
		}
	}
}


/*************************************************
  Function:		UartGetFrame
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
PUART_FRAME UartGetFrame(void)
{
	int i, ret = -1;
	for (i = 0; i < UART_RECV_MAX; i++)
	{
		if(mUartRecvBuf[i].State == FRAMEBUF_OK)
		{
			ret = i;
			break;
		}
	}

	if(-1 == ret)
	{
		return NULL;
	}
	else
	{
		return &mUartRecvBuf[ret];
	}
}

/*************************************************
  Function:		SendFrameNeedAck
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
static void SendFrameComm(INT8U Cmd, INT8U Send,INT8U NeedAck, INT8U Id, INT8U *Data, INT8U Len)
{
	INT8U i,CRC = 0x00;
	mUartSendBuf.Buf[FTRAME_INDEX_START] = FRAME_START;
	mUartSendBuf.Buf[FTRAME_INDEX_VERSION] = LogicGetFrameVersion();
	CRC += mUartSendBuf.Buf[FTRAME_INDEX_VERSION];
	mUartSendBuf.Buf[FTRAME_INDEX_CTRL] = Send+NeedAck;	
	CRC += mUartSendBuf.Buf[FTRAME_INDEX_CTRL];
	mUartSendBuf.Buf[FTRAME_INDEX_DST] = LogicGetMasterCtl();	
	CRC += mUartSendBuf.Buf[FTRAME_INDEX_DST];
	mUartSendBuf.Buf[FTRAME_INDEX_SRC] = FRAME_ADDR_RFID;	
	CRC += mUartSendBuf.Buf[FTRAME_INDEX_SRC];
	mUartSendBuf.Buf[FTRAME_INDEX_LEN] = Len+3;	
	CRC += mUartSendBuf.Buf[FTRAME_INDEX_LEN];
	mUartSendBuf.Buf[FTRAME_INDEX_ID] = Id;	
	CRC += mUartSendBuf.Buf[FTRAME_INDEX_ID];
	mUartSendBuf.Buf[FTRAME_INDEX_CMD] = Cmd;
	CRC += mUartSendBuf.Buf[FTRAME_INDEX_CMD];
	
	for (i = 0; i < Len; i++)
	{
		mUartSendBuf.Buf[FTRAME_INDEX_DATA+i] = Data[i];
		CRC += mUartSendBuf.Buf[FTRAME_INDEX_DATA+i];
	}

	mUartSendBuf.Buf[FTRAME_INDEX_DATA+Len] = CRC;
	mUartSendBuf.Len = 9+Len;
	mUartSendBuf.State = FRAMEBUF_OK;
}

/*************************************************
  Function:		SendCommandAck
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void SendCommandAck(INT8U Cmd, INT8U *Data, INT8U Len)
{
	mUartID++;
	SendFrameComm(Cmd, FRAME_SEND, FRAME_NEED_ACK, mUartID, Data, Len);
}

/*************************************************
  Function:		SendCommand
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void SendCommand(INT8U Cmd, INT8U *Data, INT8U Len)
{
	mUartID++;
	SendFrameComm(Cmd, FRAME_SEND, FRAME_NONEED_ACK, mUartID, Data, Len);
}

/*************************************************
  Function:		AckCommand
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void AckCommand(INT8U Cmd, INT8U Id, INT8U *Data, INT8U Len)
{
	SendFrameComm(Cmd, FRAME_ACK, FRAME_NONEED_ACK, Id, Data, Len);
}

/*************************************************
  Function:		AckCommandDirect
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void AckCommandDirect(INT8U Cmd, INT8U Id, INT8U *Data, INT8U Len)
{
	SendFrameComm(Cmd, FRAME_ACK, FRAME_NONEED_ACK, Id, Data, Len);
	UartSend(mUartSendBuf.Buf, mUartSendBuf.Len);
	mUartSendBuf.State = FRAMEBUF_NONE;
	delay_ms(10);
}

/*************************************************
  Function:		UartSendByte
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void UartSendByte(INT8U SendData)
{
  	TI = 0;
  	SBUF = SendData;
  	while( TI == 0 );
  	TI = 0;
}

/*************************************************
  Function:		UartSend
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void UartSend(INT8U* Data, INT8U Len)
{
	INT8U i;
  	for (i = 0; i < Len; i++)
  	{
  		UartSendByte(Data[i]);
  	}
}

/*************************************************
  Function:		UartProc
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void UartProc(void)
{
	if(mUartSendBuf.State == FRAMEBUF_OK)
	{
		UartSend(mUartSendBuf.Buf, mUartSendBuf.Len);
		mUartSendBuf.State = FRAMEBUF_NONE;
	}

	if(UART_LOG_NONE != mUartLogID)
	{
		switch (mUartLogID)
		{
			case UART_LOG_ERROR:
				#if (_LCD_MODULE_ != _LCD_NONE_)
				LCDLogD(LOG_UART_ERROR, SIZEOF(LOG_UART_ERROR), TEXT_LEFT);	
				#endif
				break;

			case UART_LOG_TIMEOUT:
				#if (_LCD_MODULE_ != _LCD_NONE_)
				LCDLogD(LOG_UART_TIMEOUT, SIZEOF(LOG_UART_TIMEOUT), TEXT_LEFT);	
				#endif
				break;

			default:
				break;
		}
		mUartLogID = UART_LOG_NONE;
	}
}

/*************************************************
  Function:		UartTimer
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void UartTimer(void)
{
	if(mUartStartRecv != UART_RECV_STOP)
	{
		if(mUartRecvTick > 0)
		{
			mUartRecvTick++;
			if(mUartRecvTick > _UART_RECV_OVERTIME_)
			{
				mUartStartRecv = UART_RECV_STOP;
				mUartRecvBuf[mUartRecvIndex].State = FRAMEBUF_NONE;
				mUartRecvTick = 0;
				mUartLogID = UART_LOG_TIMEOUT;
			}
		}
	}
}

/*************************************************
  Function:		UartInit
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void UartInit(void)
{
	INT8U i;
	
	mUartSendBuf.State = FRAMEBUF_NONE;
	for (i = 0; i < UART_RECV_MAX; i++)
	{
		mUartRecvBuf[i].State = FRAMEBUF_NONE;
	}

	#if(_SYS_TYPE ==_STC_11F32XE_)
  	SCON = 0x50;
	TMOD &= 0x0F;		
	TMOD |= 0x20;			

  	ES=1;
	TH1 = 256 - CHIP_CLK/ 12 / 32 / UART_BAUD;
	TL1 = 256 - CHIP_CLK / 12 / 32 / UART_BAUD;
	TR1 = 1;
	#endif

	#if (_SYS_TYPE == _STC_15L2K60S2_)
	ACC = P_SW1;
    ACC &= ~(S1_S0 | S1_S1);   
    P_SW1 = ACC;              
    SCON = 0x50;                
    AUXR = 0x40;             
    TMOD = 0x00;               
    TL1 = (65536 - (CHIP_CLK/4/UART_BAUD));   
    TH1 = (65536 - (CHIP_CLK/4/UART_BAUD))>>8;
    TR1 = 1;                   
    ES = 1;           

  //  S2CON = 0x50;    
  //  T2L = (65536 - (CHIP_CLK/4/UART_BAUD)); 
   // T2H = (65536 - (CHIP_CLK/4/UART_BAUD))>>8;
   // AUXR |= 0x14;    
   // IE2 = 0x01;            
	#endif
}

