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
**  Function:	��ʾ�������
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

#define LCD_Color_DarkBlue       		0x01CF //����ɫ
#define LCD_Color_LightBlue      		0x7D7C //ǳ��ɫ  
#define LCD_Color_GrayBlue       		0x5458 //����ɫ
#define LCD_Color_Back           		0xdefb //����ɫ

//---------------------------LCD Pic------------------------------------------------
#define ICON_Teacup                     1      //�豭       
#define ICON_NetBll                     2      //��״��                     
#define ICON_ExclamationMark            3      //(Ȧ+!)
#define ICON_Hospital                   4      //ҽԺͼ��        
#define ICON_Lockers                    5      //����                           
#define ICON_Microphone            		7      //��˷�
#define ICON_DateList                   8      //������ 
#define ICON_House                 	    9      //����
#define ICON_RecycleBin                 10     //����վ          
#define ICON_Loudspeaker                11     //����           
#define ICON_Gear                       13     //����     
#define ICON_Clock                      14     //ʱ��                           
#define ICON_ZY                         15     //(ZY)
#define ICON_Find                       16     //�Ŵ�                           
#define ICON_Ascii               		17     //��ĸ("A")
#define ICON_Lock                       19     //��                                    
#define ICON_RotationLock               20     //��ת��
#define ICON_Wifi               		21     //WIFI
#define ICON_Car                        22     //����
#define ICON_Bulb                       23     //����
#define ICON_Airplane                   24     //�ɻ�

#define ICONS_001                       0      //�˵�ʽͼ��(�����豭\����\ҽԺͼ��\��̾��)
#define ICONS_002                       6      //�˵�ʽͼ��(������˷�\����\����\����)      
#define ICONS_003                       12     //�˵�ʽͼ��(����ZY\�Ŵ�\����\ʱ��)             
#define ICONS_004                       18     //�˵�ʽͼ��(������\WIFI\����)             


#define PHOTO_ZhenYaoTel         		0x64   //ZY������(���ͷ��绰)
#define PHOTO_Desktop            		0x65   //������ͼƬ

#define HALF_PHOTO_ZNXG          		2      //��ӭʹ���������
#define HALF_PHOTO_PlsCard       		5      //��������ˢ��
#define HALF_PHOTO_PlsPwd        		0x68   //����������������
#define HALF_PHOTO_ZNJCG         		0x69   //��ӭʹ�����ܼĴ��
#define HALF_PHOTO_StorePlsKey        	3      //�����밴"�Ĵ�"�� 
#define HALF_PHOTO_FetchByCard   		5      //ȡ�������
#define HALF_PHOTO_FetchByFingerprint   3      //ȡ���밴ָ��       
#define HALF_PHOTO_ZNKDG                0x6D   //��ӭʹ�����ܿ�ݹ�
#define HALF_PHOTO_DeliverByCard        0x6E   //Ͷ����ˢ��
#define HALF_PHOTO_ZNXBX                0x6F   //��ӭʹ�������ű���
#define HALF_PHOTO_OpenBoxByCard        5      //�����������
#define HALF_PHOTO_OpenBoxByPwd         0x71   //����������������
#define HALF_PHOTO_CardOrPwd            0x72   //�������������뿪��
#define HALF_PHOTO_ZNYBG                0x73   //��ӭʹ�������ʰ���
#define HALF_PHOTO_FetchByPwd           0x74   //ȡ������������
#define HALF_PHOTO_JICUN_CARD           0x75   //ȡ����ˢ��

/*add by zenghl*/
#define Main_UI_Num                     10
#define UNIT_UI_Num                     15
#define HALF_PHOTO_Xsize				43     //��ʾ�� X�����С 
#define HALF_PHOTO_Ysize				279    //��ʾ�� Y�����С 
#define HALF_PHOTO_X_1					7      //��ʾ����ʼ X����
#define HALF_PHOTO_Y_1					20     //��ʾ����ʼ Y����
#define HALF_PHOTO_X_2					120    //��ʾ����ʼ X����
#define HALF_PHOTO_Y_2					20     //��ʾ����ʼ Y����

#define HALF_ICON_Xsize					48     //ͼ�� X�����С 
#define HALF_ICON_Ysize					48     //ͼ�� Y�����С 

//---------------------------LCD Type------------------------------------------------

#define LCD_TYPE_A32LCD 		      0x32	   //3.2��LCD 	


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
	NOTICE_SET_OK          = 0x01,   //��ʾ��Ϣ--���óɹ�
	NOTICE_SET_FAIL,                 //��ʾ��Ϣ--����ʧ��
	NOTICE_PWD_ERR,                  //��ʾ��Ϣ--�������
	NOTICE_SYS_INIT_ASK,             //��ʾ��Ϣ--��ʼ��ȷ��
	NOTICE_SYS_INIT_WAIT,            //��ʾ��Ϣ--��ʼ���ȴ�
	NOTICE_SYS_INIT_OK,              //��ʾ��Ϣ--��ʼ�����
	NOTICE_BOX_ID_ERR,               //��ʾ��Ϣ--��Ŵ���
	NOTICE_PLS_WAIT,                 //��ʾ��Ϣ--���Ե�
    NOTICE_ASK_OPEN_ALL_BOX,         //��ʾ��Ϣ--ȷ��ȫ��
    NOTICE_ASK_CLR_CARD,             //��ʾ��Ϣ--�忨ȷ��       
    NOTICE_ASK_CLR_CARD_OK,          //��ʾ��Ϣ--�忨���   
    NOTICE_ASK_CLR_BOX,              //��ʾ��Ϣ--����ȷ��
    NOTICE_ASK_CLR_ALLBOX,           //��ʾ��Ϣ--��������ȷ��
    NOTICE_OPENING_BOX,              //��ʾ��Ϣ--���ڿ���
    NOTICE_LOCKING_BOX_OK,           //��ʾ��Ϣ--����ɹ�
    NOTICE_UNLOCKING_BOX_OK,         //��ʾ��Ϣ--�������ɹ�

}NOTICE;

//---------------------------�Ĵ���ȡ��ʾ��Ϣ���ͱ�------------------------------------------------
typedef enum 
{
	LOCKERS_SysError,               //ϵͳ����
	LOCKERS_AdminCard,              //����
	LOCKERS_CardNotConsign,         //��δ�Ĵ�
	LOCKERS_NotExtractTime,         //δ��ȡ��ʱ��
	LOCKERS_OpenBoxFaild,           //����ʧ��
	LOCKERS_PlsCard,                //��ˢ��
	LOCKERS_IsFull,                 //��������
	LOCKERS_OpeningBox,             //���ڿ���
	LOCKERS_CardUnauthorized,       //��δ��Ȩ
	LOCKERS_NonSysCard,             //�Ǳ�ϵͳ��  
	LOCKERS_Overdue,                //�ѹ�ʹ������
	LOCKERS_LimitOfUse,             //��������,�뵽�������ϼĴ�
	LOCKERS_WriteCardFaild,         //д��ʧ��
	LOCKERS_RepairCard,             //�ϴο���ʧ��,�쳣�Ĵ�,�޸�������
	LOCKERS_ClrCardStoreInfo,       //(����ʧ��)������ϵļĴ���Ϣ
	LOCKERS_PlsCloseBoxDoor,        //��غ����� 
	LOCKERS_PlsFingerOrCard,        //�밴ָ�ƻ�ˢ��
	LOCKERS_WaitTimeout,            //�ȴ���ʱ
	LOCKERS_NoDeposit,              //δ�Ĵ�
	LOCKERS_NetError,               //�����쳣
	LOCKERS_
	
}LOCKERS_NOTICE;

extern char cStr_Message[100];
extern const char *pStr_InputPasswordHint;
extern const char *pStr_UnLockAllBox;

/*******************************************************************************************************
* Description  : ��ʾ�ַ��� 
********************************************************************************************************/
extern void  LCD_DisplayStr(uint16 s_x, uint16 s_y, uchar8 Font, const char* pStr, uint16 FColor);

/*******************************************************************************
* Function Name  : GUI_ShowSysVersion
* Description    : ��ʾϵͳ�汾
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_ShowSysVersion(void);

/*******************************************************************************
* Function Name  : GUI_ChoiceKeyboardType
* Description    : ��ʾѡ����̵�����
* Input          : None
* Output         : None
* Return         : None
* Notes          : None
*******************************************************************************/
extern void GUI_ChoiceKeyboardType(void);

/*******************************************************************************
* Function Name  : GUI_InputUseDate
* Description    : ��ʾ����ĳ�����ʹ������
* Input          : box -- ���
* Output         : None
* Return         : None
* Notes          : None
*******************************************************************************/
extern void GUI_InputUseDate(uchar8 box, uchar8* pDateStr);

/*******************************************************************************
* Function Name  : GUI_InputBoxID
* Description    : �������
* Input          : None
* Output         : None
* Return         : None
* Notes          : ��һ����ʾ������������
*******************************************************************************/
extern void GUI_InputBoxID(uchar8 BoxID);

/*******************************************************************************
* Function Name  : GUI_AdminSetBoxNum
* Description    : ����Ա��������
* Input          : type -- ����(0--��ѡ, 1--����, 2--����1...); BoxNum -- ����
* Output         : None
* Return         : None
* Notes          : ��һ����ʾ������������
*******************************************************************************/
extern void GUI_AdminSetBoxNum(uchar8 type, uchar8 BoxNum);

/*******************************************************************************
* Function Name  : GUI_Open1Box
* Description    : ������ʾ
* Input          : u8BoxNum -- ���  AddNote -- ���Ӽ���ʾ��Ϣ
* Output         : None
* Return         : None
* Notes          : AddNote : 1 -- �����ֹ���
                             2 -- ��<#>ֹͣ����
*******************************************************************************/
extern void GUI_Open1Box(uchar8 u8BoxNum, uchar8 AddNote);

/*******************************************************************************
* Function Name  : GUI_Open1BoxFaild
* Description    : ��ʾ����ʧ��
* Input          : u8BoxNum -- ���  AddNote -- ���Ӽ���ʾ��Ϣ
* Output         : None
* Return         : None
* Notes          : AddNote : 1 -- �����ֹ���
                             2 -- ��<#>ֹͣ����
*******************************************************************************/
extern void GUI_Open1BoxFaild(uchar8 u8BoxNum, uchar8 AddNote);

/*******************************************************************************
* Function Name  : GUI_BoxBeLocked
* Description    : ��ʾ���ѱ���,����ϵ����Ա
* Input          : u8BoxNum -- ��� 
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_BoxBeLocked(uchar8 box);

/*******************************************************************************************************
* Description  : ��ʾʱ��
********************************************************************************************************/
extern void  GUI_Time(void);

/*******************************************************************************************************
* Description  : ��ʾ������  
********************************************************************************************************/
extern void  GUI_Admin(void);

/*******************************************************************************
* Function Name  : GUI_UpdateDisplay(LCM_UI *ppLCM_UI,char start, char end)
* Description    : ���½�����ʾ
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_UpdateDisplay(LCM_STR_UI *pStrUi,char start, char end);

/*******************************************************************************
* Function Name  : GUI_ShowMessage
* Description    : ��ʾ��Ϣ
* Input          : pstr--Ҫ��ʾ���ַ���ָ��, color--��ɫ
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_ShowMessage(const char* pstr, uint16 color);

/*******************************************************************************
* Function Name  : GUI_ShowTowRowMessage
* Description    : ��ʾ������Ϣ
* Input          : pstr--Ҫ��ʾ���ַ���ָ��, color--��ɫ
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_ShowTowRowMessage(const char* pstr, const char* pstr02, uint16 color);

/*******************************************************************************
* Function Name  : GUI_SetTime
* Description    : ����ʱ��
* Input          : pDateStr --YY-MM-DD�ַ���ָ�� , pTimeStr--HH;MM;SS�ַ���ָ��
* Output         : None
* Return         : None
* Notes          : ���±߿���ʾ��ǰ�����ں�ʱ��
*******************************************************************************/
extern void GUI_SetTime(uchar8* pDateStr, uchar8* pTimeStr);

/*******************************************************************************
* Function Name  : GUI_SetDebugMode
* Description    : ����ģʽ��������
* Input          : bDebugMode -- ��ǰ����״̬
* Output         : None
* Return         : None
* Notes          : None
*******************************************************************************/
extern void GUI_SetDebugMode(uchar8 bDebugMode);

/*******************************************************************************
* Function Name  : GUI_SetBoxDoorCheckSwitch
* Description    : ��������״̬��⿪��
* Input          : bSwitch -- ��ǰ����״̬
* Output         : None
* Return         : None
* Notes          : None
*******************************************************************************/
extern void GUI_SetBoxDoorCheckSwitch(uchar8 bSwitch);

/*******************************************************************************
* Function Name  : GUI_SetBoxDoorCheckSwitch
* Description    : ��������״̬��⿪��
* Input          : bSwitch -- ��ǰ����״̬
* Output         : None
* Return         : None
* Notes          : None
*******************************************************************************/
extern void GUI_SetIDCardSwitch(uchar8 bSwitch);

/*******************************************************************************
* Function Name  : GUI_SetWorkMode
* Description    : ����ģʽ����
* Input          : mode -- ��ǰģʽ
* Output         : None
* Return         : None
* Notes          : None
*******************************************************************************/
extern void GUI_SetWorkMode(uchar8 mode);

/*******************************************************************************
* Function Name  : GUI_InputVoiceLevel
* Description    : ���������ȼ�
* Input          : VoiceLevel -- �����ȼ�
* Output         : None
* Return         : None
* Notes          : None
*******************************************************************************/
extern void GUI_InputVoiceLevel(uchar8 VoiceLevel);

/*******************************************************************************
* Function Name  : GUI_InputMachineNum
* Description    : �������
* Input          : iMachineNum -- ��ǰ����Ļ���
* Output         : None
* Return         : None
* Notes          : ��һ����ʾ������������
*******************************************************************************/
extern void GUI_InputMachineNum(uint32 iMachineNum);

/*******************************************************************************
* Function Name  :  GUI_SetNetSwitch
* Description    : ������Ȩ
* Input          : NetSwitch -- ��ǰ����״̬
* Output         : None
* Return         : None
* Notes          : None
*******************************************************************************/
extern void GUI_SetNetSwitch(uchar8 NetSwitch);

/*******************************************************************************
* Function Name  : GUI_ChoiceNetInfo
* Description    : ������Ϣѡ��
* Input          : None
* Output         : None
* Return         : None
* Notes          : 1.����IP 2.������IP 3.�������˿ں�
*******************************************************************************/
extern void GUI_ChoiceNetInfo(void);

/*******************************************************************************
* Function Name  : GUI_InputIp
* Description    : ���ñ���IP��ַ
* Input          : type -- 1.����IP, 2.������IP
                   pInputIpStr--�����IP��ַ(�ַ�����ʽ)
* Output         : None
* Return         : None
* Notes          : ͬʱ��ʾ�ɵ�IP
*******************************************************************************/
extern void GUI_InputIp(uchar8 type, uchar8* pInputIpStr);

/*******************************************************************************
* Function Name  : GUI_InputServerPort
* Description    : ���÷������˿ں�
* Input          : iPort--����Ķ˿ں�
* Output         : None
* Return         : None
* Notes          : ͬʱ��ʾ�ɵĶ˿ں�
*******************************************************************************/
extern void GUI_InputServerPort(uint32 iPort);

/*******************************************************************************
* Function Name  : GUI_Registration
* Description    : ϵͳע��
* Input          : pInputStr--������ַ���, InputType--��ǰ���뷨(0-����,INPUT_TYPE_ABC-��ĸ)
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_Registration(uchar8* pInputStr, uchar8 InputType);

/*******************************************************************************
* Function Name  : GUI_ShowNotice
* Description    : ��ʾ��ʾ��Ϣ
* Input          : Type--��Ϣ����
* Output         : None
* Return         : None
* Notes          : ��������ѡ��Ҫ��ʾ����Ϣ
*******************************************************************************/
extern void GUI_ShowNotice(uchar8 Type);


/*******************************************************************************************************
* Description  : ������״̬��ʾ����
********************************************************************************************************/
extern void  GUI_WorkStatus(void);

/*******************************************************************************
* Function Name  : GUI_ShowPassword
* Description    : ��ʾ����ʾ��������
* Input          : PsdNum -- ���������λ��
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_ShowPassword(uchar8 PsdNum);

/*******************************************************************************
* Function Name  : GUI_ChangePassword
* Description    : �޸ĸ߼�����Ա����
* Input          : pStr_OldPassword--������(ASCII��ʽ), ������������ַ���
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_ChangePassword(uchar8* pStr_OldPassword, uchar8* pInputStr);

/*******************************************************************************
* Function Name  : GUI_InputName
* Description    : ¼������
* Input          : pInputStr--������ַ���, InputType--��ǰ���뷨(0-����,INPUT_TYPE_ABC-��ĸ)
                   pNotice -- ������ʾ��Ϣ
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_InputName(uchar8* pInputStr, uchar8 InputType, const char* pNotice);

/*******************************************************************************
* Function Name  : GUI_AddValidCard
* Description    : ��ʾС����ʾ--����Ԥ��Ȩ��
* Input          : iCardNum--����, addmsg--������ʾ��Ϣ
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_AddValidCard(uint32 iCardNum, uchar8 addmsg);

/*******************************************************************************
* Function Name  : GUI_AddCard2Box
* Description    : ��ʾ�����ʾ--��Ȩ��
* Input          : box--���, iCardNum--����, CardCount--��ǰ��Ȩ������
* Output         : None
* Return         : None
* Notes          : �������Ȩ�û���(box=0),���ڵײ���ʾ"��<#>�л�����һ�����"
*******************************************************************************/
extern void GUI_AddCard2Box(uchar8 box, uint32 iCardNum, uchar8 CardCount);

/*******************************************************************************
* Function Name  : GUI_ClrBoxNotice
* Description    : ������ʾ��Ϣ��Ϣ
* Input          : u8BoxNum -- ���
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_ClrBoxNotice(uchar8 u8Box);

/*******************************************************************************
* Function Name  : GUI_AskLockinBox
* Description    : ��ʾ--ȷ������?
* Input          : u8BoxNum -- ���
* Output         : None
* Return         : None
* Notes          : MARK_CHOICE_ALL_BOX -- ����ѡ���������
*******************************************************************************/
extern void GUI_AskLockinBox(uchar8 u8Box);

/*******************************************************************************
* Function Name  : GUI_AskUnlockinBox
* Description    : ��ʾ--ȷ�������X��?
* Input          : u8BoxNum -- ���
* Output         : None
* Return         : None
* Notes          : MARK_CHOICE_ALL_BOX -- ����ѡ���������
*******************************************************************************/
extern void GUI_AskUnlockinBox(uchar8 u8Box);

/*******************************************************************************
* Function Name  : GUI_QueryBoxRecord
* Description    : �����¼��ѯ
* Input          : BoxNum -- ���, CurrentPage -- ��ǰҳ(0--N)
* Output         : None
* Return         : None
* Notes          : ÿҳ��ʾ4�ʿ����¼
*******************************************************************************/
extern void GUI_QueryBoxRecord(uchar8 BoxNum, uchar8 CurrentPage);

/*******************************************************************************
* Function Name  : GUI_QueryLockBoxList
* Description    : ��ʾ�����б�
* Input          : CurrentPage -- ��ǰҳ(0--N)
* Output         : None
* Return         : None
* Notes          : 
*******************************************************************************/
extern void GUI_QueryLockBoxList(uchar8 CurrentPage);

/*******************************************************************************
* Function Name  : GUI_QueryBoxStatusList
* Description    : ��ѯ��״̬�б�(����״̬�Լ�����״̬)
* Input          : CurrentPage -- ��ǰҳ(0--N)
* Output         : None
* Return         : None
* Notes          : ÿҳ��ʾ4�����״̬
*******************************************************************************/
extern void GUI_QueryBoxStatusList(uchar8 CurrentPage);

/*******************************************************************************
* Function Name  : GUI_SetCardSector
* Description    : ����IC����ʹ��������
* Input          : sector -- ����Ŀ�������
* Output         : None
* Return         : None
* Notes          : None
*******************************************************************************/
extern void GUI_SetCardSector(uchar8 sector);

/*******************************************************************************
* Function Name  : GUI_InputAllowOpenBoxNum
* Description    : ���üĴ�ʱ��������������
* Input          : boxNum -- ��������
* Output         : None
* Return         : None
* Notes          : None
*******************************************************************************/
extern void GUI_InputAllowOpenBoxNum(uchar8 boxNum);

/*******************************************************************************
* Function Name  : GUI_SetPickUpAwaitTime
* Description    : ������ʱȡ���ȴ�ʱ��
* Input          : time_s -- ʱ��(S)
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_SetPickUpAwaitTime(uint32 time_s);

/*******************************************************************************
* Function Name  : GUI_SetPickUpTimeout
* Description    : ������ʱȡ����ʱʱ��
* Input          : time_s -- ʱ��(S)
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_SetPickUpTimeout(uint32 time_s);

/*******************************************************************************
* Function Name  : GUI_SetUIntValue
* Description    : ����һ������������ֵ
* Input          : pMenu -- ��ʾ�˵�, uiOldValue -- ��ֵ, uiNewValue -- ��ֵ
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_SetUIntValue(const char *pMenu, uint32 uiOldValue, uint32 uiNewValue);

/*******************************************************************************
* Function Name  : GUI_ChoiceAdminMenu
* Description    : ѡ�����ȼ��˵�
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_ChoiceAdminMenu(void);

/*******************************************************************************
* Function Name  : GUI_AdminCardOpertion
* Description    : ����Ա��¼-->(��ѯ)�˵�����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_AdminCardOpertion(void);

/*******************************************************************************
* Function Name  : GUI_AdminMenu
* Description    : �û�����Ա��¼������
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_AdminMenu(void);

/*******************************************************************************
* Function Name  : GUI_Admin_SysSetMenu
* Description    : ����Ա��¼-->(ϵͳ)�˵�����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_Admin_SysSetMenu(void);

/*******************************************************************************
* Function Name  : GUI_Admin_ManageMenu
* Description    : ����Ա��¼-->(����)�˵�����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_Admin_ManageMenu(void);

/*******************************************************************************
* Function Name  : GUI_Admin_BoxMenu
* Description    : ����Ա��¼-->(����)�˵�����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_Admin_BoxMenu(void);

/*******************************************************************************
* Function Name  : GUI_Admin_LockersMenu
* Description    : ����Ա��¼-->(�Ĵ�)�˵�����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_Admin_LockersMenu(void);

/*******************************************************************************
* Function Name  : GUI_Admin_QueryMenu
* Description    : ����Ա��¼-->(��ѯ)�˵�����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_Admin_QueryMenu(void);

/*******************************************************************************
* Function Name  : GUI_TopAdminMenu
* Description    : �߼�����Ա��¼-->(ϵͳ)�˵�����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_TopAdminMenu(void);

/*******************************************************************************
* Function Name  : GUI_TopAdmin_SysSetMenu
* Description    : �߼�����Ա��¼-->(ϵͳ)�˵�����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_TopAdmin_SysSetMenu(void);

/*******************************************************************************
* Function Name  : GUI_SuperAdmin_SysSetMenu
* Description    : ��������Ա��¼-->(ϵͳ)�˵�����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_SuperAdmin_SysSetMenu(void);

/*******************************************************************************
* Function Name  : GUI_LockersNotice
* Description    : �Ĵ���ȡ��ʾ��Ϣ
* Input          : Type--��Ϣ����
* Output         : None
* Return         : None
* Notes          : ��������ѡ��Ҫ��ʾ����Ϣ
*******************************************************************************/
extern void GUI_LockersNotice(uchar8 Type);

/*******************************************************************************
* Function Name  : GUI_PlsCloseBoxDoor
* Description    : ��ʾ��δ��,���ȹغ�����
* Input          : box -- ��������� 
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_PlsCloseBoxDoor(uchar8 box);

/*******************************************************************************
* Function Name  : GUI_UserConsignFail
* Description    : ��ʾ����ʧ��,�����¼Ĵ�
* Input          : box -- ����ʧ�ܵ���� 
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_UserConsignFail(uchar8 box);

/*******************************************************************************
* Function Name  : GUI_UserTakeAwwyFail
* Description    : ��ʾ�û�ȡ������ʧ��,����ϵ����Ա
* Input          : box -- ����ʧ�ܵ���� 
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_UserTakeAwwyFail(uchar8 box);

/*******************************************************************************
* Function Name  : GUI_ShowIDPosition
* Description    : ��ʾ�û�����Ʒ��Nx�Ź��Nx����
* Input          : u8Locker--���, u8Box--���,
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_ShowIDPosition(uchar8 u8Locker, uchar8 u8Box);

/*******************************************************************************
* Function Name  : GUI_PackageInOtherLockers
* Description    : ��ʾ�û�����Ʒ�������
* Input          : pLockersNmae--����, u8Box--���,
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_PackageInOtherLockers(const char* pLockersNmae, uchar8 u8Box);

/*******************************************************************************
* Function Name  : GUI_LockersParameters
* Description    : ��ʾ������
* Input          : void
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_LockersParameters(void);

/*******************************************************************************
* Function Name  : GUI_NetParameters
* Description    : ��ʾ����������
* Input          : void
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_NetParameters(void);

/*******************************************************************************
* Function Name  : GUI_UserAccess
* Description    : �û��Ĵ���--��ʾ����ʾ
* Input          : Type---���ݱ��
* Output         : None
* Return         : None
*******************************************************************************/
extern void GUI_UserAccess(uint32 Type);

/*===================================File end===================================================*/ 
#endif  //_APP_GUI_H_
