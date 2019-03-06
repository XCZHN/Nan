/*===============================================================================================
==                                                                                              
**  Copyright(c) 2015  ZhenYao Science and technology co., LTD.   All rights reserved.
==                                                                                           
**	Author:		Asen                                                                           
==                                                                                          
**  Version:	v1.0.0                                                                          
==
**  Filename:	App_Record.h                                                                       
==                                                                                         
**  Function:	记录处理                                                             
==
**  Data:       2016-03-10
================================================================================================*/

#ifndef _RECORD_H_
#define _RECORD_H_



typedef enum 							    //开箱记录类型表
{
	OpenBoxType_Admin =0,                   //管理员开箱 
	OpenBoxType_Password,					//密码(或验证码)开箱 				
	OpenBoxType_Card,					    //刷卡开箱
	OpenBoxType_CardPwd,					//卡+密开箱
	OpenBoxType_Fingerprint,				//指纹开箱
	OpenBoxType_NetCmd,					    //网络指令开箱
	OpenBoxType_Postman,                    //投递员开箱					
	OpenBoxType_	
	
}E_RECORD_TYPE;

//----------------------------记录--------------------------------------------
//#define  ONE_BOX_MAX_RECORD_NUM    (128)  //一个箱最大的存储记录数 4k/32 = 128
#define  ONE_RECORD_LEN            (32)     //定义一笔记录的数据长度
#define  MAX_RECORD_NUM            (10000)  //最大的存储记录数 1M/32 = 32768

typedef	struct								//开箱记录
{
	uint32 u32RecordNum;                    //记录号
	uint32 u32BoxRecordID;                  //在对应箱中的记录编码
		
	uchar8 u8BoxID;                         //箱号
	uchar8 u8Type;                          //开箱方式
	uchar8 u8SaveOrTake;                    //存取状态(0未知,1存/2取)
	uchar8 u8Result;                        //开箱结果(0--成功,非0--失败)
	uchar8 u8Crc;                           //CRC校验

	uchar8 Time[6];                         //开箱时间
	uchar8 HandleInfo[12];                  //卡号或手机号或快递员编号
		
}ST_RECORD, *pST_RECORD;


//-------------------------------------------------Variable definitions--------------------------- 
extern ST_RECORD stRecord; 	




/***********************************************************************************************
* Function Name  : StoreOpenBoxRecord
* Description	 : 存储一笔开箱记录
* Input 		 : pstRecord -- 记录信息指针
* Output		 : None
* Return		 : 0--成功,非0参考错误代码
* Note  		 : 一笔记录固定为32Byte
***********************************************************************************************/
extern int StoreOpenBoxRecord(pST_RECORD pstRecord);

/***********************************************************************************************
* Function Name  : Clear_Record
* Description	 : 清除开箱记录
* Input 		 : None
* Output		 : None
* Return		 : None
* Note  		 : 清除所有开箱记录
***********************************************************************************************/
extern void Clear_Record(void);

/***********************************************************************************************
* Function Name  : ReadOpenBoxRecord
* Description	 : 读取一笔开箱记录
* Input 		 : RecordNum --记录号(0~N), pstRecord -- 记录信息指针
* Output		 : None
* Return		 : 0--成功,非0参考错误代码
* Note  		 : 一笔记录固定为32Byte
***********************************************************************************************/
extern int ReadOneRecord(uint32 RecordNum, pST_RECORD pstRecord);

/***********************************************************************************************
* Function Name  : ReadOneRecord_ToArrayBuf
* Description	 : 读取一笔开箱记录至数组中
* Input 		 : RecordNum --记录号(0~N), pArrayBuf -- 记录信息指针
* Output		 : None
* Return		 : 0--成功,非0参考错误代码
* Note  		 : 一笔记录固定为32Byte
***********************************************************************************************/
extern int ReadOneRecord_ToArrayBuf(uint32 RecordNum, uchar8* pArrayBuf);

/***********************************************************************************************
* Function Name  : QueryBoxRecord
* Description	 : 查询一笔开箱记录
* Input 		 : BoxNum -- 箱号, RecordNum --记录号(0~N), pstRecord -- 记录信息指针
* Output		 : None
* Return		 : 所在的记录编号
* Note  		 : 一笔记录固定为32Byte
***********************************************************************************************/
extern int QueryBoxRecord(uchar8 BoxNum, uint32 RecordID, pST_RECORD pstRecord);


/***********************************************************************************************
* Function Name  : StoreRecord_OpenBoxByCard
* Description	 : 保存1笔刷卡存取件记录
* Input 		 : box--箱号, iCard--卡号, SaveOrTake--(0未知,1存/2取)
* Output		 : None
* Return		 : None 
* Notes          : NULL
***********************************************************************************************/
extern void StoreRecord_OpenBoxByCard(uchar8 box, uint32 iCard, uchar8 SaveOrTake);

/***********************************************************************************************
* Function Name  : StoreRecord_OpenBoxByFinger
* Description	 : 保存1笔指纹开箱记录
* Input 		 : box--箱号, SaveOrTake--(0未知,1存/2取)
* Output		 : None
* Return		 : None 
* Notes          : NULL
***********************************************************************************************/
extern void StoreRecord_OpenBoxByFinger(uchar8 box, uchar8 SaveOrTake);

#endif //_RECORD_H_
/*===================================File end===================================================*/

