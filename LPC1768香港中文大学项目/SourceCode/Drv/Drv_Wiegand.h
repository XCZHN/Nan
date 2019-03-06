/*===============================================================================================
==                                                                                              
**  Copyright(c) 2014  Intelligent Admin electronic systems co., LTD. All rights reserved.
==                                                                                           
**	Author:		Asen                                                                           
==                                                                                          
**  Version:	v1.0.0                                                                          
==
**  Filename:	Drv_Wiegand.h                                                                       
==                                                                                         
**  Function:	韦根(Wiegand)接收协议
==                                                                                         
**  Data:       2016/07/11                                                                         
================================================================================================*/

#ifndef	_DRV_WIEGAND_H_
#define	_DRV_WIEGAND_H_

#include "Config.h"
#include "LPC17xx.h"

#define WG_D0  			4		// P2.4
#define WG_D1  			5		// P2.5

#define WG_TIME_OUT     5000

typedef struct
{
	uint8 BitCnt;				//位接收计数器
	uint8 bSignalFlag;			//信号接收标识
	uint8 crc_f1;               //数据位校验
	uint8 crc_f2;               //数据位校验
	uint8 DataBuf[32];	 		//数据接收缓冲区
	
}ST_WG_TYPE;

extern volatile ST_WG_TYPE gstWG;


/***********************************************************************************************
* Function Name  : Wiegand_Init
* Description	 : 独立看门狗初始化
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void Wiegand_Init(void);

/***********************************************************************************************
* Function Name  : Wiegand_ReardCardID
* Description	 : Wiegand接口接收读取ID卡号
* Input 		 : None
* Output		 : None
* Return		 : 接收到的ID卡号
***********************************************************************************************/
extern uint32 Wiegand_ReardCardID(void);


#endif
/*=============================File end========================================================*/
