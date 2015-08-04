/*********************************************************
  Copyright (C), 2013-2016
  File name:	IAP.c
  Author:   	
  Version:   	1.0
  Date: 		13.11.23
  Description:  
  History:            
*********************************************************/
#include "Include.h"

#if (STORAGE_TYPE == _SOTRAGE_IAP_)
/*************************************************
  Function:		IAPIdle
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void IAPIdle(void)
{
	IAP_CONTR = 0;
	IAP_CMD = 0;	
	IAP_TRIG = 0;	
	IAP_ADDRH = 0x80;		
	IAP_ADDRL = 0;		
}

/*************************************************
  Function:		IAPReadByte
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
INT8U IAPReadByte(INT16U addr)
{
	INT8U buf = 0x00;
	
	IAP_CONTR = ENABLE_IAP;
	IAP_CMD = _ISP_IAP_CMD_READ_;
	IAP_ADDRH = addr>>8;		
	IAP_ADDRL = addr;
	IAP_TRIG = 0x5a;
	IAP_TRIG = 0xa5;
	delay_ns(5);
	buf = IAP_DATA;
	IAPIdle();
	return buf;
}

/*************************************************
  Function:		IAPWriteByte
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void IAPWriteByte(INT16U addr, INT8U buf)
{
	IAP_CONTR = ENABLE_IAP;
	IAP_CMD = _ISP_IAP_CMD_WRITE_;
	IAP_ADDRH = addr>>8;		
	IAP_ADDRL = addr;
	IAP_DATA = buf;
	IAP_TRIG = 0x5a;
	IAP_TRIG = 0xa5;
	delay_ns(5);
	IAPIdle();
}
#endif

