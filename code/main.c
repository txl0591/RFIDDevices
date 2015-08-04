/*********************************************************
  Copyright (C), 2013-2016
  File name:	Main.c
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

/*************************************************
  Function:		Main
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void main(void)
{
	LogicInit();
	while(1)
	{
		LogicProc();
		UartProc();
	}
}

