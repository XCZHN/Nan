//MP3 ���ų��� ���ⲿ��
//QiYuan@SCUT
//2010/07/09
//������պ���
//��һ���жϴ����ֱ�Ӵӵ�Ƭ����ֲ������
#include "main.h"
u8 key_red=255; //������յ�����  
extern u8 State;

//�ж����ȼ�����/����	   
void NVIC_Configuration_RED(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	//�洢��ӳ��,������    
#ifdef  VECT_TAB_RAM  									   
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
#else   							 
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
#endif  				  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//���ȼ��ֵ���0�� �ܹ�5��		 
  	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQChannel; //ʹ���ⲿ�ж�13
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//��ռ���ȼ�
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		 //��Ӧ���ȼ�
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure); 								 
}

//�ⲿ�жϳ�ʼ������PC13
void exti_init(void)
{	
	//��GPIO����һ��Ҫ��ʹ�ܶ�Ӧʱ��
	RCC->APB2ENR|=(1<<4)|(1<<0);    //PCʱ�Ӻ�AFIOʱ��ʹ��
	GPIOC->CRH&=0XFF0FFFFF;
	GPIOC->CRH|=0X00800000;//pC13��������
	GPIOC->ODR|=(1<<13);      //����
	NVIC_Configuration_RED();  
    AFIO->EXTICR[3]|=0X0020; //EXTI13ӳ�䵽PC13   
	EXTI->IMR|=1<<13;       //����line13�ϵ��ж�
	EXTI->EMR|=1<<13;       //������line13�ϵ��¼�
	EXTI->FTSR|=1<<13;      //line13���¼��½��ش���		   
} 


//������ճ�ʼ��
void red_init(void)		
{
	exti_init();
}

u8 check(void)
{
    u8 t=0;
    while(RDATA)
    {
        t++;
        delay_us(20);
        if(t==250)return t; //��ʱ���
    }
    return t;
}      

/*-------------------------Э��--------------------------
��ʼ����9ms,������һ��4.5ms�ĸ�����,֪ͨ������ʼ����������
������26λǰ������,����ʶ����ͺŵ�ҡ�ء�
Ȼ���Ƿ���8λ���ݵ�ԭ�룬�����ַ���8λ���ݵķ���.
�����һ�������壬�Ϳ�ʼλһ��������9ms��һ��4.5ms�����ʾ������
ÿ��������ͬ�⴦ֻ������8λ���ݵ�ԭ��ͷ��룬��Щֻ��Ҫʶ��
8λԭ��Ϳ���ʶ�𰴼���
---------------------------------------------------------*/
void EXTI1_IRQHandler(void)//�ⲿ�жϷ������
{ 
    u8 res=0; u16 aa=0;           
    u8 OK=0; 
    u8 RODATA=0; 
    while(1)
    {        
        if(RDATA)//�и��������
        {
            res=check();//��ô˴θ�������       
            if(res==250)break;//�������ź�
            if(res>=200&&res<250)OK=1; //���ǰ��λ(4.5ms) 
            
            else if(res>=60&&res<90)  //����ǿ����壬��RODATA��1
            {
                aa++;
                RODATA=1;//1.5ms                
            }
            else if(res>=10&&res<50)  //�����խ���壬��RODATA��0      
            {
                aa++;
                RODATA=0;//500us              
            }
            //��ȥ26λǰ�����壬�ӵ�27λ��ʼ���ŵ�8λ�����ݵ����룬�ٺ�8λ�Ƿ���
            //��ȡ����
            if(OK&&(aa>=27)&&(aa<35))
            {
                key_red<<=1;
                key_red+=RODATA;
            } 
            
            if(aa==42) 
			{
                key_red=redchange(key_red);
			}
        }    
    }  

    EXTI->PR=1<<13; 	//����жϱ�־λ    
}  

//�����ԣ�KD-29��ȫ���������£����룩
//������104   ��Դ��72   ����1��128   ����2��64
//����3��192   ����4��32   ����5��160  ����6��96
//����7��224   ����8��16  ����9��144
//-/--:80    ����0��0    ���棺88
//SLEEP:56   S.SYS:248   CALL:216
//P.P:152    TV/AV:208
//�ϣ�200    �£�232    ��40     �ң�8
//MENU��24  SCAN��240   GAME��48
  
//�Ѻ������ݽ���ת��
u8 redchange(u8 rednum)
{
	u8 return_num=255;
    switch(rednum)
    {
        case 0  :return_num = 0;break;
        case 128:return_num = 1;break;
        case 64 :return_num = 2;break;
        case 192:return_num = 3;break;
        case 32 :return_num = 4;break;
        case 96 :return_num = 6;break;
        case 224:return_num = 7;break;
        case 16 :return_num = 8;break;
        case 144:return_num = 9;break;
        case 200:return_num = 10;break;//��
        case 232:return_num = 11;break;//��
        case 40 :return_num = 12;break;//��
        case 8  :return_num = 13;break;//��
        case 24 :return_num = 14;break;//MENU 
        case 104:return_num = 15;break;//����  
        case 80 :return_num = 16;break;//-/--  
        case 208:return_num = 17;break;//TV/AV  
        case 240:return_num = 18;break;
    }
	return return_num;
}   

 


