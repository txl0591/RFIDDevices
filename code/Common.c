/*********************************************************
  Copyright (C), 2013-2016
  File name:	common.c
  Author:   	txl
  Version:   	1.0
  Date: 		13.11.23
  Description:  
  History:            
*********************************************************/
#include "Include.h"

/*************************************************
  Function:		delay_ns
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void delay_ns(unsigned int ns)
{
	unsigned int i,k;
	for(i = 0; i < ns;i++)
	{
		k = 3;
		while (--k);
	}
}

/*************************************************
  Function:		delay_ms
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void delay_ms(INT16U tms)
{
	unsigned char i, j;
	#if (_SYS_TYPE == _STC_11F32XE_)
	while( tms-- )
	{
		i = 18;
		j = 235;
		do
		{
			while (--j);
		} while (--i);
  	}
	#endif
	
	#if (_SYS_TYPE == _STC_15L2K60S2_)
	while(tms--)
	{
		nop();
		nop();
		i = 22;
		j = 128;
		do
		{
			while (--j);
		} while (--i);
	}
	#endif
}

