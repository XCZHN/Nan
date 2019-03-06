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
**  Function:	��ʾ��Ϣ��(���ĺ���) 
==                                                                                         
**  Data:       2017/06/02                                                                       
================================================================================================*/

#ifndef _NOTICE_H_
#define _NOTICE_H_


#include "Config.h"


const char *pStr_Company = "��������ҫ�Ƽ����޹�˾";
const char *pStr_Phone = "400-886-3136";
const char *pStr_SysVersion = "ϵͳ�汾";
	
const char *pStr_SetOK = "���óɹ�!";
const char *pStr_SetFailed = "����ʧ��!";
const char *pStr_PasswordError = "�������!";
const char *pStr_AskSysInit = "ȷ��Ҫ��ʼ��ϵͳ?";
const char *pStr_SysInitWait = "���ڳ�ʼ����...";
const char *pStr_SysInitOK = "ϵͳ��ʼ���ɹ�!";
const char *pStr_BoxIdErr = "��Ŵ���!";
const char *pStr_PlsWait = "���Ե�...";
const char *pStr_AskClrCard = "ȷ���������Ȩ��?";
const char *pStr_ClrCardOk = "�忨���!";
const char *pStr_AskClrBox = "ȷ����մ���?";
const char *pStr_AskClrAllBox = "ȷ�����������?";
const char *pStr_OpeningBox = "���ڿ���...";
const char *pStr_LockingBoxOk = "����ɹ�!";
const char *pStr_UnlockingBoxOk = "�������ɹ�!";

const char *pStr_OperationTip = "������ʾ";
const char *pStr_Exit = "0.�˳�";
const char *pStr_InputType = "��ǰ���뷨:";
const char *pStr_PlsInput = "������";
const char *pStr_PlsInputPsw = "����������";
const char *pStr_PlsEnterPwd = "������ά������:";
const char *pStr_PlsInputBoxNum = "����������";
const char *pStr_PlsInputBoxID = "���������";
const char *pStr_Notice255IsAll = "����255��ѡ���������.";
const char *pStr_PlsInputMachineNum = "���������";
const char *pStr_Old = "��:";
const char *pStr_New = "��:";
const char *pStr_PlsEnterName = "�����뱾������:";
const char *pStr_PlsEnterIdCode = "������ϵͳ��ʶ��:";
const char *pStr_PlsCard = "��ˢ��!";
const char *pStr_PlsCardAgain = "���ٴ�ˢ��!";
const char *pStr_PlsEnterValidity = "������ʹ������:";
const char *pStr_PlsChoiceType = "��ѡ������!";
const char *pStr_WithoutBoxBeLock = "������";
const char *pStr_QueryLockBox ="�����ѯ(��������)";
const char *pStr_QueryBoxStatus = "��״̬��ѯ";
const char *pStr_QueryPrompt = "1.��һҳ   2.��һҳ";
const char *pStr_BoxStatus[] = {"�Ź�","�ſ�","����","����"};

const char *pStr_NxBox = "����";
const char *pStr_Open1Box = "�����ѿ�.";
const char *pStr_PlsCloseBox = "�����ֹ���!";
const char *pStr_StopOpenBox = "��<#>ֹͣ����!";
const char *pStr_AskOpenAllBox = "ȷ��ȫ��?";
const char *pStr_ClrBoxNotice = "����������Ʒ����غ�����!";
const char *pStr_Locking1Box = "ȷ����������?";
const char *pStr_LockinAllBox = "ȷ������������?";
const char *pStr_Unlocking1Box = "ȷ��������?";
const char *pStr_UnlockingAllBox = "ȷ�����������?";
const char *pStr_BoxBeLocked = "���ѱ���";

//-------------�Ĵ���ȡ������ʾ��Ϣ-----------------------------
const char *pStr_NowEmptyBoxNum = "��ǰ������: ";
const char *pStr_SysError = "ERROR:ϵͳ����!";
const char *pStr_AdminCard = "����!";
const char *pStr_CardNotConsign = "�˿�δ�Ĵ�!";
const char *pStr_CardNotRegister = "�˿�δ��Ȩ!";
const char *pStr_NotExtractTime = "δ��ȡ��ʱ��!";
const char *pStr_OpenBoxFaild = "����ʧ��!";
const char *pStr_xBoxOpenFail = "�����ſ���ʧ��!";
const char *pStr_PlsCallAdmin = "����ϵ����Ա.";
const char *pStr_LockersIsFull = "�����Ѵ���!";
const char *pStr_IdPosition = "������Ʒ��:";
const char *pStr_xLockers = "��";
const char *pStr_ThisLockers = "����";
const char *pStr_OnOtherLockers = "���������!";
const char *pStr_xBox = "����!";
const char *pStr_PlsCardToPickUp = "��ֱ��ˢ��ȡ��!";
const char *pStr_xBoxNotClose = "������δ��.";
const char *pStr_PlsCloseThisBox = "���ȹغô�����!";
const char *pStr_PlsConsignAgain = "�����¼Ĵ�.";
const char *pStr_Overdue = "���ѹ���!";
const char *pStr_NonSysCard = "�Ǳ�ϵͳ��Ȩ��!";
const char *pStr_LimitOfUse = "��������!";
const char *pStr_PlsGotoOtherLockers = "�뵽������Ĵ�!";
const char *pStr_WriteCardFaild = "д��ʧ��!";
const char *pStr_ConsignError = "�ϴ��쳣��ȡ!";
const char *pStr_RestoreCard = "�����޸�,�ɼ����Ĵ�.";
const char *pStr_PlsCloseBoxDoor = "��غ�����!";
const char *pStr_PlsFingerOrCard = "�밴ָ�ƻ�ˢ��!";
const char *pStr_WaitTimeout = "�ȴ���ʱ!";
const char *pStr_NoDeposit = "����ָδ�Ĵ���Ʒ!";
const char *pStr_NetError = "�����쳣!";

const char *pTitleStr_CloseTimeout = "���ų�ʱʱ������";
const char *pTitleStr_NoticeTime = "���ž���ά��ʱ��";

const char *pMenu_Week[]={
	"������",
	"����һ",
	"���ڶ�",
	"������",
	"������",
	"������",
	"������"
};

const char *pMenu_KeyType[]={
	"���ü�������",
    "1.��������",
	"2.��������",
	"��ѡ���������!"
};

const char *pMenu_SetDebugMode[]={
	"���Կ���",
	"��ǰ״̬: ",
	"1.����  ",
	"2.�ر�  "
};

const char *pMenu_SetCheckDoorSwitch[]={
	"����״̬��⿪��",
	"��ǰ״̬: ",
	"1.����  ",
	"2.�ر�  "
};

const char *pMenu_WorkMode[]={
	"���ù���ģʽ",
	"��ǰģʽ: ",
	"1.��һIC�� ",
	"2.Ԥ��Ȩ�� ",
	"3.д��ģʽ",
	"4.������֤",
	"5.��һָ��",
	"6.��һָ�ƻ���һ��",
};

const char *pMenu_SetBoxNum[]={
	"��������",
	"1.��������",
	"2.��������",
	
	"1.����(1)����",
	"2.����(2)����",
	"3.����(3)����",
	""
};

const char *pMenu_Registration[]={
	"�Զ��޸�",
	"���ڼ��ϵͳ...",
	"��⵽�����쳣...",
	"�����޸�����...",
	"�޸��ɹ�!"
};

const char *pMenu_SetTime[]={
	"ʱ������",
	"����: ",
	"ʱ��: "
};

const char *pMenu_VoiceLevel[]={
	"��������",
	"����������(0-30):"
};

const char *pMenu_SetNetSwitch[]={
	"���翪��",
	"��ǰ״̬: ",
	"1.����  ",
	"2.�ر�  "
};

const char *pMenu_IDCaardSwitch[]={
	"ID������������",
	"��ǰ: ",
	"1.����  ",
	"2.�ر�  "
};

const char *pMenu_TCPIP[]={
    "��������",
	"1.����IP��ַ",
	"2.������IP��ַ",
	"3.�������˿ں�"
};

const char *pMenu_ChoiceAdmin[]={
	"�����޸�",
	"0.��������Ա",
	"1.�û�����Ա",
	"2.�߼�����Ա",
};

const char *pMenu_CardAccredit[]={
	"����Ȩ",
	"��Ȩ����",
	"��ǰ���:",
	"�����!",
	"<����Ȩ>",
	"<����>",
	"��<#>�л�����һ���."
};

const char *pMenu_AdminCardOpertion[]={
	"��������",
	"1.�ڿ�",
	"2.ɾ��"
};

const char *pMenu_RecordQuery[]={
	"��¼��ѯ",
	"�����¼��ѯ",
	"�޴����!",
	"�޼�¼!",

};

const char *pMenu_OpenBoxType[]={
	"���䷽ʽ",
	"����Ա",
	"����  ",
	"�û���",
	"��+�� ",
	"ָ��  ",
	"����  ",
	"Ͷ��Ա"
};

const char *pAccessStatus[]={
	"��ȡ״̬",
	"  ��",
	"  ȡ",
	"  **"
};

const char *pMenu_CardSector[]={
	"IC����������",
	"������ʵ��ʹ�õ�IC��������!",
	"��ǰֵ:",
	"����ֵ:"
};

const char *pMenu_AllowOpenBoxNum[]={
	"�Ĵ�����",
	"���üĴ�ʱ�����ѿ�������",
	"��ǰֵ:",
	"����ֵ:"
};

const char *pMenu_PickUpAwaitTime[]={
	"��С�Ĵ�ʱ������",
	"��ǰֵ(S):",
	"��������ֵ(S):"
};

const char *pMenu_PickUpTimeout[]={
	"�Ĵ泬ʱʱ������",
	"��ǰֵ(S):",
	"��������ֵ(S):"
};

const char *pMenu_SetUIntValue[]={
	"��������",
	"��ǰֵ(S):",
	"��������ֵ(S):"
};

//--------------�û�����Ա��������-----------------------------
const char *pMenu_Admin[]={
	"����Ա�˵�",
	"1.ϵͳ",
	"2.����",
	"3.����",
	"4.�Ĵ�",
	"5.��ѯ",
	"0.�˳�"
};

const char *pMenu_Admin_SysSet[]={
	"ϵͳ����",
	"1.ģʽ",
	"2.ʱ��",
	"3.����",
	"4.���",
	"5.����",
	"6.TCP/IP",
	"7.�޸�",
	"0.�˳�"
};

const char *pMenu_Admin_Manage[]={
	"��������",
	"1.����",
	"2.����",
	"3.����",
	"4.��ʶ��",
	"5.������",
	"6.Ԥ�ڿ�",
	"7.�忨",
	"0.�˳�"
};

const char *pMenu_Admin_Box[]={
	"���Ų���",
	"1.����",
	"2.ȫ��",
	"3.����",
	"4.ȫ��",
	"5.����",
	"6.����",
};

const char *pMenu_Admin_Lockers[]={
	"�Ĵ�����",
	"1.������",
	"2.��ʱ",
	"3.��ʱȡ",
	"4.��ʾ"
};

const char *pMenu_Admin_Query[]={
	"��¼��ѯ",
	"1.�鿪��",
	"2.�汾",
	"3.����",
	"4.����",
	"5.��ʶ��",
	"6.����",
	"7.��״̬"
};

//--------------�߼�����Ա��������-----------------------------
const char *pMenu_TopAdmin[]={
	"�߹����ò˵�",
	"1.����",
	"0.�˳�"
};

const char *pMenu_TopAdmin_Set[]={
	"���ò˵�",
	"1.����",
	"2.����",
	"3.ģʽ",
	"4.����",
	"5.����",
	"6.���ż�",
	"7.ID��",
	"0.�˳�"
};

//--------------��������Ա��������-----------------------------
const char *pMenu_SuperAdmin_Set[]={
	"���ò˵�",
	"1.����",
	"2.����",
	"3.����",
	"4.��ʼ��",
	"5.����",
	"0.�˳�"
};



//---------------------�û��Ĵ����������-----------------------
const char *pMenu_UserAccess[]={
	"����!",
	"�밴ָ��",
	"��һ�βɼ�",
	"���ƿ���ָ",
	"���ٰ�ָ��",
	"�ڶ��βɼ�",
	"ָ����Ȩ�ɹ�",
	"ָ����Ȩʧ��",
	"ָ��δ��Ȩ",
    "��Чָ��!",       //	"ָ��¼�����",
	"ָ�ƺϲ�ʧ��",
	"ָ�ƴ洢ʧ��",
	"ָ������Ȩ",
	""
};

#endif
/*========================File end===================================================================================================*/

