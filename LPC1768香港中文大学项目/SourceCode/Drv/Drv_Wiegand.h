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
**  Function:	Τ��(Wiegand)����Э��
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
	uint8 BitCnt;				//λ���ռ�����
	uint8 bSignalFlag;			//�źŽ��ձ�ʶ
	uint8 crc_f1;               //����λУ��
	uint8 crc_f2;               //����λУ��
	uint8 DataBuf[32];	 		//���ݽ��ջ�����
	
}ST_WG_TYPE;

extern volatile ST_WG_TYPE gstWG;


/***********************************************************************************************
* Function Name  : Wiegand_Init
* Description	 : �������Ź���ʼ��
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void Wiegand_Init(void);

/***********************************************************************************************
* Function Name  : Wiegand_ReardCardID
* Description	 : Wiegand�ӿڽ��ն�ȡID����
* Input 		 : None
* Output		 : None
* Return		 : ���յ���ID����
***********************************************************************************************/
extern uint32 Wiegand_ReardCardID(void);


#endif
/*=============================File end========================================================*/
