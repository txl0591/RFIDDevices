/*********************************************************
  Copyright (C), 2013-2016
  File name:	Hardware.h
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
#ifndef  __HARDWARE_H__
#define  __HARDWARE_H__

#include "config.h"
#include "Spi.h"
#include "ReadRFID.h"
#include "Timer.h"
#include "Uart.h"
#include "Lcd.h"
#include "IAP.h"

#define CHIP_CLK_18432000 18432000L
#define CHIP_CLK_22118400 22118400L

#define	UART_BAUD	115200

#if(_SYS_TYPE ==_STC_11F32XE_)
sbit BEEP = P3^4; 
#define	CHIP_CLK	CHIP_CLK_18432000
#define BEEP_ON		1
#define BEEP_OFF	0
#endif

#if (_SYS_TYPE == _STC_15L2K60S2_)
sbit BEEP = P2^7; 
#define	CHIP_CLK	CHIP_CLK_22118400
#define BEEP_ON		1
#define BEEP_OFF	0
#endif

#define _HW_BEEP_COUNT_		256

#define _HW_BEEP_OK_		0x01			
#define _HW_BEEP_ERR_		0x02

/*************************************************
  Function:		BeepOK
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void SetBeepIndex(INT8U Beep);

/*************************************************
  Function:		HarewareProc
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void HarewareTimer(void);	

/*************************************************
  Function:		HarewareInit
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void HarewareInit(void);

#endif

