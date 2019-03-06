/*===============================================================================================
==                                                                                              
**  Copyright(c) 2014  Intelligent Admin electronic systems co., LTD. All rights reserved.
==                                                                                           
**	Author:		Asen                                                                           
==                                                                                          
**  Version:	v1.0.0                                                                          
==
**  Filename:	Mid_SysConfig.h                                                                       
==                                                                                         
**  Function:	ϵͳ���������ļ�
==                                                                                         
**  Data:       2014/04/28                                                                       
================================================================================================*/

#ifndef	_APP_SYS_CONFIG_H_
#define	_APP_SYS_CONFIG_H_

#include "Includes.h"


//#define GUI_ENGLISH                          0xEB

#define VALIDITY_USING                       0xEB    //��Ч���
#define INPUT_TYPE_ABC                       0xAB    //���뷨--ABC
#define ALREADY_EXIST                        0xAE    //�Ѵ��ڱ�ʶ
#define NOTICE_STAY_TIME                     2000    //��ʾ��Ϣͣ��ʱ��(ms)
#define MESSAGE_STAY_TIME                    3000    //��ʾ��Ϣͣ��ʱ��(ms)
#define MARK_CHOICE_ALL_BOX                  255     //ѡ��������ı�ʶ
#define SYS_ID_CODE_LEN                      6       //ϵͳ��ʶ�볤��

#define Default_Box_Count          			 24      //Ĭ�ϵ�����
#define MAX_BOX_NUM          			 	 96      //���������
#define MAX_AdminCardNum                     6       //�󶨵Ĺ�������
#define MAX_BoxCardNum                       1       //ÿ����󶨵Ŀ�����
#define MAX_PasswordLen                      6       //������󳤶�
#define MAX_OpenBoxKeyLen                    6       //����������󳤶�
#define MAX_BoxSnLen                         2       //�������󳤶�
#define MAX_NameLen                          6       //���Ƶ���󳤶�
#define MAX_UserNameLen                      8       //�û���������󳤶�
#define MAX_UserIdLen                        10      //���ŵ���󳤶�

#define MAX_VALID_CAED_COUNT          		 2560    //���Ԥ��Ȩ������(4*2560=10KByte)
#define MAX_UserFingerprintNum               1       //ÿ���û����ָ������


#define SUPER_CARD_ID                        343056036L
                   
//#pragma pack(1)  //-----������-----PackType--Strrt---������---------------------------


//-----------------------------------Sys run info--------------------------------------/
typedef enum 							    //ϵͳ״̬��
{
	SYS_STATUS_IDLE = 0,				 	//����״̬	
	SYS_STATUS_KEY,							//��������״̬
	SYS_STATUS_CARD,                        //ˢ��״̬
	SYS_STATUS_LOCKED,                      //����״̬
	SYS_STATUS_JiCun,						//�Ĵ�״̬
	SYS_STATUS_QuWu,						//ȡ��״̬
	SYS_STATUS_ZW,                          //ָ�Ʋ���
	SYS_STATUS_Message,                     //��ʾ��ʾ��Ϣ
	SYS_STATUS
	
}E_SYS_STATUS;

typedef  struct  							//ϵͳ���в����ṹ��
{	
	uint32 u32Temp;							//���ڼ���ʱ����ʱ����
	uint32 u32RtcTime_S;					//RTCʱ��(HH*3600 + MM*60 + SS)
	uint32 u32TimingDelay;					//��ʱ����
	uint32 u32TimeCount_ms;					//ʱ�����(ms)
	uint32 u32LedBlinkTimes;				//LED��˸����
	uint32 iLastCommTime;                   //�ϴγɹ�ͨѶʱ��(ms)
	uint32 iHandleTimeout;                  //�Ĵ���ų�ʱ��ʾ(s)

	uchar8 CurrentStatus;					//��¼ϵͳ��ǰ������״̬
	uchar8 TimeBuf[8];						//(Hex)ʱ������
	uchar8 TempBuf[128];					//��ʱ�洢��
	uchar8 CommType;						//ͨѶ��ʽ 
	uchar8 LastCommType;					//�ϴ�ͨѶ��ʽ 
	uchar8 u8Temp;							//��ʱ����
	uchar8 bTxdEnable;						//ͨѶ���Ϳ���
	uchar8 bRS485Busy;                      //RS485æµ(ռ��)���
	uchar8 bLockedSys;                      //ϵͳ(����)����
	uchar8 bForbidTimeGoBack;               //��ֹʱ�������� 

	uchar8 bNetResetSys;					//��������ϵͳ���(=0xEB��Ч)
	uchar8 u8UseBox;		         		//�Ñ��Ĵ�����	
	uchar8 bDebug_Net;                      //�������翪��(�������������Ϣ) 0 -- �ر�,��0 --����

	uchar8 u8EmptyBoxCount;                 //��ǰ������  
	uchar8 EmptyBox[MAX_BOX_NUM+2];         //��ǰ����
	
	uchar8 BoxDoorStatus[MAX_BOX_NUM+1];    //����״̬(�ֽ�0 -- N ��Ӧ0--N������, 0��ʾ�ر�,1��ʾ����)
	uchar8 BoxInsideStatus[MAX_BOX_NUM+1];  //����״̬(�ֽ�0 -- N ��Ӧ0--N������, 1��ʾ����Ʒ,0��ʾ����(û����Ʒ))
		
	ST_TIME stRtcTime;						//RTCʱ��(��TimeBuf[]ֵ��ͬ)
	
}ST_SYSTEM_RUN_INFO;


//---------------------------------------Sysconfig----------------------------------------------

typedef	struct								//ϵͳӲ�����ýṹ�塣
{
	uint32 u32ConfigVer;					//�����汾(�ṹ:YYMMDD+SRN, 15090301)
	uint32 u32MachineNum;					//(HEX)����
	uint32 u32Baudrate;						//����ͨѶ������
	uint32 u32BeatInterval;				    //������ʱ����(ms)

	uchar8 bDebugMode;                      //����ģʽ
	uchar8 CommType;					    //ͨѶ��ʽ 		
	uchar8 u8MachineType;					//����
	uchar8 u8RS485Type;						//RS485(0--�ӻ�, 1--���� )
	uchar8 u8KeyType;						//��������(0--����, 1--���� )
	uchar8 bABCSwitch;						//��ĸ���뿪��,0-- ����Ȩ, ��0��Ȩ
	uchar8 bNetwork;						//���翪��,VALIDITY_USING --����
	uchar8 u8VoiceLevel;					//��������,0-31��,0--��ʾ�ر�		
	uchar8 u8Crc;
		
}ST_SYS_HW;

typedef	struct								//��������ṹ�塣
{
	uint16 u16UDP_ListenPort;				//UDP���������˿�
	uint16 u16UDP_ServerPort;               //UDP�������˿�
	uint16 u16TCP_ListenPort;				//TCP���������˿�
	uint16 u16TCP_ServerPort;               //TCP�������˿�
	
	uchar8 MAC[6];                          //MAC��ַ
	uchar8 IP[6];                           //IP��ַ
	uchar8 Gateway[6];                      //����
	uchar8 NetMask[6];                      //��������
	
	uchar8 ServerIP[6];                     //������ip
	uchar8 ServerMAC[6];                    //������MAC��ַ
	uchar8 bAutoConnectServer;				//�������ӷ�����
	uchar8 u8Crc;
		
}ST_NETWORK;

//---------------------------LOCKERS---------------------------------------------------

typedef enum 							    //Box״̬��
{
	BOX_STATUS_IDLE = 0,				 	//����״̬	
	BOX_STATUS_OCCUPY,                   	//��Ͷ�ݵ�ռ��״̬
	BOX_STATUS
	
}E_BOX_STATUS;

typedef enum 							    //Box����״̬
{
	LSTATUS_FREE = 0,				 	    //δ���� 	
	LSTATUS_LOCKING,				    	//������
	
}E_BOX_LOCKING_STATUS;


typedef	struct								 
{ 
	uint32 iRecordCount;                    //�����¼�ۼ�
	uint32 iUserCardID;						//�����Ŀ���

	uint16 iTouchID;                        //������ָ�ƺ�
			
	uchar8 u8BoxID;        			 	    //���
	uchar8 u8SizeType;						//����(0-С��, 1-����, 2-����)
	uchar8 u8Status;   						//״̬	
	uchar8 bLocking;                        //�������

	uchar8 BoxSN[MAX_BoxSnLen+1];		    //�������� 
	uchar8 UserName[MAX_UserNameLen+1];		//ʹ�������� 
	uchar8 UserID[MAX_UserIdLen+1];		    //ʹ���߹���	
	uchar8 LastUsedTime[6];                 //�ϴ�ʹ��ʱ��(YY-MM-DD HH:MM:SS)
		
}ST_BOX;


typedef enum 							    //����ģʽ��
{
	WORK_MODE_ANY_CARD = 0,				 	//��һ������Ĵ�ģʽ
	WORK_MODE_VALID_CARD,                   //Ԥ��Ȩ����Ч��
	WORK_MODE_WRITE_CARD,                   //д��ģʽ
	WORK_MODE_ONLINE,                       //������֤ģʽ
	WORK_MODE_ANY_FINGER,                   //��һָ��
	WORK_MODE_ANY_FINGER_CARD,              //��һָ�ƻ���һ��
	WORK_MODE_VALID_FINGER,                 //Ԥ��Ȩ��ָ��
	WORK_MODE_ONLINE_FINGER,                //������ָ֤��ģʽ
	WORK_MODE_
	
}WORK_MODE;

typedef	struct								 
{			
	uint32 iValidCardCount;                 //Ԥ��Ȩ������
	uint32 iMinFetchTime;                   //��С�洢ʱ��(s)
	uint32 iMaxFetchTime;                   //��С�洢ʱ��(s)
	uint32 iHandleTimeout;                  //������ʱʱ��(s)
	uint32 iNoticeTime;                     //������ʾʱ��(s)
	uint32 iReserved;                       //Ԥ��
	uint32 AdminCard[MAX_AdminCardNum];     //����
		
	uchar8 u8BoxCount;             	        //�������
	uchar8 u8HostLockerBoxs;                //���������
	uchar8 u8ExtL01BoxNum;                  //����1������
	uchar8 u8ExtL02BoxNum;                  //����2������
	uchar8 u8ExtL03BoxNum;                  //����3������
	uchar8 bCheckBoxDoorStatus;             //������ſ���״̬
	uchar8 bEnableIdCard;                   //��Ȩ����ID��
	uchar8 u8Reserved02;	

	uchar8 u8WorkMode;			            //����ģʽ(�̶�\���) 		
	uchar8 u8OpenBoxMode;                   //���䷽ʽ
	uchar8 u8AllowBeOpenedBoxNum;           //�Ĵ��ʱ������������
	uchar8 Name[MAX_NameLen+1];		        //����(Ԥ��һ�ֽڿո�)	
	uchar8 SuperPassword[MAX_PasswordLen+1];  //��������Ա����
	uchar8 SeniorPassword[MAX_PasswordLen+1]; //�߼�����Ա����
	uchar8 AdminPassword[MAX_PasswordLen+1];  //�û�����Ա��¼����
	uchar8 Reserved[10];
		
}ST_LOCKERS;

typedef	struct								 
{
	uchar8 u8Sector;                        //������
	uchar8 u8KeyType;                       //��Կ����
	uchar8 Password[6];                     //��Կ
	
}ST_CARD_KEY;

//-----------------------------Client Info--------------------------------------------
typedef	struct								 
{	
	uchar8 Name[30];					    //��λ����
	uchar8 ValidityDate[3]; 				//��Ч��
	uchar8 IdCode[SYS_ID_CODE_LEN+1];       //��ʶ��
		
}ST_CLIENT_INFO;

typedef	struct								 
{
	uint32 iRecordCount;                    //��¼��
	uint32 iUploadCount;                    //���ϴ��ļ�¼��
	
}ST_RECORD_COUNT;


typedef struct								//ϵͳ���ýṹ�塣
{
	ST_SYS_HW 				stHardware;
	ST_NETWORK	            stNet;
	ST_LOCKERS              stLockers;	
	ST_BOX					stBox[MAX_BOX_NUM +1];		
	ST_CARD_KEY             stCardKey;
	ST_CLIENT_INFO          stClientInfo;
	ST_RECORD_COUNT         stRecord;
	
}ST_SYSTEM_CONFIG;


//#pragma pack() //-----������-----PackType--End---������--------------------
//-----------------------------------------------------------------------------------
extern const uchar8 SuperPassword[8];

extern volatile ST_SYSTEM_RUN_INFO gstSysRunInfo;
extern volatile ST_SYSTEM_CONFIG   SysConfig;


extern struct STimer *pTimer_DelayTick;
extern struct STimer *pTimer_TimeCount;
extern struct STimer *pTimer_GetRTC;




//-----------------------------�����洢��ַ��-----------------------------------------------------------------------/
#define MAX_SysConfigArgSize                         0x0000C000             //���ò�����������ֵ--48KByte

#define Addr_SysMarkerInfo                           0x11L			        //�û���Ϣ��0��ַ��ʼ

#define Addr_SysConfig								 0x200L                 //512Byte��ַ��ʼ 

//-----------------------------���ݱ�����ʼ��ַ----------------------------------------------------------------------
#define Addr_SysConfig_Backup_Skewing				 0x2800L                //��10KByte��ַ��ʼ
#define Addr_SysConfig_Backup						 0x2800L 



//-----------------------------Ӳ���ӿ�-----------------------------------------------------------------------------
#define Addr_Hardware_Offset						((uint32)&SysConfig.stHardware - (uint32)&SysConfig)
#define Addr_Hardware								(Addr_SysConfig + Addr_Hardware_Offset)

//-----------------------------�������-----------------------------------------------------------------------------
#define Addr_NetInfo_Offset						    ((uint32)&SysConfig.stNet - (uint32)&SysConfig)
#define Addr_NetInfo								(Addr_SysConfig + Addr_NetInfo_Offset)

//-----------------------------Lockers-------------------------------------------------------------------------------
#define Addr_Lockers_Offset						    ((uint32)&SysConfig.stLockers - (uint32)&SysConfig)
#define Addr_Lockers								(Addr_SysConfig + Addr_Lockers_Offset)


//-----------------------------Ԥ��Ȩ��Ϣ��ַ��----------------------------------------------------------------------/
#define Addr_PreAuthorization                       0x5000L   //�洢��EPPROM��,��20KByte��ַ��ʼ 

//-----------------------------�����¼��ַ��-----------------------------------------------------------------------/
#define Addr_RecordStart                            0x100000L        //��1MByte��ַ��ʼ
#define Addr_RecordEnd                             (0x200000L - ONE_RECORD_LEN)   //д��¼���ݵ�����ַ(1MByte�ռ�)



/*******************************************************************************
* Function Name  : void GPIO_ConfigInit(void)
* Description    : GPIO��ʼ��
* Input          : None
* Output         : None
* Return         : None
* Attention		 : SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
*******************************************************************************/
extern void GPIO_ConfigInit(void);

/*******************************************************************************
* Function Name  : Get_MCU_ID
* Description    : ��ȡMCU��ΨһID
* Input          : pMcuID -- ID�����ָ��
* Output         : None
* Return         : None
* Attention		 : STM32 96λ(12�ֽ�)�Ĳ�ƷΨһ��ݱ�ʶ ����ַ��0x1FFF F7E8
*******************************************************************************/            
extern void Get_MCU_ID(uchar8* pMcuID);

/*******************************************************************************
* Function Name  : Calc_CRC
* Description    : ����CRC
* Input          : pBuf -- ����ָ��, len -- ���ݳ���
* Output         : None
* Return         : CRC8
* Attention		 : �������ֵΪ 0xffffff / (HCLK / 8000000)
*******************************************************************************/            
extern uchar8  Calc_CRC(uchar8 *pBuf, uint32 len);

/*******************************************************************************
* Function Name  : delay_us
* Description    : ��ʱ���� -- ��λ us  (��ʾ��������,����ʱʱ���Ǿ�׼��)
* Input          : - Nus: ��ʱus
* Output         : None
* Return         : None
* Notices		 : �������ֵΪ 0xffffff / (HCLK / 8000000)
*******************************************************************************/            
extern void delay_us(uint32 Nus);

/*******************************************************************************
* Function Name  : delay_ms
* Description    : ��ʱ���� -- ��λ ms
* Input          : - nms: ��ʱms
* Output         : None
* Return         : None
* Attention		 : �������ֵ nms<=0xffffff*8*1000/SYSCLK ��72M������,nms<=1864 
*******************************************************************************/  
extern void delay_ms(uint32 nms);

/*******************************************************************************
* Function Name  : TimingDelay_Decrement
* Description    : Decrements the TimingDelay variable.
* Input          : None
* Output         : TimingDelay
* Return         : None
*******************************************************************************/
extern void TimingDelay_Decrement(void *Arg);

/*******************************************************************************
* Function Name  : TimeCount
* Description    : ��ʱ��
* Input          : None
* Output         : None
* Return         : None
* Notes          : pTimer_TimeCount = TimerCreate(1, 1, TICK_COUNT_10MS, TimeCount, NULL);
*******************************************************************************/
extern void TimeCount(void *Arg);

/*******************************************************************************
* Function Name  : GetRTCTime
* Description    : ��ȡRTCʱ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern void GetRTCTime(void *Arg);


/***********************************************************************************************
* Function Name  : Store_Parameters
* Description	 : �������洢��Flash(�Զ�����)
* Input 		 : Addr -- �洢��ַ�� pBuf -- ����ָ�룬 BufLen -- ���ݳ���
* Output		 : None
* Return		 : 0���� ����0�ο��������
***********************************************************************************************/
extern int Store_Parameters(uint32 Addr, uchar8* pBuf, uint32 BufLen);

/***********************************************************************************************
* Function Name  : Read_Parameters
* Description	 : ��Flash��ȡ����
* Input 		 : Addr -- �洢��ַ�� pBuf -- ����ָ�룬 BufLen -- ���ݳ���
* Output		 : None
* Return		 : 0���� ����0�ο��������
***********************************************************************************************/
extern int Read_Parameters(uint32 Addr,uchar8 * pBuf,uint32 ReadLen);

/***********************************************************************************************
* Function Name  : SysConfig_Init
* Description	 : ϵͳ���ò�����ʼ�� -- �ָ���������
* Input 		 : NULL
* Output		 : None
* Return		 : NULL
***********************************************************************************************/
extern int SysConfig_Init(void);

/***********************************************************************************************
* Function Name  : ReadSysConfig
* Description	 : ��ȡϵͳ���ò��� 
* Input 		 : NULL
* Variable       : SysConfig  
* Output		 : None
* Return		 : 0--�ɹ�����0--ʧ��(�����������)
***********************************************************************************************/
extern int ReadSysConfig(void);

/***********************************************************************************************
* Function Name  : CheckSysValidityDate
* Description	 : ��Ч�ڼ��
* Input 		 : NULL
* Variable       : SysConfig  
* Output		 : None
* Return		 : 0x00 -- δ������Ч��, ����Ϊ��Ч����(��),С��0��ʾ�ѹ�������
***********************************************************************************************/
extern int	CheckSysValidityDate(void);

/*========================File end======================================================================*/
#endif

