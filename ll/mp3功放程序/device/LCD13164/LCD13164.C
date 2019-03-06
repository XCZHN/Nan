#include <stm32f10x_lib.h>
#include "main.h"
#include "font.h"  
extern u8 VS_VOLT;
u8 invt=0;//不反向;  
u8 LYRIC=0;  //是否有Lrc文件标志
u8 SINGLE=0; //单曲循环标志
u8 PLAYING=0;//正在播放标志 
MENU_CONECT  m_c[4];
MENU_CONECT CurDir;//记录当前文件夹信息 
//---------------------LCD初始化----------------------- 
void LCD_init()
{   
	RCC->APB2ENR|=1<<2;//先使能外设IO PORTA时钟
	RCC->APB2ENR|=1<<3;//先使能外设IO PORTB时钟
	RCC->APB2ENR|=0x01;    //使能io复用时钟
	AFIO->MAPR|=(1<<26);   //关闭JTAG-DP，关闭SW-DP

	GPIOA->CRH &=0xFFF00000;
	GPIOA->CRH |=0X00033333;//PA口推挽输出
	GPIOA->ODR = 0x0000;//PA全部输出高
	
	GPIOB->CRL &= 0xffff0fff; //PB3 推挽输出
	GPIOB->CRL |= 0x00003000; //PB3 推挽输出
	LCD_LIGHT_SET(0);		//开背光	
		  	  										  
	LCD_SDA_SET(0);//初始化为0	    
	LCD_SCK_SET(0);
	LCD_RS_SET(0);
	LCD_RST_SET(1);
	LCD_CS_SET(1);  
    LCD_RST_SET(0); //硬复位
    delay_ms(10);
    LCD_RST_SET(1);	  
    Write_comd(0xae); //0B10101110,最后一位为0 关闭显示
    Write_comd(0xa1); //ADC select,remap 131-->0,设定行对应起始位置  
    Write_comd(0xc8); //com select,remap 63-->0 ,设定列对应起始位置 
    Write_comd(0xa2); //lcd bias select 1/9 BIAS,设置偏压,不用管,默认1/9  
    Write_comd(0x2f); //power control(VB,VR,VF=1,1,1),电压控制,不用管  
    Write_comd(0x22); //Set Regulator rsistor ratio  ,粗调对比度 0~7.作用不大
    Write_comd(0x81); //Contrast Control Register    ,细调对比度
    Write_comd(0x19); //对比度值:0~63,总共64级,值越小,越暗 
    delay_ms(7);      //延时
    Write_comd(0xaf); //0B10101111,最后一位为1 开启显示
    Cleardisplay(ALL);   //清屏

}  


//--------------- 串行模式发送数据----------------
//发送一个byte数据到lcd
void Send_Data(unsigned char datain) 
{  
    unsigned char i;  
    for(i=0;i<8;i++) 
    {
        LCD_SCK_SET(0);   
        if((datain&0x80)==0)LCD_SDA_SET(0);
        else LCD_SDA_SET(1);
        delay_us(1);
		LCD_SCK_SET(1); 
		delay_us(1);  
        datain<<=1;  
    } 
}       
//------------------串行模式写数据---------------- 
//写数据，串行模式 
void Write_data(unsigned char data) 
{
	LCD_RS_SET(1);//写数据
    LCD_CS_SET(0);//选中    
    Send_Data(data);//发送数据 
    LCD_CS_SET(1);//释放总线
}   
//------------------串行模式写命令---------------- 
//写命令，串行模式 
void Write_comd(unsigned char comd) 
{ 
   LCD_RS_SET(0);//写命令		 
   LCD_CS_SET(0);//选中   
   Send_Data(comd);//发送命令 
   LCD_CS_SET(1);//释放总线
} 
//----------------------设置列-------------------- 
//设置列:0~131
void Set_column(unsigned char column)
{       
    column+=1;//偏移一个点
	Write_comd(column&0x0f);     //发送低四位	
	Write_comd((column>>4)|0x10);//发送高四位 
}   
//----------------------设置页--------------------  
//设置页0~7
void Set_page(unsigned char page)
{ 
	Write_comd((page&0x0f)|0xb0);
}               
//----------------------设置行--------------------  
//设置开始显示行:0~64   
//通过设置这个可以使屏幕实现上下滚动
void Set_row(unsigned char row)
{ 		  
	Write_comd(((64-row)&0x3f)|0x40);
}  

//清屏
void Cleardisplay(u8 row)
{
    unsigned char  j;
    if(row<4)
    {
        Set_page(2*row);
        Set_column(0x00);
        for(j=0;j<132;j++)Write_data(0x00);
        Set_page(2*row+1);
        Set_column(0x00);
        for(j=0;j<132;j++)Write_data(0x00);        
    }else 
    {
        for(j=0;j<4;j++)Cleardisplay(j);
    } 
}                 
//在指定位置显示一个字符
//x,0~3
//y,0~131
//chr,要显示的字符    
void Show_char(unsigned char x,unsigned char y,unsigned char chr)
{      
    unsigned char t;
    Set_page(2*x);//定位行(页0~7)
    Set_column(y);//定位列       
    for(t=0;t<12;t++)
    {                              
        if(t==6) //显示下一半
        {
            Set_page(2*x+1);
            Set_column(y);
        }
        Write_data(asc2[chr-32][t]);   
    }          
}
//在指定位置开始显示一个字符串
//x,0~3
//y,0~131
//str,要显示的字符串,支持自动换行  
void Show_str(unsigned char x,unsigned char y,const unsigned char * str)
{       
    Set_page(2*x);//定位行(页0~7)
    Set_column(y);//定位列       
    while(*str!='\0')
    {
        Show_char(x,y,*str);//写一个字符
        str++; 
        y+=8;
        if(y>123){x++;y=0;} //自动换行
        if(x>3){Cleardisplay(ALL);x=0;}//整行 
    } 
} 
//在指定位置显示一个数字
//x,0~3
//y,0~131
//str,要显示的数字
void Show_num(unsigned char x,unsigned char y,unsigned int num)
{      
    Show_char(x,y,(num/1000)%10+'0');
    Show_char(x,y+8,(num%1000)/100+'0');
    Show_char(x,y+16,(num%100)/10+'0'); 
	Show_char(x,y+24,num%10+'0'); 
}   

//显示12*12的icon 
//Ico:见 FONT 定义              
void Show_icon(unsigned char x,unsigned char y,unsigned char Ico)
{      
    unsigned char t;
    Set_page(2*x);//定位行(页0~7)
    Set_column(y);//定位列    
    for(t=0;t<24;t++)
    {                              
        if(t==12) //显示下一半
        {
            Set_page(2*x+1);
            Set_column(y);
        }
        if(invt)Write_data(~icon[Ico][t]); 
        else Write_data(icon[Ico][t]);   
    }    
}    

//在指定地址开始显示一个汉字 
void Show_font(unsigned char x,unsigned char y,unsigned char *chr)
{      
    unsigned char t;
    Set_page(2*x);//定位行(页0~7)
    Set_column(y);//定位列   
    for(t=0;t<24;t++)
    {                              
        if(t==12) //显示下一半
        {
            Set_page(2*x+1);
            Set_column(y);
        } 
        Write_data(chr[t]);   
    }   
} 
     
//显示一副图片
void draw_picture(const unsigned char *p)
{
    unsigned short t1,t2; 
    for(t1=0;t1<8;t1++)
    {                        
        Set_page(t1);
        Set_column(0);
        for(t2=0;t2<131;t2++)Write_data(p[t2+t1*131]);
    }
}

/*----------------------------------------------------------------------- 
LCD_write_String: 在LCD上显示汉字或英文字符
输入参数：x、y：显示汉字的起始X、Y坐标；  
enchg:1,支持换行.0,不换行
-----------------------------------------------------------------------*/
u8 LCD_write_String(u8 x, u8 y,u8 *p,u8 enchg)
{               
    u8 bHz=0;  //字符或者中文  
	unsigned char mat[24]; //保存12X12点陈内容      
	while(*p!=0)
	{ 
		if(!bHz)
		{
			if(*p>0x80)bHz=1;//中文 
			else             //字符
			{      
                if(y>125&&enchg){x++;y=0;}//一行已满,换行  
				if(enchg>1&&x>2)return 255;//只显示中间两行 //歌词显示的时候用
				if(x>3)return 255;//越界返回      
				if(*p==13)//换行符号
				{         
				    x++;y=0;
				    p++;//跳过 
				}  
				else Show_char(x,y,* p);  
				p++;
				y+=6; 
			}
		}else
		{      
		    if(y>119&&enchg){x++;y=0;}//换行
		    if(enchg>1&&x>2)return 255;//只显示中间两行 //歌词显示的时候用
			if(x>3)return 255;//越界返回      
			bHz=0;//有汉字库 
			Get_HzMat(p,mat);//得到点阵数据  
			Show_font(x,y,mat); 
			p+=2;
			y+=12; 
		}
	} //end while  
	return 0;//读取完毕
}     


//在指的地址显示一个字符串    
//支持 flash 型数据
void LCD_write_cstr(u8 x, u8 y,const u8 *str)
{  
    Set_page(2*x);//定位行(从0~7)
    Set_column(y);//定位列  
    while(*str!='\0')
    {  
        Show_char(x,y,*str);//写一个字符
        str++; 
        y+=6;
        if(y>123)//自动换行
        {
            x++;y=0;
            Set_page(2*x);//定位行(从0~7)
            Set_column(y);//定位列   
        } 
        if(x>3){Cleardisplay(4);x=0;}//整行清除
    }
}   

//在指定位置显示一个数字
//x,0~3
//y,0~131
//str,要显示的数字      
void Show_num2(unsigned char x,unsigned char y,unsigned char num)
{              
    Show_char(x,y,(num/10)%10+'0');//显示十位
    Show_char(x,y+6,num%10+'0');   //显示个位
}  	

//在指定位置显示一个小字符0~9:kbps/
//x,0~7
//y,0~131
//str,要显示的数字 位置  
void show_minichar(unsigned char x,unsigned char y,unsigned num)
{         
    unsigned char t;
    Set_page(x);//定位行(从0~7)
    Set_column(y);//定位列   
    for(t=0;t<5;t++)Write_data(miniasc2[num][t]);
    Write_data(0x00);//插入空隙
}    
//在指定位置显示一个小数字
void Show_mininum(unsigned char x,unsigned char y,unsigned char num)
{              
    show_minichar(x,y,(num/10)%10);//显示十位 
    show_minichar(x,y+6,num%10);   //显示个位 
}  

//以下LCD代码为游戏服务
//刷新数据,把lcdram的数据写入LCD里面
void lcd_refresh(void)
{
    u8 x,y;
	unsigned short temp;
    for(y=0;y<8;y++)
    {
        Set_page(7-y);Set_column(0);
        for(x=0;x<96;x++)
        {
            temp=(unsigned short)y*96+x;
            if(temp<512)Write_data(fat_buffer[temp]);
            //else Write_data(pbuffer[temp-512]);	
			else Write_data(MCUBuffer[temp-512]);
        }
    }
}    

//x:0~7,y:0~96
//读出lcdram的数据   
u8 lcd_read(u8 x,u8 y)
{
    unsigned short temp=0;
    temp=(unsigned short)y*96+x;   
    if(temp<512)return fat_buffer[temp];
    //else return pbuffer[temp-512];
	else return MCUBuffer[temp-512];
}  

//x:0~7,y:0~96
//写入lcdram的数据                         
void lcd_write(uchar x,uchar y,uchar data)
{
    uint temp=0;
    temp=(uint)y*96+x;
    if(temp<512)fat_buffer[temp]=data;
    //else pbuffer[temp-512]=data;
	else MCUBuffer[temp-512]=data;
}    
//在 96*64的范围内任意点画点 
//val:1,填充1;val:填充0;
void WritePixel(uchar x,uchar y,uchar val)
{
    uchar lcdram=0,temp=0x01; 
    lcdram=lcd_read(x,y/8);
    temp<<=7-y%8;
    if(val)lcdram|=temp;//填充1
    else   //填充0
    {             
        temp=~temp;    
        lcdram&=temp;    
    }
    lcd_write(x,y/8,lcdram); 
}   
//读取指定位置的一点 
//x:0~23;y:0~16
uchar ReadPixel(uchar x,uchar y)
{  
    uchar lcdram=0; 
    x=x*4;y=y*4;
    lcdram=lcd_read(x,y/8);//读出点位置
    lcdram>>=7-y%8;
    if(lcdram&0x01)return 1;
    else return 0; 
}
//在 24*16的范围内任意点画点(每点4*4大小) 
//画一个小方块
void Big_pinxl(uchar x,uchar y,uchar val)
{
    uchar t1,t2;
    for(t1=0;t1<4;t1++)
    for(t2=0;t2<4;t2++)WritePixel(4*x+t1,4*y+t2,val);//画点  
}  
//清除lcdsram
void clearlcdram(void)
{
    uint t;
    for(t=0;t<1024;t++)
    { 
        if(t<512)fat_buffer[t]=0x00;
        //else pbuffer[t-512]=0x00;
		else MCUBuffer[t-512]=0x00;
    } 
}                 
//保存文件时显示的界面
//不带清屏!!!
void save_show(void)
{  
    LCD_write_cstr(2,35,"Saveing.");
    delay_ms(800);
    LCD_write_cstr(2,35,"Saveing..");
    delay_ms(800);
    LCD_write_cstr(2,35,"Saveing...");
    delay_ms(800); 
}


//音乐模式下的界面显示
//显示MP3的基本信息
void MP3_msg(uchar pause) 
{  
    uchar t;               
    Show_num2(0,12,VS_VOLT/4);//显示音量
    Show_num2(0,40,7);//显示低音
    Show_num2(0,68,7);//显示高音  


    //Show_char(0,96,dspram+'0');//显示dsp1
	Show_char(0,96,'0');//显示dsp1
    
    if(SINGLE)Show_icon(0,105,10);//单曲循环 
    else Show_icon(0,105,9);//顺序播放    
    if(LYRIC)Show_icon(0,120,11);//显示LRC图标
    else Show_icon(0,120,14);//不显示图标             
    //显示暂停与否的图标
    Show_icon(3,0,12+pause);//显示播放暂停 
    //显示超重低音图标
    Set_page(7);//定位行(页0~7)
    Set_column(100);//定位列   
   /* if(voltemp[3]==0)for(t=0;t<24;t++)Write_data(bass[t]);//显示图标 
    else for(t=0;t<24;t++)Write_data(0x00); //不显示图标	*/
	for(t=0;t<24;t++)Write_data(bass[t]);
}  

//显示超重低音
void Show_Bass(uchar i)
{
	uchar t;
	Set_page(7);//定位行(页0~7)
    Set_column(100);//定位列   
	if(i)
	{
	 	for(t=0;t<24;t++)Write_data(bass[t]);
	}
	else
	{
	  	for(t=0;t<24;t++)Write_data(0x00);
	}
}                
//音乐模式下的界面显示   
//显示MP3播放时的界面 
unsigned int file_bps=0;//比特率
unsigned int off_time=0;//偏移时间  
void Play_GUI(void)
{                   
    uchar t;
    Cleardisplay(4);   //清屏   
    invt=0;            //防止反相显示 
    off_time=0;//清除偏移时间寄存器
    file_bps=0;//清除比特率寄存器
    for(t=0;t<4;t++)Show_icon(0,28*t,5+t);  
    Set_page(6);//定位行(页0~7)
    Set_column(12);//定位列    
    Write_data(0xFC);   
    for(t=0;t<78;t++)Write_data(0x84);//显示进度调
    Write_data(0xFC);
    Set_column(111);//显示Kbps符号
    for(t=0;t<20;t++)Write_data(kbps[t]); 
}        
//音乐模式下的进度条显示
//MP3播放时的进度条显示       
void Pro_msg(unsigned long npos,uchar mnum)
{         
    uint temptime=0;
    uchar t;
    if(file_bps>0)temptime=npos/(file_bps*125);//获得当前的时间 
    if(temptime!=off_time)//秒钟变化了,更新数据
    {
        off_time=temptime;
		//显示当前运行时间
        Show_mininum(7,20,off_time/60);//显示分钟
        show_minichar(7,32,10);//显示冒号
        Show_mininum(7,38,off_time%60);//显示秒钟   
    }
    npos=npos*80;//放大一百倍 
    mnum=npos/m_c[mnum].FileLen;//获得当前的位置百分比 
    if(mnum>80)return;//越界处理
    Set_page(6);//定位行(页0~7)
    Set_column(12);//定位列    
    for(t=0;t<mnum;t++)Write_data(0xFC);
}   
//设置模式下的界面显示
//显示vs1003的gui图标 
void Vs1003_GUI(void)
{         
    Cleardisplay(4);   //清屏   
    invt=0;            //防止反相显示
    LCD_write_cstr(0,35,"VS1003 Set");     
    //音量界面
    LCD_write_cstr(1,35,"Volume:"); 
    //高频界面
    LCD_write_cstr(2,0,"H Freq:");
    LCD_write_cstr(2,54,"Khz");
    LCD_write_cstr(2,78,"Treb:");
    //低音界面   
    LCD_write_cstr(3,0,"L Freq:");
    LCD_write_cstr(3,54,"0hz");
    LCD_write_cstr(3,78,"Bass:"); 
}    
   

     

   
