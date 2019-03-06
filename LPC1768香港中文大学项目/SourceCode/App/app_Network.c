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
*全局变量说明
**************************************************************/
uchar8 efp[1024];					//接收缓冲区
uint  DLen;                         //用户数据的长度


//以下存放本地和远程服务器固定MAC,IP,Mask,gate,Port,上电从flash种读取
uchar8 SR_MAC[6];				//发送站(本机)物理地址6 BYTE
uchar8 S_IP[4];					//发送站(本机)IP地址  4 BYTE
uchar8 S_MASK[4];				//发送站(本机)子网掩码4 BYTE
uchar8 S_gate[4];				//发送站(本机)网关    4 BYTE
uchar8 S_PORT[2];				//发送站(本机)端口    2 BYTE

uchar8 DE_MAC[6];				//接收站(PC上位机)物理地址6 BYTE
uchar8 D_IP[4];					//接收站(PC上位机)IP地址  4 BYTE
uchar8 D_mask[4];				//接收站(PC上位机)子网掩码4 BYTE
uchar8 D_gate[4];				//接收站(PC上位机)网关    4 BYTE
uchar8 D_port[2];				//接收站(PC上位机)端口    2 BYTE

//以下存放本地和远程临时MAC,IP,Mask,gate,Port(FE00H~)
//接收目的地址数据时，保存的地址信号，供数据返回给目的地使用
uchar8 LOC_MAC[6];				//(本机)物理地址
uchar8 LOC_IP[4];				//(本机)IP地址
uchar8 LOC_MASK[4];				//(本机)子网掩码
uchar8 LOC_gate[4];				//(本机)网关
uchar8 LOC_PORT[2];				//(本机)端口

uchar8 Dest_MAC[6];				//(PC上位机)物理地址
uchar8 Dest_IP[4];				//(PC上位机)IP地址
uchar8 Dest_MASK[4];			//(PC上位机)子网掩码
uchar8 Dest_gate[4];			//(PC上位机)网关 
uchar8 Dest_PORT[2];			//(PC上位机)端口 

/**********************************************************
*效验和：把首部划分为16位的段，并把各段相加，在把和取反，即为效验和
*如果字节数为奇数，最后一个字节补0
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
*UDP的校验和不同上面的计算,这里的校验和包括3部分：伪首部，UDP首部，应用层来的数据
*其中伪首部为IP分组的首部一部分，其中有些字段要写0
*伪首部格式如下:
*     32位源IP地址
*     32位目的IP地址
*全0 + 8位协议(17) + 16位UDP总长度
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
*检测是否为ARP协议数据
***********************************************************/
uchar8 is_arp(void)
{
	uchar8 ret=0;
	if(((efp[E_ptype]==0x08)&&(efp[E_ptype+1]==0x06))&&(DLen>=28))
	{										//E_ptype==0x0806 ?,(ARP分组包头为28字节) ?	
		if((efp[HTYPE]==0x00)&&(efp[HTYPE+1]==0x01))
		{									//ARP硬件类型0x0001
			if((efp[ARPPRO]==0x08)&&(efp[ARPPRO+1]==0x00))
			{								//ARP协议类型(ARP协议0x0800)
				ret=1;
			}
		}
	}
	return ret;
}

/**********************************************************
*检测是否为IP协议数据
***********************************************************/
uint is_ip(void)
{
	uint ver,hlen;
	uint sum,i;
	uint ret=0;
	
	if((efp[E_ptype]==0x08)&&(efp[E_ptype+1]==0x00)&&(DLen>=20))
	{										//E_ptype==0x0800 ?  ,(IP分组包头为20字节) ?
		ver=(efp[IP_HDR_VER]>>4)&0x0f;		//版本IP^4
		hlen=(efp[IP_HDR_VER]&0x0f)<<2;		//IP包首部长度(20-60字节)	
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
*检测是否为icmp协议数据
***********************************************************/
uchar8 is_icmp(void)
{
	uchar8 ret=0;
	uint sum;
	if((efp[IP_HDR_Pcol]==0x01)&&(DLen>=8))
	{										//协议类型=1?.ICMP分组包头为8字节?
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
*检测是否为udp协议数据
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
*获取发送方的MAC地址和IP地址
***********************************************************/
void getip_srce(void)
{
	memcpy(Dest_IP,efp+IP_HDR_SIP,4);		//把发送的IP地址保存起来
	memcpy(Dest_MAC,efp+E_srce,6);			//把发送方的MAC地址保存起来
}

/**********************************************************
*读取接收方的MAC地址和IP地址
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
*获取发送方的UDP数据
***********************************************************/
void getudp_srce(void)
{
	getip_srce();
	memcpy(Dest_PORT,efp+IP_UDP_SPORT,2);
}

/**********************************************************
*读取接收方的UDP数据
***********************************************************/
void getudp_locdest(void)
{
	getip_locdest();
	memcpy(LOC_PORT,efp+IP_UDP_DPORT,2);
}

/**********************************************************
*生成发送数据
***********************************************************/
void make_frame(uint pcol)
{
	efp[E_ptype]=HIGH(pcol);			//协议类型:0x0806:ARP协议     0x0800:IP协议
	efp[E_ptype+1]=LOW(pcol);
	memcpy(efp+E_dest,Dest_MAC,6);		//目的MAC地址(DA)
	memcpy(efp+E_srce,SR_MAC,6);		//发送方的MAC地址(SA)
	DLen+=14;
}

/**********************************************************
*生成IP数据包
***********************************************************/
void make_ip(uchar8 IP_PCOL)
{
	uint sum;
	efp[IP_HDR_ident]=0;
	efp[IP_HDR_ident+1]=1;
	efp[IP_HDR_flags]=0;
	efp[IP_HDR_flags+1]=0;
	efp[IP_HDR_VER]=0x45;				//IP版本IP^4和报头长度5*4=20 BYTE
	efp[IP_HDR_DS]=0;					//区域服务
	efp[IP_HDR_TIME]=0x80;				//数据报生存时间
	efp[IP_HDR_Pcol]=IP_PCOL;			//UDP=17,TCP=6,ICMP=1
	memcpy(efp+IP_HDR_SIP,S_IP,4);		//本地固定IP地址
	memcpy(efp+IP_HDR_DIP,Dest_IP,4);	//远程IP
	DLen+=20;
	efp[IP_HDR_LEN]=HIGH(DLen);
	efp[IP_HDR_LEN+1]=LOW(DLen);
	efp[IP_HDR_CHK]=0;
	efp[IP_HDR_CHK+1]=0;
	sum=~csum(efp+IP_HDR_VER,20);
	efp[IP_HDR_CHK]=HIGH(sum);
	efp[IP_HDR_CHK+1]=LOW(sum);
    make_frame(0x0800);					//ip协议类型0x0800
}

/**********************************************************
*生成icmp数据包
***********************************************************/
void make_icmp(void)
{
	uint sum;
	efp[IP_ICMP_TYPE]=0;				//应答
	efp[IP_ICMP_CHK]=0;
	efp[IP_ICMP_CHK+1]=0;
	DLen+=8;
	sum=~csum(efp+IP_ICMP_TYPE,DLen);
	efp[IP_ICMP_CHK]=HIGH(sum);
	efp[IP_ICMP_CHK+1]=LOW(sum);
	make_ip(0x01);						//IP_PCOL=1
}

/**********************************************************
*生成UDP数据包
***********************************************************/
void make_udp(void)
{
	uint sum;
	memcpy(efp+IP_UDP_SPORT,S_PORT,2);	//本地固定端口号
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
*生成ARP数据包
***********************************************************/
void make_arp(uchar8 ctype)
{
	memcpy(efp+SMAC,SR_MAC,6);				//读取本地的MAC地址6byte
	memcpy(efp+DMAC,Dest_MAC,6);			//读取接收站的MAC地址6byte
	efp[HTYPE]=0x00;
	efp[HTYPE+1]=0x01;						//以太网类型为0x0001
	efp[ARPPRO]=0x08;
	efp[ARPPRO+1]=0x00;						//协议类型(IP协议0x0800)
	efp[MACLEN]=0x06;
	efp[IPLEN]=0x04;
	efp[code_type]=0x00;
	efp[code_type+1]=ctype;					//操作类型1byte(请求0001H，应答0002H)
	memcpy(efp+SIP1,S_IP,4);				//源IP地址
	memcpy(efp+DIP1,Dest_IP,4);				//目的IP地址
    DLen=28;
    make_frame(0x0806);						//ARP协议类型0x0806
}

/**********************************************************
*UDP数据发送
***********************************************************/
void udp_transmit()
{
	make_udp();
  put_ethernet(efp,DLen);
}

/**********************************************************
*udp数据接收
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
*函数名称：	do_net_process
*功能描述：	网络处理程序
************************************************************/
uchar8 do_net_process(void)
{
	uint ret=0;
	if(is_arp())
	{
		if(!memcmp(S_IP,efp+DIP1,4))		//与本地IP地址比较
		{
			if((efp[code_type]==0x00)&&(efp[code_type+1]==0x01))
			{								//操作类型=0x0001,为ARP请求
				memcpy(Dest_IP,efp+SIP1,4);	//保存发送方IP地址，以便返回
				memcpy(Dest_MAC,efp+SMAC,6);//保存发送方MAC地址
        		make_arp(0x02);        		//ARP应答：0x0002
       			put_ethernet(efp,DLen);
			}
			else if((efp[code_type]==0x00)&&(efp[code_type+1]==0x02))
			{								//操作类型==02H,ARP应答,默认为向服务器请求时，服务器的应答
				memcpy(DE_MAC,efp+SMAC,4);	//返回数据时的目的地址(服务器的固定MAC地址)
			}
		}
	}
	else if(is_ip())
	{	
		if(!memcmp(S_IP,efp+IP_HDR_DIP,4))
		{									//与本地IP地址或广播地址比较
			getip_srce();      				//获取发送方的IP，MAC
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
* Description    : 读取网络地址信息
* Input          : None
* Output         : None
* Return         : None
* Notes          : 将SysConfig.stNet复制到局部变量中
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
* Description    : 自动保存服务器地址信息
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
* Description    : 自动保存服务器MAC地址 
* Input          : None
* Output         : None
* Return         : None
* Notes          : 保存上位机的MAC地址,以便于主动发送心跳包等数据
***********************************************************************************************/
void AutoSave_ServerMAC(void)
{
	uint32 iAddr = 0;
	
	if(memcmp((uchar8*)&SysConfig.stNet.ServerMAC[0], Dest_MAC, 6))
	{
		memcpy((uchar8*)&SysConfig.stNet.ServerMAC[0], Dest_MAC, 6);
		//保存数据
		iAddr = Addr_SysConfig + ((uint32)&SysConfig.stNet.ServerMAC[0] - (uint32)&SysConfig);
		Store_Parameters(iAddr, (uchar8*)&SysConfig.stNet.ServerMAC[0], sizeof(SysConfig.stNet.ServerMAC));
	}
}

/***********************************************************************************************
* Function Name  : CreateSysMAC
* Description    : 生成MAC地址
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
* Description    : 网络接口初始化
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
* Description    : 设备在UDP中作为客户端发送数据包
* Input          : Data -- 待发送数据, len -- 数据长度
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
* Description    : 设备在UDP中作为客户端发送数据包
* Input          : Data -- 待发送数据, len -- 数据长度
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
* Description    : UDP处理线程
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


