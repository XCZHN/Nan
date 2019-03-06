/*===============================================================================================
==                                                                                              
**  Copyright(c) 2014  ZhenYao Science and technology co., LTD.   All rights reserved.
==                                                                                           
**	Author:		Asen                                                                     
==                                                                                          
**  Version:	v2.0.0                                                                          
==
**  Filename:	App_KeyUI.c                                                                       
==                                                                                         
**  Function:	键盘操作界面程序
==
**  Data:       2014/08/11                                                                       
================================================================================================*/

#ifndef	_APP_KEY_INTERFACE_H_
#define	_APP_KEY_INTERFACE_H_

#include "Config.h"   

typedef struct
{
	uchar8 bHaveKey;			//有按键标记
	uchar8 u8KeyValue;			//检测到的键值
	uchar8 u8Box;			    //选中的箱号
	uchar8 u8NumValue;			//检测到的数字键键值
	uchar8 u8KeyCount;			//按键数统计
	uchar8 u8TempValue;         //临时值
	uchar8 u8ValueType;         //临时值类型
	uchar8 u8Select;			//在菜单中选择的行号
	uchar8 u8InputType;
	uchar8 bAddInto;			//进入按键菜单标记
	uchar8 u8AdminMenuStep;		//记录主菜单轨迹
	uchar8 u8SecondMenuStep;	//记录二级菜单轨迹
	uchar8 u8ThreeMenuStep;	    //记录三级菜单轨迹
	uchar8 u8FourMenuStep;	    //记录四级菜单轨迹
	uchar8 u8FiveMenuStep;	    //记录五级菜单轨迹

	uchar8 IpBuf[6];
	uchar8 KeyBuf[16];
	uchar8 tempBuf[20];

	uint16 u16TouchID;          //指纹号

	uint32 iCardUID;            //卡号
	uint32 iTempValue;          //临时值
	uint32 iLastKeyTime;        //上次按键时间
	
}ST_KEYBOARD;


extern	volatile ST_KEYBOARD gstKeyboard;


/***********************************************************************************************
* Function Name  : Admin_Login
* Description	 : 用户帐号登录操作
* Input 		 : 0--按键操作, 0xCC--刷卡进入
* Output		 : None
* Return		 : None 
* Notes          : 按键流程: #-->*-->000-->Enter-->PWD
                   刷卡流程: Card-->Enter-->PWD
***********************************************************************************************/
extern void Admin_Login(uchar8 Type);

/***********************************************************************************************
* Function Name  : KeyboardFunction
* Description	 : 键盘操作程序
* Input 		 : None
* Output		 : None
* Return		 : None 
* Others		 : None
***********************************************************************************************/
extern void  KeyboardFunction(void);

/***********************************************************************************************
* Function Name  : DepositKeyDown
* Description	 : 寄存键按下
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : 系统空闲状态下才扫描[寄存]独立按键
***********************************************************************************************/
extern void DepositKeyDown(void);

#endif  //_APP_KEY_INTERFACE_H_
/*===================================File end===================================================*/


