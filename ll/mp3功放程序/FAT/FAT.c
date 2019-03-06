//FAT文件系统管理
//2010/06/18 revise
//QiYuan
#include "main.h"

#define FSIZE 12
u32 file_cluster;
u32 file_size;
extern u8  SD_Type;//SD卡的类型
DWORD root_dir_clu=2;    //根目录簇号
DWORD FirstDataSector=2;	//数据区的第一个扇区
WORD  sector_size=512;	//每扇区字节数
DWORD FATsectors=0;		//FAT表所占扇区数
BYTE   cluster_size=8;		//每簇扇区数
DWORD fat_address=0;		//第一个FAT表(FAT1)所在扇区
DWORD root_address=0;		//根目录所在扇区
DWORD root_dir_size=0;	//fat16根目录所占扇区数
DWORD Root_Entries=0;		//根目录下目录项数 fat32为0
BYTE  fat_type=32;			//fat类型,默认fat32

DWORD Cur_Dir_Cluster=0;//当前目录簇号
DWORD Fat_Dir_Cluster=0;//父目录簇号 在FAT文件夹里面CopyDirentruyItem函数中修改!


//文件信息缓存区		   		    															   
FileInfoStruct F_Info[8]={0};
//外部全局变量

//FAT 数据缓存区,不能和jpg_buffer共用.否则在FAT32文件系统里面,可能出错!!
u8 fat_buffer[512]={0};
u8 LongNameBuffer[MAX_LONG_NAME_SIZE]={0};//长文件名的缓存区
BOOL LongNameFlag = 0;//是否存在长文件名的标志

u32 FONT16CLUSTER=0;//16字体的首簇
u32 FONT12CLUSTER=0;//12字体的首簇

u32 sys_ico[9]={0};  //系统图标缓存区!不能篡改!	 
u32 sys_ico1[9]={0};  //系统图标缓存区!不能篡改!
u32 file_ico[8]={0}; //文件图标缓存区 folder;mus;pic;book;//文件夹定义
u32 File_DIR[6]={0};	//根目录下的各大文件夹0:SYSTEM 1:TXT 2:MP3 3:PIC 4:RECEIVE

const unsigned char *folder[12]=
{
"SYSTEM     ",
"FONT       ",
"SYSICO     ",
"GAME       ",
"LEVEL1     ",
"LEVEL2     ",
"LEVEL3     ",
"MP3        ",
"TXT        ",
"RECEIVE    ",
"SEND       ",
"PIC        "
};	

//系统文件定义
const unsigned char *sysfile[19]=
{			  
"FONT16  FON",
"FONT12  FON",
//系统主界面图标
"MUSIC   BMP",
"PICTURE BMP",
"TXT     BMP",
"ALARM   BMP",
"TIME    BMP",
"SETTING BMP",  
"GAME    BMP",
"RADIO   BMP",
"NEXT    BMP",
//系统文件图标	12开始
"FOLDER  BMP",
"MUS     BMP",
"PIC     BMP",
"BOOK    BMP",
"DRAW    BMP",
"CALCU   BMP",
"QQ1     BMP",
"QQ2     BMP"
};

//第二页图标
const unsigned char *sysfile1[9]=
{			  
//系统主界面图标
"LIGHT   BMP",
"SETUP   BMP",
"QQ      BMP",
"JISUAN  BMP",  
"DAT     BMP",
"MESS    BMP",
"PICT    BMP",
"SEND    BMP",
"LAST    BMP",
};


u32 get_DW(u16 i)		 //得到buffer里偏移i为起始的双字
{
 	u32 temp = 0;
	temp += fat_buffer[i+3]<<24;	
	temp += fat_buffer[i+2]<<16;
	temp += fat_buffer[i+1]<<8;
	temp += fat_buffer[i];
	return temp;
}
//FAT初始化，不含SD的初始化，用之前应先调用sd的初始化
u8 FAT_Init(void)//Initialize of FAT  need initialize SD first
{  		   
	DBR 		*DBR_Point  = 0;
	DBR_BPB     *BPB_Point = 0;			  
	u32 BPB_address=0;
	u32 cap=0;//容量
	Cleardisplay(ALL);
	Show_str(0,0,"Insert SD card...");
	while(SD_Init());  //等待SD卡初始化正确完成,NO_ERROR时返回为0   
    SD_CS_ENABLE();	
	Cleardisplay(ALL);    
	Show_str(0,0,"Initial OK !");
	if(SD_Type==SD_TYPE_V2HC)		//显示版本号
	{
		Show_str(1,0,"SD_Type : SDHC");	
	}
	else if(SD_Type==SD_TYPE_V1)
	{
		Show_str(1,0,"SD_Type : SD1.0");	
	}
	else
	{
	}
	 
	(SD_ReadSingleBlock(0,fat_buffer));//读MBR
	if(fat_buffer[0x1fe]!=0x55 || fat_buffer[0x1ff]!=0xaa)	//判断SD卡分区是否出错
	{
		return 1;
	}

	if(get_DW(0x1C6))  		BPB_address = get_DW(0x1C6); //得到分区起始地址
	else if(get_DW(0x1D6))	BPB_address = get_DW(0x1D6); //得到分区起始地址
	else if(get_DW(0x1E6))	BPB_address = get_DW(0x1E6); //得到分区起始地址
	else if(get_DW(0x1F6))  BPB_address = get_DW(0x1F6); //得到分区起始地址//这里有问题的(只适用我的SDHC 4GB)

	SD_ReadSingleBlock(BPB_address, fat_buffer);//得到分区信息
	DBR_Point =  (DBR*)fat_buffer;
	BPB_Point = (DBR_BPB *)DBR_Point->bsBPB;
	if((get_DW(0x20)>0x800000))	//如果超过4G						
	{													
		cap=(u16)((get_DW(0x20)/(1024*1024))*512);	 	//读取容量
	}
	else  cap=(u16)((get_DW(0x20)*512)/(1024*1024));	//读取容量
	Show_num(2,0,cap);
	Show_str(2,32,"MB");
	if(fat_buffer[0x52]=='F'&&fat_buffer[0x53]=='A'&&fat_buffer[0x54]=='T'&&fat_buffer[0x55]=='3'&&fat_buffer[0x56]=='2')		
	{
		fat_type = 32;	//FAT32
		FATsectors	  = BPB_Point->bpbBigFATsecs;//一个FAT表占用的扇区数
		root_dir_clu = BPB_Point->bpbRootClust;//根目录所在簇号
	}
	else 
	{
		fat_type=16;	//FAT16
		FATsectors	= BPB_Point->bpbFATsecs;  //一个FAT表占用的扇区数 
		root_dir_clu = 2; //根目录开始簇号
	}
	Show_str(2,60,"FAT");		//显示fat类型 
	Show_num(2,84,fat_type);

	///////////////////////FAT初始化成功后得到的信息////////////////////////////
	sector_size	= BPB_Point->bpbBytesPerSec;      //每扇区字节数
	cluster_size	= (BYTE)BPB_Point->bpbSecPerClust;//每簇扇区数
	fat_address	= BPB_address+BPB_Point->bpbResSectors;//第一个FAT表所在扇区号=DBR所在扇区号+保留扇区

	Root_Entries	= BPB_Point->bpbRootDirEnts;		//根目录项数 ，FAT32为0
	root_dir_size	= (Root_Entries*32)/512;		//根目录占用的扇区数，所以这个也为0
	root_address = fat_address+BPB_Point->bpbFATs*FATsectors;//第一个目录(根目录)扇区号=第一个FAT表做在扇区号+(FAT表数)*(一个FAT所占扇区数)
	FirstDataSector	= root_address+root_dir_size;//第一个数据扇区号(对于FAT32来说就是根目录所在扇区号)

	return 0; 
}     

//读下一簇簇号
//传入当前簇号(2~n)，返回下一簇的簇号
//Return the cluster number of next cluster of file
//Suitable for system which has limited RAM
unsigned long FAT_NextCluster(unsigned long cluster)
{
	DWORD sector;
	DWORD offset;  

	if(fat_type==32)offset = cluster/128;//FAT32的FAT表中,用四个字节表示一个簇地址.512/4=128
	else offset = cluster/256;			 //FAT16的FAT表中,用两个字节表示一个簇地址.512/2=256
	if(cluster<2)return 0x0ffffff8;		 //簇0,1不能用于存放
	sector=fat_address+offset;//计算实际扇区数
	if(SD_ReadSingleBlock(sector,fat_buffer))return 0x0ffffff8;//读取FAT表,发生错误时返回0x0ffffff8
	if(fat_type==32) //如果是FAT32文件系统
	{
		offset=cluster%128;//查找位置
		sector=((unsigned long *)fat_buffer)[offset];	
	}
	else //如果是FAT16文件系统
	{
		offset=cluster%256;//查找位置
		sector=((unsigned short *)fat_buffer)[offset];
	}
	return (unsigned long)sector;//return the cluste number
}

//读前一簇簇号
//cluster:当前簇号
//startcluster:文件开始的簇号
//返回值:cluster前一个簇的簇号
//返回0xfffffff8则错误！
unsigned long FAT_PrevCluster(unsigned long cluster,unsigned long startcluster)
{
	DWORD temp;
	DWORD tempcluster;					  
	tempcluster=startcluster;//从起始簇号开始查找
	if(cluster==startcluster)return 0x0ffffff8;//查找错误
	while(1)
	{ 
		temp=FAT_NextCluster(tempcluster);  //查找当前簇的下一簇
		if(temp==cluster)return tempcluster;//下一簇就等于最终簇,返回前一簇,就是最终簇的上一簇
		else tempcluster=temp;				//继续下一簇查找
		if(temp==0x0ffffff8)return 0x0ffffff8;//查找错误
	}
}

//将簇号转变为扇区号
u32 fatClustToSect(u32 cluster)
{
	return root_address+(DWORD)(cluster-2)*(DWORD)cluster_size;	 
}

//文件类型
//返回值:对应的类型   0,mp3;1,wma;2,wav,3,mid;4,lrc;5,txt;6,c;7,h;8,jpg;9,jpeg;10,bmp;11,file;12,FON;   
const unsigned char *filetype[13]={"MP3","WMA","WAV","MID","LRC","TXT","C  ","H  ","JPG","JPE","BMP","   ","FON"};
//返回扩展名类型
//输入:exName 文件扩展名
u16 FileType_Tell(u8 * exName)
{
    u8 i;
    u8 t;   
    for(i=0;i<13;i++)
    { 
        for(t=0;t<3;t++)if(exName[t]!=filetype[i][t])break; 
        if(t==3)break;	//查找到对应的文件类型
    } 
    return 1<<i;//返回文件类型????????????????what's the matter???????????
}  

//复制文件记录项信息
void CopyDirentruyItem(FileInfoStruct *Desti,Short_DIR *Source)
{
	BYTE i;
	for(i=0;i<8;i++)Desti->F_ShortName[i]=Source->deName[i];//复制短文件名
	Desti->F_Type         = FileType_Tell(Source->deExtension);	//复制扩展名
	Desti->F_StartCluster = Source->deStartCluster + (((unsigned long)Source->deHighClust)<<16);//不用管
	Desti->F_Size         = Source->deFileSize;	   //复制文件大小
	Desti->F_Attr         = Source->deAttributes;  //文件属性
	Desti->F_CurClust     = 0;//扇区...
	Desti->F_Offset       = 0;//偏移0 	

	//FAT的簇号不能是0(更目录簇号)
	if(fat_type==32&&Desti->F_StartCluster==0)  
	{																  
		Desti->F_StartCluster=root_dir_clu;//改变这个簇号.使其等于根目录所在簇号!!
	}   
	if(LongNameFlag)//存在长文件名
	{
		LongNameBuffer[MAX_LONG_NAME_SIZE-1] = 0;
		LongNameBuffer[MAX_LONG_NAME_SIZE-2] = 0;
		UniToGB(LongNameBuffer);  //把Unicode代码转换为ASICII码
		for(i=0;i<80;i++)Desti->F_Name[i] = LongNameBuffer[i];//复制长文件名
	}else //短文件名
	{	
		if(Source->deName[0]==0x2e)//得到一个父目录(修改为:":\")
		{	
			//保存父目录簇号   
			Fat_Dir_Cluster=Desti->F_StartCluster; 	 
			Desti->F_Name[0]=':';
			Desti->F_Name[1]=0x5c;//'\'
			Desti->F_Name[2]='\0';//加入结束符
		}else	    			  //普通文件
		{
			for(i=0;i<11;i++)Desti->F_Name[i] = Source->deName[i];//复制短文件名  
			Desti->F_Name[11]='\0';//加入结束符
		}
	} 															   	  								  
	return ;
}


//浏览目标文件夹下面的一个文件类
//dir_clust:当前目录所在簇号
//FileInfo :目标文件的实体对象(FileInfoStruct体)
//type     :要查找的文件类型:1<<0,mp3;1<<1,wma;1<<2,wav,1<<3,mid;1<<4,1<<lrc;
//                           1<<5,txt;1<<6,jpg;1<<7,jpeg;1<<8,bmp;1<<9,file; 
//count    :0,返回当前目录下,该类型文件的个数;不为零时,返回第count个文件的详细信息
//返回值   :1,操作成功.0,操作失败
u8 Get_File_Info(u32 dir_clust,FileInfoStruct *FileInfo,u16 type,u16 *count)
{ 			  	   
	DWORD sector;
	DWORD cluster=dir_clust;
	DWORD tempclust;
	unsigned char cnt;
	unsigned int offset;		 
	unsigned short cont=0;//文件索引标志 <65536
	unsigned char j; //long name fat_buffer offset;
	unsigned char *p;//long name fat_buffer pointer
	Short_DIR *item = 0;
	Long_DIR *we =0;	  
	cont=0;
	LongNameFlag = 0;//清空长文件名标志

	//SD_Init();//初始化SD卡，在意外拔出之后可以正常使用
	//goto SD;
	if(cluster==0 && fat_type==16)//FAT16根目录读取
	{			 
		for(cnt=0;cnt<root_dir_size;cnt++)
		{
			if(SD_ReadSingleBlock(root_address+cnt,fat_buffer))return 1;//读数错误
//SD:			for(offset=0;offset<512;offset++)fat_buffer[offset]=temp_buf[offset];	   
			for(offset=0;offset<512;offset+=32)
			{
				item=(Short_DIR *)(&fat_buffer[offset]);//指针转换
				//找到一个可用的文件
				if((item->deName[0]!=0x2E)&&(item->deName[0]!=0x00)&&(item->deName[0]!=0xe5)
				||((item->deName[0]==0x2E)&&(item->deName[1]==0x2E)))//找到一个合法文件.忽略".",使用".."
				{		   
					if(item->deAttributes == 0x0f)//找到一个长文件名
					{
						we = (Long_DIR *)(&fat_buffer[offset]);
						j = 26 *( (we->weCnt-1) & WIN_CNT);//长文件名的长度
                        if(j<MAX_LONG_NAME_SIZE-25)
						{
							p = &LongNameBuffer[j];//偏移到目标地址
							for (j=0;j<10;j++)	*p++ = we->wePart1[j];			
							for (j=0;j<12;j++)	*p++ = we->wePart2[j];
							for (j=0;j<4;j++)	*p++ = we->wePart3[j];	
							if (we->weCnt & 0x40) (*(unsigned int *)p) = 0;  				
							if ((we->weCnt & WIN_CNT) == 1) LongNameFlag = 1;//最后一个长文件项找到了	
						}	    
					}else 
					{  	 										 									 
						if(type&FileType_Tell(item->deExtension))//找到一个目标文件
						{
							cont++;//文件索引(找到的文件数目)增加
						 }
						 //查找该目录下,type类型的文件个数
						if(*count&&cont==*count)
						{
							//printf("\ncount:%d",*count);
							CopyDirentruyItem(FileInfo,item);//复制目录项,提取详细信息 

							return 1;//找到目标文件成功	 
						}
						LongNameFlag=0;//清空长文件名
					}
				}
			}
		}  				 
	}
	else//其他文件夹/FAT32系统
	{
		tempclust=cluster;
		while(1)
		{
			sector=fatClustToSect(tempclust);  //簇号转换为扇区号
			for(cnt=0;cnt<cluster_size;cnt++)
			{
				if(SD_ReadSingleBlock(sector+cnt,fat_buffer))return 1;
				for(offset=0;offset<512;offset+=32)//每次偏移32个字节
				{
					item=(Short_DIR *)(&fat_buffer[offset]);
					if((item->deName[0]!=0x2E)&&(item->deName[0]!=0x00)&&(item->deName[0]!=0xe5)
					||((item->deName[0]==0x2E)&&(item->deName[1]==0x2E)))//找到一个合法文件.忽略".",使用".."
					{				
						if(item->deAttributes == 0x0f) //得到一个长文件名
						{
							we = (Long_DIR *)(&fat_buffer[offset]);
							j = 26 *( (we->weCnt-1) & WIN_CNT);	//52+26=78+2=80
							if(j<MAX_LONG_NAME_SIZE-25)
							{
								p = &LongNameBuffer[j];//p指向长文件名的存放地址
								for (j=0;j<10;j++)	*p++ = we->wePart1[j];			
								for (j=0;j<12;j++)	*p++ = we->wePart2[j];
								for (j=0;j<4;j++)	*p++ = we->wePart3[j];	
								if (we->weCnt & 0x40) (*(unsigned int *)p) = 0;				
								if ((we->weCnt & WIN_CNT) == 1) LongNameFlag = 1;	
							}										    			 
						}	  
						else  //短文件名
						{
							if(type&FileType_Tell(item->deExtension))//找到一个目标文件
							{
								cont++;//文件索引(找到的文件数目)增加
							 }
							 //查找该目录下,type类型的文件个数
							if(*count&&cont==*count)  //换作if(*count==cont)不行吗？？？？？？？？？？？
							{
								CopyDirentruyItem(FileInfo,item);//复制目录项,提取详细信息  
								return 1;//找到目标文件成功	 
							}
							LongNameFlag=0;//清空长文件名
						}
					}
				}
			}						  
			tempclust=FAT_NextCluster(tempclust);//查找下一个簇号
			if(tempclust==0x0fffffff||tempclust==0x0ffffff8 ||(fat_type==16&&tempclust==0xffff))break;
		}  
	}
	if(*count==0)
	{
		*count=cont;//得到总共文件数目
		return 1;   //操作成功,找到cont个符合条件的文件了
	}else return 0; //操作失败,没找到文件,或者出错
}


//打开文件
void F_Open(FileInfoStruct *FileInfo)
{
	FileInfo->F_CurClust=FileInfo->F_StartCluster;//当前簇为首簇
	FileInfo->F_Offset=0;//偏移扇区为0
}
//读取512个字节
//FileInfo:要读取的文件
//buf     :数据缓存区
//返回值  :0,操作失败,1,操作成功
unsigned char F_Read(FileInfoStruct *FileInfo,u8 *buf)
{	
	DWORD sector;			   		  
	sector=fatClustToSect(FileInfo->F_CurClust);//得到当前簇号对应的扇区号	   	 		    
	if(SD_ReadSingleBlock(sector+FileInfo->F_Offset,buf))return 0;//读数错误   
	FileInfo->F_Offset++; //扇区号加1
	if(FileInfo->F_Offset==cluster_size)	//簇的尽头,换簇
	{
		FileInfo->F_Offset=0;		    
		FileInfo->F_CurClust=FAT_NextCluster(FileInfo->F_CurClust);//读取下一个簇号
		if((fat_type==16&&FileInfo->F_CurClust==0xffff) \
		||FileInfo->F_CurClust==0x0ffffff8||FileInfo->F_CurClust == 0x0fffffff)return 0;//error	    
	} 
	return 1;//读取成功
}

//读取前512个字节
//FileInfo:要读取的文件
//buf     :数据缓存区
//返回值  :0,操作失败,1,操作成功
unsigned char F_PrevRead(FileInfoStruct *FileInfo,u8 *buf)
{	
	DWORD sector;  
	if(FileInfo->F_Offset>1)FileInfo->F_Offset-=2;//未超簇，直接跳到上一扇区  //>1???-2??
	else 					  //超簇了,换到上一簇
	{
		FileInfo->F_CurClust=FAT_PrevCluster(FileInfo->F_CurClust,FileInfo->F_StartCluster);//读取上一个簇号
		if((fat_type==16&&FileInfo->F_CurClust==0xffff) \
		||FileInfo->F_CurClust==0x0ffffff8||FileInfo->F_CurClust == 0x0fffffff)return 0;//读数错误	 
		if(FileInfo->F_Offset==1)FileInfo->F_Offset=cluster_size-1;//跳到最后一个扇区 
		else FileInfo->F_Offset=cluster_size-2;//读倒数第二簇  
	}  
	sector=fatClustToSect(FileInfo->F_CurClust);//得到当前簇号对应的扇区号	   
	if(SD_ReadSingleBlock(sector+FileInfo->F_Offset,buf))return 0;//读数错误   
	return 1;//读取成功
}

//比较两个字符串相等不
//相等,返回1,不相等,返回0;
u8 mystrcmp(u8*s1,u8*s2)
{
	u8 len1,len2;
	len1=len2=0;
	while(*s1!='\0')
	{
		len1++;s1++;
	}
	while(*s2!='\0')
	{
		len2++;s2++;
	}
	if(len1!=len2)return 0;//不相等 
	s1-=len1;s2-=len1;
	while(*s1!='\0')
	{
		if(*s1!=*s2)return 0;//不相等 
		s1++;s2++;
	}
	return 1;
}	 
//查找系统文件
//在指定目录下,找寻一个指定类型的指定名字的文件
//cluster:文件夹的簇号!!!
//Name   :文件的名字
//type   :文件类型
//返回值 :该文件的开始簇号
u32 F_Search(u32 cluster,unsigned char *Name,u16 type)
{
	DWORD sector;			 
	DWORD tempclust;
	unsigned char cnt;
	unsigned int offset; 

	Short_DIR *item = 0;	  
	FileInfoStruct FileInfo;    			    
	if(cluster==0 && fat_type==16)//FAT16根目录读取
	{			 
		for(cnt=0;cnt<root_dir_size;cnt++)
		{
			if(SD_ReadSingleBlock(root_address+cnt,fat_buffer))return 1;//读数错误    
			for(offset=0;offset<512;offset+=32)
			{
				item=(Short_DIR *)(&fat_buffer[offset]);//指针转换
				//找到一个可用的文件
				if((item->deName[0] != 0x00) && (item->deName[0] != 0xe5))//找到一个合法文件
				{		   
					if(item->deAttributes != AM_LFN)//忽略长文件名	 
					{  	 				 
						CopyDirentruyItem(&FileInfo,item);//复制目录项,提取详细信息	   
						if(FileInfo.F_Type&type)//找到一个合适的类型了
						{
						   // printf("File Name:%s\n",FileInfo.F_Name);
	
							//找到了文件,返回这个文件的首簇
							if(mystrcmp(Name,FileInfo.F_Name))return FileInfo.F_StartCluster; 
						} 	  
						LongNameFlag=0;//清空长文件名
					}
				}
			}
		}  				 
	}
	else//其他文件夹/FAT32系统
	{
		tempclust=cluster;
		while(1)
		{	
			sector=fatClustToSect(tempclust);
			for(cnt=0;cnt<cluster_size;cnt++)
			{
				if(SD_ReadSingleBlock(sector+cnt,fat_buffer))return 1;
				for(offset=0;offset<512;offset+=32)
				{
					item=(Short_DIR *)(&fat_buffer[offset]);
					if((item->deName[0] != 0x00) && (item->deName[0] != 0xe5))
					{				
						if(item->deAttributes != AM_LFN) //忽略长文件名		    
						{  	 				 
							CopyDirentruyItem(&FileInfo,item);//复制目录项,提取详细信息	  
							if(FileInfo.F_Type&type)//找到一个合适的类型了
							{	 
								//找到了文件,返回这个文件的首簇
								if(mystrcmp(Name,FileInfo.F_Name))
								{	
								 	file_cluster=FileInfo.F_StartCluster;
									file_size=FileInfo.F_Size;	
								  	return FileInfo.F_StartCluster; 
								}
								
							} 	  
							LongNameFlag=0;//清空长文件名
						}
					}
				}
			}						  
			tempclust=FAT_NextCluster(tempclust);//查找下一个簇号
			if(tempclust==0x0fffffff||tempclust==0x0ffffff8 ||(fat_type==16&&tempclust==0xffff))break;
		}  
	}
	return 0;//失败						    
}





//获取系统文件的存储地址
//次步出错,则无法启动!!!
//返回0，失败。返回1，成功
//sel:bit7：0,查找系统文件
//sel:bit7：1,查找游戏文件
//sel:bit 0~6 编号
u8 SysInfoGet(u8 sel)
{			 
	u32 cluster=0;
	u32 syscluster=0;
	u8 t=0;	  
//	u8 imgx[9];
//	u8 sizex;  //图片大小 也就是难度的大小	 
	//得到根目录的簇号
	if(fat_type==32)   //如果是FAT32文件系统
		cluster = root_dir_clu;
	else  //否则
		cluster=0;	

	cluster = F_Search(cluster,(unsigned char *)folder[0],T_FILE);//查找SYSTEM文件夹
	if(cluster==0)return 0;//系统文件夹丢失	
	syscluster=cluster;//保存系统文件夹所在簇号
	File_DIR[0]=cluster;	//SYSTEM
	
		
	cluster=F_Search(root_dir_clu,(unsigned char *)folder[8],T_FILE);//在根文件夹下查找TXT文件夹
	if(cluster==0)return 0;//字体文件夹丢失	
	File_DIR[1]=cluster;
	
	cluster=F_Search(root_dir_clu,(unsigned char *)folder[7],T_FILE);//在根文件夹下查找MP3文件夹
	if(cluster==0)return 0;//字体文件夹丢失	
	File_DIR[2]=cluster;	
	
	cluster=F_Search(root_dir_clu,(unsigned char *)folder[9],T_FILE);//在根文件夹下查找RECEIVE文件夹
	if(cluster==0)return 0;//字体文件夹丢失	
	File_DIR[4]=cluster;
	
	cluster=F_Search(root_dir_clu,(unsigned char *)folder[11],T_FILE);//在根文件夹下查找PIC文件夹
	if(cluster==0)return 0;//字体文件夹丢失	
	File_DIR[5]=cluster;			
				   
/*	
	if(sel&0x80)//查找游戏文件
	{	
		sizex=sel&0x7F;//得到size 3*3、4*4、5*5									  
		cluster=F_Search(cluster,(unsigned char *)folder[3],T_FILE);//在system文件夹下查找game文件夹
		if(cluster==0)return 0;  		
					   
		cluster=F_Search(cluster,(unsigned char *)folder[sizex+1],T_FILE);//在game文件夹下查找LEVEL(sizex-2)文件夹	
		if(cluster==0)return 0;//系统文件夹丢失	   	  			   
		sizex*=sizex;//取平方
		for(t=0;t<sizex+1;t++) //X*X图片地址获取
		{
			Get_Game_Pic_Str(t,imgx);//得到图片编号		  
			Pic_Addr[t]=F_Search(cluster,imgx,T_BMP);//在LEVEL1文件夹下查找BMP图片
			if(Pic_Addr[t]==0)return 0;//系统文件夹丢失	 	
		}	 
		//printf("level ok\n");
	}
*/
//	else//查找系统文件
//	{	 										 
		//先查找字体
		cluster=F_Search(syscluster,(unsigned char *)folder[1],T_FILE);//在system文件夹下查找FONT文件夹
		if(cluster==0)return 0;//字体文件夹丢失	  
		
		FONT16CLUSTER=F_Search(cluster,(unsigned char *)sysfile[0],T_FON);//在system文件夹下查找FONT16字体文件
		if(FONT16CLUSTER==0)return 0;//FONT16字体文件丢失		 
			  
		FONT12CLUSTER=F_Search(cluster,(unsigned char *)sysfile[1],T_FON);//在system文件夹下查找FONT12字体文件
		if(FONT12CLUSTER==0)return 0;//FONT12字体文件丢失						  	 
			
		cluster=F_Search(syscluster,(unsigned char *)folder[2],T_FILE);//在system文件夹下查找SYSICO文件夹
		if(cluster==0)return 0; 
		for(t=0;t<9;t++)//查找系统图标,九个
		{
			sys_ico[t]=F_Search(cluster,(unsigned char *)sysfile[t+2],T_BMP);//在SYSICO文件夹下查找系统图标
			if(sys_ico[t]==0)return 0;//失败		 
		}
		for(t=0;t<9;t++)//查找第二页的主界面图标
		{
			sys_ico1[t]=F_Search(cluster,(unsigned char *)sysfile1[t],T_BMP);//在SYSICO文件夹下查找系统图标
			if(sys_ico1[t]==0)return 0;//失败		
		}
		for(t=11;t<19;t++)//查找系统图标,6个
		{
			file_ico[t-11]=F_Search(cluster,(unsigned char *)sysfile[t],T_BMP);//在SYSICO文件夹下查找文件图标
			if(file_ico[t-11]==0)return 0;//失败 	  
		}
		
//	}
	return 1;//成功

}



//在指定dir_cluster下搜索编号为file_num的file_type类型文件
//Name：文件夹名
//返回: 文件名指针
u8* get_file_name(u8 file_num,u32 dir_cluster,u16 file_type)
{
	u8 *return_filename;
	u8 num=1;
	DWORD tempclust=dir_cluster;
	DWORD sector;
	u8 cnt;u16 offset;
	Short_DIR *item = 0;	  
	FileInfoStruct FileInfo;  
	while(1)
	{
		sector=fatClustToSect(tempclust);
		for(cnt=0;cnt<cluster_size;cnt++)
		{
			if(SD_ReadSingleBlock(sector+cnt,fat_buffer))return 0;
			for(offset=0;offset<512;offset+=32)
			{
				item=(Short_DIR *)(&fat_buffer[offset]);
				if((item->deName[0] != 0x00) && (item->deName[0] != 0xe5))
				{				
					if(item->deAttributes != AM_LFN) //忽略长文件名		    
					{  	 				 
						CopyDirentruyItem(&FileInfo,item);//复制目录项,提取详细信息	  
						if(FileInfo.F_Type&file_type)//找到一个TXT类型
						{	
							if(num==file_num) 
							{
								return_filename=FileInfo.F_Name;
								return return_filename;	//得到文件文件												
							}

							num++;	
						} 	  
						LongNameFlag=0;//清空长文件名
					}
				}
			}
		}						  
		tempclust=FAT_NextCluster(tempclust);//查找下一个簇号
		if(tempclust==0x0fffffff||tempclust==0x0ffffff8 ||(fat_type==16&&tempclust==0xffff))break;	
	}
	return 0;
}

u8 get_file_num(u32 dir_cluster,u16 type)
{    
	u16 num;
	FileInfoStruct *FileInfo=0;
	num=0;		//指针所指的数清0
	while(!Get_File_Info(dir_cluster,FileInfo,type,&num));		//获取TXT文件个数
	return (u8)num;
}


 //Cluster 簇的序号 返回为该簇号所对应的扇区地址
//得到Cluster
unsigned long GetSysFileSector(u8 uoh,unsigned int Cluster)
{
	unsigned long foffset=0;
	unsigned int i; 
	
	if(uoh==16)foffset=FONT16CLUSTER;//得到unicode码的首簇  
    else if(uoh==12)foffset=FONT12CLUSTER; 
	i=Cluster;
	while(i)
	{
	    //printf("foffset:%d\n",foffset);
	    foffset=FAT_NextCluster(foffset);
	    i--;
	}
	//printf("final foffset:%d\n",foffset);
	return (root_address+ (foffset-2)*8); //返回字符所在扇区号
}    

//code 字符指针开始
//从字库中查找出字模
//code 字符串的开始地址,ascii码
//mat  数据存放地址 FSIZE*2 bytes大小
void Get_HzMat(unsigned char *code,unsigned char *mat)
{
	unsigned char qh,wh;
	unsigned char i;
	unsigned int  sector,cluster,secoff;
	unsigned long foffset; 
	if(*code<0xa1)if(*code++<0xa1)//非 常用汉字
	{   
	    code--;
	    for(i=0;i<(FSIZE*2);i++)*mat++=0xff;//填充满格
	    return; //结束访问
	}          
	qh=(*code++)-0xa1;	 //汉字内码高8位
	wh=(*code)-0xa1;   	 //汉字内码低8位

	foffset=((unsigned long)94*qh+wh)*(FSIZE*2);//得到字库中的字节偏移量   
	//LCD_ShowNum(20,1,wh,4,WHITE,BLUE);
	sector=foffset/sector_size;              //得到总的完整的扇区数(不要写成(unsigned int ) 
 	secoff= (unsigned int) foffset%sector_size;//扇区内的字节数偏移
	wh=(unsigned char)sector%cluster_size;		//簇内偏移 
	cluster=(unsigned int)sector/cluster_size;//得到总的簇数
	
	foffset=GetSysFileSector(FSIZE,cluster); //取汉字库的clusor簇数的扇区地址 
	 
	if(sector_size-secoff>=(FSIZE*2))//确定是否跨扇区?
	{        
		 SD_Read_Bytes(foffset+wh,mat,secoff,(FSIZE*2));  //读取(FSIZE*2)字节
	}
	else
	{
		i=sector_size-secoff; //读取的数据已跨扇区
		SD_Read_Bytes(foffset+wh,mat,secoff,i);
		if(++wh>cluster_size)
		{  
			wh=0; 
			foffset=GetSysFileSector(0,++cluster);//读取的数据已经跨簇
		} 
		SD_Read_Bytes(foffset+wh,mat+i,0,(FSIZE*2)-i); //读取剩余的数据
	} 
}  

