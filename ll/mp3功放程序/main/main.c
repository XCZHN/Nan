//MP3 ���ų���
//QiYuan@SCUT
//2010/07/09
#include "main.h"

u8 State=0;			//״̬����
u8 extern key;
u8 key_point=0;		//����ָ��
//ϵͳ��ʼ��
void Sys_Init()
{
  	Stm32_Clock_Init(9);
	delay_init(72);
	LCD_init();     //��ʼ��LCD13164
	Cleardisplay(ALL);   //����
	Vs1003_Init();						//��ʼ��VS1003��IO��
	while(FAT_Init());					//FAT��ʼ��������SD����ʼ��	
	SysInfoGet(1);						//��ȡϵͳ�ļ���Ϣ
	delay_ms(1000);
	keyboard_init();	 
}

//������
int main()
{	
	Sys_Init();  	        
	while (1)
    {
		switch(State)
		{
			case 1:	//���˵�
				Load_Sys_ICO();
		 		while(State==1)
				{
					if(key==LEFT||key==RIGHT)
					{
						Load_Sys_ICO();
						key=255;
					} 	
				}
				break;
			case 2://���ֲ�����
				Mus_Viewer();
				break;
			case 3://������
                break;
			case 4://��ʾʱ��
			   	Cleardisplay(ALL);   //����
				while(State==4)
				{
					LCD_write_String(1,0,"Date: 2010/07/09",0);//�޸��/�����ʾ��ʹ��
					LCD_write_String(0,0,"Time: 23:59:30",0);//�޸��/�����ʾ��ʹ��
					LCD_write_String(3,0,"Happy every day~~",0);//�޸��/�����ʾ��ʹ��	
				}
				break;
			case 5://FFT
				Show_FFT();
				break;
			case 6://̰ʳ��
			    Snake_Ini(0,0,2);
                Play_Snake();//��Ϸ 
				TIM4->CR1&=~(1<<0);   //ʧ�ܶ�ʱ��3  
				break;
			case 8://����˹����
	            fk_init();
	            Play_Russia();
				TIM4->CR1&=~(1<<0);   //ʧ�ܶ�ʱ��3  
				break;
			
		}		
    }     
}

