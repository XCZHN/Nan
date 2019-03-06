#ifndef __FAT_H__
#define __FAT_H__	

#include "comm.h"	  

	    	   	  																	    
#define MSDOSFSROOT     0               // cluster 0 means the root dir
#define CLUST_FREE      0               // cluster 0 also means a free cluster
#define MSDOSFSFREE     CLUST_FREE
#define CLUST_FIRST     2             	// first legal cluster number
#define CLUST_RSRVD     0xfff6      	// reserved cluster range
#define CLUST_BAD       0xfff7     		// a cluster with a defect
#define CLUST_EOFS      0xfff8     		// start of eof cluster range
#define CLUST_EOFE      0xffff      	// end of eof cluster range

//dir_clust:当前目录簇号
//FileInfo: 文件指针
//type:     要浏览的文件类型
//返回值:   当前文件索引
//文件夹中 ".":代表当前目录,"..":代表上一级目录
//文件目录项的文件属性位
#define	AM_RDO	0x01	//只读文件
#define	AM_HID	0x02	//隐藏文件
#define	AM_SYS	0x04	//系统文件
#define	AM_VOL	0x08	//卷标
#define AM_LFN	0x0F	//长文件名字段
#define AM_DIR	0x10	//文件夹
#define AM_ARC	0x20	//归档文件

//MBR中512个字节的定义     
typedef __packed struct 
{
	BYTE	psPartCode[512-64-2];		// pad so struct is 512b
	BYTE	DPTable[64];					// four partition records (64 bytes)
	BYTE	psBootSectSig0;				// two signature bytes (2 bytes)
	BYTE	psBootSectSig1;
#define BOOTSIG0        0x55
#define BOOTSIG1        0xaa
}MBR;



typedef __packed struct  
{
	BYTE	exDriveNumber;				// drive number (0x80)//0x00 for floopy disk 0x80 for hard disk
	BYTE	exReserved1;				// reserved should always set 0
	BYTE	exBootSignature;			// ext. boot signature (0x29)
#define EXBOOTSIG       0x29
	BYTE	exVolumeID[4];				// volume ID number
	BYTE	exVolumeLabel[11];			// volume label "NO NAME"
	BYTE	exFileSysType[8];			// fs type (FAT12 or FAT)
}extboot;




//DBR中512个字节的定义
typedef __packed struct 
{
	BYTE	bsJump[3];					// 跳转指令
	BYTE	bsOemName[8];				// 不用管
	BYTE	bsBPB[53];					// BPB占DBR中的53个字节
	BYTE	bsExt[26];					// Bootsector Extension
	BYTE	bsBootCode[418];			// pad so structure is 512b
	BYTE	bsBootSectSig2;				// boot sector signature byte 0x00 
	BYTE	bsBootSectSig3;				// boot sector signature byte 0x00
	BYTE	bsBootSectSig0;				// boot sector signature byte 0x55 
	BYTE	bsBootSectSig1;				// boot sector signature byte 0xAA
#define BOOTSIG0        0x55
#define BOOTSIG1        0xaa
#define BOOTSIG2        0x00
#define BOOTSIG3        0x00
}DBR;



//BPB中53个字节的定义
typedef __packed struct 
{
		WORD	bpbBytesPerSec;	// 每扇区字节数
		BYTE	bpbSecPerClust;	// 每簇扇区数
		WORD	bpbResSectors;	// 保留扇区数
		BYTE	bpbFATs;		// FAT表数
		WORD	bpbRootDirEnts;	// number of root directory entries
		WORD	bpbSectors;		// 总扇区数
		BYTE	bpbMedia;		// media descriptor
		WORD	bpbFATsecs;		// 一个FAT表占扇区数
		WORD	bpbSecPerTrack;	// sectors per track
		WORD	bpbHeads;		// number of heads
		DWORD	bpbHiddenSecs;	// # of hidden sectors
// 3.3 compat ends here
		DWORD	bpbHugeSectors;	// # of sectors if bpbSectors == 0
// 5.0 compat ends here
		DWORD     bpbBigFATsecs;// like bpbFATsecs for FAT32
		WORD      bpbExtFlags;	// extended flags:
#define FATNUM    0xf			// mask for numbering active FAT
#define FATMIRROR 0x80			// FAT is mirrored (like it always was)
		WORD      bpbFSVers;	// filesystem version
#define FSVERS    0				// currently only 0 is understood
		DWORD     bpbRootClust;	// 根目录所在簇号 very important
		WORD      bpbFSInfo;	// filesystem info structure sector
		WORD      bpbBackup;	// backup boot sector
		// There is a 12 byte filler here, but we ignore it
}DBR_BPB;	

  

// 文件结构体。短文件目录项32字节定义
typedef __packed struct
{
		BYTE		deName[8];      	// 文件名
#define SLOT_EMPTY      0x00            // slot has never been used
#define SLOT_E5         0x05            // the real value is 0xE5
#define SLOT_DELETED    0xE5            // file in this slot deleted
#define SLOT_DIR		0x2E			// a directorymmm
		BYTE		deExtension[3]; 	// 文件扩展名
		BYTE		deAttributes;   	// 文件属性
#define ATTR_NORMAL     0x00            // 普通文件
#define ATTR_READONLY   0x01            // 只读文件
#define ATTR_HIDDEN     0x02            // 隐藏文件
#define ATTR_SYSTEM     0x04            // 系统文件
#define ATTR_VOLUME     0x08            // 卷标
#define ATTR_LONG_FILENAME	0x0F		// 长文件名标志			    
#define ATTR_DIRECTORY  0x10            // 文件夹文件
#define ATTR_ARCHIVE    0x20            // 新的或者归档文件
		BYTE        deLowerCase;    	// 固定为0
#define LCASE_BASE      0x08            // filename base in lower case
#define LCASE_EXT       0x10            // filename extension in lower case
		BYTE        deCHundredth;   	// 忽略
		BYTE        deCTime[2];     	// 文件创建时间
		BYTE        deCDate[2];     	// 文件创建日期
		BYTE        deADate[2];     	// 最后访问日期
		WORD        deHighClust; 		// 文件开始簇高2字节
		BYTE        deMTime[2];     	// 最后修改时间
		BYTE        deMDate[2];     	// 最后修改日期
		WORD        deStartCluster; 	// 文件开始簇低2字节
		DWORD       deFileSize;  		// 文件大小
}Short_DIR;	  



#define DIRENTRIES_PER_SECTOR	0x10	//每个扇区512个字节 

// Structure of a Win95 long name directory entry
//长文件名定义
typedef __packed struct 
{
		BYTE			weCnt;			// 
#define WIN_LAST        0x40
#define WIN_CNT         0x3f
		BYTE		wePart1[10];
		BYTE		weAttributes;
#define ATTR_WIN95      0x0f
		BYTE		weReserved1;
		BYTE		weChksum;
		BYTE		wePart2[12];
		WORD       	weReserved2;
		BYTE		wePart3[4];
}Long_DIR;


typedef struct  
{
	BYTE F_ShortName[8];//保存八个字节的短文件名
	BYTE F_Name[MAX_LONG_NAME_SIZE];
	unsigned long F_StartCluster;			//< file starting cluster for last file accessed
	unsigned long F_Size;					//< file size for last file accessed
	u8  F_Attr;				            	//< file attr for last file accessed
	u16	F_Type;
	//unsigned short CreateTime;			//< file creation time for last file accessed
	//unsigned short CreateDate;			//< file creation date for last file accessed
	unsigned long F_CurClust;				//文件当前簇号
	unsigned int  F_Offset;				    //文件在簇号里面的扇区偏移
}FileInfoStruct;


//目录要显示的4行文件名
typedef  struct{
    unsigned  char ShortName[12];//短文件名存储	
    unsigned  char LongName[52]; //长文件名存储
    unsigned  long Clust;        //内容的首簇 
    unsigned  long FileLen;      //文件长度    
    unsigned  char Type;         //文件类型  
    unsigned  int  FileIndex;    //文件偏移
}MENU_CONECT; 

#define WIN_ENTRY_CHARS	13      // Number of chars per Long_DIR

// Maximum filename length in Win95
// Note: Must be < sizeof(dirent.d_name)
#define WIN_MAXLEN      255

// This is the format of the contents of the deTime field in the direntry
// structure.
// We don't use bitfields because we don't know how compilers for
// arbitrary machines will lay them out.
#define DT_2SECONDS_MASK        0x1F    // seconds divided by 2
#define DT_2SECONDS_SHIFT       0
#define DT_MINUTES_MASK         0x7E0   // minutes
#define DT_MINUTES_SHIFT        5
#define DT_HOURS_MASK           0xF800  // hours
#define DT_HOURS_SHIFT          11

// This is the format of the contents of the deDate field in the direntry
// structure.
#define DD_DAY_MASK				0x1F	// day of month
#define DD_DAY_SHIFT			0
#define DD_MONTH_MASK			0x1E0	// month
#define DD_MONTH_SHIFT			5
#define DD_YEAR_MASK			0xFE00	// year - 1980
#define DD_YEAR_SHIFT			9


	  						   
//FAT原有的函数	    
u8 FAT_Init(void);//FAT初始化												  
unsigned long FAT_NextCluster(unsigned long cluster);//查找下一簇号	
u32 fatClustToSect(u32 cluster);//将簇号转换为扇区号
u8 Get_File_Info(u32 dir_clust,FileInfoStruct *FileInfo,u16 type,u16 *count);//查找文件
void F_Open(FileInfoStruct *FileInfo);//打开文件
unsigned char F_Read(FileInfoStruct *FileInfo,u8 *buf);//读文件，size=0代表整个文件
unsigned long FAT_OpenDir(BYTE * dir);//打开目录   
u32 F_Search(u32 cluster,unsigned char *Name,u16 type);
u32 get_DW(u16 i);		 //得到buffer里偏移i为起始的双字
u8 SysInfoGet(u8 sel);//搜索系统文件夹
void CopyDirentruyItem(FileInfoStruct *Desti,Short_DIR *Source);
//数据跳过，计算一定偏移量后的地址，若出现扇区以上的偏移，最后读出该扇区的数据
extern u8 skip_data(u32* i,u8* sector,u32* cluster,u32* data_address,u32 offset);
u8* get_file_name(u8 file_num,u32 dir_cluster,u16 file_type);
u8 get_file_num(u32 dir_cluster,u16 type);
unsigned long FAT_PrevCluster(unsigned long cluster,unsigned long startcluster);
unsigned char F_Write(FileInfoStruct *FileInfo,u8 *buf);


extern u32 file_cluster;
extern u32 file_size;
extern u8  SD_Type;//SD卡的类型
//FAT部分代码 					    
//正点原子@SCUT
//全局变量区域
extern DWORD root_dir_clu;    //根目录簇号
extern DWORD FirstDataSector;	//数据区的第一个扇区
extern WORD  sector_size;	//每扇区字节数
extern DWORD FATsectors;		//FAT表所占扇区数
extern BYTE  cluster_size;		//每簇扇区数
extern DWORD fat_address;		//第一个FAT表(FAT1)所在扇区
extern DWORD root_address;		//根目录所在扇区
extern DWORD root_dir_size;	//fat16根目录所占扇区数
extern DWORD Root_Entries;		//根目录下目录项数 fat32为0
extern BYTE  fat_type;			//fat类型,默认fat32
extern DWORD Cur_Dir_Cluster;//当前目录簇号
extern DWORD Fat_Dir_Cluster;//父目录簇号 在FAT文件夹里面CopyDirentruyItem函数中修改!
extern void Get_HzMat(unsigned char *code,unsigned char *mat);

//文件信息缓存区		   		    															   
extern FileInfoStruct F_Info[];
extern u32 FONT16CLUSTER;//16字体的首簇
extern u32 FONT12CLUSTER;//12字体的首簇
extern u8  fat_buffer[];
extern BOOL LongNameFlag;//是否存在长文件名的标志
extern u32 File_DIR[];
extern u32 sys_ico[];  //系统图标缓存区!不能篡改!
extern u32 sys_ico1[];  //系统图标缓存区!不能篡改!
extern u32 file_ico[]; 

	  
#endif
















