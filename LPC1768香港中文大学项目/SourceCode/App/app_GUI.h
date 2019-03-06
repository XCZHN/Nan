/*===============================================================================================
==                                                                                              
**  Copyright(c) 2014  ZhenYao Science and technology co., LTD.   All rights reserved.
==                                                                                           
**	Author:		Asen                                                                     
==                                                                                          
**  Version:	v1.0.0                                                                          
==
**  Filename:	App_GUI.c                                                                       
==                                                                                         
**  Function:	显示界面程序
==
**  Data:       2015/10/20                                                                        
================================================================================================*/

#ifndef	_APP_GUI_H_
#define	_APP_GUI_H_


#include "Includes.h"

//---------------------------LCD-------------------------------------------------------
#define LCD_WIDE            320
#define LCD_HIGH            240

#define LCD_WIDE_START      319
#define LCD_HIGH_START      0
#define LCD_WIDE_END        0
#define LCD_HIGH_END        239

#define FONT_SIZE08          8
#define FONT_SIZE16          16
#define FONT_SIZE24          24
#define FONT_SIZE32          32
#define FONT_SIZE64          64

#define HALF_PHOTO_HIGH      0x25    
#define HALF_PHOTO_WIDE      0x117   

/*
#define FONT_16X16_POINT     32 
*/

//---------------------------LCD color------------------------------------------------
#define LCD_Color_White          		0xFFFF
#define LCD_Color_Black          		0x0000
#define LCD_Color_Blue           		0x001F
#define LCD_Color_Bread          		0xF81F
#define LCD_Color_Grey           		0xF7DE
#define LCD_Color_Red            		0xF800
#define LCD_Color_Magenta        		0xF81F
#define LCD_Color_Green          		0x07E0
#define LCD_Color_Cyan           		0x7FFF
#define LCD_Color_Yellow         		0xFFE0

#define LCD_Color_DarkBlue       		0x01CF //深蓝色
#define LCD_Color_LightBlue      		0x7D7C //浅蓝色  
#define LCD_Color_GrayBlue       		0x5458 //灰蓝色
#define LCD_Color_Back           		0xdefb //背景色

//---------------------------LCD Pic------------------------------------------------
#define ICON_Teacup                     1      //茶杯       
#define ICON_NetBll                     2      //网状球                     
#define ICON_ExclamationMark            3      //(圈+!)
#define ICON_Hospital                   4      //医院图标        
#define ICON_Lockers                    5      //箱子                           
#define ICON_Microphone            		7      //麦克风
#define ICON_DateList                   8      //日历表 
#define ICON_House                 	    9      //房子
#define ICON_RecycleBin                 10     //回收站          
#define ICON_Loudspeaker                11     //喇叭           
#define ICON_Gear                       13     //齿轮     
#define ICON_Clock                      14     //时钟                           
#define ICON_ZY                         15     //(ZY)
#define ICON_Find                       16     //放大镜                           
#define ICON_Ascii               		17     //字母("A")
#define ICON_Lock                       19     //锁                                    
#define ICON_RotationLock               20     //旋转锁
#define ICON_Wifi               		21     //WIFI
#define ICON_Car                        22     //卡车
#define ICON_Bulb                       23     //灯泡
#define ICON_Airplane                   24     //飞机

#define ICONS_001                       0      //菜单式图标(包含茶杯\网球\医院图标\感叹号)
#define ICONS_002                       6      //菜单式图标(包含麦克风\日历\喇叭\房子)      
#define ICONS_003                       12     //菜单式图标(包含ZY\放大镜\齿轮\时钟)             
#define ICONS_004                       18     //菜单式图标(包含锁\WIFI\卡车)             


#define PHOTO_ZhenYaoTel         		0x64   //ZY主界面(带客服电话)
#define PHOTO_Desktop            		0x65   //主界面图片

#define HALF_PHOTO_ZNXG          		2      //欢迎使用智能箱柜
#define HALF_PHOTO_PlsCard       		5      //开箱门请刷卡
#define HALF_PHOTO_PlsPwd        		0x68   //开箱门请输入密码
#define HALF_PHOTO_ZNJCG         		0x69   //欢迎使用智能寄存柜
#define HALF_PHOTO_StorePlsKey        	3      //存物请按"寄存"键 
#define HALF_PHOTO_FetchByCard   		5      //取物请读卡
#define HALF_PHOTO_FetchByFingerprint   3      //取物请按指纹       
#define HALF_PHOTO_ZNKDG                0x6D   //欢迎使用智能快递柜
#define HALF_PHOTO_DeliverByCard        0x6E   //投递请刷卡
#define HALF_PHOTO_ZNXBX                0x6F   //欢迎使用智能信报箱
#define HALF_PHOTO_OpenBoxByCard        5      //开箱门请读卡
#define HALF_PHOTO_OpenBoxByPwd         0x71   //开箱门请输入密码
#define HALF_PHOTO_CardOrPwd            0x72   //读卡或输入密码开箱
#define HALF_PHOTO_ZNYBG                0x73   //欢迎使用智能邮包柜
#define HALF_PHOTO_FetchByPwd           0x74   //取件请输入密码
#define HALF_PHOTO_JICUN_CARD           0x75   //取件请刷卡

/*add by zenghl*/
#define Main_UI_Num                     10
#define UNIT_UI_Num                     15
#define HALF_PHOTO_Xsize				43     //提示框 X方向大小 
#define HALF_PHOTO_Ysize				279    //提示框 Y方向大小 
#define HALF_PHOTO_X_1					7      //提示框起始 X坐标
#define HALF_PHOTO_Y_1					20     //提示框起始 Y坐标
#define HALF_PHOTO_X_2					120    //提示框起始 X坐标
#define HALF_PHOTO_Y_2					20     //提示框起始 Y坐标

#define HALF_ICON_Xsize					48     //图标 X方向大小 
#define HALF_ICON_Ysize					48     //图标 Y方向大小 

//---------------------------LCD Type------------------------------------------------

#define LCD_TYPE_A32LCD 		      0x32	   //3.2寸LCD 	


typedef struct
{
	unsigned short s_x,s_y;
	unsigned short e_x,e_y;
	unsigned short FColor;
	unsigned short BColor;
	unsigned char Font;
	char Ascii;
	unsigned char PicNum;
	char *pStr;
	char TxtBuff[256];
	
}LCM_STR_UI;


//---------------------------Message------------------------------------------------
typedef enum 
{
	NOTICE_SET_OK          = 0x01,   //提示信息--设置成功
	NOTICE_SET_FAIL,                 //提示信息--设置失败
	NOTICE_PWD_ERR,                  //提示信息--密码错误
	NOTICE_SYS_INIT_ASK,             //提示信息--初始化确认
	NOTICE_SYS_INIT_WAIT,            //提示信息--初始化等待
	NOTICE_SYS_INIT_OK,              //提示信息--初始化完成
	NOTICE_BOX_ID_ERR,               //提示信息--箱号错误
	NOTICE_PLS_WAIT,                 //提示信息--请稍等
    NOTICE_ASK_OPEN_ALL_BOX,         //提示信息--确认全开
    NOTICE_ASK_CLR_CARD,             //提示信息--清卡确认       
    NOTICE_ASK_CLR_CARD_OK,          //提示信息--清卡完成   
    NOTICE_ASK_CLR_BOX,              //提示信息--清箱确认
    NOTICE_ASK_CLR_ALLBOX,           //提示信息--清所有箱确认
    NOTICE_OPENING_BOX,              //提示信息--正在开箱
    NOTICE_LOCKING_BOX_OK,           //提示信息--锁箱成功
    NOTICE_UNLOCKING_BOX_OK,         //提示信息--解除锁箱成功

}NOTICE;

//---------------------------寄存柜存取提示信息类型表------------------------------------------------
typedef enum 
{
	LOCKERS_SysError,               //系统故障
	LOCKERS_AdminCard,              //管理卡
	LOCKERS_CardNotConsign,         //此未寄存
	LOCKERS_NotExtractTime,         //未到取件时间
	LOCKERS_OpenBoxFaild,           //开箱失败
	LOCKERS_PlsCard,                //请刷卡
	LOCKERS_IsFull,                 //本柜已满
	LOCKERS_OpeningBox,             //正在开箱
	LOCKERS_CardUnauthorized,       //卡未授权
	LOCKERS_NonSysCard,             //非本系统卡  
	LOCKERS_Overdue,                //已过使用期限
	LOCKERS_LimitOfUse,             //受限区域,请到其它柜上寄存
	LOCKERS_WriteCardFaild,         //写卡失败
	LOCKERS_RepairCard,             //上次开箱失败,异常寄存,修复卡数据
	LOCKERS_ClrCardStoreInfo,       //(开箱失败)清除卡上的寄存信息
	LOCKERS_PlsCloseBoxDoor,        //请关好箱门 
	LOCKERS_PlsFingerOrCard,        //请按指纹或刷卡
	LOCKERS_WaitTimeout,            //等待超时
	LOCKERS_NoDeposit,              //未寄存
	LOCKERS_NetError,               //网络异常
	LOCKERS_
	
}LOCKERS_NOTICE;

extern char cStr_Message[100];
extern const char *pStr_InputPasswordHint;
extern const char *pStr_UnLockAllBox;

/*******************************************************************************************************
* Description  : 显示字符串 
********************************************************************************************************/
extern void  LCD_DisplayStr(uint16 s_x, uint16 s_y, uchar8 Font, const char* pStr, uint16 FColor);

/*******************************************************************************
* Function Name  : GUI_ShowSysVersion
* Description    : 显示系统版本
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_ShowSysVersion(void);

/*******************************************************************************
* Function Name  : GUI_ChoiceKeyboardType
* Description    : 提示选择键盘的类型
* Input          : None
* Output         : None
* Return         : None
* Notes          : None
*******************************************************************************/
extern void GUI_ChoiceKeyboardType(void);

/*******************************************************************************
* Function Name  : GUI_InputUseDate
* Description    : 显示输入某个箱的使用日期
* Input          : box -- 箱号
* Output         : None
* Return         : None
* Notes          : None
*******************************************************************************/
extern void GUI_InputUseDate(uchar8 box, uchar8* pDateStr);

/*******************************************************************************
* Function Name  : GUI_InputBoxID
* Description    : 输入箱号
* Input          : None
* Output         : None
* Return         : None
* Notes          : 在一个提示框中输入数字
*******************************************************************************/
extern void GUI_InputBoxID(uchar8 BoxID);

/*******************************************************************************
* Function Name  : GUI_AdminSetBoxNum
* Description    : 管理员设置箱数
* Input          : type -- 类型(0--待选, 1--主柜, 2--副柜1...); BoxNum -- 箱数
* Output         : None
* Return         : None
* Notes          : 在一个提示框中输入数字
*******************************************************************************/
extern void GUI_AdminSetBoxNum(uchar8 type, uchar8 BoxNum);

/*******************************************************************************
* Function Name  : GUI_Open1Box
* Description    : 开箱提示
* Input          : u8BoxNum -- 箱号  AddNote -- 附加加提示信息
* Output         : None
* Return         : None
* Notes          : AddNote : 1 -- 请随手关门
                             2 -- 按<#>停止开箱
*******************************************************************************/
extern void GUI_Open1Box(uchar8 u8BoxNum, uchar8 AddNote);

/*******************************************************************************
* Function Name  : GUI_Open1BoxFaild
* Description    : 提示开箱失败
* Input          : u8BoxNum -- 箱号  AddNote -- 附加加提示信息
* Output         : None
* Return         : None
* Notes          : AddNote : 1 -- 请随手关门
                             2 -- 按<#>停止开箱
*******************************************************************************/
extern void GUI_Open1BoxFaild(uchar8 u8BoxNum, uchar8 AddNote);

/*******************************************************************************
* Function Name  : GUI_BoxBeLocked
* Description    : 提示箱已被锁,请联系管理员
* Input          : u8BoxNum -- 箱号 
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_BoxBeLocked(uchar8 box);

/*******************************************************************************************************
* Description  : 显示时间
********************************************************************************************************/
extern void  GUI_Time(void);

/*******************************************************************************************************
* Description  : 显示主界面  
********************************************************************************************************/
extern void  GUI_Admin(void);

/*******************************************************************************
* Function Name  : GUI_UpdateDisplay(LCM_UI *ppLCM_UI,char start, char end)
* Description    : 更新界面显示
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_UpdateDisplay(LCM_STR_UI *pStrUi,char start, char end);

/*******************************************************************************
* Function Name  : GUI_ShowMessage
* Description    : 提示消息
* Input          : pstr--要显示的字符串指针, color--颜色
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_ShowMessage(const char* pstr, uint16 color);

/*******************************************************************************
* Function Name  : GUI_ShowTowRowMessage
* Description    : 提示两行消息
* Input          : pstr--要显示的字符串指针, color--颜色
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_ShowTowRowMessage(const char* pstr, const char* pstr02, uint16 color);

/*******************************************************************************
* Function Name  : GUI_SetTime
* Description    : 设置时间
* Input          : pDateStr --YY-MM-DD字符串指针 , pTimeStr--HH;MM;SS字符串指针
* Output         : None
* Return         : None
* Notes          : 在下边框显示当前的日期和时间
*******************************************************************************/
extern void GUI_SetTime(uchar8* pDateStr, uchar8* pTimeStr);

/*******************************************************************************
* Function Name  : GUI_SetDebugMode
* Description    : 调试模式开关设置
* Input          : bDebugMode -- 当前开关状态
* Output         : None
* Return         : None
* Notes          : None
*******************************************************************************/
extern void GUI_SetDebugMode(uchar8 bDebugMode);

/*******************************************************************************
* Function Name  : GUI_SetBoxDoorCheckSwitch
* Description    : 设置箱门状态检测开关
* Input          : bSwitch -- 当前开关状态
* Output         : None
* Return         : None
* Notes          : None
*******************************************************************************/
extern void GUI_SetBoxDoorCheckSwitch(uchar8 bSwitch);

/*******************************************************************************
* Function Name  : GUI_SetBoxDoorCheckSwitch
* Description    : 设置箱门状态检测开关
* Input          : bSwitch -- 当前开关状态
* Output         : None
* Return         : None
* Notes          : None
*******************************************************************************/
extern void GUI_SetIDCardSwitch(uchar8 bSwitch);

/*******************************************************************************
* Function Name  : GUI_SetWorkMode
* Description    : 工作模式设置
* Input          : mode -- 当前模式
* Output         : None
* Return         : None
* Notes          : None
*******************************************************************************/
extern void GUI_SetWorkMode(uchar8 mode);

/*******************************************************************************
* Function Name  : GUI_InputVoiceLevel
* Description    : 设置音量等级
* Input          : VoiceLevel -- 音量等级
* Output         : None
* Return         : None
* Notes          : None
*******************************************************************************/
extern void GUI_InputVoiceLevel(uchar8 VoiceLevel);

/*******************************************************************************
* Function Name  : GUI_InputMachineNum
* Description    : 输入机号
* Input          : iMachineNum -- 当前输入的机号
* Output         : None
* Return         : None
* Notes          : 在一个提示框中输入数字
*******************************************************************************/
extern void GUI_InputMachineNum(uint32 iMachineNum);

/*******************************************************************************
* Function Name  :  GUI_SetNetSwitch
* Description    : 联网授权
* Input          : NetSwitch -- 当前开关状态
* Output         : None
* Return         : None
* Notes          : None
*******************************************************************************/
extern void GUI_SetNetSwitch(uchar8 NetSwitch);

/*******************************************************************************
* Function Name  : GUI_ChoiceNetInfo
* Description    : 网络信息选择
* Input          : None
* Output         : None
* Return         : None
* Notes          : 1.本机IP 2.服务器IP 3.服务器端口号
*******************************************************************************/
extern void GUI_ChoiceNetInfo(void);

/*******************************************************************************
* Function Name  : GUI_InputIp
* Description    : 设置本机IP地址
* Input          : type -- 1.本机IP, 2.服务器IP
                   pInputIpStr--输入的IP地址(字符串格式)
* Output         : None
* Return         : None
* Notes          : 同时显示旧的IP
*******************************************************************************/
extern void GUI_InputIp(uchar8 type, uchar8* pInputIpStr);

/*******************************************************************************
* Function Name  : GUI_InputServerPort
* Description    : 设置服务器端口号
* Input          : iPort--输入的端口号
* Output         : None
* Return         : None
* Notes          : 同时显示旧的端口号
*******************************************************************************/
extern void GUI_InputServerPort(uint32 iPort);

/*******************************************************************************
* Function Name  : GUI_Registration
* Description    : 系统注册
* Input          : pInputStr--输入的字符串, InputType--当前输入法(0-数字,INPUT_TYPE_ABC-字母)
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_Registration(uchar8* pInputStr, uchar8 InputType);

/*******************************************************************************
* Function Name  : GUI_ShowNotice
* Description    : 显示提示信息
* Input          : Type--信息类型
* Output         : None
* Return         : None
* Notes          : 根据类型选择要显示的信息
*******************************************************************************/
extern void GUI_ShowNotice(uchar8 Type);


/*******************************************************************************************************
* Description  : 按工作状态显示界面
********************************************************************************************************/
extern void  GUI_WorkStatus(void);

/*******************************************************************************
* Function Name  : GUI_ShowPassword
* Description    : 提示框显示输入密码
* Input          : PsdNum -- 输入的密码位数
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_ShowPassword(uchar8 PsdNum);

/*******************************************************************************
* Function Name  : GUI_ChangePassword
* Description    : 修改高级管理员密码
* Input          : pStr_OldPassword--旧密码(ASCII格式), 新输入的密码字符串
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_ChangePassword(uchar8* pStr_OldPassword, uchar8* pInputStr);

/*******************************************************************************
* Function Name  : GUI_InputName
* Description    : 录入名称
* Input          : pInputStr--输入的字符串, InputType--当前输入法(0-数字,INPUT_TYPE_ABC-字母)
                   pNotice -- 附带提示信息
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_InputName(uchar8* pInputStr, uchar8 InputType, const char* pNotice);

/*******************************************************************************
* Function Name  : GUI_AddValidCard
* Description    : 提示小框显示--增加预授权卡
* Input          : iCardNum--卡号, addmsg--附带提示信息
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_AddValidCard(uint32 iCardNum, uchar8 addmsg);

/*******************************************************************************
* Function Name  : GUI_AddCard2Box
* Description    : 提示大框显示--授权卡
* Input          : box--箱号, iCardNum--卡号, CardCount--当前授权卡数量
* Output         : None
* Return         : None
* Notes          : 如果是授权用户卡(box=0),则在底部显示"按<#>切换至下一信箱号"
*******************************************************************************/
extern void GUI_AddCard2Box(uchar8 box, uint32 iCardNum, uchar8 CardCount);

/*******************************************************************************
* Function Name  : GUI_ClrBoxNotice
* Description    : 清箱提示信息消息
* Input          : u8BoxNum -- 箱号
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_ClrBoxNotice(uchar8 u8Box);

/*******************************************************************************
* Function Name  : GUI_AskLockinBox
* Description    : 提示--确定锁箱?
* Input          : u8BoxNum -- 箱号
* Output         : None
* Return         : None
* Notes          : MARK_CHOICE_ALL_BOX -- 代表选中所有箱号
*******************************************************************************/
extern void GUI_AskLockinBox(uchar8 u8Box);

/*******************************************************************************
* Function Name  : GUI_AskUnlockinBox
* Description    : 提示--确定解除锁X箱?
* Input          : u8BoxNum -- 箱号
* Output         : None
* Return         : None
* Notes          : MARK_CHOICE_ALL_BOX -- 代表选中所有箱号
*******************************************************************************/
extern void GUI_AskUnlockinBox(uchar8 u8Box);

/*******************************************************************************
* Function Name  : GUI_QueryBoxRecord
* Description    : 开箱记录查询
* Input          : BoxNum -- 箱号, CurrentPage -- 当前页(0--N)
* Output         : None
* Return         : None
* Notes          : 每页显示4笔开箱记录
*******************************************************************************/
extern void GUI_QueryBoxRecord(uchar8 BoxNum, uchar8 CurrentPage);

/*******************************************************************************
* Function Name  : GUI_QueryLockBoxList
* Description    : 显示锁箱列表
* Input          : CurrentPage -- 当前页(0--N)
* Output         : None
* Return         : None
* Notes          : 
*******************************************************************************/
extern void GUI_QueryLockBoxList(uchar8 CurrentPage);

/*******************************************************************************
* Function Name  : GUI_QueryBoxStatusList
* Description    : 查询箱状态列表(箱门状态以及箱内状态)
* Input          : CurrentPage -- 当前页(0--N)
* Output         : None
* Return         : None
* Notes          : 每页显示4个箱的状态
*******************************************************************************/
extern void GUI_QueryBoxStatusList(uchar8 CurrentPage);

/*******************************************************************************
* Function Name  : GUI_SetCardSector
* Description    : 设置IC卡的使用扇区号
* Input          : sector -- 输入的卡扇区号
* Output         : None
* Return         : None
* Notes          : None
*******************************************************************************/
extern void GUI_SetCardSector(uchar8 sector);

/*******************************************************************************
* Function Name  : GUI_InputAllowOpenBoxNum
* Description    : 设置寄存时允许的最大开箱数量
* Input          : boxNum -- 开箱数量
* Output         : None
* Return         : None
* Notes          : None
*******************************************************************************/
extern void GUI_InputAllowOpenBoxNum(uchar8 boxNum);

/*******************************************************************************
* Function Name  : GUI_SetPickUpAwaitTime
* Description    : 设置限时取件等待时间
* Input          : time_s -- 时间(S)
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_SetPickUpAwaitTime(uint32 time_s);

/*******************************************************************************
* Function Name  : GUI_SetPickUpTimeout
* Description    : 设置限时取件超时时间
* Input          : time_s -- 时间(S)
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_SetPickUpTimeout(uint32 time_s);

/*******************************************************************************
* Function Name  : GUI_SetUIntValue
* Description    : 设置一个参数的整型值
* Input          : pMenu -- 提示菜单, uiOldValue -- 旧值, uiNewValue -- 新值
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_SetUIntValue(const char *pMenu, uint32 uiOldValue, uint32 uiNewValue);

/*******************************************************************************
* Function Name  : GUI_ChoiceAdminMenu
* Description    : 选择管理等级菜单
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_ChoiceAdminMenu(void);

/*******************************************************************************
* Function Name  : GUI_AdminCardOpertion
* Description    : 管理员登录-->(查询)菜单界面
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_AdminCardOpertion(void);

/*******************************************************************************
* Function Name  : GUI_AdminMenu
* Description    : 用户管理员登录主界面
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_AdminMenu(void);

/*******************************************************************************
* Function Name  : GUI_Admin_SysSetMenu
* Description    : 管理员登录-->(系统)菜单界面
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_Admin_SysSetMenu(void);

/*******************************************************************************
* Function Name  : GUI_Admin_ManageMenu
* Description    : 管理员登录-->(管理)菜单界面
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_Admin_ManageMenu(void);

/*******************************************************************************
* Function Name  : GUI_Admin_BoxMenu
* Description    : 管理员登录-->(箱门)菜单界面
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_Admin_BoxMenu(void);

/*******************************************************************************
* Function Name  : GUI_Admin_LockersMenu
* Description    : 管理员登录-->(寄存)菜单界面
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_Admin_LockersMenu(void);

/*******************************************************************************
* Function Name  : GUI_Admin_QueryMenu
* Description    : 管理员登录-->(查询)菜单界面
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_Admin_QueryMenu(void);

/*******************************************************************************
* Function Name  : GUI_TopAdminMenu
* Description    : 高级管理员登录-->(系统)菜单界面
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_TopAdminMenu(void);

/*******************************************************************************
* Function Name  : GUI_TopAdmin_SysSetMenu
* Description    : 高级管理员登录-->(系统)菜单界面
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_TopAdmin_SysSetMenu(void);

/*******************************************************************************
* Function Name  : GUI_SuperAdmin_SysSetMenu
* Description    : 超级管理员登录-->(系统)菜单界面
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_SuperAdmin_SysSetMenu(void);

/*******************************************************************************
* Function Name  : GUI_LockersNotice
* Description    : 寄存柜存取提示信息
* Input          : Type--信息类型
* Output         : None
* Return         : None
* Notes          : 根据类型选择要显示的信息
*******************************************************************************/
extern void GUI_LockersNotice(uchar8 Type);

/*******************************************************************************
* Function Name  : GUI_PlsCloseBoxDoor
* Description    : 提示箱未关,请先关好箱门
* Input          : box -- 开启的箱号 
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_PlsCloseBoxDoor(uchar8 box);

/*******************************************************************************
* Function Name  : GUI_UserConsignFail
* Description    : 提示开箱失败,请重新寄存
* Input          : box -- 开启失败的箱号 
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_UserConsignFail(uchar8 box);

/*******************************************************************************
* Function Name  : GUI_UserTakeAwwyFail
* Description    : 提示用户取件开箱失败,请联系管理员
* Input          : box -- 开启失败的箱号 
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_UserTakeAwwyFail(uchar8 box);

/*******************************************************************************
* Function Name  : GUI_ShowIDPosition
* Description    : 提示用户的物品在Nx号柜的Nx号箱
* Input          : u8Locker--柜号, u8Box--箱号,
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_ShowIDPosition(uchar8 u8Locker, uchar8 u8Box);

/*******************************************************************************
* Function Name  : GUI_PackageInOtherLockers
* Description    : 提示用户的物品别的箱上
* Input          : pLockersNmae--柜名, u8Box--箱号,
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_PackageInOtherLockers(const char* pLockersNmae, uchar8 u8Box);

/*******************************************************************************
* Function Name  : GUI_LockersParameters
* Description    : 显示箱柜参数
* Input          : void
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_LockersParameters(void);

/*******************************************************************************
* Function Name  : GUI_NetParameters
* Description    : 显示箱柜网络参数
* Input          : void
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_NetParameters(void);

/*******************************************************************************
* Function Name  : GUI_UserAccess
* Description    : 用户寄存物--提示框显示
* Input          : Type---内容标号
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_UserAccess(uint32 Type);

/*===================================File end===================================================*/ 
#endif  //_APP_GUI_H_
