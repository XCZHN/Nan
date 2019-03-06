//MP3 功放程序
//QiYuan@SCUT
//2010/07/09
#include "main.h"

u8 State=0;			//状态变量
u8 extern key;
u8 key_point=0;		//键盘指针
//系统初始化
void Sys_Init()
{
  	Stm32_Clock_Init(9);
	delay_init(72);
	LCD_init();     //初始化LCD13164
	Cleardisplay(ALL);   //清屏
	Vs1003_Init();						//初始化VS1003的IO口
	while(FAT_Init());					//FAT初始化，包括SD卡初始化	
	SysInfoGet(1);						//获取系统文件信息
	delay_ms(1000);
	keyboard_init();	 
}

//主函数
int main()
{	
	Sys_Init();  	        
	while (1)
    {
		switch(State)
		{
			case 1:	//主菜单
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
			case 2://音乐播放器
				Mus_Viewer();
				break;
			case 3://电子书
                break;
			case 4://显示时间
			   	Cleardisplay(ALL);   //清屏
				while(State==4)
				{
					LCD_write_String(1,0,"Date: 2010/07/09",0);//无歌词/歌词显示不使能
					LCD_write_String(0,0,"Time: 23:59:30",0);//无歌词/歌词显示不使能
					LCD_write_String(3,0,"Happy every day~~",0);//无歌词/歌词显示不使能	
				}
				break;
			case 5://FFT
				Show_FFT();
				break;
			case 6://贪食蛇
			    Snake_Ini(0,0,2);
                Play_Snake();//游戏 
				TIM4->CR1&=~(1<<0);   //失能定时器3  
				break;
			case 8://俄罗斯方块
	            fk_init();
	            Play_Russia();
				TIM4->CR1&=~(1<<0);   //失能定时器3  
				break;
			
		}		
    }     
}

