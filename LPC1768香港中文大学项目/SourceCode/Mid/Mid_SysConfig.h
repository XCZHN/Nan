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
**  Function:	系统参数配置文件
==                                                                                         
**  Data:       2014/04/28                                                                       
================================================================================================*/

#ifndef	_APP_SYS_CONFIG_H_
#define	_APP_SYS_CONFIG_H_

#include "Includes.h"


//#define GUI_ENGLISH                          0xEB

#define VALIDITY_USING                       0xEB    //有效标记
#define INPUT_TYPE_ABC                       0xAB    //输入法--ABC
#define ALREADY_EXIST                        0xAE    //已存在标识
#define NOTICE_STAY_TIME                     2000    //提示信息停留时间(ms)
#define MESSAGE_STAY_TIME                    3000    //提示消息停留时间(ms)
#define MARK_CHOICE_ALL_BOX                  255     //选中所有箱的标识
#define SYS_ID_CODE_LEN                      6       //系统标识码长度

#define Default_Box_Count          			 24      //默认的箱数
#define MAX_BOX_NUM          			 	 96      //最大总箱数
#define MAX_AdminCardNum                     6       //绑定的管理卡数量
#define MAX_BoxCardNum                       1       //每个箱绑定的卡数量
#define MAX_PasswordLen                      6       //密码最大长度
#define MAX_OpenBoxKeyLen                    6       //开箱密码最大长度
#define MAX_BoxSnLen                         2       //箱编码最大长度
#define MAX_NameLen                          6       //名称的最大长度
#define MAX_UserNameLen                      8       //用户姓名的最大长度
#define MAX_UserIdLen                        10      //工号的最大长度

#define MAX_VALID_CAED_COUNT          		 2560    //最大预授权卡数量(4*2560=10KByte)
#define MAX_UserFingerprintNum               1       //每个用户最大指纹数量


#define SUPER_CARD_ID                        343056036L
                   
//#pragma pack(1)  //-----☆☆☆☆☆-----PackType--Strrt---☆☆☆☆☆---------------------------


//-----------------------------------Sys run info--------------------------------------/
typedef enum 							    //系统状态表
{
	SYS_STATUS_IDLE = 0,				 	//空闲状态	
	SYS_STATUS_KEY,							//按键操作状态
	SYS_STATUS_CARD,                        //刷卡状态
	SYS_STATUS_LOCKED,                      //锁机状态
	SYS_STATUS_JiCun,						//寄存状态
	SYS_STATUS_QuWu,						//取物状态
	SYS_STATUS_ZW,                          //指纹操作
	SYS_STATUS_Message,                     //显示提示信息
	SYS_STATUS
	
}E_SYS_STATUS;

typedef  struct  							//系统运行参数结构体
{	
	uint32 u32Temp;							//用于计算时的临时调用
	uint32 u32RtcTime_S;					//RTC时间(HH*3600 + MM*60 + SS)
	uint32 u32TimingDelay;					//延时计数
	uint32 u32TimeCount_ms;					//时间计数(ms)
	uint32 u32LedBlinkTimes;				//LED闪烁次数
	uint32 iLastCommTime;                   //上次成功通讯时间(ms)
	uint32 iHandleTimeout;                  //寄存关门超时提示(s)

	uchar8 CurrentStatus;					//记录系统当前的运行状态
	uchar8 TimeBuf[8];						//(Hex)时间存放区
	uchar8 TempBuf[128];					//临时存储区
	uchar8 CommType;						//通讯方式 
	uchar8 LastCommType;					//上次通讯方式 
	uchar8 u8Temp;							//临时变量
	uchar8 bTxdEnable;						//通讯发送控制
	uchar8 bRS485Busy;                      //RS485忙碌(占用)标记
	uchar8 bLockedSys;                      //系统(过期)被锁
	uchar8 bForbidTimeGoBack;               //禁止时间往回走 

	uchar8 bNetResetSys;					//网络重启系统标记(=0xEB有效)
	uchar8 u8UseBox;		         		//用戶寄存的箱号	
	uchar8 bDebug_Net;                      //调试网络开关(串口输出调试信息) 0 -- 关闭,非0 --开启

	uchar8 u8EmptyBoxCount;                 //当前空箱数  
	uchar8 EmptyBox[MAX_BOX_NUM+2];         //当前空箱
	
	uchar8 BoxDoorStatus[MAX_BOX_NUM+1];    //箱门状态(字节0 -- N 对应0--N号箱门, 0表示关闭,1表示开启)
	uchar8 BoxInsideStatus[MAX_BOX_NUM+1];  //箱内状态(字节0 -- N 对应0--N号箱门, 1表示有物品,0表示空箱(没有物品))
		
	ST_TIME stRtcTime;						//RTC时钟(与TimeBuf[]值相同)
	
}ST_SYSTEM_RUN_INFO;


//---------------------------------------Sysconfig----------------------------------------------

typedef	struct								//系统硬件配置结构体。
{
	uint32 u32ConfigVer;					//参数版本(结构:YYMMDD+SRN, 15090301)
	uint32 u32MachineNum;					//(HEX)机号
	uint32 u32Baudrate;						//串口通讯波特率
	uint32 u32BeatInterval;				    //心跳包时间间隔(ms)

	uchar8 bDebugMode;                      //调试模式
	uchar8 CommType;					    //通讯方式 		
	uchar8 u8MachineType;					//机型
	uchar8 u8RS485Type;						//RS485(0--从机, 1--主机 )
	uchar8 u8KeyType;						//键盘类型(0--触摸, 1--矩阵 )
	uchar8 bABCSwitch;						//字母输入开关,0-- 不授权, 非0授权
	uchar8 bNetwork;						//网络开关,VALIDITY_USING --开启
	uchar8 u8VoiceLevel;					//语音音量,0-31级,0--表示关闭		
	uchar8 u8Crc;
		
}ST_SYS_HW;

typedef	struct								//网络参数结构体。
{
	uint16 u16UDP_ListenPort;				//UDP本机监听端口
	uint16 u16UDP_ServerPort;               //UDP服务器端口
	uint16 u16TCP_ListenPort;				//TCP本机监听端口
	uint16 u16TCP_ServerPort;               //TCP服务器端口
	
	uchar8 MAC[6];                          //MAC地址
	uchar8 IP[6];                           //IP地址
	uchar8 Gateway[6];                      //网关
	uchar8 NetMask[6];                      //网络掩码
	
	uchar8 ServerIP[6];                     //服务器ip
	uchar8 ServerMAC[6];                    //服务器MAC地址
	uchar8 bAutoConnectServer;				//主动链接服务器
	uchar8 u8Crc;
		
}ST_NETWORK;

//---------------------------LOCKERS---------------------------------------------------

typedef enum 							    //Box状态表
{
	BOX_STATUS_IDLE = 0,				 	//空闲状态	
	BOX_STATUS_OCCUPY,                   	//已投递的占用状态
	BOX_STATUS
	
}E_BOX_STATUS;

typedef enum 							    //Box锁箱状态
{
	LSTATUS_FREE = 0,				 	    //未被锁 	
	LSTATUS_LOCKING,				    	//被封箱
	
}E_BOX_LOCKING_STATUS;


typedef	struct								 
{ 
	uint32 iRecordCount;                    //开箱记录累计
	uint32 iUserCardID;						//关联的卡号

	uint16 iTouchID;                        //关联的指纹号
			
	uchar8 u8BoxID;        			 	    //箱号
	uchar8 u8SizeType;						//类型(0-小箱, 1-中箱, 2-大箱)
	uchar8 u8Status;   						//状态	
	uchar8 bLocking;                        //被锁标记

	uchar8 BoxSN[MAX_BoxSnLen+1];		    //编码名称 
	uchar8 UserName[MAX_UserNameLen+1];		//使用者姓名 
	uchar8 UserID[MAX_UserIdLen+1];		    //使用者工号	
	uchar8 LastUsedTime[6];                 //上次使用时间(YY-MM-DD HH:MM:SS)
		
}ST_BOX;


typedef enum 							    //工作模式表
{
	WORK_MODE_ANY_CARD = 0,				 	//任一卡随机寄存模式
	WORK_MODE_VALID_CARD,                   //预授权的有效卡
	WORK_MODE_WRITE_CARD,                   //写卡模式
	WORK_MODE_ONLINE,                       //在线验证模式
	WORK_MODE_ANY_FINGER,                   //任一指纹
	WORK_MODE_ANY_FINGER_CARD,              //任一指纹或任一卡
	WORK_MODE_VALID_FINGER,                 //预授权的指纹
	WORK_MODE_ONLINE_FINGER,                //在线验证指纹模式
	WORK_MODE_
	
}WORK_MODE;

typedef	struct								 
{			
	uint32 iValidCardCount;                 //预授权卡数量
	uint32 iMinFetchTime;                   //最小存储时间(s)
	uint32 iMaxFetchTime;                   //最小存储时间(s)
	uint32 iHandleTimeout;                  //操作超时时间(s)
	uint32 iNoticeTime;                     //操作提示时长(s)
	uint32 iReserved;                       //预留
	uint32 AdminCard[MAX_AdminCardNum];     //管理卡
		
	uchar8 u8BoxCount;             	        //箱格数量
	uchar8 u8HostLockerBoxs;                //主柜的箱数
	uchar8 u8ExtL01BoxNum;                  //副柜1的箱数
	uchar8 u8ExtL02BoxNum;                  //副柜2的箱数
	uchar8 u8ExtL03BoxNum;                  //副柜3的箱数
	uchar8 bCheckBoxDoorStatus;             //检测箱门开启状态
	uchar8 bEnableIdCard;                   //授权兼容ID卡
	uchar8 u8Reserved02;	

	uchar8 u8WorkMode;			            //工作模式(固定\随机) 		
	uchar8 u8OpenBoxMode;                   //开箱方式
	uchar8 u8AllowBeOpenedBoxNum;           //寄存的时候允许开门数量
	uchar8 Name[MAX_NameLen+1];		        //名称(预留一字节空格)	
	uchar8 SuperPassword[MAX_PasswordLen+1];  //超级管理员密码
	uchar8 SeniorPassword[MAX_PasswordLen+1]; //高级管理员密码
	uchar8 AdminPassword[MAX_PasswordLen+1];  //用户管理员登录密码
	uchar8 Reserved[10];
		
}ST_LOCKERS;

typedef	struct								 
{
	uchar8 u8Sector;                        //卡扇区
	uchar8 u8KeyType;                       //密钥类型
	uchar8 Password[6];                     //密钥
	
}ST_CARD_KEY;

//-----------------------------Client Info--------------------------------------------
typedef	struct								 
{	
	uchar8 Name[30];					    //单位名称
	uchar8 ValidityDate[3]; 				//有效期
	uchar8 IdCode[SYS_ID_CODE_LEN+1];       //标识码
		
}ST_CLIENT_INFO;

typedef	struct								 
{
	uint32 iRecordCount;                    //记录数
	uint32 iUploadCount;                    //已上传的记录数
	
}ST_RECORD_COUNT;


typedef struct								//系统配置结构体。
{
	ST_SYS_HW 				stHardware;
	ST_NETWORK	            stNet;
	ST_LOCKERS              stLockers;	
	ST_BOX					stBox[MAX_BOX_NUM +1];		
	ST_CARD_KEY             stCardKey;
	ST_CLIENT_INFO          stClientInfo;
	ST_RECORD_COUNT         stRecord;
	
}ST_SYSTEM_CONFIG;


//#pragma pack() //-----☆☆☆☆☆-----PackType--End---☆☆☆☆☆--------------------
//-----------------------------------------------------------------------------------
extern const uchar8 SuperPassword[8];

extern volatile ST_SYSTEM_RUN_INFO gstSysRunInfo;
extern volatile ST_SYSTEM_CONFIG   SysConfig;


extern struct STimer *pTimer_DelayTick;
extern struct STimer *pTimer_TimeCount;
extern struct STimer *pTimer_GetRTC;




//-----------------------------参数存储地址表-----------------------------------------------------------------------/
#define MAX_SysConfigArgSize                         0x0000C000             //配置参数允许的最大值--48KByte

#define Addr_SysMarkerInfo                           0x11L			        //用户信息从0地址开始

#define Addr_SysConfig								 0x200L                 //512Byte地址开始 

//-----------------------------数据备份起始地址----------------------------------------------------------------------
#define Addr_SysConfig_Backup_Skewing				 0x2800L                //从10KByte地址开始
#define Addr_SysConfig_Backup						 0x2800L 



//-----------------------------硬件接口-----------------------------------------------------------------------------
#define Addr_Hardware_Offset						((uint32)&SysConfig.stHardware - (uint32)&SysConfig)
#define Addr_Hardware								(Addr_SysConfig + Addr_Hardware_Offset)

//-----------------------------网络参数-----------------------------------------------------------------------------
#define Addr_NetInfo_Offset						    ((uint32)&SysConfig.stNet - (uint32)&SysConfig)
#define Addr_NetInfo								(Addr_SysConfig + Addr_NetInfo_Offset)

//-----------------------------Lockers-------------------------------------------------------------------------------
#define Addr_Lockers_Offset						    ((uint32)&SysConfig.stLockers - (uint32)&SysConfig)
#define Addr_Lockers								(Addr_SysConfig + Addr_Lockers_Offset)


//-----------------------------预授权信息地址表----------------------------------------------------------------------/
#define Addr_PreAuthorization                       0x5000L   //存储在EPPROM中,从20KByte地址开始 

//-----------------------------开箱记录地址表-----------------------------------------------------------------------/
#define Addr_RecordStart                            0x100000L        //从1MByte地址开始
#define Addr_RecordEnd                             (0x200000L - ONE_RECORD_LEN)   //写记录数据的最大地址(1MByte空间)



/*******************************************************************************
* Function Name  : void GPIO_ConfigInit(void)
* Description    : GPIO初始化
* Input          : None
* Output         : None
* Return         : None
* Attention		 : SYSTICK的时钟固定为HCLK时钟的1/8
*******************************************************************************/
extern void GPIO_ConfigInit(void);

/*******************************************************************************
* Function Name  : Get_MCU_ID
* Description    : 读取MCU的唯一ID
* Input          : pMcuID -- ID存放区指针
* Output         : None
* Return         : None
* Attention		 : STM32 96位(12字节)的产品唯一身份标识 基地址：0x1FFF F7E8
*******************************************************************************/            
extern void Get_MCU_ID(uchar8* pMcuID);

/*******************************************************************************
* Function Name  : Calc_CRC
* Description    : 计算CRC
* Input          : pBuf -- 数据指针, len -- 数据长度
* Output         : None
* Return         : CRC8
* Attention		 : 参数最大值为 0xffffff / (HCLK / 8000000)
*******************************************************************************/            
extern uchar8  Calc_CRC(uchar8 *pBuf, uint32 len);

/*******************************************************************************
* Function Name  : delay_us
* Description    : 延时函数 -- 单位 us  (经示波器测试,此延时时间是精准的)
* Input          : - Nus: 延时us
* Output         : None
* Return         : None
* Notices		 : 参数最大值为 0xffffff / (HCLK / 8000000)
*******************************************************************************/            
extern void delay_us(uint32 Nus);

/*******************************************************************************
* Function Name  : delay_ms
* Description    : 延时函数 -- 单位 ms
* Input          : - nms: 延时ms
* Output         : None
* Return         : None
* Attention		 : 参数最大值 nms<=0xffffff*8*1000/SYSCLK 对72M条件下,nms<=1864 
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
* Description    : 计时器
* Input          : None
* Output         : None
* Return         : None
* Notes          : pTimer_TimeCount = TimerCreate(1, 1, TICK_COUNT_10MS, TimeCount, NULL);
*******************************************************************************/
extern void TimeCount(void *Arg);

/*******************************************************************************
* Function Name  : GetRTCTime
* Description    : 读取RTC时间
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern void GetRTCTime(void *Arg);


/***********************************************************************************************
* Function Name  : Store_Parameters
* Description	 : 将参数存储至Flash(自动备份)
* Input 		 : Addr -- 存储地址， pBuf -- 数据指针， BufLen -- 数据长度
* Output		 : None
* Return		 : 0正常 ，非0参考错误代码
***********************************************************************************************/
extern int Store_Parameters(uint32 Addr, uchar8* pBuf, uint32 BufLen);

/***********************************************************************************************
* Function Name  : Read_Parameters
* Description	 : 从Flash读取参数
* Input 		 : Addr -- 存储地址， pBuf -- 数据指针， BufLen -- 数据长度
* Output		 : None
* Return		 : 0正常 ，非0参考错误代码
***********************************************************************************************/
extern int Read_Parameters(uint32 Addr,uchar8 * pBuf,uint32 ReadLen);

/***********************************************************************************************
* Function Name  : SysConfig_Init
* Description	 : 系统配置参数初始化 -- 恢复出厂设置
* Input 		 : NULL
* Output		 : None
* Return		 : NULL
***********************************************************************************************/
extern int SysConfig_Init(void);

/***********************************************************************************************
* Function Name  : ReadSysConfig
* Description	 : 读取系统配置参数 
* Input 		 : NULL
* Variable       : SysConfig  
* Output		 : None
* Return		 : 0--成功，非0--失败(详见错误代码表)
***********************************************************************************************/
extern int ReadSysConfig(void);

/***********************************************************************************************
* Function Name  : CheckSysValidityDate
* Description	 : 有效期检查
* Input 		 : NULL
* Variable       : SysConfig  
* Output		 : None
* Return		 : 0x00 -- 未设置有效期, 其它为有效期限(天),小于0表示已过期天数
***********************************************************************************************/
extern int	CheckSysValidityDate(void);

/*========================File end======================================================================*/
#endif

