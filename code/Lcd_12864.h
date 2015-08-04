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

#if (_LCD_MODULE_ == _LCD_12864B_)

sbit LCD_D0 = P2^3;
sbit LCD_D1 = P2^2;
sbit LCD_D2 = P2^1;
sbit LCD_D3 = P2^0;
sbit LCD_D4 = P3^7;
sbit LCD_D5 = P3^6;
sbit LCD_D6 = P3^5;
sbit LCD_D7 = P3^4;

sbit RST= P3^3;
sbit RS = P2^6;   
sbit RW = P2^5;     
sbit EN = P2^4; 

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
  Function:		LCD12864Init
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void LCD12864Init(void); 

#endif
#endif

