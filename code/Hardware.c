/*********************************************************
  Copyright (C), 2013-2016
  File name:	Hardware.c
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

static INT8U mBeepIndex = 0;
static INT16U mBeepCount = 0;

/*************************************************
  Function:		PortInit
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
static void PortInit(void)
{
	#if(_SYS_TYPE ==_STC_11F32XE_)
	P0M1 = 0x00;
	P0M0 = 0xff;

	P3M1 &= ~0x10;
	P3M0 |= 0x10;

	P34 = 0;
	P35 = 0;
	#endif

	#if(_SYS_TYPE ==_STC_15L2K60S2_)
	P2M1 = 0x00;
	P2M0 = 0x80;
	#endif
}

/*************************************************
  Function:		BeepOK
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
static void BeepCtl(INT8U Beep)
{
	if(Beep%2 != 0)
	{
		BEEP = BEEP_ON;
	}
	else
	{
		BEEP = BEEP_OFF;
	}	
}

/*************************************************
  Function:		BeepOK
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void SetBeepIndex(INT8U Beep)
{
	mBeepIndex = Beep;
}

/*************************************************
  Function:		HarewareTimer
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void HarewareTimer(void)	
{	
	if(mBeepCount > 0)
	{
		mBeepCount--;
		BeepCtl(mBeepCount);
	}
	else
	{
		if(mBeepIndex > 0)
		{
			mBeepIndex--;
			mBeepCount = _HW_BEEP_COUNT_;
		}
	}
}

/*************************************************
  Function:		HarewareInit
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void HarewareInit(void)
{
	EA = 0;
	PortInit();
	UartInit();
	TimerInit();
	RFIDInit();
	LCDInit();
	EA = 1;
	TimerStart();
}

