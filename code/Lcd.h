/*********************************************************
  Copyright (C), 2013-2016
  File name:	Lcd.h
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
#ifndef  __LCD_H__
#define  __LCD_H__

#include "Config.h"
#if (_LCD_MODULE_ == _LCD_1602_)
#include "Lcd_1602.h"
#endif
#if (_LCD_MODULE_ == _LCD_12864B_)
#include "lcd_12864.h"
#endif
#include "Type.h"

#define TEXT_LEFT			0
#define TEXT_RIGHT			1
#define TEXT_CENTER			2

#define LOG_UART_ERROR			"RECV ERROR"
#define LOG_UART_TIMEOUT		"RECV TIMEOUT"
#define LOG_DATA_SIZE			"DATA SIZE"
#define LOG_DATA_ADDR			"DATA ADDR"
#if (_LCD_MODULE_ != _LCD_NONE_)
/*************************************************
  Function:		LCDLog
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void LCDLog(INT8U* ndata, INT8U nlen, INT8U Mode);

/*************************************************
  Function:		LCDLogD
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void LCDLogD(INT8U* ndata, INT8U nlen, INT8U Mode);

/*************************************************
  Function:		LCDLogECHO
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void LCDLogECHO(INT8U ndata, INT8U Mode);
#endif
/*************************************************
  Function:		LCDInit
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void LCDInit(void); 

#endif
