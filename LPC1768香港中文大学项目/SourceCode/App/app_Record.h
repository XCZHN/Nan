/*===============================================================================================
==                                                                                              
**  Copyright(c) 2015  ZhenYao Science and technology co., LTD.   All rights reserved.
==                                                                                           
**	Author:		Asen                                                                           
==                                                                                          
**  Version:	v1.0.0                                                                          
==
**  Filename:	App_Record.h                                                                       
==                                                                                         
**  Function:	��¼����                                                             
==
**  Data:       2016-03-10
================================================================================================*/

#ifndef _RECORD_H_
#define _RECORD_H_



typedef enum 							    //�����¼���ͱ�
{
	OpenBoxType_Admin =0,                   //����Ա���� 
	OpenBoxType_Password,					//����(����֤��)���� 				
	OpenBoxType_Card,					    //ˢ������
	OpenBoxType_CardPwd,					//��+�ܿ���
	OpenBoxType_Fingerprint,				//ָ�ƿ���
	OpenBoxType_NetCmd,					    //����ָ���
	OpenBoxType_Postman,                    //Ͷ��Ա����					
	OpenBoxType_	
	
}E_RECORD_TYPE;

//----------------------------��¼--------------------------------------------
//#define  ONE_BOX_MAX_RECORD_NUM    (128)  //һ�������Ĵ洢��¼�� 4k/32 = 128
#define  ONE_RECORD_LEN            (32)     //����һ�ʼ�¼�����ݳ���
#define  MAX_RECORD_NUM            (10000)  //���Ĵ洢��¼�� 1M/32 = 32768

typedef	struct								//�����¼
{
	uint32 u32RecordNum;                    //��¼��
	uint32 u32BoxRecordID;                  //�ڶ�Ӧ���еļ�¼����
		
	uchar8 u8BoxID;                         //���
	uchar8 u8Type;                          //���䷽ʽ
	uchar8 u8SaveOrTake;                    //��ȡ״̬(0δ֪,1��/2ȡ)
	uchar8 u8Result;                        //������(0--�ɹ�,��0--ʧ��)
	uchar8 u8Crc;                           //CRCУ��

	uchar8 Time[6];                         //����ʱ��
	uchar8 HandleInfo[12];                  //���Ż��ֻ��Ż���Ա���
		
}ST_RECORD, *pST_RECORD;


//-------------------------------------------------Variable definitions--------------------------- 
extern ST_RECORD stRecord; 	




/***********************************************************************************************
* Function Name  : StoreOpenBoxRecord
* Description	 : �洢һ�ʿ����¼
* Input 		 : pstRecord -- ��¼��Ϣָ��
* Output		 : None
* Return		 : 0--�ɹ�,��0�ο��������
* Note  		 : һ�ʼ�¼�̶�Ϊ32Byte
***********************************************************************************************/
extern int StoreOpenBoxRecord(pST_RECORD pstRecord);

/***********************************************************************************************
* Function Name  : Clear_Record
* Description	 : ��������¼
* Input 		 : None
* Output		 : None
* Return		 : None
* Note  		 : ������п����¼
***********************************************************************************************/
extern void Clear_Record(void);

/***********************************************************************************************
* Function Name  : ReadOpenBoxRecord
* Description	 : ��ȡһ�ʿ����¼
* Input 		 : RecordNum --��¼��(0~N), pstRecord -- ��¼��Ϣָ��
* Output		 : None
* Return		 : 0--�ɹ�,��0�ο��������
* Note  		 : һ�ʼ�¼�̶�Ϊ32Byte
***********************************************************************************************/
extern int ReadOneRecord(uint32 RecordNum, pST_RECORD pstRecord);

/***********************************************************************************************
* Function Name  : ReadOneRecord_ToArrayBuf
* Description	 : ��ȡһ�ʿ����¼��������
* Input 		 : RecordNum --��¼��(0~N), pArrayBuf -- ��¼��Ϣָ��
* Output		 : None
* Return		 : 0--�ɹ�,��0�ο��������
* Note  		 : һ�ʼ�¼�̶�Ϊ32Byte
***********************************************************************************************/
extern int ReadOneRecord_ToArrayBuf(uint32 RecordNum, uchar8* pArrayBuf);

/***********************************************************************************************
* Function Name  : QueryBoxRecord
* Description	 : ��ѯһ�ʿ����¼
* Input 		 : BoxNum -- ���, RecordNum --��¼��(0~N), pstRecord -- ��¼��Ϣָ��
* Output		 : None
* Return		 : ���ڵļ�¼���
* Note  		 : һ�ʼ�¼�̶�Ϊ32Byte
***********************************************************************************************/
extern int QueryBoxRecord(uchar8 BoxNum, uint32 RecordID, pST_RECORD pstRecord);


/***********************************************************************************************
* Function Name  : StoreRecord_OpenBoxByCard
* Description	 : ����1��ˢ����ȡ����¼
* Input 		 : box--���, iCard--����, SaveOrTake--(0δ֪,1��/2ȡ)
* Output		 : None
* Return		 : None 
* Notes          : NULL
***********************************************************************************************/
extern void StoreRecord_OpenBoxByCard(uchar8 box, uint32 iCard, uchar8 SaveOrTake);

/***********************************************************************************************
* Function Name  : StoreRecord_OpenBoxByFinger
* Description	 : ����1��ָ�ƿ����¼
* Input 		 : box--���, SaveOrTake--(0δ֪,1��/2ȡ)
* Output		 : None
* Return		 : None 
* Notes          : NULL
***********************************************************************************************/
extern void StoreRecord_OpenBoxByFinger(uchar8 box, uchar8 SaveOrTake);

#endif //_RECORD_H_
/*===================================File end===================================================*/

