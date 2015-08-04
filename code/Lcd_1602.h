/*********************************************************
  Copyright (C), 2013-2016
  File name:	Lcd_1602.h
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
#ifndef  __LCD_1602_H__
#define  __LCD_1602_H__

#include "Config.h"
#include "Type.h"

#if (_LCD_MODULE_ == _LCD_1602_)

#define LCM_Data P1
sbit LCM_RW = P3^2; 
sbit LCM_RS = P3^3;
sbit LCM_E  = P4^3;

/*************************************************
  Function:		LCDClearLine
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void LCDClearLine(INT8U Xpos, INT8U Ypos, INT8U len);

/*************************************************
  Function:		LCDClearScreen
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void LCDClearScreen(void);

/*************************************************
  Function:		LCDWriteOneChar
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void LCDWriteOneChar(INT8U Xpos, INT8U Ypos, INT8U DData);

/*************************************************
  Function:		LCDWriteLineChar
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void LCDWriteLineChar(INT8U Xpos, INT8U Ypos, INT8U *DData, INT8U Len);

/*************************************************
  Function:		LCD1602Init
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void LCD1602Init(void); 
#endif

#endif
