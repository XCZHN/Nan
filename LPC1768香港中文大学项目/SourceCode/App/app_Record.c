/*===============================================================================================
==                                                                                              
**  Copyright(c) 2014  ZhenYao Science and technology co., LTD.   All rights reserved.
==                                                                                           
**	Author:		Asen                                                                           
==                                                                                          
**  Version:	v1.0.0                                                                          
==
**  Filename:	App_Record.c                                                                       
==                                                                                         
**  Function:	记录处理                                                             
==
**  Data:       2016-03-10
================================================================================================*/

#include "Includes.h"


//-------------------------------------------------Variable definitions--------------------------- 

ST_RECORD stRecord; 	

/***********************************************************************************************
* Function Name  : StoreOpenBoxRecord
* Description	 : 存储一笔开箱记录
* Input 		 : pstRecord -- 记录信息指针
* Output		 : None
* Return		 : 0--成功,非0参考错误代码
* Note  		 : 一笔记录固定为32Byte
***********************************************************************************************/
int StoreOpenBoxRecord(pST_RECORD pstRecord)
{
	int iRet = 0;
	uint32 iAddr = Addr_RecordStart;

	if(sizeof(ST_RECORD) > ONE_RECORD_LEN)   //预防后续增加变量时出错
	{
		Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 5);
		delay_ms(1234);
		return -1;
	}
	
	//计算记录存储地址
	iAddr += (SysConfig.stRecord.iRecordCount % MAX_RECORD_NUM) * ONE_RECORD_LEN;
	if(iAddr > Addr_RecordEnd)
	{
		Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 15);
		delay_ms(5000);
		return 0xFF;
	}
	
	pstRecord->u32RecordNum = SysConfig.stRecord.iRecordCount;
	pstRecord->u32BoxRecordID = SysConfig.stBox[pstRecord->u8BoxID].iRecordCount;
	memcpy((uchar8*)&pstRecord->Time[0], (uchar8*)&gstSysRunInfo.TimeBuf[0], 6);

	//记录数据,写FLAH前先判断是否新的扇区,是则先擦除
	//FLASH_AutoWriteBuffer((uchar8*)pstRecord, iAddr, ONE_RECORD_LEN);
	if(0 == iAddr % FLASH_MAX_SectorByte)
	{
		FLASH_SectorErase(iAddr);	
		delay_ms(12);
	}
	FLASH_BufferWrite((uchar8*)pstRecord, iAddr, ONE_RECORD_LEN);

	//存储箱信息
	SysConfig.stRecord.iRecordCount += 1;
	iAddr = Addr_SysConfig + ((uint32)&SysConfig.stRecord.iRecordCount - (uint32)&SysConfig);
	Store_Parameters(iAddr, (uchar8*)&SysConfig.stRecord.iRecordCount, 4);

	SysConfig.stBox[pstRecord->u8BoxID].iRecordCount += 1;
	iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[pstRecord->u8BoxID].iRecordCount - (uint32)&SysConfig);
	Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[pstRecord->u8BoxID].iRecordCount, 4);
	
	return iRet;
}

/***********************************************************************************************
* Function Name  : Clear_Record
* Description	 : 清除开箱记录
* Input 		 : None
* Output		 : None
* Return		 : None
* Note  		 : 清除所有开箱记录
***********************************************************************************************/
void Clear_Record(void)
{
	uint32 iAddr = 0;
	uchar8 i = 0, BlockNum = 0;
	
	//更新上传记录指针
	SysConfig.stRecord.iUploadCount = SysConfig.stRecord.iRecordCount;
	iAddr = Addr_SysConfig + ((uint32)&SysConfig.stRecord.iUploadCount - (uint32)&SysConfig);
	Store_Parameters(iAddr, (uchar8*)&SysConfig.stRecord.iUploadCount, 4);	

	//清除记录数据
	BlockNum = (ONE_RECORD_LEN * MAX_RECORD_NUM) / FLASH_MAX_BlockByte;
	for(i=0; i<BlockNum; i++)
	{
		iAddr = Addr_RecordStart + (i*FLASH_MAX_BlockByte);
		FLASH_BlockErase(iAddr);
		delay_ms(1200);  //0.75~1.5S
	}
}

/***********************************************************************************************
* Function Name  : ReadOpenBoxRecord
* Description	 : 读取一笔开箱记录
* Input 		 : RecordNum --记录号(0~N), pstRecord -- 记录信息指针
* Output		 : None
* Return		 : 0--成功,非0参考错误代码
* Note  		 : 一笔记录固定为32Byte
***********************************************************************************************/
int ReadOneRecord(uint32 RecordNum, pST_RECORD pstRecord)
{
	int iRet = 0;
	uint32 iAddr = Addr_RecordStart;
	
	//计算记录存储地址
	iAddr += (RecordNum % MAX_RECORD_NUM) * ONE_RECORD_LEN;

	//记录数据
	FLASH_FastReadBuffer((uchar8*)pstRecord, iAddr, sizeof(ST_RECORD));
	
	return iRet;
}

/***********************************************************************************************
* Function Name  : ReadOneRecord_ToArrayBuf
* Description	 : 读取一笔开箱记录至数组中
* Input 		 : RecordNum --记录号(0~N), pArrayBuf -- 记录信息指针
* Output		 : None
* Return		 : 0--成功,非0参考错误代码
* Note  		 : 一笔记录固定为32Byte
***********************************************************************************************/
int ReadOneRecord_ToArrayBuf(uint32 RecordNum, uchar8* pArrayBuf)
{
	int iRet = 0;
	uchar8 addr = 0;
	memset((uchar8*)&stRecord, 0x0, sizeof(ST_RECORD));

	ReadOneRecord(RecordNum, &stRecord);

	//转换成数组格式
	memcpy(&pArrayBuf[addr], (uchar8*)&stRecord.u32RecordNum, 4);   
	addr += 4;
	pArrayBuf[addr++] = stRecord.u8BoxID;
	pArrayBuf[addr++] = stRecord.u8Type;
	pArrayBuf[addr++] = stRecord.u8SaveOrTake;
	pArrayBuf[addr++] = stRecord.u8Result;

	memcpy(&pArrayBuf[addr], (uchar8*)&stRecord.Time[0], 6);  
	addr += 6;
	memcpy(&pArrayBuf[addr], (uchar8*)&stRecord.HandleInfo[0], 12);  
	addr += 12;

	return iRet;
}

/***********************************************************************************************
* Function Name  : QueryBoxRecord
* Description	 : 查询一笔开箱记录
* Input 		 : u8BoxID -- 箱号, RecordNum --记录号(0~N), pstRecord -- 记录信息指针
* Output		 : None
* Return		 : 所在的记录编号
* Note  		 : 一笔记录固定为32Byte
***********************************************************************************************/
int QueryBoxRecord(uchar8 u8BoxID, uint32 RecordID, pST_RECORD pstRecord)
{
	int iRet = 0xFFFFFFFF;
	uint32 i = 0;
	uint32 iRecordNum = 0;

	for(i=0; i<MAX_RECORD_NUM; i++)
	{
		WDT_Feed();

		//倒过来读,从最大的记录号开始读取,提高读取效率
		if(SysConfig.stRecord.iRecordCount > i) 
		{
			iRecordNum = SysConfig.stRecord.iRecordCount - i;
			iRecordNum -= 1; //记录号是从0开始的
			//读取记录数据
			memset((uchar8*)&stRecord, 0x0, sizeof(ST_RECORD));
			ReadOneRecord(iRecordNum, &stRecord);

			//比对数据
			if((stRecord.u8BoxID > MAX_BOX_NUM)||(0 == stRecord.u8BoxID))
			{
				return 0xFFFFFFFF;
			}
			else if((u8BoxID == stRecord.u8BoxID)&&(RecordID == stRecord.u32BoxRecordID))
			{
				memcpy((uchar8*)pstRecord, (uchar8*)&stRecord, sizeof(ST_RECORD));
				return i;
			}
		}
		else
		{
			return 0xFFFFFFFF;
		}
	}
		
	return iRet;
}


/***********************************************************************************************
* Function Name  : StoreRecord_OpenBoxByCard
* Description	 : 保存1笔刷卡存取件记录
* Input 		 : box--箱号, iCard--卡号, SaveOrTake--(0未知,1存/2取)
* Output		 : None
* Return		 : None 
* Notes          : NULL
***********************************************************************************************/
void StoreRecord_OpenBoxByCard(uchar8 box, uint32 iCard, uchar8 SaveOrTake)
{	
	ST_RECORD stRecord; 	
	memset((uchar8*)&stRecord, 0x0, sizeof(ST_RECORD));

	stRecord.u8BoxID = box;
	stRecord.u8Type = OpenBoxType_Card;
	stRecord.u8SaveOrTake = SaveOrTake;
	memcpy((uchar8*)stRecord.HandleInfo, (uchar8*)&iCard, 4);
	memcpy((uchar8*)&stRecord.Time[0], (uchar8*)&gstSysRunInfo.TimeBuf[0], 6);
	
	StoreOpenBoxRecord(&stRecord);
}

/***********************************************************************************************
* Function Name  : StoreRecord_OpenBoxByFinger
* Description	 : 保存1笔指纹开箱记录
* Input 		 : box--箱号, SaveOrTake--(0未知,1存/2取)
* Output		 : None
* Return		 : None 
* Notes          : NULL
***********************************************************************************************/
void StoreRecord_OpenBoxByFinger(uchar8 box, uchar8 SaveOrTake)
{	
	ST_RECORD stRecord; 	
	memset((uchar8*)&stRecord, 0x0, sizeof(ST_RECORD));

	stRecord.u8BoxID = box;
	stRecord.u8Type = OpenBoxType_Fingerprint;
	stRecord.u8SaveOrTake = SaveOrTake;
	memcpy((uchar8*)&stRecord.Time[0], (uchar8*)&gstSysRunInfo.TimeBuf[0], 6);
	
	StoreOpenBoxRecord(&stRecord);
}

/*===================================File end===================================================*/


