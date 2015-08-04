/*********************************************************
  Copyright (C), 2013-2016
  File name:	Uart.h
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
#ifndef _UART_H_
#define _UART_H_

#define S1_S0 						0x40              //P_SW1.6
#define S1_S1 						0x80              //P_SW1.7

#define UART_RECV_STOP				0
#define UART_RECV_HEAD				1
#define UART_RECV_DATA				2

#define UART_FRAME_MAX				70
#define UART_RECV_MAX				2

#define FRAME_MIN_SIZE				9
#define FRAME_START					0xAA

#define FRAME_ADDR_NONE				0xFF
#define FRAME_ADDR_RFID				0x00
#define FRAME_ADDR_PC				0x01
#define FRAME_ADDR_ANDROID			0x02

#define FRAME_SEND					0x00
#define FRAME_ACK					0x80

#define FRAME_NEED_ACK				0x00
#define FRAME_NONEED_ACK			0x01

#define FRAMEBUF_OK					0x02
#define FRAMEBUF_RECV				0x01
#define FRAMEBUF_NONE				0x00

#define FTRAME_INDEX_START 			0x00
#define FTRAME_INDEX_VERSION		0x01
#define FTRAME_INDEX_CTRL 			0x02
#define FTRAME_INDEX_DST 			0x03
#define FTRAME_INDEX_SRC 			0x04
#define FTRAME_INDEX_LEN 			0x05
#define FTRAME_INDEX_ID 			0x06
#define FTRAME_INDEX_CMD 			0x07
#define FTRAME_INDEX_DATA			0x08

#define UART_LOG_NONE				0x00
#define UART_LOG_ERROR				0x01   //接收错误
#define UART_LOG_TIMEOUT			0x02   //接收超时

#define S2R1						0x01
#define S2T1						0x02

typedef struct 
{
	INT8U State;
	INT8U Len;
	INT8U Buf[UART_FRAME_MAX];
}UART_FRAME,*PUART_FRAME;

/*************************************************
  Function:		SendCommandAck
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void SendCommandAck(INT8U Cmd, INT8U *Data, INT8U Len);

/*************************************************
  Function:		SendCommand
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void SendCommand(INT8U Cmd, INT8U *Data, INT8U Len);

/*************************************************
  Function:		AckCommand
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void AckCommand(INT8U Cmd, INT8U Id, INT8U *Data, INT8U Len);

/*************************************************
  Function:		AckCommandDirect
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void AckCommandDirect(INT8U Cmd, INT8U Id, INT8U *Data, INT8U Len);

/*************************************************
  Function:		UartSetDstDevice
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void UartSetDstDevice(INT8U Device);

/*************************************************
  Function:		UartSendByte
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void UartSendByte(INT8U SendData);

/*************************************************
  Function:		UartSend
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void UartSend(INT8U* Data, INT8U Len);

/*************************************************
  Function:		UartGetFrame
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
PUART_FRAME UartGetFrame(void);

/*************************************************
  Function:		UartProc
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void UartProc(void);

/*************************************************
  Function:		UartTimer
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void UartTimer(void);

/*************************************************
  Function:		UartInit
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void UartInit(void);


#endif
