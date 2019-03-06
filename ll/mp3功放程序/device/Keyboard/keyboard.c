//MP3 功放程序  按键控制部分
//QiYuan@SCUT
//2010/07/09
#include "main.h"
extern u8 State;
unsigned char key=255;

//获取键值
void get_key()
{
	key=0;
	if(LS148_A0) key=1;
	if(LS148_A1) key+=1<<1;
	if(LS148_A2) key+=1<<2;
}

//按键处理
void Key_Handle()
{
	get_key();
	while(PEN==0);
	if(State==0) 	//开机
	{
		State=1;
		key=255;
		return;
	}
	else if(State==1)	//主界面
	{
		if(key==KEY_A)
		{
			State=key_point+2;
		}
		else if(key==RIGHT)
		{
			 key_point=4;
		}
		else if(key==LEFT)
		{
			 key_point=0;
		}
		else
		{
			if(key==UP)
			{
				Show_str(key_point%4,104,"  ");			//显示fat类型
			 	if(key_point==0) key_point=3;
				else if(key_point==4) key_point=7;
				else key_point--;
				Show_str(key_point%4,104,"<-");			//显示fat类型
			}
			else if(key==DOWN)
			{
				Show_str(key_point%4,104,"  ");			//显示fat类型
			 	if(key_point==3) key_point=0;
				else if(key_point==7) key_point=4;
				else key_point+=1;
				Show_str(key_point%4,104,"<-");			//显示fat类型
			}			
		}

	}
	else if(State==4)	//显示时间
	{
		if(key==KEY_B)
		{
			State=1;
			return;
		}
	}
	else if(State==2) 	//音乐播放
	{
	 	
	}
	else if(State==5)	//FFT显示
	{
		
	}		
}

//外部中断服务程序
void EXTI9_5_IRQHandler(void)
{
	delay_ms(10);
	if(PEN!=0)
	{
		EXTI->PR=1<<8;        //清除中断标志位 
		return;
	}
	Key_Handle();	  		   	  		   
	EXTI->PR=1<<8;        //清除中断标志位 
} 				  

//中断优先级管理/开启	   
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	//存储器映射,不用理    
#ifdef  VECT_TAB_RAM  									   
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
#else   							 
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
#endif  				  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);//优先级分到第0组 总共5组		 
  	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQChannel; //使用外部中断8
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		 //响应优先级
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure); 								 
} 	

void keyboard_init(void)
{	
	//注意,时钟使能之后,对GPIO的操作才有效
	//所以上拉之前,必须使能时钟.才能实现真正的上拉输出
	RCC->APB2ENR|=1<<2;    //PA时钟使能
	RCC->APB2ENR|=1<<3;    //PB时钟使能
	RCC->APB2ENR|=0x01;    //使能io复用时钟
	AFIO->MAPR|=(1<<26);   //关闭JTAG-DP，关闭SW-DP
	GPIOB->CRH&=0XFFFFFFF0;//PB8输入
	GPIOB->CRH|=0X00000008;//pB8上拉输入
	GPIOB->ODR|=1<<8;
	
	GPIOA->CRH&=0X000FFFFF;//PA13 14 15输入
	GPIOA->CRH|=0X88800000;
	GPIOA->ODR|=(1<<13)|(1<<14)|(1<<15); 
 
	NVIC_Configuration();  
    AFIO->EXTICR[2]|=0X0001; //EXTI8映射到PB8
	EXTI->IMR|=1<<8;       //开启line6上的中断
	EXTI->EMR|=1<<8;       //不屏蔽line6上的事件
	EXTI->FTSR|=1<<8;      //line6上事件下降沿触发	
}

