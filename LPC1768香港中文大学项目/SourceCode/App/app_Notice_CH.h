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
**  Function:	提示信息表(中文汉字) 
==                                                                                         
**  Data:       2017/06/02                                                                       
================================================================================================*/

#ifndef _NOTICE_H_
#define _NOTICE_H_


#include "Config.h"


const char *pStr_Company = "深圳市振耀科技有限公司";
const char *pStr_Phone = "400-886-3136";
const char *pStr_SysVersion = "系统版本";
	
const char *pStr_SetOK = "设置成功!";
const char *pStr_SetFailed = "设置失败!";
const char *pStr_PasswordError = "密码错误!";
const char *pStr_AskSysInit = "确定要初始化系统?";
const char *pStr_SysInitWait = "正在初始化中...";
const char *pStr_SysInitOK = "系统初始化成功!";
const char *pStr_BoxIdErr = "箱号错误!";
const char *pStr_PlsWait = "请稍等...";
const char *pStr_AskClrCard = "确定清除已授权卡?";
const char *pStr_ClrCardOk = "清卡完成!";
const char *pStr_AskClrBox = "确定清空此箱?";
const char *pStr_AskClrAllBox = "确定清空所有箱?";
const char *pStr_OpeningBox = "正在开箱...";
const char *pStr_LockingBoxOk = "锁箱成功!";
const char *pStr_UnlockingBoxOk = "解除锁箱成功!";

const char *pStr_OperationTip = "操作提示";
const char *pStr_Exit = "0.退出";
const char *pStr_InputType = "当前输入法:";
const char *pStr_PlsInput = "请输入";
const char *pStr_PlsInputPsw = "请输入密码";
const char *pStr_PlsEnterPwd = "请输入维修密码:";
const char *pStr_PlsInputBoxNum = "请输入箱数";
const char *pStr_PlsInputBoxID = "请输入箱号";
const char *pStr_Notice255IsAll = "输入255则选中所有箱号.";
const char *pStr_PlsInputMachineNum = "请输入机号";
const char *pStr_Old = "旧:";
const char *pStr_New = "新:";
const char *pStr_PlsEnterName = "请输入本柜名称:";
const char *pStr_PlsEnterIdCode = "请输入系统标识码:";
const char *pStr_PlsCard = "请刷卡!";
const char *pStr_PlsCardAgain = "请再次刷卡!";
const char *pStr_PlsEnterValidity = "请输入使用期限:";
const char *pStr_PlsChoiceType = "请选择类型!";
const char *pStr_WithoutBoxBeLock = "无锁箱";
const char *pStr_QueryLockBox ="锁箱查询(已锁箱门)";
const char *pStr_QueryBoxStatus = "箱状态查询";
const char *pStr_QueryPrompt = "1.上一页   2.下一页";
const char *pStr_BoxStatus[] = {"门关","门开","无物","有物"};

const char *pStr_NxBox = "号箱";
const char *pStr_Open1Box = "箱门已开.";
const char *pStr_PlsCloseBox = "请随手关门!";
const char *pStr_StopOpenBox = "按<#>停止开箱!";
const char *pStr_AskOpenAllBox = "确认全开?";
const char *pStr_ClrBoxNotice = "清理箱内物品后请关好箱门!";
const char *pStr_Locking1Box = "确定封锁此箱?";
const char *pStr_LockinAllBox = "确定封锁所有箱?";
const char *pStr_Unlocking1Box = "确定解封此箱?";
const char *pStr_UnlockingAllBox = "确定解封所有箱?";
const char *pStr_BoxBeLocked = "箱已被锁";

//-------------寄存柜存取操作提示信息-----------------------------
const char *pStr_NowEmptyBoxNum = "当前空箱数: ";
const char *pStr_SysError = "ERROR:系统故障!";
const char *pStr_AdminCard = "管理卡!";
const char *pStr_CardNotConsign = "此卡未寄存!";
const char *pStr_CardNotRegister = "此卡未授权!";
const char *pStr_NotExtractTime = "未到取件时间!";
const char *pStr_OpenBoxFaild = "开箱失败!";
const char *pStr_xBoxOpenFail = "号箱门开启失败!";
const char *pStr_PlsCallAdmin = "请联系管理员.";
const char *pStr_LockersIsFull = "本柜已存满!";
const char *pStr_IdPosition = "您的物品在:";
const char *pStr_xLockers = "柜";
const char *pStr_ThisLockers = "本柜";
const char *pStr_OnOtherLockers = "其它箱柜中!";
const char *pStr_xBox = "号箱!";
const char *pStr_PlsCardToPickUp = "请直接刷卡取件!";
const char *pStr_xBoxNotClose = "号箱门未关.";
const char *pStr_PlsCloseThisBox = "请先关好此箱门!";
const char *pStr_PlsConsignAgain = "请重新寄存.";
const char *pStr_Overdue = "卡已过期!";
const char *pStr_NonSysCard = "非本系统授权卡!";
const char *pStr_LimitOfUse = "受限区域!";
const char *pStr_PlsGotoOtherLockers = "请到其它柜寄存!";
const char *pStr_WriteCardFaild = "写卡失败!";
const char *pStr_ConsignError = "上次异常存取!";
const char *pStr_RestoreCard = "卡已修复,可继续寄存.";
const char *pStr_PlsCloseBoxDoor = "请关好箱门!";
const char *pStr_PlsFingerOrCard = "请按指纹或刷卡!";
const char *pStr_WaitTimeout = "等待超时!";
const char *pStr_NoDeposit = "此手指未寄存物品!";
const char *pStr_NetError = "网络异常!";

const char *pTitleStr_CloseTimeout = "关门超时时间设置";
const char *pTitleStr_NoticeTime = "关门警报维持时间";

const char *pMenu_Week[]={
	"星期日",
	"星期一",
	"星期二",
	"星期三",
	"星期四",
	"星期五",
	"星期六"
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
	"1.任一IC卡 ",
	"2.预授权卡 ",
	"3.写卡模式",
	"4.在线验证",
	"5.任一指纹",
	"6.任一指纹或任一卡",
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
	"时间设置",
	"日期: ",
	"时间: "
};

const char *pMenu_VoiceLevel[]={
	"语音设置",
	"请输入音量(0-30):"
};

const char *pMenu_SetNetSwitch[]={
	"网络开关",
	"当前状态: ",
	"1.开启  ",
	"2.关闭  "
};

const char *pMenu_IDCaardSwitch[]={
	"ID卡读卡器开关",
	"当前: ",
	"1.开启  ",
	"2.关闭  "
};

const char *pMenu_TCPIP[]={
    "网络设置",
	"1.本机IP地址",
	"2.服务器IP地址",
	"3.服务器端口号"
};

const char *pMenu_ChoiceAdmin[]={
	"密码修改",
	"0.超级管理员",
	"1.用户管理员",
	"2.高级管理员",
};

const char *pMenu_CardAccredit[]={
	"卡授权",
	"授权管理卡",
	"当前箱号:",
	"请读卡!",
	"<已授权>",
	"<已满>",
	"按<#>切换至下一箱号."
};

const char *pMenu_AdminCardOpertion[]={
	"管理卡操作",
	"1.授卡",
	"2.删卡"
};

const char *pMenu_RecordQuery[]={
	"记录查询",
	"号箱记录查询",
	"无此箱号!",
	"无记录!",

};

const char *pMenu_OpenBoxType[]={
	"开箱方式",
	"管理员",
	"密码  ",
	"用户卡",
	"卡+密 ",
	"指纹  ",
	"网令  ",
	"投递员"
};

const char *pAccessStatus[]={
	"存取状态",
	"  存",
	"  取",
	"  **"
};

const char *pMenu_CardSector[]={
	"IC卡扇区设置",
	"请设置实际使用的IC卡扇区号!",
	"当前值:",
	"设置值:"
};

const char *pMenu_AllowOpenBoxNum[]={
	"寄存设置",
	"设置寄存时允许已开箱数量",
	"当前值:",
	"设置值:"
};

const char *pMenu_PickUpAwaitTime[]={
	"最小寄存时间设置",
	"当前值(S):",
	"请输入新值(S):"
};

const char *pMenu_PickUpTimeout[]={
	"寄存超时时间设置",
	"当前值(S):",
	"请输入新值(S):"
};

const char *pMenu_SetUIntValue[]={
	"参数设置",
	"当前值(S):",
	"请输入新值(S):"
};

//--------------用户管理员操作界面-----------------------------
const char *pMenu_Admin[]={
	"管理员菜单",
	"1.系统",
	"2.管理",
	"3.箱门",
	"4.寄存",
	"5.查询",
	"0.退出"
};

const char *pMenu_Admin_SysSet[]={
	"系统设置",
	"1.模式",
	"2.时钟",
	"3.语音",
	"4.柜号",
	"5.联网",
	"6.TCP/IP",
	"7.修复",
	"0.退出"
};

const char *pMenu_Admin_Manage[]={
	"管理设置",
	"1.管密",
	"2.管理卡",
	"3.柜名",
	"4.标识码",
	"5.卡扇区",
	"6.预授卡",
	"7.清卡",
	"0.退出"
};

const char *pMenu_Admin_Box[]={
	"箱门操作",
	"1.单开",
	"2.全开",
	"3.单清",
	"4.全清",
	"5.锁箱",
	"6.解锁",
};

const char *pMenu_Admin_Lockers[]={
	"寄存设置",
	"1.开箱数",
	"2.超时",
	"3.延时取",
	"4.提示"
};

const char *pMenu_Admin_Query[]={
	"记录查询",
	"1.查开箱",
	"2.版本",
	"3.机号",
	"4.柜名",
	"5.标识码",
	"6.锁箱",
	"7.箱状态"
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
	"5.箱数",
	"6.开门检",
	"7.ID卡",
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



//---------------------用户寄存物操作界面-----------------------
const char *pMenu_UserAccess[]={
	"箱满!",
	"请按指纹",
	"第一次采集",
	"请移开手指",
	"请再按指纹",
	"第二次采集",
	"指纹授权成功",
	"指纹授权失败",
	"指纹未授权",
    "无效指纹!",       //	"指纹录入错误",
	"指纹合并失败",
	"指纹存储失败",
	"指纹已授权",
	""
};

#endif
/*========================File end===================================================================================================*/

