/*********************************************************
  Copyright (C), 2013-2016
  File name:	Logic.h
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
#ifndef _LOGIC_H_
#define _LOGIC_H_

#define ICCARD_COUNT		16

/*************************************************
  Function:		LogicGetFrameVersion
  Description:  获取协议版本
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
INT8U LogicGetFrameVersion(void);

/*************************************************
  Function:		LogicGetMasterCtl
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
INT8U LogicGetMasterCtl(void);

/*************************************************
  Function:		LogicGetMasterCtl
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void LogicSetMasterCtl(INT8U Ctl);

/*************************************************
  Function:		LogicProc
  Description:  命令解析
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void LogicProc(void);

/*************************************************
  Function:		LogicInit
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void LogicInit(void);


#endif

