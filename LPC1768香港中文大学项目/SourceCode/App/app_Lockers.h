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
**  Function:	寄存柜存取操作流程                                                             
==
**  Data:       2017-06-27
================================================================================================*/

#ifndef APP_LOCKERS_H_
#define APP_LOCKERS_H_

#include "Config.h"   


#define USER_STORE         1       //用户寄存标识
#define USER_TAKEAWAY      2       //用户取物标识
#define USER_STORE_FAILD   0xE1    //用户寄存失败标识
#define SERVER_REPLY_MASK  0xEB    //服务器应答标记
#define ALLOW_CONSIGN      0x00    //允许寄存m
#define NOTALLOW_CONSIGN   0xA1    //已寄存,不允许寄存
#define WAITING_TIMEOUT    3000    //不允许寄存

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
* Description	 : 用户在线请求取件
* Input 		 : iCardID -- 卡ID号
* Output		 : None
* Return		 : None 
***********************************************************************************************/
extern void UserTakeAway_OnlineAsk(uint32 iCardID, uchar8 box);

/******************************************************************************************************
* Function Name  : ServerReply_CommHandle
* Description    : 服务器应答指令执行
* Input          : CMD--命令字, pRecvBuf－－命令数据, dLen -- 数据部分长度
* Variable       : gstCmdPacket -- 通讯指令数据包
* Output         : None
* Return         : None
******************************************************************************************************/
extern void ServerReply_CommHandle(uchar8 CMD, uchar8* pRecvBuf, uint16 dLen);

/***********************************************************************************************
* Function Name  : User_Consign
* Description	 : 用户寄存操作流程
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : 按寄存模式选择对应的操作流程
***********************************************************************************************/
extern void User_Consign(void);

/***********************************************************************************************
* Function Name  : Lockers_TakeAwayByFinger
* Description	 : 按指纹取件
* Input 		 : None
* Output		 : None
* Return		 : None 
***********************************************************************************************/
extern void Lockers_TakeAwayByFinger(void);

#endif //APP_LOCKERS_H_


/*===================================File end===================================================*/

