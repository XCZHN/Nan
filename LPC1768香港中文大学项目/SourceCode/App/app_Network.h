/*===============================================================================================
==                                                                                              
**  Copyright(c) 2015  ZhenYao Science and technology co., LTD.   All rights reserved.
==                                                                                           
**	Author:		Asen                                                                           
==                                                                                          
**  Version:	v1.0.0                                                                          
==
**  Filename:	Network.h                                                                       
==                                                                                         
**  Function:	����ͨ�ų���ӿ�                                                                 
==
**  Data:       2015-09-11
================================================================================================*/

#ifndef _NETWORK_H_
#define _NETWORK_H_



/*************************************************************
*
**************************************************************/		
//��ַ����  (DA+SA+TYPE+DATA)
#define	E_dest		  0x0000	//Ŀ��MAC��ַ6 byte(0000h~0005H)
#define E_srce		  0x0006	//  ԴMAC��ַ6 byte(0006h~000bh)
#define E_ptype		  0x000c	//Э������2 byte(000ch~000dh)
//����ΪARP���ݱ���ʽ (DATA)
#define HTYPE		  0x000e	//(ARP) Ӳ������2 byte(000eh~000fh)
#define ARPPRO        0x0010	//(ARP) Э������2 byte(0010h~0011h)
#define MACLEN        0x0012	//(ARP) Ӳ������1 byte(0012h)
#define IPLEN         0x0013	//(ARP) Э�鳤��1 byte(0013h)
#define code_type     0x0014	//(ARP) ��������2 byte(0014h~0015h)
#define SMAC          0x0016	//(ARP) ����վMAC��ַ6 byte(0015h~001ah)
#define SIP1          0x001c	//(ARP) ����վIP��ַ4 byte(001bh~001eh)
#define DMAC          0x0020	//(ARP) ����վMAC��ַ6 byte(001fh~0024h)
#define DIP1          0x0026  	//(ARP) ����վIP��ַ4 byte(0025h~0028h)	
//����ΪIP���ݱ��ײ���ʽ(20�ֽ�) (DATA)
#define IP_HDR_VER    0x000e	//(IP)  1BYTE(000EH)
#define IP_HDR_DS     0x000f	//(IP)  1BYTE(000FH)
#define IP_HDR_LEN    0x0010	//(IP)  2BYTE(0010H~0011H)
#define IP_HDR_ident  0x0012	//(IP)  2BYTE(0012H~0013H)
#define IP_HDR_flags  0x0014	//(IP)  2BYTE(0014H~0015H)
#define IP_HDR_TIME   0x0016	//(IP)  1BYTE(0016H)
#define IP_HDR_Pcol   0x0017	//(IP)  1BYTE(0017H)
#define IP_HDR_CHK    0x0018	//(IP)  2BYTE(0018H~0019H)
#define IP_HDR_SIP    0x001a	//(IP)  4BYTE(001AH~001DH)
#define IP_HDR_DIP    0x001e	//(IP)  4BYTE(001EH~0021H)
//����ΪIP����UDP�����ײ���ʽ(8�ֽ�) (DATA)
#define IP_UDP_SPORT  0x0022	//(IP->UDP) 2BYTE(0022H~0023H)
#define IP_UDP_DPORT  0x0024	//(IP->UDP) 2BYTE(0024H~0025h)
#define IP_UDP_LEN    0x0026	//(IP->UDP) 2BYTE(0026H~0027H)
#define IP_UDP_CHK    0x0028	//(IP->UDP) 2BYTE(0028H~0029H)
#define IP_UDP_DATA   0x002a	//(IP->UDP) N byte(UDP���ݲ���)
//����ΪIP����ICMP�����ײ���ʽ(8�ֽ�) (DATA)
#define IP_ICMP_TYPE  0x0022	//(IP->ICMP) 1BYTE(0022H)
#define IP_ICMP_CODE  0x0023	//(IP->ICMP) 1BYTE(0023H)
#define IP_ICMP_CHK   0x0024	//(IP->ICMP) 2BYTE(0024H~0025H)
#define IP_ICMP_ident 0x0026	//(IP->ICMP) 2BYTE(0026H~0027H)
#define IP_ICMP_seq   0x0028	//(IP->ICMP) 2BYTE(0028H~0029H)

//----�û�ͨѶЭ���������ڵ�λ��-----------------------------------
#define CMD_DATA_ADDR   0x002A 


extern void udp_transmit(void);
extern uchar8 do_net_process(void);

/***********************************************************************************************
* Function Name  : ReadNetAddrInfo
* Description    : ��ȡ�����ַ��Ϣ
* Input          : None
* Output         : None
* Return         : None
* Notes          : ��SysConfig.stNet���Ƶ��ֲ�������
***********************************************************************************************/
extern void ReadNetAddrInfo(void);

/***********************************************************************************************
* Function Name  : AutoSave_ServerAddr
* Description    : �Զ������������ַ��Ϣ
* Input          : None
* Output         : None
* Return         : None
* Notes          : None
***********************************************************************************************/
extern void AutoSave_ServerAddr(void);

/***********************************************************************************************
* Function Name  : AutoSave_ServerMAC
* Description    : �Զ����������MAC��ַ 
* Input          : None
* Output         : None
* Return         : None
* Notes          : ������λ����MAC��ַ,�Ա�����������������������
***********************************************************************************************/
extern void AutoSave_ServerMAC(void);

/***********************************************************************************************
* Function Name  : CreateSysMAC
* Description    : ����MAC��ַ
* Input          : None
* Output         : None
* Return         : None
* Notes          : None
***********************************************************************************************/
extern void CreateSysMAC(uchar8 Key, uchar8* pMacBuf);

/***********************************************************************************************
* Function Name  : Network_Init
* Description    : ����ӿڳ�ʼ��
* Input          : None
* Output         : None
* Return         : None
* Notes          : None
***********************************************************************************************/
extern void Network_Init(void);

/***********************************************************************************************
* Function Name  : UdpClient_SendData
* Description    : �豸��UDP����Ϊ�ͻ��˷������ݰ�
* Input          : Data -- ����������, len -- ���ݳ���
* Output         : None
* Return         : None
* Notes          : 
***********************************************************************************************/
extern void UdpClient_SendData(uchar8* pStrBuf, uint16 len);

/***********************************************************************************************
* Function Name  : UdpServer_SendData
* Description    : �豸��UDP����Ϊ�ͻ��˷������ݰ�
* Input          : Data -- ����������, len -- ���ݳ���
* Output         : None
* Return         : None
* Notes          : 
***********************************************************************************************/
extern void UdpServer_SendData(uchar8* pStrBuf, uint16 len);

/***********************************************************************************************
* Function Name  : Thread_UDP
* Description    : UDP�����߳�
* Input          : None
* Output         : None
* Return         : None
* Notes          : None
***********************************************************************************************/
extern void Thread_UDP(void);

#endif //_NETWORK_H_


/*===================================File end===================================================*/

