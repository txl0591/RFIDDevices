/*********************************************************
  Copyright (C), 2013-2016
  File name:	Lcd.c
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
#if (_LCD_MODULE_ != _LCD_NONE_)

#if _LCD_LOG_
static INT8U data mLcd[LCD_W]; 
static INT8U data mLcdline1Len = 0;
static INT8U data mLcdline2Len = 0;
static INT8U data mLcdline1Mode = 0;
static INT8U data mLcdline2Mode = 0;
static INT8U data mLogID = 0;
#endif

/*************************************************
  Function:		LCDClearLineMode
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
  #define TEXT_LEFT			0
  #define TEXT_RIGHT		1
  #define TEXT_CENTER		2
*************************************************/
static void LCDClearLineMode(INT8U Ypos, INT8U Len, INT8U Mode)
{
	INT8U X;

	switch(Mode)
	{
		case TEXT_LEFT:
			X = 0;
			break;

		case TEXT_RIGHT:
			X = (LCD_W-Len);			
			break;

		case TEXT_CENTER:
			X = (LCD_W-Len)/2;
			break;	

		default:
			X = 0;
			break;
	}
	LCDClearLine(X, Ypos, Len);
}

/*************************************************
  Function:		LCDWriteLineMode
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
  #define TEXT_LEFT			0
  #define TEXT_RIGHT		1
  #define TEXT_CENTER		2
*************************************************/
static void LCDWriteLineMode(INT8U Ypos, INT8U *DData, INT8U Len, INT8U Mode)
{
	INT8U X;

	switch(Mode)
	{
		case TEXT_LEFT:
			X = 0;
			break;

		case TEXT_RIGHT:
			X = (LCD_W-Len);			
			break;

		case TEXT_CENTER:
			X = (LCD_W-Len)/2;
			break;	

		default:
			X = 0;
			break;
	}

	LCDWriteLineChar(X, Ypos, DData, Len);
}
#endif
#if (_LCD_MODULE_ != _LCD_NONE_)
/*************************************************
  Function:		LCDLog
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void LCDLog(INT8U* ndata, INT8U nlen, INT8U Mode)
{
	#if _LCD_LOG_
	if(mLcdline1Len == 0)
	{
		memcpy(mLcd, ndata, nlen);
		mLcdline1Len = nlen;	
		mLcdline1Mode = Mode;
		LCDWriteLineMode(0, mLcd, mLcdline1Len, mLcdline1Mode);	
	}
	else
	{
		if(mLcdline2Len > 0)
		{	
			LCDClearLineMode(0, mLcdline1Len, mLcdline1Mode);
			LCDWriteLineMode(0, mLcd, mLcdline2Len, mLcdline2Mode);
			mLcdline1Mode = mLcdline2Mode;
			mLcdline1Len = mLcdline2Len;
		}

		memcpy(mLcd, ndata, nlen);
		mLcdline2Len = nlen;	
		mLcdline2Mode = Mode;
		LCDWriteLineMode(1, mLcd, mLcdline2Len, mLcdline2Mode);	
	}
	#endif
}

/*************************************************
  Function:		LCDLogD
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void LCDLogD(INT8U* ndata, INT8U nlen, INT8U Mode)
{
	#if _LCD_LOG_
	mLogID++;
	if(mLogID >= 10)
	{
		mLogID = 0;
	}
	if(mLcdline1Len == 0)
	{
		mLcd[0] = '0'+mLogID;
		mLcd[1] = ':';	
		memcpy(mLcd+2, ndata, nlen);
		mLcdline1Len = nlen+2;	
		mLcdline1Mode = Mode;
		LCDWriteLineMode(0, mLcd, mLcdline1Len, mLcdline1Mode);	
	}
	else
	{
		if(mLcdline2Len > 0)
		{
			LCDClearLineMode(0, mLcdline1Len, mLcdline1Mode);
			LCDWriteLineMode(0, mLcd, mLcdline2Len, mLcdline2Mode);
			mLcdline1Mode = mLcdline2Mode;
			mLcdline1Len = mLcdline2Len;
		}

		mLcd[0] = '0'+mLogID;
		mLcd[1] = ':';	
		memcpy(mLcd+2, ndata, nlen);
		mLcdline2Len = nlen+2;	
		mLcdline2Mode = Mode;
		LCDWriteLineMode(1, mLcd, mLcdline2Len, mLcdline2Mode);	
	}
	#endif
}

/*************************************************
  Function:		LCDLogECHO
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void LCDLogECHO(INT8U ndata, INT8U Mode)
{
	INT8U qdata[10];
	INT8U a = ndata>>4;
	INT8U b = ndata&0x0F;
	INT8U len = 9;
	qdata[0] = 'E';
	qdata[1] = 'C';
	qdata[2] = 'H';
	qdata[3] = 'O';	
	qdata[4] = ' ';	

	if(ECHO_OK == ndata)
	{
		qdata[5] = 'O';
		qdata[6] = 'K';
		len = 7;
	}
	else
	{
		qdata[5] = '0';
		qdata[6] = 'X';
		if(a > 9)
		{
			qdata[7] = 'A'+a-10;
		}
		else
		{
			qdata[7] = '0'+a;
		}
		if(b > 9)
		{
			qdata[8] = 'A'+b-10;
		}
		else
		{
			qdata[8] = '0'+b;
		}
	}
	LCDLogD(qdata, len, Mode);
}

/*************************************************
  Function:		LCDVersion
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void LCDVersion(void)
{
	LCDLog(_VERSION_, SIZEOF(_VERSION_), TEXT_CENTER);
}
#endif
/*************************************************
  Function:		LCDInit
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void LCDInit(void) 
{
	#if _LCD_LOG_
	mLcdline1Len = 0;
	#endif
	
	#if (_LCD_MODULE_ == _LCD_1602_)
	LCD1602Init();
	#endif

	#if (_LCD_MODULE_ == _LCD_12864B_)
	LCD12864Init();
	#endif
	
	#if (_LCD_MODULE_ != _LCD_NONE_)
	LCDClearScreen();
	LCDVersion();
	#endif
}



