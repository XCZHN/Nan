/*===============================================================================================
==                                                                                              
**  Copyright(c) 2014  ZhenYao Science and technology co., LTD.   All rights reserved.
==                                                                                           
**	Author:		Asen                                                                     
==                                                                                          
**  Version:	V1.0.0                                                                          
==
**  Filename:	Network.c                                                                       
==                                                                                         
**  Function:	Application Network source file
==
**  Data:       2015/09/11                                                                       
================================================================================================*/

#include "Includes.h"

/*************************************************************
*ȫ�ֱ���˵��
**************************************************************/
uchar8 efp[1024];					//���ջ�����
uint  DLen;                         //�û����ݵĳ���


//���´�ű��غ�Զ�̷������̶�MAC,IP,Mask,gate,Port,�ϵ��flash�ֶ�ȡ
uchar8 SR_MAC[6];				//����վ(����)�����ַ6 BYTE
uchar8 S_IP[4];					//����վ(����)IP��ַ  4 BYTE
uchar8 S_MASK[4];				//����վ(����)��������4 BYTE
uchar8 S_gate[4];				//����վ(����)����    4 BYTE
uchar8 S_PORT[2];				//����վ(����)�˿�    2 BYTE

uchar8 DE_MAC[6];				//����վ(PC��λ��)�����ַ6 BYTE
uchar8 D_IP[4];					//����վ(PC��λ��)IP��ַ  4 BYTE
uchar8 D_mask[4];				//����վ(PC��λ��)��������4 BYTE
uchar8 D_gate[4];				//����վ(PC��λ��)����    4 BYTE
uchar8 D_port[2];				//����վ(PC��λ��)�˿�    2 BYTE

//���´�ű��غ�Զ����ʱMAC,IP,Mask,gate,Port(FE00H~)
//����Ŀ�ĵ�ַ����ʱ������ĵ�ַ�źţ������ݷ��ظ�Ŀ�ĵ�ʹ��
uchar8 LOC_MAC[6];				//(����)�����ַ
uchar8 LOC_IP[4];				//(����)IP��ַ
uchar8 LOC_MASK[4];				//(����)��������
uchar8 LOC_gate[4];				//(����)����
uchar8 LOC_PORT[2];				//(����)�˿�

uchar8 Dest_MAC[6];				//(PC��λ��)�����ַ
uchar8 Dest_IP[4];				//(PC��λ��)IP��ַ
uchar8 Dest_MASK[4];			//(PC��λ��)��������
uchar8 Dest_gate[4];			//(PC��λ��)���� 
uchar8 Dest_PORT[2];			//(PC��λ��)�˿� 

/**********************************************************
*Ч��ͣ����ײ�����Ϊ16λ�ĶΣ����Ѹ�����ӣ��ڰѺ�ȡ������ΪЧ���
*����ֽ���Ϊ���������һ���ֽڲ�0
***********************************************************/
uint csum(uchar8 dp[],uint count)
{
	ulong total=0;
	uint n,i,carries;
	
	n=count/2;
	for(i=0;i<n;i++)
	{
		total+=((dp[i*2]&0x00ff)<<8)+(dp[i*2+1]&0xff);
	}
	if(count&1)
	{
		total+=(dp[i*2]&0x00ff)<<8;
	}
	while ((carries=(uint)(total>>16))!=0)
    	total = (total & 0xffff) + carries;
    return((uint)total);
}

/**********************************************************
*UDP��У��Ͳ�ͬ����ļ���,�����У��Ͱ���3���֣�α�ײ���UDP�ײ���Ӧ�ò���������
*����α�ײ�ΪIP������ײ�һ���֣�������Щ�ֶ�Ҫд0
*α�ײ���ʽ����:
*     32λԴIP��ַ
*     32λĿ��IP��ַ
*ȫ0 + 8λЭ��(17) + 16λUDP�ܳ���
***********************************************************/
uint check_udp(uint count)
{
	ulong sum;
	uchar8 tm_buff[12];
	memcpy(tm_buff,S_IP,4);
	memcpy(tm_buff+4,Dest_IP,4);
	tm_buff[8]=0x00;
	tm_buff[9]=0x11;
	tm_buff[10]=HIGH(count);
	tm_buff[11]=LOW(count);
	sum=csum(efp+IP_UDP_SPORT,count);
	sum+=csum(tm_buff,12);
	return((uint)(sum + (sum /0x10000)));
}

/**********************************************************
*����Ƿ�ΪARPЭ������
***********************************************************/
uchar8 is_arp(void)
{
	uchar8 ret=0;
	if(((efp[E_ptype]==0x08)&&(efp[E_ptype+1]==0x06))&&(DLen>=28))
	{										//E_ptype==0x0806 ?,(ARP�����ͷΪ28�ֽ�) ?	
		if((efp[HTYPE]==0x00)&&(efp[HTYPE+1]==0x01))
		{									//ARPӲ������0x0001
			if((efp[ARPPRO]==0x08)&&(efp[ARPPRO+1]==0x00))
			{								//ARPЭ������(ARPЭ��0x0800)
				ret=1;
			}
		}
	}
	return ret;
}

/**********************************************************
*����Ƿ�ΪIPЭ������
***********************************************************/
uint is_ip(void)
{
	uint ver,hlen;
	uint sum,i;
	uint ret=0;
	
	if((efp[E_ptype]==0x08)&&(efp[E_ptype+1]==0x00)&&(DLen>=20))
	{										//E_ptype==0x0800 ?  ,(IP�����ͷΪ20�ֽ�) ?
		ver=(efp[IP_HDR_VER]>>4)&0x0f;		//�汾IP^4
		hlen=(efp[IP_HDR_VER]&0x0f)<<2;		//IP���ײ�����(20-60�ֽ�)	
		sum=~csum(efp+IP_HDR_VER,hlen);
		if((ver==4) && (DLen>=hlen) && (sum==0))
		{
			if(DLen>(((efp[IP_HDR_LEN]&0x00ff)<<8)+(efp[IP_HDR_LEN+1]&0xff)))
				DLen=((efp[IP_HDR_LEN]&0x00ff)<<8)+(efp[IP_HDR_LEN+1]&0xff);
			DLen-=hlen;
			if(DLen!=0)
			{
				if(hlen>20)
				{
					for(i=0;i<DLen;i++)
						efp[i+22]=efp[i+22+(hlen-20)];
					DLen-=hlen-20;
				}
			}
			ret=DLen;
		}
	}
	return ret;
}

/**********************************************************
*����Ƿ�ΪicmpЭ������
***********************************************************/
uchar8 is_icmp(void)
{
	uchar8 ret=0;
	uint sum;
	if((efp[IP_HDR_Pcol]==0x01)&&(DLen>=8))
	{										//Э������=1?.ICMP�����ͷΪ8�ֽ�?
		sum=csum(efp+IP_ICMP_TYPE,DLen);
		if(sum==0xffff)
		{
			DLen=DLen-8;
			ret=1;
		}
	}
	return ret;
}

/**********************************************************
*����Ƿ�ΪudpЭ������
***********************************************************/
uchar8 is_udp(void)
{
	uchar8 ret=0;
	uint sum;
	if((efp[IP_HDR_Pcol]==0x11)&&(DLen>=8))
	{
		sum=check_udp(DLen);
		if((sum==0xffff)||((efp[IP_UDP_CHK]==0)&&(efp[IP_UDP_CHK+1]==0)))
		{
			DLen=DLen-8;
			ret=1;
		}
	}
	return ret;
}

/**********************************************************
*��ȡ���ͷ���MAC��ַ��IP��ַ
***********************************************************/
void getip_srce(void)
{
	memcpy(Dest_IP,efp+IP_HDR_SIP,4);		//�ѷ��͵�IP��ַ��������
	memcpy(Dest_MAC,efp+E_srce,6);			//�ѷ��ͷ���MAC��ַ��������
}

/**********************************************************
*��ȡ���շ���MAC��ַ��IP��ַ
***********************************************************/
void getip_locdest(void)
{
	memcpy(LOC_MAC,SR_MAC,6);
	memcpy(LOC_IP,S_IP,4);
	memcpy(LOC_MASK,S_MASK,4);
	memcpy(LOC_gate,S_gate,4);
	memcpy(LOC_PORT,S_PORT,2);
}

/**********************************************************
*��ȡ���ͷ���UDP����
***********************************************************/
void getudp_srce(void)
{
	getip_srce();
	memcpy(Dest_PORT,efp+IP_UDP_SPORT,2);
}

/**********************************************************
*��ȡ���շ���UDP����
***********************************************************/
void getudp_locdest(void)
{
	getip_locdest();
	memcpy(LOC_PORT,efp+IP_UDP_DPORT,2);
}

/**********************************************************
*���ɷ�������
***********************************************************/
void make_frame(uint pcol)
{
	efp[E_ptype]=HIGH(pcol);			//Э������:0x0806:ARPЭ��     0x0800:IPЭ��
	efp[E_ptype+1]=LOW(pcol);
	memcpy(efp+E_dest,Dest_MAC,6);		//Ŀ��MAC��ַ(DA)
	memcpy(efp+E_srce,SR_MAC,6);		//���ͷ���MAC��ַ(SA)
	DLen+=14;
}

/**********************************************************
*����IP���ݰ�
***********************************************************/
void make_ip(uchar8 IP_PCOL)
{
	uint sum;
	efp[IP_HDR_ident]=0;
	efp[IP_HDR_ident+1]=1;
	efp[IP_HDR_flags]=0;
	efp[IP_HDR_flags+1]=0;
	efp[IP_HDR_VER]=0x45;				//IP�汾IP^4�ͱ�ͷ����5*4=20 BYTE
	efp[IP_HDR_DS]=0;					//�������
	efp[IP_HDR_TIME]=0x80;				//���ݱ�����ʱ��
	efp[IP_HDR_Pcol]=IP_PCOL;			//UDP=17,TCP=6,ICMP=1
	memcpy(efp+IP_HDR_SIP,S_IP,4);		//���ع̶�IP��ַ
	memcpy(efp+IP_HDR_DIP,Dest_IP,4);	//Զ��IP
	DLen+=20;
	efp[IP_HDR_LEN]=HIGH(DLen);
	efp[IP_HDR_LEN+1]=LOW(DLen);
	efp[IP_HDR_CHK]=0;
	efp[IP_HDR_CHK+1]=0;
	sum=~csum(efp+IP_HDR_VER,20);
	efp[IP_HDR_CHK]=HIGH(sum);
	efp[IP_HDR_CHK+1]=LOW(sum);
    make_frame(0x0800);					//ipЭ������0x0800
}

/**********************************************************
*����icmp���ݰ�
***********************************************************/
void make_icmp(void)
{
	uint sum;
	efp[IP_ICMP_TYPE]=0;				//Ӧ��
	efp[IP_ICMP_CHK]=0;
	efp[IP_ICMP_CHK+1]=0;
	DLen+=8;
	sum=~csum(efp+IP_ICMP_TYPE,DLen);
	efp[IP_ICMP_CHK]=HIGH(sum);
	efp[IP_ICMP_CHK+1]=LOW(sum);
	make_ip(0x01);						//IP_PCOL=1
}

/**********************************************************
*����UDP���ݰ�
***********************************************************/
void make_udp(void)
{
	uint sum;
	memcpy(efp+IP_UDP_SPORT,S_PORT,2);	//���ع̶��˿ں�
	memcpy(efp+IP_UDP_DPORT,Dest_PORT,2);
	DLen+=8;
	efp[IP_UDP_LEN]=HIGH(DLen);
	efp[IP_UDP_LEN+1]=LOW(DLen);
	efp[IP_UDP_CHK]=0;
	efp[IP_UDP_CHK+1]=0;
	make_ip(17);						//IP_PCOL=17
	
	sum =~(check_udp(DLen-34));
	efp[IP_UDP_CHK]=HIGH(sum);
	efp[IP_UDP_CHK+1]=LOW(sum);
	if((efp[IP_UDP_CHK]==0)&&(efp[IP_UDP_CHK+1]==0))
	{
		efp[IP_UDP_CHK]=0xff;
		efp[IP_UDP_CHK+1]=0xff;
	}
}

/**********************************************************
*����ARP���ݰ�
***********************************************************/
void make_arp(uchar8 ctype)
{
	memcpy(efp+SMAC,SR_MAC,6);				//��ȡ���ص�MAC��ַ6byte
	memcpy(efp+DMAC,Dest_MAC,6);			//��ȡ����վ��MAC��ַ6byte
	efp[HTYPE]=0x00;
	efp[HTYPE+1]=0x01;						//��̫������Ϊ0x0001
	efp[ARPPRO]=0x08;
	efp[ARPPRO+1]=0x00;						//Э������(IPЭ��0x0800)
	efp[MACLEN]=0x06;
	efp[IPLEN]=0x04;
	efp[code_type]=0x00;
	efp[code_type+1]=ctype;					//��������1byte(����0001H��Ӧ��0002H)
	memcpy(efp+SIP1,S_IP,4);				//ԴIP��ַ
	memcpy(efp+DIP1,Dest_IP,4);				//Ŀ��IP��ַ
    DLen=28;
    make_frame(0x0806);						//ARPЭ������0x0806
}

/**********************************************************
*UDP���ݷ���
***********************************************************/
void udp_transmit()
{
	make_udp();
  put_ethernet(efp,DLen);
}

/**********************************************************
*udp���ݽ���
***********************************************************/
uchar8 udp_receive(void)
{
	uchar8 ret=0;
	getudp_srce();
	getudp_locdest();
	if(!memcmp(S_PORT,efp+IP_UDP_DPORT,2))
	{
		ret=1;
	}
	else if((efp[IP_UDP_DPORT]==0x00)&&(efp[IP_UDP_DPORT+1]==0x07))
	{
		udp_transmit();
	}
	return ret;
}

/************************************************************
*�������ƣ�	do_net_process
*����������	���紦�����
************************************************************/
uchar8 do_net_process(void)
{
	uint ret=0;
	if(is_arp())
	{
		if(!memcmp(S_IP,efp+DIP1,4))		//�뱾��IP��ַ�Ƚ�
		{
			if((efp[code_type]==0x00)&&(efp[code_type+1]==0x01))
			{								//��������=0x0001,ΪARP����
				memcpy(Dest_IP,efp+SIP1,4);	//���淢�ͷ�IP��ַ���Ա㷵��
				memcpy(Dest_MAC,efp+SMAC,6);//���淢�ͷ�MAC��ַ
        		make_arp(0x02);        		//ARPӦ��0x0002
       			put_ethernet(efp,DLen);
			}
			else if((efp[code_type]==0x00)&&(efp[code_type+1]==0x02))
			{								//��������==02H,ARPӦ��,Ĭ��Ϊ�����������ʱ����������Ӧ��
				memcpy(DE_MAC,efp+SMAC,4);	//��������ʱ��Ŀ�ĵ�ַ(�������Ĺ̶�MAC��ַ)
			}
		}
	}
	else if(is_ip())
	{	
		if(!memcmp(S_IP,efp+IP_HDR_DIP,4))
		{									//�뱾��IP��ַ��㲥��ַ�Ƚ�
			getip_srce();      				//��ȡ���ͷ���IP��MAC
			if(is_icmp())
			{	
				if(efp[IP_ICMP_TYPE]==0x08)
				{
					make_icmp();
        			put_ethernet(efp,DLen);
				}
			}
			else if(is_udp())
			{
				if(udp_receive())
				{
					ret=1;
				}
			}
		}
	}
    return (ret);
}




/***********************************************************************************************
* Function Name  : ReadNetAddrInfo
* Description    : ��ȡ�����ַ��Ϣ
* Input          : None
* Output         : None
* Return         : None
* Notes          : ��SysConfig.stNet���Ƶ��ֲ�������
***********************************************************************************************/
void ReadNetAddrInfo(void)
{ 
	memcpy(SR_MAC, (uchar8*)&SysConfig.stNet.MAC[0], 6);
	memcpy(S_IP, (uchar8*)&SysConfig.stNet.IP[0], 4);
	memcpy(S_MASK, (uchar8*)&SysConfig.stNet.NetMask[0], 4);
	memcpy(S_gate, (uchar8*)&SysConfig.stNet.Gateway[0], 4);

	S_PORT[1] = SysConfig.stNet.u16UDP_ListenPort & 0xFF;
	S_PORT[0] = ((SysConfig.stNet.u16UDP_ListenPort & 0xFF00) >> 8);
	
	memcpy(D_IP, (uchar8*)&SysConfig.stNet.ServerIP[0],4);
	D_port[1] = SysConfig.stNet.u16UDP_ServerPort & 0xFF;
	D_port[0] = ((SysConfig.stNet.u16UDP_ServerPort & 0xFF00) >> 8);
	memcpy(DE_MAC, (uchar8*)&SysConfig.stNet.ServerMAC[0], 6);

	memcpy(Dest_IP, D_IP, 4);		 
	memcpy(Dest_PORT, D_port, 2);		
	memcpy(Dest_MAC, DE_MAC, 6);		
}

/***********************************************************************************************
* Function Name  : AutoSave_ServerAddr
* Description    : �Զ������������ַ��Ϣ
* Input          : None
* Output         : None
* Return         : None
* Notes          : None
***********************************************************************************************/
void AutoSave_ServerAddr(void)
{
	if(memcmp(D_IP, Dest_IP, 4)||memcmp(DE_MAC, Dest_MAC, 6)||memcmp(D_port, Dest_PORT, 2))
	{
		memcpy(D_IP,Dest_IP,4);
		memcpy(D_port,Dest_PORT,2);

		memcpy((uchar8*)&SysConfig.stNet.ServerIP[0], D_IP, 4);
        SysConfig.stNet.u16UDP_ServerPort = D_port[0];
	    SysConfig.stNet.u16UDP_ServerPort += (D_port[1] >> 8);
		memcpy((uchar8*)&SysConfig.stNet.ServerMAC[0], Dest_MAC, 6);

		Store_Parameters(Addr_NetInfo, (uchar8*)&SysConfig.stNet, sizeof(ST_NETWORK));
	}
}

/***********************************************************************************************
* Function Name  : AutoSave_ServerMAC
* Description    : �Զ����������MAC��ַ 
* Input          : None
* Output         : None
* Return         : None
* Notes          : ������λ����MAC��ַ,�Ա�����������������������
***********************************************************************************************/
void AutoSave_ServerMAC(void)
{
	uint32 iAddr = 0;
	
	if(memcmp((uchar8*)&SysConfig.stNet.ServerMAC[0], Dest_MAC, 6))
	{
		memcpy((uchar8*)&SysConfig.stNet.ServerMAC[0], Dest_MAC, 6);
		//��������
		iAddr = Addr_SysConfig + ((uint32)&SysConfig.stNet.ServerMAC[0] - (uint32)&SysConfig);
		Store_Parameters(iAddr, (uchar8*)&SysConfig.stNet.ServerMAC[0], sizeof(SysConfig.stNet.ServerMAC));
	}
}

/***********************************************************************************************
* Function Name  : CreateSysMAC
* Description    : ����MAC��ַ
* Input          : None
* Output         : None
* Return         : None
* Notes          : None
***********************************************************************************************/
void CreateSysMAC(uchar8 Key, uchar8* pMacBuf)
{
	uchar8 McuID[15] = {0};

	McuID[0] = 0xA0;
	McuID[1] = Key;

	McuID[2] = SysConfig.stNet.IP[0];
	McuID[3] = SysConfig.stNet.IP[1];
	McuID[4] = SysConfig.stNet.IP[2];
	McuID[5] = SysConfig.stNet.IP[3];

	memcpy(pMacBuf, McuID, 6);
}

/***********************************************************************************************
* Function Name  : Network_Init
* Description    : ����ӿڳ�ʼ��
* Input          : None
* Output         : None
* Return         : None
* Notes          : None
***********************************************************************************************/
void Network_Init(void)
{	
	uchar8 MyMacAddr[10] = {0};
	 
	CreateSysMAC(0xAC, MyMacAddr);
	memcpy((uchar8*)&SysConfig.stNet.MAC[0], MyMacAddr, 6);
	
	ReadNetAddrInfo();

	encInit();
}

/***********************************************************************************************
* Function Name  : UdpClient_SendData
* Description    : �豸��UDP����Ϊ�ͻ��˷������ݰ�
* Input          : Data -- ����������, len -- ���ݳ���
* Output         : None
* Return         : None
* Notes          : 
***********************************************************************************************/
void UdpClient_SendData(uchar8* pStrBuf, uint16 len)
{
	ReadNetAddrInfo();

	memcpy(&efp[CMD_DATA_ADDR], pStrBuf, len);
	DLen = len;
	udp_transmit();
}

/***********************************************************************************************
* Function Name  : UdpServer_SendData
* Description    : �豸��UDP����Ϊ�ͻ��˷������ݰ�
* Input          : Data -- ����������, len -- ���ݳ���
* Output         : None
* Return         : None
* Notes          : 
***********************************************************************************************/
void UdpServer_SendData(uchar8* pStrBuf, uint16 len)
{
	memcpy(&efp[CMD_DATA_ADDR], pStrBuf, len);
	DLen = len;
	udp_transmit();
}

/***********************************************************************************************
* Function Name  : Thread_UDP
* Description    : UDP�����߳�
* Input          : None
* Output         : None
* Return         : None
* Notes          : None
***********************************************************************************************/
void Thread_UDP(void)
{	
	if(VALIDITY_USING != SysConfig.stHardware.bNetwork)
	{
		return;
	} 

	DLen = get_ethernet(efp);
	if(DLen > 0)
	{
		if(do_net_process())
		{
		
			if(Comm_PacketDeal(&efp[CMD_DATA_ADDR], DLen, COMM_TYPE_UDP_SERVER) == CMD_VALIDITY_PACKET)
			{
				AutoSave_ServerMAC();
			}
		}
	}
}



/*===================================File end===================================================*/


