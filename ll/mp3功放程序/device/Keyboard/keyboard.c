//MP3 ���ų���  �������Ʋ���
//QiYuan@SCUT
//2010/07/09
#include "main.h"
extern u8 State;
unsigned char key=255;

//��ȡ��ֵ
void get_key()
{
	key=0;
	if(LS148_A0) key=1;
	if(LS148_A1) key+=1<<1;
	if(LS148_A2) key+=1<<2;
}

//��������
void Key_Handle()
{
	get_key();
	while(PEN==0);
	if(State==0) 	//����
	{
		State=1;
		key=255;
		return;
	}
	else if(State==1)	//������
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
				Show_str(key_point%4,104,"  ");			//��ʾfat����
			 	if(key_point==0) key_point=3;
				else if(key_point==4) key_point=7;
				else key_point--;
				Show_str(key_point%4,104,"<-");			//��ʾfat����
			}
			else if(key==DOWN)
			{
				Show_str(key_point%4,104,"  ");			//��ʾfat����
			 	if(key_point==3) key_point=0;
				else if(key_point==7) key_point=4;
				else key_point+=1;
				Show_str(key_point%4,104,"<-");			//��ʾfat����
			}			
		}

	}
	else if(State==4)	//��ʾʱ��
	{
		if(key==KEY_B)
		{
			State=1;
			return;
		}
	}
	else if(State==2) 	//���ֲ���
	{
	 	
	}
	else if(State==5)	//FFT��ʾ
	{
		
	}		
}

//�ⲿ�жϷ������
void EXTI9_5_IRQHandler(void)
{
	delay_ms(10);
	if(PEN!=0)
	{
		EXTI->PR=1<<8;        //����жϱ�־λ 
		return;
	}
	Key_Handle();	  		   	  		   
	EXTI->PR=1<<8;        //����жϱ�־λ 
} 				  

//�ж����ȼ�����/����	   
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	//�洢��ӳ��,������    
#ifdef  VECT_TAB_RAM  									   
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
#else   							 
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
#endif  				  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);//���ȼ��ֵ���0�� �ܹ�5��		 
  	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQChannel; //ʹ���ⲿ�ж�8
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//��ռ���ȼ�
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		 //��Ӧ���ȼ�
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure); 								 
} 	

void keyboard_init(void)
{	
	//ע��,ʱ��ʹ��֮��,��GPIO�Ĳ�������Ч
	//��������֮ǰ,����ʹ��ʱ��.����ʵ���������������
	RCC->APB2ENR|=1<<2;    //PAʱ��ʹ��
	RCC->APB2ENR|=1<<3;    //PBʱ��ʹ��
	RCC->APB2ENR|=0x01;    //ʹ��io����ʱ��
	AFIO->MAPR|=(1<<26);   //�ر�JTAG-DP���ر�SW-DP
	GPIOB->CRH&=0XFFFFFFF0;//PB8����
	GPIOB->CRH|=0X00000008;//pB8��������
	GPIOB->ODR|=1<<8;
	
	GPIOA->CRH&=0X000FFFFF;//PA13 14 15����
	GPIOA->CRH|=0X88800000;
	GPIOA->ODR|=(1<<13)|(1<<14)|(1<<15); 
 
	NVIC_Configuration();  
    AFIO->EXTICR[2]|=0X0001; //EXTI8ӳ�䵽PB8
	EXTI->IMR|=1<<8;       //����line6�ϵ��ж�
	EXTI->EMR|=1<<8;       //������line6�ϵ��¼�
	EXTI->FTSR|=1<<8;      //line6���¼��½��ش���	
}

