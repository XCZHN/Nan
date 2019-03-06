/*====================================================================================
==                                                                                              
**  Copyright(c) 2014-09  Asen. All rights reserved.                                 
==                                                                                           
**	Author:		Asen                                                                        
==                                                                                          
**  Version:	v1.0.0                                                                          
==
**  Filename:	Drv_RFM.h                                                                       
==                                                                                         
**  Function:	读卡芯片 FM1702SL 驱动控制程序
==                                                                                         
**  Data:       2014/09/22                                                                       
=====================================================================================*/
#ifndef	_DRV_FM1702SL_H_
#define	_DRV_FM1702SL_H_


#include "Config.h"
#include "LPC17xx.h"



//-----------------------------硬件接口功能定义---------------------------------------/
#define	FM_RST_pin	        10     				 //P1.10

#define FM_RST_H		    (LPC_GPIO1->FIOSET |= (1<<FM_RST_pin)) 
#define FM_RST_L    		(LPC_GPIO1->FIOCLR |= (1<<FM_RST_pin))


/******************** FM1702SL 命令码*****************************************/
#define Transceive 	0x1E	//发送接收命令
#define Transmit   	0x1A	//发送命令
#define ReadE2     	0x03   	//读取FM1715 EEPROM命令
#define WriteE2    	0x01   	//写FM1715 EEPROM命令
#define Authent1   	0x0c   	//验证命令认证过程第1步
#define Authent2   	0x14   	//验证命令认证过程第2步
#define LoadKeyE2  	0x0B   	//将密钥从EEPROM复制到KEY缓存
#define LoadKey    	0x19   	//将密钥从FIFO memory复制到key memory

#define RF_TimeOut 	0x50  	//send command delay 
#define Req        	0x01   
#define Sel	   		0x02

/******************** FM1702SL 寄存器地址定义*********************************/
/*  
	FM1702SL的内部寄存器按功能不同分成8组每组为一页包含8个寄存器:
	Page0:指令和状态寄存器组
	Page1:控制和状态寄存器组
	Page2:发射及编码控制寄存器组
	Page3:接收及解码控制寄存器组
	Page4:时间及校验控制寄存器组              FM1702SL_REG_
	Page5:FIFOTimer及IRQ控制寄存器组
	Page6~Page7:预留寄存器组
*/
#define FM1702SL_REG_Page_Sel			0x00	/* 选择寄存器组 */
#define FM1702SL_REG_Command			0x01	/* 命令寄存器 */
#define FM1702SL_REG_FIFO				0x02	/* 64字节FIFO缓冲的输入输出寄存器 */
#define FM1702SL_REG_PrimaryStatus		0x03	/* 发射器接收器及FIFO的状态寄存器1 */
#define FM1702SL_REG_FIFO_Length		0x04	/* 当前FIFO内字节数寄存器 */
#define FM1702SL_REG_SecondaryStatus	0x05	/* 各种状态标示寄存器 */
#define FM1702SL_REG_InterruptEn		0x06	/* 中断使能/禁止控制寄存器 */
#define FM1702SL_REG_Int_Req			0x07	/* 中断请求标识寄存器 */
#define FM1702SL_REG_Control			0x09	/* 各种控制标示寄存器 */
#define FM1702SL_REG_ErrorFlag			0x0A	/* 上一条指令结束后错误标示 */
#define FM1702SL_REG_CollPos			0x0B	/* 侦测到的第一个冲突位的位置 */
#define FM1702SL_REG_TimerValue			0x0C	/* 定时器当前值 */
#define FM1702SL_REG_Bit_Frame			0x0F	/* 位帧调整寄存器 */
#define FM1702SL_REG_TxControl			0x11	/* 发送控制寄存器 */
#define FM1702SL_REG_CWConductance		0x12	/* 选择发射脚TX1和TX2发射天线的阻抗 */
#define FM1702SL_REG_ModConductance		0x13	/* 定义输出驱动阻抗 */
#define FM1702SL_REG_CoderControl		0x14	/* 定义编码模式和时钟频率 */
#define FM1702SL_REG_DecoderControl		0x1a	/* 解码控制寄存器 */
#define FM1702SL_REG_RxControl1			0x19	/* 接收增益控制 */
#define FM1702SL_REG_RxControl2			0x1e	/* 解码控制及选择接收源 */
#define FM1702SL_REG_RxWait				0x21	/* 选择发射和接收之间的时间间隔 */
#define FM1702SL_REG_ChannelRedundancy	0x22	/* 选择数据校验种类和模式 */
#define FM1702SL_REG_CRCPresetLSB		0x23	/* CRC预置寄存器低8位 */
#define FM1702SL_REG_CRCPresetMSB		0x24	/* CRC预置寄存器高8位 */
#define FM1702SL_REG_MFOUTSelect		0x26	/* mfOUT configure register */
#define FM1702SL_REG_TimerClock			0x2a	/* 选择TIMER时钟的分频 */
#define FM1702SL_REG_TimerControl		0x2b	/* 选择TIMER启动/停止条件 */
#define FM1702SL_REG_TimerReload		0x2c	/* TIMER预置值 */
#define FM1702SL_REG_TypeSH				0x31	/* 认证模式选择 */
#define FM1702SL_REG_TestDigiSelect		0x3d	/* Test pin configure register */

#define FM1702_OK		    0
/************************* 函数错误代码定义 ERR CODE  *************************/
#define FM1702_NOTAGERR		1		/* 无卡 */
#define FM1702_CRCERR		2		/* 卡片CRC校验错误 */
#define FM1702_EMPTY		3		/* 数值溢出错误 */
#define FM1702_AUTHERR		4		/* 验证不成功 */
#define FM1702_PARITYERR	5		/* 卡片奇偶校验错误 */
#define FM1702_CODEERR		6		/* 通讯错误(BCC校验错) */
#define FM1702_SERNRERR		8		/* 卡片序列号错误(anti-collision 错误) */
#define FM1702_SELECTERR	9		/* 卡片数据长度字节错误(SELECT错误) */
#define FM1702_NOTAUTHERR	10		/* 卡片没有通过验证 */
#define FM1702_BITCOUNTERR	11		/* 从卡片接收到的位数错误 */
#define FM1702_BYTECOUNTERR	12		/* 从卡片接收到的字节数错误仅读函数有效 */
#define FM1702_RESTERR		13		/* 调用restore函数出错 */
#define FM1702_TRANSERR		14		/* 调用transfer函数出错 */
#define FM1702_WRITEERR		15		/* 调用write函数出错 */
#define FM1702_INCRERR		16		/* 调用increment函数出错 */
#define FM1702_DECRERR		17		/* 调用decrement函数出错 */
#define FM1702_READERR		18		/* 调用read函数出错 */
#define FM1702_LOADKEYERR	19		/* 调用LOADKEY函数出错 */
#define FM1702_FRAMINGERR	20		/* FM1702帧错误 */
#define FM1702_REQERR		21		/* 调用req函数出错 */
#define FM1702_SELERR		22		/* 调用sel函数出错 */

#define FM1702_ANTICOLLERR	23		/* 24 调用anticoll函数出错 */
#define FM1702_INTIVALERR	24		/* 25 调用初始化函数出错 */
#define FM1702_READVALERR	25		/* 26 调用高级读块值函数出错 */
#define FM1702_DESELECTERR	26		/* 27 */
#define FM1702_CMD_ERR		42		/* 28 命令错误 */

/************************* 射频卡通信命令码定义 *************************/
#define RF_CMD_REQUEST_STD	0x26	//连续性读卡指令
#define RF_CMD_REQUEST_ALL	0x52	//全自动读卡指令
#define RF_CMD_ANTICOL		0x93	//防重叠
#define RF_CMD_SELECT		0x93	//选卡片
#define RF_CMD_AUTH_LA		0x60	//选择 Key_A
#define RF_CMD_AUTH_LB		0x61	//选择 Key_B
#define RF_CMD_READ		    0x30	//读
#define RF_CMD_WRITE		0xa0	//写
#define RF_CMD_INC		    0xc1	//增值
#define RF_CMD_DEC		    0xc0	//减值
#define RF_CMD_RESTORE		0xc2	//重储
#define RF_CMD_TRANSFER		0xb0	//传送
#define RF_CMD_HALT		    0x50	//停机

/************************* 卡片类型定义定义 ********************************/
#define TYPEA_MODE			0		//type A mode	
#define AuthKey_A			0x60
#define AuthKey_B			0x61

#define TM0_HIGH			0xf0	//timer0 high bit,4ms setting
#define TM0_LOW				0x60    //timer0 low bit
#define TIMEOUT				(150) 	//Timer out ,4ms x150 =0.6s
#define RF_TRY_COUNT        (3)		//读写卡操作尝试次数

#define CARD_MAX_SECTION    (16)	//MF1卡扇区的数量
#define CARD_MAX_BLOCK      (64)	//MF1卡数据块的数量



//************************======function declare========************************
extern uchar8	g_Card_RevBuffer[40];			// command send/receive buffer
extern uchar8   g_Card_tagtype[2];				// 卡片标识字符 
extern uchar8	g_Card_UID[5];					// 存放物理卡号

extern uchar8	RF_Init(void);
extern uchar8	MIF_Request(uchar8 mode);
extern uchar8	MIF_AntiColl(void);
extern uchar8 MIF_Select(void);
extern uchar8 MIF_Load_Key(uchar8 *keybuf);
extern uchar8 MIF_Authentication(uchar8 *pCardUID, uchar8 SecNR, uchar8 keyType);
extern uchar8 MIF_Read(uchar8 *databuf, uchar8 Block_Adr);
extern uchar8 MIF_Write(uchar8 *databuf, uchar8 Block_Adr);
extern uchar8 MIF_Halt(void);
extern uchar8 RF_ActiveCard(uchar8* pUID, uint8 u8Section, uchar8 u8KeyType, uchar8* pKey);
extern uchar8	RF_Find_Card(void);
extern uchar8	RF_Read_CardUID(uchar8* pUID);
extern uint32	RF_GetCardUID(void);
extern uint8 RF_ReadCard_1Block(uchar8* pUID, uint8 u8Block, uchar8 u8KeyType, uchar8* pKey, uchar8* pData);
extern uint8 RF_WriteCard_1Block(uchar8* pUID, uint8 u8Block, uchar8 u8KeyType, uchar8* pKey, uchar8* pData);
extern uint8 RF_ReadCard(uchar8* pUID, uint8 sector, uchar8 u8KeyType, uchar8* pKey, uchar8* pData);
extern uint8 RF_WriteCard(uchar8* pUID, uint8 sector, uchar8 u8KeyType, uchar8* pKey, uchar8* pData);
extern uint8 RF_BlockValueIncrement(uchar8* pUID, uint8 u8Block, uchar8 u8KeyType, uchar8* pKey, uchar8* pData);
extern uint8 RF_BlockValueDecrement(uchar8* pUID, uint8 u8Block, uchar8 u8KeyType, uchar8* pKey, uchar8* pData);
extern uint8 RF_BlockRestore(uchar8* pUID, uint8 u8Block, uchar8 u8KeyType, uchar8* pKey);

#endif
/*========================File end===================================================================================================*/

