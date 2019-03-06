/*===============================================================================================
==                                                                                              
**  Copyright(c) 2015  shenzhen zhenyao technology co., LTD. All rights reserved.
==                                                                                           
**	Author:		zenghl                                                                          
==                                                                                          
**  Version:	v1.0.0                                                                          
==
**  Filename:	Fingerprint.c                                                                      
==                                                                                         
**  Function:	指纹识别应用层文件  
==
**  Data:       2015-11-26
================================================================================================*/

#ifndef	_APP_FINGERPRINT_H_
#define	_APP_FINGERPRINT_H_

/*----------------- Fingerprint module define ----------------------------------*/

#define	FG_EN_pin	           28		//P4.28

#define	Fingerprint_pin  	2        //P2.2


#define GET_Fingerprint_STATE     (LPC_GPIO2->FIOPIN&0x04)   

#define MAX_ZW_UNM                     1000
//------------------------指纹指令-------------------------------------
#define ZW_PackageID_CMD               0x01      //数据包标识--指令包
#define ZW_PackageID_PACKAGE           0x02      //数据包标识--数据包

#define ZW_CMD_GetImg                  0x01      //获取图像
#define ZW_CMD_GenChar                 0x02      //生成特征码
#define ZW_CMD_Match                   0x03      //精确对比两枚指纹
#define ZW_CMD_Search                  0x04      //搜索指纹
#define ZW_CMD_RegModel                0x05      //特征合成模块
#define ZW_CMD_Store                   0x06      //存储模板
#define ZW_CMD_LoadChar                0x07      //读出模板
#define ZW_CMD_UpChar                  0x08      //上传特征或模块
#define ZW_CMD_DownChar                0x09      //下载特征或模块 
#define ZW_CMD_DeletChar               0x0C      //删除模板
#define ZW_CMD_Empty                   0x0D      //清空指纹
#define ZW_CMD_SetSysPara              0x0E      //设置模块系统基本参数
#define ZW_CMD_ReadSysPara             0x0F      //读系统参数
#define ZW_CMD_WriteNotepad            0x18      //写记事本 
#define ZW_CMD_ReadNotepad             0x19      //读记事本
#define ZW_CMD_TempleteNum             0x1D      //读有效模板个数  

#define ZW_StopHandle                  0xFFFF    //停止执行

extern uchar8 FingerprintData[520];  //存储指纹数据(指纹号2B + 512B指纹数据)

/***********************************************************************************************
* Function Name  : Fingerprint_Init
* Description	 : 指纹模块初始化
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void Fingerprint_Init(void);

/***********************************************************************************************
* Function Name  : Fingerprint_PowerOn
* Description	 : 指纹模块上电
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void Fingerprint_PowerOn(void);

/***********************************************************************************************
* Function Name  : Fingerprint_PowerOff
* Description	 : 指纹模块关电
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void Fingerprint_PowerOff(void);

/***********************************************************************************************
* Function Name  : SendCmdToFingerprint_GetImg
* Description	 : 获取图像
* Input 		 : None
* Output		 : None
* Return		 : None
* Note  		 : 发送:EF 01	FF FF FF FF 01 00 03 01 00 05
*				   返回:EF 01	FF FF FF FF 07 00 03 xx(确认码1字节) sum(校验和2字节)
*				   确认码=0x00 表示录入成功
*				   确认码=0x01 表示收包有错
*				   确认码=0x02 表示传感器上无手指
*				   确认码=0x03 表示录入不成功
***********************************************************************************************/
extern void SendCmdToFingerprint_GetImg(void);

/***********************************************************************************************
* Function Name  : SendCmdToFingerprint_GenChar
* Description	 : 图像生成特征
* Input 		 : BufferID---缓存ID
* Output		 : None
* Return		 : None
* Note  		 : 发送:EF 01	FF FF FF FF 01 00 04 02 BufferID sum
*				   返回:EF 01	FF FF FF FF 07 00 03 xx(确认码1字节) sum(校验和2字节)
*				   确认码=0x00 表示生成特征成功
*				   确认码=0x01 表示收包有错
*				   确认码=0x06 表示指纹图像太乱而生不成特征
*				   确认码=0x07 表示指纹图像正常，但特征点太少而生不成特征
*				   确认码=0x15 表示图像缓冲区内没有有效原始图而生不成图像
***********************************************************************************************/
extern void SendCmdToFingerprint_GenChar(uchar8 BufferID);

/***********************************************************************************************
* Function Name  : SendCmdToFingerprint_RegModel
* Description	 : 特征合成模板
* Input 		 : None
* Output		 : None
* Return		 : None
* Note  		 : 发送:EF 01	FF FF FF FF 01 00 03 05 00 09
*				   返回:EF 01	FF FF FF FF 07 00 03 xx(确认码1字节) sum(校验和2字节)
*				   确认码=0x00 表示合并成功
*				   确认码=0x01 表示收包有错
*				   确认码=0x0a 表示合并失败(两枚指纹不属于同一手指)
***********************************************************************************************/
extern void SendCmdToFingerprint_RegModel(void);

/***********************************************************************************************
* Function Name  : SendCmdToFingerprint_Store
* Description	 : 存储模板
* Input 		 : BufferID---缓存ID   PageID--存储位置ID
* Output		 : None
* Return		 : None
* Note  		 :  发送:EF 01	FF FF FF FF 01(包标识01) 00 06(包长度0006) 06(指令代码06) BufferID(缓冲区号1字节) PageID(位置号2字节) 
     				 sum(检验和2字节)
*					返回:EF 01	FF FF FF FF 07 00 03 xx(确认码1字节) sum(校验和2字节)
*					确认码=0x00 表示存储成功
*					确认码=0x01 表示收包有错
*					确认码=0x0b 表示PageID超出指纹库范围
*					确认码=0x11 表示写FLASH出错
***********************************************************************************************/
extern void SendCmdToFingerprint_Store(uchar8 BufferID, uint16 PageID);

/***********************************************************************************************
* Function Name  : SendCmdToFingerprint_DeletChar
* Description	 : 删除模板
* Input 		 : PageID--存储位置ID  num--删除字节数
* Output		 : None
* Return		 : None
* Note  		 : 发送:EF 01	FF FF FF FF 01 00 07(包长度) 0C(命令) PageID(页码2字节) N(删除个数2字节) sum(检验和2字节)
*				   返回:EF 01	FF FF FF FF 07 00 03 xx(确认码1字节) sum(校验和2字节)
*				   确认码=0x00 表示删除模板成功
*				   确认码=0x01 表示收包有错
*				   确认码=0x02 表示删除模板失败
***********************************************************************************************/
extern void SendCmdToFingerprint_DeletChar(uint16 PageID, uint16 num);

/***********************************************************************************************
* Function Name  : SendCmdToFingerprint_Empty
* Description	 : 清空指纹库
* Input 		 : None
* Output		 : None
* Return		 : None
* Note  		 : 发送:EF 01	FF FF FF FF 01 00 03 0d 00 11
*				   返回:EF 01	FF FF FF FF 07 00 03 xx(确认码1字节) sum(校验和2字节)
*				   确认码=0x00 表示清空成功
*				   确认码=0x01 表示收包有错
*				   确认码=0x02 表示清空失败
***********************************************************************************************/
extern void SendCmdToFingerprint_Empty(void);

/***********************************************************************************************
* Function Name  : SendCmdToFingerprint_Search
* Description	 : 搜索指纹
* Input 		 : BufferID--缓存ID  StartPage--起始页   PageNum--页数
* Output		 : None
* Return		 : None
* Note  		 : 发送:EF 01	FF FF FF FF 01(包标识01) 00 08(包长度0008) 04(指令代码04) BufferID(缓冲区号1字节) StartPage(起始页2字节) 
*    		       PageNum(页数2字节) sum(检验和2字节)
*				   返回:EF 01	FF FF FF FF 07 00 07 xx(确认码1字节) PageID(页码2字节) MatchScore(得分2字节) sum(校验和2字节)
*				   确认码=0x00 表示搜索到
*				   确认码=0x01 表示收包有错
*				   确认码=0x09 表示没搜索到，此时页码与得分为0
***********************************************************************************************/
extern void SendCmdToFingerprint_Search(uchar8 BufferID, uint16 StartPage, uint16 PageNum);



/***********************************************************************************************
* Function Name  : Fingerprint_Search
* Description	 : 指纹搜索
* Input 		 : handle -- (=0xFF操作超时取消指纹录入)
* Output		 : None
* Return		 : 搜索到匹配的指纹返回指纹号(1--200), 
                   0xA000-0xA00F 对应当前执行步骤0-16
				   0xE0xx-0xEFxx 错误代号, 0xFFFF--未找到匹配指纹号
* Note  		 : 内部包含了操作提示
***********************************************************************************************/
extern uint16 Fingerprint_Search(uint16 handle);

/***********************************************************************************************
* Function Name  : Fingerprint_Login
* Description	 : 指纹注册
* Input 		 : FingerNum -- 生成存储指纹号(=0xFFFF操作超时取消指纹录入)
* Output		 : None
* Return		 : 0xA000-0xA00F 对应当前执行步骤0-16
                   >=0xE000--出现异常,终止执行, 
                   0xABCD--录入指纹成功
* Note  		 : 内部包含了操作提示
***********************************************************************************************/
extern uint16 Fingerprint_Login(uint16 FingerNum);

/***********************************************************************************************
* Function Name  : Fingerprint_GetChar
* Description	 : 读取一个指纹数据
* Input 		 : FingerID -- 存储的指纹号 
* Output		 : None
* Return		 : 0x01--获取成功, 其它 --获取失败, 
* Note  		 :  
***********************************************************************************************/
extern uchar8 Fingerprint_GetChar(uint16 FingerID);


#endif
/*========================File end=======================================================*/

