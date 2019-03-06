//加速度传感器硬件函数
//2010/06/10 revise
//QiYuan@SCUT
#include "main.h"

u8 adc_count=0;//记录adc的位置  
u8 adc_over=0;   //64次adc转换完成

void Timer2_Init(void);

void ADC_GPIO_Init(void)
{
	RCC->APB2ENR|=1<<2;		//先使能外设IO PORTA时钟
	GPIOA->CRL&=0xfffff0ff;	//PA2模拟输入
}

void ADC1_Init()
{
	RCC->CFGR|=(ADC_CLK_12<<14);			//ADC时钟12M
	RCC->APB2ENR|=(1<<9);					//使能ADC1时钟
	ADC1->CR1|=(1<<8);						//模式：独立，扫描

	//ADC1->CR2|=(1<<1)|(1<<8);				//连续转换,DMA使能
	ADC1->CR2|=(0<<1)|(0<<8);				//单次转换,DMA不使能
	ADC1->SMPR1|=(5<<0)|(5<<3)|(5<<6);		//采样周期为55.5个ADC_CLK
	ADC1->SQR1|=(0<<20);					//共转换1个通道
	ADC1->SQR3|=(2<<0);						//依次转换通道2  PA2
	ADC1->CR2|=(1<<ADON);					//使能ADC1
	ADC1->CR2|=(1<<ADON);					//使能ADC1,必须要执行两次才可以启动ADC，否则不启动，i don't know why
	ADC1->CR2|=(1<<RSTCAL);					//复位校准
	while(ADC1->CR2&(1<<RSTCAL));			//等待复位校准结束
	ADC1->CR2|=(1<<CAL);					//AD校准
	while((ADC1->CR2)&(1<<CAL));			//等待AD校准结束
}


//TIMER2中断优先级设定					  	   
void NVIC_TIMER2Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	//存储器映射,不用理    
#ifdef  VECT_TAB_RAM  									   
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
#else   							 
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);		 
	//第0组,没有抢断优先级
	//第4组,没有响应优先级				  
	//第2组,有4个抢断,4个响应
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//优先级分到第1组 总共5组		 
  	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQChannel; 
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//阶级1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		 //阶层0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure); 
	//设置TIM2的中断 												  		 		   	 
  	NVIC_Init(&NVIC_InitStructure);        
#endif  				  					   							 
}   

//开启中断,并映射 
void Timer2_Init(void)
{	
	NVIC_TIMER2Configuration();	
	RCC->APB1ENR|=1<<0;//TIM2时钟使能    
	TIM2->PSC=17;		//预分频器18,得到4Mhz的计数时钟
	TIM2->ARR=90;		//自动重装
	TIM2->CNT=90;		//44Khz ADC采样率										
	TIM2->DIER|=1<<0;   //允许更新中断	
	TIM2->DIER|=1<<6;   //允许触发中断
	TIM2->SR|=(1<<0);		  //清楚中断标志
	TIM2->CR1=1<<7;  //ARPE使能 
	TIM2->CR1|=1<<0;   //使能定时器2				 					  										  
} 

 void ADC_DMA_int(void)
{
	ADC_GPIO_Init();
	ADC1_Init();
	Timer2_Init();	
}



//定时器2中断服务函数
void TIM2_IRQHandler(void)
{
	if(TIM2->SR&(1<<0))	  		//判断是否更新中断
	{
		ADC1->CR2|=(1<<ADON);			//启动转换
		ADC1->CR2|=(1<<ADON);			//启动转换
		while((ADC1->SR&(1<<EOC))==0);	//等待转换结束
		XR[adc_count]=(ADC1->DR);	//让值减小
		XR[adc_count]=XR[adc_count]/1000;	 
		adc_count++;
		if(adc_count==128)
		{
			adc_count=0;//清零
			adc_over=1;
			TIM2->CR1&=~(1<<0);   //关闭定时器2 
		} 
	}
	TIM2->SR&=~(1<<0);		    //清楚中断标志	
}




 	 	

