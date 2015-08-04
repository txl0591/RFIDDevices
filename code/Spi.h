/*********************************************************
  Copyright (C), 2013-2016
  File name:	spi.h
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
#ifndef _SPI_H_
#define _SPI_H_

#if(_SYS_TYPE == _STC_11F32XE_)
sbit  SPI_CS = P2^7;
sbit  SPI_CLK = P2^6;
sbit  SPI_MOSI = P2^5;
sbit  SPI_MISO = P2^4;
#endif

#if(_SYS_TYPE == _STC_15L2K60S2_)
sbit  SPI_CS = P1^2;
sbit  SPI_CLK = P1^5;
sbit  SPI_MOSI = P1^3;
sbit  SPI_MISO = P1^4;
#endif

#define SET_SPI_CS  SPI_CS = 1
#define CLR_SPI_CS  SPI_CS = 0

#define SET_SPI_CK  SPI_CLK = 1
#define CLR_SPI_CK  SPI_CLK = 0

#define SET_SPI_MOSI  SPI_MOSI = 1
#define CLR_SPI_MOSI  SPI_MOSI = 0

#define STU_SPI_MISO  SPI_MISO

/*************************************************
  Function:		SPIReadByte
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
unsigned char SPIReadByte(unsigned char Addr); 

/*************************************************
  Function:		SPIWrite
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void SPIWriteByte(unsigned char Addr, unsigned char Data);

#endif

