/*********************************************************
  Copyright (C), 2013-2016
  File name:	Common.h
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
#ifndef _COMMON_H_
#define _COMMON_H_

#define nop() _nop_()

#define SIZEOF(t)  (sizeof(t)-1)

/*************************************************
  Function:		delay_ns
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void delay_ns(unsigned int ns);

/*************************************************
  Function:		delay_ms
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void delay_ms(INT16U tms);

#endif


