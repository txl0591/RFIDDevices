/*********************************************************
  Copyright (C), 2013-2016
  File name:	spi.c
  Author:   	txl
  Version:   	1.0
  Date: 		13.11.23
  Description:  
  History:            
*********************************************************/
#include "Include.h"

/*************************************************
  Function:		SPIReadByte
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
static unsigned char SPIRead(void)
{
	unsigned char SPICount;                               
	unsigned char SPIData; 
	                 
	SPIData = 0;
	for (SPICount = 0; SPICount < 8; SPICount++)          
	{
		SPIData <<=1;                                     
		CLR_SPI_CK;
		nop();nop();
		if(STU_SPI_MISO)
		{
 			SPIData|=0x01;
		}
		SET_SPI_CK;
		nop();nop();
	}              
	return (SPIData);
} 

/*************************************************
  Function:		SPIWrite
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
static void SPIWrite(unsigned char SPIData)
{
	unsigned char SPICount;                     
	for (SPICount = 0; SPICount < 8; SPICount++)
	{
		if (SPIData & 0x80)
		{
			SET_SPI_MOSI;
		}
		else
		{
			CLR_SPI_MOSI;
		} 
		nop();nop();nop();nop();
		CLR_SPI_CK;nop();nop();nop();nop();
		SET_SPI_CK;nop();nop();nop();nop();
		SPIData <<= 1;
	}         	
}

/*************************************************
  Function:		SPIReadByte
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
unsigned char SPIReadByte(unsigned char Addr)
{
    unsigned char ucResult=0;
	CLR_SPI_CS;
	SPIWrite(Addr);
	ucResult=SPIRead();
	SET_SPI_CS;
	return ucResult;
} 

/*************************************************
  Function:		SPIWrite
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void SPIWriteByte(unsigned char Addr, unsigned char Data)
{
	CLR_SPI_CS;
	SPIWrite(Addr);
	SPIWrite(Data);
	SET_SPI_CS;
}

