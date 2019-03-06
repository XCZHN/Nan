/*===============================================================================================
==                                                                                              
**  Copyright(c) 2014  Intelligent Admin electronic systems co., LTD. All rights reserved.
==                                                                                           
**	Author:		Asen                                                                     
==                                                                                          
**  Version:	V2.0.0                                                                          
==
**  Filename:	App_Comm.c                                                                       
==                                                                                         
**  Function:	通讯接收解码、发送组包编码处理
==
**  Others:     
==
**  Data:       2014/04/28                                                          
================================================================================================*/

#ifndef	_MID_COMM_H_
#define	_MID_COMM_H_

#include "Config.h"

/*===============================================================================================
协议版本记录:

=★= V10 =★=  
1. 初始版本,CRC为累加和校验,数据不加密;

=★= V20 =★= 
1.CRC--为异或校验;

================================================================================================*/
typedef	enum										//系统通讯方式表
{
	COMM_TYPE_FREE = 0,								//空闲状态	
	COMM_TYPE_UART,									//串口通讯 
	COMM_TYPE_RS485, 								//RS485通讯(UART2)
	COMM_TYPE_RFM,									//RF433通讯
	COMM_TYPE_TCP_CLIENT,                           //作为TCP客户端与服务器通信
	COMM_TYPE_TCP_SERVER,                           //作为TCP服务器与其它设备通讯
	COMM_TYPE_UDP_CLIENT,							//作为UDP客户端与服务器通信
	COMM_TYPE_UDP_SERVER,							//作为UDP服务器与其它设备通讯
	COMM_TYPE_GPRS,                                 //GPRS										
	COMM_TYPE_WIFI, 								//WIFI								
	COMM_TYPE_RS485_SLAVE = 0xCA,					//RS485作为从机通讯
	COMM_TYPE_RS485_HOST = 0xAC,					//RS485作为主机通讯(UART2,用于与扩展的锁控板通信)
	COMM_TYPE_UART_03,								//串口3通讯 
	COMM_TYPE_USB									 	

}E_COMM_TYPE_TABLE;


#define CMD_PACKET_MIN_LEN          (14)           //命令数据包最小长度 
#define CMD_VALIDITY_PACKET         (0xAC)         //有效的命令数据包 
#define CMD_NULLITY_PACKET          (0xCC)         //无效效的命令数据包 
//====================Define command format======================================================
#define CMD_HEADER					(0xAA)
#define CMD_TYPE_ASK				(0xAC)         //命令类型 -- 请求
#define CMD_TYPE_REPLY				(0xCA)         //命令类型 -- 应答
#define CMD_VERSION                 (0x20)         //当前协议版本
#define CMD_KEY_VERSION             (0x00)         //当前协议密文版本

#define	MAX_CMD_DATA_LENGTH	         1024
#define CMD_CALC_CRC				(0xCC)		   //声明命令必须校验CRC，如果不需要校验CRC则屏蔽此声明
#define CMD_END 					(0xBB)


//--------------------接收解码部分-----------------------------------------

typedef struct 							//数据接收解码结构体
{
	unsigned char  u8RxdData;			//串口接收到的数据
	unsigned char  u8Step;				//接收解码步伐
	unsigned char  u8KeyVer;			//密文版本
	unsigned char  u8CmdData;			//
	unsigned char  u8Crc;				//

	unsigned short u16DataCnt;			//接收到的数据长度
	
}ST_RXD_DECODING;	

typedef struct 							//通讯数据包结构体
{
	unsigned long   u32MachineNum;		//机号
	unsigned short  u16length;          //数据长度

	unsigned char   u8AskOrReplay;		//请求还是应答
	unsigned char	u8MachineType;		//机型
	unsigned char   u8Command;			//命令字
	unsigned char   DataBuf[1024];
	unsigned char   bGetDataComplete;	//命令完整性标志

}ST_CMD_PACKET;	

//----------------------------RS485从机数据--------------------------------------------
typedef struct 							   //通讯数据包结构体
{
	unsigned long   iLockerNum;		       //机号
	unsigned short  u16length;             //数据长度

	unsigned char   u8Cmd;			       //命令字
	unsigned char   DataBuf[128];

}ST_CMD_SLAVE_PACKET;	

extern ST_RXD_DECODING  Rxd_Decoding;
extern ST_CMD_PACKET	gstCmdPacket;
extern ST_CMD_SLAVE_PACKET  stCmdSlavePacket;


/***********************************************************************************************
* Function Name  : Comm_Decoding
* Description	 : 按协议解析命令
* Input 		 : u8data -- 接收数据
* Output		 : None
* Return		 : None 
* Others		 : None
***********************************************************************************************/
extern void Comm_Decoding(uchar8 u8data);

/***********************************************************************************************
* Function Name  : Comm_PacketDeal
* Description	 : 数据包处理
* Input 		 : pPacket -- 数据包指针, u32Len -- 数据长度, CommType -- 通信方式
* Output		 : None
* Return		 : None 
* Others		 : CMD_VALIDITY_PACKET -- 有效, 无效则返回错误代码
***********************************************************************************************/
extern int Comm_PacketDeal(uchar8* pPacket, uint32 u32Len, uchar8 CommType);

/***********************************************************************************************
* Function Name  : CommDeal_Reply
* Description	 : 通讯指令执行后的回复协议接口
* Input 		 : u8Cmd:命令字 pBuf:数据指针, u8Status:执行状态 len:数据长度
* Output		 : None
* Return		 : None 
* Others		 : None
***********************************************************************************************/
extern void CommDeal_Reply(uchar8 u8Cmd, uchar8 u8Status, uchar8* pBuf, uint32 u32Len);


/***********************************************************************************************
* Function Name  : Comm_ApplyFor
* Description	 : 主动申请的通讯
* Input 		 : u8Cmd:命令字 pBuf:数据指针, u32Len:数据长度 CommType:通讯方式
* Output		 : None
* Return		 : None 
* Others		 : None
***********************************************************************************************/
extern void Comm_ApplyFor(uchar8 u8Cmd, uchar8* pBuf, uint32 u32Len, uchar8 CommType);


/***********************************************************************************************
* Function Name  : SendCmdToExtendLockers
* Description	 : 发指令到副柜上
* Input 		 : LockersNo -- 副柜机号, u8Cmd--命令字, pBuf --命令数据, len--数据长度
* Output		 : None
* Return		 : None 
* Note  		 : None 
***********************************************************************************************/
extern void SendCmdToExtendLockers(uint32 LockersNo, uchar8 u8Cmd, uchar8* pBuf, uint16 len);

/***********************************************************************************************
* Function Name  : SlavePacketDecoding
* Description	 : 按协议解析从机回复的数据包
* Input 		 : pPacket -- 待解析数据区指针, Len--数据长度 
* Output		 : None
* Return		 : 0--无效数据包, 其它--有效数据包的命令字 
* Others		 : None
***********************************************************************************************/
extern uchar8 SlavePacketDecoding(uchar8* pPacket, uint32 Len, ST_CMD_SLAVE_PACKET* pstCmdPacket);


#endif
/*======================================File end========================================================================*/

