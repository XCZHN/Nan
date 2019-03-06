/*===============================================================================================
==                                                                                              
**  Copyright(c) 2014  Intelligent Admin electronic systems co., LTD. All rights reserved.
==                                                                                           
**	Author:		Asen                                                                           
==                                                                                          
**  Version:	v1.0.0                                                                          
==
**  Filename:	ComDeal.h                                                                       
==                                                                                         
**  Function:	����ͨ��Ӧ�ò��ļ�  
==
**  Data:       2014-08-18
================================================================================================*/

#ifndef	_APP_COM_DEAL_H_
#define	_APP_COM_DEAL_H_

#include "Config.h"   


//===============================������==========================================================
#define	CMD_GET_VERSION					0x01			//��ȡ�汾��    
#define	CMD_SET_REST					0x02	   		//Զ�̸�λ      
#define	CMD_SET_NUM						0x03			//���û���              
#define	CMD_GET_NUM						0x04			//��ȡ����(��������ͨ��ʱPC��ȡδ֪����)              
#define	CMD_SET_TIME					0x05        	//����ϵͳʱ��              
#define	CMD_GET_TIME					0x06        	//��ȡϵͳʱ��    
#define	CMD_PWR_OFF					    0x07	   		//Զ�̹ػ�     
#define	CMD_SYS_INIT					0x08	   		//ϵͳ��ʼ��    
#define	CMD_SET_NET_SWITCH              0x09            //�������翪��
#define	CMD_SET_IP                      0x0A            //����IP��ַ
#define	CMD_GET_IP                      0x0B            //��ȡIP��ַ
#define	CMD_SET_SERVER_IPINFO           0x0C            //���÷�����IP��ַ���˿ں�
#define	CMD_GET_SERVER_IPINFO           0x0D            //��ȡ������IP��ַ���˿ں�
#define	CMD_RING_BUZZER					0x10			//�������
#define	CMD_EEPROM_WRITE                0x25            //��EEPROMд����
#define	CMD_EEPROM_READ                 0x26            //��FLASH������ 

#define	CMD_FIND_CARD					0x2A			//Ѱ��			
#define	CMD_SET_CARD_KEY                0x2D            //���ÿ���Կ
#define	CMD_GET_CARD_KEY                0x2E            //��ȡ����Կ

#define	CMD_PLAY_VOICE				    0x51			//����ָ������
#define	CMD_SET_VOICE_LEVEL			    0x53			//���������ȼ�
#define	CMD_VOICE_CMD_TEST			    0x54			//����ģ��ָ�����

#define	CMD_SET_BOX_NUM                 0xA0            //���ð�������
#define	CMD_OPEN_BOX_RETURN_STATUS      0xA1            //���䲢�ҷ���״̬
#define	CMD_OPEN_BOX                    0xA2            //����(������״̬)
#define	CMD_GET_BOXDOOR_STATE           0xA3		    //��ȡ����״̬
#define	CMD_GET_BOX_STATE               0xA4		    //��ȡ����״̬
#define	CMD_SET_BOX_INFO				0xA5            //����ָ�������Ϣ
#define	CMD_GET_BOX_INFO				0xA6			//��ȡָ�������Ϣ
#define	CMD_SET_LOCKERS                 0xAA            //����ϵͳ����ģʽ
#define	CMD_SET_VALID_CARD_LIST			0xAB            //����Ԥ��Ȩ���ű�
#define	CMD_GET_VALID_CARD_LIST			0xAC			//��ȡԤ��Ȩ���ű�

#define	CMD_SET_BoxUserCard             0xB1            //�����û���
#define	CMD_GET_BoxUserCard             0xB2            //��ȡ�û���
#define	CMD_GET_AllBoxCard              0xB4            //��ȡ�����Ŀ���
#define	CMD_LockBox                     0xB5            //����
#define	CMD_UnlockBox                   0xB6            //������
#define	CMD_SET_Admin                   0xB8            //���ù���Ա�˺ź�����
#define	CMD_GET_RecordPoint             0xB9            //��ȡ��¼ָ��
#define	CMD_GET_RECORD                  0xBA            //�ɼ���¼
#define	CMD_GET_RECORD_AGAIN            0xBB            //���ɼ���¼
#define	CMD_CLR_AllRecord               0xBC            //�����¼

#define	CMD_ADD_Fingerprint             0xD1            //����ָ������
#define	CMD_CLR_Fingerprint             0xD2            //ɾ��ָ������
#define	CMD_GET_Fingerprint             0xD3            //��ȡָ������

#define	CMD_USER_ASK_CONSIGN            0xF1            //�û�����Ĵ�
#define	CMD_USER_ASK_TAKEAWAY           0xF2            //�û�����ȡ��

//----------------------ִ�д����------------------------------------------- 
#define Err_EmptyCmd					0xEF			//����Чָ����ظ�
#define Err_DataLen					    0x0E			//���ݳ��ȴ���
#define Err_Data						0xE0			//�����ֲ���
#define Err_Time						0xE1			//ʱ�����ڸ�ʽ����
#define Err_Eeprom						0xE4			//EEPROM�����¼ʧ��
#define Err_FlashErase                  0xE5            //Flash����ʧ��
#define Err_FlashWrite                  0xE6            //Flashдʧ��
#define Err_FlashRead                   0xE7            //Flash��ʧ��
#define Err_EepromWrite                 0xE8            //Eepromдʧ��
#define Err_EepromRead                  0xE9            //Eeprom��ʧ��
#define Err_UpdateVoiceFile             0xEA            //���������ļ�ʧ��
#define Err_BoxInfo						0xEB			//�����Ϣ����
#define Err_FindCard					0xF0			//Ѱ������
#define Err_ReadCard					0xF1		    //��������
#define Err_WriteCard					0xF2			//д������
#define Err_NoRecord                    0xF5            //�޴˼�¼  



/******************************************************************************************************
* Function Name  : Thread_CommHandle
* Description    : ������յ������ݣ���֤���ͻ���ͨ�����������ִ��
* Input          : None
* Variable       : g_stCmdPacket -- ͨѶָ�����ݰ�
* Output         : None
* Return         : None
******************************************************************************************************/
extern void Thread_CommHandle(void);



#endif
/*========================File end=======================================================*/

