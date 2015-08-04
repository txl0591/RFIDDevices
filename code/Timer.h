/*********************************************************
  Copyright (C), 2013-2016
  File name:	Timer.h
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
#ifndef _TIMER_H_
#define _TIMER_H_

#include "Config.h"

#define TIMER_50US			1
#define TIMER_100US			2
#define TIMER_200US			3
#define TIMER_500US			4

#define TIMER_TYPE			TIMER_50US

#if(TIMER_TYPE == TIMER_50US)
#if(_SYS_TYPE ==_STC_11F32XE_)
#define TIMER_TICK_H	 	0xFF
#define TIMER_TICK_L	 	0xB3
#endif
#if (_SYS_TYPE == _STC_15L2K60S2_)
#define TIMER_TICK_H	 	0xFB
#define TIMER_TICK_L	 	0xAE
#endif
#define DEFAULT_TIMER_MAX	22
#endif

#if(TIMER_TYPE == TIMER_100US)
#if(_SYS_TYPE ==_STC_11F32XE_)
#define TIMER_TICK_H	 	0xFF
#define TIMER_TICK_L	 	0x66 
#endif
#if (_SYS_TYPE == _STC_15L2K60S2_)
#define TIMER_TICK_H	 	0xF7
#define TIMER_TICK_L	 	0x5C
#endif
#define DEFAULT_TIMER_MAX	11
#endif

#if(TIMER_TYPE == TIMER_200US)
#if(_SYS_TYPE ==_STC_11F32XE_)
#define TIMER_TICK_H	 	0xFE
#define TIMER_TICK_L	 	0xCD
#endif
#if (_SYS_TYPE == _STC_15L2K60S2_)
#define TIMER_TICK_H	 	0xEE
#define TIMER_TICK_L	 	0xB8
#endif
#define DEFAULT_TIMER_MAX	5
#endif

#if(TIMER_TYPE == TIMER_500US)
#if(_SYS_TYPE ==_STC_11F32XE_)
#define TIMER_TICK_H	 	0xFD
#define TIMER_TICK_L	 	0x00
#endif
#if (_SYS_TYPE == _STC_15L2K60S2_)
#define TIMER_TICK_H	 	0xD4
#define TIMER_TICK_L	 	0xCD
#endif
#define DEFAULT_TIMER_MAX	2
#endif

#define _TIMER_UART_		0x00	
#define _TIMER_HARDWARE_	0x01
#define _TIMER_MAX_			0x02

#define TIMER_US_500		10
#define TIMER_MS(i)			(DEFAULT_TIMER_MAX*i)
#define TIMER_S(i)			(1000*DEFAULT_TIMER_MAX*i)

/*************************************************
  Function:		TimerStart
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void TimerStart(void); 

/*************************************************
  Function:		TimerStop
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void TimerStop(void);

/*************************************************
  Function:		TimerInit
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void TimerInit(void);

#endif
