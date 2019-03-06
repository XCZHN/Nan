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
**  Function:	IC卡ID卡相关操作                                                               
==
**  Data:       2015-09-11
================================================================================================*/

#ifndef _CARD_H_
#define _CARD_H_


#include "Config.h"   


#define CARD_NameLen          8
#define CARD_JobIdLen         8

typedef	struct								//用户卡信息
{
	uint32 iEnableStartLockers;             //允许使用的起始箱号
	uint32 iEnableEndLockers;               //允许使用的结束箱号
	
	uchar8 SysCode[6];                      //系统标识码
	uchar8 ValidDate[3];                    //有效期
	uchar8 Name[CARD_NameLen];              //姓名   
	uchar8 JobID[CARD_JobIdLen];            //工号 

	uchar8 OnLocker[6];                     //所在柜名称
	uchar8 u8OnBox;                         //所在箱号
	uchar8 OnTime[6];                       //存取时间
	uchar8 bStoreOrTake;                    //1.存.2.取
	uchar8 Reserved;                        //预留
	uchar8 u8Crc;                           //CRC
		
}ST_USER_CARD;

typedef	struct                              //参数设置卡
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
* Description	 : 读取IC或ID卡的卡序列号
* Input 		 : None
* Output		 : None
* Return		 : 读到的卡号
***********************************************************************************************/
extern uint32 Get_IC_ID_CardSN(void);

/***********************************************************************************************
* Function Name  : CheckCardValid
* Description	 : 检查预授权卡
* Input 		 : iCardID -- 卡号
* Output		 : None
* Return		 : 0 <= -- 有效(此卡号所在的相对位置), 其它--无效
***********************************************************************************************/
extern int CheckCardValid(uint32 iCardID);

/***********************************************************************************************
* Function Name  : AddOneValidCardNum
* Description	 : 增加一个预授权卡号
* Input 		 : iCardNum -- 卡号
* Output		 : None
* Return		 : 添加结果
***********************************************************************************************/
extern uchar8 AddOneValidCardNum(uint32 iCardNum);


/***********************************************************************************************
* Function Name  : ArgCardOperation
* Description	 : 读取参数设置卡并执行参数设置
* Input 		 : void
* Output		 : None
* Return		 : None 
* Notes          : 设置卡中的某些参数会自加后写回卡中
***********************************************************************************************/
extern void ArgCardOperation(void);

/***********************************************************************************************
* Function Name  : CardOperation
* Description	 : 刷卡取件操作流程
* Input 		 : handle
* Output		 : None
* Return		 : None 
* Notes          : 系统空闲时才读卡 
***********************************************************************************************/
extern void CardOperation(uchar8 handle);


#endif //_CARD_H_


/*===================================File end===================================================*/

