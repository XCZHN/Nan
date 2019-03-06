/*===============================================================================================
==                                                                                              
**  Copyright(c) 2014  Intelligent Admin electronic systems co., LTD. All rights reserved.
==                                                                                           
**	Author:		Asen                                                                           
==                                                                                          
**  Version:	v1.0.0                                                                          
==
**  Filename:	Lockers.h                                                                       
==                                                                                         
**  Function:	储物箱处理程序 
==                                                                                         
**  Data:       2015/09/18                                                                       
================================================================================================*/

#ifndef	_LOCKERS_H_
#define	_LOCKERS_H_


#include "Includes.h"


/***********************************************************************************************
* Function Name  : CheckEmptyBoxNum
* Description	 : 检查空箱数
* Input 		 : None
* Output		 : None
* Return		 : 空箱数
* Note  		 :  
***********************************************************************************************/
extern uchar8 CheckEmptyBoxNum(void);

/***********************************************************************************************
* Function Name  : ScanAllBoxStatus
* Description	 : 扫描所有箱门状态
* Input 		 : void
* Output		 : None
* Return		 : None
* Note  		 : 包括扩展箱柜的箱门状态
***********************************************************************************************/
extern void ScanAllBoxStatus(void);

/***********************************************************************************************
* Function Name  : SelectOneBoxToOpen
* Description	 : 选择开启某个箱门
* Input 		 : boxId -- 待开启的箱门号, bCheckResult:0不检测开门结果,非0则检测开门结果
* Output		 : None
* Return		 : 执行结果 0--开箱成功, 其它--开箱失败
* Notes          : 自动判断是在主柜还是副柜上
***********************************************************************************************/
extern uchar8 SelectOneBoxToOpen(uint8 boxId, uchar8 bCheckResult);

/***********************************************************************************************
* Function Name  : HandleTimeoutAlert
* Description	 : 寄存关门超时提醒
* Input 		 : 
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void HandleTimeoutAlert(void);

/***********************************************************************************************
* Function Name  : UpdateEmptyBoxList
* Description	 : 更新空箱列表
* Input 		 : None
* Output		 : None
* Return		 : None
* Note  		 : 将空箱重新装载到空箱列表中 
***********************************************************************************************/
extern void UpdateEmptyBoxList(void);

/***********************************************************************************************
* Function Name  : AddOneEmptyBox
* Description	 : 往空箱队列末尾增加一个空箱箱号
* Input 		 : box--箱号
* Output		 : None
* Return		 : None
* Note  		 : None 
***********************************************************************************************/
extern void AddOneEmptyBox(uchar8 box);

/***********************************************************************************************
* Function Name  : AddOneEmptyBox
* Description	 : 往空箱队列中删除一个空箱箱号
* Input 		 : box--箱号
* Output		 : None
* Return		 : None
* Note  		 : None 
***********************************************************************************************/
extern void DeleteOneEmptyBox(uchar8 box);


/***********************************************************************************************
* Function Name  : CheckBoxStatusChange
* Description	 : 检查箱门\箱内状态改变
* Input 		 : None
* Output		 : None
* Return		 : None
* Note  		 : 当检查到有改变时立即上传至服务器
***********************************************************************************************/
extern void CheckBoxStatusChange(void);

/***********************************************************************************************
* Function Name  : CheckValidityDate
* Description	 : 检查某个日期的有效性
* Input 		 : pDate -- 日期
* Output		 : None
* Return		 : 0x00 -- 未设置有效期, 其它为有效期限(天),小于0表示已过期天数
***********************************************************************************************/
extern int	CheckValidityDate(uchar8* pDate);


/***********************************************************************************************
* Function Name  : VoicePlay_DoorNotClose
* Description	 : 播放"xx号箱门未关,请关门!"
* Input 		 : u8DoorNum -- 门号
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void VoicePlay_DoorNotClose(uchar8 u8DoorNum);

/***********************************************************************************************
* Function Name  : VoicePlay_xBoxOpen
* Description	 : 播放"xx号箱门已开!"
* Input 		 : u8Box -- 箱门号
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void VoicePlay_xBoxOpen(uchar8 u8Box);


#endif
/*========================File end======================================================================*/

