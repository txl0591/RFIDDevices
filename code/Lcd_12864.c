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

#define _LCD_DELAY_		50

#if (_LCD_MODULE_ == _LCD_12864B_)
/*************************************************
  Function:		LCDWrite
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
static void LCDWrite(INT8U Data) 
{
	LCD_D0 = Data&0x01;
	LCD_D1 = (Data&0x02>>1);
	LCD_D2 = (Data&0x04>>2);
	LCD_D3 = (Data&0x08>>3);
	LCD_D4 = (Data&0x10>>4);
	LCD_D5 = (Data&0x20>>5);
	LCD_D6 = (Data&0x40>>6);
	LCD_D7 = (Data&0x80>>7);
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
	RS = 1;
	delay_ns(_LCD_DELAY_);
	RW = 0;
	delay_ns(_LCD_DELAY_);
	EN = 1;
	delay_ns(_LCD_DELAY_);
	LCDWrite(Data);
	EN = 0;	
	delay_ns(_LCD_DELAY_);  
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
	RS = 0;
	delay_ns(_LCD_DELAY_);
	RW = 0;
	EN = 1;
	delay_ns(_LCD_DELAY_);
	LCDWrite(Cmd);	
	EN = 0;
	delay_ns(_LCD_DELAY_);
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
	INT8U Cmd = 0;
	switch (Ypos)
	{
		case 0:
			Cmd = 0x80+Xpos;
			break;

		case 1:
			Cmd = 0x90+Xpos;
			break;
			
		case 2:
			Cmd = 0x88+Xpos;
			break;
		
		case 3:
			Cmd = 0x98+Xpos;
			break;	

		default:
			Cmd = 0x80+Xpos;
			break;
	}

	LCDWriteCommand(Cmd);
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
	for(ii = 0;ii < 4; ii++)
	{ 
		for(jj = 0; jj < 8; jj++)
		{
			LCDWriteOneChar(jj, ii,' ');
		}
	}
}


/*************************************************
  Function:		LCD12864Init
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void LCD12864Init(void) 
{
	RST=0;
	delay_ns(_LCD_DELAY_);
	RST=1;
	delay_ns(_LCD_DELAY_);
	LCDWriteCommand(0x30); //功能设置 8位数据，基本指令
	LCDWriteCommand(0x01); //清除显示
	LCDWriteCommand(0x02); //地址归位
	LCDWriteCommand(0x80); //设置DDRAM地址  
	LCDWriteCommand(0x0C); //显示状态 ON，游标OFF，反白OFF
}
#endif

