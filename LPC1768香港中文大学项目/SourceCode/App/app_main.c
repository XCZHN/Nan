/*===============================================================================================
==                                                                                              
**  Copyright(c) 2014  ZhenYao Science and technology co., LTD.   All rights reserved.
==                                                                                           
**	Author:		Asen                                                                     
==                                                                                          
**  Version:	V1.0.0                                                                          
==
**  Filename:	Main.c                                                                       
==                                                                                         
**  Function:	Application Main source file
==
**  Data:       2015/09/11                                                                       
================================================================================================*/

#include "Includes.h"


const char  START_MESSAGE[]=
{
	//内存标记信息
	"Let us face the reality, so that we are committed to our ideal!"
};

const char  ProgramVer_Tab[]=
{
	//程序版本号：
	"V3.5.180203"
};

const char  Firmware_Tab[]=
{
	//程序标签
	"[随机寄存柜]"
};

/*===============================================================================================
程序版本修改记录:



=★= V2.5.170408 =★=
1.将管理卡独立开,以便设置多张管理卡

=★= V3.2.17063001 =★=
1.支持多种随机使用模式


=★= V3.2.170817xx =★=
1.增加ID卡功能,并增加设置开关

=★= V3.2.170904xx =★=
1.增加独立寄存按键功能

=★= V3.4.170908xx =★=
1.修复功能卡设置机号时保存数据时的错误
2.增加设置管理员密码功能

=★= V3.4.170922xx =★=
1.增加指纹寄存功能

=★= V3.5.171021xx =★=
1.修复网络指令锁箱解锁的BUG
2.修改本机清除预授权卡功能BUG
================================================================================================*/



/***********************************************************************************************
* Function Name   : Variable_Init
* Description	  : 上电初始化变量.
* Input		      : None
* Output		  : None
* Return		  : None
***********************************************************************************************/
void Variable_Init(void)
{
	memset((uchar8*)&gstSysRunInfo, 0x0, sizeof(gstSysRunInfo));	
	memset((uchar8*)&gstCmdPacket, 0x0, sizeof(gstCmdPacket));
	memset((uchar8*)&gstKeyboard, 0x0, sizeof(gstKeyboard));
}	
 
 
/***********************************************************************************************
* Function Name  : System_Init
* Description	 : 系统初始化
* Input		     : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
void System_Init(void)
{
	SystemInit();	

	SysTick_Init(1000);
	SysTimerInit();
	pTimer_DelayTick  = TimerCreate(1, 1, TICK_COUNT_1MS,   TimingDelay_Decrement, NULL);
	pTimer_TimeCount  = TimerCreate(1, 1, TICK_COUNT_10MS,  TimeCount, NULL);
	pTimer_Beep		  = TimerCreate(1, 1, TICK_COUNT_10MS,  BeepCallback, NULL);
	pTimer_LedBlink	  = TimerCreate(0, 1, TICK_COUNT_500MS, SignalLedBlink, NULL);
	pTimer_ReadRTC    = TimerCreate(1, 1, TICK_COUNT_500MS, Tick_ReadRTC, NULL);
	SysTick_Enable();

	GPIO_ConfigInit();
	
	Time0_Init();
	UART0_Init(19200);
	UART1_Init(57600);
	UART2_Init(9600);
	UART3_Init(115200);
	Beep_Init();
	LED_Init();
	SPI_Host_Init();
	I2C_Eeprom_Init();
	FLASH_Init();
	RTC_Init();
	Drv_74HC164_Init();
	Drv_74HC165_Init();
	CloseAllBox();
	ExtendFunction_Init();
	RF_Init();

	Voice_Init();	 
	Keyboard_Init();
	
	Beep_Init();
	LED_Init();
	LCD_Init();
	Fingerprint_Init();
}


/*==============================================================================================*/
/* 						(*^__^*)	 Main Function	   (*^__^*) 							    */ 		 
/*==============================================================================================*/
int main(void)
{		
 	Variable_Init();
	
	System_Init();
	delay_ms(300);
	ReadSysConfig();
	if(VALIDITY_USING == SysConfig.stLockers.bEnableIdCard)
	{
		Wiegand_Init();
	}
	else if(VALIDITY_USING != SysConfig.stHardware.bDebugMode)
	{
	//	WDT_Init();
	}
	delay_ms(123);
	Set_VoiceLevel(SysConfig.stHardware.u8VoiceLevel);
	//delay_ms(321);

	if(VALIDITY_USING == SysConfig.stHardware.bNetwork)
	{
		gstSysRunInfo.bDebug_Net = 1; 
		Network_Init();
		WDT_Feed();	
		gstSysRunInfo.bDebug_Net = 0;
	}

	UpdateEmptyBoxList();       
	GUI_Admin();
	WDT_Feed(); 	
	SetLedBlink(TICK_COUNT_400MS, 0xFFFFFFEE); 
	Buzzer(700);
	
	while(1)
	{
		WDT_Feed();
		GetRTCTime(NULL);
		CardOperation(1);
		ScanAllBoxStatus();
		WDT_Feed();
		KeyboardFunction();
		DepositKeyDown();
		HandleTimeoutAlert();
		Lockers_TakeAwayByFinger();
		GUI_WorkStatus();
		WDT_Feed();
		Thread_UDP();
		Thread_CommHandle();
	}  
} 

/*===================================File end===================================================*/

 
