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

#ifndef	_APP_FINGERPRINT_H_
#define	_APP_FINGERPRINT_H_

/*----------------- Fingerprint module define ----------------------------------*/

#define	FG_EN_pin	           28		//P4.28

#define	Fingerprint_pin  	2        //P2.2


#define GET_Fingerprint_STATE     (LPC_GPIO2->FIOPIN&0x04)   

#define MAX_ZW_UNM                     1000
//------------------------ָ��ָ��-------------------------------------
#define ZW_PackageID_CMD               0x01      //���ݰ���ʶ--ָ���
#define ZW_PackageID_PACKAGE           0x02      //���ݰ���ʶ--���ݰ�

#define ZW_CMD_GetImg                  0x01      //��ȡͼ��
#define ZW_CMD_GenChar                 0x02      //����������
#define ZW_CMD_Match                   0x03      //��ȷ�Ա���öָ��
#define ZW_CMD_Search                  0x04      //����ָ��
#define ZW_CMD_RegModel                0x05      //�����ϳ�ģ��
#define ZW_CMD_Store                   0x06      //�洢ģ��
#define ZW_CMD_LoadChar                0x07      //����ģ��
#define ZW_CMD_UpChar                  0x08      //�ϴ�������ģ��
#define ZW_CMD_DownChar                0x09      //����������ģ�� 
#define ZW_CMD_DeletChar               0x0C      //ɾ��ģ��
#define ZW_CMD_Empty                   0x0D      //���ָ��
#define ZW_CMD_SetSysPara              0x0E      //����ģ��ϵͳ��������
#define ZW_CMD_ReadSysPara             0x0F      //��ϵͳ����
#define ZW_CMD_WriteNotepad            0x18      //д���±� 
#define ZW_CMD_ReadNotepad             0x19      //�����±�
#define ZW_CMD_TempleteNum             0x1D      //����Чģ�����  

#define ZW_StopHandle                  0xFFFF    //ִֹͣ��

extern uchar8 FingerprintData[520];  //�洢ָ������(ָ�ƺ�2B + 512Bָ������)

/***********************************************************************************************
* Function Name  : Fingerprint_Init
* Description	 : ָ��ģ���ʼ��
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void Fingerprint_Init(void);

/***********************************************************************************************
* Function Name  : Fingerprint_PowerOn
* Description	 : ָ��ģ���ϵ�
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void Fingerprint_PowerOn(void);

/***********************************************************************************************
* Function Name  : Fingerprint_PowerOff
* Description	 : ָ��ģ��ص�
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void Fingerprint_PowerOff(void);

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
extern void SendCmdToFingerprint_GetImg(void);

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
extern void SendCmdToFingerprint_GenChar(uchar8 BufferID);

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
extern void SendCmdToFingerprint_RegModel(void);

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
extern void SendCmdToFingerprint_Store(uchar8 BufferID, uint16 PageID);

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
extern void SendCmdToFingerprint_DeletChar(uint16 PageID, uint16 num);

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
extern void SendCmdToFingerprint_Empty(void);

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
extern void SendCmdToFingerprint_Search(uchar8 BufferID, uint16 StartPage, uint16 PageNum);



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
extern uint16 Fingerprint_Search(uint16 handle);

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
extern uint16 Fingerprint_Login(uint16 FingerNum);

/***********************************************************************************************
* Function Name  : Fingerprint_GetChar
* Description	 : ��ȡһ��ָ������
* Input 		 : FingerID -- �洢��ָ�ƺ� 
* Output		 : None
* Return		 : 0x01--��ȡ�ɹ�, ���� --��ȡʧ��, 
* Note  		 :  
***********************************************************************************************/
extern uchar8 Fingerprint_GetChar(uint16 FingerID);


#endif
/*========================File end=======================================================*/

