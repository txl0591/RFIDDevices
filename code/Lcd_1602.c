/*********************************************************
  Copyright (C), 2013-2016
  File name:	Lcd_1602.c
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

#define _LCD_DELAY_		1500

#if (_LCD_MODULE_ == _LCD_1602_)
/*************************************************
  Function:		LCDInit
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
static INT8U swapbit(INT8U udata)
{
	INT8U ii,tmp=0;
	for(ii = 0; ii < 8; ii++)
	{
		tmp <<= 1;
		if(udata & 0x01)
		{
		  tmp |= 0x01;
		}
		udata >>= 1;
	}
	return tmp;
}

/*************************************************
  Function:		LCDWriteData
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
static void LCDWriteData(INT8U Data)
{
	delay_ns(_LCD_DELAY_);
	LCM_Data = swapbit(Data);
	LCM_RS = 1;
	LCM_RW = 0;
	LCM_E = 0; 
	LCM_E = 0; 
	delay_ns(_LCD_DELAY_);
	LCM_E = 1;
}

/*************************************************
  Function:		LCDWriteCommand
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
static void LCDWriteCommand(INT8U Cmd) 
{
	delay_ns(_LCD_DELAY_);
	LCM_Data = swapbit(Cmd);
	LCM_RS = 0;
	LCM_RW = 0; 
	LCM_E = 0;
	LCM_E = 0;
	LCM_E = 1; 
}

/*************************************************
  Function:		LCDClearLine
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void LCDClearLine(INT8U Xpos, INT8U Ypos, INT8U len)
{
	INT8U jj;
	for(jj = Xpos; jj < (Xpos+len); jj++)
	{
		LCDWriteOneChar(jj, Ypos,' ');
	}
}

/*************************************************
  Function:		LCDClearScreen
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void LCDClearScreen(void)
{
	INT8U ii,jj;
	for(ii = 0;ii < 2; ii++)
	{ 
		for(jj = 0; jj < 16; jj++)
		{
			LCDWriteOneChar(jj, ii,' ');
		}
	}
}

/*************************************************
  Function:		LCDWriteOneChar
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void LCDWriteOneChar(INT8U Xpos, INT8U Ypos, INT8U DData)
{
	INT8U X = Xpos;
	INT8U Y = Ypos;
	
	Y &= 0x1;
	X &= 0xF; 
	if (Y) X |= 0x40; 
	X |= 0x80; 
	LCDWriteCommand(X); 
	LCDWriteData(DData);
}

/*************************************************
  Function:		LCDWriteLineChar
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void LCDWriteLineChar(INT8U Xpos, INT8U Ypos, INT8U *DData, INT8U Len)
{
	INT8U i;
	INT8U X = Xpos;
	INT8U Y = Ypos;
	INT8U ListLength = 0;
	Y &= 0x1;
	X &= 0xF; 

	for(i = 0; i < Len; i++)
	{
		if (X <= 0xF) 
		{
			LCDWriteOneChar(X, Y, *DData); 
			DData++;
			X++;
		}
	}
}

/*************************************************
  Function:		LCD1602Init
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void LCD1602Init(void) 
{
	LCM_Data = 0;
	LCDWriteCommand(0x38);
	delay_ns(_LCD_DELAY_);
	LCDWriteCommand(0x38);
	delay_ns(_LCD_DELAY_); 
	LCDWriteCommand(0x38);
	delay_ns(_LCD_DELAY_); 
	LCDWriteCommand(0x38); // 显示模式设置,开始要求每次检测忙信号
	LCDWriteCommand(0x08); // 关闭显示
	LCDWriteCommand(0x01); // 显示清屏
	LCDWriteCommand(0x06); // 显示光标移动设置
	LCDWriteCommand(0x0C); // 显示开及光标设置
}

#endif

