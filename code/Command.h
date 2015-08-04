/*********************************************************
  Copyright (C), 2013-2016
  File name:	Command.h
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
#ifndef _COMMAN_H_
#define _COMMAN_H_

typedef enum
{
	RFID_ANTENNA     		= 0x00,
	RFID_CLR_PASSWD  		= 0x11,	
	RFID_SET_PASSWD  		= 0x12,
	RFID_GET_PASSWD  		= 0x13,
	RFID_SET_CRCBLK   		= 0x14,
	RFID_GET_CRCBLK   		= 0x15,
		
	RFID_WRITE_BLOCK 		= 0x21,
	RFID_READ_BLOCK  		= 0x22,	
	RFID_WRITE_MULT_BLOCK 	= 0x23,
	RFID_READ_MULT_BLOCK 	= 0x24,
	RFID_READ_CARDID 		= 0x25,

	RFID_GET_VERSION 		= 0x30,
	RFID_POWER_SYNC  		= 0x31,
	RFID_BEEP		 		= 0x32,
	RFID_RECOVER_SYS 		= 0x33,	
}RFID_COMMAND;

typedef enum
{
	ECHO_OK 			= 0x00,
	ECHO_ERR 			= 0x01,		
	ECHO_ERR_SIZE 		= 0x02,
	ECHO_ERR_COLLISION  = 0x03,
	ECHO_ERR_PASSWD 	= 0x04,
	ECHO_ERR_WRITEDATA 	= 0x05,
	ECHO_ERR_READDATA 	= 0x06,
	ECHO_ERR_CHECK_CARD	= 0x07,
	ECHO_ERR_WRITEPWD 	= 0x08,
}ECHO_COMMAND;

typedef enum
{
	RFID_CARD_NONE		= 0x00,
	RFID_CARD_INIT		= 0x01,	
	RFID_CARD_USER		= 0x02,
	RFID_CARD_SYS		= 0x03,
	RFID_CARD_ALL		= 0x04,	
}RFID_CARD_TYPE;


#define RFID_BEEP_OK		0x00
#define RFID_BEEP_ERR		0x01

#define RFID_ANTENNA_ON		0x00
#define RFID_ANTENNA_OFF	0x01

#define READ_MULT_START		0x01
#define READ_MULT_CENTER	0x10
#define READ_MULT_END		0x11


#endif

