/*===============================================================================================
==                                                                                              
**  Copyright(c) 2015  Intelligent Admin electronic systems co., LTD. All rights reserved.
==                                                                                           
**	Author:		Asen                                                                           
==                                                                                          
**  Version:	v1.0.0                                                                          
==
**  Filename:	Lockers.c                                                                       
==                                                                                         
**  Function:	�����䴦����� 
==                                                                                         
**  Data:       2015/09/18                                                                     
================================================================================================*/

#include "Includes.h"

//-------------------------------------------------Variable definitions--------------------------- 


/***********************************************************************************************
* Function Name  : CheckBoxOnLockersNum
* Description	 : ���������ڵĹ��
* Input 		 : BoxNum -- ���, pExtBoxNum -- ����չ���е������
* Output		 : None
* Return		 : ������ڵĹ��(0--����, ����--������) 
* Note  		 : None 
***********************************************************************************************/
uchar8 CheckBoxOnLockersNum(uchar8 BoxNum, uchar8* pExtBoxNum)
{
	uchar8 LockersNum = 0;
	uchar8 ExtL01StartBoxNo = 0;   //1�Ÿ������ʼ���
	uchar8 ExtL02StartBoxNo = 0;   //2�Ÿ������ʼ���
	uchar8 ExtL03StartBoxNo = 0;   //3�Ÿ������ʼ���
	uchar8 ExtendBoxNum = 1;       //���������չ���е������

	if((0 == SysConfig.stLockers.u8ExtL01BoxNum)&&(0 == SysConfig.stLockers.u8ExtL02BoxNum))
	{
		*pExtBoxNum = BoxNum;
		return 0;
	}
	
	ExtL01StartBoxNo = SysConfig.stLockers.u8HostLockerBoxs +1;   
	ExtL02StartBoxNo = ExtL01StartBoxNo + SysConfig.stLockers.u8ExtL01BoxNum;
	ExtL03StartBoxNo = ExtL02StartBoxNo + SysConfig.stLockers.u8ExtL02BoxNum;

	if(BoxNum >= (ExtL03StartBoxNo + SysConfig.stLockers.u8ExtL03BoxNum))
	{
		LockersNum = 4;
		ExtendBoxNum = 0;  //������չ��Χ,��Ч���
	}
	else if(BoxNum >= ExtL03StartBoxNo)
	{
		LockersNum = 3;
		ExtendBoxNum = (BoxNum - ExtL03StartBoxNo) +1;
	}
	else if(BoxNum >= ExtL02StartBoxNo)
	{
		LockersNum = 2;
		ExtendBoxNum = (BoxNum - ExtL02StartBoxNo) +1;
	}
	else if(BoxNum >= ExtL01StartBoxNo)
	{
		LockersNum = 1;
		ExtendBoxNum = (BoxNum - ExtL01StartBoxNo) +1;
	}
	else  //��������
	{
		ExtendBoxNum = BoxNum; 
	}
	
	*pExtBoxNum = ExtendBoxNum;
	return LockersNum;
}

/***********************************************************************************************
* Function Name  : CheckEmptyBoxNum
* Description	 : ��������
* Input 		 : None
* Output		 : None
* Return		 : ������
* Note  		 :  
***********************************************************************************************/
uchar8 CheckEmptyBoxNum(void)
{
	int i = 0;
	uchar8 EmptyBoxNum = 0;

	for(i=1; i<=SysConfig.stLockers.u8BoxCount; i++)
	{
		if((BOX_STATUS_IDLE == SysConfig.stBox[i].u8Status)&&(LSTATUS_FREE == SysConfig.stBox[i].bLocking))
		{
			EmptyBoxNum ++;
		}
	}
	gstSysRunInfo.u8EmptyBoxCount = EmptyBoxNum;
		
	return EmptyBoxNum;
}


/***********************************************************************************************
* Function Name  : ParseBoxDoorStatus
* Description	 : ��������״̬,����״̬��ŵ� gstSysRunInfo.BoxDoorStatus
* Input 		 : LockerNum--���,��pStatusBuf--״̬���� 
* Output		 : None
* Return		 : None
* Note  		 :  
***********************************************************************************************/
void ParseBoxDoorStatus(uchar8 LockerNum, uchar8* pStatusBuf)
{
	uchar8 i = 0;
	uchar8 BoxByteAddr = 0;  //��������ֽ�
	uchar8 BoxBitAddr = 0;   //��������ֽڵ�λ��ַ
	uchar8 BoxCount = 0;
	uchar8 BoxStatusAddr = 0;

	if(0 == LockerNum)
	{
		BoxCount = SysConfig.stLockers.u8HostLockerBoxs;
		BoxStatusAddr = 0;
		for(i=1; i<= BoxCount; i++)		
		{
			BoxByteAddr = ((i-1) /8);						   //�������״̬�����ֽ�
			BoxBitAddr = ((i-1) %8);						   //�������״̬���ڶ�Ӧ��λ
			gstSysRunInfo.BoxDoorStatus[BoxStatusAddr + i] = ((pStatusBuf[BoxByteAddr] >> BoxBitAddr) &0x01);
		}	
	}
	else if(1 == LockerNum)
	{
		BoxCount = SysConfig.stLockers.u8ExtL01BoxNum;
		BoxStatusAddr = SysConfig.stLockers.u8HostLockerBoxs + 1;
		memcpy((uchar8*)&gstSysRunInfo.BoxDoorStatus[BoxStatusAddr], pStatusBuf, BoxCount);
	}
	else if(2 == LockerNum)
	{
		BoxCount = SysConfig.stLockers.u8ExtL02BoxNum;
		BoxStatusAddr = SysConfig.stLockers.u8HostLockerBoxs + SysConfig.stLockers.u8ExtL01BoxNum + 1;
		memcpy((uchar8*)&gstSysRunInfo.BoxDoorStatus[BoxStatusAddr], pStatusBuf, BoxCount);
	}
	else if(3 == LockerNum)
	{
		BoxCount = SysConfig.stLockers.u8ExtL03BoxNum;
			
		BoxStatusAddr = SysConfig.stLockers.u8HostLockerBoxs + 1; 
		BoxStatusAddr += SysConfig.stLockers.u8ExtL01BoxNum; 
		BoxStatusAddr += SysConfig.stLockers.u8ExtL02BoxNum;
		memcpy((uchar8*)&gstSysRunInfo.BoxDoorStatus[BoxStatusAddr], pStatusBuf, BoxCount);
	}
}

/***********************************************************************************************
* Function Name  : ParseBoxInsideStatus
* Description	 : ��������״̬,����״̬��ŵ� gstSysRunInfo.BoxInsideStatus
* Input 		 : LockerNum--���,��pStatusBuf--״̬���� 
* Output		 : None
* Return		 : None
* Note  		 :  
***********************************************************************************************/
void ParseBoxInsideStatus(uchar8 LockerNum, uchar8* pStatusBuf)
{
	uchar8 i = 0;
	uchar8 BoxByteAddr = 0;  //��������ֽ�
	uchar8 BoxBitAddr = 0;   //��������ֽڵ�λ��ַ
	uchar8 BoxCount = 0;
	uchar8 BoxStatusAddr = 0;

	if(0 == LockerNum)
	{
		for(i=1; i<= SysConfig.stLockers.u8HostLockerBoxs; i++)		
		{
			BoxByteAddr = ((i-1) /8);						   //�������״̬�����ֽ�
			BoxBitAddr = ((i-1) %8);						   //�������״̬���ڶ�Ӧ��λ
			gstSysRunInfo.BoxInsideStatus[i] = ((pStatusBuf[BoxByteAddr] >> BoxBitAddr) &0x01);
		}	
	}
	else if(1 == LockerNum)
	{
		BoxCount = SysConfig.stLockers.u8ExtL01BoxNum;
		BoxStatusAddr = SysConfig.stLockers.u8HostLockerBoxs +1;
		memcpy((uchar8*)&gstSysRunInfo.BoxInsideStatus[BoxStatusAddr], pStatusBuf, BoxCount);
	}
	else if(2 == LockerNum)
	{
		BoxCount = SysConfig.stLockers.u8ExtL02BoxNum;
		BoxStatusAddr = SysConfig.stLockers.u8HostLockerBoxs + SysConfig.stLockers.u8ExtL01BoxNum +1;
		memcpy((uchar8*)&gstSysRunInfo.BoxInsideStatus[BoxStatusAddr], pStatusBuf, BoxCount);
	}
	else if(3 == LockerNum)
	{
		BoxCount = SysConfig.stLockers.u8ExtL03BoxNum;
			
		BoxStatusAddr = SysConfig.stLockers.u8HostLockerBoxs +1; 
		BoxStatusAddr += SysConfig.stLockers.u8ExtL01BoxNum; 
		BoxStatusAddr += SysConfig.stLockers.u8ExtL02BoxNum;
		memcpy((uchar8*)&gstSysRunInfo.BoxInsideStatus[BoxStatusAddr], pStatusBuf, BoxCount);
	}
}

/***********************************************************************************************
* Function Name  : ScanLocalBoxStatus
* Description	 : ɨ�豾������,����״̬
* Input 		 : None
* Output		 : None
* Return		 : None
* Note  		 :
***********************************************************************************************/
void ScanLocalBoxStatus(void)
{
	static uint32 LastScanTime;
	static uchar8 ScanStep;
		
	switch(ScanStep)
	{
		case 0: //����ִ��ʱ����,����ɨ��̫��Ƶ��
		{
			if(gstSysRunInfo.u32TimeCount_ms - LastScanTime > 600)
			{
				LastScanTime = gstSysRunInfo.u32TimeCount_ms;
				ScanStep ++;
			}
		}
		break;
		
		case 1: //ɨ������״̬
		{
			memset((uchar8*)&gstSysRunInfo.TempBuf[0], 0x0, sizeof(gstSysRunInfo.TempBuf));
			gstSysRunInfo.u8Temp = GetBoxDoorStatus((uchar8*)&gstSysRunInfo.TempBuf[0], SysConfig.stLockers.u8HostLockerBoxs);
			ParseBoxDoorStatus(0, (uchar8*)gstSysRunInfo.TempBuf);
			ScanStep = 0;
		}
		break;

		default:
		{
			ScanStep = 0;
		}
		break;
	}
}

/***********************************************************************************************
* Function Name  : ScanExtendBoxStatus
* Description	 : ��չ��������\���״̬
* Input 		 : bReset -- ��λ��־(0��Ч)
* Output		 : None
* Return		 : None
* Note  		 : ��ǰ֧����չ3������  
***********************************************************************************************/
void ScanExtendBoxStatus(uchar8 bReset)
{
	static uint32 LastScanTime;
	static uchar8 HandleStep;

	if(0 == bReset)
	{
		HandleStep = 0;	
		LastScanTime = gstSysRunInfo.u32TimeCount_ms;
		return;
	}
	
	if((SYS_STATUS_KEY == gstSysRunInfo.CurrentStatus)&&(6 == gstKeyboard.u8AdminMenuStep))
	{
		return;	
	}
	else if(gstSysRunInfo.bRS485Busy)
	{
		LastScanTime = gstSysRunInfo.u32TimeCount_ms;
		return;
	}

	switch(HandleStep)
	{
		case 0: //ɨ��״̬(����ִ��ʱ����,����ɨ��̫��Ƶ��)
		{
			if(gstSysRunInfo.u32TimeCount_ms - LastScanTime > 300)
			{
				LastScanTime = gstSysRunInfo.u32TimeCount_ms;
				if(SysConfig.stLockers.u8ExtL01BoxNum)
				{
					HandleStep ++;
				}
			}
			else
			{
				HandleStep = 0;	
			}
		}
		break;

		case 1: //ɨ����չ���ذ�1������״̬ 
		{
			Clean_RS485RxdBuf();

			gstSysRunInfo.TempBuf[0] = 1;
			gstSysRunInfo.TempBuf[1] = SysConfig.stLockers.u8ExtL01BoxNum;
			SendCmdToExtendLockers(1, CMD_GET_BOXDOOR_STATE, (uchar8*)&gstSysRunInfo.TempBuf[0], 2);

			LastScanTime = gstSysRunInfo.u32TimeCount_ms;
			HandleStep ++;
		}
		break;

		case 2: //��һ���ٶ�ȡRS485���ص����� 
		{
			if(gstSysRunInfo.u32TimeCount_ms - LastScanTime > 200)  //ʵ����19200BPS��,���ܻ�Ӧ���ʱ��Ϊ135ms
			{
				if(RS485DataCnt > CMD_PACKET_MIN_LEN)
				{					
					memset((uchar8*)&stCmdSlavePacket, 0x0, sizeof(stCmdSlavePacket));
					if(CMD_GET_BOXDOOR_STATE == SlavePacketDecoding(RS485DataBuf, RS485DataCnt, &stCmdSlavePacket))
					{
						if(1 == stCmdSlavePacket.iLockerNum)  //������
						{
							//Ӧ��ָ���е����ݽṹΪ ִ��״̬1+��ʼ���ź�1+������1+����״̬N 	
							ParseBoxDoorStatus(1, (uchar8*)&stCmdSlavePacket.DataBuf[3]);
						}						
					}
				}	
				
				if(SysConfig.stLockers.u8ExtL02BoxNum)
				{
					HandleStep ++;
				}
				else
				{
					LastScanTime = gstSysRunInfo.u32TimeCount_ms;
					HandleStep =0;
				}
			}
		}
		break;
		
		case 3: //ɨ����չ���ذ�2������״̬ 
		{
			Clean_RS485RxdBuf();

			gstSysRunInfo.TempBuf[0] = 1;
			gstSysRunInfo.TempBuf[1] = SysConfig.stLockers.u8ExtL02BoxNum;
			SendCmdToExtendLockers(2, CMD_GET_BOXDOOR_STATE, (uchar8*)&gstSysRunInfo.TempBuf[0], 2);

			LastScanTime = gstSysRunInfo.u32TimeCount_ms;
			HandleStep ++;
		}
		break;		

		case 4: //��һ���ٶ�ȡRS485���ص����� 
		{
			if(gstSysRunInfo.u32TimeCount_ms - LastScanTime > 200)
			{
				if(RS485DataCnt > CMD_PACKET_MIN_LEN)
				{					
					memset((uchar8*)&stCmdSlavePacket, 0x0, sizeof(stCmdSlavePacket));
					if(CMD_GET_BOXDOOR_STATE == SlavePacketDecoding(RS485DataBuf, RS485DataCnt, &stCmdSlavePacket))
					{
						if(2 == stCmdSlavePacket.iLockerNum)  //������
						{
							//Ӧ��ָ���е����ݽṹΪ ִ��״̬1+��ʼ���ź�1+������1+����״̬N 	
							ParseBoxDoorStatus(2, (uchar8*)&stCmdSlavePacket.DataBuf[3]);
						}
					}
				}					
				
				if(SysConfig.stLockers.u8ExtL03BoxNum)
				{
					HandleStep ++;
				}
				else
				{
					LastScanTime = gstSysRunInfo.u32TimeCount_ms;
					HandleStep =0;
				}
			}
		}
		break;

		case 5: //ɨ����չ���ذ�3������״̬ 
		{
			Clean_RS485RxdBuf();

			gstSysRunInfo.TempBuf[0] = 1;
			gstSysRunInfo.TempBuf[1] = SysConfig.stLockers.u8ExtL03BoxNum;
			SendCmdToExtendLockers(3, CMD_GET_BOXDOOR_STATE, (uchar8*)&gstSysRunInfo.TempBuf[0], 2);

			LastScanTime = gstSysRunInfo.u32TimeCount_ms;
			HandleStep ++;
		}
		break;		

		case 6: //��һ���ٶ�ȡRS485���ص����� 
		{
			if(gstSysRunInfo.u32TimeCount_ms - LastScanTime > 200)
			{
				LastScanTime = gstSysRunInfo.u32TimeCount_ms;
				if(RS485DataCnt)
				{					
					memset((uchar8*)&stCmdSlavePacket, 0x0, sizeof(stCmdSlavePacket));
					if(CMD_GET_BOXDOOR_STATE == SlavePacketDecoding(RS485DataBuf, RS485DataCnt, &stCmdSlavePacket))
					{
						if(3 == stCmdSlavePacket.iLockerNum)  //������
						{
							//Ӧ��ָ���е����ݽṹΪ ִ��״̬1+��ʼ���ź�1+������1+����״̬N 	
							ParseBoxDoorStatus(3, (uchar8*)&stCmdSlavePacket.DataBuf[3]);
						}
					}
				}	
				LastScanTime = gstSysRunInfo.u32TimeCount_ms;
				HandleStep = 0;
			}
		}
		break;
			
		default:
		{
			HandleStep = 0;	
		}
		break;
	}
}

/***********************************************************************************************
* Function Name  : ScanAllBoxStatus
* Description	 : ɨ����������״̬
* Input 		 : void
* Output		 : None
* Return		 : None
* Note  		 : ������չ��������״̬
***********************************************************************************************/
void ScanAllBoxStatus(void)
{
	if(SysConfig.stLockers.bCheckBoxDoorStatus)
	{
		ScanLocalBoxStatus();
		ScanExtendBoxStatus(1);
	}
}

/***********************************************************************************************
* Function Name  : Choice1BoxOpen
* Description	 : �Զ�����һ������, ����������Ƿ����ɹ�
* Input 		 : Box -- �����������ź�
* Output		 : None
* Return		 : ִ�н�� 0--����ɹ�, ����--����ʧ��
* Notes          : �Զ��ж����������Ǹ�����
***********************************************************************************************/
uchar8 Choice1BoxOpen(uint8 BoxNum)
{
	uchar8 ExtendLockerNum = 0;
	uchar8 ExtendNewBoxNum = 0;
	uchar8 SendBuf[20] = {0};
		
	if(BoxNum <= SysConfig.stLockers.u8HostLockerBoxs)
	{
		OpenOneBox(BoxNum);
	}
	else
	{
		ExtendLockerNum = CheckBoxOnLockersNum(BoxNum, &ExtendNewBoxNum);
		if(ExtendLockerNum) 
		{
			SendBuf[0] = ExtendNewBoxNum;
			gstSysRunInfo.bRS485Busy = 1;
			delay_ms(220);
			SendCmdToExtendLockers(ExtendLockerNum, CMD_OPEN_BOX, SendBuf, 1);
			gstSysRunInfo.bRS485Busy = 0;
			ScanExtendBoxStatus(0);
		}
	}
	return 0;
}

/***********************************************************************************************
* Function Name  : Choice1BoxOpen_CheckResult
* Description	 : �Զ�����һ������,���Ҽ������״̬,����ʧ�����ٳ���һ��
* Input 		 : Box -- �����������ź�
* Output		 : None
* Return		 : ִ�н�� 0--����ɹ�, ����--����ʧ��
* Notes          : �Զ��ж����������Ǹ�����
***********************************************************************************************/
uchar8 Choice1BoxOpen_CheckResult(uint8 BoxNum)
{
	uchar8 status = 0;
	uchar8 SendBuf[20] = {0};
	uchar8 WaitCut = 0;
	uchar8 ExtendLockerNum = 0;
	uchar8 ExtendNewBoxNum = 0;
		
	if(BoxNum <= SysConfig.stLockers.u8HostLockerBoxs)
	{
		OpenOneBox(BoxNum);
		delay_ms(800);
		memset((uchar8*)&gstSysRunInfo.TempBuf[0], 0x0, sizeof(gstSysRunInfo.TempBuf));
		gstSysRunInfo.u8Temp = GetBoxDoorStatus((uchar8*)&gstSysRunInfo.TempBuf[0], SysConfig.stLockers.u8HostLockerBoxs);
		ParseBoxDoorStatus(0, (uchar8*)gstSysRunInfo.TempBuf);
		if(0 == gstSysRunInfo.BoxDoorStatus[BoxNum])  //����ʧ���ٳ���һ��
		{
			OpenOneBox(BoxNum);
			delay_ms(800);
			memset((uchar8*)&gstSysRunInfo.TempBuf[0], 0x0, sizeof(gstSysRunInfo.TempBuf));
			gstSysRunInfo.u8Temp = GetBoxDoorStatus((uchar8*)&gstSysRunInfo.TempBuf[0], SysConfig.stLockers.u8HostLockerBoxs);
			ParseBoxDoorStatus(0, (uchar8*)gstSysRunInfo.TempBuf);
			if(0 == gstSysRunInfo.BoxDoorStatus[BoxNum])  //����ʧ�� 
			{
				status = 0xEE;		
			}
		}
	}
	else
	{
		gstSysRunInfo.bRS485Busy = 1;
		delay_ms(220);

		ExtendLockerNum = CheckBoxOnLockersNum(BoxNum, &ExtendNewBoxNum);
		if(ExtendLockerNum) 
		{
			Clean_RS485RxdBuf();
			SendBuf[0] = ExtendNewBoxNum;
			SendCmdToExtendLockers(ExtendLockerNum, CMD_OPEN_BOX_RETURN_STATUS, SendBuf, 1);
			WaitCut = 30;
			status = 0xEE;	
			while(WaitCut --)
			{						
				if(RS485DataCnt >= 14)
				{					
					memset((uchar8*)&stCmdSlavePacket, 0x0, sizeof(stCmdSlavePacket));
					if(CMD_OPEN_BOX_RETURN_STATUS == SlavePacketDecoding(RS485DataBuf, RS485DataCnt, &stCmdSlavePacket))
					{
						Clean_RS485RxdBuf();
						if((ExtendLockerNum == stCmdSlavePacket.iLockerNum)&&(CMD_OPEN_BOX_RETURN_STATUS == stCmdSlavePacket.u8Cmd))	//�����ź������
						{
							if(0 == stCmdSlavePacket.DataBuf[0])
							{
								//�����ɹ�
								status = 0; 
							}
							else
							{
								status = 0xEE;	
							}
							break;
						}
					}
				}
				delay_ms(100);
			}
		}
		gstSysRunInfo.bRS485Busy = 0;
		ScanExtendBoxStatus(0);
	}
	return status;
}

/***********************************************************************************************
* Function Name  : SelectOneBoxToOpen
* Description	 : ѡ����ĳ������
* Input 		 : boxId -- �����������ź�, bCheckResult:0����⿪�Ž��,��0���⿪�Ž��
* Output		 : None
* Return		 : ִ�н�� 0--����ɹ�, ����--����ʧ��
* Notes          : �Զ��ж����������Ǹ�����
***********************************************************************************************/
uchar8 SelectOneBoxToOpen(uint8 boxId, uchar8 bCheckResult)
{
	uchar8 status = 0;

	if(bCheckResult)
	{
		status = Choice1BoxOpen_CheckResult(boxId);
		if(OK != status)
		{
			status = Choice1BoxOpen_CheckResult(boxId);
		}
	}
	else
	{
		status = Choice1BoxOpen(boxId);
	}
	
	return status;
}

/***********************************************************************************************
* Function Name  : HandleTimeoutAlert
* Description	 : �Ĵ���ų�ʱ����
* Input 		 : 
* Output		 : None
* Return		 : None
***********************************************************************************************/
void HandleTimeoutAlert(void)
{
	static uint32 iLastHandleTime;
	static uchar8 step;

	switch(step)
	{
		case 0: //����ʱ
		{
			if((gstSysRunInfo.u32TimeCount_ms - iLastHandleTime) >= 1000) 
			{
				iLastHandleTime = gstSysRunInfo.u32TimeCount_ms; 

				if(gstSysRunInfo.iHandleTimeout)
				{
					gstSysRunInfo.iHandleTimeout --;
					if(0 == gstSysRunInfo.iHandleTimeout)
					{
						//���Ĵ�ʱ�򿪵��Ź���û��
						if(1 == gstSysRunInfo.BoxDoorStatus[gstSysRunInfo.u8UseBox])
						{
							if(SYS_STATUS_IDLE == gstSysRunInfo.CurrentStatus)
							{
								gstSysRunInfo.CurrentStatus = SYS_STATUS_Message;
								Beep(BEEP_TIME_200MS, BEEP_TIME_200MS, 0xFFFFFFFF);
								GUI_LockersNotice(LOCKERS_PlsCloseBoxDoor);
								iLastHandleTime = gstSysRunInfo.u32TimeCount_ms; 
								step ++;
							}	
						}
					}
				}
			}
		}
		break;

		case 1: //��ʾ��Ϣͣ��
		{
			//���Ĵ�ʱ�򿪵��Ź���û��
			if((0 == gstSysRunInfo.BoxDoorStatus[gstSysRunInfo.u8UseBox])
				||((gstSysRunInfo.u32TimeCount_ms - iLastHandleTime) > (SysConfig.stLockers.iNoticeTime * 1000)))
			{
				Beep(BEEP_TIME_200MS, BEEP_TIME_200MS, 1);	
				GUI_Admin();
				gstSysRunInfo.CurrentStatus = SYS_STATUS_IDLE;
				step = 0;
			}
		}
		break;
		
		default:
		{
			step = 0;
		}
		break;
	}	
}

/***********************************************************************************************
* Function Name  : UpdateEmptyBoxList
* Description	 : ���¿����б�
* Input 		 : None
* Output		 : None
* Return		 : None
* Note  		 : ����������װ�ص������б��� 
***********************************************************************************************/
void UpdateEmptyBoxList(void)
{
	int i = 0, j = 0;

	memset((uchar8*)&gstSysRunInfo.EmptyBox[0], 0x0, sizeof(gstSysRunInfo.EmptyBox));

	gstSysRunInfo.u8EmptyBoxCount = 0;
	for(i=1; i<=SysConfig.stLockers.u8BoxCount; i++)
	{
		if((BOX_STATUS_IDLE == SysConfig.stBox[i].u8Status)&&(LSTATUS_FREE == SysConfig.stBox[i].bLocking))
		{
			gstSysRunInfo.EmptyBox[j++] = i;
			gstSysRunInfo.u8EmptyBoxCount ++;
		}
	}
}

/***********************************************************************************************
* Function Name  : AddOneEmptyBox
* Description	 : ���������ĩβ����һ���������
* Input 		 : box--���
* Output		 : None
* Return		 : None
* Note  		 : None 
***********************************************************************************************/
void AddOneEmptyBox(uchar8 box)
{
	uchar8 i = 0;

	//�Ȳ�ѯ�¿����б�����û��
	for(i=0; i<sizeof(gstSysRunInfo.EmptyBox); i++)
	{
		if(box == gstSysRunInfo.EmptyBox[i])
		{
			return;
		}
	}
	
	//���������λ
	for(i=0; i<SysConfig.stLockers.u8BoxCount; i++)
	{
		if(0 == gstSysRunInfo.EmptyBox[i])
		{
			gstSysRunInfo.EmptyBox[i] = box;
			gstSysRunInfo.u8EmptyBoxCount +=1;
			break;
		}
	}
}

/***********************************************************************************************
* Function Name  : AddOneEmptyBox
* Description	 : �����������ɾ��һ���������
* Input 		 : box--���
* Output		 : None
* Return		 : None
* Note  		 : None 
***********************************************************************************************/
void DeleteOneEmptyBox(uchar8 box)
{
	uchar8 i = 0;
	uint32 Addr = 0;

	//����
	for(i=0; i<sizeof(gstSysRunInfo.EmptyBox); i++)
	{
		if(box == gstSysRunInfo.EmptyBox[i])
		{
			Addr = i;
			break;
		}
	}
	
	if(i == sizeof(gstSysRunInfo.EmptyBox))
	{
		//û�ҵ�
		return;
	}
	else
	{
		//ɾ��
		for(i=Addr; i<(sizeof(gstSysRunInfo.EmptyBox)-1); i++)
		{
			gstSysRunInfo.EmptyBox[i] = gstSysRunInfo.EmptyBox[i+1];
		}
		//ȷ�����һ��Ϊ0
		gstSysRunInfo.EmptyBox[(sizeof(gstSysRunInfo.EmptyBox)-1)] = 0;
		gstSysRunInfo.u8EmptyBoxCount -= 1;
	}
}

/***********************************************************************************************
* Function Name  : CheckBoxStatusChange
* Description	 : �������\����״̬�ı�
* Input 		 : None
* Output		 : None
* Return		 : None
* Note  		 : ����鵽�иı�ʱ�����ϴ���������
***********************************************************************************************/
void CheckBoxStatusChange(void)
{
	static uchar8 BoxDoorStatus[MAX_BOX_NUM+1] = {0};     
	static uchar8 BoxInsideStatus[MAX_BOX_NUM+1] = {0};   

	if(memcmp(&BoxDoorStatus[1], (uchar8*)&gstSysRunInfo.BoxDoorStatus[1], SysConfig.stLockers.u8BoxCount))
	{
		memcpy(&BoxDoorStatus[1], (uchar8*)&gstSysRunInfo.BoxDoorStatus[1], SysConfig.stLockers.u8BoxCount);
	}
	else if(memcmp(&BoxInsideStatus[1], (uchar8*)&gstSysRunInfo.BoxInsideStatus[1], SysConfig.stLockers.u8BoxCount))
	{
		memcpy(&BoxInsideStatus[1], (uchar8*)&gstSysRunInfo.BoxInsideStatus[1], SysConfig.stLockers.u8BoxCount);
	}
}

/***********************************************************************************************
* Function Name  : CheckValidityDate
* Description	 : ���ĳ�����ڵ���Ч��
* Input 		 : pDate -- ����
* Output		 : None
* Return		 : 0x00 -- δ������Ч��, ����Ϊ��Ч����(��),С��0��ʾ�ѹ�������
***********************************************************************************************/
int	CheckValidityDate(uchar8* pDate)
{
	int iRet = VALIDITY_USING;
	uint32 iCurrentTime = 0;
	uint32 iValidityTime = 0;

	if((0 == pDate[0])||(0xFF == pDate[0]))
	{
		//δ����ʹ����Ч��
		return 0;
	}
	else if((99 == pDate[0])&&(12 == pDate[1]))
	{
		//������Ч
		return 0;
	}

	iCurrentTime = (gstSysRunInfo.stRtcTime.u8Year * 365) + (gstSysRunInfo.stRtcTime.u8Month * 30) + gstSysRunInfo.stRtcTime.u8Day;
	iValidityTime = (pDate[0] * 365) + (pDate[1] *30) + pDate[2];

	iRet = iValidityTime - iCurrentTime;
	return iRet;
}


/***********************************************************************************************
* Function Name  : VoicePlay_DoorNotClose
* Description	 : ����"xx������δ��,�����!"
* Input 		 : u8DoorNum -- �ź�
* Output		 : None
* Return		 : None
***********************************************************************************************/
void VoicePlay_DoorNotClose(uchar8 u8DoorNum)
{
#if GUI_ENGLISH
	Voice_Play(3);
	Voice_Play(u8DoorNum+10);
	Voice_Play(38);
#else
	uchar8 NumBit = 0;
	uchar8 PlayNum = 0;

	if(u8DoorNum > 99)
	{
		return;
	}
	else if(u8DoorNum > 9)
	{
		NumBit = 2;
	}
	else
	{
		NumBit = 1;	
	}

	switch(NumBit)
	{
		case 2:
		{	
			PlayNum = (u8DoorNum / 10) %10;	
			if(1 == PlayNum)
			{
				Voice_Play(YY_Num_Shi);
			}
			else if(PlayNum > 1)
			{
				Voice_Play(PlayNum + YY_Num_0);
				Voice_Play(YY_Num_Shi);
			}
			else if(0 == PlayNum)
			{
				if((3 == NumBit)&&(u8DoorNum % 10))
				{
					Voice_Play(YY_Num_0);
				}
			}
		}
		case 1:
		{
			PlayNum = (u8DoorNum % 10);
			if(PlayNum)
			{
				Voice_Play(PlayNum + YY_Num_0);
			}
			Voice_Play(YY_HaoXiangMengWeiGuan);
		}
		break;
	}
#endif
}

/***********************************************************************************************
* Function Name  : VoicePlay_xBoxOpen
* Description	 : ����"xx�������ѿ�!"
* Input 		 : u8Box -- ���ź�
* Output		 : None
* Return		 : None
***********************************************************************************************/
void VoicePlay_xBoxOpen(uchar8 u8Box)
{
#if GUI_ENGLISH
	Voice_Play(3);
	Voice_Play(u8Box+10);
	Voice_Play(5);
#else
	uchar8 NumBit = 0;
	uchar8 PlayNum = 0;

	if(u8Box > 99)
	{
		return;
	}
	else if(u8Box > 9)
	{
		NumBit = 2;
	}
	else
	{
		NumBit = 1;	
	}

	switch(NumBit)
	{
		case 2:
		{	
			PlayNum = (u8Box / 10) %10;	
			if(1 == PlayNum)
			{
				Voice_Play(YY_Num_Shi);
			}
			else if(PlayNum > 1)
			{
				Voice_Play(PlayNum + YY_Num_0);
				Voice_Play(YY_Num_Shi);
			}
			else if(0 == PlayNum)
			{
				if((3 == NumBit)&&(u8Box % 10))
				{
					Voice_Play(YY_Num_0);
				}
			}
		}
		case 1:
		{
			PlayNum = (u8Box % 10);
			if(PlayNum)
			{
				Voice_Play(PlayNum + YY_Num_0);
			}
			Voice_Play(YY_HaoXiangMengYiKai);
		}
		break;
	}
#endif
}


/*========================File end======================================================================*/

