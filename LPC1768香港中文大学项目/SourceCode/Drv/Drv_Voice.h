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
**  Function:	语音模块驱动程序 
==                                                                                         
**  Data:       2016/04/06                                                                      
================================================================================================*/

#ifndef _VOICE_H_
#define _VOICE_H_

#include "Config.h"
#include "LPC17xx.h"


#define VOICE_BUSY_pin  	0      //未链接BUSY管脚

#define	Get_VoiceBusyBit	1	

//--------------------WT588D语音模块配置管脚-----------------
#define TXD2			8		// P2.8
#define RXD2			9		// P2.9

#define	SET_RSTYY_PIN		(LPC_GPIO2->FIOSET |= (1<<TXD2))
#define	CLR_RSTYY_PIN		(LPC_GPIO2->FIOCLR |= (1<<TXD2))
#define	SET_DATAYY_PIN		(LPC_GPIO2->FIOSET |= (1<<RXD2))
#define	CLR_DATAYY_PIN		(LPC_GPIO2->FIOCLR |= (1<<RXD2))



#define YY_Cmd_GotoSleep    0x0A  //进入睡眠 -- 低功耗
#define YY_Cmd_Reset        0x0C  //芯片复位
#define YY_Cmd_Play         0x0D  //播放
#define YY_Cmd_Stop         0x0E  //暂停

#define YY_Cmd_MoveNext     0x01  //上一曲
#define YY_Cmd_MovePrevious 0x02  //下一曲
#define YY_CurrentVolume    0x00  //播放当前音量


#define VOICE_LEVEL_MAX     0x1E  //最大音量


#define YY_Welcome                1 //0. 欢迎使用               
#define YY_Welcome_PhoneLockers   6 //1. 欢迎使用智能手机柜               
#define YY_QinShuRuXiangHao       2 //2. 请输入箱号             
#define YY_QinShuRuFangHao        3 //3. 请输入房号 

#define YY_QinShuRuMiMa           4 //4. 请输入密码             
#define YY_QinZaiCiShuRuMiMa      36 //请再次输入密码  
#define YY_QinChongXinShuRuMiMa   37 //请重新输入密码             
#define YY_HaoXiangMengYiKai      38 //号箱门已开
#define YY_CunWuTiShi             39 //放入物品后请关好箱门
#define YY_QuWuTiShi              40 //取出物品后请关好箱门

#define YY_PlsOldPwd              5 //5. 请输入原密码           
#define YY_PlsNewPwd              6 //6. 请输入新密码           
#define YY_PlsCard                7 //7. 请读卡                 
#define YY_XiangMengYiKai         8 //8. 箱门已开，请随手关门   
#define YY_WelcomeLockers         9 //9.欢迎使用智能储物柜 

#define YY_Num_0                  10 //数字0                     
#define YY_Num_1                  11 //数字1                     
#define YY_Num_2                  12 //数字2                     
#define YY_Num_3                  13 //数字3                     
#define YY_Num_4                  14 //数字4                     
#define YY_Num_5                  15 //数字5                     
#define YY_Num_6                  16 //数字6                     
#define YY_Num_7                  17 //数字7                     
#define YY_Num_8                  18 //数字8                     
#define YY_Num_9                  19 //数字9                     
#define YY_Num_Shi                20 //十                    

#define YY_QingGuanHaoXiangMen    6  //6.请关好箱门
#define YY_xBoxDoorOpen           21 //号箱门已开，请随手关门
#define YY_QingQueRenGuanMen      22 //请确认箱门是否关好
#define YY_xHaoXianMengWeiGuan    31 //31.号箱门未关请关门
#define YY_HaoXiangMengWeiGuan    31 //31.号箱门未关请关门

//------------------------指纹操作语音-------------------------------------
#define YY_Welcome                      1         //欢迎使用智能寄存柜
#define YY_QingShuXiangHao              2         //请输入箱号
#define YY_QingShuMiMa                  4         //请输入密码
#define YY_xBoxDoorOpen                 21        //号箱门已开，请随手关门
#define YY_QingAnZhiWen                 23        //23.请按指纹
#define YY_QingZaiAnZhiWen              24        //24.请再按指纹
#define YY_ZhiWenShouQuanOK             25        //25.指纹授权成功
#define YY_ZhiWenDuiBiOK                26        //26.指纹对比成功
#define YY_ZhiWenDuiBiShiBai            27        //27.指纹对比失败
#define YY_ZhiWenYiShouQuan             28        //28.指纹已授权
#define YY_ZhiWenWeiShouQuan            29        //29.指纹未授权
#define YY_ZhiWenShouQuanShiBai         30        //30.指纹授权失败
#define YY_xHaoXianMengWeiGuan          31        //31.号箱门未关请关门
#define YY_QingYiKaiShouZhi             32        //32.请移开手指
#define YY_JiChunShiBai                 33        //33.寄存失败请重新寄存
#define YY_XiangMenGuZhan               34        //34.箱门故障请联系管理员
#define YY_MenGuZhanQingJiCun           35        //35.箱门故障请重新寄存


/***********************************************************************************************
* Function Name  : Voice_SendCmd
* Description	 : 给语音模块发送指令
* Input 		 : u8Cmd -- 指令
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void Voice_SendCmd(uchar8 u8Cmd);

/***********************************************************************************************
* Function Name  : Voice_Init
* Description	 : 语音模块初始化操作
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void Voice_Init(void);

/***********************************************************************************************
* Function Name  : Set_VoiceLevel
* Description	 : 设置语音音量等级
* Input 		 : VoiceLvel--语音音量等级(0-1E)
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void Set_VoiceLevel(uchar8 VoiceLvel);

/***********************************************************************************************
* Function Name  : Voice_Play
* Description	 : 播放指定语音
* Input 		 : u16Voice -- 语音编号
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void Voice_Play(uint16 u16Voice);

/***********************************************************************************************
* Function Name  : Wait_VoiceModule_Free
* Description	 : 等待语言模块进入空闲状态(播放完上一指令语言)
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void Wait_VoiceModule_Free(void);


#endif
/*========================File end===================================================================================================*/

