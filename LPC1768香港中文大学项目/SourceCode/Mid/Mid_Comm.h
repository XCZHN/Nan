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
**  Function:	ͨѶ���ս��롢����������봦��
==
**  Others:     
==
**  Data:       2014/04/28                                                          
================================================================================================*/

#ifndef	_MID_COMM_H_
#define	_MID_COMM_H_

#include "Config.h"

/*===============================================================================================
Э��汾��¼:

=��= V10 =��=  
1. ��ʼ�汾,CRCΪ�ۼӺ�У��,���ݲ�����;

=��= V20 =��= 
1.CRC--Ϊ���У��;

================================================================================================*/
typedef	enum										//ϵͳͨѶ��ʽ��
{
	COMM_TYPE_FREE = 0,								//����״̬	
	COMM_TYPE_UART,									//����ͨѶ 
	COMM_TYPE_RS485, 								//RS485ͨѶ(UART2)
	COMM_TYPE_RFM,									//RF433ͨѶ
	COMM_TYPE_TCP_CLIENT,                           //��ΪTCP�ͻ����������ͨ��
	COMM_TYPE_TCP_SERVER,                           //��ΪTCP�������������豸ͨѶ
	COMM_TYPE_UDP_CLIENT,							//��ΪUDP�ͻ����������ͨ��
	COMM_TYPE_UDP_SERVER,							//��ΪUDP�������������豸ͨѶ
	COMM_TYPE_GPRS,                                 //GPRS										
	COMM_TYPE_WIFI, 								//WIFI								
	COMM_TYPE_RS485_SLAVE = 0xCA,					//RS485��Ϊ�ӻ�ͨѶ
	COMM_TYPE_RS485_HOST = 0xAC,					//RS485��Ϊ����ͨѶ(UART2,��������չ�����ذ�ͨ��)
	COMM_TYPE_UART_03,								//����3ͨѶ 
	COMM_TYPE_USB									 	

}E_COMM_TYPE_TABLE;


#define CMD_PACKET_MIN_LEN          (14)           //�������ݰ���С���� 
#define CMD_VALIDITY_PACKET         (0xAC)         //��Ч���������ݰ� 
#define CMD_NULLITY_PACKET          (0xCC)         //��ЧЧ���������ݰ� 
//====================Define command format======================================================
#define CMD_HEADER					(0xAA)
#define CMD_TYPE_ASK				(0xAC)         //�������� -- ����
#define CMD_TYPE_REPLY				(0xCA)         //�������� -- Ӧ��
#define CMD_VERSION                 (0x20)         //��ǰЭ��汾
#define CMD_KEY_VERSION             (0x00)         //��ǰЭ�����İ汾

#define	MAX_CMD_DATA_LENGTH	         1024
#define CMD_CALC_CRC				(0xCC)		   //�����������У��CRC���������ҪУ��CRC�����δ�����
#define CMD_END 					(0xBB)


//--------------------���ս��벿��-----------------------------------------

typedef struct 							//���ݽ��ս���ṹ��
{
	unsigned char  u8RxdData;			//���ڽ��յ�������
	unsigned char  u8Step;				//���ս��벽��
	unsigned char  u8KeyVer;			//���İ汾
	unsigned char  u8CmdData;			//
	unsigned char  u8Crc;				//

	unsigned short u16DataCnt;			//���յ������ݳ���
	
}ST_RXD_DECODING;	

typedef struct 							//ͨѶ���ݰ��ṹ��
{
	unsigned long   u32MachineNum;		//����
	unsigned short  u16length;          //���ݳ���

	unsigned char   u8AskOrReplay;		//������Ӧ��
	unsigned char	u8MachineType;		//����
	unsigned char   u8Command;			//������
	unsigned char   DataBuf[1024];
	unsigned char   bGetDataComplete;	//���������Ա�־

}ST_CMD_PACKET;	

//----------------------------RS485�ӻ�����--------------------------------------------
typedef struct 							   //ͨѶ���ݰ��ṹ��
{
	unsigned long   iLockerNum;		       //����
	unsigned short  u16length;             //���ݳ���

	unsigned char   u8Cmd;			       //������
	unsigned char   DataBuf[128];

}ST_CMD_SLAVE_PACKET;	

extern ST_RXD_DECODING  Rxd_Decoding;
extern ST_CMD_PACKET	gstCmdPacket;
extern ST_CMD_SLAVE_PACKET  stCmdSlavePacket;


/***********************************************************************************************
* Function Name  : Comm_Decoding
* Description	 : ��Э���������
* Input 		 : u8data -- ��������
* Output		 : None
* Return		 : None 
* Others		 : None
***********************************************************************************************/
extern void Comm_Decoding(uchar8 u8data);

/***********************************************************************************************
* Function Name  : Comm_PacketDeal
* Description	 : ���ݰ�����
* Input 		 : pPacket -- ���ݰ�ָ��, u32Len -- ���ݳ���, CommType -- ͨ�ŷ�ʽ
* Output		 : None
* Return		 : None 
* Others		 : CMD_VALIDITY_PACKET -- ��Ч, ��Ч�򷵻ش������
***********************************************************************************************/
extern int Comm_PacketDeal(uchar8* pPacket, uint32 u32Len, uchar8 CommType);

/***********************************************************************************************
* Function Name  : CommDeal_Reply
* Description	 : ͨѶָ��ִ�к�Ļظ�Э��ӿ�
* Input 		 : u8Cmd:������ pBuf:����ָ��, u8Status:ִ��״̬ len:���ݳ���
* Output		 : None
* Return		 : None 
* Others		 : None
***********************************************************************************************/
extern void CommDeal_Reply(uchar8 u8Cmd, uchar8 u8Status, uchar8* pBuf, uint32 u32Len);


/***********************************************************************************************
* Function Name  : Comm_ApplyFor
* Description	 : ���������ͨѶ
* Input 		 : u8Cmd:������ pBuf:����ָ��, u32Len:���ݳ��� CommType:ͨѶ��ʽ
* Output		 : None
* Return		 : None 
* Others		 : None
***********************************************************************************************/
extern void Comm_ApplyFor(uchar8 u8Cmd, uchar8* pBuf, uint32 u32Len, uchar8 CommType);


/***********************************************************************************************
* Function Name  : SendCmdToExtendLockers
* Description	 : ��ָ�������
* Input 		 : LockersNo -- �������, u8Cmd--������, pBuf --��������, len--���ݳ���
* Output		 : None
* Return		 : None 
* Note  		 : None 
***********************************************************************************************/
extern void SendCmdToExtendLockers(uint32 LockersNo, uchar8 u8Cmd, uchar8* pBuf, uint16 len);

/***********************************************************************************************
* Function Name  : SlavePacketDecoding
* Description	 : ��Э������ӻ��ظ������ݰ�
* Input 		 : pPacket -- ������������ָ��, Len--���ݳ��� 
* Output		 : None
* Return		 : 0--��Ч���ݰ�, ����--��Ч���ݰ��������� 
* Others		 : None
***********************************************************************************************/
extern uchar8 SlavePacketDecoding(uchar8* pPacket, uint32 Len, ST_CMD_SLAVE_PACKET* pstCmdPacket);


#endif
/*======================================File end========================================================================*/

