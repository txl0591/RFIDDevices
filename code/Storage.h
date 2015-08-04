/*********************************************************
  Copyright (C), 2013-2016
  File name:	Storage.h
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
#ifndef  __STORAGE_H__
#define  __STORAGE_H__

#include "Config.h"

#define _SOTRAGE_NONE_		0x00
#define _SOTRAGE_CODE_		0x01
#define _SOTRAGE_IAP_		0x02

#if _STORAGE_MODULE_
#if (_SYS_TYPE == _STC_11F32XE_)
#define STORAGE_TYPE 		_SOTRAGE_CODE_
#endif

#if (_SYS_TYPE == _STC_15L2K60S2_)
#define STORAGE_TYPE 		_SOTRAGE_IAP_
#endif

#else
#define STORAGE_TYPE 		_SOTRAGE_NONE_
#endif

/**********************************
BLK0   /-0-/-1-/..15-/
/ «∑ÒŒ™≥ı º◊¥Ã¨/–≠“È∞Ê±æ/√‹¬Î∞Ê±æ/
BLK1   /-0..15-/
/±£¡Ù/
BLK2   /-0..5-/-6..9-/-10..15-/
/√‹¬ÎA1/øÿ÷∆◊÷/√‹¬ÎB1/
...
BLK18   /-0..5-/-6..9-/-10..15-/
/√‹¬ÎA16/øÿ÷∆◊÷/√‹¬ÎB16/
**********************************/

#define ADDRESS_INIT			0x0000
#define ADDRESS_UART_VERSION	0x0001
#define ADDRESS_PWD_VERSION		0x0002
#define ADDRESS_PASSWD(i)		(0x0020+(i*16))
#define ADDRESS_CHECK			0x0120
#if _STORAGE_MODULE_
/*************************************************
  Function:		StorageRead
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
INT8U StorageReadByte(INT16U addr);

/*************************************************
  Function:		StorageWrite
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void StorageWriteByte(INT16U addr, INT8U buf);

/*************************************************
  Function:		StorageRead
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
INT8U StorageRead(INT16U addr, INT8U *buf, INT16U len);

/*************************************************
  Function:		StorageWrite
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
INT16U StorageWrite(INT16U addr, INT8U *buf, INT16U len);

#endif
/*************************************************
  Function:		StorageRecover
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void StorageRecover(void);

/*************************************************
  Function:		StorageInit
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void StorageInit(void);

#endif

