/**************************************************************/
/*                  ̰ʳ����Ϸ  V1.0                          */ 
//�ο�:�������ѺͲ��еĴ���
//���е�Դ��������,����ʹ��
//����ԭ��@scut  08/09/11
/**************************************************************/

#include "main.h"

//��Χ�޶�
#define MAX_GAME_X 23
#define MAX_GAME_Y 15  
//��󳤶�
#define MAX_LENTH 13 //�ߵ���󳤶� ����������126����
#define MAX_LEVEL_Snake 18 //����ٶȵȼ�

u16 timer4_count=0;
//��Ϸ������־
u8 GAME_OVER=0;  //��Ϸ������־
u8 PASS=0;       //��ͨ�صı�־
u8 pauseen=0;    //��ͣ 
u8 GAME_PLAY=0;//��Ϸģʽ    

//����       
u8 LEVELEPM_Snake; //���漶�� 
u8 LEVELEPM_Russia;
u16 MAXSCORE_Snake;  //��߷ּ�¼
u16 MAXSCORE_Russia;

u8 LEVEL=0;         //�ȼ��ݴ���   
u16 score=0;          //�����ݴ���

        
void Snake_msg(void);

//��¼��״̬�Ľṹ��
struct SNAKE
{ 
	unsigned char Direction;//�ƶ�����
#define TO_RIGHT RIGHT
#define TO_LEFT LEFT
#define TO_UP UP
#define TO_DOWN DOWN
#define PAUSE KEY_C
  
#define LEVAND   0X06
#define LEVSUB   0X07 
#define SAVELEV  0x08  
#define BACK     0X09
	unsigned int  Dot_Cnt;//������
	unsigned char Next_X;//Ŀ���
	unsigned char Next_Y;
} Snake_Data;//�����ṹ�����  

//�ж��Ƿ����������Լ���������
unsigned char Hit_Self(unsigned char x,unsigned char y)
{return ReadPixel(x,y);}//ԭ��    

//�����ʼ��,x y Ϊ��ʼ����,lengthΪ����
void Snake_Ini(unsigned char x,unsigned char y,unsigned char length)
{
	unsigned char i;
	unsigned int position;   
	GAME_OVER=0;//�������� 
	Snake_Data.Direction = TO_RIGHT;//Ĭ�Ϸ��� 
	Snake_Data.Dot_Cnt=length;//������  
	clearlcdram();//����            
	for(i=0;i<length;i++)Big_pinxl(x+i,y,1);//���length �������  
	while(1)//ȷ��Ŀ���
	{
	 	srand(timer4_count);//�õ�����	   
		position = rand()>>4;//���������     
		if(Hit_Self(position%24,position/128))continue;//��������е㴦�����²��� 
		Big_pinxl(position%24,position/128,1);//���
		Snake_Data.Next_X = position%24;//����Ŀ���
		Snake_Data.Next_Y = position/128;
		break;
	}
	lcd_refresh();//ˢ��
}  
   
         
//�����µ�����,����һ����
//ͬʱ����������峤�ȼ�һ
uchar seedget(void)
{    
    u16 position; 
    Snake_Data.Dot_Cnt+=1;//���1 
    score+=10; 
    if(Snake_Data.Dot_Cnt>MAX_LENTH)
    {
        if(LEVEL<MAX_LEVEL_Snake)
        {
            LEVEL++;//��������
        }
        else PASS=1; //��ͨ��  
        Snake_msg();    
        Snake_Ini(0,0,2); 
        delay_ms(1000);delay_ms(1000);  
        key=255;
        return 0;
    }
    Snake_msg();   
    while(1)//���¼�����Ŀ���
    {
		srand(timer4_count);//�õ�����
        position = rand()>>4;
		if(Hit_Self(position%24,position/128))continue;//ֱ���ú��ʵ����� 24*16����
		Big_pinxl(position%24,position/128,1);
		Snake_Data.Next_X = position%24;
		Snake_Data.Next_Y = position/128;
		break;
    }    
    Big_pinxl(Snake_Data.Next_X,Snake_Data.Next_Y,1);//��ʾ�µ�ʳ�� 
    delay_ms(100);//��ʱ,������  
    return 1;
}
//�������ݱ�����,��Ϊpbuffer��512���ֽ�
//����snake_body����ָ���������512-260����ô��
//uchar *snake_body=pbuffer+260;//ָ���ַpbuffer[260]
uchar *snake_body=MCUBuffer+260;//ָ���ַpbuffer[260]

//������:snake_run
//����:�����к���
//�������:һ��ȫ�ֱ���flag_snake���߸�����������ж��˶�����
//ע������:���ܶ�����,�����ж�·��,ʳ��,����,����
//ʹ�����:�ڲ�����
void snake_run(void)
{
	uchar tmp_head_x,tmp_head_y;
	uchar i; 
	tmp_head_x=snake_body[0];//ȡ����ͷX����
	tmp_head_y=snake_body[1];//ȡ����ͷY����  
	switch(Snake_Data.Direction) //ȡ��ͷ����
	{
		case TO_DOWN://������ y++ 
			if(tmp_head_y==0)
				GAME_OVER=1;//�������ײǽ��,����GAMEOVER��־����ͬ
			else tmp_head_y--;
			break;
		case TO_UP://������ y-- 
			if(tmp_head_y==MAX_GAME_Y)
				GAME_OVER=1;
			else tmp_head_y++; 
			break;
		case TO_LEFT://������ x-- 
			if(tmp_head_x==0)
				GAME_OVER=1;
			else tmp_head_x--;
			break;
		case TO_RIGHT://������ x++   
			if(tmp_head_x==MAX_GAME_X)
				GAME_OVER=1;
			else tmp_head_x++;
			break;
		default:break;
	}
	if(!GAME_OVER) //�����֮ǰû��ײǽ���Ϳ��Խ�����һ���ж�
	{ //�����ǵõ�ʳ����жϡ� 
	    //��ͷ��ʳ������û�ص��ʹ���û�гԵ�ʳ��
		if(Snake_Data.Next_X==tmp_head_x&&Snake_Data.Next_Y==tmp_head_y) 
		{ //�ò���ʳ��Ĵ���  
			snake_body[2*(Snake_Data.Dot_Cnt+1)]=snake_body[2*Snake_Data.Dot_Cnt]; //������β�ͣ��������������崫��)
			snake_body[2*(Snake_Data.Dot_Cnt+1)+1]=snake_body[2*Snake_Data.Dot_Cnt+1];
			for(i=Snake_Data.Dot_Cnt;i>0;i--) //���崫��  
			{
			    snake_body[2*i]=snake_body[2*(i-1)];
			    snake_body[2*i+1]=snake_body[2*i-1];
			}
			snake_body[0]=tmp_head_x;
		    snake_body[1]=tmp_head_y;//snake_flag&=~0x02;//��ʳ���־
			if(!seedget())return;//�õ�������,�����������,��ֱ�ӷ��� 
		}  
		else//�õ�ʳ��Ĵ��� 
 		{ 
			Big_pinxl(snake_body[2*Snake_Data.Dot_Cnt],snake_body[2*Snake_Data.Dot_Cnt+1],0);//����β�� 
			for(i=Snake_Data.Dot_Cnt;i>0;i--) //���崫��  
			{
			    snake_body[2*i]=snake_body[2*(i-1)];
			    snake_body[2*i+1]=snake_body[2*i-1];
			}
			snake_body[0]=tmp_head_x;
		    snake_body[1]=tmp_head_y;
		}
		Big_pinxl(tmp_head_x,tmp_head_y,1);//��ʾ����ͷ
	}
	for(i=1;i<Snake_Data.Dot_Cnt+1;i++) //�ж��Ƿ�ײ���Լ� 
	{
		if(snake_body[0]==snake_body[2*i]&&snake_body[1]==snake_body[2*i+1]) //ײ���˾���GAMEOVER��־ 
		{
			GAME_OVER=1;  
			break;
		}
	}
	lcd_refresh();//��ʾ  
}    
//��ʾ��Ϸʱ�ķ����͵ȼ���Ϣ 
void Snake_msg(void)
{                 
    Show_num2(1,109,LEVEL);  	
    Show_num2(3,103,score/100);
    Show_num2(3,115,score%100); 
}
//��ʾ��Ϸʱ�Ľ��� 
void Snake_GUI(void)
{    
    uchar i=0;     
	for(i=0;i<8;i++)
	{
	    Set_page(i);Set_column(96);//��ʾ�߽���
	    Write_data(0xff);
	}
    LCD_write_cstr(0,99,"LEVEL");
    if(pauseen)LCD_write_cstr(2,99,"PAUSE");
    else LCD_write_cstr(2,99,"SCORE");    
}                                 
//��Ϸ����ʱ�Ľ���
void Game_Over_Show(uchar gametype)
{ 
	Cleardisplay(4);//����
	LCD_write_cstr(0,24,"Max Score:"); //��ʾ��߷�  
    if(gametype==0)//snake game
    {    
        if(MAXSCORE_Snake<score)MAXSCORE_Snake=score;//�޸���߷�ֵ
        Show_num2(0,84,MAXSCORE_Snake/100);//snake game max score
        Show_num2(0,96,MAXSCORE_Snake%100);  
        if(score==MAXSCORE_Snake)LCD_write_cstr(1,36,"Well Done!");//��Ҵ�������߷� 
	    else LCD_write_cstr(1,36,"Game Over!");  //��Ϸ����
	}else //russia game
    {    
        if(MAXSCORE_Russia<score)MAXSCORE_Russia=score;
        Show_num2(0,84,MAXSCORE_Russia/100);//russia game max score
        Show_num2(0,96,MAXSCORE_Russia%100);  
        if(score==MAXSCORE_Russia)LCD_write_cstr(1,36,"Well Done!");//��Ҵ�������߷� 
	    else LCD_write_cstr(1,36,"Game Over!");  //��Ϸ����
	}          
	delay_ms(1000);
	LCD_write_cstr(2,20,"Your Score:"); //��ʾ���߷���
	Show_num2(2,86,score/100);
    Show_num2(2,98,score%100); 
	delay_ms(3000);	
	GAME_PLAY=0;//�ر���Ϸģʽ
}                 

//������Ϸ,��ѭ��.
void Play_Snake(void)
{       
    uint lev;
	Timer4_Init();
    if(LEVELEPM_Snake>MAX_LEVEL_Snake)LEVELEPM_Snake=8;//��ʼ���ȼ� 
    else LEVEL=LEVELEPM_Snake;
    if(MAXSCORE_Snake>9999)MAXSCORE_Snake=0;//��ʼ������   
    
    Snake_GUI(); 
    score=0;  //�������
    pauseen=0;//�ر���ͣ 
    GAME_PLAY=1;//��Ϸģʽ
    PASS=0;//ͨ�ر�־���
    Snake_msg(); 
	while(!GAME_OVER)
	{
		if(key!=255)
		{
			switch(key)//���ݰ�������ȷ���ƶ����� �������෴�����ƶ�
			{
				case TO_RIGHT :if(Snake_Data.Direction!=TO_LEFT)Snake_Data.Direction = TO_RIGHT;break;
				case TO_LEFT  :if(Snake_Data.Direction!=TO_RIGHT)Snake_Data.Direction = TO_LEFT;break;
				case TO_UP    :if(Snake_Data.Direction!=TO_DOWN)Snake_Data.Direction = TO_UP;break;
				case TO_DOWN  :if(Snake_Data.Direction!=TO_UP)Snake_Data.Direction = TO_DOWN;break;
				case PAUSE://��ͣ
				{
				    pauseen=!pauseen;  
				    Snake_GUI();//�ı�gui
				    break;
				}
				case KEY_D://ˮƽ����
				{
				    if(LEVEL)LEVEL--;
				    else LEVEL=0;
				    Snake_msg();//������ʾ 
				    break;
				}
				/*case LEVSUB://ˮƽ����
				{
				    if(LEVEL)LEVEL--;
				    else LEVEL=0;
				    Snake_msg();//������ʾ 
				    break;
				}
				case LEVAND://ˮƽ����
				{
				    if(LEVEL<MAX_LEVEL_Snake)LEVEL++;
				    else LEVEL=MAX_LEVEL_Snake; 
				    Snake_msg();//������ʾ
				    break;
				}  
				case SAVELEV://����LEVEL��Ϣ
				{               
				    Cleardisplay(4);//����
				    LEVELEPM_Snake=LEVEL;//����ȼ���Ϣ
				    save_show();//��ʾ����ʱ�Ľ���
				    Snake_GUI();   
				    Snake_msg(); //�ָ���ʾ
				    lcd_refresh();//ˢ�� 
				    READY=0;//���������־ 
				    break; 
				}	*/
				case KEY_B:{GAME_PLAY=0;State=1;return ;}//�������˳�
				default:break;
			}
			key=255;		
		}
 
		if(PASS)//��ͨ����!!!
		{
		    Cleardisplay(4);//���� 
		    if(MAXSCORE_Snake<score)MAXSCORE_Snake=score;//�޸���߷�ֵ
	        LCD_write_cstr(0,24,"Max Score:"); //��ʾ��߷�
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
		    GAME_PLAY=0;//��Ϸ�����
			State=1;
		    return ;
		}
		if(!pauseen)snake_run();//���ײ�����ܺ��Լ���gameover��
		else Snake_GUI();//������Ϣ 
		lev=LEVEL;
		lev=(MAX_LEVEL_Snake-lev)*20+40; 
		delay_ms(lev);//������ʱ����
	}
	Game_Over_Show(0);//snake game  
} 



//TIMER1�ж����ȼ��趨					  	   
void NVIC_TIMER4Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	//�洢��ӳ��,������    
#ifdef  VECT_TAB_RAM  									   
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
#else   							 
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);		 
	//��0��,û���������ȼ�								  
	//��4��,û����Ӧ���ȼ�				  
	//��2��,��4������,4����Ӧ
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//���ȼ��ֵ���2�� �ܹ�5��		 
  	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQChannel; 
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//�׼�1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		 //�ײ�0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure); 
	//����TIM4���ж� 												  		 		   	 
  	NVIC_Init(&NVIC_InitStructure);        
#endif  				  					   							 
}   

//�����ж�,��ӳ�� 
void Timer4_Init(void)
{	
	NVIC_TIMER4Configuration();	
	RCC->APB1ENR|=1<<2;//TIM4ʱ��ʹ��    
	TIM4->PSC=17;		//Ԥ��Ƶ��18,�õ�4Mhz�ļ���ʱ��
	TIM4->ARR=100;		//�Զ���װ
	TIM4->CNT=100;		//40Khz ADC������										
	TIM4->DIER|=1<<0;   //��������ж�	
	TIM4->DIER|=1<<6;   //�������ж�
	TIM4->SR|=(1<<0);		  //����жϱ�־
	TIM4->CR1=1<<7;  //ARPEʹ�� 
	TIM4->CR1|=1<<0;   //ʹ�ܶ�ʱ��4				 					  										  
} 
//��ʱ��2�жϷ�����
void TIM4_IRQHandler(void)
{
	if(TIM4->SR&(1<<0))	  		//�ж��Ƿ�����ж�
	{
		timer4_count++;
		if(timer4_count==65535)timer4_count=0;
	}
	TIM4->SR&=~(1<<0);		    //����жϱ�־	
}





