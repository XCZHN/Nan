/*===============================================================================================
==                                                                                              
**  Copyright(c) 2017  ZhenYao. All rights reserved.
==                                                                                           
**	Author:		Asen                                                                        
==                                                                                          
**  Version:	v1.0.0                                                                          
==
**  Filename:	app_Notice.h                                                                       
==                                                                                         
**  Function:	提示信息表(英文版) 
==                                                                                         
**  Data:       2017/06/02                                                                       
================================================================================================*/

#ifndef _NOTICE_H_
#define _NOTICE_H_


#include "Config.h"


const char *pStr_Company = "Zhenyao Technology Co., Ltd.";
const char *pStr_Phone = "www.szzhenyao.com";
const char *pStr_SysVersion = "SysVersion";
	
const char *pStr_SetOK = "Settings OK!";
const char *pStr_SetFailed = "Settings failed!";
const char *pStr_PasswordError = "Wrong password!";
const char *pStr_AskSysInit = "确定要初始化系统?";
const char *pStr_SysInitWait = "正在初始化中...";
const char *pStr_SysInitOK = "系统初始化成功!";
const char *pStr_BoxIdErr = "Box ID Wrong!";
const char *pStr_PlsWait = "Pls wait...";
const char *pStr_AskClrCard = "Confirm del card?";
const char *pStr_ClrCardOk = "Del card ok!";
const char *pStr_AskClrBox = "Confirm Clean ?";
const char *pStr_AskClrAllBox = "Confirm Clean all?";
const char *pStr_OpeningBox = "Opening box...";
const char *pStr_LockingBoxOk = "Lock box OK!";
const char *pStr_UnlockingBoxOk = "Unlock box ok!";

const char *pStr_OperationTip = "Operation Prompt";
const char *pStr_Exit = "0.Exit";
const char *pStr_InputType = "Current input method:";
const char *pStr_PlsInput = "Pls Enter";
const char *pStr_PlsInputPsw = "Pls Enter password.";
const char *pStr_PlsEnterPwd = "Pls enter repair pwd:";
const char *pStr_PlsInputBoxID = "Enter box no.";
const char *pStr_Notice255IsAll = "Enter 255 to select all boxes.";
const char *pStr_PlsInputMachineNum = "Pls enter Lockers ID";
const char *pStr_Old = "Old:";
const char *pStr_New = "New:";
const char *pStr_PlsEnterName = "Pls enter lockers name:";
const char *pStr_PlsEnterIdCode = "Pls enter Identif code:";
const char *pStr_PlsCard = "Swipe card!";
const char *pStr_PlsCardAgain = "Swipe card again!";
const char *pStr_PlsEnterValidity = "请输入使用期限:";
const char *pStr_PlsChoiceType = "请选择类型!";
const char *pStr_WithoutBoxBeLock = "No locked box!";
const char *pStr_QueryLockBox ="Query locked boxes";
const char *pStr_QueryBoxStatus = "Query box status";
const char *pStr_QueryPrompt = "1.Previous  2.Next ";
const char *pStr_BoxStatus[] = {"Closed","Opened","Empty","Occupied"};

const char *pStr_NxBox = "----";
const char *pStr_Open1Box = "is opened.";
const char *pStr_PlsCloseBox = "Don't forget close the door!";
const char *pStr_StopOpenBox = "Press <#> to stop!";
const char *pStr_AskOpenAllBox = "Full open?";
const char *pStr_ClrBoxNotice = "Don't forget close zhe door!";
const char *pStr_Locking1Box = "Confirm lock this box?";
const char *pStr_LockinAllBox = "Confirm lock all box?";
const char *pStr_Unlocking1Box = "Confirm unlock boa?";
const char *pStr_UnlockingAllBox = "Confirm unlock all box?";
const char *pStr_BoxBeLocked = "Box is locked!";

//-------------寄存柜存取操作提示信息-----------------------------
const char *pStr_NowEmptyBoxNum = "Empty lockers box: ";
const char *pStr_SysError = "ERROR:System failure!";
const char *pStr_AdminCard = "Manage Card!";
const char *pStr_CardNotConsign = "Not deposited!";
const char *pStr_CardNotRegister = "Not authorized!";
const char *pStr_NotExtractTime = "Non-pickup time!";
const char *pStr_OpenBoxFaild = "Fail to open the door!";
const char *pStr_xBoxOpenFail = "Fail to open!";
const char *pStr_PlsCallAdmin = "Pls contact admin.";
const char *pStr_LockersIsFull = "IS FULL !";
const char *pStr_IdPosition = "Your items in:";
const char *pStr_xLockers = "Locker";
const char *pStr_ThisLockers = "In ";
const char *pStr_OnOtherLockers = "Other Locker!";
const char *pStr_xBox = "(Box)!";
const char *pStr_PlsCardToPickUp = "Directly swipe card!";
const char *pStr_xBoxNotClose = "is not closed.";
const char *pStr_PlsCloseThisBox = "Pls close it!";
const char *pStr_PlsConsignAgain = "Pls re-deposit.";
const char *pStr_Overdue = "Card overdue!";
const char *pStr_NonSysCard = "Not Authorized!";
const char *pStr_LimitOfUse = "Restricted Area!";
const char *pStr_PlsGotoOtherLockers = "Pls deposit in other cabinets!";
const char *pStr_WriteCardFaild = "Fail to write card!";
const char *pStr_ConsignError = "Last exception access!";
const char *pStr_RestoreCard = "Card has been repaired.";
const char *pStr_PlsCloseBoxDoor = "Pls close door!";

const char *pTitleStr_CloseTimeout = "Close door timeover";
const char *pTitleStr_NoticeTime = "Alarm to keep time";

const char *pMenu_Week[]={
	"Sunday",
	"Monday",
	"Tuesday",
	"Wednesday",
	"Thursday",
	"Friday",
	"Saturday"
};

const char *pMenu_KeyType[]={
	"设置键盘类型",
    "1.触摸键盘",
	"2.金属键盘",
	"请选择键盘类型!"
};

const char *pMenu_SetDebugMode[]={
	"调试开关",
	"当前状态: ",
	"1.开启  ",
	"2.关闭  "
};

const char *pMenu_SetCheckDoorSwitch[]={
	"箱门状态检测开关",
	"当前状态: ",
	"1.开启  ",
	"2.关闭  "
};

const char *pMenu_WorkMode[]={
	"设置工作模式",
	"当前模式: ",
	"1.任意IC卡 ",
	"2.预授权卡 ",
	"3.写卡模式",
	"4.在线验证"
};

const char *pMenu_SetBoxNum[]={
	"箱数设置",
	"1.主板箱数",
	"2.副板箱数",
	
	"1.副板(1)箱数",
	"2.副板(2)箱数",
	"3.副板(3)箱数",
	""
};

const char *pMenu_Registration[]={
	"自动修复",
	"正在检测系统...",
	"检测到数据异常...",
	"正在修复数据...",
	"修复成功!"
};

const char *pMenu_SetTime[]={
	"Time settings",
	"Date: ",
	"Time: "
};

const char *pMenu_VoiceLevel[]={
	"Voice settings",
	"Enter Volume(0-30):"
};

const char *pMenu_SetNetSwitch[]={
	"Network switch",
	"Current state: ",
	"1.ON  ",
	"2.OFF "
};

const char *pMenu_TCPIP[]={
    "Network settings",
	"1.Local IP",
	"2.Server IP",
	"3.Server port"
};

const char *pMenu_ChoiceAdmin[]={
	"Change Password",
	"1.超级管理员",
	"2.高级管理员",
	"3.用户管理员"
};

const char *pMenu_CardAccredit[]={
	"Card authorization",
	"Add admin card",
	"current box ID:",
	"Pls swipe card!",
	"<OK>",
	"<Full>",
	"Press<#>switch to next boxNo."
};

const char *pMenu_AdminCardOpertion[]={
	"Admin Card operation",
	"1.Add admin card.",
	"2.Del admin card."
};

const char *pMenu_RecordQuery[]={
	"Record query",
	"box record",	
	"No such a box no.!",
	"No record!"
};

const char *pMenu_OpenBoxType[]={
	"Box opening type",
	"Admin",
	"Password",
	"Card",
	"Card+Pwd",
	"Fingerprint",
	"Net cmd",
	"Unknown"
};

const char *pAccessStatus[]={
	"Consign status",
	" Consign",
	" Take",
	" ****"
};

const char *pMenu_CardSector[]={
	"Card sector Settings",
	"Pls Set the card sector!",
	"Current:",
	"New Value:"
};

const char *pMenu_AllowOpenBoxNum[]={
	"Consign settings",
	"Allow opend boxes when consign.",
	"Current:",
	"Enter value:"
};

const char *pMenu_PickUpAwaitTime[]={
	"Min consign time set",
	"Current(S):",
	"Enter value(S):"
};

const char *pMenu_PickUpTimeout[]={
	"Consign timeover set",
	"Current(S):",
	"Enter value(S):"
};

const char *pMenu_SetUIntValue[]={
	"Parameter Settings",
	"Current(S):",
	"Enter value(S):"
};

//--------------用户管理员操作界面-----------------------------
const char *pMenu_Admin[]={
	"Administrator menu",
	"1.System",
	"2.Manage",
	"3.Door",
	"4.Consign",
	"5.Query",
	"0.Exit"
};

const char *pMenu_Admin_SysSet[]={
	"System settings",
	"1.Mode",
	"2.Clock",
	"3.Voice",
	"4.ID",
	"5.Network",
	"6.TCP/IP",
	"7.Repair",
	"0.Exit"
};

const char *pMenu_Admin_Manage[]={
	"Management settings",
	"1.Password.",
	"2.Admin Card.",
	"3.Lockers Name.",
	"4.Identification code.",
	"5.Card sector.",
	"6.Add Valid Card.",
	"7.Clean all Valid Card.",
	"0.Exit."
};

const char *pMenu_Admin_Box[]={
	"Lockers door operating",
	"1.Open a locker door.",
	"2.Open all locker door.",
	"3.Clear a locker box.",
	"4.Clear all locker box.",
	"5.Lock a locker box.",
	"6.Unlock a locker box.",
};

const char *pMenu_Admin_Lockers[]={
	"Consign Settings",
	"1.Allow opening boxes.",
	"2.Close the door overtime.",
	"3.Minimum storage time.",
	"4.Closing door hints."
};

const char *pMenu_Admin_Query[]={
	"Query menu",
	"1.Opening box record.",
	"2.Version.",
	"3.Lockers ID",
	"4.Lockers Name",
	"5.Identification Code",
	"6.Locked box.",
	"7.Lockers box status."
};

//--------------高级管理员操作界面-----------------------------
const char *pMenu_TopAdmin[]={
	"高管设置菜单",
	"1.设置",
	"0.退出"
};

const char *pMenu_TopAdmin_Set[]={
	"设置菜单",
	"1.管密",
	"2.管理卡",
	"3.模式",
	"4.调试",
	"5.开门检",
	"0.退出"
};

//--------------超级管理员操作界面-----------------------------
const char *pMenu_SuperAdmin_Set[]={
	"设置菜单",
	"1.管密",
	"2.期限",
	"3.键盘",
	"4.初始化",
	"5.箱数",
	"0.退出"
};

#endif
/*========================File end===================================================================================================*/

