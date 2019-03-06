//MP3 功放程序 红外部分
//QiYuan@SCUT
//2010/07/09
//红外接收函数
//改一改中断处理就直接从单片机移植过来了
#include "main.h"
u8 key_red=255; //红外接收到数据  
extern u8 State;

//中断优先级管理/开启	   
void NVIC_Configuration_RED(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	//存储器映射,不用理    
#ifdef  VECT_TAB_RAM  									   
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
#else   							 
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
#endif  				  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//优先级分到第0组 总共5组		 
  	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQChannel; //使用外部中断13
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//抢占优先级
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		 //响应优先级
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure); 								 
}

//外部中断初始化函数PC13
void exti_init(void)
{	
	//对GPIO操作一定要先使能对应时钟
	RCC->APB2ENR|=(1<<4)|(1<<0);    //PC时钟和AFIO时钟使能
	GPIOC->CRH&=0XFF0FFFFF;
	GPIOC->CRH|=0X00800000;//pC13上拉输入
	GPIOC->ODR|=(1<<13);      //上拉
	NVIC_Configuration_RED();  
    AFIO->EXTICR[3]|=0X0020; //EXTI13映射到PC13   
	EXTI->IMR|=1<<13;       //开启line13上的中断
	EXTI->EMR|=1<<13;       //不屏蔽line13上的事件
	EXTI->FTSR|=1<<13;      //line13上事件下降沿触发		   
} 


//红外接收初始化
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
        if(t==250)return t; //超时溢出
    }
    return t;
}      

/*-------------------------协议--------------------------
开始拉低9ms,接着是一个4.5ms的高脉冲,通知器件开始传送数据了
接着是26位前导脉冲,用于识别该型号的摇控。
然后是发送8位数据的原码，接着又发送8位数据的反码.
最后是一个长脉冲，和开始位一样，拉低9ms和一个4.5ms脉冲表示结束。
每个按键不同这处只在于那8位数据的原码和反码，因些只须要识别
8位原码就可以识别按键了
---------------------------------------------------------*/
void EXTI1_IRQHandler(void)//外部中断服务程序
{ 
    u8 res=0; u16 aa=0;           
    u8 OK=0; 
    u8 RODATA=0; 
    while(1)
    {        
        if(RDATA)//有高脉冲出现
        {
            res=check();//获得此次高脉冲宽度       
            if(res==250)break;//非有用信号
            if(res>=200&&res<250)OK=1; //获得前导位(4.5ms) 
            
            else if(res>=60&&res<90)  //如果是宽脉冲，则RODATA＝1
            {
                aa++;
                RODATA=1;//1.5ms                
            }
            else if(res>=10&&res<50)  //如果是窄脉冲，则RODATA＝0      
            {
                aa++;
                RODATA=0;//500us              
            }
            //舍去26位前导脉冲，从第27位开始接着的8位是数据的正码，再后8位是反码
            //获取正码
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

    EXTI->PR=1<<13; 	//清除中断标志位    
}  

//经测试：KD-29的全部解码如下（正码）
//静音：104   电源：72   数字1：128   数字2：64
//数字3：192   数字4：32   数字5：160  数字6：96
//数字7：224   数字8：16  数字9：144
//-/--:80    数字0：0    交替：88
//SLEEP:56   S.SYS:248   CALL:216
//P.P:152    TV/AV:208
//上：200    下：232    左：40     右：8
//MENU：24  SCAN：240   GAME：48
  
//把红外数据进行转换
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
        case 200:return_num = 10;break;//上
        case 232:return_num = 11;break;//下
        case 40 :return_num = 12;break;//左
        case 8  :return_num = 13;break;//右
        case 24 :return_num = 14;break;//MENU 
        case 104:return_num = 15;break;//静音  
        case 80 :return_num = 16;break;//-/--  
        case 208:return_num = 17;break;//TV/AV  
        case 240:return_num = 18;break;
    }
	return return_num;
}   

 


