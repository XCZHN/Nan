/*===============================================================================================
==                                                                                              
**  Copyright(c) 2015  shenzhen zhenyao technology co., LTD. All rights reserved.
==                                                                                           
**	Author:		zenghl                                                                          
==                                                                                          
**  Version:	v1.0.0                                                                          
==
**  Filename:	Fingerprint.c                                                                      
==                                                                                         
**  Function:	ָ��ʶ��Ӧ�ò��ļ�  
==
**  Data:       2015-11-26
================================================================================================*/

#include "Includes.h"


uchar8 FingerprintData[520] = {0};  //�洢ָ������(ָ�ƺ�2B + 512Bָ������)
uchar8 FingerprintReplyData[20] = {0};


/***********************************************************************************************
* Function Name  : Fingerprint_Init
* Description	 : ָ��ģ���ʼ��
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
void Fingerprint_Init(void)
{
	LPC_PINCON->PINSEL4 &= 0xFFFFFFCF;
	LPC_GPIO2->FIODIR &= ~(1<<Fingerprint_pin);
}

/***********************************************************************************************
* Function Name  : SendDataToFingerprint
* Description	 : ����ָ�ָ��ģ��
* Input 		 : None
* Output		 : None
* Return		 : None
* Note  		 : ����ǰ��������մ洢��  
***********************************************************************************************/
void SendDataToFingerprint(uchar8* pBuf, uint16 len)
{
	//������½��մ洢��
	memset(FingerprintDataBuf, 0x0, sizeof(FingerprintDataBuf));
	FingerprintDataCnt =0;

	UART1_SendNxByte(pBuf, len);
}

/***********************************************************************************************
* Function Name  : SendCmdToFingerprint
* Description	 : ����ָ�ָ��ģ��
* Input 		 : PackageID :����ʶ(0x01--�����, 0x02--���ݰ�, 0x07--Ӧ���, 0x08--������) 
                   cmd : ָ����
                   pPackageBuf : �������ݰ�
                   PackageLen : ���ݰ�����
* Output		 : None
* Return		 : None
* Note  		 : �������ݺ��������մ洢��,�Ա��ں�����������
***********************************************************************************************/
void SendCmdToFingerprint(uchar8 PackageID, uchar8 cmd, uchar8* pPackageBuf, uint16 PackageLen)
{
	uchar8 SendBuf[256] = {0};
	uint16 SendLen = 0;
	uint16 Crc = 0;
	uint16 i = 0;

	//����½��մ洢��
	memset(FingerprintDataBuf, 0xFF, sizeof(FingerprintDataBuf));
	FingerprintDataCnt =0;

	//��ͷ
	SendBuf[SendLen++] = 0xEF;
	SendBuf[SendLen++] = 0x01;

	//ģ���ַ
	SendBuf[SendLen++] = 0xFF;
	SendBuf[SendLen++] = 0xFF;
	SendBuf[SendLen++] = 0xFF;
	SendBuf[SendLen++] = 0xFF;

    //����ʶ
	SendBuf[SendLen++] = PackageID;


	//ָ����
	if(ZW_PackageID_CMD == PackageID)
	{
		//������(ָ��+����)
		SendBuf[SendLen++] = ((PackageLen +3)&0xFF00) >> 8;
		SendBuf[SendLen++] = (PackageLen +3) &0xFF;

		SendBuf[SendLen++] = cmd;
	}
	else
	{
		//������(����)
		SendBuf[SendLen++] = ((PackageLen +2)&0xFF00) >> 8;
		SendBuf[SendLen++] = (PackageLen +2) &0xFF;
	}

	//����
	if(PackageLen)
	{
		memcpy(&SendBuf[SendLen], pPackageBuf, PackageLen);
		SendLen += PackageLen;
	}

	//У����
	for(i=6; i<SendLen; i++)
	{
		Crc += SendBuf[i];
	}
	SendBuf[SendLen++] = (Crc & 0xFF00) >> 8;
	SendBuf[SendLen++] = Crc & 0xFF;
	
	//����ָ���
	UART1_SendNxByte(SendBuf, SendLen);
}

/***********************************************************************************************
* Function Name  : CheckFingerprintReply
* Description	 : ָ��ģ��Ļظ�����
* Input 		 : CmdDataLen : �ظ�ָ���е����ݳ���                 
* Output		 : pCmdDataBuf : �ظ�ָ���е�����
* Return		 : ָ��ִ��ȷ����
* Note  		 : 
***********************************************************************************************/
uchar8 CheckFingerprintReply(uint16 CmdDataLen, uchar8* pCmdDataBuf)
{
	uchar8 Ret = 0xFF;
	uint16 Crc = 0;
	uint16 i = 0;
		
	if(FingerprintDataCnt >= (12 + CmdDataLen))
 	{
 		if((FingerprintDataBuf[0]==0xEF)&&(FingerprintDataBuf[1]==0x01)&&(FingerprintDataBuf[2]==0xFF)
			&&(FingerprintDataBuf[3]==0xFF)&&(FingerprintDataBuf[4]==0xFF)&&(FingerprintDataBuf[5]==0xFF))
 		{
 			CmdDataLen = FingerprintDataBuf[7] * 256;
			CmdDataLen += FingerprintDataBuf[8];
			CmdDataLen -= 3;  //��ȥȷ����1B��У���2B
				
 			Crc = FingerprintDataBuf[6];  //����ʶ
			Crc += FingerprintDataBuf[7];
			Crc += FingerprintDataBuf[8];
			Crc += FingerprintDataBuf[9]; //ȷ����

			for(i=0; i<CmdDataLen; i++)
			{
				Crc += FingerprintDataBuf[10 + i];
				//��ȡ���ݲ���
				pCmdDataBuf[i] = FingerprintDataBuf[10 + i];
			}
			
			if(Crc == (FingerprintDataBuf[10+i]*256+FingerprintDataBuf[11+i]))
			{
	 			Ret = FingerprintDataBuf[9]; //����״̬��
			}
			else
			{
				Ret = 0xCC;
			} 		
		}
	}

	return Ret;
}

/***********************************************************************************************
* Function Name  : AwaitingerprintResult
* Description	 : �ȴ�ָ��ģ��Ļظ�ָ��ִ�н��
* Input 		 : AwaitTime_ms -- �ȴ�ʱ��(ms)
* Output		 : None
* Return		 : ָ��ִ��ȷ����(0x01--�հ��д�...)
* Note  		 : 
***********************************************************************************************/
uchar8 AwaitingerprintResult(uint32 WaitTimeout_ms)
{
	uchar8 Ret = 0xFF;
	uint16 Crc = 0;
	uint16 DataLen = 0;
	uint16 i = 0;

	while(WaitTimeout_ms --)
	{
		if(FingerprintDataCnt >= 12)
	 	{
	 		if((FingerprintDataBuf[0]==0xEF)&&(FingerprintDataBuf[1]==0x01)&&(FingerprintDataBuf[2]==0xFF)
				&&(FingerprintDataBuf[3]==0xFF)&&(FingerprintDataBuf[4]==0xFF)&&(FingerprintDataBuf[5]==0xFF))
	 		{
	 			DataLen = FingerprintDataBuf[7] * 256;
				DataLen += FingerprintDataBuf[8];
				DataLen -= 3;  //��ȥȷ����1B��У���2B
					
	 			Crc = FingerprintDataBuf[6];  //����ʶ
				Crc += FingerprintDataBuf[7];
				Crc += FingerprintDataBuf[8];
				Crc += FingerprintDataBuf[9]; //ȷ����

				for(i=0; i<DataLen; i++)
				{
					Crc += FingerprintDataBuf[10 + i];
				}
				
				if(Crc == (FingerprintDataBuf[10+i]*256+FingerprintDataBuf[11+i]))
				{
		 			Ret = FingerprintDataBuf[9];
				}
				else
				{
					Ret = 0xCC;
				}
	 		}
			return Ret;
		}
		else
		{
			delay_ms(1);
		}
	}

	return Ret;
}

/***********************************************************************************************
* Function Name  : SendCmdToFingerprint_GetImg
* Description	 : ��ȡͼ��
* Input 		 : None
* Output		 : None
* Return		 : None
* Note  		 : ����:EF 01	FF FF FF FF 01 00 03 01 00 05
*				   ����:EF 01	FF FF FF FF 07 00 03 xx(ȷ����1�ֽ�) sum(У���2�ֽ�)
*				   ȷ����=0x00 ��ʾ¼��ɹ�
*				   ȷ����=0x01 ��ʾ�հ��д�
*				   ȷ����=0x02 ��ʾ������������ָ
*				   ȷ����=0x03 ��ʾ¼�벻�ɹ�
***********************************************************************************************/
void SendCmdToFingerprint_GetImg(void)
{
	uchar8 SendBuf[12] = {0};
	SendCmdToFingerprint(ZW_PackageID_CMD, ZW_CMD_GetImg, SendBuf, 0);
}

/***********************************************************************************************
* Function Name  : SendCmdToFingerprint_GenChar
* Description	 : ͼ����������
* Input 		 : BufferID---����ID
* Output		 : None
* Return		 : None
* Note  		 : ����:EF 01	FF FF FF FF 01 00 04 02 BufferID sum
*				   ����:EF 01	FF FF FF FF 07 00 03 xx(ȷ����1�ֽ�) sum(У���2�ֽ�)
*				   ȷ����=0x00 ��ʾ���������ɹ�
*				   ȷ����=0x01 ��ʾ�հ��д�
*				   ȷ����=0x06 ��ʾָ��ͼ��̫�Ҷ�����������
*				   ȷ����=0x07 ��ʾָ��ͼ����������������̫�ٶ�����������
*				   ȷ����=0x15 ��ʾͼ�񻺳�����û����Чԭʼͼ��������ͼ��
***********************************************************************************************/
void SendCmdToFingerprint_GenChar(uchar8 BufferID)
{
	uchar8 SendBuf[12] = {0};
	SendBuf[0] = BufferID;
	SendCmdToFingerprint(ZW_PackageID_CMD, ZW_CMD_GenChar, SendBuf, 1);
}

/***********************************************************************************************
* Function Name  : SendCmdToFingerprint_RegModel
* Description	 : �����ϳ�ģ��
* Input 		 : None
* Output		 : None
* Return		 : None
* Note  		 : ����:EF 01	FF FF FF FF 01 00 03 05 00 09
*				   ����:EF 01	FF FF FF FF 07 00 03 xx(ȷ����1�ֽ�) sum(У���2�ֽ�)
*				   ȷ����=0x00 ��ʾ�ϲ��ɹ�
*				   ȷ����=0x01 ��ʾ�հ��д�
*				   ȷ����=0x0a ��ʾ�ϲ�ʧ��(��öָ�Ʋ�����ͬһ��ָ)
***********************************************************************************************/
void SendCmdToFingerprint_RegModel(void)
{
	uchar8 SendBuf[12] = {0};
	SendCmdToFingerprint(ZW_PackageID_CMD, ZW_CMD_RegModel, SendBuf, 0);
}

/***********************************************************************************************
* Function Name  : SendCmdToFingerprint_Store
* Description	 : �洢ģ��
* Input 		 : BufferID---����ID   PageID--�洢λ��ID
* Output		 : None
* Return		 : None
* Note  		 :  ����:EF 01	FF FF FF FF 01(����ʶ01) 00 06(������0006) 06(ָ�����06) BufferID(��������1�ֽ�) PageID(λ�ú�2�ֽ�) 
     				 sum(�����2�ֽ�)
*					����:EF 01	FF FF FF FF 07 00 03 xx(ȷ����1�ֽ�) sum(У���2�ֽ�)
*					ȷ����=0x00 ��ʾ�洢�ɹ�
*					ȷ����=0x01 ��ʾ�հ��д�
*					ȷ����=0x0b ��ʾPageID����ָ�ƿⷶΧ
*					ȷ����=0x11 ��ʾдFLASH����
***********************************************************************************************/
void SendCmdToFingerprint_Store(uchar8 BufferID, uint16 PageID)
{
	uchar8 SendBuf[12] = {0};
	SendBuf[0] = BufferID;
	SendBuf[1] = ((PageID&0xFF00)>>8); 
	SendBuf[2] = (PageID&0x00FF);
	SendCmdToFingerprint(ZW_PackageID_CMD, ZW_CMD_Store, SendBuf, 3);
}

/***********************************************************************************************
* Function Name  : SendCmdToFingerprint_DeletChar
* Description	 : ɾ��ģ��
* Input 		 : PageID--�洢λ��ID  num--ɾ���ֽ���
* Output		 : None
* Return		 : None
* Note  		 : ����:EF 01	FF FF FF FF 01 00 07(������) 0C(����) PageID(ҳ��2�ֽ�) N(ɾ������2�ֽ�) sum(�����2�ֽ�)
*				   ����:EF 01	FF FF FF FF 07 00 03 xx(ȷ����1�ֽ�) sum(У���2�ֽ�)
*				   ȷ����=0x00 ��ʾɾ��ģ��ɹ�
*				   ȷ����=0x01 ��ʾ�հ��д�
*				   ȷ����=0x02 ��ʾɾ��ģ��ʧ��
***********************************************************************************************/
void SendCmdToFingerprint_DeletChar(uint16 PageID, uint16 num)
{
	uchar8 SendBuf[12] = {0};
	SendBuf[0]= ((PageID & 0xFF00)>>8);
	SendBuf[1]= (PageID & 0x00FF);	
	SendBuf[2]= ((num & 0xFF00)>>8);
	SendBuf[3]= (num & 0x00FF);	
	SendCmdToFingerprint(ZW_PackageID_CMD, ZW_CMD_DeletChar, SendBuf, 4);
}

/***********************************************************************************************
* Function Name  : SendCmdToFingerprint_Empty
* Description	 : ���ָ�ƿ�
* Input 		 : None
* Output		 : None
* Return		 : None
* Note  		 : ����:EF 01	FF FF FF FF 01 00 03 0d 00 11
*				   ����:EF 01	FF FF FF FF 07 00 03 xx(ȷ����1�ֽ�) sum(У���2�ֽ�)
*				   ȷ����=0x00 ��ʾ��ճɹ�
*				   ȷ����=0x01 ��ʾ�հ��д�
*				   ȷ����=0x02 ��ʾ���ʧ��
***********************************************************************************************/
void SendCmdToFingerprint_Empty(void)
{
	uchar8 SendBuf[12] = {0};
	SendCmdToFingerprint(ZW_PackageID_CMD, ZW_CMD_Empty, SendBuf, 0);
}

/***********************************************************************************************
* Function Name  : SendCmdToFingerprint_Search
* Description	 : ����ָ��
* Input 		 : BufferID--����ID  StartPage--��ʼҳ   PageNum--ҳ��
* Output		 : None
* Return		 : None
* Note  		 : ����:EF 01	FF FF FF FF 01(����ʶ01) 00 08(������0008) 04(ָ�����04) BufferID(��������1�ֽ�) StartPage(��ʼҳ2�ֽ�) 
*    		       PageNum(ҳ��2�ֽ�) sum(�����2�ֽ�)
*				   ����:EF 01	FF FF FF FF 07 00 07 xx(ȷ����1�ֽ�) PageID(ҳ��2�ֽ�) MatchScore(�÷�2�ֽ�) sum(У���2�ֽ�)
*				   ȷ����=0x00 ��ʾ������
*				   ȷ����=0x01 ��ʾ�հ��д�
*				   ȷ����=0x09 ��ʾû����������ʱҳ����÷�Ϊ0
***********************************************************************************************/
void SendCmdToFingerprint_Search(uchar8 BufferID, uint16 StartPage, uint16 PageNum)
{
	uchar8 SendBuf[12] = {0};
	SendBuf[0]= BufferID;
	SendBuf[1]= ((StartPage & 0xFF00)>>8);
	SendBuf[2]= (StartPage & 0x00FF);	
	SendBuf[3]= ((PageNum & 0xFF00)>>8);
	SendBuf[4]= (PageNum & 0x00FF);	
	SendCmdToFingerprint(ZW_PackageID_CMD, ZW_CMD_Search, SendBuf, 5);
}

/***********************************************************************************************
* Function Name  : Fingerprint_Search
* Description	 : ָ������
* Input 		 : handle -- (=0xFF������ʱȡ��ָ��¼��)
* Output		 : None
* Return		 : ������ƥ���ָ�Ʒ���ָ�ƺ�(1--200), 
                   0xA000-0xA00F ��Ӧ��ǰִ�в���0-16
				   0xE0xx-0xEFxx �������, 0xFFFF--δ�ҵ�ƥ��ָ�ƺ�
* Note  		 : �ڲ������˲�����ʾ
***********************************************************************************************/
uint16 Fingerprint_Search(uint16 handle)
{
	static uint32 LastScanTime;
	static uchar8 HandleStep;
	uchar8 cRet = 0xFF;
	uint16 iHandleRet = 0xA000;

	if(ZW_StopHandle == handle)  //�������ʱȡ��ָ��¼��
	{
		HandleStep = 0;
		return 0xFFFF;
	}

	switch(HandleStep)
	{	
		case 0: //����Ƿ�����ָ����
		{
			if(0x00 == GET_Fingerprint_STATE)
			{
			//	delay_ms(100);
			}
			
			if(0x00 == GET_Fingerprint_STATE)
			{
				HandleStep ++;	
			}
			else
			{
				HandleStep = 0;
				return 0xE000;
			}
		}
		break;

		case 1: //��һ�βɼ�
		{ 
			SendCmdToFingerprint_GetImg();
			LastScanTime = gstSysRunInfo.u32TimeCount_ms;
			HandleStep++;
		}
		break;

		case 2:
		{
			if(gstSysRunInfo.u32TimeCount_ms-LastScanTime < 1100)
			{
			 	cRet = CheckFingerprintReply(0, FingerprintReplyData);
	 			if(0x00 == cRet)
	 			{
	 				HandleStep++;
	 			}
				else if(0xFF != cRet)
				{
					//���յ����ݰ�������
					HandleStep = 0;
					//GUI_UserAccess(9); //9--ָ��¼�����
					return 0xE100;
				}
			}
			else //��ʱ-- ���յ����ݰ�������or��ʱδ��Ӧ
			{
				HandleStep = 0;
				//GUI_UserAccess(9); //9--ָ��¼�����
				return 0xE200;
			}
		}
		break;

		case 3: //ͼ������������1
		{
			SendCmdToFingerprint_GenChar(1);
			HandleStep++;
			LastScanTime = gstSysRunInfo.u32TimeCount_ms;
		}
		break;

		case 4:
		{
			if(gstSysRunInfo.u32TimeCount_ms-LastScanTime < 1000)
			{
			 	cRet = CheckFingerprintReply(0, FingerprintReplyData);
	 			if(0x00 == cRet)
	 			{
	 				HandleStep++;
	 			}
				else if(0xFF != cRet)
				{
					//����������ʧ��
					HandleStep = 0;
					//GUI_UserAccess(9); //9--ָ��¼�����
					return 0xE300;
				}
			}
			else //��ʱ--���յ����ݰ�������or��ʱδ��Ӧ
			{
				HandleStep = 0;
				//GUI_UserAccess(9); //9--ָ��¼�����
				return 0xE400;
			}
		}
		break;

		case 5://���ָ���Ƿ�����Ȩ
		{
			SendCmdToFingerprint_Search(1, 0, MAX_ZW_UNM);//��⵱ǰMAX_ZW_UNMöָ��
			HandleStep++;
			LastScanTime = gstSysRunInfo.u32TimeCount_ms;
		}
		break;

		case 6:
		{
			if(gstSysRunInfo.u32TimeCount_ms-LastScanTime < 2300)
			{
			 	cRet = CheckFingerprintReply(4, FingerprintReplyData);
	 			if(0x09 == cRet) //δ������
	 			{
	 				HandleStep = 0;
					return 0xFFFF;
	 			}
				else if(0x00 == cRet)//������ָ��
				{
					HandleStep = 0;
					return (FingerprintReplyData[0]*256+FingerprintReplyData[1]);
				}
				else if(0xFF != cRet)
				{
					//���յ����ݰ�������
					HandleStep = 0;
					//GUI_UserAccess(13);//ָ��δ��Ȩ
					return 0xE600;
				}
			}
			else //��ʱ--���յ����ݰ�������or��ʱδ��Ӧ	
			{								
				HandleStep = 0;
				//GUI_UserAccess(13);//ָ��δ��Ȩ
				return 0xE700;
			}
		}
		break;

		default:
		{
			HandleStep = 0;
		}
		break;
	}	
	
	iHandleRet += HandleStep; 
	return iHandleRet;
}

/***********************************************************************************************
* Function Name  : Fingerprint_Login
* Description	 : ָ��ע��
* Input 		 : FingerNum -- ���ɴ洢ָ�ƺ�(=0xFFFF������ʱȡ��ָ��¼��)
* Output		 : None
* Return		 : 0xA000-0xA00F ��Ӧ��ǰִ�в���0-16
                   >=0xE000--�����쳣,��ִֹ��, 
                   0xABCD--¼��ָ�Ƴɹ�
* Note  		 : �ڲ������˲�����ʾ
***********************************************************************************************/
uint16 Fingerprint_Login(uint16 FingerNum)
{
	static uint32 LastScanTime, Timeout;
	static uchar8 HandleStep;
	uchar8 cRet = 0xFF;

	if(FingerNum >= MAX_ZW_UNM)  
	{
		HandleStep = 0;
		return 0xA000;
	}
	
	switch(HandleStep)
	{
		case 0: //����Ƿ�����ָ����
		{
			if(0x00 == GET_Fingerprint_STATE)
			{
				delay_ms(321);
			}
			
			if(0x00 == GET_Fingerprint_STATE)
			{
				LastScanTime = gstSysRunInfo.u32TimeCount_ms;
				HandleStep++;				
			}
			else
			{
				if(gstSysRunInfo.u32TimeCount_ms-LastScanTime > 3500)
				{
					LastScanTime = gstSysRunInfo.u32TimeCount_ms;
					Voice_Play(YY_QingAnZhiWen);
				}
				HandleStep =0;
			}
		}
		break;

		case 1: //��һ�βɼ�
		{ 
			LastScanTime = gstSysRunInfo.u32TimeCount_ms;
			GUI_UserAccess(2);
			SendCmdToFingerprint_GetImg();
			HandleStep++;
		}
		break;

		case 2:
		{
			if(gstSysRunInfo.u32TimeCount_ms-LastScanTime < 1100)
			{
			 	cRet = CheckFingerprintReply(0, FingerprintReplyData);				
	 			if(0x00 == cRet)
	 			{
	 				HandleStep++;
	 			}
				else if(0xFF != cRet)
				{
					HandleStep = 0;
					GUI_UserAccess(9); //9--ָ��¼�����
					return 0xE200;
				}
			}
			else //��ʱ--���յ����ݰ�������or��ʱδ��Ӧ	
			{
				HandleStep = 0;
				GUI_UserAccess(9); //9--ָ��¼�����
				return 0xE300;
			}
		}
		break;

		case 3: //ͼ������������1
		{
			SendCmdToFingerprint_GenChar(1);
			HandleStep++;
			LastScanTime = gstSysRunInfo.u32TimeCount_ms;
		}
		break;

		case 4:
		{
			if(gstSysRunInfo.u32TimeCount_ms - LastScanTime < 1000)
			{
			 	cRet = CheckFingerprintReply(0, FingerprintReplyData);				
	 			if(0x00 == cRet)
	 			{
	 				Timeout = gstSysRunInfo.u32TimeCount_ms;
	 				HandleStep++;
					GUI_UserAccess(3); //��ʾ"���ƿ���ָ"
					if(0x00 == GET_Fingerprint_STATE)
					{
						Voice_Play(YY_QingYiKaiShouZhi);
					}
	 			}
				else if(0xFF != cRet)
				{
					//����������ʧ��
					HandleStep = 0;
					GUI_UserAccess(9); //9--ָ��¼�����
					return 0xE400;
				}
			}
			else //��ʱ--���յ����ݰ�������or��ʱδ��Ӧ
			{										
				HandleStep = 0;
				GUI_UserAccess(9); //9--ָ��¼�����
				return 0xE500;
			}
		}
		break;

		case 5: //�ȴ���ָ�ƿ�
		{
			if((GET_Fingerprint_STATE)||(gstSysRunInfo.u32TimeCount_ms - Timeout > 3210))
			{
				HandleStep++;
			}
			else
			{
				if(gstSysRunInfo.u32TimeCount_ms - LastScanTime > 2000)
				{
					LastScanTime = gstSysRunInfo.u32TimeCount_ms;
					GUI_UserAccess(3); //��ʾ"���ƿ���ָ"
					Voice_Play(YY_QingYiKaiShouZhi);
				}
			}
		}
		break;
		
		case 6: //��ʾ�ٰ�ָ��
		{
			if(GET_Fingerprint_STATE)
			{
				GUI_UserAccess(4); //��ʾ"���ٰ�ָ��"
				Voice_Play(YY_QingZaiAnZhiWen);
			}
			HandleStep++;
		}
		break;

		case 7://�ڶ��βɼ� 
		{
			if(0x00 == GET_Fingerprint_STATE)
			{
				delay_ms(220);
				if(0x00 == GET_Fingerprint_STATE)
				{
					GUI_UserAccess(5); //�ڶ��βɼ�ָ��
					SendCmdToFingerprint_GetImg();
					HandleStep++;
					LastScanTime = gstSysRunInfo.u32TimeCount_ms;				
				}
			}
		}
		break;

		case 8: //�����ڶ��βɼ���Ӧ���� 
		{
			if(gstSysRunInfo.u32TimeCount_ms - LastScanTime < 1100)
			{
			 	cRet = CheckFingerprintReply(0, FingerprintReplyData);				
	 			if(0x00 == cRet)
	 			{
	 				HandleStep++;
	 			}
				else if(0xFF != cRet)
				{
					//���յ����ݰ�������
					HandleStep = 0;
					GUI_UserAccess(9);
					return 0xE800;
				}
			}
			else //��ʱ--���յ����ݰ�������or��ʱδ��Ӧ
			{										
				HandleStep = 0;
				GUI_UserAccess(9); //9--ָ��¼�����
				return 0xE900;
			}
		}
		break;

		case 9: //ͼ������������2
		{
			SendCmdToFingerprint_GenChar(2);
			HandleStep++;
			LastScanTime = gstSysRunInfo.u32TimeCount_ms;
		}
		break;

		case 10:
		{
			if(gstSysRunInfo.u32TimeCount_ms-LastScanTime < 700)
			{
			 	cRet = CheckFingerprintReply(0, FingerprintReplyData);				
	 			if(0x00 == cRet)
	 			{
	 				HandleStep++;
	 			}
				else if(0xFF != cRet)
				{
					//����������2ʧ��
					HandleStep = 0;
					GUI_UserAccess(9);
					return 0xEA00;
				}
			}
			else //��ʱ--���յ����ݰ�������or��ʱδ��Ӧ
			{										
				HandleStep = 0;
				GUI_UserAccess(9); //9--ָ��¼�����
				return 0xEC00;
			}
		}
		break;

		case 11: //�����ϳ�ģ��
		{
			SendCmdToFingerprint_RegModel();
			HandleStep++;
			LastScanTime = gstSysRunInfo.u32TimeCount_ms;
		}
		break;

		case 12:
		{
			if(gstSysRunInfo.u32TimeCount_ms-LastScanTime < 500)
			{
			 	cRet = CheckFingerprintReply(0, FingerprintReplyData);				
	 			if(0x00 == cRet)
	 			{
	 				HandleStep++;
	 			}
				else if(0xFF != cRet)
				{
					//�ϳ�ģ��ʧ��
					HandleStep = 0;
					GUI_UserAccess(10); //10--ָ�ƺϲ�ʧ��
					return 0xED00;
				}
			}
			else //��ʱ--���յ����ݰ�������or��ʱδ��Ӧ
			{										
				HandleStep = 0;
				GUI_UserAccess(10); //10--ָ�ƺϲ�ʧ�� 
				return 0xEF00;
			}
		}
		break;

		case 13://���ָ���Ƿ�����Ȩ
		{
			SendCmdToFingerprint_Search(1, 0, MAX_ZW_UNM);//��⵱ǰMAX_ZW_UNMöָ��
			HandleStep++;
			LastScanTime = gstSysRunInfo.u32TimeCount_ms;
		}
		break;

		case 14:
		{
			if(gstSysRunInfo.u32TimeCount_ms-LastScanTime < 700)
			{
			 	cRet = CheckFingerprintReply(4, FingerprintReplyData);				
	 			if(0x09 == cRet) //δ������
	 			{
	 				HandleStep++;
	 			}
				else if(0x00 == cRet) //ָ������Ȩ
				{
					HandleStep = 0;
					GUI_UserAccess(12);
					return 0xF000;
				}
				else if(0xFF != cRet)
				{
					HandleStep = 0;
					GUI_UserAccess(9);
					return 0xF100;
			 	}
			}
			else //��ʱ--���յ����ݰ�������or��ʱδ��Ӧ
			{										
				HandleStep = 0;
				GUI_UserAccess(9);	
				return 0xF300;
			}
		}
		break;

		case 15://�洢ָ��
		{
			SendCmdToFingerprint_Store(1, FingerNum);
			HandleStep++;
			LastScanTime = gstSysRunInfo.u32TimeCount_ms;
		}
		break;

		case 16:
		{
			if(gstSysRunInfo.u32TimeCount_ms-LastScanTime < 500)
			{
				cRet = CheckFingerprintReply(0, FingerprintReplyData);				
				if(0x00 == cRet)
	 			{
	 				HandleStep = 0;
					GUI_UserAccess(6);
					return 0xABCD; //ָ��¼��ɹ�,��ɲ���
	 			}
				else if(0xFF != cRet)
				{
					HandleStep = 0;
					GUI_UserAccess(7);
					return 0xF500;
				}
			}
			else //��ʱ--���յ����ݰ�������or��ʱδ��Ӧ
			{
				HandleStep = 0;
				GUI_UserAccess(7); //ָ�ƴ洢ʧ��
				return 0xF600;
			}
		}
		break;
		
		default:
		{
			HandleStep =0;
		}
		break;
	}

	return (HandleStep +0xA000);
}

/***********************************************************************************************
* Function Name  : Fingerprint_GetChar
* Description	 : ��ȡһ��ָ������
* Input 		 : FingerID -- �洢��ָ�ƺ� 
* Output		 : None
* Return		 : 0x01--��ȡ�ɹ�, ���� --��ȡʧ��, 
* Note  		 :  
***********************************************************************************************/
uchar8 Fingerprint_GetChar(uint16 FingerID)
{
	uchar8 u8Ret = 0;
	uchar8 Buf[12] = {0};
	uint16 Len = 0;
		
	memset(FingerprintData, 0x0, sizeof(FingerprintData));
	
	//����ģ�鵽Buffer1��
	Buf[Len++] = 1;
	Buf[Len++] = (FingerID & 0xFF00) >>8;
	Buf[Len++] = FingerID & 0xFF;
	SendCmdToFingerprint(0x01, ZW_CMD_LoadChar, Buf, Len);
	u8Ret = AwaitingerprintResult(321);
	if(0 == u8Ret)
	{
		//�ϴ�������ģ��
		Len = 0;
		memset(Buf, 0x0, sizeof(Buf));
		Buf[Len++] = 1;  //Buffer1
		SendCmdToFingerprint(0x01, ZW_CMD_UpChar, Buf, Len);
		u8Ret = AwaitingerprintResult(520);
		delay_ms(520);  //�ȴ�ָ������
		if(0 == u8Ret)
		{
			if(568 == FingerprintDataCnt)
			{
				//��ȡָ������
				FingerprintData[0] = (FingerID &0xFF00) >>8;
				FingerprintData[1] = FingerID & 0xFF;
				memcpy(&FingerprintData[2], &FingerprintDataBuf[21], 128);
				memcpy(&FingerprintData[130], &FingerprintDataBuf[160], 128);
				memcpy(&FingerprintData[258], &FingerprintDataBuf[299], 128);
				memcpy(&FingerprintData[386], &FingerprintDataBuf[438], 128);

				u8Ret = 1;
			}
			else
			{
				u8Ret = 0xFF;
			}
		}
		else
		{
			u8Ret = 0xFF;
		}
	}
	
	return u8Ret;
}

/*========================File end==============================================================================*/

