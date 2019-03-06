/*===============================================================================================
==                                                                                              
**  Copyright(c) 2016  Intelligent Admin electronic systems co., LTD. All rights reserved.
==                                                                                           
**	Author:		Asen                                                                        
==                                                                                          
**  Version:	v1.0.0                                                                          
==
**  Filename:	Drv_Voice.h                                                                      
==                                                                                         
**  Function:	����ģ���������� 
==                                                                                         
**  Data:       2016/04/06                                                                      
================================================================================================*/

#ifndef _VOICE_H_
#define _VOICE_H_

#include "Config.h"
#include "LPC17xx.h"


#define VOICE_BUSY_pin  	0      //δ����BUSY�ܽ�

#define	Get_VoiceBusyBit	1	

//--------------------WT588D����ģ�����ùܽ�-----------------
#define TXD2			8		// P2.8
#define RXD2			9		// P2.9

#define	SET_RSTYY_PIN		(LPC_GPIO2->FIOSET |= (1<<TXD2))
#define	CLR_RSTYY_PIN		(LPC_GPIO2->FIOCLR |= (1<<TXD2))
#define	SET_DATAYY_PIN		(LPC_GPIO2->FIOSET |= (1<<RXD2))
#define	CLR_DATAYY_PIN		(LPC_GPIO2->FIOCLR |= (1<<RXD2))



#define YY_Cmd_GotoSleep    0x0A  //����˯�� -- �͹���
#define YY_Cmd_Reset        0x0C  //оƬ��λ
#define YY_Cmd_Play         0x0D  //����
#define YY_Cmd_Stop         0x0E  //��ͣ

#define YY_Cmd_MoveNext     0x01  //��һ��
#define YY_Cmd_MovePrevious 0x02  //��һ��
#define YY_CurrentVolume    0x00  //���ŵ�ǰ����


#define VOICE_LEVEL_MAX     0x1E  //�������


#define YY_Welcome                1 //0. ��ӭʹ��               
#define YY_Welcome_PhoneLockers   6 //1. ��ӭʹ�������ֻ���               
#define YY_QinShuRuXiangHao       2 //2. ���������             
#define YY_QinShuRuFangHao        3 //3. �����뷿�� 

#define YY_QinShuRuMiMa           4 //4. ����������             
#define YY_QinZaiCiShuRuMiMa      36 //���ٴ���������  
#define YY_QinChongXinShuRuMiMa   37 //��������������             
#define YY_HaoXiangMengYiKai      38 //�������ѿ�
#define YY_CunWuTiShi             39 //������Ʒ����غ�����
#define YY_QuWuTiShi              40 //ȡ����Ʒ����غ�����

#define YY_PlsOldPwd              5 //5. ������ԭ����           
#define YY_PlsNewPwd              6 //6. ������������           
#define YY_PlsCard                7 //7. �����                 
#define YY_XiangMengYiKai         8 //8. �����ѿ��������ֹ���   
#define YY_WelcomeLockers         9 //9.��ӭʹ�����ܴ���� 

#define YY_Num_0                  10 //����0                     
#define YY_Num_1                  11 //����1                     
#define YY_Num_2                  12 //����2                     
#define YY_Num_3                  13 //����3                     
#define YY_Num_4                  14 //����4                     
#define YY_Num_5                  15 //����5                     
#define YY_Num_6                  16 //����6                     
#define YY_Num_7                  17 //����7                     
#define YY_Num_8                  18 //����8                     
#define YY_Num_9                  19 //����9                     
#define YY_Num_Shi                20 //ʮ                    

#define YY_QingGuanHaoXiangMen    6  //6.��غ�����
#define YY_xBoxDoorOpen           21 //�������ѿ��������ֹ���
#define YY_QingQueRenGuanMen      22 //��ȷ�������Ƿ�غ�
#define YY_xHaoXianMengWeiGuan    31 //31.������δ�������
#define YY_HaoXiangMengWeiGuan    31 //31.������δ�������

//------------------------ָ�Ʋ�������-------------------------------------
#define YY_Welcome                      1         //��ӭʹ�����ܼĴ��
#define YY_QingShuXiangHao              2         //���������
#define YY_QingShuMiMa                  4         //����������
#define YY_xBoxDoorOpen                 21        //�������ѿ��������ֹ���
#define YY_QingAnZhiWen                 23        //23.�밴ָ��
#define YY_QingZaiAnZhiWen              24        //24.���ٰ�ָ��
#define YY_ZhiWenShouQuanOK             25        //25.ָ����Ȩ�ɹ�
#define YY_ZhiWenDuiBiOK                26        //26.ָ�ƶԱȳɹ�
#define YY_ZhiWenDuiBiShiBai            27        //27.ָ�ƶԱ�ʧ��
#define YY_ZhiWenYiShouQuan             28        //28.ָ������Ȩ
#define YY_ZhiWenWeiShouQuan            29        //29.ָ��δ��Ȩ
#define YY_ZhiWenShouQuanShiBai         30        //30.ָ����Ȩʧ��
#define YY_xHaoXianMengWeiGuan          31        //31.������δ�������
#define YY_QingYiKaiShouZhi             32        //32.���ƿ���ָ
#define YY_JiChunShiBai                 33        //33.�Ĵ�ʧ�������¼Ĵ�
#define YY_XiangMenGuZhan               34        //34.���Ź�������ϵ����Ա
#define YY_MenGuZhanQingJiCun           35        //35.���Ź��������¼Ĵ�


/***********************************************************************************************
* Function Name  : Voice_SendCmd
* Description	 : ������ģ�鷢��ָ��
* Input 		 : u8Cmd -- ָ��
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void Voice_SendCmd(uchar8 u8Cmd);

/***********************************************************************************************
* Function Name  : Voice_Init
* Description	 : ����ģ���ʼ������
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void Voice_Init(void);

/***********************************************************************************************
* Function Name  : Set_VoiceLevel
* Description	 : �������������ȼ�
* Input 		 : VoiceLvel--���������ȼ�(0-1E)
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void Set_VoiceLevel(uchar8 VoiceLvel);

/***********************************************************************************************
* Function Name  : Voice_Play
* Description	 : ����ָ������
* Input 		 : u16Voice -- �������
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void Voice_Play(uint16 u16Voice);

/***********************************************************************************************
* Function Name  : Wait_VoiceModule_Free
* Description	 : �ȴ�����ģ��������״̬(��������һָ������)
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void Wait_VoiceModule_Free(void);


#endif
/*========================File end===================================================================================================*/

