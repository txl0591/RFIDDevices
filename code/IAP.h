/*********************************************************
  Copyright (C), 2013-2016
  File name:	IAP.h
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
#ifndef _IAP_H_
#define _IAP_H_

#include "Storage.h"

#define _ISP_IAP_CMD_IDLE_			0x00
#define _ISP_IAP_CMD_READ_			0x01
#define _ISP_IAP_CMD_WRITE_			0x02
#define _ISP_IAP_CMD_ERASE_			0x03

#define ENABLE_IAP					0x82

#if(STORAGE_TYPE == _SOTRAGE_IAP_)	
/*************************************************
  Function:		IAPIdle
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void IAPIdle(void);

/*************************************************
  Function:		IAPReadByte
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
INT8U IAPReadByte(INT16U addr);

/*************************************************
  Function:		IAPWriteByte
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void IAPWriteByte(INT16U addr, INT8U buf);

#endif

#endif

