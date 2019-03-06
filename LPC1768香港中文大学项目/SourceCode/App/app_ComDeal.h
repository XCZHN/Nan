/*===============================================================================================
==                                                                                              
**  Copyright(c) 2014  Intelligent Admin electronic systems co., LTD. All rights reserved.
==                                                                                           
**	Author:		Asen                                                                           
==                                                                                          
**  Version:	v1.0.0                                                                          
==
**  Filename:	ComDeal.h                                                                       
==                                                                                         
**  Function:	串口通信应用层文件  
==
**  Data:       2014-08-18
================================================================================================*/

#ifndef	_APP_COM_DEAL_H_
#define	_APP_COM_DEAL_H_

#include "Config.h"   


//===============================命令字==========================================================
#define	CMD_GET_VERSION					0x01			//读取版本号    
#define	CMD_SET_REST					0x02	   		//远程复位      
#define	CMD_SET_NUM						0x03			//设置机号              
#define	CMD_GET_NUM						0x04			//读取机号(方便网络通信时PC获取未知机号)              
#define	CMD_SET_TIME					0x05        	//设置系统时间              
#define	CMD_GET_TIME					0x06        	//读取系统时间    
#define	CMD_PWR_OFF					    0x07	   		//远程关机     
#define	CMD_SYS_INIT					0x08	   		//系统初始化    
#define	CMD_SET_NET_SWITCH              0x09            //设置网络开关
#define	CMD_SET_IP                      0x0A            //设置IP地址
#define	CMD_GET_IP                      0x0B            //读取IP地址
#define	CMD_SET_SERVER_IPINFO           0x0C            //设置服务器IP地址及端口号
#define	CMD_GET_SERVER_IPINFO           0x0D            //读取服务器IP地址及端口号
#define	CMD_RING_BUZZER					0x10			//响蜂鸣器
#define	CMD_EEPROM_WRITE                0x25            //往EEPROM写数据
#define	CMD_EEPROM_READ                 0x26            //从FLASH读数据 

#define	CMD_FIND_CARD					0x2A			//寻卡			
#define	CMD_SET_CARD_KEY                0x2D            //设置卡密钥
#define	CMD_GET_CARD_KEY                0x2E            //读取卡密钥

#define	CMD_PLAY_VOICE				    0x51			//播放指定语音
#define	CMD_SET_VOICE_LEVEL			    0x53			//设置音量等级
#define	CMD_VOICE_CMD_TEST			    0x54			//语言模块指令测试

#define	CMD_SET_BOX_NUM                 0xA0            //设置包箱数量
#define	CMD_OPEN_BOX_RETURN_STATUS      0xA1            //开箱并且返回状态
#define	CMD_OPEN_BOX                    0xA2            //开箱(不返回状态)
#define	CMD_GET_BOXDOOR_STATE           0xA3		    //读取箱门状态
#define	CMD_GET_BOX_STATE               0xA4		    //读取包箱状态
#define	CMD_SET_BOX_INFO				0xA5            //设置指定箱号信息
#define	CMD_GET_BOX_INFO				0xA6			//读取指定箱号信息
#define	CMD_SET_LOCKERS                 0xAA            //设置系统工作模式
#define	CMD_SET_VALID_CARD_LIST			0xAB            //设置预授权卡号表
#define	CMD_GET_VALID_CARD_LIST			0xAC			//读取预授权卡号表

#define	CMD_SET_BoxUserCard             0xB1            //设置用户卡
#define	CMD_GET_BoxUserCard             0xB2            //读取用户卡
#define	CMD_GET_AllBoxCard              0xB4            //读取多个箱的卡号
#define	CMD_LockBox                     0xB5            //锁箱
#define	CMD_UnlockBox                   0xB6            //解锁箱
#define	CMD_SET_Admin                   0xB8            //设置管理员账号和密码
#define	CMD_GET_RecordPoint             0xB9            //读取记录指针
#define	CMD_GET_RECORD                  0xBA            //采集记录
#define	CMD_GET_RECORD_AGAIN            0xBB            //补采集记录
#define	CMD_CLR_AllRecord               0xBC            //清除记录

#define	CMD_ADD_Fingerprint             0xD1            //加载指纹数据
#define	CMD_CLR_Fingerprint             0xD2            //删除指纹数据
#define	CMD_GET_Fingerprint             0xD3            //读取指纹数据

#define	CMD_USER_ASK_CONSIGN            0xF1            //用户请求寄存
#define	CMD_USER_ASK_TAKEAWAY           0xF2            //用户请求取件

//----------------------执行错误表------------------------------------------- 
#define Err_EmptyCmd					0xEF			//非有效指令，不回复
#define Err_DataLen					    0x0E			//数据长度错误
#define Err_Data						0xE0			//按制字不对
#define Err_Time						0xE1			//时间日期格式错误。
#define Err_Eeprom						0xE4			//EEPROM保存记录失败
#define Err_FlashErase                  0xE5            //Flash擦除失败
#define Err_FlashWrite                  0xE6            //Flash写失败
#define Err_FlashRead                   0xE7            //Flash读失败
#define Err_EepromWrite                 0xE8            //Eeprom写失败
#define Err_EepromRead                  0xE9            //Eeprom读失败
#define Err_UpdateVoiceFile             0xEA            //更新语音文件失败
#define Err_BoxInfo						0xEB			//箱号信息错误
#define Err_FindCard					0xF0			//寻卡出错
#define Err_ReadCard					0xF1		    //读卡出错
#define Err_WriteCard					0xF2			//写卡出错
#define Err_NoRecord                    0xF5            //无此记录  



/******************************************************************************************************
* Function Name  : Thread_CommHandle
* Description    : 处理接收到的数据，验证机型机号通过后调用命令执行
* Input          : None
* Variable       : g_stCmdPacket -- 通讯指令数据包
* Output         : None
* Return         : None
******************************************************************************************************/
extern void Thread_CommHandle(void);



#endif
/*========================File end=======================================================*/

