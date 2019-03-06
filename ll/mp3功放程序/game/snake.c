/**************************************************************/
/*                  贪食蛇游戏  V1.0                          */ 
//参考:无名网友和波仔的代码
//波仔的源代码有误,不能使用
//正点原子@scut  08/09/11
/**************************************************************/

#include "main.h"

//范围限定
#define MAX_GAME_X 23
#define MAX_GAME_Y 15  
//最大长度
#define MAX_LENTH 13 //蛇的最大长度 理论上允许到126个点
#define MAX_LEVEL_Snake 18 //最高速度等级

u16 timer4_count=0;
//游戏结束标志
u8 GAME_OVER=0;  //游戏结束标志
u8 PASS=0;       //过通关的标志
u8 pauseen=0;    //暂停 
u8 GAME_PLAY=0;//游戏模式    

//级别       
u8 LEVELEPM_Snake; //保存级别 
u8 LEVELEPM_Russia;
u16 MAXSCORE_Snake;  //最高分记录
u16 MAXSCORE_Russia;

u8 LEVEL=0;         //等级暂存器   
u16 score=0;          //分数暂存器

        
void Snake_msg(void);

//记录蛇状态的结构体
struct SNAKE
{ 
	unsigned char Direction;//移动方向
#define TO_RIGHT RIGHT
#define TO_LEFT LEFT
#define TO_UP UP
#define TO_DOWN DOWN
#define PAUSE KEY_C
  
#define LEVAND   0X06
#define LEVSUB   0X07 
#define SAVELEV  0x08  
#define BACK     0X09
	unsigned int  Dot_Cnt;//蛇身长度
	unsigned char Next_X;//目标点
	unsigned char Next_Y;
} Snake_Data;//声明结构体变量  

//判断是否碰到了蛇自己的身体了
unsigned char Hit_Self(unsigned char x,unsigned char y)
{return ReadPixel(x,y);}//原形    

//蛇身初始化,x y 为起始坐标,length为长度
void Snake_Ini(unsigned char x,unsigned char y,unsigned char length)
{
	unsigned char i;
	unsigned int position;   
	GAME_OVER=0;//清结束标记 
	Snake_Data.Direction = TO_RIGHT;//默认方向 
	Snake_Data.Dot_Cnt=length;//蛇身长度  
	clearlcdram();//清屏            
	for(i=0;i<length;i++)Big_pinxl(x+i,y,1);//填充length 点个数据  
	while(1)//确定目标点
	{
	 	srand(timer4_count);//得到种子	   
		position = rand()>>4;//产生随机数     
		if(Hit_Self(position%24,position/128))continue;//如果落在有点处则重新产生 
		Big_pinxl(position%24,position/128,1);//填充
		Snake_Data.Next_X = position%24;//设置目标点
		Snake_Data.Next_Y = position/128;
		break;
	}
	lcd_refresh();//刷新
}  
   
         
//产生新的种子,即下一个点
//同时对蛇身的身体长度加一
uchar seedget(void)
{    
    u16 position; 
    Snake_Data.Dot_Cnt+=1;//点加1 
    score+=10; 
    if(Snake_Data.Dot_Cnt>MAX_LENTH)
    {
        if(LEVEL<MAX_LEVEL_Snake)
        {
            LEVEL++;//级别增加
        }
        else PASS=1; //过通关  
        Snake_msg();    
        Snake_Ini(0,0,2); 
        delay_ms(1000);delay_ms(1000);  
        key=255;
        return 0;
    }
    Snake_msg();   
    while(1)//重新计算新目标点
    {
		srand(timer4_count);//得到种子
        position = rand()>>4;
		if(Hit_Self(position%24,position/128))continue;//直到得合适的坐标 24*16以内
		Big_pinxl(position%24,position/128,1);
		Snake_Data.Next_X = position%24;
		Snake_Data.Next_Y = position/128;
		break;
    }    
    Big_pinxl(Snake_Data.Next_X,Snake_Data.Next_Y,1);//显示新的食物 
    delay_ms(100);//延时,避免死  
    return 1;
}
//蛇身数据保存曲,因为pbuffer有512个字节
//所以snake_body可以指向的数据有512-260个这么多
//uchar *snake_body=pbuffer+260;//指向地址pbuffer[260]
uchar *snake_body=MCUBuffer+260;//指向地址pbuffer[260]

//函数名:snake_run
//功能:蛇运行函数
//输入参数:一个全局变量flag_snake，蛇根据这个变量判断运动方向
//注意事项:蛇跑动函数,用于判断路径,食物,长大,死亡
//使用情况:内部调用
void snake_run(void)
{
	uchar tmp_head_x,tmp_head_y;
	uchar i; 
	tmp_head_x=snake_body[0];//取得蛇头X坐标
	tmp_head_y=snake_body[1];//取得蛇头Y坐标  
	switch(Snake_Data.Direction) //取蛇头方向
	{
		case TO_DOWN://向下走 y++ 
			if(tmp_head_y==0)
				GAME_OVER=1;//这个代表撞墙了,就置GAMEOVER标志，下同
			else tmp_head_y--;
			break;
		case TO_UP://向上走 y-- 
			if(tmp_head_y==MAX_GAME_Y)
				GAME_OVER=1;
			else tmp_head_y++; 
			break;
		case TO_LEFT://向左走 x-- 
			if(tmp_head_x==0)
				GAME_OVER=1;
			else tmp_head_x--;
			break;
		case TO_RIGHT://向右走 x++   
			if(tmp_head_x==MAX_GAME_X)
				GAME_OVER=1;
			else tmp_head_x++;
			break;
		default:break;
	}
	if(!GAME_OVER) //如果在之前没有撞墙，就可以进行下一步判断
	{ //以下是得到食物的判断。 
	    //蛇头和食物坐标没重叠就代表没有吃到食物
		if(Snake_Data.Next_X==tmp_head_x&&Snake_Data.Next_Y==tmp_head_y) 
		{ //得不到食物的处理  
			snake_body[2*(Snake_Data.Dot_Cnt+1)]=snake_body[2*Snake_Data.Dot_Cnt]; //保留蛇尾巴（这是增长型柔体传动)
			snake_body[2*(Snake_Data.Dot_Cnt+1)+1]=snake_body[2*Snake_Data.Dot_Cnt+1];
			for(i=Snake_Data.Dot_Cnt;i>0;i--) //柔体传动  
			{
			    snake_body[2*i]=snake_body[2*(i-1)];
			    snake_body[2*i+1]=snake_body[2*i-1];
			}
			snake_body[0]=tmp_head_x;
		    snake_body[1]=tmp_head_y;//snake_flag&=~0x02;//清食物标志
			if(!seedget())return;//得到新种子,如果换级别了,就直接返回 
		}  
		else//得到食物的处理 
 		{ 
			Big_pinxl(snake_body[2*Snake_Data.Dot_Cnt],snake_body[2*Snake_Data.Dot_Cnt+1],0);//灭蛇尾巴 
			for(i=Snake_Data.Dot_Cnt;i>0;i--) //柔体传动  
			{
			    snake_body[2*i]=snake_body[2*(i-1)];
			    snake_body[2*i+1]=snake_body[2*i-1];
			}
			snake_body[0]=tmp_head_x;
		    snake_body[1]=tmp_head_y;
		}
		Big_pinxl(tmp_head_x,tmp_head_y,1);//显示新蛇头
	}
	for(i=1;i<Snake_Data.Dot_Cnt+1;i++) //判断是否撞中自己 
	{
		if(snake_body[0]==snake_body[2*i]&&snake_body[1]==snake_body[2*i+1]) //撞中了就置GAMEOVER标志 
		{
			GAME_OVER=1;  
			break;
		}
	}
	lcd_refresh();//显示  
}    
//显示游戏时的分数和等级信息 
void Snake_msg(void)
{                 
    Show_num2(1,109,LEVEL);  	
    Show_num2(3,103,score/100);
    Show_num2(3,115,score%100); 
}
//显示游戏时的界面 
void Snake_GUI(void)
{    
    uchar i=0;     
	for(i=0;i<8;i++)
	{
	    Set_page(i);Set_column(96);//显示边界线
	    Write_data(0xff);
	}
    LCD_write_cstr(0,99,"LEVEL");
    if(pauseen)LCD_write_cstr(2,99,"PAUSE");
    else LCD_write_cstr(2,99,"SCORE");    
}                                 
//游戏结束时的界面
void Game_Over_Show(uchar gametype)
{ 
	Cleardisplay(4);//清屏
	LCD_write_cstr(0,24,"Max Score:"); //显示最高分  
    if(gametype==0)//snake game
    {    
        if(MAXSCORE_Snake<score)MAXSCORE_Snake=score;//修改最高分值
        Show_num2(0,84,MAXSCORE_Snake/100);//snake game max score
        Show_num2(0,96,MAXSCORE_Snake%100);  
        if(score==MAXSCORE_Snake)LCD_write_cstr(1,36,"Well Done!");//玩家创造了最高分 
	    else LCD_write_cstr(1,36,"Game Over!");  //游戏结束
	}else //russia game
    {    
        if(MAXSCORE_Russia<score)MAXSCORE_Russia=score;
        Show_num2(0,84,MAXSCORE_Russia/100);//russia game max score
        Show_num2(0,96,MAXSCORE_Russia%100);  
        if(score==MAXSCORE_Russia)LCD_write_cstr(1,36,"Well Done!");//玩家创造了最高分 
	    else LCD_write_cstr(1,36,"Game Over!");  //游戏结束
	}          
	delay_ms(1000);
	LCD_write_cstr(2,20,"Your Score:"); //显示玩者分数
	Show_num2(2,86,score/100);
    Show_num2(2,98,score%100); 
	delay_ms(3000);	
	GAME_PLAY=0;//关闭游戏模式
}                 

//进行游戏,主循环.
void Play_Snake(void)
{       
    uint lev;
	Timer4_Init();
    if(LEVELEPM_Snake>MAX_LEVEL_Snake)LEVELEPM_Snake=8;//初始化等级 
    else LEVEL=LEVELEPM_Snake;
    if(MAXSCORE_Snake>9999)MAXSCORE_Snake=0;//初始化分数   
    
    Snake_GUI(); 
    score=0;  //分数清空
    pauseen=0;//关闭暂停 
    GAME_PLAY=1;//游戏模式
    PASS=0;//通关标志清空
    Snake_msg(); 
	while(!GAME_OVER)
	{
		if(key!=255)
		{
			switch(key)//根据按键方向确定移动方向 不可向相反方向移动
			{
				case TO_RIGHT :if(Snake_Data.Direction!=TO_LEFT)Snake_Data.Direction = TO_RIGHT;break;
				case TO_LEFT  :if(Snake_Data.Direction!=TO_RIGHT)Snake_Data.Direction = TO_LEFT;break;
				case TO_UP    :if(Snake_Data.Direction!=TO_DOWN)Snake_Data.Direction = TO_UP;break;
				case TO_DOWN  :if(Snake_Data.Direction!=TO_UP)Snake_Data.Direction = TO_DOWN;break;
				case PAUSE://暂停
				{
				    pauseen=!pauseen;  
				    Snake_GUI();//改变gui
				    break;
				}
				case KEY_D://水平增加
				{
				    if(LEVEL)LEVEL--;
				    else LEVEL=0;
				    Snake_msg();//更新显示 
				    break;
				}
				/*case LEVSUB://水平增加
				{
				    if(LEVEL)LEVEL--;
				    else LEVEL=0;
				    Snake_msg();//更新显示 
				    break;
				}
				case LEVAND://水平减少
				{
				    if(LEVEL<MAX_LEVEL_Snake)LEVEL++;
				    else LEVEL=MAX_LEVEL_Snake; 
				    Snake_msg();//更新显示
				    break;
				}  
				case SAVELEV://保存LEVEL信息
				{               
				    Cleardisplay(4);//清屏
				    LEVELEPM_Snake=LEVEL;//保存等级信息
				    save_show();//显示保存时的界面
				    Snake_GUI();   
				    Snake_msg(); //恢复显示
				    lcd_refresh();//刷新 
				    READY=0;//清除按键标志 
				    break; 
				}	*/
				case KEY_B:{GAME_PLAY=0;State=1;return ;}//无条件退出
				default:break;
			}
			key=255;		
		}
 
		if(PASS)//过通关了!!!
		{
		    Cleardisplay(4);//清屏 
		    if(MAXSCORE_Snake<score)MAXSCORE_Snake=score;//修改最高分值
	        LCD_write_cstr(0,24,"Max Score:"); //显示最高分
	        Show_num2(0,84,MAXSCORE_Snake/100);
            Show_num2(0,96,MAXSCORE_Snake%100); 
            
		    LCD_write_cstr(1,18,"Well Done! Dude.");
		    delay_ms(2000);
		    LCD_write_cstr(2,5,"Congratulations!!!"); 
		    delay_ms(2000);
		    LCD_write_cstr(3,20,"Your Score:"); 
		    Show_num2(3,86,score/100);
            Show_num2(3,98,score%100); 
		    delay_ms(4000);
		    GAME_PLAY=0;//游戏玩结束
			State=1;
		    return ;
		}
		if(!pauseen)snake_run();//如果撞倒四周和自己就gameover了
		else Snake_GUI();//更新信息 
		lev=LEVEL;
		lev=(MAX_LEVEL_Snake-lev)*20+40; 
		delay_ms(lev);//级别延时控制
	}
	Game_Over_Show(0);//snake game  
} 



//TIMER1中断优先级设定					  	   
void NVIC_TIMER4Configuration(void)
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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//优先级分到第2组 总共5组		 
  	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQChannel; 
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//阶级1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		 //阶层0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure); 
	//设置TIM4的中断 												  		 		   	 
  	NVIC_Init(&NVIC_InitStructure);        
#endif  				  					   							 
}   

//开启中断,并映射 
void Timer4_Init(void)
{	
	NVIC_TIMER4Configuration();	
	RCC->APB1ENR|=1<<2;//TIM4时钟使能    
	TIM4->PSC=17;		//预分频器18,得到4Mhz的计数时钟
	TIM4->ARR=100;		//自动重装
	TIM4->CNT=100;		//40Khz ADC采样率										
	TIM4->DIER|=1<<0;   //允许更新中断	
	TIM4->DIER|=1<<6;   //允许触发中断
	TIM4->SR|=(1<<0);		  //清楚中断标志
	TIM4->CR1=1<<7;  //ARPE使能 
	TIM4->CR1|=1<<0;   //使能定时器4				 					  										  
} 
//定时器2中断服务函数
void TIM4_IRQHandler(void)
{
	if(TIM4->SR&(1<<0))	  		//判断是否更新中断
	{
		timer4_count++;
		if(timer4_count==65535)timer4_count=0;
	}
	TIM4->SR&=~(1<<0);		    //清楚中断标志	
}





