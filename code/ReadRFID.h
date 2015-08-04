/*********************************************************
  Copyright (C), 2013-2016
  File name:	ReadRFID.h
  Author:   	
  Version:   	1.0
  Date: 		13.11.23
  Description:  ����ģ��
  History:            
*********************************************************/
#ifndef	__READRFID_H__
#define __READRFID_H__

// RFID Word Mode
#define ISO14443_A			  1

// RFID Commond
#define PCD_IDLE              0x00               //ȡ����ǰ����
#define PCD_AUTHENT           0x0E               //��֤��Կ
#define PCD_RECEIVE           0x08               //��������
#define PCD_TRANSMIT          0x04               //��������
#define PCD_TRANSCEIVE        0x0C               //���Ͳ���������
#define PCD_RESETPHASE        0x0F               //��λ
#define PCD_CALCCRC           0x03               //CRC����

// Mifare_One Commond
#define PICC_REQIDL           0x26               //Ѱ��������δ��������״̬
#define PICC_REQALL           0x52               //Ѱ��������ȫ����
#define PICC_ANTICOLL1        0x93               //����ײ
#define PICC_ANTICOLL2        0x95               //����ײ
#define PICC_AUTHENT1A        0x60               //��֤A��Կ
#define PICC_AUTHENT1B        0x61               //��֤B��Կ
#define PICC_READ             0x30               //����
#define PICC_WRITE            0xA0               //д��
#define PICC_DECREMENT        0xC0               //�ۿ�
#define PICC_INCREMENT        0xC1               //��ֵ
#define PICC_RESTORE          0xC2               //�������ݵ�������
#define PICC_TRANSFER         0xB0               //���滺����������
#define PICC_HALT             0x50               //����

// Mifare_One Register
#define     RFU00                 0x00    		 // PAGE 0
#define     CommandReg            0x01    
#define     ComIEnReg             0x02    
#define     DivlEnReg             0x03    
#define     ComIrqReg             0x04    
#define     DivIrqReg             0x05
#define     ErrorReg              0x06    
#define     Status1Reg            0x07    
#define     Status2Reg            0x08    
#define     FIFODataReg           0x09
#define     FIFOLevelReg          0x0A
#define     WaterLevelReg         0x0B
#define     ControlReg            0x0C
#define     BitFramingReg         0x0D
#define     CollReg               0x0E
#define     RFU0F                 0x0F
     
#define     RFU10                 0x10			// PAGE 1	
#define     ModeReg               0x11
#define     TxModeReg             0x12
#define     RxModeReg             0x13
#define     TxControlReg          0x14
#define     TxAutoReg             0x15
#define     TxSelReg              0x16
#define     RxSelReg              0x17
#define     RxThresholdReg        0x18
#define     DemodReg              0x19
#define     RFU1A                 0x1A
#define     RFU1B                 0x1B
#define     MifareReg             0x1C
#define     RFU1D                 0x1D
#define     RFU1E                 0x1E
#define     SerialSpeedReg        0x1F

#define     RFU20                 0x20  		// PAGE 2    
#define     CRCResultRegM         0x21
#define     CRCResultRegL         0x22
#define     RFU23                 0x23
#define     ModWidthReg           0x24
#define     RFU25                 0x25
#define     RFCfgReg              0x26
#define     GsNReg                0x27
#define     CWGsCfgReg            0x28
#define     ModGsCfgReg           0x29
#define     TModeReg              0x2A
#define     TPrescalerReg         0x2B
#define     TReloadRegH           0x2C
#define     TReloadRegL           0x2D
#define     TCounterValueRegH     0x2E
#define     TCounterValueRegL     0x2F
// PAGE 3      
#define     RFU30                 0x30			// PAGE 3      
#define     TestSel1Reg           0x31
#define     TestSel2Reg           0x32
#define     TestPinEnReg          0x33
#define     TestPinValueReg       0x34
#define     TestBusReg            0x35
#define     AutoTestReg           0x36
#define     VersionReg            0x37
#define     AnalogTestReg         0x38
#define     TestDAC1Reg           0x39  
#define     TestDAC2Reg           0x3A   
#define     TestADCReg            0x3B   
#define     RFU3C                 0x3C   
#define     RFU3D                 0x3D   
#define     RFU3E                 0x3E   
#define     RFU3F		  		  0x3F

// RFID Echo
#define 	MI_OK                 (0)
#define 	MI_NOTAGERR           (-1)
#define 	MI_ERR                (-2)

// RFID Pwd
#define		RFID_PWD_A				0x60 
#define		RFID_PWD_B				0x61 


#if(_SYS_TYPE == _STC_11F32XE_)
sbit  RFID_RST = P2^3;
#endif

#if(_SYS_TYPE == _STC_15L2K60S2_)
sbit  RFID_RST = P0^0;
#endif

#define SET_RFIDRST  RFID_RST=1
#define CLR_RFIDRST  RFID_RST=0

/*************************************************
  Function:		RFIDFIndCard
  Description:  
  Input:		
  	1.req_code	 0x52 = Ѱ��Ӧ�������з���14443A��׼�Ŀ�
  				 0x26 = Ѱδ��������״̬�Ŀ�
  Output:		
  	1.pTagType
  				 0x4400 = Mifare_UltraLight
                 0x0400 = Mifare_One(S50)
                 0x0200 = Mifare_One(S70)
                 0x0800 = Mifare_Pro(X)
                 0x4403 = Mifare_DESFire
  Return:		
  Others:
*************************************************/
int RFIDFIndCard(INT8U req_code,INT8U *pTagType);

/*************************************************
  Function:		RFIDCollision
  Description:  RFID ����ײ
  Input:		
  	1.	pSnr	CardID
  Output:		
  Return:		
  Others:
*************************************************/
int RFIDCollision(INT8U *pSnr);

/*************************************************
  Function:		RFIDSelectCard
  Description:  RFID ѡ��
  Input:		
  	1.	CardID	
  Output:		
  Return:		
  Others:
*************************************************/
int RFIDSelectCard(INT8U *CardId);

/*************************************************
  Function:		RFIDSelectCard
  Description:  RFID ���Ƭ��������״̬
  Input:		
  	1.	CardID	
  Output:		
  Return:		
  Others:
*************************************************/
int RFIDCardIdle(void);

/*************************************************
  Function:		RFIDCheckCardPwd
  Description:  RFID ��֤��Ƭ����
  Input:		
  	1.	auth_mode	0x60 = ��֤A��Կ  0x61 = ��֤B��Կ 
  	2.	Block		���ַ
  	3.	Pwd			����
  	4.	CardId		��Ƭ���к�
  Output:		
  Return:		
  Others:
*************************************************/
int RFIDCheckCardPwd(INT8U auth_mode,INT8U Block,INT8U *Pwd,INT8U *CardId);

/*************************************************
  Function:		RFIDReadBlock
  Description:  RFID ��ȡһ������
  Input:		
  	1.	addr	
  	2.	pData	
  Output:		
  Return:		
  Others:
*************************************************/
int RFIDReadBlock(INT8U addr,INT8U *pData);

/*************************************************
  Function:		RFIDWriteBlock
  Description:  RFID дһ������
  Input:		
  	1.	addr	
  	2.	pData	
  Output:		
  Return:		
  Others:
*************************************************/  
int RFIDWriteBlock(INT8U addr,INT8U *pData);

/*************************************************
  Function:		RFIDAntennaOn
  Description:  RFID ������
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/  
void RFIDAntennaOn(void);

/*************************************************
  Function:		RFIDAntennaOff
  Description:  RFID �ر�����
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/  
void RFIDAntennaOff(void);

/*************************************************
  Function:		RFIDReset
  Description:  RFID ��λ
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/  
int RFIDReset(void);

/*************************************************
  Function:		RFIDConfigMode
  Description:  RFID ����
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/  
int RFIDConfigMode(INT8U type);

/*************************************************
  Function:		RFIDInit
  Description:  RFID ��ʼ��
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/  
void RFIDInit(void);


#endif
