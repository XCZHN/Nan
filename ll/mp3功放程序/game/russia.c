#include "main.h"  
//正点原子@scut 08/09/12
//实现功能:玩俄罗斯方块,面积 12*16
//参考代码:徐文军俄罗斯方块程序

extern u16 timer4_count;

extern u8 GAME_OVER;  //游戏结束标志
extern u8 PASS;       //过通关的标志
extern u8 pauseen;    //暂停 
extern u8 GAME_PLAY;//游戏模式    

//级别       
extern u8 LEVELEPM_Snake; //保存级别 
extern u8 LEVELEPM_Russia;
extern u16 MAXSCORE_Snake;  //最高分记录
extern u16 MAXSCORE_Russia;

extern struct SNAKE	
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

extern u8 LEVEL;         //等级暂存器   
extern u16 score;          //分数暂存器


void fk_reffk(void);				//刷新方块
void fk_refnew(void);				//刷新预览方块
void fk_refline(uchar yy);			//刷新1行背景
uchar fk_chk(void);					//冲突检查
void fk_new(void);					//产生新方块
void fk_add(void);					//方块合并
void Play_Russia(void);  
void Russia_msg(void); 

#define MAX_LEVEL_Russia 8 //最高级别  计算公司 延时 :90-LEVEL*10;最大延时90*5ms,最小10*5ms
//我的液晶用 12*16大小(每点大小为4*4)的空间来做运动空间 
#define FULLMAP	0x0fff				/*掩码*/
#define LINEGUAN	15				/*20行过一关*/
#define NEWX	13					/*预览方块X位置*/
#define NEWY	12					/*预览方块Y位置*/
#define LINEMAX	15					/*屏幕最高15行*/
 
u8 fk_run=0;//俄罗斯方块开始标志     

//俄罗斯方块数据结构体
struct Russia
{
    u16 fk_map[LINEMAX+4];		        //背景映象
    u8  fk_x,fk_y,fk_r;				//方块左右、高度、方向
    u8 fk_type;						//方块形状
    u8 fk_oldx,fk_oldy,fk_oldr;		//方块上次左右、高度、方向
    u8 fk_newtype,fk_newr;			//新方块形状、方向  
}Russia_Data;  

//模块,总共七种模块
const u8 fk_mod[7][4][4]={			//方块模型号,4个方向,4行
    7,2,0,0,1,3,1,0,2,7,0,0,2,3,2,0,    //_|_
    2,3,1,0,3,6,0,0,2,3,1,0,3,6,0,0,    //_|~
    1,3,2,0,6,3,0,0,1,3,2,0,6,3,0,0,    //~|_
    1,1,3,0,4,7,0,0,3,2,2,0,7,1,0,0,    //|__
    3,1,1,0,1,7,0,0,2,2,3,0,7,4,0,0,    //__|
    1,1,1,1,15,0,0,0,1,1,1,1,15,0,0,0,  //____
    3,3,0,0,3,3,0,0,3,3,0,0,3,3,0,0     //田字
};      
//刷新方块
//这个程序很耗时间
void fk_reffk(void)					 
{
	u8 i,j;
	u8 temp; 
	for (i=0;i<4;i++)//清除原来的方块
	{
		temp=(fk_mod[Russia_Data.fk_type][Russia_Data.fk_oldr][i]);
		for (j=Russia_Data.fk_oldx;j<Russia_Data.fk_oldx+4;j++)
		{
			if(temp&0x01)Big_pinxl(j,Russia_Data.fk_oldy+i,0);//清除一个4*4的点 
			temp>>=1;
		}
	} 
	for (i=0;i<4;i++)//显示新的方块
	{
		temp=(fk_mod[Russia_Data.fk_type][Russia_Data.fk_r][i]);
		for (j=Russia_Data.fk_x;j<Russia_Data.fk_x+4;j++)
		{
			if(temp&0x01)Big_pinxl(j,Russia_Data.fk_y+i,1);//填充一个4*4的点 
			temp>>=1;
		}
	}
	Russia_Data.fk_oldx=Russia_Data.fk_x;Russia_Data.fk_oldy=Russia_Data.fk_y;Russia_Data.fk_oldr=Russia_Data.fk_r;	//保存新方块位置 
	lcd_refresh();//刷新lcd显存
} 
 






//刷新预览方块
void fk_refnew(void)				
{
	u8 i,j;
	u8 temp;
	//预览方块
	for (i=0;i<4;i++)
	{
		temp=fk_mod[Russia_Data.fk_newtype][Russia_Data.fk_newr][i];
		for (j=NEWX;j<NEWX+4;j++)
		{ 
		    Big_pinxl(j,NEWY+i,temp&0x01); 
			temp>>=1;
		}
	}  
	lcd_refresh();//刷新lcd显存
} 
 
//刷新1行背景
void fk_refline(uchar yy)					
{
	u8 i;
	u16 temp; 
	temp=Russia_Data.fk_map[yy];
	for (i=0;i<12;i++)
	{ 
	    Big_pinxl(i,yy,temp&0x01); 
	    temp >>= 1;
	} 
	lcd_refresh();
}  
//冲突检查        
//check ok
uchar fk_chk(void)					
{
	u8 i;
	u8 neq=0; 
	for (i=0;i<4;i++)
	{    
		if (((((uint)fk_mod[Russia_Data.fk_type][Russia_Data.fk_r][i])<<Russia_Data.fk_x)+(Russia_Data.fk_map[Russia_Data.fk_y+i]))!=((((uint)fk_mod[Russia_Data.fk_type][Russia_Data.fk_r][i])<<Russia_Data.fk_x)|(Russia_Data.fk_map[Russia_Data.fk_y+i])))
		neq=1;  
	}
	return(neq);
} 	  
//check ok 
//产生新方块
void fk_new(void)						
{       
    srand(timer4_count);//得到种子
	srand(rand()+Russia_Data.fk_x+Russia_Data.fk_y+Russia_Data.fk_r);
	Russia_Data.fk_oldx=Russia_Data.fk_x=5;        //新方块的坐标
	Russia_Data.fk_oldy=Russia_Data.fk_y=LINEMAX; 
	
	Russia_Data.fk_type=Russia_Data.fk_newtype;//把上次的信息赋给这次要显示的
	Russia_Data.fk_oldr=Russia_Data.fk_newr; 	
	Russia_Data.fk_r=Russia_Data.fk_oldr; 
    Russia_Data.fk_newtype = rand()%7;
	Russia_Data.fk_newr=rand()%4;  
	fk_refnew();//刷新预览方块  
}         
//方块合并
//check ok
void fk_add(void)					
{
	uchar i,j;
	uchar full=0x00;
	uchar fulltemp;
	uchar fullline=0x00;   
	for (i=0;i<4;i++)//方块合并
	{
		Russia_Data.fk_map[Russia_Data.fk_y+i]|=(uint)(fk_mod[Russia_Data.fk_type][Russia_Data.fk_r][i])<<Russia_Data.fk_x;//把map里面的每一行的相应位同模型比较
		full <<= 1;
		if ((Russia_Data.fk_y+i>=0)&&(Russia_Data.fk_map[Russia_Data.fk_y+i]==0xffff))full|=0x01;//统计满行的行数 
	}
	if (full!=0)//有满行
	{
		for (j=0;j<3;j++)		//消行闪烁3次
		{ 
			fulltemp=full;
			for (i=0;i<4;i++) //4行
			{
				if ((fulltemp&0x08)!=0)//清除所有满行
				{
					Russia_Data.fk_map[Russia_Data.fk_y+i]^=FULLMAP;
				}
				fk_refline(Russia_Data.fk_y+i);//刷新一行的显示数据(总共四次,也就是一个图形的最大长度)
				fulltemp<<=1;
			}
			delay_ms(200);
		}
		fulltemp=full;
		for (i=Russia_Data.fk_y;i<LINEMAX+4+fullline;i++)//Russia_Data.fk_y 以上的所有的行下移
		{
			if ((i<LINEMAX+4)&&(Russia_Data.fk_map[i-fullline]!=Russia_Data.fk_map[i]))
			{
				Russia_Data.fk_map[i-fullline]=Russia_Data.fk_map[i];
				fk_refline(i-fullline);
			}
			if ((i>=LINEMAX+4)&&(Russia_Data.fk_map[i-fullline] != ~FULLMAP))
			{
				Russia_Data.fk_map[i-fullline]=~FULLMAP;//背景映象 清空
				fk_refline(i-fullline);
			}
			if ((fulltemp&0x08)!=0)fullline++;//满行标志,统计满行的行数 
			fulltemp <<= 1;
		} 
		fulltemp=1;//分数统计,消一行得2分,两行得四分,三行得八分,四行得十六分
		while(fullline)
		{
		    fulltemp*=2;
		    fullline--;
		} 
		score+=fulltemp; 
		Russia_msg();       //显示分数
		fk_new();			//产生新方块
	}
	else if (Russia_Data.fk_y==LINEMAX)fk_run=0;//在最高位置碰撞且不能消行则游戏结束 
	else fk_new(); 
}  
//方块初始化 
void fk_init(void)					
{
    uchar i;
    Cleardisplay(4);//LCD清屏 
    clearlcdram();//清空sram  
    Russia_Data.fk_map[0]=0xffff;            //背景映象   
    for (i=0;i<LINEMAX+4;i++)
    {
        Russia_Data.fk_map[i]=~FULLMAP;          //背景映象
        fk_refline(i);               //刷新1行背景
    } 
    fk_run = 1; 
    fk_new();                        //产生新方块
}             
//俄罗斯方块显示分数等级信息
void Russia_msg(void)
{                 
    Show_num(1,109,LEVEL);  
    Show_num(3,103,score/100);
    Show_num(3,115,score%100); 
}     
void Russia_GUI(void)
{ 
    uchar i=0;     
	for(i=0;i<64;i++)WritePixel(48,i,1);//第一条虚线,写入显存
	for(i=0;i<64;i++)WritePixel(95,i,1);//第二调虚线,写入缓存
	for(i=0;i<47;i++)WritePixel(i+48,44,1);//第三调虚线,写入缓存 
    LCD_write_cstr(0,99,"LEVEL");
    if(pauseen)LCD_write_cstr(2,99,"PAUSE");
    else LCD_write_cstr(2,99,"SCORE"); 
    Russia_msg();  
}
//俄罗斯方块主程序
//玩游戏
void Play_Russia(void)						
{ 
	uchar timecount=0;//计时器  
	uchar overtime=0;
	Timer4_Init();
    if(LEVELEPM_Russia>MAX_LEVEL_Russia)LEVELEPM_Russia=4;//初始化等级 
    else LEVEL=LEVELEPM_Russia; //读取eeprom数据
    if(MAXSCORE_Russia>9999)MAXSCORE_Russia=0;//初始化分数   	 
	overtime=90-LEVEL*10;//最快10ms延时
	
	score=0;  //分数清空
	pauseen=0;//关闭暂停 
	Russia_GUI(); 
	GAME_PLAY=1;//开始游戏
	fk_run=1;  
	while(fk_run)//死循环,一直玩到死,或者用户退出
	{ 
	    if(key!=255)
	    { 
	        timecount=0;//计时器清空 
	       	switch(key)
			{ 
				case TO_DOWN:		//下移 
				{
		 			if (fk_run)	//游戏中下移
					{
						if((Russia_Data.fk_y>0)&&!(fk_chk()))Russia_Data.fk_y--;//下一一个
						if(fk_chk()){Russia_Data.fk_y++;break;}//冲突取消,直接退出
						if((Russia_Data.fk_y>0)&&!(fk_chk()))Russia_Data.fk_y--;//下一一个
						if(fk_chk())Russia_Data.fk_y++;//冲突取消
						break;
					}
					else fk_new();				//产生新方块 
					break;  
				} 
				case TO_RIGHT:		//右移  
				{  
					if (fk_run)					//游戏中右移
					{
						if(Russia_Data.fk_x<12)
						{
							Russia_Data.fk_x++;
							if(fk_chk())Russia_Data.fk_x--;//有冲突取消操作
						}
					}     
					break;   
				}
				case TO_LEFT:		//左移
				{
					if (fk_run)						//游戏中左移
					{
						if(Russia_Data.fk_x>0)
						{
							Russia_Data.fk_x--;
							if(fk_chk())Russia_Data.fk_x++;//有冲突取消操作
						}
					} 
					break;
				}
				case TO_UP:		//右转  
				{
					if (fk_run)						//游戏中右转
					{
						Russia_Data.fk_r++;
						if(fk_chk())Russia_Data.fk_r--;//有冲突取消操作
						Russia_Data.fk_r&=0x03;
					}
					else fk_run=1;//初始化时  
					break;
				}                    
            case KEY_D://水平增加
			{
			    if(LEVEL)LEVEL--;
			    else LEVEL=0;  
			    overtime=90-LEVEL*10;//最快的时候,延时10ms
			    Russia_msg();//更新显示 
			    break;
			}

				case PAUSE:pauseen=!pauseen;Russia_GUI();break; 
				case KEY_B:{GAME_PLAY=0;State=1;return ;}//无条件退出
			}  
			fk_reffk();	//刷新显示
			continue;
	    }
	    if(timecount==overtime)  
	    {    
	        timecount=0;//计时器清空  
	        if(pauseen)continue;//暂停,解除此次操作,重新开始
	         
	        if (fk_run)//游戏中下移
            {
			    if(Russia_Data.fk_y>0)
			    {
			        Russia_Data.fk_y--;
			        if(fk_chk())
			        {
			            Russia_Data.fk_y++;//有冲突取消操作,执行碰撞组合
					    fk_add();	//方块合并	 
			        }
			    }
			    else fk_add();		//方块合并   
			    fk_reffk();//刷新  
			    continue;
	        } 
	    }   
	    delay_ms(5);    
	    timecount++;//计时器增加
    }
    Game_Over_Show(1);//游戏结束
}

  
