/*====================================================================================
==                                                                                              
**  Copyright(c) 2014-09  Asen. All rights reserved.                                 
==                                                                                           
**	Author:		Asen                                                                        
==                                                                                          
**  Version:	v1.0.0                                                                          
==
**  Filename:	Drv_RFM.h                                                                       
==                                                                                         
**  Function:	����оƬ FM1702SL �������Ƴ���
==                                                                                         
**  Data:       2014/09/22                                                                       
=====================================================================================*/
#ifndef	_DRV_FM1702SL_H_
#define	_DRV_FM1702SL_H_


#include "Config.h"
#include "LPC17xx.h"



//-----------------------------Ӳ���ӿڹ��ܶ���---------------------------------------/
#define	FM_RST_pin	        10     				 //P1.10

#define FM_RST_H		    (LPC_GPIO1->FIOSET |= (1<<FM_RST_pin)) 
#define FM_RST_L    		(LPC_GPIO1->FIOCLR |= (1<<FM_RST_pin))


/******************** FM1702SL ������*****************************************/
#define Transceive 	0x1E	//���ͽ�������
#define Transmit   	0x1A	//��������
#define ReadE2     	0x03   	//��ȡFM1715 EEPROM����
#define WriteE2    	0x01   	//дFM1715 EEPROM����
#define Authent1   	0x0c   	//��֤������֤���̵�1��
#define Authent2   	0x14   	//��֤������֤���̵�2��
#define LoadKeyE2  	0x0B   	//����Կ��EEPROM���Ƶ�KEY����
#define LoadKey    	0x19   	//����Կ��FIFO memory���Ƶ�key memory

#define RF_TimeOut 	0x50  	//send command delay 
#define Req        	0x01   
#define Sel	   		0x02

/******************** FM1702SL �Ĵ�����ַ����*********************************/
/*  
	FM1702SL���ڲ��Ĵ��������ܲ�ͬ�ֳ�8��ÿ��Ϊһҳ����8���Ĵ���:
	Page0:ָ���״̬�Ĵ�����
	Page1:���ƺ�״̬�Ĵ�����
	Page2:���估������ƼĴ�����
	Page3:���ռ�������ƼĴ�����
	Page4:ʱ�估У����ƼĴ�����              FM1702SL_REG_
	Page5:FIFOTimer��IRQ���ƼĴ�����
	Page6~Page7:Ԥ���Ĵ�����
*/
#define FM1702SL_REG_Page_Sel			0x00	/* ѡ��Ĵ����� */
#define FM1702SL_REG_Command			0x01	/* ����Ĵ��� */
#define FM1702SL_REG_FIFO				0x02	/* 64�ֽ�FIFO�������������Ĵ��� */
#define FM1702SL_REG_PrimaryStatus		0x03	/* ��������������FIFO��״̬�Ĵ���1 */
#define FM1702SL_REG_FIFO_Length		0x04	/* ��ǰFIFO���ֽ����Ĵ��� */
#define FM1702SL_REG_SecondaryStatus	0x05	/* ����״̬��ʾ�Ĵ��� */
#define FM1702SL_REG_InterruptEn		0x06	/* �ж�ʹ��/��ֹ���ƼĴ��� */
#define FM1702SL_REG_Int_Req			0x07	/* �ж������ʶ�Ĵ��� */
#define FM1702SL_REG_Control			0x09	/* ���ֿ��Ʊ�ʾ�Ĵ��� */
#define FM1702SL_REG_ErrorFlag			0x0A	/* ��һ��ָ�����������ʾ */
#define FM1702SL_REG_CollPos			0x0B	/* ��⵽�ĵ�һ����ͻλ��λ�� */
#define FM1702SL_REG_TimerValue			0x0C	/* ��ʱ����ǰֵ */
#define FM1702SL_REG_Bit_Frame			0x0F	/* λ֡�����Ĵ��� */
#define FM1702SL_REG_TxControl			0x11	/* ���Ϳ��ƼĴ��� */
#define FM1702SL_REG_CWConductance		0x12	/* ѡ�����TX1��TX2�������ߵ��迹 */
#define FM1702SL_REG_ModConductance		0x13	/* ������������迹 */
#define FM1702SL_REG_CoderControl		0x14	/* �������ģʽ��ʱ��Ƶ�� */
#define FM1702SL_REG_DecoderControl		0x1a	/* ������ƼĴ��� */
#define FM1702SL_REG_RxControl1			0x19	/* ����������� */
#define FM1702SL_REG_RxControl2			0x1e	/* ������Ƽ�ѡ�����Դ */
#define FM1702SL_REG_RxWait				0x21	/* ѡ����ͽ���֮���ʱ���� */
#define FM1702SL_REG_ChannelRedundancy	0x22	/* ѡ������У�������ģʽ */
#define FM1702SL_REG_CRCPresetLSB		0x23	/* CRCԤ�üĴ�����8λ */
#define FM1702SL_REG_CRCPresetMSB		0x24	/* CRCԤ�üĴ�����8λ */
#define FM1702SL_REG_MFOUTSelect		0x26	/* mfOUT configure register */
#define FM1702SL_REG_TimerClock			0x2a	/* ѡ��TIMERʱ�ӵķ�Ƶ */
#define FM1702SL_REG_TimerControl		0x2b	/* ѡ��TIMER����/ֹͣ���� */
#define FM1702SL_REG_TimerReload		0x2c	/* TIMERԤ��ֵ */
#define FM1702SL_REG_TypeSH				0x31	/* ��֤ģʽѡ�� */
#define FM1702SL_REG_TestDigiSelect		0x3d	/* Test pin configure register */

#define FM1702_OK		    0
/************************* ����������붨�� ERR CODE  *************************/
#define FM1702_NOTAGERR		1		/* �޿� */
#define FM1702_CRCERR		2		/* ��ƬCRCУ����� */
#define FM1702_EMPTY		3		/* ��ֵ������� */
#define FM1702_AUTHERR		4		/* ��֤���ɹ� */
#define FM1702_PARITYERR	5		/* ��Ƭ��żУ����� */
#define FM1702_CODEERR		6		/* ͨѶ����(BCCУ���) */
#define FM1702_SERNRERR		8		/* ��Ƭ���кŴ���(anti-collision ����) */
#define FM1702_SELECTERR	9		/* ��Ƭ���ݳ����ֽڴ���(SELECT����) */
#define FM1702_NOTAUTHERR	10		/* ��Ƭû��ͨ����֤ */
#define FM1702_BITCOUNTERR	11		/* �ӿ�Ƭ���յ���λ������ */
#define FM1702_BYTECOUNTERR	12		/* �ӿ�Ƭ���յ����ֽ����������������Ч */
#define FM1702_RESTERR		13		/* ����restore�������� */
#define FM1702_TRANSERR		14		/* ����transfer�������� */
#define FM1702_WRITEERR		15		/* ����write�������� */
#define FM1702_INCRERR		16		/* ����increment�������� */
#define FM1702_DECRERR		17		/* ����decrement�������� */
#define FM1702_READERR		18		/* ����read�������� */
#define FM1702_LOADKEYERR	19		/* ����LOADKEY�������� */
#define FM1702_FRAMINGERR	20		/* FM1702֡���� */
#define FM1702_REQERR		21		/* ����req�������� */
#define FM1702_SELERR		22		/* ����sel�������� */

#define FM1702_ANTICOLLERR	23		/* 24 ����anticoll�������� */
#define FM1702_INTIVALERR	24		/* 25 ���ó�ʼ���������� */
#define FM1702_READVALERR	25		/* 26 ���ø߼�����ֵ�������� */
#define FM1702_DESELECTERR	26		/* 27 */
#define FM1702_CMD_ERR		42		/* 28 ������� */

/************************* ��Ƶ��ͨ�������붨�� *************************/
#define RF_CMD_REQUEST_STD	0x26	//�����Զ���ָ��
#define RF_CMD_REQUEST_ALL	0x52	//ȫ�Զ�����ָ��
#define RF_CMD_ANTICOL		0x93	//���ص�
#define RF_CMD_SELECT		0x93	//ѡ��Ƭ
#define RF_CMD_AUTH_LA		0x60	//ѡ�� Key_A
#define RF_CMD_AUTH_LB		0x61	//ѡ�� Key_B
#define RF_CMD_READ		    0x30	//��
#define RF_CMD_WRITE		0xa0	//д
#define RF_CMD_INC		    0xc1	//��ֵ
#define RF_CMD_DEC		    0xc0	//��ֵ
#define RF_CMD_RESTORE		0xc2	//�ش�
#define RF_CMD_TRANSFER		0xb0	//����
#define RF_CMD_HALT		    0x50	//ͣ��

/************************* ��Ƭ���Ͷ��嶨�� ********************************/
#define TYPEA_MODE			0		//type A mode	
#define AuthKey_A			0x60
#define AuthKey_B			0x61

#define TM0_HIGH			0xf0	//timer0 high bit,4ms setting
#define TM0_LOW				0x60    //timer0 low bit
#define TIMEOUT				(150) 	//Timer out ,4ms x150 =0.6s
#define RF_TRY_COUNT        (3)		//��д���������Դ���

#define CARD_MAX_SECTION    (16)	//MF1������������
#define CARD_MAX_BLOCK      (64)	//MF1�����ݿ������



//************************======function declare========************************
extern uchar8	g_Card_RevBuffer[40];			// command send/receive buffer
extern uchar8   g_Card_tagtype[2];				// ��Ƭ��ʶ�ַ� 
extern uchar8	g_Card_UID[5];					// ���������

extern uchar8	RF_Init(void);
extern uchar8	MIF_Request(uchar8 mode);
extern uchar8	MIF_AntiColl(void);
extern uchar8 MIF_Select(void);
extern uchar8 MIF_Load_Key(uchar8 *keybuf);
extern uchar8 MIF_Authentication(uchar8 *pCardUID, uchar8 SecNR, uchar8 keyType);
extern uchar8 MIF_Read(uchar8 *databuf, uchar8 Block_Adr);
extern uchar8 MIF_Write(uchar8 *databuf, uchar8 Block_Adr);
extern uchar8 MIF_Halt(void);
extern uchar8 RF_ActiveCard(uchar8* pUID, uint8 u8Section, uchar8 u8KeyType, uchar8* pKey);
extern uchar8	RF_Find_Card(void);
extern uchar8	RF_Read_CardUID(uchar8* pUID);
extern uint32	RF_GetCardUID(void);
extern uint8 RF_ReadCard_1Block(uchar8* pUID, uint8 u8Block, uchar8 u8KeyType, uchar8* pKey, uchar8* pData);
extern uint8 RF_WriteCard_1Block(uchar8* pUID, uint8 u8Block, uchar8 u8KeyType, uchar8* pKey, uchar8* pData);
extern uint8 RF_ReadCard(uchar8* pUID, uint8 sector, uchar8 u8KeyType, uchar8* pKey, uchar8* pData);
extern uint8 RF_WriteCard(uchar8* pUID, uint8 sector, uchar8 u8KeyType, uchar8* pKey, uchar8* pData);
extern uint8 RF_BlockValueIncrement(uchar8* pUID, uint8 u8Block, uchar8 u8KeyType, uchar8* pKey, uchar8* pData);
extern uint8 RF_BlockValueDecrement(uchar8* pUID, uint8 u8Block, uchar8 u8KeyType, uchar8* pKey, uchar8* pData);
extern uint8 RF_BlockRestore(uchar8* pUID, uint8 u8Block, uchar8 u8KeyType, uchar8* pKey);

#endif
/*========================File end===================================================================================================*/

