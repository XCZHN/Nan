/*===============================================================================================
==                                                                                              
**  Copyright(c) 2017  ZhenYao Science and technology co., LTD.   All rights reserved.
==                                                                                           
**	Author:		Asen                                                                           
==                                                                                          
**  Version:	v1.0.0                                                                          
==
**  Filename:	app_Lockers.h                                                                       
==                                                                                         
**  Function:	�Ĵ���ȡ��������                                                             
==
**  Data:       2017-06-27
================================================================================================*/

#ifndef APP_LOCKERS_H_
#define APP_LOCKERS_H_

#include "Config.h"   


#define USER_STORE         1       //�û��Ĵ��ʶ
#define USER_TAKEAWAY      2       //�û�ȡ���ʶ
#define USER_STORE_FAILD   0xE1    //�û��Ĵ�ʧ�ܱ�ʶ
#define SERVER_REPLY_MASK  0xEB    //������Ӧ����
#define ALLOW_CONSIGN      0x00    //����Ĵ�m
#define NOTALLOW_CONSIGN   0xA1    //�ѼĴ�,������Ĵ�
#define WAITING_TIMEOUT    3000    //������Ĵ�

typedef	struct
{
	uint32 iLastHandleTime;
	uint32 iTimeout;
	uint32 uiReadCardTime;

	uchar8 TryCount;
	
}ST_CONSIGN;

typedef	struct
{
	uint32 iCardID;
	uint32 iWaitingTime;

	uchar8 u8Cmd;
	uchar8 OnLockers[8];
	uchar8 u8OnBox;
	uchar8 bAllowConsign;
	uchar8 bReplyOk;
	uchar8 u8TryCnt;
	uchar8 Notice[20];
	
}ST_ONLINEE_ASK;

extern volatile ST_ONLINEE_ASK stOnlineAsk;

/***********************************************************************************************
* Function Name  : UserTakeAway_OnlineAsk
* Description	 : �û���������ȡ��
* Input 		 : iCardID -- ��ID��
* Output		 : None
* Return		 : None 
***********************************************************************************************/
extern void UserTakeAway_OnlineAsk(uint32 iCardID, uchar8 box);

/******************************************************************************************************
* Function Name  : ServerReply_CommHandle
* Description    : ������Ӧ��ָ��ִ��
* Input          : CMD--������, pRecvBuf������������, dLen -- ���ݲ��ֳ���
* Variable       : gstCmdPacket -- ͨѶָ�����ݰ�
* Output         : None
* Return         : None
******************************************************************************************************/
extern void ServerReply_CommHandle(uchar8 CMD, uchar8* pRecvBuf, uint16 dLen);

/***********************************************************************************************
* Function Name  : User_Consign
* Description	 : �û��Ĵ��������
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : ���Ĵ�ģʽѡ���Ӧ�Ĳ�������
***********************************************************************************************/
extern void User_Consign(void);

/***********************************************************************************************
* Function Name  : Lockers_TakeAwayByFinger
* Description	 : ��ָ��ȡ��
* Input 		 : None
* Output		 : None
* Return		 : None 
***********************************************************************************************/
extern void Lockers_TakeAwayByFinger(void);

#endif //APP_LOCKERS_H_


/*===================================File end===================================================*/

