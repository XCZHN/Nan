/*===============================================================================================
==                                                                                              
**  Copyright(c) 2015  ZhenYao Science and technology co., LTD.   All rights reserved.
==                                                                                           
**	Author:		Asen                                                                           
==                                                                                          
**  Version:	v1.0.0                                                                          
==
**  Filename:	Card.h                                                                       
==                                                                                         
**  Function:	IC��ID����ز���                                                               
==
**  Data:       2015-09-11
================================================================================================*/

#ifndef _CARD_H_
#define _CARD_H_


#include "Config.h"   


#define CARD_NameLen          8
#define CARD_JobIdLen         8

typedef	struct								//�û�����Ϣ
{
	uint32 iEnableStartLockers;             //����ʹ�õ���ʼ���
	uint32 iEnableEndLockers;               //����ʹ�õĽ������
	
	uchar8 SysCode[6];                      //ϵͳ��ʶ��
	uchar8 ValidDate[3];                    //��Ч��
	uchar8 Name[CARD_NameLen];              //����   
	uchar8 JobID[CARD_JobIdLen];            //���� 

	uchar8 OnLocker[6];                     //���ڹ�����
	uchar8 u8OnBox;                         //�������
	uchar8 OnTime[6];                       //��ȡʱ��
	uchar8 bStoreOrTake;                    //1.��.2.ȡ
	uchar8 Reserved;                        //Ԥ��
	uchar8 u8Crc;                           //CRC
		
}ST_USER_CARD;

typedef	struct                              //�������ÿ�
{
	char   SysIdCode[6 +1];
	char   Name[6 +1];
	uint32 iCardID;
	uchar8 u8WorkMode;
	uchar8 u8AllowOpenBoxNum;

	uint32 iID;
	uint32 iMinTime;
	uint32 iMaxTime;
	uint32 iCloseTimeout;
	uint32 iAlarmTime;

}ST_PARAMET_CARD;

extern volatile ST_USER_CARD stUserCard;


/***********************************************************************************************
* Function Name  : Get_IC_ID_CardSN
* Description	 : ��ȡIC��ID���Ŀ����к�
* Input 		 : None
* Output		 : None
* Return		 : �����Ŀ���
***********************************************************************************************/
extern uint32 Get_IC_ID_CardSN(void);

/***********************************************************************************************
* Function Name  : CheckCardValid
* Description	 : ���Ԥ��Ȩ��
* Input 		 : iCardID -- ����
* Output		 : None
* Return		 : 0 <= -- ��Ч(�˿������ڵ����λ��), ����--��Ч
***********************************************************************************************/
extern int CheckCardValid(uint32 iCardID);

/***********************************************************************************************
* Function Name  : AddOneValidCardNum
* Description	 : ����һ��Ԥ��Ȩ����
* Input 		 : iCardNum -- ����
* Output		 : None
* Return		 : ��ӽ��
***********************************************************************************************/
extern uchar8 AddOneValidCardNum(uint32 iCardNum);


/***********************************************************************************************
* Function Name  : ArgCardOperation
* Description	 : ��ȡ�������ÿ���ִ�в�������
* Input 		 : void
* Output		 : None
* Return		 : None 
* Notes          : ���ÿ��е�ĳЩ�������ԼӺ�д�ؿ���
***********************************************************************************************/
extern void ArgCardOperation(void);

/***********************************************************************************************
* Function Name  : CardOperation
* Description	 : ˢ��ȡ����������
* Input 		 : handle
* Output		 : None
* Return		 : None 
* Notes          : ϵͳ����ʱ�Ŷ��� 
***********************************************************************************************/
extern void CardOperation(uchar8 handle);


#endif //_CARD_H_


/*===================================File end===================================================*/

