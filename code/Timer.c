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
#include "Timer.h"
#include "Hardware.h"

static INT32U mTimer[_TIMER_MAX_];
static INT32U mTimerMax[_TIMER_MAX_];
static INT8U ntime = 0; 
/*************************************************
  Function:		Timer0_Interrupt
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void Timer0_Interrupt(void) interrupt 1
{
	INT8U i;
	TL0 = TIMER_TICK_L;
	TH0 = TIMER_TICK_H;
	for (i = 0; i < _TIMER_MAX_; i++)
	{	
		mTimer[i]++;
		
		if(mTimer[i] >= mTimerMax[i])
		{
			mTimer[i] = 0;
			switch(i)
			{
				case _TIMER_UART_:		
					UartTimer();
					break;

				case _TIMER_HARDWARE_:
					HarewareTimer();
					break;

				default:
					break;
			}
		}
	}
}

/*************************************************
  Function:		TimerStart
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void TimerStart(void)  
{
	TL0 = TIMER_TICK_L;
	TH0 = TIMER_TICK_H;
	ET0 = 1;
	TR0 = 1;	
}

/*************************************************
  Function:		TimerStop
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void TimerStop(void)  
{
	TR0 = 0;			
}

/*************************************************
  Function:		TimerSetOverTime
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void TimerSetOverTime(INT8U Type, INT32U OverTime)  
{
	if(Type < _TIMER_MAX_)
	{
		mTimerMax[Type] = OverTime;
	}
}

/*************************************************
  Function:		TimerInit
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void TimerInit(void)  
{
	int i;

	for(i = 0; i < _TIMER_MAX_; i++)
	{
		mTimer[i] = 0;
	}
	#if(_SYS_TYPE ==_STC_11F32XE_)
	TMOD &= 0xF0;	
	TMOD |= 0x01;	
	#endif

	#if(_SYS_TYPE == _STC_15L2K60S2_)
	AUXR |= 0x80;		
	TMOD &= 0xF0;		
	#endif
	
	TimerSetOverTime(_TIMER_UART_, TIMER_MS(10));
	TimerSetOverTime(_TIMER_HARDWARE_, TIMER_US_500);
	
	TimerStop();
}


