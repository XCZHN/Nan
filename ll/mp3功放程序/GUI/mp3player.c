//MP3 功放程序 MP3部分
//QiYuan@SCUT
//2010/07/09
#include "main.h"  
extern u8 VS_VOLT;
extern u8 VS_BASS;
extern u8 M_Type;
u8	MCUBuffer[10*64];  
//short	QtZzMCUBuffer[10*64];
//long iclip[1024];//4k BYTES
u8 voltemp[6]={50,7,7,0,0,0};//声音寄存器,第一次刷机时的默认设置 
//u8 INITDSP=0;  //0~4 eeprom级别 五种音效  
u16 f_kbps=0;//歌曲文件位率	
//进度条,时间,比特率 等事实信息显示控制
void MP3_PROG(u32 pos,u32 lenth)
{	
	static u16 playtime=0;//播放时间标记	     
	float propos=0;	
	u16 time=0;// 时间变量
	u16 temp=0;
	u8 t=0;
	if(f_kbps==0xffff)//未更新过
	{
		playtime=0;
		f_kbps=GetHeadInfo();	   //获得比特率

		Show_mininum(6,93,f_kbps/10);//显示高两位
		show_minichar(6,105,f_kbps%10);//显示最低位  
		 
		time=(lenth/f_kbps)/125;//总时间
		//显示总时间
		if(f_kbps)time=(lenth/f_kbps)/125;//得到秒钟数   (文件长度(字节)/(1000/8)/比特率=持续秒钟数    	  
		else time=0;//非法位率

		show_minichar(7,50,11); 
        Show_mininum(7,56,time/60);//显示分钟
        show_minichar(7,68,10);
        Show_mininum(7,74,time%60);//显示秒钟  
	}
	if(lenth)
	{
		propos=(float)pos*512/lenth;//得到百分比 	
		if(propos>1)propos=1;//限制propos的范围
	}else return;//文件为空,返回    
  	if(f_kbps)//对位率不为0 的文件
	{
		time=(lenth/f_kbps)/125;//总时间
		time=propos*time;       //得到当前比例时间
	}else time=GetDecodeTime(); //对位率为0的文件,得到解码时间
 	
	propos=propos*80;//得到实际长度
	if(propos>80) return;//越界处理
	Set_page(6);//定位行(页0~7)
    Set_column(12);//定位列    
    for(t=0;t<propos;t++)Write_data(0xFC);
		  
	if(time!=playtime)//1s时间到,更新显示数据
	{	  
		temp=GetHeadInfo(); //获得比特率   				 
		if(temp)
		{
			f_kbps=temp;//更新KBPS	
			time=(((float)lenth/f_kbps)/125)*(propos/80);//更新时间	 
		}	
		playtime=time;//更新时间 	 
		//显示播放时间			 
		//LCD_ShowNum(30,70,time/60,2,MP3COLOR4,GREEN,0); //分钟	   
		//LCD_ShowNum(54,70,time%60,2,MP3COLOR4,GREEN,0);//秒钟 
		//显示当前运行时间
	    Show_mininum(7,20,time/60);//显示分钟
        show_minichar(7,32,10);//显示冒号
        Show_mininum(7,38,time%60);//显示秒钟  
	}   		 
}  
 
 /*
//check ok 08/09/06
// 歌词显示控制  
//同时控制显示解码时间   
//mnum 当前文件的序号 0~3
//注意:只支持单排时间的lrc正常显示
void LyricDisplayCtrl(uchar mnum)
{  
	static unsigned int time=0xffff;
	static unsigned char ms=0;  
	unsigned long lenth=0; 
	unsigned int t1;  
	if(!bt30MS)  return;//30ms未到
	ms+=3;//ms++
	bt30MS=0;  
	t1=GetDecodeTime(); //读取播放时间  
	if(t1!=time)//秒钟不合
	{
		time=t1;ms=0;//同步时间  
		timer=time;  //修改同步时间计数器(这个是歌词的准确计时器,不是显示的时间!) 
		             //如果这个乱了的话,歌词也会乱,所以快进后显示不了歌词
		//得到比特率
		t1=GetHeadInfo();  
        //显示歌曲长度             
        if(t1!=file_bps&&t1>0)//只有当kbps变化时才执行,提高系统速度
        {              
            //显示比特率Kbps
		    Show_mininum(6,93,t1/10);//显示高两位
		    show_minichar(6,105,t1%10);//显示最低位   
            file_bps=t1;//保存此次信息长度;
            //printf("BIT Rate is:%d\n",file_bps); 
            lenth=m_c[mnum].FileLen/125;//得到文件长度xxxx Kbps
            t1=lenth/t1;
            show_minichar(7,50,11); 
            Show_mininum(7,56,t1/60);//显示分钟
            show_minichar(7,68,10);
            Show_mininum(7,74,t1%60);//显示秒钟  
		}
		return;
	}
	if(!ENLRC||!LYRIC)return;//无歌词或者歌词被禁止使用  
	if(((t1==lyric.t_sec)&&(ms>=lyric.t_ms))||(t1>lyric.t_sec))
	{    
		ReadLrcFileData();  //读取歌曲数据并显示
	}   
}   	 */ 


//显示歌曲名字
//num:歌曲序号           
void music_name(FileInfoStruct *FileName)
{
    //u8 i;
    /*while(m_c[mnum].LongName[i]!='\0')//把短名字放中间显示
    {
        i++;
        if(i>21){i=22;break;}
    }     */             
    //i=66-3*i;//对13164液晶而言
    LCD_write_String(1,0,FileName->F_Name,2);//无歌词/歌词显示不使能    
    
}

							  	 
//播放选定的歌曲 					  
//index:当前播放的歌曲索引
//total:总共音乐文件的个数
//关于音乐的几乎所有控制都在这个里面
u16 Mus_Play(FileInfoStruct *FileName,u16 index,u16 total)
{	 
    unsigned long bfactor=0; //物理扇区,绝对地址
    unsigned long bcluster=0;//物理簇号
	u8 pause=1; 
	u32 file_pos=0;          //记录文件播放的长度 
    u8 count=0; 			 //扇区计数器,用于换簇
    u16 i,n;
	u8 keycnt;	 //按键计数器
	u8 FFT_LRC=0;//默认FFT

 	u8 *BUFFER=(u8*)MCUBuffer;//指向JPEG的开始512字节
	u8 *spedate=(u8*)MCUBuffer+512;//当前频谱分析的结果寄存器
  	u8 *topval=(u8*)MCUBuffer+526; //FFT柱的最高点记录表
	u8 *flyval=(u8*)MCUBuffer+540; //FFT柱的悬浮点记录表
			  	   
	u8 mark=1;		//标记
	u8 timer_temp=0;		//频谱定时器,为了不与全局同名，因此改成temp
	u32 topstay1;   //最高点频点延时1
	u32 topstay2;	//最高点频点延时2
	//bit0~9用来标记功能 ,默认全部循环
	//bit0,1:循环模式;bit2,3,4:音量设置;bit5,6:其他设置
	//bit7:超重低音标记;bit8:是否有歌词;bit9:音效存/取标记
	u16 sysfun=1;
	u16 temp;		  
	u8 forpreclust;//快进快退需要的簇数   								 
	timer3_init();//初始化歌词显示定时器  	 
	if(!voltemp[5])sysfun|=1<<7;   
RESTART: 			 		  
	forpreclust=FileName->F_Size/(cluster_size*sector_size*50);//每次前进/后退 2%所需要的簇数	   
	if(forpreclust<2)forpreclust=2;//最少也要两个簇
	//fft部分,变量清空
	topstay1=0;
	topstay1=topstay1;
	topstay2=0;
	topstay2=topstay2;
	file_pos=0;	  
	keycnt=0;
	keycnt=keycnt;
	f_kbps=0xffff;//文件位率设置为不可能值
	count=0;	  //扇区计数器	  	  
    bfactor=fatClustToSect(FileName->F_StartCluster);//得到开始簇对应的扇区
	bcluster=FileName->F_StartCluster;				 //得到文件开始簇号	 
	//从当前目录下找歌词文件,有,则标记.	 
	/*if(FindLrcFile(Cur_Dir_Cluster,FileName->F_ShortName))
	{							   
		sysfun|=1<<8;     //标记有歌词
		Lrc_Read_Init();  //初始化歌词读取
	}else sysfun&=~(1<<8);//标记没有歌词*/	 		  
	Mp3Reset();       //硬复位
	Vs1003SoftReset();//软复位VS1003
	LoadPatch();      //加载频谱分析代码 
 	//Load_Gui_Mp3(sysfun);//加载播放界面
	Play_GUI();
	MP3_msg(pause); 
	//Show_Str_Mid(0,24,FileName->F_Name,MP3COLOR4,WHITE,0x01,240); //显示歌曲名字
	music_name(FileName);  //显示歌名
	timer_temp=5;//设置到一个不可能的值,初始化信息栏
	timer_temp=timer_temp;						 	
	//FFT数据清空
	for(i=0;i<14;i++)//清空相关数据
	{
		topval[i]=0;		 	    
		flyval[i]=0;
		spedate[i]=0;
	}								  
	key=255;		 	    	
	while(1)   //播放音乐的主循环
    {
        if(SD_ReadSingleBlock(bfactor,BUFFER))break;//读取一个扇区的数据 
        i=0;  	  
        count++;//扇区计数器 
		do      //主播放循环
        {  
			while(VS_DREQ==0&&((sysfun&0X0400)==0)) //等待高/进行频谱显示/非暂停/按键没有按下 
			{
				if(FileName->F_Type!=T_MID)//不是MID格式的，则处理
				{
					if(FFT_LRC==0)//频谱模式,对于非MID格式有效
					{
						if(mark)
						{
							GetSpec(spedate);//得到频谱数据
							MP3_PROG(file_pos,FileName->F_Size);//MP3进度显示/播放时间  
						}
						//else LCD_ShowFFT(spedate,topval,flyval,&topstay1,&topstay2);//频谱显示     	  
						/*timer_temp++; 
						if(timer_temp>=3)//FFT最高的悬浮点控制 
						{
							Top_Delet_FFT(flyval,&topstay1,&topstay2);//删除高端
							timer_temp=0; 
						}*/   
					}else//歌词模式
					{					   											    
						if(mark&&sysfun&(1<<8))	
						{
						 	//LyricDisplayCtrl();//存在歌词,则显示
						}
						else if(mark==0)MP3_PROG(file_pos,FileName->F_Size);//MP3进度显示/播放时间 				  
					}	
					mark=!mark;
				}else//MID格式文件 
				{	  
					if(NPEN)break;
					MP3_PROG(file_pos,FileName->F_Size);//MID进度显示/播放时间 	
				}	
			}
			if(VS_DREQ!=0&&((sysfun&0X0400)==0))  //非暂停,送数据给VS1003
			{			   	 
	            for(n=0;n<32;n++)Vs1003_DATA_Write(BUFFER[i++]);//发送音乐数据    
			} 							
			if(key!=255)//有按键按下
			{	 
				switch(key)
				{

					case UP://上一曲
						index--;
						if(index==0)index=total;
						Get_File_Info(Cur_Dir_Cluster,FileName,T_MP3|T_WMA|T_WAV|T_MID,&index);
						goto RESTART;
					case DOWN://下一曲
						index++;
						if(index>total)index=1;
						Get_File_Info(Cur_Dir_Cluster,FileName,T_MP3|T_WMA|T_WAV|T_MID,&index);
						goto RESTART;
					case KEY_C://播放暂停
						temp=(sysfun>>10)&0x01;
						temp=!temp;	  	   
						if(temp)
						{
						   	pause=0;
							Show_icon(3,0,12+pause);//显示播放暂停 //播放
						}
						else
						{
						   	pause=1;
							Show_icon(3,0,12+pause);//显示播放暂停 //播放	
						}						    
						sysfun&=0XFBFF;
						sysfun|=temp<<10;
						break;
					case KEY_B://退出
					   	Vs1003SoftReset();//软复位
						return 1; 

					case LEFT://音量减
						if(VS_VOLT<=4)VS_VOLT=0;
						else VS_VOLT=VS_VOLT-4;
						Show_num2(0,12,VS_VOLT/4);//显示音量
						VS_SET_VOL(VS_VOLT,VS_BASS);
						break;
					case RIGHT://音量加	
						if(VS_VOLT>=251)VS_VOLT=255;
						else VS_VOLT=VS_VOLT+4;
						Show_num2(0,12,VS_VOLT/4);//显示音量
						VS_SET_VOL(VS_VOLT,VS_BASS);
						break;
					 case KEY_D://超重低音//Supper Bass 部分
					 		 
						if(VS_BASS==0)
						{
							VS_BASS=255;
							VS_SET_VOL(VS_VOLT,VS_BASS);
							Show_Bass(1);//显示Bass
						}
						else 
						{
							VS_BASS=0;
							VS_SET_VOL(VS_VOLT,VS_BASS);
							Show_Bass(0);//不显示Bass
						}
						break;
				}  
				key=255;	  						  
			} 		           
        }while(i<511);//循环发送512个字节 
		file_pos++;//增加512个字节	  
        bfactor++; //扇区加

        //prgpos+=512;//进度条增加   
        //Pro_msg(prgpos,mnum);//显示进度条    
		if(file_pos>(FileName->F_Size/512))break;//文件长度到了  结束了，跳出
        if(count>=cluster_size)//一个簇结束,换簇
        {
            count=0;  		   
            bcluster=FAT_NextCluster(bcluster);	   
			//文件结束 
			if(((fat_type==16)&&bcluster==0xffff)||bcluster==0x0ffffff8||bcluster==0x0fffffff)break;//error
			bfactor=fatClustToSect(bcluster);
	    } 
    } 						  
	//init_pt2314(NONE_Audio);//静音处理  			   
	Mp3Reset();       //硬复位
	Vs1003SoftReset();//软复位VS1003

	temp=sysfun&0x03;//0/1				 
	if(temp==1)//全部循环
	{
		index++; 						   
    	if(index>total)index=1;
		key=0xffff;	
		Get_File_Info(Cur_Dir_Cluster,FileName,T_MP3|T_WMA|T_WAV|T_MID,&index);			  
	}								   
	goto RESTART;//重新回到那里播放,单曲/全部循环	 
}

//音乐浏览器
//支持FAT32/16文件系统的浏览.
//check:09/04/18
void Mus_Viewer(void)
{		      
	FileInfoStruct *FileInfo;		 
	u16 mus_cnt=0;//当前目录下音乐文件的个数
	u16 index=0;  //当前选择的文件编号	 
	Cleardisplay(ALL);   //清屏
	//从根目录开始浏览
	if(fat_type==32)Cur_Dir_Cluster=root_dir_clu;//根目录簇号
 	else Cur_Dir_Cluster=0;						   
	Fat_Dir_Cluster=Cur_Dir_Cluster;//父目录就是当前目录
	while(1)
	{
BROS:	 	     
		index=File_Browse(T_MP3|T_WMA|T_WAV|T_MID);//浏览文件夹    
		if(!index){State=1;return;}//结束浏览,退出此功能!!!!!
		mus_cnt=0;	 
		Get_File_Info(Cur_Dir_Cluster,FileInfo,T_MP3|T_WMA|T_WAV|T_MID,&mus_cnt);//获取当前文件夹下面的目标文件个数 
		FileInfo=&F_Info[0];//开辟暂存空间.
		while(1)
		{
			Get_File_Info(Cur_Dir_Cluster,FileInfo,T_MP3|T_WMA|T_WAV|T_MID,&index);//得到这首歌曲的信息	 
			Cleardisplay(ALL); //清13164显示  
			Mus_Play(FileInfo,index,mus_cnt);			   	   
			goto BROS;	   
		}
	}			 
}



















