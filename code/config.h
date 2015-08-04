/*********************************************************
  Copyright (C), 2013-2016
  File name:	Config.h
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
#ifndef  __CONFIG_H__
#define  __CONFIG_H__

#define _STC_11F32XE_			1
#define _STC_15L2K60S2_			2

#define _SYS_TYPE				_STC_15L2K60S2_

#define _LCD_NONE_				0
#define _LCD_1602_				1
#define _LCD_12864B_			2

#if (_SYS_TYPE == _STC_11F32XE_)
#define _LCD_MODULE_			_LCD_1602_
#define _UART_MODULE_			1
#define _RFID_MODULE_			1
#define _UART_DATA_ENY_			0
#define _STORAGE_MODULE_		1
#endif

#if (_SYS_TYPE == _STC_15L2K60S2_)
#define _LCD_MODULE_			_LCD_NONE_
#define _UART_MODULE_			1
#define _RFID_MODULE_			1
#define _UART_DATA_ENY_			0
#define _STORAGE_MODULE_		1
#endif


#if (_LCD_MODULE_ == _LCD_1602_)
#define LCD_W	16
#define LCD_H	2
#endif

#if (_LCD_MODULE_ == _LCD_12864B_)
#define LCD_W	16
#define LCD_H	4
#endif

#if (_LCD_MODULE_ != _LCD_NONE_)
#define _LCD_LOG_				1
#else
#define _LCD_LOG_				0
#endif

#define _VERSION_				"RFID V1.0"
#define _HW_VERSION_			0x10
#define _SW_VERSION_			0x10
#define UARTFRAME_VERSION		0x01

#endif

