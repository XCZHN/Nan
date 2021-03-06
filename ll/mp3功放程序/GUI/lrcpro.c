#include "main.h"					    			  
//歌词控制部分
//支持格式[XX:XX]/[XX:XX:XX]/[XX:XX.X]
//同时也支持多个并排的的歌词格式
//如[XX:XX.XX][XX:XX.XX]也是支持的	 
//正点原子@HYW
//2009.04.30
//V1.1	 		

//设计思想:
//第一步:把lrc文件的tag全部复制出来,存放到tag_table(最大支持426个tag,多出的部分将不予理会)
//第二步:把得到的tag,按顺序排列.得到标准的时间tag,保存在tag_table里面
//第三步:再按照tag的时间顺序,遍历tag,同时读取lrc文件的歌词,当读到符合条件的时候,复制tag及歌词到
//       歌词保存的地址buffer1,buffer2中.这样就得到了整个歌词
//注意:buffer1+buffer2的大小为6144个字节,超出部分将丢失.
  u8 bt30MS=0;                       //30ms到了的标志
/*					  
u16 lrc_tag_cnt=0; //记录tag的总数
u8  lrctagtype=0;  //记录lrc的类型 0:[XX:XX] 1:[XX:XX.XX]	  
//歌词文件的信息存放地址		   
FileInfoStruct *LrcFile=&F_Info[1];//开辟暂存空间.	 
u8 *tag_table=(u8 *)MCUBuffer;     //LRC时间戳暂存地址,后面将用于存放一句LRC歌词 

//LRC歌词的信息		 
typedef struct{ 
	u8   first;    //标志歌词是否更新了
	u16  t_sec;    //暂存歌词的时间(秒)
	u8   t_ms;     //保存32MS计数脉冲			 			    
    u16  lrcoff;   //歌词文件读取的长度
	u8   tagcnt;   //歌词结束标记
}LYRIC_STRUCT;
LYRIC_STRUCT lyric; //歌词信息

///////////////////////////////////////////////////////////////
unsigned short	code_value_table[4][256];
		  
u8 WriteByteLrcBuf(u16 addr,u8 data);//写入一个字节数据
u8 ReadByteLrcBuf(u16 addr);         //读取一个字节数据
u8 Get_Byte(u8 rst);//从sd得到一个字节
u8 Get_Num(void);   //得到一个数字
u8 Get_Tag(void); //得到tag列表
void Init_Tag(void);//对得到的tag排序
 
//从lrcbufer里面读取一个字节 
//读数错误,返回0xff     
//addr:地址
//返回值:读到的数据
u8 ReadByteLrcBuf(u16 addr)
{						  
	u8 *buffer1=(u8 *)iclip;//得到第一个buf存放的地址
	u8 *buffer2=(u8 *)code_value_table;//第二个buf地址
										    
	if(addr<4096)return buffer1[addr];
	else 
	{				 
		if(addr>6144)return 0xff;
		return buffer2[addr];//返回数据
	}
}


//向lrcbufer里面写一个字节,地址自动增加
//超出范围,返回0xff
//data:要写入的数据  
//addr:地址
//返回值:操作标志
//最多存储6Kbyte大小的歌词.
u8 WriteByteLrcBuf(u16 addr,u8 data)
{						  
	u8 *buffer1=(u8 *)iclip;//得到第一个buf存放的地址
	u8 *buffer2=(u8 *)code_value_table;//第二个buf地址	  							  
	if(addr<4096)buffer1[addr]=data; //写入数据到buffer1
	else 
	{			 
		if(addr>6144)return 0xff;
		buffer2[addr-4097]=data;  //写入数据到buffer2
	}
	return 1;
}

//复制一句歌词
//tmin,tsec,tmms:要复制的歌词tag
//返回值:0xff,结束.1,复制成功	  
u8 CopyString(u8 tmin,u8 tsec,u8 tmms)
{
	static u16 tagpos=0;   //歌词tag标志
	static u16 lrcbufpos=0;//歌词缓存地址
	u8 temp;
	u8 temp1;
	u16 tagcnt=0;

	if(tmin==0xff){tagpos=0;lrcbufpos=0;return 1;}//复位
	if(lrctagtype)tagcnt=(lrc_tag_cnt-1)*3;
	else tagcnt=(lrc_tag_cnt-1)*2;	
	if(tagpos>tagcnt)//读取结束了
	{
		WriteByteLrcBuf(lrcbufpos,'\0');//写入结束符
		return 0xff;
	}	 
	if(tmin==tag_table[tagpos]&&tsec==tag_table[tagpos+1]&&lrctagtype==0)//[XX:XX]类型歌词
	{	  
		tagpos+=2;
	}else if(tmin==tag_table[tagpos]&&tsec==tag_table[tagpos+1]&&tmms==tag_table[tagpos+2]&&lrctagtype==1) //类型2
	{
		tagpos+=3;
	}else return 1;
	WriteByteLrcBuf(lrcbufpos++,'['); //写一个字节
	WriteByteLrcBuf(lrcbufpos++,tmin);//写入分钟
	WriteByteLrcBuf(lrcbufpos++,':'); 
	WriteByteLrcBuf(lrcbufpos++,tsec);//写入秒钟
	if(lrctagtype)//XX:XX.XX格式
	{
		WriteByteLrcBuf(lrcbufpos++,'.'); 
		WriteByteLrcBuf(lrcbufpos++,tmms);//写入秒钟
	}
	WriteByteLrcBuf(lrcbufpos++,']');     //写入最后的结束标志
	 
	while(1)//一直等到得到tag结束标志
	{
		temp=Get_Byte(0);
		if(temp=='[') //可能意味着下一个tag的到来
		{
			WriteByteLrcBuf(lrcbufpos,temp);//先记录下来,以防不是
			temp1=Get_Byte(0);
			temp=temp1;//复制
			temp1-='0';
			if(temp1<10)//如果是数据,肯定是下一个tag了
			{
				while(1)
				{
					temp1=Get_Byte(0);
					if(temp1==']'||temp1==0xff)break;//读到tag末尾
				}
			}else  //不是tag,是数据
			{
				lrcbufpos++;
				WriteByteLrcBuf(lrcbufpos++,temp);//记录下来	  
				break;//读到有效数据了
			}
		}else if(temp!=' ')	//忽略空格
		{
			if(temp!=0XFF&&temp!=0X00)WriteByteLrcBuf(lrcbufpos++,temp);//记录下来 
			break;//读到了有效数据
		} 							   
	}						 
	while(1)//开始复制文本
	{
		temp=Get_Byte(0);//得到一个字符
		if(temp!=0XFF&&temp!=0X00)WriteByteLrcBuf(lrcbufpos++,temp);//复制合法字符      
		if(temp==0X0A||temp==0XFF||temp==0X00)//本句读取结束
		{
			WriteByteLrcBuf(lrcbufpos++,temp);
			break;
		}	    
	}
	return 1;
}   

//复制LRC歌词到ram里面
void CopyLrcToRam(void)
{					    		 
	u8 numx;
	u8 temp;
	u8 min;
	u8 sec;
	u8 mms;		
					    
	CopyString(0xff,0,0);//复位
	Get_Byte(1);//复位 
	while(1)
	{
		while(1)//一直等到得到tag开始标志
		{
			temp=Get_Byte(0);	 
			if(temp=='[')break;
			else if(temp==0xff) //所有数据读取结束了
			{
				Get_Byte(1);//复位 重新读
			}			   
		}
		numx=Get_Num();//得到第一个数字	  
		if(Get_Byte(0)!=':'||numx==0xff)continue;
		min=numx;
		numx=Get_Num();//得到第二个数字
		temp=Get_Byte(0);
		if(temp!='.'&&temp!=']'||numx==0xff)continue;
		sec=numx;	    
		if(temp!='.')  //[XX:XX]格式
		{				     
			if(CopyString(min,sec,0)==0XFF)return ;//复制完毕	 		    
			continue;
		}			   
		numx=Get_Num();//得到第三个数字
		temp=Get_Byte(0);
		if(temp!=']'||numx==0xff)continue;//[XX:XX.XX]格式
		mms=numx;
		if(CopyString(min,sec,mms)==0XFF)return ;//复制完毕	 
	}
}

	
//歌词存放地址,借用JPEG解码部分里面的RAM	   
u8 *LRC_DATA=(u8 *)QtZzMCUBuffer; 

//从歌词文件中得到一个LRC字符
//结束返回0XFF
//rst:1,复位,从头开始读书
//    2,偏移到上一个地址
u8 Get_Byte(u8 rst)
{		 
	static u16 bytepos;
	u8 temp;					  
	if(rst)//复位,不读取数据
	{
		if(rst==1)//复位
		{
			F_Open(LrcFile);//打开LRC文件
			F_Read(LrcFile,LRC_DATA);//读出512个字节	  
			bytepos=0;	  
		}
		else bytepos--;//偏移到上一簇 对付[XX:XX.X]格式的歌词
		return 0xff;
	}   
	if(bytepos>511)//超过一个扇区了,读下一个扇区
	{
		temp=F_Read(LrcFile,LRC_DATA);//读出512个字节	 
		bytepos=0; 
		if(temp==0)return 0xff;//读数错误	   
	} 
    return LRC_DATA[bytepos++];
}  
 
//得到一个数字
//失败返回0xff
u8 Get_Num(void)
{
	u8 t1=0;
	u8 t2=0;
	t1=Get_Byte(0);//得到第一位数据	 十位
	t2=Get_Byte(0);//得到第二位数据  各位
	if(t1<'9'&&t1>'0'&&t2==']')//遇到[XX:XX.X]这种格式
	{
		Get_Byte(2);//偏移到上一个地址  否则无法检测']'
		return (t1-'0')*10;
	}	 
	t1-='0';//得到ASCII数值
	t2-='0';
	if(t1<10&&t2<10)//得到合法数据
	{
		return t1*10+t2;
	}
	return 0XFF;
}	  
//得到歌词文件的所有时间戳 tag
//返回值：1,成功获取了tag。0，获取tag失败
u8 Get_Tag(void)
{	
	u16 i=0;														    
			 
	u8 numx;
	u8 temp;
	u8 min;
	u8 sec;
	u8 mms;			   
    lrc_tag_cnt=0;   //tag数清空
    lrctagtype=0;    //类型清空	  
	Get_Byte(1);     //复位 
	while(1)
	{
		if(lrc_tag_cnt>426)break;//超过了最大存储空间,后续不在保存
		while(1)//一直等到得到tag开始标志
		{
			temp=Get_Byte(0);  	   		 
			if(temp=='[')break;
			else if(temp==0xff)
			{
				if(lrc_tag_cnt)return 1;//所有数据读取结束了，偏移字节暂定为20	
				else return 0;//获取tag失败	非法的lrc文件	 
			}
		}
		numx=Get_Num();//得到第一个数字	  
		if(Get_Byte(0)!=':'||numx==0xff)continue;
		min=numx;
		numx=Get_Num();//得到第二个数字
		temp=Get_Byte(0);
		if(temp!='.'&&temp!=']'||numx==0xff)continue;
		sec=numx;		  					    
		if(temp!='.')//[XX:XX]格式
		{
			tag_table[i]=min;
			tag_table[i+1]=sec;
			i+=2;
			lrctagtype=0; //xx:xx格式
			lrc_tag_cnt++;//记录tag数
			continue;
		}
		numx=Get_Num();//得到第三个数字
		temp=Get_Byte(0);
		if(temp!=']'||numx==0xff)continue;//[XX:XX.XX]格式
		mms=numx;
		lrctagtype=1;  //xx:xx.xx格式
		lrc_tag_cnt++; //记录tag数
		tag_table[i]=min;
		tag_table[i+1]=sec;
		tag_table[i+2]=mms;
		i+=3;									 
	}
	return 1;
}
 
//初始化所有tag
//按升序排列    
void Init_Tag(void)
{
	u16 t=0;		 
	u8 t1;
	u8 temp;
	u16 tagcnt=0;
	u8 offset;

	if(lrctagtype)//XX:XX.XX
	{
		tagcnt=(lrc_tag_cnt-1)*3;
		offset=3; //XX:XX 
	}else 	 
	{
		tagcnt=(lrc_tag_cnt-1)*2;
		offset=2;
	}	  							 
	do//将数据X升序排列
	{	
		t1=0;		  
		for(t=0;t<tagcnt;t+=offset)
		{
			if(tag_table[t]>tag_table[t+offset])//分钟数大于后者,调换所有
			{
				temp=tag_table[t+offset];		  //分钟调换
				tag_table[t+offset]=tag_table[t];
				tag_table[t]=temp;

				temp=tag_table[t+offset+1];		  //秒钟调换
				tag_table[t+offset+1]=tag_table[t+1];
				tag_table[t+1]=temp;
				if(offset==3)
				{
					temp=tag_table[t+5];		  //mm调换
					tag_table[t+5]=tag_table[t+2];
					tag_table[t+2]=temp;   
				}	   
				t1=1; 	//有改变
			}else if(tag_table[t+1]>tag_table[t+offset+1]&&tag_table[t]==tag_table[t+offset])//秒钟数大于后者,调换后两位
			{
				temp=tag_table[t+offset+1];		  //秒钟调换
				tag_table[t+offset+1]=tag_table[t+1];
				tag_table[t+1]=temp;

				if(offset==3)
				{
					temp=tag_table[t+5];		  //mm调换
					tag_table[t+5]=tag_table[t+2];
					tag_table[t+2]=temp;   
				}	   
				t1=1; 	//有改变
			}else if(offset==3)	//XX:XX.XX
			{
				if(tag_table[t+2]>tag_table[t+5]&&tag_table[t]==tag_table[t+3]&&tag_table[t+1]==tag_table[t+4])//mm数大于后者,调换后两位
				{						 
					temp=tag_table[t+5];		  //mm调换
					tag_table[t+5]=tag_table[t+2];
					tag_table[t+2]=temp;
					t1=1; 	//有改变
				}
			} 
		}
	}while(t1);	
	for(t=0;t<=tagcnt;t+=offset)
	{
		if(offset==3)printf("[%d:%d.%d]\n",tag_table[t],tag_table[t+1],tag_table[t+2]);
		else printf("[%d:%d]\n",tag_table[t],tag_table[t+1]);
	}
}   
//在当前的目录中查找歌词文件,找到置相关数据至lyric中
//入口: 1.dircluster    当前目录的首簇号,若是FAT12或FAT16的根目录则为0
//如果找到,则返回1
//没找到,返回0
u8 FindLrcFile(unsigned long dircluster,unsigned char *strName)
{							  
	unsigned char  name[8];       
	u16 lrc_cnt=0;//当前目录下LRC文件的个数
	u16 i;
	u8 t;							    
	FileInfoStruct *FileInfo;

	//printf("START SEARCH LRC:%s\n\n",strName);
	for(i=0;i<8;i++)name[i]=strName[i];//取前面八个字节   	 						   
	lrc_cnt=0;	  
	FileInfo=&F_Info[2];//开辟LRC暂存空间.
	Get_File_Info(Cur_Dir_Cluster,FileInfo,T_LRC,&lrc_cnt);//获取当前文件夹下面的lrc文件个数  	  
	for(i=1;i<=lrc_cnt;i++)
	{
		Get_File_Info(Cur_Dir_Cluster,FileInfo,T_LRC,&i);//得到这个LRC的信息	 
        for(t=0;t<8;t++)if(name[t]!=FileInfo->F_ShortName[t])break;                    
        if(t==8) //找到歌词文件了
		{	
			//printf("Find Matched LRC\n");									  
			Get_File_Info(Cur_Dir_Cluster,LrcFile,T_LRC,&i);//读取信息到LrcFile	    
			
			//printf("LRC Name:%s\n",LrcFile->F_Name);			
			//printf("LRC Size:%d\n",LrcFile->F_Size);						  
			
			t=Get_Tag();  //得到tag
			if(t==0)break;//读取tag失败了
			//printf("Get LRC Tag OK\n");	 
			Init_Tag();	   //复制tag
			//printf("Init Tag OK!!\n");	 	
			CopyLrcToRam();//复制歌词到ram�
			return 1;  				 
		}
	}   
	return 0;
}  

//LRC读取初始化
void Lrc_Read_Init(void)
{
	lyric.t_sec=0;
	lyric.t_ms=0;    //时间清空 
	lyric.tagcnt=0;  //歌词没有结束
	lyric.lrcoff=0;  //偏移复位
	lyric.first=1;   //标记一下		  
}

//读取一句有效的歌词记录  
//并显示
void ReadLrcFileData(void)
{  
	static u8 mutiline=0;
	unsigned char *lbuf=(u8 *)MCUBuffer;//一句歌词的保存区间  
	unsigned char c;
	u8 cc;	
	u16 t;	 							     
				   
	if(lyric.tagcnt>=lrc_tag_cnt) return;//歌词读取已经结束了
	 
	lyric.t_sec=0xffff;
	lyric.t_ms=0; //时间清空 
	//从当前的位置读取歌词文件中的64个字节至缓冲区lbuf					   
	t=0;
	if(!lyric.first)//歌词还没有更新,显示歌词
	{  		    
		while(1)//读取一句歌词到lbuf里面
		{
			c=ReadByteLrcBuf(lyric.lrcoff++);//取出一个字节    
			if(c=='[')
			{
				cc=ReadByteLrcBuf(lyric.lrcoff);//试探下一个数据
				if(cc<=9){lyric.lrcoff--;break;}//试探成功	
			}
			if(c==0x0d||c==0x0a||c=='\0')break;//读到回车符号/结束符号    
			lbuf[t]=c;//记录
			t++;         
		}	    
	   	lbuf[t]='\0';//添加结束符 
		//POINT_COLOR=LRCCOLOR;
		if(mutiline)DrawBlock(0,154,239,219,BLACK);//擦除所有歌词
		else DrawBlock(0,154,239,170,BLACK);//只要擦除一行就ok了
		
		Show_Str_Mid(0,154,lbuf,WHITE,LRCCOLOR,0X03,240);//显示歌词,自动换行
		if(t>28)mutiline=1;//要多行显示 	   
		else mutiline=0;   //不要多行行显示	  	  			   
	}        
	//读取下一句歌词
	lyric.first=0;//标记歌词更新      	    
	while(1)//歌词信息读取信息
	{ 								  
		c=ReadByteLrcBuf(lyric.lrcoff++);//读取TAG的第一个字节    
		if(c=='[')//读取一个TAG 
		{
			lyric.tagcnt++;//tag标记
			lyric.t_sec=ReadByteLrcBuf(lyric.lrcoff++);//读取分钟
			lyric.t_sec*=60;
			lyric.lrcoff++;//偏移到秒钟
			lyric.t_sec+=ReadByteLrcBuf(lyric.lrcoff++);//读取秒钟 
			c=ReadByteLrcBuf(lyric.lrcoff++);
			if(c==']')break;//XX:XX格式	    
			//XX:XX.XX 格式	  
			lyric.t_ms=ReadByteLrcBuf(lyric.lrcoff++);  //读取mm 	  
		}else if(c==']')break;	 
	} 
}             
//check ok 08/09/06
// 歌词显示控制  
//同时控制显示解码时间   
//mnum 当前文件的序号 0~3
//注意:只支持单排时间的lrc正常显示
void LyricDisplayCtrl(void)
{  
	static unsigned int time=0;//time=0xffff;
	static unsigned char ms=0; //用来标记进行了多少个10ms    
	unsigned int t1;  
	if(!bt30MS)  return;//30ms未到
	ms+=3;//ms++
	bt30MS=0;   
    t1=GetDecodeTime(); //读取播放时间  
	if(t1!=time)//秒钟不合
	{
		time=t1;
		ms=0;//同步时间   
		return;
	}  											  
	if(((t1==lyric.t_sec)&&(ms>=lyric.t_ms))||(t1>lyric.t_sec))//时间符合,显示歌词
	{    
		ReadLrcFileData();  //读取歌曲数据并显示
	}   
} */

	 
//定时器3中断服务程序
//标记30ms中断时间到.
void TIM3_IRQHandler(void)
{ 					 		    
	if(TIM3->SR&0X0001)bt30MS=1;//溢出中断	    		   
	TIM3->SR&=~(1<<0);//清除中断标志位 	 
}
//TIMER3中断优先级设定
//在mp3里面优先级最高						  	   
void NVIC_TIMER3Configuration(void)
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
  	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQChannel; //使用外部中断1
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//阶级1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		 //阶层0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure); 
	//设置TIM3的中断 												  		 		   	 
  	NVIC_Init(&NVIC_InitStructure);        
#endif  				  					   							 
}   
//初始化红外接收引脚的设置
//开启中断,并映射 
void timer3_init(void)
{	
	//此部分需手动修改IO口设置
	RCC->APB1ENR|=1<<1;//TIM3时钟使能    
  	NVIC_TIMER3Configuration();										 
	TIM3->ARR=300;   //设定计数器自动重装值//刚好30ms 
	TIM3->PSC=7199;  //预分频器7200,得到10Khz的计数时钟
	//这两个东东要同时设置才可以使用中断
	TIM3->DIER|=1<<0;   //允许更新中断
	TIM3->DIER|=1<<6;   //允许触发中断

	TIM3->CR1=0x0080;  //ARPE使能 
	TIM3->CR1|=0x01;   //使能定时器3					 					  										  
}
































