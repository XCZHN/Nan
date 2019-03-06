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

//dir_clust:��ǰĿ¼�غ�
//FileInfo: �ļ�ָ��
//type:     Ҫ������ļ�����
//����ֵ:   ��ǰ�ļ�����
//�ļ����� ".":����ǰĿ¼,"..":������һ��Ŀ¼
//�ļ�Ŀ¼����ļ�����λ
#define	AM_RDO	0x01	//ֻ���ļ�
#define	AM_HID	0x02	//�����ļ�
#define	AM_SYS	0x04	//ϵͳ�ļ�
#define	AM_VOL	0x08	//���
#define AM_LFN	0x0F	//���ļ����ֶ�
#define AM_DIR	0x10	//�ļ���
#define AM_ARC	0x20	//�鵵�ļ�

//MBR��512���ֽڵĶ���     
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




//DBR��512���ֽڵĶ���
typedef __packed struct 
{
	BYTE	bsJump[3];					// ��תָ��
	BYTE	bsOemName[8];				// ���ù�
	BYTE	bsBPB[53];					// BPBռDBR�е�53���ֽ�
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



//BPB��53���ֽڵĶ���
typedef __packed struct 
{
		WORD	bpbBytesPerSec;	// ÿ�����ֽ���
		BYTE	bpbSecPerClust;	// ÿ��������
		WORD	bpbResSectors;	// ����������
		BYTE	bpbFATs;		// FAT����
		WORD	bpbRootDirEnts;	// number of root directory entries
		WORD	bpbSectors;		// ��������
		BYTE	bpbMedia;		// media descriptor
		WORD	bpbFATsecs;		// һ��FAT��ռ������
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
		DWORD     bpbRootClust;	// ��Ŀ¼���ڴغ� very important
		WORD      bpbFSInfo;	// filesystem info structure sector
		WORD      bpbBackup;	// backup boot sector
		// There is a 12 byte filler here, but we ignore it
}DBR_BPB;	

  

// �ļ��ṹ�塣���ļ�Ŀ¼��32�ֽڶ���
typedef __packed struct
{
		BYTE		deName[8];      	// �ļ���
#define SLOT_EMPTY      0x00            // slot has never been used
#define SLOT_E5         0x05            // the real value is 0xE5
#define SLOT_DELETED    0xE5            // file in this slot deleted
#define SLOT_DIR		0x2E			// a directorymmm
		BYTE		deExtension[3]; 	// �ļ���չ��
		BYTE		deAttributes;   	// �ļ�����
#define ATTR_NORMAL     0x00            // ��ͨ�ļ�
#define ATTR_READONLY   0x01            // ֻ���ļ�
#define ATTR_HIDDEN     0x02            // �����ļ�
#define ATTR_SYSTEM     0x04            // ϵͳ�ļ�
#define ATTR_VOLUME     0x08            // ���
#define ATTR_LONG_FILENAME	0x0F		// ���ļ�����־			    
#define ATTR_DIRECTORY  0x10            // �ļ����ļ�
#define ATTR_ARCHIVE    0x20            // �µĻ��߹鵵�ļ�
		BYTE        deLowerCase;    	// �̶�Ϊ0
#define LCASE_BASE      0x08            // filename base in lower case
#define LCASE_EXT       0x10            // filename extension in lower case
		BYTE        deCHundredth;   	// ����
		BYTE        deCTime[2];     	// �ļ�����ʱ��
		BYTE        deCDate[2];     	// �ļ���������
		BYTE        deADate[2];     	// ����������
		WORD        deHighClust; 		// �ļ���ʼ�ظ�2�ֽ�
		BYTE        deMTime[2];     	// ����޸�ʱ��
		BYTE        deMDate[2];     	// ����޸�����
		WORD        deStartCluster; 	// �ļ���ʼ�ص�2�ֽ�
		DWORD       deFileSize;  		// �ļ���С
}Short_DIR;	  



#define DIRENTRIES_PER_SECTOR	0x10	//ÿ������512���ֽ� 

// Structure of a Win95 long name directory entry
//���ļ�������
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
	BYTE F_ShortName[8];//����˸��ֽڵĶ��ļ���
	BYTE F_Name[MAX_LONG_NAME_SIZE];
	unsigned long F_StartCluster;			//< file starting cluster for last file accessed
	unsigned long F_Size;					//< file size for last file accessed
	u8  F_Attr;				            	//< file attr for last file accessed
	u16	F_Type;
	//unsigned short CreateTime;			//< file creation time for last file accessed
	//unsigned short CreateDate;			//< file creation date for last file accessed
	unsigned long F_CurClust;				//�ļ���ǰ�غ�
	unsigned int  F_Offset;				    //�ļ��ڴغ����������ƫ��
}FileInfoStruct;


//Ŀ¼Ҫ��ʾ��4���ļ���
typedef  struct{
    unsigned  char ShortName[12];//���ļ����洢	
    unsigned  char LongName[52]; //���ļ����洢
    unsigned  long Clust;        //���ݵ��״� 
    unsigned  long FileLen;      //�ļ�����    
    unsigned  char Type;         //�ļ�����  
    unsigned  int  FileIndex;    //�ļ�ƫ��
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


	  						   
//FATԭ�еĺ���	    
u8 FAT_Init(void);//FAT��ʼ��												  
unsigned long FAT_NextCluster(unsigned long cluster);//������һ�غ�	
u32 fatClustToSect(u32 cluster);//���غ�ת��Ϊ������
u8 Get_File_Info(u32 dir_clust,FileInfoStruct *FileInfo,u16 type,u16 *count);//�����ļ�
void F_Open(FileInfoStruct *FileInfo);//���ļ�
unsigned char F_Read(FileInfoStruct *FileInfo,u8 *buf);//���ļ���size=0���������ļ�
unsigned long FAT_OpenDir(BYTE * dir);//��Ŀ¼   
u32 F_Search(u32 cluster,unsigned char *Name,u16 type);
u32 get_DW(u16 i);		 //�õ�buffer��ƫ��iΪ��ʼ��˫��
u8 SysInfoGet(u8 sel);//����ϵͳ�ļ���
void CopyDirentruyItem(FileInfoStruct *Desti,Short_DIR *Source);
//��������������һ��ƫ������ĵ�ַ���������������ϵ�ƫ�ƣ�������������������
extern u8 skip_data(u32* i,u8* sector,u32* cluster,u32* data_address,u32 offset);
u8* get_file_name(u8 file_num,u32 dir_cluster,u16 file_type);
u8 get_file_num(u32 dir_cluster,u16 type);
unsigned long FAT_PrevCluster(unsigned long cluster,unsigned long startcluster);
unsigned char F_Write(FileInfoStruct *FileInfo,u8 *buf);


extern u32 file_cluster;
extern u32 file_size;
extern u8  SD_Type;//SD��������
//FAT���ִ��� 					    
//����ԭ��@SCUT
//ȫ�ֱ�������
extern DWORD root_dir_clu;    //��Ŀ¼�غ�
extern DWORD FirstDataSector;	//�������ĵ�һ������
extern WORD  sector_size;	//ÿ�����ֽ���
extern DWORD FATsectors;		//FAT����ռ������
extern BYTE  cluster_size;		//ÿ��������
extern DWORD fat_address;		//��һ��FAT��(FAT1)��������
extern DWORD root_address;		//��Ŀ¼��������
extern DWORD root_dir_size;	//fat16��Ŀ¼��ռ������
extern DWORD Root_Entries;		//��Ŀ¼��Ŀ¼���� fat32Ϊ0
extern BYTE  fat_type;			//fat����,Ĭ��fat32
extern DWORD Cur_Dir_Cluster;//��ǰĿ¼�غ�
extern DWORD Fat_Dir_Cluster;//��Ŀ¼�غ� ��FAT�ļ�������CopyDirentruyItem�������޸�!
extern void Get_HzMat(unsigned char *code,unsigned char *mat);

//�ļ���Ϣ������		   		    															   
extern FileInfoStruct F_Info[];
extern u32 FONT16CLUSTER;//16������״�
extern u32 FONT12CLUSTER;//12������״�
extern u8  fat_buffer[];
extern BOOL LongNameFlag;//�Ƿ���ڳ��ļ����ı�־
extern u32 File_DIR[];
extern u32 sys_ico[];  //ϵͳͼ�껺����!���ܴ۸�!
extern u32 sys_ico1[];  //ϵͳͼ�껺����!���ܴ۸�!
extern u32 file_ico[]; 

	  
#endif
















