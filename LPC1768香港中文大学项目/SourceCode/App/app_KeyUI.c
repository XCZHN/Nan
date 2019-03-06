/*===============================================================================================
==                                                                                              
**  Copyright(c) 2014  ZhenYao Science and technology co., LTD.   All rights reserved.
==                                                                                           
**	Author:		Asen                                                                     
==                                                                                          
**  Version:	v2.0.0                                                                          
==
**  Filename:	App_KeyInterface.c                                                                       
==                                                                                         
**  Function:	键盘操作界面程序
==
**  Data:       2014/08/11                                                                       
================================================================================================*/

#include "Includes.h"
//#include "App_GUI.h"

volatile ST_KEYBOARD gstKeyboard;
volatile ST_TIME stTime;

/***********************************************************************************************
* Function Name  : NumKeyToASCII
* Description	 : Key 0-9 数字键标志转换成对应的ASCII码
* Input 		 : 按键值代号
* Output		 : None
* Return		 : 按键对ASCII值
***********************************************************************************************/
uchar8  NumKeyToASCII(uchar8 u8KeyValue)
{	
	switch(u8KeyValue)
	{		
		case Key_1: return 0x41;  //"A"
		case Key_2: return 0x42;  //"B"
		case Key_3: return 0x43;  //"C"
		case Key_4: return 0x44;  //"D"
		case Key_5: return 0x45;  //"E"
		case Key_6: return 0x46;  //"F"
		case Key_7: return 0x47;  //"G"
		case Key_8: return 0x48;  //"H"
		case Key_9: return 0x49;  //"I"
		case Key_0: return 0x4A;  //"J"
			
		case Key_Star: return 0x4B; //"K"	 
		case Key_Jin:  return 0x4C; //"L"	
	}
	return 0;
}

/***********************************************************************************************
* Function Name  : KeyChangeNumValue
* Description	 : 按键改变一个数值
* Input 		 : u32ObjNum -- 目标值, u8KeyValue -- 当前键值, u32MaxNewValue -- 新值得上限
* Output		 : None
* Return		 : 新的数值 
* Others		 : None
***********************************************************************************************/
uint32 KeyChangeNumValue(uint32 u32ObjNum, uchar8 u8KeyValue, uint32 u32MaxNewValue)
{
	uint32 u32Value = u32ObjNum;

	switch(u8KeyValue)
	{		
		case Key_0:
		case Key_1:
		case Key_2:
		case Key_3:
		case Key_4:
		case Key_5:
		case Key_6:
		case Key_7:
		case Key_8:
		case Key_9:
		{
			u8KeyValue = NumKeyToNumValue(u8KeyValue);
			u32Value = u32Value * 10;
			u32Value += u8KeyValue;
			if(u32Value > u32MaxNewValue)
			{
				u32Value = u8KeyValue;
			}
		}
		break;

		case Key_Star:   //用作回删键
		case Key_Jin:
		case Key_Back:
		{
			u32Value = u32Value / 10;
		}
		break;
	}
	return u32Value;
}

/***********************************************************************************************
* Function Name  : KeyInputIpAddr
* Description	 : 按键输入IP字符串
* Input 		 : u8KeyValue -- 当前键值, pIP -- IP数组, pIpStrBuf --IP字符串存储区指针
* Output		 : None
* Return		 : 输入的IP字符串长度
* Others		 : None
***********************************************************************************************/
uchar8 KeyInputIpAddr(uchar8 u8KeyValue, uchar8* pIP, uchar8* pIpStrBuf)
{
	uchar8 tempbuf[8] = {0};
	uchar8 IpStrLen = strlen((char*)pIpStrBuf);
	uchar8 PointNum = 0;
	uchar8 NewLen = 0;
	int i = 0;
	for(i=0; i<IpStrLen; i++)
	{
		if('.' == pIpStrBuf[i])
		{
			PointNum ++;
		}
	}

	switch(PointNum)
	{
		case 0: //输入IP的第一段
		{
			if((Key_0 <= gstKeyboard.u8KeyValue)&&(Key_9 >= gstKeyboard.u8KeyValue))
			{
				pIP[0] = KeyChangeNumValue(pIP[0], gstKeyboard.u8KeyValue, 255);

				memset(pIpStrBuf, 0x0, IpStrLen);
				IpStrLen = UInt_To_Ascii(pIP[0], pIpStrBuf);	
				if(3 == IpStrLen)
				{
					pIpStrBuf[IpStrLen++] = '.';	
				}
			}
			
			if((Key_Star == gstKeyboard.u8KeyValue)&&('.' != pIpStrBuf[IpStrLen -1]))
			{
				pIpStrBuf[IpStrLen++] = '.';	
			}
			else if(Key_Jin == gstKeyboard.u8KeyValue) //回删一字节
			{
				if(IpStrLen)
				{
					pIP[0] = KeyChangeNumValue(pIP[0], Key_Back, 255);
					memset(pIpStrBuf, 0x0, IpStrLen);
					IpStrLen = UInt_To_Ascii(pIP[0], pIpStrBuf);	
				}
			}
		}
		break;

		case 1: //输入IP的第2段
		{
			if((Key_0 <= gstKeyboard.u8KeyValue)&&(Key_9 >= gstKeyboard.u8KeyValue))
			{
				pIP[1] = KeyChangeNumValue(pIP[1], gstKeyboard.u8KeyValue, 255);

				memset(pIpStrBuf, 0x0, IpStrLen);
				IpStrLen = UInt_To_Ascii(pIP[0], pIpStrBuf);
				pIpStrBuf[IpStrLen++] = '.';
				NewLen = UInt_To_Ascii(pIP[1], tempbuf);
				strcat((char*)pIpStrBuf, (char*)tempbuf);
				IpStrLen += NewLen;				
				if(3 == NewLen)
				{
					pIpStrBuf[IpStrLen++] = '.';	
				}
			}

			if((Key_Star == gstKeyboard.u8KeyValue)&&('.' != pIpStrBuf[IpStrLen -1]))
			{
				pIpStrBuf[IpStrLen++] = '.';	
			}
			else if(Key_Jin == gstKeyboard.u8KeyValue) //回删一字节
			{
				if((Key_0 <= pIpStrBuf[IpStrLen-1])&&(Key_9 >= pIpStrBuf[IpStrLen-1])) //判断回删的是数字还是"."
				{
					pIP[1] = KeyChangeNumValue(pIP[1], Key_Back, 255);

					//重新生成IP字符串					
					memset(pIpStrBuf, 0x0, IpStrLen);
					IpStrLen = UInt_To_Ascii(pIP[0], pIpStrBuf);
					pIpStrBuf[IpStrLen++] = '.';
					if(pIP[1])  //如果pIP[1]=0则说明此段IP已删除完
					{

						NewLen = UInt_To_Ascii(pIP[1], tempbuf);
						strcat((char*)pIpStrBuf, (char*)tempbuf);	
						IpStrLen += NewLen;
					}
				}
				else
				{
					pIP[1] = 0;
					pIpStrBuf[IpStrLen -1] = 0x0;
					IpStrLen --;
				}
			}
		}
		break;

		case 2: //输入IP的第3段
		{
			if((Key_0 <= gstKeyboard.u8KeyValue)&&(Key_9 >= gstKeyboard.u8KeyValue))
			{
				pIP[2] = KeyChangeNumValue(pIP[2], gstKeyboard.u8KeyValue, 255);

				//重新生成IP字符串
				memset(pIpStrBuf, 0x0, IpStrLen);
				IpStrLen = UInt_To_Ascii(pIP[0], pIpStrBuf);
				pIpStrBuf[IpStrLen++] = '.';
				NewLen = UInt_To_Ascii(pIP[1], tempbuf);
				strcat((char*)pIpStrBuf, (char*)tempbuf);	
				IpStrLen += NewLen;
				pIpStrBuf[IpStrLen++] = '.';
				memset(tempbuf, 0x0, sizeof(tempbuf));
				NewLen = UInt_To_Ascii(pIP[2], tempbuf);
				strcat((char*)pIpStrBuf, (char*)tempbuf);
				IpStrLen += NewLen; 	
				if(3 == NewLen)
				{
					pIpStrBuf[IpStrLen++] = '.';	
				}
			}

			if((Key_Star == gstKeyboard.u8KeyValue)&&('.' != pIpStrBuf[IpStrLen -1]))
			{
				pIpStrBuf[IpStrLen++] = '.';	
			}
			else if(Key_Jin == gstKeyboard.u8KeyValue) //回删一字节
			{
				if((Key_0 <= pIpStrBuf[IpStrLen-1])&&(Key_9 >= pIpStrBuf[IpStrLen-1])) //判断回删的是数字还是"."
				{
					pIP[2] = KeyChangeNumValue(pIP[2], Key_Back, 255);

					//重新生成IP字符串
					memset(pIpStrBuf, 0x0, IpStrLen);
					IpStrLen = UInt_To_Ascii(pIP[0], pIpStrBuf);
					pIpStrBuf[IpStrLen++] = '.';
					NewLen = UInt_To_Ascii(pIP[1], tempbuf);
					strcat((char*)pIpStrBuf, (char*)tempbuf);	
					IpStrLen += NewLen;
					pIpStrBuf[IpStrLen++] = '.';
					if(pIP[2])  //如果pIP[x]=0则说明此段IP已删除完
					{
						memset(tempbuf, 0x0, sizeof(tempbuf));
						NewLen = UInt_To_Ascii(pIP[2], tempbuf);
						strcat((char*)pIpStrBuf, (char*)tempbuf);
						IpStrLen += NewLen; 			
					}
				}				
				else
				{
					pIP[2] = 0;
					pIpStrBuf[IpStrLen -1] = 0x0;
					IpStrLen --;
				}
			}
		}
		break;

		case 3: //输入IP的第4段
		{
			if(Key_Jin == gstKeyboard.u8KeyValue) //回删一字节
			{
				if((Key_0 <= pIpStrBuf[IpStrLen-1])&&(Key_9 >= pIpStrBuf[IpStrLen-1])) //判断回删的是数字还是"."
				{
					pIP[3] = KeyChangeNumValue(pIP[3], Key_Back, 255);

					//重新生成IP字符串
					memset(pIpStrBuf, 0x0, IpStrLen);
					IpStrLen = UInt_To_Ascii(pIP[0], pIpStrBuf);
					pIpStrBuf[IpStrLen++] = '.';
					NewLen = UInt_To_Ascii(pIP[1], tempbuf);
					strcat((char*)pIpStrBuf, (char*)tempbuf);	
					IpStrLen += NewLen;
					pIpStrBuf[IpStrLen++] = '.';
					memset(tempbuf, 0x0, sizeof(tempbuf));
					NewLen = UInt_To_Ascii(pIP[2], tempbuf);
					strcat((char*)pIpStrBuf, (char*)tempbuf);
					IpStrLen += NewLen;					
					pIpStrBuf[IpStrLen++] = '.';
					if(pIP[3])  //如果pIP[x]=0则说明此段IP已删除完
					{
						memset(tempbuf, 0x0, sizeof(tempbuf));
						NewLen = UInt_To_Ascii(pIP[3], tempbuf);
						strcat((char*)pIpStrBuf, (char*)tempbuf);
						IpStrLen += NewLen; 			
					}
				}
				else
				{
					pIP[3] = 0;
					pIpStrBuf[IpStrLen -1] = 0x0;
					IpStrLen --;
				}
			}
			else
			{
				NewLen = UInt_To_Ascii(pIP[3], tempbuf);
				if(NewLen < 3)
				{
					if((Key_0 <= gstKeyboard.u8KeyValue)&&(Key_9 >= gstKeyboard.u8KeyValue))
					{
						pIP[3] = KeyChangeNumValue(pIP[3], gstKeyboard.u8KeyValue, 255);

						//重新生成IP字符串
						memset(pIpStrBuf, 0x0, IpStrLen);
						IpStrLen = UInt_To_Ascii(pIP[0], pIpStrBuf);
						pIpStrBuf[IpStrLen++] = '.';
						NewLen = UInt_To_Ascii(pIP[1], tempbuf);
						strcat((char*)pIpStrBuf, (char*)tempbuf);	
						IpStrLen += NewLen;
						pIpStrBuf[IpStrLen++] = '.';
						memset(tempbuf, 0x0, sizeof(tempbuf));
						NewLen = UInt_To_Ascii(pIP[2], tempbuf);
						strcat((char*)pIpStrBuf, (char*)tempbuf);
						IpStrLen += NewLen;					
						pIpStrBuf[IpStrLen++] = '.';
						memset(tempbuf, 0x0, sizeof(tempbuf));
						NewLen = UInt_To_Ascii(pIP[3], tempbuf);
						strcat((char*)pIpStrBuf, (char*)tempbuf);
						IpStrLen += NewLen; 	
					}
				}
			}			
		}
		break;

		default:
			break;
	}

	return IpStrLen;
}

/***********************************************************************************************
* Function Name  : KeyInputDate
* Description	 : 按键输入日期
* Input 		 : Key--按键值
* Output		 : None
* Return		 : None
* Others		 : None
***********************************************************************************************/
uchar8 KeyInputDate(uchar8 Key)
{
	switch(gstKeyboard.u8Select)
	{ 
		case 0: //提示输入
		{
			gstKeyboard.bAddInto = 0;
			gstKeyboard.u8KeyCount = 0;
			memset((uchar8*)&stTime, 0x0, sizeof(stTime));
			memset((uchar8*)gstKeyboard.tempBuf, 0x0, sizeof(gstKeyboard.tempBuf));  
			strcpy((char*)gstKeyboard.tempBuf, "20_");
			GUI_InputUseDate(gstKeyboard.u8Box, (uchar8*)gstKeyboard.tempBuf);
			gstKeyboard.u8Select ++;
		}
		break;	

		case 1: //输入年
		{
			if((Key >= Key_0)&&(Key <= Key_9))
			{
				stTime.u8Year = KeyChangeNumValue(stTime.u8Year, Key, 99); 	
				gstKeyboard.u8KeyCount ++;
				if(2 == gstKeyboard.u8KeyCount)
				{
					sprintf((char*)gstKeyboard.tempBuf, "20%02d-", stTime.u8Year);
					gstKeyboard.u8KeyCount = 0;
					gstKeyboard.u8Select ++;
				}
				else
				{
					sprintf((char*)gstKeyboard.tempBuf, "20%d_", stTime.u8Year);
				}
				GUI_InputUseDate(gstKeyboard.u8Box, (uchar8*)gstKeyboard.tempBuf);
			}
		}
		break;

		case 2: //输入月
		{
			if((Key >= Key_0)&&(Key <= Key_9))
			{
				stTime.u8Month = KeyChangeNumValue(stTime.u8Month, Key, 12); 			
				gstKeyboard.u8KeyCount ++;
				if(2 == gstKeyboard.u8KeyCount)
				{
					sprintf((char*)gstKeyboard.tempBuf, "20%02d-%02d-", stTime.u8Year, stTime.u8Month);
					gstKeyboard.u8KeyCount = 0;
					gstKeyboard.u8Select ++;
				}
				else
				{
					sprintf((char*)gstKeyboard.tempBuf, "20%02d-%d_", stTime.u8Year, stTime.u8Month);
				}
				GUI_InputUseDate(gstKeyboard.u8Box, (uchar8*)gstKeyboard.tempBuf);
			}
		}
		break;

		case 3: //输入日
		{
			if((Key >= Key_0)&&(Key <= Key_9))
			{
				stTime.u8Day = KeyChangeNumValue(stTime.u8Day, Key, 31); 
				gstKeyboard.u8KeyCount ++;
				if(1 == gstKeyboard.u8KeyCount)
				{
					sprintf((char*)gstKeyboard.tempBuf, "20%02d-%02d-%d_", stTime.u8Year, stTime.u8Month, stTime.u8Day);
				}
				else
				{
					sprintf((char*)gstKeyboard.tempBuf, "20%02d-%02d-%02d", stTime.u8Year, stTime.u8Month, stTime.u8Day);
				}
				GUI_InputUseDate(gstKeyboard.u8Box, (uchar8*)gstKeyboard.tempBuf);
			}
			else if(Key_Enter == Key)
			{
				gstKeyboard.u8Select = 0;
			    return 0xFF;  //输入完成
			}
		}
		break;

		default:
			break;
	}

	return gstKeyboard.u8Select;
}

/***********************************************************************************************
* Function Name  : Set_KeyboardType
* Description	 : 设置键盘类型
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : 主菜单-->系统-->键盘-->键盘类型设置(属于第四级菜单)
***********************************************************************************************/
void Set_KeyboardType(void)
{
	if(Key_Cancel == gstKeyboard.u8KeyValue)
	{	
		gstKeyboard.u8FourMenuStep = 0;
		gstKeyboard.u8ThreeMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8FourMenuStep)
		{
			case 0: //显示选择界面
			{
				gstKeyboard.bAddInto = 0;
				GUI_ChoiceKeyboardType();
				gstKeyboard.u8FourMenuStep ++;
			}
			break;	

			case 1: //选择(选择:1触摸键盘, 2金属键盘)
			{
				if((gstKeyboard.u8NumValue == 1)||(gstKeyboard.u8NumValue == 2))
				{
					if(1 == gstKeyboard.u8NumValue)
					{
						SysConfig.stHardware.u8KeyType = 0;
					}
					else
					{
						SysConfig.stHardware.u8KeyType = 1;
					}
					Store_Parameters(Addr_Hardware, (uchar8*)&SysConfig.stHardware, sizeof(ST_SYS_HW));
					//提示设置成功
					GUI_ShowNotice(NOTICE_SET_OK);
					Buzzer(600);
					delay_ms(1234); 				
					gstKeyboard.u8FourMenuStep = 0;
					gstKeyboard.u8ThreeMenuStep = 0;
					gstKeyboard.bAddInto = 1;
				}
			}
			break;
			
			default:
			{
				gstKeyboard.u8FourMenuStep = 0;
			}
			break;
		}
	}
}

/***********************************************************************************************
* Function Name  : Set_SysConfigInit
* Description	 : 恢复出厂设置
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : 主菜单-->系统-->初始化-->初始化确认(属于第四级菜单)
***********************************************************************************************/
void Set_SysConfigInit(void)
{
	if(Key_Cancel == gstKeyboard.u8KeyValue)
	{	
		gstKeyboard.u8FourMenuStep = 0;
		gstKeyboard.u8ThreeMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8FourMenuStep)
		{
			case 0: //显示选择界面
			{
				gstKeyboard.bAddInto = 0;
				GUI_ShowNotice(NOTICE_SYS_INIT_ASK);
				gstKeyboard.u8FourMenuStep ++;
			}
			break;	

			case 1: //初始化确认
			{
				if(Key_Enter == gstKeyboard.u8KeyValue)
				{
					GUI_ShowNotice(NOTICE_SYS_INIT_WAIT);
					SysConfig_Init();
					//提示设置成功
					GUI_ShowNotice(NOTICE_SYS_INIT_OK);
					delay_ms(1234); 				
					gstKeyboard.u8FourMenuStep = 0;
					gstKeyboard.u8ThreeMenuStep = 0;
					gstKeyboard.bAddInto = 1;
				}
			}
			break;
			
			default:
			{
				gstKeyboard.u8FourMenuStep = 0;
			}
			break;
		}
	}
}

/***********************************************************************************************
* Function Name  : Set_BoxNum
* Description	 : 设置箱数
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : 主菜单-->系统-->箱数-->箱数设置(属于第四级菜单)
***********************************************************************************************/
void Set_BoxNum(void)
{
	uint32 iAddr = 0;
	
	if(Key_Cancel == gstKeyboard.u8KeyValue)
	{	
		if(1 == gstKeyboard.u8FourMenuStep)
		{
			gstKeyboard.u8ThreeMenuStep = 0;
		}
		gstKeyboard.u8FourMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8FourMenuStep)
		{
			case 0: //显示选择界面
			{
				gstKeyboard.bAddInto = 0;
				gstKeyboard.u8ValueType = 0;
				gstKeyboard.u8Box = 0;
				GUI_AdminSetBoxNum(gstKeyboard.u8ValueType, gstKeyboard.u8Box);
				gstKeyboard.u8FourMenuStep ++;
			}
			break;	

			case 1: //选择(1.主柜, 2.副柜)
			{				
				if(1 == gstKeyboard.u8NumValue)
				{
					gstKeyboard.u8ValueType = 1;
					gstKeyboard.u8Box = SysConfig.stLockers.u8HostLockerBoxs;
					GUI_AdminSetBoxNum(gstKeyboard.u8ValueType, gstKeyboard.u8Box);
					gstKeyboard.u8ValueType = gstKeyboard.u8NumValue;
					gstKeyboard.u8FourMenuStep = 3;
				}
				else if(2 == gstKeyboard.u8NumValue)
				{
					gstKeyboard.u8ValueType = 2;
					gstKeyboard.u8Box = 0;
					GUI_AdminSetBoxNum(gstKeyboard.u8ValueType, gstKeyboard.u8Box);
					gstKeyboard.u8FourMenuStep ++;
				}
			}
			break;

			case 2: //选择副柜号
			{
				if((gstKeyboard.u8NumValue >= 1)&&(gstKeyboard.u8NumValue <= 3))
				{
					gstKeyboard.u8ValueType = 2 + gstKeyboard.u8NumValue;
					if(3 == gstKeyboard.u8ValueType)
					{
						gstKeyboard.u8Box = SysConfig.stLockers.u8ExtL01BoxNum;
					}
					else if(4 == gstKeyboard.u8ValueType)
					{
						gstKeyboard.u8Box = SysConfig.stLockers.u8ExtL02BoxNum;
					}
					else if(5 == gstKeyboard.u8ValueType)
					{
						gstKeyboard.u8Box = SysConfig.stLockers.u8ExtL03BoxNum;
					}
					else
					{
						gstKeyboard.u8Box = 0;
					}
					GUI_AdminSetBoxNum(gstKeyboard.u8ValueType, gstKeyboard.u8Box);
					gstKeyboard.u8FourMenuStep ++;
				}			
			}
			break;
		
			case 3: //设置箱数
			{
				if(Key_Enter == gstKeyboard.u8KeyValue)
				{
					if(1 == gstKeyboard.u8ValueType)
					{
						SysConfig.stLockers.u8HostLockerBoxs = gstKeyboard.u8Box;
						iAddr = Addr_SysConfig + ((uint32)&SysConfig.stLockers.u8HostLockerBoxs - (uint32)&SysConfig);
						Store_Parameters(iAddr, (uchar8*)&SysConfig.stLockers.u8HostLockerBoxs, 1);
					}
					else if(3 == gstKeyboard.u8ValueType)
					{
						SysConfig.stLockers.u8ExtL01BoxNum = gstKeyboard.u8Box;
						iAddr = Addr_SysConfig + ((uint32)&SysConfig.stLockers.u8ExtL01BoxNum - (uint32)&SysConfig);
						Store_Parameters(iAddr, (uchar8*)&SysConfig.stLockers.u8ExtL01BoxNum, 1);
					}
					else if(4 == gstKeyboard.u8ValueType)
					{
						SysConfig.stLockers.u8ExtL02BoxNum = gstKeyboard.u8Box;
						iAddr = Addr_SysConfig + ((uint32)&SysConfig.stLockers.u8ExtL02BoxNum - (uint32)&SysConfig);
						Store_Parameters(iAddr, (uchar8*)&SysConfig.stLockers.u8ExtL02BoxNum, 1);
					}
					else if(5 == gstKeyboard.u8ValueType)
					{
						SysConfig.stLockers.u8ExtL03BoxNum = gstKeyboard.u8Box;
						iAddr = Addr_SysConfig + ((uint32)&SysConfig.stLockers.u8ExtL03BoxNum - (uint32)&SysConfig);
						Store_Parameters(iAddr, (uchar8*)&SysConfig.stLockers.u8ExtL03BoxNum, 1);
					}

					//计算总箱数并保存参数
  					SysConfig.stLockers.u8BoxCount = SysConfig.stLockers.u8HostLockerBoxs 
						                            + SysConfig.stLockers.u8ExtL01BoxNum 
						                            + SysConfig.stLockers.u8ExtL02BoxNum
						                            + SysConfig.stLockers.u8ExtL03BoxNum;
  					iAddr = Addr_SysConfig + ((uint32)&SysConfig.stLockers.u8BoxCount - (uint32)&SysConfig);
					Store_Parameters(iAddr, (uchar8*)&SysConfig.stLockers.u8BoxCount, 1);

					//提示设置成功
					GUI_ShowNotice(NOTICE_SET_OK);
					Buzzer(600);
					delay_ms(1234);					
					gstKeyboard.u8FourMenuStep = 0;
					gstKeyboard.bAddInto = 1;

					//将所有箱号装入空箱列表
					UpdateEmptyBoxList();					
				}
				else if((Key_0 <= gstKeyboard.u8KeyValue)&&(Key_9 >= gstKeyboard.u8KeyValue))
				{
					gstKeyboard.u8Box = KeyChangeNumValue(gstKeyboard.u8Box, gstKeyboard.u8KeyValue, MAX_BOX_NUM);
					GUI_AdminSetBoxNum(gstKeyboard.u8ValueType, gstKeyboard.u8Box);
				}
			}
			break;
			
			default:
			{
				gstKeyboard.u8FourMenuStep = 0;
			}
			break;
		}
	}
}

/***********************************************************************************************
* Function Name  : Set_DateTime
* Description	 : 设置时间
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : 主菜单-->系统-->时钟-->时钟设置(属于第4级菜单)
***********************************************************************************************/
void Set_DateTime(void)
{	
	int iTaday = 0;
	int iInputDate = 0;

	if(Key_Cancel == gstKeyboard.u8KeyValue)
	{
		gstKeyboard.u8FourMenuStep = 0;
		gstKeyboard.u8ThreeMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8FourMenuStep)
		{
			case 0: //提示输入日期
			{
				gstKeyboard.bAddInto = 0;
				gstKeyboard.u8KeyCount = 0;
				gstKeyboard.u8TempValue = 0;
				memset((uchar8*)&stTime, 0x0, sizeof(stTime));
				memset((uchar8*)gstKeyboard.KeyBuf, 0x0, sizeof(gstKeyboard.KeyBuf));   //用于存储年月日字符串
				memset((uchar8*)gstKeyboard.tempBuf, 0x0, sizeof(gstKeyboard.tempBuf)); //用于存储时分秒字符串
				strcpy((char*)gstKeyboard.KeyBuf, "20");
				GUI_SetTime((uchar8*)gstKeyboard.KeyBuf, (uchar8*)gstKeyboard.tempBuf);
				gstKeyboard.u8FourMenuStep ++;
			}
			break;	

			case 1: //输入年份
			{
				if((Key_0 <= gstKeyboard.u8KeyValue)&&(Key_9 >= gstKeyboard.u8KeyValue))
				{						
					gstKeyboard.u8KeyCount ++;
					if(1 == gstKeyboard.u8KeyCount)
					{
						gstKeyboard.u8TempValue = KeyChangeNumValue(gstKeyboard.u8TempValue, gstKeyboard.u8KeyValue, 99);
						gstKeyboard.KeyBuf[2] = gstKeyboard.u8NumValue + 0x30;
					}
					else
					{
						gstKeyboard.u8TempValue = KeyChangeNumValue(gstKeyboard.u8TempValue, gstKeyboard.u8KeyValue, 99);
						stTime.u8Year = gstKeyboard.u8TempValue;
						sprintf((char*)gstKeyboard.KeyBuf, "20%02d-", stTime.u8Year);
						gstKeyboard.u8TempValue = 0;
						gstKeyboard.u8FourMenuStep ++;
					}
					GUI_SetTime((uchar8*)gstKeyboard.KeyBuf, (uchar8*)gstKeyboard.tempBuf);
				}
			}
			break;
			
			case 2: //输入月
			{				
				if((Key_0 <= gstKeyboard.u8KeyValue)&&(Key_9 >= gstKeyboard.u8KeyValue))
				{
					gstKeyboard.u8KeyCount ++;
					if(3 == gstKeyboard.u8KeyCount)
					{
						gstKeyboard.u8TempValue = KeyChangeNumValue(gstKeyboard.u8TempValue, gstKeyboard.u8KeyValue, 12);
						gstKeyboard.KeyBuf[5] = gstKeyboard.u8NumValue + 0x30;
					}
					else
					{
						gstKeyboard.u8TempValue = KeyChangeNumValue(gstKeyboard.u8TempValue, gstKeyboard.u8KeyValue, 12);
						stTime.u8Month = gstKeyboard.u8TempValue;
						sprintf((char*)gstKeyboard.KeyBuf, "20%02d-%02d-", stTime.u8Year, stTime.u8Month);
						gstKeyboard.u8TempValue = 0;
						gstKeyboard.u8FourMenuStep ++;
					}
					GUI_SetTime((uchar8*)gstKeyboard.KeyBuf, (uchar8*)gstKeyboard.tempBuf);
				}
			}
			break;

			case 3: //输入日
			{				
				if((Key_0 <= gstKeyboard.u8KeyValue)&&(Key_9 >= gstKeyboard.u8KeyValue))
				{
					gstKeyboard.u8KeyCount ++;
					if(5 == gstKeyboard.u8KeyCount)
					{
						gstKeyboard.u8TempValue = KeyChangeNumValue(gstKeyboard.u8TempValue, gstKeyboard.u8KeyValue, 31);
						gstKeyboard.KeyBuf[8] = gstKeyboard.u8NumValue + 0x30;
					}
					else
					{
						gstKeyboard.u8TempValue = KeyChangeNumValue(gstKeyboard.u8TempValue, gstKeyboard.u8KeyValue, 31);
						stTime.u8Day = gstKeyboard.u8TempValue; 
						sprintf((char*)gstKeyboard.KeyBuf, "20%02d-%02d-%02d", stTime.u8Year, stTime.u8Month, stTime.u8Day);
						gstKeyboard.u8TempValue = 0;
						gstKeyboard.u8FourMenuStep ++;
					}
					GUI_SetTime((uchar8*)gstKeyboard.KeyBuf, (uchar8*)gstKeyboard.tempBuf);
				}
			}
			break;


			case 4: //输入时
			{
				if((Key_0 <= gstKeyboard.u8KeyValue)&&(Key_9 >= gstKeyboard.u8KeyValue))
				{						
					gstKeyboard.u8KeyCount ++;
					if(7 == gstKeyboard.u8KeyCount)
					{
						gstKeyboard.u8TempValue = KeyChangeNumValue(gstKeyboard.u8TempValue, gstKeyboard.u8KeyValue, 23);
						gstKeyboard.tempBuf[0] = gstKeyboard.u8NumValue + 0x30;
					}
					else
					{
						gstKeyboard.u8TempValue = KeyChangeNumValue(gstKeyboard.u8TempValue, gstKeyboard.u8KeyValue, 23);
						stTime.u8Hour = gstKeyboard.u8TempValue;
						sprintf((char*)gstKeyboard.tempBuf, "%02d:", stTime.u8Hour);
						gstKeyboard.u8TempValue = 0;
						gstKeyboard.u8FourMenuStep ++;
					}
					GUI_SetTime((uchar8*)gstKeyboard.KeyBuf, (uchar8*)gstKeyboard.tempBuf);
				}
			}
			break;
			
			case 5: //输入分
			{				
				if((Key_0 <= gstKeyboard.u8KeyValue)&&(Key_9 >= gstKeyboard.u8KeyValue))
				{
					gstKeyboard.u8KeyCount ++;
					if(9 == gstKeyboard.u8KeyCount)
					{
						gstKeyboard.u8TempValue = KeyChangeNumValue(gstKeyboard.u8TempValue, gstKeyboard.u8KeyValue, 59);
						gstKeyboard.tempBuf[3] = gstKeyboard.u8NumValue + 0x30;
					}
					else
					{
						gstKeyboard.u8TempValue = KeyChangeNumValue(gstKeyboard.u8TempValue, gstKeyboard.u8KeyValue, 59);
						stTime.u8Minute = gstKeyboard.u8TempValue;
						sprintf((char*)gstKeyboard.tempBuf, "%02d:%02d:", stTime.u8Hour, stTime.u8Minute);
						gstKeyboard.u8TempValue = 0;
						gstKeyboard.u8FourMenuStep ++;
					}
					GUI_SetTime((uchar8*)gstKeyboard.KeyBuf, (uchar8*)gstKeyboard.tempBuf);
				}
			}
			break;

			case 6: //输入秒
			{				
				if((Key_0 <= gstKeyboard.u8KeyValue)&&(Key_9 >= gstKeyboard.u8KeyValue))
				{
					gstKeyboard.u8KeyCount ++;
					if(11 == gstKeyboard.u8KeyCount)
					{
						gstKeyboard.u8TempValue = KeyChangeNumValue(gstKeyboard.u8TempValue, gstKeyboard.u8KeyValue, 59);
						gstKeyboard.tempBuf[6] = gstKeyboard.u8NumValue + 0x30;
					}
					else
					{
						gstKeyboard.u8TempValue = KeyChangeNumValue(gstKeyboard.u8TempValue, gstKeyboard.u8KeyValue, 59);
						stTime.u8Second = gstKeyboard.u8TempValue; 
						sprintf((char*)gstKeyboard.tempBuf, "%02d:%02d:%02d", stTime.u8Hour, stTime.u8Minute, stTime.u8Second);
						gstKeyboard.u8TempValue = 0;
						gstKeyboard.u8FourMenuStep ++;
					}
					GUI_SetTime((uchar8*)gstKeyboard.KeyBuf, (uchar8*)gstKeyboard.tempBuf);
				}
			}
			break;

			case 7: //等待确认
			{				
				if(Key_Enter == gstKeyboard.u8KeyValue)
				{
					if(OK == Chack_Date(stTime.u8Year, stTime.u8Month, stTime.u8Day)&&(OK == Chack_Time(stTime.u8Hour, stTime.u8Minute, stTime.u8Second)))
					{
						stTime.u8WeekDay = Calc_Weekday(stTime.u8Year, stTime.u8Month, stTime.u8Day);

						iTaday = (gstSysRunInfo.stRtcTime.u8Year * 365) + (gstSysRunInfo.stRtcTime.u8Month * 30) + gstSysRunInfo.stRtcTime.u8Day;
						iInputDate = (stTime.u8Year * 365) + (stTime.u8Month * 30) + stTime.u8Day;
						//如果设置了有效期就禁止时间往回倒
						if((gstSysRunInfo.bForbidTimeGoBack)&&(iInputDate < iTaday))
						{
							Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 5);
							GUI_ShowNotice(NOTICE_SET_FAIL);
						}
						else
						{
							RTC_SetDateTime((pST_TIME)&stTime);							
							GUI_ShowNotice(NOTICE_SET_OK);
							Buzzer(600);
						}
 					}
					else
					{
						Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 5);
						GUI_ShowNotice(NOTICE_SET_FAIL);
					}
					delay_ms(1234);					
					gstKeyboard.u8ThreeMenuStep = 0;
					gstKeyboard.u8FourMenuStep = 0;
					gstKeyboard.bAddInto = 1;
				}
			}
			break;
			
			default:
			{
				gstKeyboard.u8FourMenuStep = 0;
			}
			break;
		}
	}
}

/***********************************************************************************************
* Function Name  : Set_VoiceModuleLevel
* Description	 : 设置语音模块的音量等级
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : 主菜单-->系统-->语音-->音量设置(属于第四级菜单)
***********************************************************************************************/
void Set_VoiceModuleLevel(void)
{
	uint32 iAddr = 0;

	if(Key_Cancel == gstKeyboard.u8KeyValue)
	{	
		gstKeyboard.u8FourMenuStep = 0;
		gstKeyboard.u8ThreeMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8FourMenuStep)
		{
			case 0: //显示提示界面
			{
				gstKeyboard.bAddInto = 0;
				gstKeyboard.KeyBuf[0] = SysConfig.stHardware.u8VoiceLevel;
				GUI_InputVoiceLevel(gstKeyboard.KeyBuf[0]);   
				gstKeyboard.u8FourMenuStep ++;
			}
			break;	

			case 1: //语音音量(0-30)
			{
				if(Key_Enter == gstKeyboard.u8KeyValue)
				{
					SysConfig.stHardware.u8VoiceLevel = gstKeyboard.KeyBuf[0];
					iAddr = Addr_SysConfig + ((uint32)&SysConfig.stHardware.u8VoiceLevel - (uint32)&SysConfig);
					Store_Parameters(iAddr, (uchar8*)&SysConfig.stHardware.u8VoiceLevel, 1);

					GUI_ShowNotice(NOTICE_SET_OK);
					Buzzer(600);
					delay_ms(1234); 
					gstKeyboard.u8ThreeMenuStep = 0;
					gstKeyboard.u8FourMenuStep = 0;
					gstKeyboard.bAddInto = 1;
				}
				else if((gstKeyboard.u8KeyValue >= Key_0)&&(gstKeyboard.u8KeyValue <= Key_9))
				{
					gstKeyboard.KeyBuf[0] = KeyChangeNumValue(gstKeyboard.KeyBuf[0], gstKeyboard.u8KeyValue, VOICE_LEVEL_MAX);
					GUI_InputVoiceLevel(gstKeyboard.KeyBuf[0]);
				}
			}
			break;
			
			default:
			{
				gstKeyboard.u8FourMenuStep = 0;
			}
			break;
		}
	}
}

/***********************************************************************************************
* Function Name  : Set_MachineNum
* Description	 : 设置机号
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : 主菜单-->系统-->地址-->机号设置(属于第四级菜单)
***********************************************************************************************/
void Set_MachineNum(void)
{
	if(Key_Cancel == gstKeyboard.u8KeyValue)
	{	
		gstKeyboard.u8FourMenuStep = 0;
		gstKeyboard.u8ThreeMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8FourMenuStep)
		{
			case 0: //显示界面
			{
				gstKeyboard.bAddInto = 0;
				gstKeyboard.iTempValue = 0;
				GUI_InputMachineNum(gstKeyboard.iTempValue);
				gstKeyboard.u8FourMenuStep ++;
			}
			break;	
			
			case 1: //设置机号
			{
				if(Key_Enter == gstKeyboard.u8KeyValue)
				{
					SysConfig.stHardware.u32MachineNum = gstKeyboard.iTempValue;						
					Store_Parameters(Addr_Hardware, (uchar8*)&SysConfig.stHardware, sizeof(ST_SYS_HW));
					//提示设置成功
					GUI_ShowNotice(NOTICE_SET_OK);
					Buzzer(600);
					delay_ms(1234); 				
					gstKeyboard.u8FourMenuStep = 0;
					gstKeyboard.u8ThreeMenuStep = 0;
					gstKeyboard.bAddInto = 1;
				}
				else
				{
					gstKeyboard.iTempValue = KeyChangeNumValue(gstKeyboard.iTempValue, gstKeyboard.u8KeyValue, 0xFFFFFF00);
					GUI_InputMachineNum(gstKeyboard.iTempValue);
				}
			}
			break;
			
			default:
			{
				gstKeyboard.u8FourMenuStep = 0;
			}
			break;
		}
	}
}

/***********************************************************************************************
* Function Name  : Set_NetSwitch
* Description	 : 设置联网开关
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : 主菜单-->系统-->联网-->联网选择(属于第四级菜单)
***********************************************************************************************/
void Set_NetSwitch(void)
{
	uint32 iAddr = 0;
	
	if(Key_Cancel == gstKeyboard.u8KeyValue)
	{	
		gstKeyboard.u8FourMenuStep = 0;
		gstKeyboard.u8ThreeMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8FourMenuStep)
		{
			case 0: //显示提示界面
			{
				gstKeyboard.bAddInto = 0;
				GUI_SetNetSwitch(SysConfig.stHardware.bNetwork);   
				gstKeyboard.u8FourMenuStep ++;
			}
			break;	

			case 1: //联网开箱授权
			{
				if((Key_1 == gstKeyboard.u8KeyValue)||(Key_2 == gstKeyboard.u8KeyValue))
				{
					if(Key_1 == gstKeyboard.u8KeyValue)
					{
						SysConfig.stHardware.bNetwork = VALIDITY_USING;
					}
					else
					{
						SysConfig.stHardware.bNetwork = 0;
					}
					
					GUI_SetNetSwitch(SysConfig.stHardware.bNetwork);
					
					iAddr = Addr_SysConfig + ((uint32)&SysConfig.stHardware.bNetwork - (uint32)&SysConfig);
					Store_Parameters(iAddr, (uchar8*)&SysConfig.stHardware.bNetwork, 4);

					delay_ms(456); 
					GUI_ShowNotice(NOTICE_SET_OK);
					Buzzer(600);
					delay_ms(1234); 
					gstKeyboard.u8ThreeMenuStep = 0;
					gstKeyboard.u8FourMenuStep = 0;
					gstKeyboard.bAddInto = 1;
					
				}
			}
			break;
			
			default:
			{
				gstKeyboard.u8FourMenuStep = 0;
			}
			break;
		}
	}
}

/***********************************************************************************************
* Function Name  : Set_TcpIpInfo
* Description	 : 设置网络信息
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : 主菜单-->系统-->网络-->网络设置(属于第四级菜单)
***********************************************************************************************/
void Set_TcpIpInfo(void)
{
	uint32 iAddr= 0;
	
	if(Key_Cancel == gstKeyboard.u8KeyValue)
	{	
		if(1 == gstKeyboard.u8FourMenuStep)
		{
			gstKeyboard.u8ThreeMenuStep = 0;
		}
		gstKeyboard.u8FourMenuStep = 0;
		gstKeyboard.bAddInto = 1;			
	}
	else
	{
		switch(gstKeyboard.u8FourMenuStep)
		{
			case 0: //显示选择界面
			{
				gstKeyboard.bAddInto = 0;
				GUI_ChoiceNetInfo();
				memset((uchar8*)gstKeyboard.KeyBuf, 0x0, sizeof(gstKeyboard.KeyBuf));
				memset((uchar8*)gstKeyboard.IpBuf, 0x0, sizeof(gstKeyboard.IpBuf));
				gstKeyboard.iTempValue = 0;
				gstKeyboard.u8FourMenuStep ++;
			}
			break;	

			case 1: //网络-->选择(1.本机IP 2.服务器IP 3.服务器端口号)
			{
				if((gstKeyboard.u8NumValue >= 1)&&(gstKeyboard.u8NumValue <= 3))
				{					
					if(1 == gstKeyboard.u8NumValue) 
					{
						GUI_InputIp(1, (uchar8*)gstKeyboard.KeyBuf);
						gstKeyboard.u8FourMenuStep = 2;
					}
					else if(2 == gstKeyboard.u8NumValue)
					{
						GUI_InputIp(2, (uchar8*)gstKeyboard.KeyBuf);
						gstKeyboard.u8FourMenuStep = 3;
					}
					else if(3 == gstKeyboard.u8NumValue)
					{
						GUI_InputServerPort(gstKeyboard.iTempValue);
						gstKeyboard.u8FourMenuStep = 4;
					}
				}
			}
			break;

			case 2: //网络-->设置本机IP
			{
				if(Key_Enter == gstKeyboard.u8KeyValue)
				{
					if((gstKeyboard.IpBuf[0])&&(gstKeyboard.IpBuf[1])&&(gstKeyboard.IpBuf[3]))
					{
						SysConfig.stNet.IP[0] = gstKeyboard.IpBuf[0];
						SysConfig.stNet.IP[1] = gstKeyboard.IpBuf[1];
						SysConfig.stNet.IP[2] = gstKeyboard.IpBuf[2];
						SysConfig.stNet.IP[3] = gstKeyboard.IpBuf[3];

						//网关自动修改过来				
						memcpy((uchar8*)&SysConfig.stNet.Gateway[0], (uchar8*)&SysConfig.stNet.IP[0], 3);	 
						SysConfig.stNet.Gateway[3] = 1;
						Store_Parameters(Addr_NetInfo, (uchar8*)&SysConfig.stNet , sizeof(ST_NETWORK));
						//提示设置成功
						GUI_ShowNotice(NOTICE_SET_OK);
						Buzzer(600);
					}
					else
					{
						Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 5);
						GUI_ShowNotice(NOTICE_SET_FAIL);
					}
					delay_ms(1234);					
					gstKeyboard.u8FourMenuStep = 0;
					gstKeyboard.bAddInto = 1;
				}
				else
				{
					KeyInputIpAddr(gstKeyboard.u8KeyValue, (uchar8*)gstKeyboard.IpBuf, (uchar8*)gstKeyboard.KeyBuf);
					GUI_InputIp(1, (uchar8*)gstKeyboard.KeyBuf);
				}
			}
			break;

			case 3: //网络-->设置服务器IP
			{
				if(Key_Enter == gstKeyboard.u8KeyValue)
				{
					if((gstKeyboard.IpBuf[0])&&(gstKeyboard.IpBuf[1])&&(gstKeyboard.IpBuf[3]))
					{
						SysConfig.stNet.ServerIP[0] = gstKeyboard.IpBuf[0];
						SysConfig.stNet.ServerIP[1] = gstKeyboard.IpBuf[1];
						SysConfig.stNet.ServerIP[2] = gstKeyboard.IpBuf[2];
						SysConfig.stNet.ServerIP[3] = gstKeyboard.IpBuf[3];

						iAddr = Addr_SysConfig + ((uint32)&SysConfig.stNet.ServerIP[0] - (uint32)&SysConfig);
						Store_Parameters(iAddr, (uchar8*)&SysConfig.stNet.ServerIP[0], sizeof(SysConfig.stNet.ServerIP));
						//提示设置成功
						GUI_ShowNotice(NOTICE_SET_OK);
						Buzzer(600);
					}
					else
					{
						Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 5);
						GUI_ShowNotice(NOTICE_SET_FAIL);
					}
					delay_ms(1234);					
					gstKeyboard.u8FourMenuStep = 0;
					gstKeyboard.bAddInto = 1;
				}
				else
				{				
					KeyInputIpAddr(gstKeyboard.u8KeyValue, (uchar8*)gstKeyboard.IpBuf, (uchar8*)gstKeyboard.KeyBuf);
					GUI_InputIp(2, (uchar8*)gstKeyboard.KeyBuf);
				}
			}
			break;
			
			case 4: //网络-->设置服务器端口号
			{
				if(Key_Enter == gstKeyboard.u8KeyValue)
				{
					SysConfig.stNet.u16UDP_ServerPort = gstKeyboard.iTempValue;		
					iAddr = Addr_SysConfig + ((uint32)&SysConfig.stNet.u16UDP_ServerPort - (uint32)&SysConfig);
				    Store_Parameters(iAddr, (uchar8*)&SysConfig.stNet.u16UDP_ServerPort, 2);

					//提示设置成功
					GUI_ShowNotice(NOTICE_SET_OK);
					Buzzer(600);
					delay_ms(1234);					
					gstKeyboard.u8FourMenuStep = 0;
					gstKeyboard.bAddInto = 1;
				}
				else
				{
					gstKeyboard.iTempValue = KeyChangeNumValue(gstKeyboard.iTempValue, gstKeyboard.u8KeyValue, 9123456);
					GUI_InputServerPort(gstKeyboard.iTempValue);
				}
			}
			break;
			
			default:
			{
			}
			break;
		}
	}
}

/***********************************************************************************************
* Function Name  : Sys_Registration
* Description	 : 系统注册
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : 主菜单-->系统-->注册-->系统注册(属于第四级菜单)
***********************************************************************************************/
void Sys_Registration(void)
{
	extern const char *pMenu_Registration[];
	
	if(Key_Cancel == gstKeyboard.u8KeyValue)
	{	
		gstKeyboard.u8FourMenuStep = 0;
		gstKeyboard.u8ThreeMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8FourMenuStep)
		{
			case 0: //显示界面
			{
				gstKeyboard.bAddInto = 0;
				memset((uchar8*)gstKeyboard.KeyBuf, 0x0, sizeof(gstKeyboard.KeyBuf));
				gstKeyboard.u8KeyCount = 0;
				gstKeyboard.u8InputType = 0;  
				GUI_Registration((uchar8*)gstKeyboard.KeyBuf, gstKeyboard.u8InputType);
				gstKeyboard.u8FourMenuStep ++;
			}
			break;	

			case 1: //输入注册码
			{
				if(Key_Enter == gstKeyboard.u8KeyValue)
				{
					if(0xEB == CheckRegistrationCode((uchar8*)gstKeyboard.KeyBuf))
					{
						CheckSysValidityDate();
						GUI_ShowMessage(pMenu_Registration[1], LCD_Color_Red);
						delay_ms(3210);
						GUI_ShowMessage(pMenu_Registration[2], LCD_Color_Red);
						delay_ms(1000);
						GUI_ShowMessage(pMenu_Registration[3], LCD_Color_Red);
						delay_ms(3210);
						GUI_ShowMessage(pMenu_Registration[4], LCD_Color_Red);
						Buzzer(600);
					}
					else
					{
						GUI_ShowNotice(NOTICE_PWD_ERR);
						Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 5);
					}
					delay_ms(1234);	
					gstKeyboard.u8FourMenuStep = 0;
					gstKeyboard.u8ThreeMenuStep = 0;
					gstKeyboard.bAddInto = 1;
				}
				else if(Key_Shift == gstKeyboard.u8KeyValue)
				{
					if(INPUT_TYPE_ABC == gstKeyboard.u8InputType)
					{
						gstKeyboard.u8InputType = 0;
					}
					else
					{
						gstKeyboard.u8InputType = INPUT_TYPE_ABC;
					}
					GUI_Registration((uchar8*)gstKeyboard.KeyBuf, gstKeyboard.u8InputType);
				}
				else if(((Key_Star == gstKeyboard.u8KeyValue)||((Key_Jin == gstKeyboard.u8KeyValue)))&&(INPUT_TYPE_ABC != gstKeyboard.u8InputType))
				{			
					if(gstKeyboard.u8KeyCount)
					{
						gstKeyboard.u8KeyCount --;
						gstKeyboard.KeyBuf[gstKeyboard.u8KeyCount] = 0;
						GUI_Registration((uchar8*)gstKeyboard.KeyBuf, gstKeyboard.u8InputType);
					}
				}
				else if(gstKeyboard.u8KeyCount < 10)
				{
					if(INPUT_TYPE_ABC == gstKeyboard.u8InputType)
					{
						gstKeyboard.KeyBuf[gstKeyboard.u8KeyCount] = NumKeyToASCII(gstKeyboard.u8KeyValue);
						gstKeyboard.u8KeyCount ++;
					}
					else
					{
						if((gstKeyboard.u8KeyValue >= Key_0)&&(gstKeyboard.u8KeyValue <= Key_9))
						{
							gstKeyboard.KeyBuf[gstKeyboard.u8KeyCount] = gstKeyboard.u8NumValue + 0x30;
							gstKeyboard.u8KeyCount ++;
						}
					}
					GUI_Registration((uchar8*)gstKeyboard.KeyBuf, gstKeyboard.u8InputType);
				}
			}
			break;
			
			default:
			{
				gstKeyboard.u8FourMenuStep = 0;
			}
			break;
		}
	}
}

/***********************************************************************************************
* Function Name  : Set_AdminPassword
* Description	 : 修改用户管理员密码
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : 主菜单-->管理-->管密-->管密修改(属于第四级菜单)
***********************************************************************************************/
void Set_AdminPassword(void)
{
	uint32 iAddr = 0;
	
	if(Key_Cancel == gstKeyboard.u8KeyValue)
	{	
		gstKeyboard.u8FourMenuStep = 0;
		gstKeyboard.u8ThreeMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8FourMenuStep)
		{
			case 0: //显示界面
			{
				gstKeyboard.bAddInto = 0;
				gstKeyboard.u8KeyCount = 0;
				memset((uchar8*)gstKeyboard.KeyBuf, 0x0, sizeof(gstKeyboard.KeyBuf));
				memset((uchar8*)gstKeyboard.tempBuf, 0x0, sizeof(gstKeyboard.tempBuf));
				GUI_ChangePassword((uchar8*)SysConfig.stLockers.AdminPassword, (uchar8*)gstKeyboard.KeyBuf);
				gstKeyboard.u8FourMenuStep ++;
			}
			break;	

			case 1: //选择-->3.用户管理员密码  
			{				
				if(Key_Enter == gstKeyboard.u8KeyValue)
				{
					memset((uchar8*)SysConfig.stLockers.AdminPassword, 0x0, sizeof(SysConfig.stLockers.AdminPassword));
					memcpy((uchar8*)SysConfig.stLockers.AdminPassword, (uchar8*)gstKeyboard.KeyBuf, 6);

					//保存数据&&提示设置成功	
					iAddr = Addr_SysConfig + ((uint32)&SysConfig.stLockers.AdminPassword[0] - (uint32)&SysConfig);
					Store_Parameters(iAddr, (uchar8*)&SysConfig.stLockers.AdminPassword[0], sizeof(SysConfig.stLockers.AdminPassword));
					GUI_ShowNotice(NOTICE_SET_OK);
					Buzzer(600);
					delay_ms(1234); 
					gstKeyboard.u8ThreeMenuStep = 0;
					gstKeyboard.u8FourMenuStep = 0;
					gstKeyboard.bAddInto = 1;
				}
				else if((Key_Star == gstKeyboard.u8KeyValue)||((Key_Jin == gstKeyboard.u8KeyValue)))
				{			
					if(gstKeyboard.u8KeyCount)
					{
						gstKeyboard.u8KeyCount --;
						gstKeyboard.KeyBuf[gstKeyboard.u8KeyCount] = 0; 					
						GUI_ChangePassword((uchar8*)SysConfig.stLockers.AdminPassword, (uchar8*)gstKeyboard.KeyBuf);
					}
				}
				else if(gstKeyboard.u8KeyCount < MAX_PasswordLen)
				{				
					if((gstKeyboard.u8KeyValue >= Key_0)&&(gstKeyboard.u8NumValue <= Key_9))
					{
						gstKeyboard.KeyBuf[gstKeyboard.u8KeyCount] = gstKeyboard.u8NumValue + 0x30;
						gstKeyboard.u8KeyCount ++;
						GUI_ChangePassword((uchar8*)SysConfig.stLockers.AdminPassword, (uchar8*)gstKeyboard.KeyBuf);
					}
				}
			}
			break;
		
			default:
			{
				gstKeyboard.u8FourMenuStep = 0;
			}
			break;
		}
	}
}

/***********************************************************************************************
* Function Name  : Set_LockersName
* Description	 : 录入本柜名称
* Input 		 : void
* Output		 : None
* Return		 : None 
***********************************************************************************************/
void Set_LockersName(void)
{
	uint32 iAddr = 0;
	extern const char* pStr_PlsEnterName;
	
	if(Key_Cancel == gstKeyboard.u8KeyValue)
	{	
		gstKeyboard.u8FourMenuStep = 0;
		gstKeyboard.u8ThreeMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8FourMenuStep)
		{
			case 0: //显示界面
			{
				gstKeyboard.bAddInto = 0;
				memset((uchar8*)gstKeyboard.KeyBuf, 0x0, sizeof(gstKeyboard.KeyBuf));
				gstKeyboard.u8KeyCount = 0;
				gstKeyboard.u8InputType = 0;  
				GUI_InputName((uchar8*)gstKeyboard.KeyBuf, gstKeyboard.u8InputType, pStr_PlsEnterName);
				gstKeyboard.u8FourMenuStep ++;
			}
			break;	

			case 1: 
			{
				if(Key_Enter == gstKeyboard.u8KeyValue)
				{
					memset((uchar8*)&SysConfig.stLockers.Name[0], 0x0, sizeof(SysConfig.stLockers.Name));
					memcpy((uchar8*)&SysConfig.stLockers.Name[0], (uchar8*)gstKeyboard.KeyBuf, gstKeyboard.u8KeyCount);
					iAddr = Addr_SysConfig + ((uint32)&SysConfig.stLockers.Name[0] - (uint32)&SysConfig);
					Store_Parameters(iAddr, (uchar8*)SysConfig.stLockers.Name, sizeof(SysConfig.stLockers.Name));
					GUI_ShowNotice(NOTICE_SET_OK);
					Buzzer(500);
					
					delay_ms(1234);	
					gstKeyboard.u8FourMenuStep = 0;
					gstKeyboard.u8ThreeMenuStep = 0;
					gstKeyboard.bAddInto = 1;
				}
				else if(Key_Shift == gstKeyboard.u8KeyValue)
				{
					if(INPUT_TYPE_ABC == gstKeyboard.u8InputType)
					{
						gstKeyboard.u8InputType = 0;
					}
					else
					{
						gstKeyboard.u8InputType = INPUT_TYPE_ABC;
					}
					GUI_InputName((uchar8*)gstKeyboard.KeyBuf, gstKeyboard.u8InputType, pStr_PlsEnterName);
				}
				else if(((Key_Star == gstKeyboard.u8KeyValue)||((Key_Jin == gstKeyboard.u8KeyValue)))&&(INPUT_TYPE_ABC != gstKeyboard.u8InputType))
				{			
					if(gstKeyboard.u8KeyCount)
					{
						gstKeyboard.u8KeyCount --;
						gstKeyboard.KeyBuf[gstKeyboard.u8KeyCount] = 0;
						GUI_InputName((uchar8*)gstKeyboard.KeyBuf, gstKeyboard.u8InputType, pStr_PlsEnterName);
					}
				}
				else if(gstKeyboard.u8KeyCount < MAX_NameLen)
				{
					if(INPUT_TYPE_ABC == gstKeyboard.u8InputType)
					{
						gstKeyboard.KeyBuf[gstKeyboard.u8KeyCount] = NumKeyToASCII(gstKeyboard.u8KeyValue);
						gstKeyboard.u8KeyCount ++;
					}
					else
					{
						if((gstKeyboard.u8KeyValue >= Key_0)&&(gstKeyboard.u8KeyValue <= Key_9))
						{
							gstKeyboard.KeyBuf[gstKeyboard.u8KeyCount] = gstKeyboard.u8NumValue + 0x30;
							gstKeyboard.u8KeyCount ++;
						}
					}
					GUI_InputName((uchar8*)gstKeyboard.KeyBuf, gstKeyboard.u8InputType, pStr_PlsEnterName);
				}
			}
			break;
			
			default:
			{
				gstKeyboard.u8FourMenuStep = 0;
			}
			break;
		}
	}
}

/***********************************************************************************************
* Function Name  : Set_ClientIdCode
* Description	 : 录入客户标识码
* Input 		 : void
* Output		 : None
* Return		 : None 
***********************************************************************************************/
void Set_ClientIdCode(void)
{
	uint32 iAddr = 0;
	extern const char* pStr_PlsEnterIdCode;
	
	if(Key_Cancel == gstKeyboard.u8KeyValue)
	{	
		gstKeyboard.u8FourMenuStep = 0;
		gstKeyboard.u8ThreeMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8FourMenuStep)
		{
			case 0: //显示界面
			{
				gstKeyboard.bAddInto = 0;
				memset((uchar8*)gstKeyboard.KeyBuf, 0x0, sizeof(gstKeyboard.KeyBuf));
				gstKeyboard.u8KeyCount = 0;
				gstKeyboard.u8InputType = 0;  
				GUI_InputName((uchar8*)gstKeyboard.KeyBuf, gstKeyboard.u8InputType, pStr_PlsEnterIdCode);
				gstKeyboard.u8FourMenuStep ++;
			}
			break;	

			case 1: 
			{
				if(Key_Enter == gstKeyboard.u8KeyValue)
				{
					memset((uchar8*)&SysConfig.stClientInfo.IdCode[0], 0x0, sizeof(SysConfig.stClientInfo.IdCode));
					memcpy((uchar8*)&SysConfig.stClientInfo.IdCode[0], (uchar8*)gstKeyboard.KeyBuf, gstKeyboard.u8KeyCount);
					iAddr = Addr_SysConfig + ((uint32)&SysConfig.stClientInfo.IdCode[0] - (uint32)&SysConfig);
					Store_Parameters(iAddr, (uchar8*)SysConfig.stClientInfo.IdCode, sizeof(SysConfig.stClientInfo.IdCode));
					GUI_ShowNotice(NOTICE_SET_OK);
					Buzzer(500);
					
					delay_ms(1234);	
					gstKeyboard.u8FourMenuStep = 0;
					gstKeyboard.u8ThreeMenuStep = 0;
					gstKeyboard.bAddInto = 1;
				}
				else if(Key_Shift == gstKeyboard.u8KeyValue)
				{
					if(INPUT_TYPE_ABC == gstKeyboard.u8InputType)
					{
						gstKeyboard.u8InputType = 0;
					}
					else
					{
						gstKeyboard.u8InputType = INPUT_TYPE_ABC;
					}
					GUI_InputName((uchar8*)gstKeyboard.KeyBuf, gstKeyboard.u8InputType, pStr_PlsEnterIdCode);
				}
				else if(((Key_Star == gstKeyboard.u8KeyValue)||((Key_Jin == gstKeyboard.u8KeyValue)))&&(INPUT_TYPE_ABC != gstKeyboard.u8InputType))
				{			
					if(gstKeyboard.u8KeyCount)
					{
						gstKeyboard.u8KeyCount --;
						gstKeyboard.KeyBuf[gstKeyboard.u8KeyCount] = 0;
						GUI_InputName((uchar8*)gstKeyboard.KeyBuf, gstKeyboard.u8InputType, pStr_PlsEnterIdCode);
					}
				}
				else if(gstKeyboard.u8KeyCount < SYS_ID_CODE_LEN)
				{
					if(INPUT_TYPE_ABC == gstKeyboard.u8InputType)
					{
						gstKeyboard.KeyBuf[gstKeyboard.u8KeyCount] = NumKeyToASCII(gstKeyboard.u8KeyValue);
						gstKeyboard.u8KeyCount ++;
					}
					else
					{
						if((gstKeyboard.u8KeyValue >= Key_0)&&(gstKeyboard.u8KeyValue <= Key_9))
						{
							gstKeyboard.KeyBuf[gstKeyboard.u8KeyCount] = gstKeyboard.u8NumValue + 0x30;
							gstKeyboard.u8KeyCount ++;
						}
					}
					GUI_InputName((uchar8*)gstKeyboard.KeyBuf, gstKeyboard.u8InputType, pStr_PlsEnterIdCode);
				}
			}
			break;
			
			default:
			{
				gstKeyboard.u8FourMenuStep = 0;
			}
			break;
		}
	}
}

/***********************************************************************************************
* Function Name  : Set_AddAdminCard
* Description	 : 增加管理卡
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : 主菜单-->用户-->授卡-->授卡-->增加管理卡(属于第五级菜单)
***********************************************************************************************/
void Set_AddAdminCard(void)
{
	static uint32 LastScanTime;
	static uint32 NoCardTime;
	static uint32 LastOperationTime;
	static uchar8 TryCount;
	uint32 iCardID01 = 0;
	static uchar8 CardCount;
	int i = 0;
	uint32 iAddr = 0;

	if(Key_Cancel == gstKeyboard.u8KeyValue)
	{	
		gstKeyboard.u8FourMenuStep = 0;
		gstKeyboard.u8FiveMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8FiveMenuStep)
		{
			case 0: //显示界面
			{
				gstKeyboard.bAddInto = 1;  //要进来读卡
				gstKeyboard.iTempValue = 0;
				CardCount = 0;
				GUI_AddCard2Box(0, 0, 0);
				gstKeyboard.u8FiveMenuStep ++;
			}
			break;	
			
			case 1: //读卡
			{
				if(gstSysRunInfo.u32TimeCount_ms - LastScanTime > 500)
				{
					LastScanTime = gstSysRunInfo.u32TimeCount_ms;
					gstKeyboard.iTempValue = Get_IC_ID_CardSN();
					if(gstKeyboard.iTempValue)
					{
						//先检查下此卡是否已授权
						for(i=0; i<MAX_AdminCardNum; i++)
						{  
							if(gstKeyboard.iTempValue == SysConfig.stLockers.AdminCard[i])
							{
								//此卡已授权
								GUI_AddCard2Box(0, gstKeyboard.iTempValue, ALREADY_EXIST);	
							    Buzzer(300);
								//等待卡离开
								TryCount = 0;
								gstKeyboard.u8FiveMenuStep = 2;  
								NoCardTime = 0;
								return;
							}
						}

						//再检查授权卡数量
						CardCount = 0;
						for(i=0; i<MAX_AdminCardNum; i++)
						{
							if(0 != SysConfig.stLockers.AdminCard[i])
							{
								CardCount ++;	
							}
						}
						if(MAX_AdminCardNum == CardCount)
						{
							//授权已满
							GUI_AddCard2Box(0, gstKeyboard.iTempValue, MAX_AdminCardNum);	
							Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
						}
						else
						{
							GUI_AddCard2Box(0, gstKeyboard.iTempValue, CardCount);	
							SysConfig.stLockers.AdminCard[CardCount] = gstKeyboard.iTempValue;							

							iAddr = Addr_SysConfig + ((uint32)&SysConfig.stLockers.AdminCard[0] - (uint32)&SysConfig);
							Store_Parameters(iAddr, (uchar8*)&SysConfig.stLockers.AdminCard[0], sizeof(SysConfig.stLockers.AdminCard));

							Buzzer(300);
						}
						TryCount = 0;
						gstKeyboard.u8FiveMenuStep = 2;  //等待卡离开
						NoCardTime = 0;
					}
					else
					{
						NoCardTime += 500;
						if(NoCardTime > 11000)  //超时不刷卡自动退出(gstKeyboard.bAddInto = 1, 因此自动取消键退出无效)
						{
							NoCardTime = 0;
							gstKeyboard.u8FourMenuStep = 0;
							gstKeyboard.u8FiveMenuStep = 0;
							gstKeyboard.bAddInto = 1;
						}
					}
				}
			}
			break;

			case 2: //等待卡离开
			{				
				if(gstSysRunInfo.u32TimeCount_ms - LastOperationTime > 200)
				{
					LastOperationTime = gstSysRunInfo.u32TimeCount_ms;
					iCardID01 = Get_IC_ID_CardSN();
					if((0 == iCardID01)||(iCardID01 != gstKeyboard.iTempValue))
					{
						TryCount ++;
						if(TryCount >= 2)
						{							
							gstKeyboard.iTempValue = 0;
							gstKeyboard.u8FiveMenuStep = 0;
						}
					}
					else
					{
						TryCount = 0;
					}
				}
			}
			break;

			default:
			{
				gstKeyboard.u8FiveMenuStep = 0;
			}
			break;
		}
	}	
}

/***********************************************************************************************
* Function Name  : Set_DeleteAdminCard
* Description	 : 删除管理卡
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : 主菜单-->用户-->授卡-->清卡-->删除管理卡(全部)(属于第五级菜单)
***********************************************************************************************/
void Set_DeleteAdminCard(void)
{
	uint32 i = 0;
	uint32 iAddr = 0;

	if(Key_Cancel == gstKeyboard.u8KeyValue)
	{	
		gstKeyboard.u8FourMenuStep = 0;
		gstKeyboard.u8FiveMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8FiveMenuStep)
		{
			case 0: //显示界面
			{
				gstKeyboard.bAddInto = 1;  
				gstKeyboard.iTempValue = 0;
				GUI_ShowNotice(NOTICE_ASK_CLR_CARD);
				gstKeyboard.u8FiveMenuStep ++;
			}
			break;	
			
			case 1: //退卡
			{
				if(Key_Enter== gstKeyboard.u8KeyValue)
				{
					for(i=0; i<MAX_AdminCardNum; i++)
					{
						SysConfig.stLockers.AdminCard[i] = 0;
					}
					//保存信息
					iAddr = Addr_SysConfig + ((uint32)&SysConfig.stLockers.AdminCard[0] - (uint32)&SysConfig);
					Store_Parameters(iAddr, (uchar8*)&SysConfig.stLockers.AdminCard[0], sizeof(SysConfig.stLockers.AdminCard));
					GUI_ShowNotice(NOTICE_ASK_CLR_CARD_OK);
					Buzzer(600);
					delay_ms(1234);	
					gstKeyboard.u8FourMenuStep = 0;
					gstKeyboard.u8FiveMenuStep = 0;
					gstKeyboard.bAddInto = 1;
				}
			}
			break;
			
			default:
			{
				gstKeyboard.u8FiveMenuStep = 0;
			}
			break;
		}
	}	
}

/***********************************************************************************************
* Function Name  : Set_AdminCard
* Description	 : 授卡管理
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : 主菜单-->用户-->授卡-->授卡管理(属于第四级菜单)
***********************************************************************************************/
void Set_AdminCard(void)
{
	if((Key_Cancel == gstKeyboard.u8KeyValue)&&(1 == gstKeyboard.u8FourMenuStep))
	{		
		gstKeyboard.u8ThreeMenuStep = 0;
		gstKeyboard.u8FourMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8FourMenuStep)
		{
			case 0: //显示选择界面
			{
				gstKeyboard.bAddInto = 0;
				GUI_AdminCardOpertion();
				gstKeyboard.u8FourMenuStep ++;
			}
			break;

			case 1:
			{
				switch(gstKeyboard.u8KeyValue)
				{
					case Key_0: //退出
					{
						gstKeyboard.u8ThreeMenuStep = 0;
						gstKeyboard.u8FourMenuStep = 0;
						gstKeyboard.bAddInto = 1;
					}
					break;
					
					case Key_1: //授卡
					{						
						gstKeyboard.u8FourMenuStep =2;
						gstKeyboard.bAddInto = 1;						
					}
					break;
				
					case Key_2: //退卡
					{
						gstKeyboard.u8FourMenuStep =3;
						gstKeyboard.bAddInto = 1;	
					}
					break;
				}

			}
			break;
			
			case 2: 
			{
				Set_AddAdminCard();
			}
			break;
			
			case 3: 
			{
				Set_DeleteAdminCard();
			}
			break;

			default:
			{
				gstKeyboard.u8FourMenuStep = 0;
				gstKeyboard.u8ThreeMenuStep = 0;	
			}
			break;
		}
	}
}

/***********************************************************************************************
* Function Name  : Set_DebugMode
* Description	 : 设置调试模式开关 
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : 主菜单-->系统-->调试-->调试选择(属于第四级菜单)
***********************************************************************************************/
void Set_DebugMode(void)
{
	uint32 iAddr = 0;
	
	if(Key_Cancel == gstKeyboard.u8KeyValue)
	{	
		gstKeyboard.u8FourMenuStep = 0;
		gstKeyboard.u8ThreeMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8FourMenuStep)
		{
			case 0: //显示提示界面
			{
				gstKeyboard.bAddInto = 0;
				GUI_SetDebugMode(SysConfig.stHardware.bDebugMode);   
				gstKeyboard.u8FourMenuStep ++;
			}
			break;	

			case 1:  
			{
				if((Key_1 == gstKeyboard.u8KeyValue)||(Key_2 == gstKeyboard.u8KeyValue))
				{
					if(Key_1 == gstKeyboard.u8KeyValue)
					{
						SysConfig.stHardware.bDebugMode = VALIDITY_USING;
					}
					else
					{
						SysConfig.stHardware.bDebugMode = 0;
					}
					
					GUI_SetDebugMode(SysConfig.stHardware.bDebugMode);
					delay_ms(600); 
					
					iAddr = Addr_SysConfig + ((uint32)&SysConfig.stHardware.bDebugMode - (uint32)&SysConfig);
					Store_Parameters(iAddr, (uchar8*)&SysConfig.stHardware.bDebugMode, 1);

					GUI_ShowNotice(NOTICE_SET_OK);
					Buzzer(600);
					delay_ms(NOTICE_STAY_TIME /2); 
					gstKeyboard.u8ThreeMenuStep = 0;
					gstKeyboard.u8FourMenuStep = 0;
					gstKeyboard.bAddInto = 1;
					
				}
			}
			break;
			
			default:
			{
				gstKeyboard.u8FourMenuStep = 0;
			}
			break;
		}
	}
}

/***********************************************************************************************
* Function Name  : Set_BoxDoorStatusCheck
* Description	 : 设置是否检测开门状态开关 
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : 主菜单-->系统-->xxx-->xxxx(属于第四级菜单)
***********************************************************************************************/
void Set_BoxDoorStatusCheck(void)
{
	uint32 iAddr = 0;
	
	if(Key_Cancel == gstKeyboard.u8KeyValue)
	{	
		gstKeyboard.u8FourMenuStep = 0;
		gstKeyboard.u8ThreeMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8FourMenuStep)
		{
			case 0: //显示提示界面
			{
				gstKeyboard.bAddInto = 0;
				GUI_SetBoxDoorCheckSwitch(SysConfig.stLockers.bCheckBoxDoorStatus);   
				gstKeyboard.u8FourMenuStep ++;
			}
			break;	

			case 1:  
			{
				if((Key_1 == gstKeyboard.u8KeyValue)||(Key_2 == gstKeyboard.u8KeyValue))
				{
					if(Key_1 == gstKeyboard.u8KeyValue)
					{
						SysConfig.stLockers.bCheckBoxDoorStatus = VALIDITY_USING;
					}
					else
					{
						SysConfig.stLockers.bCheckBoxDoorStatus = 0;
					}
					
					GUI_SetBoxDoorCheckSwitch(SysConfig.stLockers.bCheckBoxDoorStatus);
					delay_ms(600); 
					
					iAddr = Addr_SysConfig + ((uint32)&SysConfig.stLockers.bCheckBoxDoorStatus - (uint32)&SysConfig);
					Store_Parameters(iAddr, (uchar8*)&SysConfig.stLockers.bCheckBoxDoorStatus, 1);

					GUI_ShowNotice(NOTICE_SET_OK);
					Buzzer(600);
					delay_ms(NOTICE_STAY_TIME /2); 
					gstKeyboard.u8ThreeMenuStep = 0;
					gstKeyboard.u8FourMenuStep = 0;
					gstKeyboard.bAddInto = 1;
					
				}
			}
			break;
			
			default:
			{
				gstKeyboard.u8FourMenuStep = 0;
			}
			break;
		}
	}
}

/***********************************************************************************************
* Function Name  : Set_IDCardSwitch
* Description	 : 设置ID卡兼容开关 
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : 主菜单-->系统-->xxx-->xxxx(属于第四级菜单)
***********************************************************************************************/
void Set_IDCardSwitch(void)
{
	uint32 iAddr = 0;
	
	if(Key_Cancel == gstKeyboard.u8KeyValue)
	{	
		gstKeyboard.u8FourMenuStep = 0;
		gstKeyboard.u8ThreeMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8FourMenuStep)
		{
			case 0: //显示提示界面
			{
				gstKeyboard.bAddInto = 0;
				GUI_SetIDCardSwitch(SysConfig.stLockers.bEnableIdCard);   
				gstKeyboard.u8FourMenuStep ++;
			}
			break;	

			case 1:  
			{
				if((Key_1 == gstKeyboard.u8KeyValue)||(Key_2 == gstKeyboard.u8KeyValue))
				{
					if(Key_1 == gstKeyboard.u8KeyValue)
					{
						SysConfig.stLockers.bEnableIdCard = VALIDITY_USING;
					}
					else
					{
						SysConfig.stLockers.bEnableIdCard = 0;
					}
					
					GUI_SetIDCardSwitch(SysConfig.stLockers.bEnableIdCard);
					delay_ms(600); 
					
					iAddr = Addr_SysConfig + ((uint32)&SysConfig.stLockers.bEnableIdCard - (uint32)&SysConfig);
					Store_Parameters(iAddr, (uchar8*)&SysConfig.stLockers.bEnableIdCard, 1);

					GUI_ShowNotice(NOTICE_SET_OK);
					Buzzer(600);
					delay_ms(NOTICE_STAY_TIME /2); 
					gstKeyboard.u8ThreeMenuStep = 0;
					gstKeyboard.u8FourMenuStep = 0;
					gstKeyboard.bAddInto = 1;
					
				}
			}
			break;
			
			default:
			{
				gstKeyboard.u8FourMenuStep = 0;
			}
			break;
		}
	}
}

/***********************************************************************************************
* Function Name  : Set_WorkMode
* Description	 : 设置工作模式 
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : 主菜单-->系统-->选项-->模式选择(属于第四级菜单)
***********************************************************************************************/
void Set_WorkMode(void)
{
	uint32 iAddr = 0;
	
	if(Key_Cancel == gstKeyboard.u8KeyValue)
	{	
		gstKeyboard.u8FourMenuStep = 0;
		gstKeyboard.u8ThreeMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8FourMenuStep)
		{
			case 0: //显示提示界面
			{
				gstKeyboard.bAddInto = 0;
				GUI_SetWorkMode(SysConfig.stLockers.u8WorkMode);   
				gstKeyboard.u8FourMenuStep ++;
			}
			break;	

			case 1: 
			{
				if((Key_1 <= gstKeyboard.u8KeyValue)&&(gstKeyboard.u8KeyValue <= Key_6))
				{
					SysConfig.stLockers.u8WorkMode = gstKeyboard.u8NumValue -1;
					
					GUI_SetWorkMode(SysConfig.stLockers.u8WorkMode);   
					
					iAddr = Addr_SysConfig + ((uint32)&SysConfig.stLockers.u8WorkMode - (uint32)&SysConfig);
					Store_Parameters(iAddr, (uchar8*)&SysConfig.stLockers.u8WorkMode, 1);

					delay_ms(456); 
					GUI_ShowNotice(NOTICE_SET_OK);
					Buzzer(600);
					delay_ms(1234); 
					gstKeyboard.u8ThreeMenuStep = 0;
					gstKeyboard.u8FourMenuStep = 0;
					gstKeyboard.bAddInto = 1;				
				}
			}
			break;
			
			default:
			{
				gstKeyboard.u8FourMenuStep = 0;
			}
			break;
		}
	}
}

/***********************************************************************************************
* Function Name  : Set_LockersValidityDate
* Description	 : 设置箱柜有效期
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : 主菜单-->设置-->期限-->有效期(属于第四级菜单)
***********************************************************************************************/
void Set_LockersValidityDate(void)
{
	uint32 iAddr = 0;
	uchar8 ret = 0;

	if(Key_Cancel == gstKeyboard.u8KeyValue)
	{	
		gstKeyboard.u8ThreeMenuStep = 0;
		gstKeyboard.u8FourMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8FourMenuStep)
		{
			case 0: //显示界面
			{
				gstKeyboard.bAddInto = 0;
				gstKeyboard.u8Box = 0;
				gstKeyboard.u8Select = 0;
				KeyInputDate(0);
				gstKeyboard.u8FourMenuStep ++;
			}
			break;	

			case 1:  
			{
				ret = KeyInputDate(gstKeyboard.u8KeyValue);
				if(0xFF == ret)
				{
					SysConfig.stClientInfo.ValidityDate[0] = stTime.u8Year;
					SysConfig.stClientInfo.ValidityDate[1] = stTime.u8Month;
					SysConfig.stClientInfo.ValidityDate[2] = stTime.u8Day;
					
					iAddr = Addr_SysConfig + ((uint32)&SysConfig.stClientInfo.ValidityDate[0] - (uint32)&SysConfig);
					Store_Parameters(iAddr, (uchar8*)SysConfig.stClientInfo.ValidityDate, sizeof(SysConfig.stClientInfo.ValidityDate));
					GUI_ShowNotice(NOTICE_SET_OK);
					Buzzer(500);
					delay_ms(1200);
					gstKeyboard.u8FourMenuStep = 0;
					gstKeyboard.bAddInto = 1;
				}
			}
			break;
			
			default:
			{
				gstKeyboard.u8FourMenuStep = 0;
			}
			break;
		}
	}

}

/***********************************************************************************************
* Function Name  : Set_CardSector
* Description	 : 设置IC卡的使用扇区号
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : 主菜单-->管理-->xxx->xxxx(属于第四级菜单)
***********************************************************************************************/
void Set_CardSector(void)
{
	uint32 iAddr = 0;

	if(Key_Cancel == gstKeyboard.u8KeyValue)
	{	
		gstKeyboard.u8FourMenuStep = 0;
		gstKeyboard.u8ThreeMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8FourMenuStep)
		{
			case 0: //显示提示界面
			{
				gstKeyboard.bAddInto = 0;
				gstKeyboard.u8TempValue = 0;
				GUI_SetCardSector(gstKeyboard.u8TempValue);   
				gstKeyboard.u8FourMenuStep ++;
			}
			break;	

			case 1:  
			{
				if(Key_Enter == gstKeyboard.u8KeyValue)
				{
					SysConfig.stCardKey.u8Sector = gstKeyboard.u8TempValue;
					iAddr = Addr_SysConfig + ((uint32)&SysConfig.stCardKey.u8Sector - (uint32)&SysConfig);
					Store_Parameters(iAddr, (uchar8*)&SysConfig.stCardKey.u8Sector, 1);

					GUI_ShowNotice(NOTICE_SET_OK);
					Buzzer(600);
					delay_ms(NOTICE_STAY_TIME); 
					gstKeyboard.u8ThreeMenuStep = 0;
					gstKeyboard.u8FourMenuStep = 0;
					gstKeyboard.bAddInto = 1;
				}
				else if((gstKeyboard.u8KeyValue >= Key_0)&&(gstKeyboard.u8KeyValue <= Key_9))
				{
					gstKeyboard.u8TempValue = KeyChangeNumValue(gstKeyboard.u8TempValue, gstKeyboard.u8KeyValue, 32);
					GUI_SetCardSector(gstKeyboard.u8TempValue);
				}
			}
			break;
			
			default:
			{
				gstKeyboard.u8FourMenuStep = 0;
			}
			break;
		}
	}
}

/***********************************************************************************************
* Function Name  : Set_AllowOpenBoxNum
* Description	 : 设置寄存时允许的最大开箱数量
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : 主菜单-->系统-->xxx->xxxx(属于第四级菜单)
***********************************************************************************************/
void Set_AllowOpenBoxNum(void)
{
	uint32 iAddr = 0;

	if(Key_Cancel == gstKeyboard.u8KeyValue)
	{	
		gstKeyboard.u8FourMenuStep = 0;
		gstKeyboard.u8ThreeMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8FourMenuStep)
		{
			case 0: //显示提示界面
			{
				gstKeyboard.bAddInto = 0;
				gstKeyboard.u8TempValue = 0;
				GUI_InputAllowOpenBoxNum(gstKeyboard.u8TempValue);   
				gstKeyboard.u8FourMenuStep ++;
			}
			break;	

			case 1:  
			{
				if(Key_Enter == gstKeyboard.u8KeyValue)
				{
					SysConfig.stLockers.u8AllowBeOpenedBoxNum = gstKeyboard.u8TempValue;
					iAddr = Addr_SysConfig + ((uint32)&SysConfig.stLockers.u8AllowBeOpenedBoxNum - (uint32)&SysConfig);
					Store_Parameters(iAddr, (uchar8*)&SysConfig.stLockers.u8AllowBeOpenedBoxNum, 1);

					GUI_ShowNotice(NOTICE_SET_OK);
					Buzzer(600);
					delay_ms(NOTICE_STAY_TIME); 
					gstKeyboard.u8ThreeMenuStep = 0;
					gstKeyboard.u8FourMenuStep = 0;
					gstKeyboard.bAddInto = 1;
				}
				else if((gstKeyboard.u8KeyValue >= Key_0)&&(gstKeyboard.u8KeyValue <= Key_9))
				{
					gstKeyboard.u8TempValue = KeyChangeNumValue(gstKeyboard.u8TempValue, gstKeyboard.u8KeyValue, 255);
					GUI_InputAllowOpenBoxNum(gstKeyboard.u8TempValue);
				}
			}
			break;
			
			default:
			{
				gstKeyboard.u8FourMenuStep = 0;
			}
			break;
		}
	}
}

/***********************************************************************************************
* Function Name  : Set_PickUpTimeout
* Description	 : 设置寄存超时时间(最大寄存时间)
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : 主菜单-->系统-->超时时间-->时间设置(属于第四级菜单)
***********************************************************************************************/
void Set_PickUpTimeout(void)
{
	uint32 iAddr = 0;

	if(Key_Cancel == gstKeyboard.u8KeyValue)
	{	
		gstKeyboard.u8FourMenuStep = 0;
		gstKeyboard.u8ThreeMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8FourMenuStep)
		{
			case 0: //显示提示界面
			{
				gstKeyboard.bAddInto = 0;
				gstKeyboard.iTempValue = 0;
				GUI_SetPickUpTimeout(gstKeyboard.iTempValue);   
				gstKeyboard.u8FourMenuStep ++;
			}
			break;	

			case 1:  
			{
				if(Key_Enter == gstKeyboard.u8KeyValue)
				{
					SysConfig.stLockers.iMaxFetchTime = gstKeyboard.iTempValue;
					iAddr = Addr_SysConfig + ((uint32)&SysConfig.stLockers.iMaxFetchTime - (uint32)&SysConfig);
					Store_Parameters(iAddr, (uchar8*)&SysConfig.stLockers.iMaxFetchTime, sizeof(SysConfig.stLockers.iMaxFetchTime));

					GUI_ShowNotice(NOTICE_SET_OK);
					Buzzer(600);
					delay_ms(NOTICE_STAY_TIME); 
					gstKeyboard.u8ThreeMenuStep = 0;
					gstKeyboard.u8FourMenuStep = 0;
					gstKeyboard.bAddInto = 1;
				}
				else if((gstKeyboard.u8KeyValue >= Key_0)&&(gstKeyboard.u8KeyValue <= Key_9))
				{
					gstKeyboard.iTempValue = KeyChangeNumValue(gstKeyboard.iTempValue, gstKeyboard.u8KeyValue, 5000000);
					GUI_SetPickUpTimeout(gstKeyboard.iTempValue);   
				}
			}
			break;
			
			default:
			{
				gstKeyboard.u8FourMenuStep = 0;
			}
			break;
		}
	}
}

/***********************************************************************************************
* Function Name  : Set_PickUpAwaitTime
* Description	 : 设置开箱等待时间(最小寄存时间)
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : 主菜单-->系统-->延时取-->时间设置(属于第四级菜单)
***********************************************************************************************/
void Set_PickUpAwaitTime(void)
{
	uint32 iAddr = 0;

	if(Key_Cancel == gstKeyboard.u8KeyValue)
	{	
		gstKeyboard.u8FourMenuStep = 0;
		gstKeyboard.u8ThreeMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8FourMenuStep)
		{
			case 0: //显示提示界面
			{
				gstKeyboard.bAddInto = 0;
				gstKeyboard.iTempValue = 0;
				GUI_SetPickUpAwaitTime(gstKeyboard.iTempValue);   
				gstKeyboard.u8FourMenuStep ++;
			}
			break;	

			case 1:  
			{
				if(Key_Enter == gstKeyboard.u8KeyValue)
				{
					SysConfig.stLockers.iMinFetchTime = gstKeyboard.iTempValue;
					iAddr = Addr_SysConfig + ((uint32)&SysConfig.stLockers.iMinFetchTime - (uint32)&SysConfig);
					Store_Parameters(iAddr, (uchar8*)&SysConfig.stLockers.iMinFetchTime, sizeof(SysConfig.stLockers.iMinFetchTime));

					GUI_ShowNotice(NOTICE_SET_OK);
					Buzzer(600);
					delay_ms(NOTICE_STAY_TIME); 
					gstKeyboard.u8ThreeMenuStep = 0;
					gstKeyboard.u8FourMenuStep = 0;
					gstKeyboard.bAddInto = 1;
				}
				else if((gstKeyboard.u8KeyValue >= Key_0)&&(gstKeyboard.u8KeyValue <= Key_9))
				{
					gstKeyboard.iTempValue = KeyChangeNumValue(gstKeyboard.iTempValue, gstKeyboard.u8KeyValue, 50000);
					GUI_SetPickUpAwaitTime(gstKeyboard.iTempValue);   
				}
			}
			break;
			
			default:
			{
				gstKeyboard.u8FourMenuStep = 0;
			}
			break;
		}
	}
}

/***********************************************************************************************
* Function Name  : Set_HandleTimeout
* Description	 : 设置(寄存关门)操作超时提示时间
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : 主菜单-->系统-->提示-->延时时间设置(属于第四级菜单)
***********************************************************************************************/
void Set_HandleTimeout(void)
{
	uint32 iAddr = 0;
	extern const char* pTitleStr_CloseTimeout;
	extern const char* pTitleStr_NoticeTime;
	
	if(Key_Cancel == gstKeyboard.u8KeyValue)
	{	
		gstKeyboard.u8FourMenuStep = 0;
		gstKeyboard.u8ThreeMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8FourMenuStep)
		{
			case 0: //显示提示界面
			{
				gstKeyboard.bAddInto = 0;
				gstKeyboard.iTempValue = 0;
				GUI_SetUIntValue(pTitleStr_CloseTimeout, SysConfig.stLockers.iHandleTimeout, gstKeyboard.iTempValue);   
				gstKeyboard.u8FourMenuStep ++;
			}
			break;	

			case 1: //设置关门超时时间
			{
				if(Key_Enter == gstKeyboard.u8KeyValue)
				{
					SysConfig.stLockers.iHandleTimeout = gstKeyboard.iTempValue;
					iAddr = Addr_SysConfig + ((uint32)&SysConfig.stLockers.iHandleTimeout - (uint32)&SysConfig);
					Store_Parameters(iAddr, (uchar8*)&SysConfig.stLockers.iHandleTimeout, 4);

					GUI_ShowNotice(NOTICE_SET_OK);
					Buzzer(600);
					delay_ms(600);
					
					gstKeyboard.iTempValue = 0;
					GUI_SetUIntValue(pTitleStr_NoticeTime, SysConfig.stLockers.iNoticeTime, gstKeyboard.iTempValue);   
					gstKeyboard.u8FourMenuStep += 1;
				}
				else if((gstKeyboard.u8KeyValue >= Key_0)&&(gstKeyboard.u8KeyValue <= Key_9))
				{
					gstKeyboard.iTempValue = KeyChangeNumValue(gstKeyboard.iTempValue, gstKeyboard.u8KeyValue, 1000000);
					GUI_SetUIntValue(pTitleStr_CloseTimeout, SysConfig.stLockers.iHandleTimeout, gstKeyboard.iTempValue);   
				}
			}
			break;
				
			case 2: //设置提示维持时间
			{
				if(Key_Enter == gstKeyboard.u8KeyValue)
				{
					SysConfig.stLockers.iNoticeTime = gstKeyboard.iTempValue;
					iAddr = Addr_SysConfig + ((uint32)&SysConfig.stLockers.iNoticeTime - (uint32)&SysConfig);
					Store_Parameters(iAddr, (uchar8*)&SysConfig.stLockers.iNoticeTime, 4);

					GUI_ShowNotice(NOTICE_SET_OK);
					Buzzer(600);
					delay_ms(800);
					gstKeyboard.u8ThreeMenuStep = 0;
					gstKeyboard.u8FourMenuStep = 0;
					gstKeyboard.bAddInto = 1;
				}
				else if((gstKeyboard.u8KeyValue >= Key_0)&&(gstKeyboard.u8KeyValue <= Key_9))
				{
					gstKeyboard.iTempValue = KeyChangeNumValue(gstKeyboard.iTempValue, gstKeyboard.u8KeyValue, 1000000);
					GUI_SetUIntValue(pTitleStr_NoticeTime, SysConfig.stLockers.iNoticeTime, gstKeyboard.iTempValue);   
				}
			}
			break;
			
			default:
			{
				gstKeyboard.u8FourMenuStep = 0;
			}
			break;
		}
	}
}

/***********************************************************************************************
* Function Name  : Add_ValidCard
* Description	 : 预授权卡
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : 主菜单-->管理-->预授权-->增加有效卡号(属于第四级菜单)
***********************************************************************************************/
void Add_ValidCard(void)
{
	static uint32 LastScanTime;
	static uint32 NoCardTime;
	static uchar8 TryCount;
	uchar8 ret = 0;
	uint32 iCardID01 = 0;

	if(Key_Cancel == gstKeyboard.u8KeyValue)
	{	
		gstKeyboard.u8ThreeMenuStep = 0;
		gstKeyboard.u8FourMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		if(SysConfig.stLockers.iValidCardCount >= MAX_VALID_CAED_COUNT)
		{
			GUI_ShowMessage("Is Full!", LCD_Color_Red);
			Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 5);
			delay_ms(2000);
			
			gstKeyboard.u8ThreeMenuStep = 0;
			gstKeyboard.u8FourMenuStep = 0;
			gstKeyboard.bAddInto = 1;
		}

		switch(gstKeyboard.u8FourMenuStep)
		{
			case 0: //显示界面
			{
				gstKeyboard.bAddInto = 1;  //要进来读卡
				gstKeyboard.iCardUID = 0;
				GUI_AddValidCard(gstKeyboard.iCardUID, 0);
				gstKeyboard.u8FourMenuStep ++;
			}
			break;	
			
			case 1: //读卡
			{
				if(gstSysRunInfo.u32TimeCount_ms - LastScanTime > 500)
				{
					LastScanTime = gstSysRunInfo.u32TimeCount_ms;
					gstKeyboard.iCardUID = Get_IC_ID_CardSN();
					if(gstKeyboard.iCardUID)
					{
						ret = AddOneValidCardNum(gstKeyboard.iCardUID);
						GUI_AddValidCard(gstKeyboard.iCardUID, ret);	
						Buzzer(300);
						NoCardTime = 0;
						TryCount = 0;
						delay_ms(500);
						gstKeyboard.u8FourMenuStep = 2;
					}
					else
					{
						NoCardTime += 500;
						if(NoCardTime > 11000)  //超时不刷卡自动退出
						{
							NoCardTime = 0;
							gstKeyboard.u8ThreeMenuStep = 0;
							gstKeyboard.u8FourMenuStep = 0;
							gstKeyboard.bAddInto = 1;
						}

					}
				}
			}
			break;

			case 2: //等待卡离开
			{	
				if(gstSysRunInfo.u32TimeCount_ms - LastScanTime > 220)
				{
					LastScanTime = gstSysRunInfo.u32TimeCount_ms;
					iCardID01 = Get_IC_ID_CardSN();
					if((0 == iCardID01)||(iCardID01 != gstKeyboard.iCardUID))
					{
						TryCount ++;
						if(TryCount >= 2)
						{
							GUI_AddValidCard(0, 0);
							gstKeyboard.u8FourMenuStep = 1;
						}
					}
				}
			}
			break;
			
			default:
			{
				gstKeyboard.u8FourMenuStep = 0;
			}
			break;
		}
	}	
}

/***********************************************************************************************
* Function Name  : Clr_AllValidCard
* Description	 : 管理员登录-->清除所有预授权卡
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : 主菜单-->....(属于第四级菜单)
***********************************************************************************************/
void Clr_AllValidCard(void)
{
	int i = 1;
	uint32 iAddr = 0;
	
	if(Key_Cancel == gstKeyboard.u8KeyValue)
	{	
		gstKeyboard.u8FourMenuStep = 0;
		gstKeyboard.u8ThreeMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8FourMenuStep)
		{
			case 0: //显示界面
			{
				gstKeyboard.bAddInto = 0;
				GUI_ShowNotice(NOTICE_ASK_CLR_CARD);
				gstKeyboard.u8FourMenuStep ++;
			}
			break;	
			
			case 1: //确认全清
			{
				if(Key_Enter == gstKeyboard.u8KeyValue)
				{
					GUI_ShowNotice(NOTICE_PLS_WAIT);
					gstKeyboard.iTempValue = 0;
				//	for(i=0; i<SysConfig.stLockers.iValidCardCount; i++)
					for(i=0; i<MAX_VALID_CAED_COUNT; i++)
					{
						WDT_Feed();
						iAddr = Addr_PreAuthorization + (i *4);
						I2C_EE_BufferWrite((uchar8*)&gstKeyboard.iTempValue, iAddr, 4);	
					}
					
					//保存记录变量  
					SysConfig.stLockers.iValidCardCount = 0;
					iAddr = Addr_SysConfig + ((uint32)&SysConfig.stLockers.iValidCardCount - (uint32)&SysConfig);
					Store_Parameters(iAddr, (uchar8*)&SysConfig.stLockers.iValidCardCount, 4);

					GUI_ShowNotice(NOTICE_SET_OK);					
					Buzzer(600);
					delay_ms(3210); 	
					gstKeyboard.u8FourMenuStep = 0;
					gstKeyboard.u8ThreeMenuStep = 0;
					gstKeyboard.bAddInto = 1;
				}			
			}
			break;
			
			default:
			{
				gstKeyboard.u8FourMenuStep = 0;
			}
			break;
		}
	}
}

/***********************************************************************************************
* Function Name  : Open_OneBox
* Description	 : 管理员登录-->单开箱门
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : 主菜单-->箱门-->单开-->开箱(属于第四级菜单)
***********************************************************************************************/
void Open_OneBox(void)
{
	if(Key_Cancel == gstKeyboard.u8KeyValue)
	{	
		if(1==gstKeyboard.u8FourMenuStep)
		{
			gstKeyboard.u8ThreeMenuStep = 0;
		}
		gstKeyboard.u8FourMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8FourMenuStep)
		{
			case 0: //显示界面
			{
				gstKeyboard.bAddInto = 0;
				gstKeyboard.u8Box = 0;
				GUI_InputBoxID(gstKeyboard.u8Box);
				gstKeyboard.u8FourMenuStep ++;
			}
			break;	
			
			case 1: //输入箱号-->开箱
			{
				if(Key_Enter == gstKeyboard.u8KeyValue)
				{
					if(gstKeyboard.u8Box)
					{
						GUI_Open1Box(gstKeyboard.u8Box, 1);
						SelectOneBoxToOpen(SysConfig.stBox[gstKeyboard.u8Box].u8BoxID, 0);
						delay_ms(1000);
						
						memset((uchar8*)&stRecord, 0x0, sizeof(ST_RECORD));		
						stRecord.u8BoxID = gstKeyboard.u8Box;
						stRecord.u8Type = OpenBoxType_Admin;
						stRecord.u8SaveOrTake = 0; 
						StoreOpenBoxRecord(&stRecord);	
						
						gstKeyboard.u8FourMenuStep = 0;
						gstKeyboard.bAddInto = 1;
					}
				}
				else 
				{
					if((gstKeyboard.u8KeyValue >= Key_0)&&(gstKeyboard.u8KeyValue <= Key_9))
					{
						gstKeyboard.u8Box = KeyChangeNumValue(gstKeyboard.u8Box, gstKeyboard.u8KeyValue, SysConfig.stLockers.u8BoxCount);
						GUI_InputBoxID(gstKeyboard.u8Box);
					}
				}				
			}
			break;
			
			default:
			{
				gstKeyboard.u8FourMenuStep = 0;
			}
			break;
		}
	}
}

/***********************************************************************************************
* Function Name  : Open_AllBox
* Description	 : 管理员登录-->全开箱门
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : 主菜单-->箱门-->全开-->全开箱(属于第四级菜单)
***********************************************************************************************/
void Open_AllBox(void)
{
	int i = 1;
	uchar8 ret = 0;
	
	if(Key_Cancel == gstKeyboard.u8KeyValue)
	{	
		gstKeyboard.u8FourMenuStep = 0;
		gstKeyboard.u8ThreeMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8FourMenuStep)
		{
			case 0: //显示界面
			{
				gstKeyboard.bAddInto = 0;
				GUI_ShowNotice(NOTICE_ASK_OPEN_ALL_BOX);
				gstKeyboard.u8FourMenuStep ++;
			}
			break;	
			
			case 1: //确认开箱
			{
				if((Key_Enter == gstKeyboard.u8KeyValue)||(gstKeyboard.bAddInto))
				{
					for(i=1; i<=SysConfig.stLockers.u8BoxCount; i++)
					{	
						if(SysConfig.stLockers.bCheckBoxDoorStatus)
						{
							ret = SelectOneBoxToOpen(i, 1);
							if(OK == ret)
							{
								GUI_Open1Box(i, 2);
							}
							else
							{
								Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
								GUI_Open1BoxFaild(i, 2);
							}
						}
						else
						{
							GUI_Open1Box(i, 2);
							SelectOneBoxToOpen(i, 0);
							delay_ms(500);
						}
						memset((uchar8*)&stRecord, 0x0, sizeof(ST_RECORD));	 
						stRecord.u8BoxID = i;
						stRecord.u8Type = OpenBoxType_Admin;
						stRecord.u8SaveOrTake = 0; 
						StoreOpenBoxRecord(&stRecord);	
						
						gstKeyboard.u8KeyValue = Key_Get_KeyDown();	
						if(Key_Jin == gstKeyboard.u8KeyValue)
						{
							gstKeyboard.u8FourMenuStep = 0;
							gstKeyboard.u8ThreeMenuStep = 0;
							gstKeyboard.bAddInto = 1;
							return;
						}
					}

					if(VALIDITY_USING == SysConfig.stHardware.bDebugMode)
					{
						delay_ms(1000);
					}
					else
					{
						gstKeyboard.u8FourMenuStep = 0;
						gstKeyboard.u8ThreeMenuStep = 0;
					}
					gstKeyboard.bAddInto = 1;
				}			
			}
			break; 
			
			default:
			{
				gstKeyboard.u8FourMenuStep = 0;
			}
			break;
		}
	}
}

/***********************************************************************************************
* Function Name  : Clean_1Box
* Description	 : 管理员登录-->单清箱门
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : 主菜单-->箱门-->单清-->清箱(属于第四级菜单)
***********************************************************************************************/
void Clean_OneBox(void)
{
	uint32 iAddr = 0;
	extern const char* pStr_AskClrBox;
	
	if(Key_Cancel == gstKeyboard.u8KeyValue)
	{	
		if(1==gstKeyboard.u8FourMenuStep)
		{
			gstKeyboard.u8ThreeMenuStep = 0;
		}
		gstKeyboard.u8FourMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8FourMenuStep)
		{
			case 0: //显示界面
			{
				gstKeyboard.bAddInto = 0;
				gstKeyboard.u8Box = 0;
				GUI_InputBoxID(gstKeyboard.u8Box);
				gstKeyboard.u8FourMenuStep ++;
			}
			break;	
			
			case 1: //输入箱号-->清箱
			{
				if(Key_Enter == gstKeyboard.u8KeyValue)
				{
					if(gstKeyboard.u8Box)
					{
						SelectOneBoxToOpen(gstKeyboard.u8Box, 0);
						GUI_ClrBoxNotice(gstKeyboard.u8Box);
						
						//删除关联的卡和指纹
						SendCmdToFingerprint_DeletChar(SysConfig.stBox[gstKeyboard.u8Box].iTouchID, 1);
						SysConfig.stBox[gstKeyboard.u8Box].iUserCardID = 0;
						SysConfig.stBox[gstKeyboard.u8Box].iTouchID = 0;
						SysConfig.stBox[gstKeyboard.u8Box].u8Status = BOX_STATUS_IDLE;
						
						iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[gstKeyboard.u8Box] - (uint32)&SysConfig);
						Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[gstKeyboard.u8Box], sizeof(ST_BOX));

						//保存一笔开箱记录
						memset((uchar8*)&stRecord, 0x0, sizeof(ST_RECORD));	 
						stRecord.u8BoxID = gstKeyboard.u8Box;
						stRecord.u8Type = OpenBoxType_Admin;
						stRecord.u8SaveOrTake = 0; 
						StoreOpenBoxRecord(&stRecord);	

						//更新空箱列表
						UpdateEmptyBoxList();
						Buzzer(600);
						delay_ms(NOTICE_STAY_TIME);	
						gstKeyboard.u8FourMenuStep = 0;
						gstKeyboard.bAddInto = 1;						
					}
				}	
				else 
				{
					if((gstKeyboard.u8KeyValue >= Key_0)&&(gstKeyboard.u8KeyValue <= Key_9))
					{
						gstKeyboard.u8Box = KeyChangeNumValue(gstKeyboard.u8Box, gstKeyboard.u8KeyValue, MAX_BOX_NUM);
						GUI_InputBoxID(gstKeyboard.u8Box);
						LCD_DisplayStr(160, 240, FONT_SIZE24, pStr_AskClrBox, LCD_Color_Red);
					}
				}		
			}
			break;
			
			default:
			{
				gstKeyboard.u8FourMenuStep = 0;
			}
			break;
		}
	}
}

/***********************************************************************************************
* Function Name  : Clean_AllBox
* Description	 : 管理员登录-->全清箱门
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : 主菜单-->箱门-->全清-->全清箱(属于第四级菜单)
***********************************************************************************************/
void Clean_AllBox(void)
{
	int i = 1;
	uint32 iAddr = 0;
	
	if(Key_Cancel == gstKeyboard.u8KeyValue)
	{	
		gstKeyboard.u8FourMenuStep = 0;
		gstKeyboard.u8ThreeMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8FourMenuStep)
		{
			case 0: //显示界面
			{
				gstKeyboard.bAddInto = 0;
				GUI_ShowNotice(NOTICE_ASK_CLR_ALLBOX);
				gstKeyboard.u8FourMenuStep ++;
			}
			break;	
			
			case 1: //确认全清箱
			{
				if(Key_Enter == gstKeyboard.u8KeyValue)
				{
					for(i=1; i<=SysConfig.stLockers.u8BoxCount; i++)
					{
						WDT_Feed();
						SelectOneBoxToOpen(i, 0);
						GUI_Open1Box(i, 2);
	
						//删除关联的卡和指纹
						SendCmdToFingerprint_DeletChar(SysConfig.stBox[i].iTouchID, 1);
						SysConfig.stBox[i].iUserCardID = 0;
						SysConfig.stBox[i].iTouchID = 0;
						SysConfig.stBox[i].u8Status = BOX_STATUS_IDLE;

						iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[i] - (uint32)&SysConfig);
						Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[i], sizeof(ST_BOX));

						//保存一笔开箱记录
						memset((uchar8*)&stRecord, 0x0, sizeof(ST_RECORD));	 
						stRecord.u8BoxID = i;
						stRecord.u8Type = OpenBoxType_Admin;
						stRecord.u8SaveOrTake = 0; 
						StoreOpenBoxRecord(&stRecord);	
						delay_ms(500);
						
						//按"#"键停止执行
						gstKeyboard.u8KeyValue = Key_Get_KeyDown();	
						if(Key_Jin == gstKeyboard.u8KeyValue)
						{
							break;
						}
					}
					SendCmdToFingerprint_Empty();//全清指纹

					GUI_ClrBoxNotice(MARK_CHOICE_ALL_BOX);
					Buzzer(600);
					delay_ms(3210); 
					//更新空箱列表
					UpdateEmptyBoxList();		
					gstKeyboard.u8FourMenuStep = 0;
					gstKeyboard.u8ThreeMenuStep = 0;
					gstKeyboard.bAddInto = 1;
				}			
			}
			break;
			
			default:
			{
				gstKeyboard.u8FourMenuStep = 0;
			}
			break;
		}
	}
}

/***********************************************************************************************
* Function Name  : Lockin_Box
* Description	 : 管理员登录-->锁箱
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : 主菜单-->箱门-->锁箱-->锁定操作(属于第四级菜单)
***********************************************************************************************/
void Lockin_Box(void)
{
	uint32 iAddr = 0;
	int i = 0;
	extern const char *pStr_Notice255IsAll;
	
	if(Key_Cancel == gstKeyboard.u8KeyValue)
	{	
		if(1==gstKeyboard.u8FourMenuStep)
		{
			gstKeyboard.u8ThreeMenuStep = 0;
		}
		gstKeyboard.u8FourMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8FourMenuStep)
		{
			case 0: //显示界面
			{
				gstKeyboard.bAddInto = 0;
				gstKeyboard.u8Box = 0;
				GUI_InputBoxID(gstKeyboard.u8Box);
				LCD_DisplayStr(160, 280, FONT_SIZE16, pStr_Notice255IsAll, LCD_Color_Red);
				gstKeyboard.u8FourMenuStep ++;
			}
			break;	
			
			case 1: //输入箱号-->锁箱
			{
				if(Key_Enter == gstKeyboard.u8KeyValue)
				{
					if(MARK_CHOICE_ALL_BOX == gstKeyboard.u8Box)
					{
						GUI_ShowNotice(NOTICE_PLS_WAIT);
						for(i=1; i<=SysConfig.stLockers.u8BoxCount; i++)
						{
							WDT_Feed();
							gstKeyboard.u8Box = i;							
							SysConfig.stBox[gstKeyboard.u8Box].bLocking = LSTATUS_LOCKING;
							iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[gstKeyboard.u8Box].bLocking - (uint32)&SysConfig);
							Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[gstKeyboard.u8Box].bLocking, 1);
						}
						GUI_ShowNotice(NOTICE_LOCKING_BOX_OK);
						UpdateEmptyBoxList(); 
						Buzzer(600);
						delay_ms(NOTICE_STAY_TIME);	
						gstKeyboard.u8FourMenuStep = 0;
						gstKeyboard.u8ThreeMenuStep = 0;
						gstKeyboard.bAddInto = 1;
					}
					else if((gstKeyboard.u8Box)&&(gstKeyboard.u8Box <= MAX_BOX_NUM))
					{
						SysConfig.stBox[gstKeyboard.u8Box].bLocking = LSTATUS_LOCKING;
						iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[gstKeyboard.u8Box].bLocking - (uint32)&SysConfig);
						Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[gstKeyboard.u8Box].bLocking, 1);
						
						GUI_ShowNotice(NOTICE_LOCKING_BOX_OK);
						UpdateEmptyBoxList(); 
						Buzzer(600);
						delay_ms(NOTICE_STAY_TIME);	
						gstKeyboard.u8FourMenuStep = 0;
						gstKeyboard.bAddInto = 1;
					}
					else
					{
						Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
						GUI_ShowNotice(NOTICE_BOX_ID_ERR);
						delay_ms(NOTICE_STAY_TIME);	
						gstKeyboard.u8FourMenuStep = 0;
						gstKeyboard.bAddInto = 1;
					}
				}	
				else 
				{
					if((gstKeyboard.u8KeyValue >= Key_0)&&(gstKeyboard.u8KeyValue <= Key_9))
					{
						gstKeyboard.u8Box = KeyChangeNumValue(gstKeyboard.u8Box, gstKeyboard.u8KeyValue, MARK_CHOICE_ALL_BOX);
						if(gstKeyboard.u8Box)
						{
							GUI_AskLockinBox(gstKeyboard.u8Box);
						}
						else
						{
							GUI_InputBoxID(gstKeyboard.u8Box);
							LCD_DisplayStr(160, 280, FONT_SIZE16, pStr_Notice255IsAll, LCD_Color_Red);
						}
					}
				}		
			}
			break;
			
			default:
			{
				gstKeyboard.u8FourMenuStep = 0;
			}
			break;
		}
	}
}

/***********************************************************************************************
* Function Name  : Admin_Unlockin1Box
* Description	 : 管理员登录-->解锁
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : 主菜单-->箱门-->解锁-->解锁操作(属于第四级菜单)
***********************************************************************************************/
void Unlockin_Box(void)
{
	uint32 iAddr = 0;
	int i = 0;
	extern const char *pStr_Notice255IsAll;

	if(Key_Cancel == gstKeyboard.u8KeyValue)
	{	
		if(1==gstKeyboard.u8FourMenuStep)
		{
			gstKeyboard.u8ThreeMenuStep = 0;
		}
		gstKeyboard.u8FourMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8FourMenuStep)
		{
			case 0: //显示界面
			{
				gstKeyboard.bAddInto = 0;
				gstKeyboard.u8Box = 0;
				GUI_InputBoxID(gstKeyboard.u8Box);
				LCD_DisplayStr(160, 280, FONT_SIZE16, pStr_Notice255IsAll, LCD_Color_Red);
				gstKeyboard.u8FourMenuStep ++;
			}
			break;	
			
			case 1: //输入箱号-->解锁
			{
				if(Key_Enter == gstKeyboard.u8KeyValue)
				{
					if(MARK_CHOICE_ALL_BOX == gstKeyboard.u8Box)
					{
						GUI_ShowNotice(NOTICE_PLS_WAIT);
						for(i=1; i<=SysConfig.stLockers.u8BoxCount; i++)
						{
							WDT_Feed();
							gstKeyboard.u8Box = i;							
							SysConfig.stBox[gstKeyboard.u8Box].bLocking = LSTATUS_FREE;
							iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[gstKeyboard.u8Box].bLocking - (uint32)&SysConfig);
							Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[gstKeyboard.u8Box].bLocking, 1);

						}
						GUI_ShowNotice(NOTICE_UNLOCKING_BOX_OK);
						UpdateEmptyBoxList(); 
						Buzzer(600);
						delay_ms(NOTICE_STAY_TIME);	
						gstKeyboard.u8FourMenuStep = 0;
						gstKeyboard.u8ThreeMenuStep = 0;
						gstKeyboard.bAddInto = 1;
					}
					else if((gstKeyboard.u8Box)&&(gstKeyboard.u8Box <= MAX_BOX_NUM))
					{
						SysConfig.stBox[gstKeyboard.u8Box].bLocking = LSTATUS_FREE;
						iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[gstKeyboard.u8Box].bLocking - (uint32)&SysConfig);
						Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[gstKeyboard.u8Box].bLocking, 1);
						
						GUI_ShowNotice(NOTICE_UNLOCKING_BOX_OK);
						UpdateEmptyBoxList(); 
						Buzzer(600);
						delay_ms(NOTICE_STAY_TIME);	
						gstKeyboard.u8FourMenuStep = 0;
						gstKeyboard.bAddInto = 1;
					}
					else
					{
						Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
						GUI_ShowNotice(NOTICE_BOX_ID_ERR);
						delay_ms(NOTICE_STAY_TIME);	
						gstKeyboard.u8FourMenuStep = 0;
						gstKeyboard.bAddInto = 1;
					}
				}	
				else 
				{
					if((gstKeyboard.u8KeyValue >= Key_0)&&(gstKeyboard.u8KeyValue <= Key_9))
					{
						gstKeyboard.u8Box = KeyChangeNumValue(gstKeyboard.u8Box, gstKeyboard.u8KeyValue, MARK_CHOICE_ALL_BOX);
						if(gstKeyboard.u8Box)
						{
							GUI_AskUnlockinBox(gstKeyboard.u8Box);
						}
						else
						{
							GUI_InputBoxID(gstKeyboard.u8Box);
							LCD_DisplayStr(160, 280, FONT_SIZE16, pStr_Notice255IsAll, LCD_Color_Red);
						}
					}
				}		
			}
			break;
			
			default:
			{
				gstKeyboard.u8FourMenuStep = 0;
			}
			break;
		}
	}
}

/***********************************************************************************************
* Function Name  : Query_BoxRecord
* Description	 : 箱门记录查询
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : 主菜单-->查询-->记录-->箱门记录查询(属于第四级菜单)
***********************************************************************************************/
void Query_BoxRecord(void)
{
	static uchar8 ChoicePage;

	if(Key_Cancel == gstKeyboard.u8KeyValue)
	{	
		gstKeyboard.u8FourMenuStep = 0;
		gstKeyboard.u8ThreeMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8FourMenuStep)
		{
			case 0: //显示界面
			{
				gstKeyboard.bAddInto = 0;
				gstKeyboard.u8Box = 0;
				GUI_InputBoxID(gstKeyboard.u8Box);
				gstKeyboard.u8FourMenuStep ++;
			}
			break;	
			
			case 1: //输入箱号
			{
				if(Key_Enter == gstKeyboard.u8KeyValue)
				{
					if(gstKeyboard.u8Box)
					{
						gstKeyboard.bAddInto = 0;		
						ChoicePage = 0;
						GUI_QueryBoxRecord(gstKeyboard.u8Box, 0);
						gstKeyboard.u8FourMenuStep ++;
					}
				}
				else 
				{
					if((gstKeyboard.u8KeyValue >= Key_0)&&(gstKeyboard.u8KeyValue <= Key_9))
					{
						gstKeyboard.u8Box = KeyChangeNumValue(gstKeyboard.u8Box, gstKeyboard.u8KeyValue, MAX_BOX_NUM);
						GUI_InputBoxID(gstKeyboard.u8Box);
					}
				}				
			}
			break;

			case 2: //上下翻页
			{
				if((gstKeyboard.u8KeyValue >= Key_1)&&(gstKeyboard.u8KeyValue <= Key_2))
				{
					if(gstKeyboard.u8KeyValue == Key_1)//上一页
					{
						if(ChoicePage > 0)
						{

							ChoicePage --;
							GUI_QueryBoxRecord(gstKeyboard.u8Box, ChoicePage);
						}
					}
					else if(gstKeyboard.u8KeyValue == Key_2)//下一页
					{
						if((ChoicePage +1)* 4 < SysConfig.stBox[gstKeyboard.u8Box].iRecordCount)
						{
							ChoicePage ++;
							GUI_QueryBoxRecord(gstKeyboard.u8Box, ChoicePage);
						}
					}
				}
			}
			break;	
			
			default:
			{
				gstKeyboard.u8FourMenuStep = 0;
			}
			break;
		}
	}
}

/***********************************************************************************************
* Function Name  : Query_LockBox
* Description	 : 箱门锁箱查询
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : 主菜单-->查询-->锁箱-->锁箱查询(属于第四级菜单)
***********************************************************************************************/
void Query_LockBox(void)
{
	uint32 i=0;
	
	if(Key_Cancel == gstKeyboard.u8KeyValue)
	{	
		gstKeyboard.u8ThreeMenuStep = 0;
		gstKeyboard.u8FourMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8FourMenuStep)
		{
			case 0: //显示界面
			{
				gstKeyboard.bAddInto = 0;
				gstKeyboard.iTempValue = 0;
				
				for(i=1; i<=SysConfig.stLockers.u8BoxCount; i++)
				{
					if(LSTATUS_LOCKING == SysConfig.stBox[i].bLocking)
					{
						gstKeyboard.iTempValue ++;
					}
				}		
				
				GUI_QueryLockBoxList(0);
				gstKeyboard.u8TempValue = 0;
				gstKeyboard.u8FourMenuStep ++;
			}
			break;	

			case 1: // 上下翻页
			{
				if((gstKeyboard.u8KeyValue >= Key_1)&&(gstKeyboard.u8KeyValue <= Key_2))
				{					
					if(gstKeyboard.u8KeyValue == Key_1)//上一页
					{
						if(gstKeyboard.u8TempValue)
						{
							gstKeyboard.u8TempValue --;
							GUI_QueryLockBoxList(gstKeyboard.u8TempValue);
						}
					}
					else if(gstKeyboard.u8KeyValue == Key_2)//下一页
					{
						if(gstKeyboard.iTempValue > ((gstKeyboard.u8TempValue +1) * 24))
						{
							gstKeyboard.u8TempValue ++;
							GUI_QueryLockBoxList(gstKeyboard.u8TempValue);
						}
					}
				}
			}
			break;
			
			default:
			{
				gstKeyboard.u8FourMenuStep = 0;
			}
			break;
		}
	}
}

/***********************************************************************************************
* Function Name  : Query_BoxStatus
* Description	 : 箱门状态查询
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : 主菜单-->查询-->状态-->箱门状态查询(属于第四级菜单)
***********************************************************************************************/
void Query_BoxStatus(void)
{
	if(Key_Cancel == gstKeyboard.u8KeyValue)
	{	
		gstKeyboard.u8FourMenuStep = 0;
		gstKeyboard.u8ThreeMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8FourMenuStep)
		{
			case 0: //显示界面
			{
				gstKeyboard.bAddInto = 0;
				gstKeyboard.u8TempValue = 0;

				GUI_QueryBoxStatusList(0);
				gstKeyboard.u8FourMenuStep ++;		
			}
			break;	

			case 1: //上下翻页
			{
				if((gstKeyboard.u8KeyValue >= Key_1)&&(gstKeyboard.u8KeyValue <= Key_2))
				{					
					if(gstKeyboard.u8KeyValue == Key_1)//上一页
					{						
						if(gstKeyboard.u8TempValue)
						{
							gstKeyboard.u8TempValue --;
						}
					}
					else if(gstKeyboard.u8KeyValue == Key_2)//下一页
					{
						if((gstKeyboard.u8TempValue +1)*4 < SysConfig.stLockers.u8BoxCount)
						{
							gstKeyboard.u8TempValue ++;
						}
					}
					GUI_QueryBoxStatusList(gstKeyboard.u8TempValue);
				}
			}
			break;
			
			default:
			{
				gstKeyboard.u8FourMenuStep = 0;
			}
			break;
		}
	}
}

/***********************************************************************************************
* Function Name  : TopAdmin_SetPassword
* Description	 : 超级管理员修改管理员密码
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : 登录密码管理(1.超管, 2.高级管理员密码, 3.用户管理员密码)
***********************************************************************************************/
void TopAdmin_SetPassword(void)
{
	uint32 iAddr = 0;
	
	if(Key_Cancel == gstKeyboard.u8KeyValue)
	{	
		if(gstKeyboard.u8FourMenuStep <= 1)
		{
			gstKeyboard.u8ThreeMenuStep = 0;
		}
		gstKeyboard.u8FourMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8FourMenuStep)
		{
			case 0: //显示选择界面
			{
				gstKeyboard.bAddInto = 0;
				GUI_ChoiceAdminMenu();

				gstKeyboard.u8KeyCount = 0;
				memset((uchar8*)gstKeyboard.KeyBuf, 0x0, sizeof(gstKeyboard.KeyBuf));
				memset((uchar8*)gstKeyboard.tempBuf, 0x0, sizeof(gstKeyboard.tempBuf));
				gstKeyboard.u8FourMenuStep ++;
			}
			break;
			
			case 1: //选择-->1.用户管理员密码 2.高级管理员密码  
			{
				if(Key_1 == gstKeyboard.u8KeyValue)
				{
					gstKeyboard.u8FourMenuStep = 3;					
					GUI_ChangePassword((uchar8*)SysConfig.stLockers.AdminPassword, (uchar8*)gstKeyboard.KeyBuf);
				}
				else if(Key_2 == gstKeyboard.u8KeyValue)
				{
					gstKeyboard.u8FourMenuStep = 2;					
					GUI_ChangePassword((uchar8*)SysConfig.stLockers.SeniorPassword, (uchar8*)gstKeyboard.KeyBuf);
				}
			}
			break;
			
			case 2: //选择-->2.高级管理员密码 
			{
				if(Key_Enter == gstKeyboard.u8KeyValue)
				{
					if(6 == gstKeyboard.u8KeyCount)  //固定6位密码
					{
						memset((uchar8*)SysConfig.stLockers.SeniorPassword, 0x0, sizeof(SysConfig.stLockers.SeniorPassword));
						memcpy((uchar8*)SysConfig.stLockers.SeniorPassword, (uchar8*)gstKeyboard.KeyBuf, 6);
						//保存数据&&提示设置成功						
						iAddr = Addr_SysConfig + ((uint32)&SysConfig.stLockers.SeniorPassword[0] - (uint32)&SysConfig);
						Store_Parameters(iAddr, (uchar8*)&SysConfig.stLockers.SeniorPassword[0], sizeof(SysConfig.stLockers.SeniorPassword));
						GUI_ShowNotice(NOTICE_SET_OK);
						Buzzer(600);
						delay_ms(1234);	
						gstKeyboard.u8FourMenuStep = 0;
						gstKeyboard.bAddInto = 1;
					}
				}
				else if((Key_Star == gstKeyboard.u8KeyValue)||((Key_Jin == gstKeyboard.u8KeyValue)))
				{			
					if(gstKeyboard.u8KeyCount)
					{
						gstKeyboard.u8KeyCount --;
						gstKeyboard.KeyBuf[gstKeyboard.u8KeyCount] = 0;
						GUI_ChangePassword((uchar8*)SysConfig.stLockers.SeniorPassword, (uchar8*)gstKeyboard.KeyBuf);
					}
				}
				else if(gstKeyboard.u8KeyCount < 6)
				{
					if((gstKeyboard.u8KeyValue >= Key_0)&&(gstKeyboard.u8KeyValue <= Key_9))
					{
						gstKeyboard.KeyBuf[gstKeyboard.u8KeyCount] = gstKeyboard.u8NumValue + 0x30;
						gstKeyboard.u8KeyCount ++;
						GUI_ChangePassword((uchar8*)SysConfig.stLockers.SeniorPassword, (uchar8*)gstKeyboard.KeyBuf);
					}
				}
			}
			break;
			
			case 3: //选择-->3.用户管理员密码  
			{				
				if(Key_Enter == gstKeyboard.u8KeyValue)
				{
					memset((uchar8*)SysConfig.stLockers.AdminPassword, 0x0, sizeof(SysConfig.stLockers.AdminPassword));
					memcpy((uchar8*)SysConfig.stLockers.AdminPassword, (uchar8*)gstKeyboard.KeyBuf, 6);

					//保存数据&&提示设置成功						
					iAddr = Addr_SysConfig + ((uint32)&SysConfig.stLockers.AdminPassword[0] - (uint32)&SysConfig);
					Store_Parameters(iAddr, (uchar8*)&SysConfig.stLockers.AdminPassword[0], sizeof(SysConfig.stLockers.AdminPassword));
					GUI_ShowNotice(NOTICE_SET_OK);
					Buzzer(600);
					delay_ms(1234);	
					gstKeyboard.u8FourMenuStep = 0;
					gstKeyboard.bAddInto = 1;
				}
				else if((Key_Star == gstKeyboard.u8KeyValue)||((Key_Jin == gstKeyboard.u8KeyValue)))
				{			
					if(gstKeyboard.u8KeyCount)
					{
						gstKeyboard.u8KeyCount --;
						gstKeyboard.KeyBuf[gstKeyboard.u8KeyCount] = 0;						
						GUI_ChangePassword((uchar8*)SysConfig.stLockers.AdminPassword, (uchar8*)gstKeyboard.KeyBuf);
					}
				}
				else if(gstKeyboard.u8KeyCount < 6)
				{				
					if((gstKeyboard.u8KeyValue >= Key_0)&&(gstKeyboard.u8NumValue <= Key_9))
					{
						gstKeyboard.KeyBuf[gstKeyboard.u8KeyCount] = gstKeyboard.u8NumValue + 0x30;
						gstKeyboard.u8KeyCount ++;
						GUI_ChangePassword((uchar8*)SysConfig.stLockers.AdminPassword, (uchar8*)gstKeyboard.KeyBuf);
					}
				}
			}
			break;
			
			default:
			{
				gstKeyboard.u8FourMenuStep = 0;
			}
			break;
		}
	}
}

/***********************************************************************************************
* Function Name  : Admin_SysSetMenu
* Description	 : 用户管理员--><系统>操作菜单
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : None
***********************************************************************************************/
void Admin_SysSetMenu(void)
{
	if((Key_Cancel == gstKeyboard.u8KeyValue)&&(1 == gstKeyboard.u8ThreeMenuStep))
	{		
		gstKeyboard.u8SecondMenuStep = 0;
		gstKeyboard.u8ThreeMenuStep = 0;
		gstKeyboard.u8FourMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8ThreeMenuStep)
		{
			case 0: //显示选择界面
			{
				gstKeyboard.bAddInto = 0;
				GUI_Admin_SysSetMenu();
				gstKeyboard.u8FourMenuStep = 0;
				gstKeyboard.u8ThreeMenuStep ++;
			}
			break;
			
			case 1: //选择要设置的参数选项
			{
				switch(gstKeyboard.u8KeyValue)
				{
					case Key_0: //退出
					{
						gstKeyboard.u8SecondMenuStep = 0;
						gstKeyboard.u8ThreeMenuStep = 0;
						gstKeyboard.u8FourMenuStep = 0;
						gstKeyboard.bAddInto = 1;
					}
					break;
					
					case Key_1: //选择-->模式
					{						
					//	gstKeyboard.u8ThreeMenuStep = 2;
					//	gstKeyboard.bAddInto = 1;						
					}
					break;
				
					case Key_2: //选择-->时钟
					{
						gstKeyboard.u8ThreeMenuStep = 3;
						gstKeyboard.bAddInto = 1;	
					}
					break;
				
					case Key_3: //选择-->语音
					{
						gstKeyboard.u8ThreeMenuStep = 4;
						gstKeyboard.bAddInto = 1;	
					}
					break;
				
					case Key_4: //选择-->机号
					{
						gstKeyboard.u8ThreeMenuStep = 5;
						gstKeyboard.bAddInto = 1;	
					}
					break;

					case Key_5: //选择-->网络
					{
						gstKeyboard.u8ThreeMenuStep = 6;
						gstKeyboard.bAddInto = 1;	
					}
					break;

					case Key_6: //选择-->TCP\IP
					{
						gstKeyboard.u8ThreeMenuStep = 7;
						gstKeyboard.bAddInto = 1;	
					}
					break;

					case Key_7: //选择-->修复
					{
						gstKeyboard.u8ThreeMenuStep = 8;
						gstKeyboard.bAddInto = 1;	
					}
					break;

					default:
					{
						gstKeyboard.u8ThreeMenuStep = 0;
					}
					break;
				}
			}
			break;
			
			case 2:  
			{
				Set_WorkMode();
			}
			break;

 			case 3: 
 			{
				Set_DateTime();
			}
			break;

			case 4:  
			{
				Set_VoiceModuleLevel();
			}
			break;

			case 5:  
			{
				Set_MachineNum();
			}
			break;

			case 6:  
			{
				Set_NetSwitch(); 
			}
			break;

			case 7:  
			{
				Set_TcpIpInfo();
			}
			break;

			case 8:  
			{
				Sys_Registration();
			}
			break;

			default:
			{
				gstKeyboard.u8ThreeMenuStep = 0;
			}
			break;
		}
	}
}

/***********************************************************************************************
* Function Name  : Admin_ManageMenu
* Description	 : 用户管理员--><管理>操作菜单
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : None
***********************************************************************************************/
void Admin_ManageMenu(void)
{
	if((Key_Cancel == gstKeyboard.u8KeyValue)&&(1 == gstKeyboard.u8ThreeMenuStep))
	{		
		gstKeyboard.u8SecondMenuStep = 0;
		gstKeyboard.u8ThreeMenuStep = 0;
		gstKeyboard.u8FourMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8ThreeMenuStep)
		{
			case 0: //显示选择界面
			{
				gstKeyboard.bAddInto = 0;
				GUI_Admin_ManageMenu();
				gstKeyboard.u8FourMenuStep = 0;
				gstKeyboard.u8ThreeMenuStep ++;
			}
			break;
			
			case 1: //选择要设置的参数选项
			{
				switch(gstKeyboard.u8KeyValue)
				{
					case Key_0: //退出
					{
						gstKeyboard.u8SecondMenuStep = 0;
						gstKeyboard.u8ThreeMenuStep = 0;
						gstKeyboard.u8FourMenuStep = 0;
						gstKeyboard.bAddInto = 1;
					}
					break;
					
					case Key_1: //选择-->管密 
					{						
						gstKeyboard.u8ThreeMenuStep = 2;
						gstKeyboard.bAddInto = 1;						
					}
					break;
				
					case Key_2: //选择-->管理卡
					{						
						gstKeyboard.u8ThreeMenuStep = 3;
						gstKeyboard.bAddInto = 1;						
					}
					break;
				
					case Key_3: //选择-->柜名
					{						
						gstKeyboard.u8ThreeMenuStep = 4;
						gstKeyboard.bAddInto = 1;						
					}
					break;				
					
					case Key_4: //选择-->标识码
					{						
						gstKeyboard.u8ThreeMenuStep = 5;
						gstKeyboard.bAddInto = 1;						
					}
					break;				
					
					case Key_5: //选择-->卡扇区号
					{						
						gstKeyboard.u8ThreeMenuStep = 6;
						gstKeyboard.bAddInto = 1;						
					}
					break;				
					
					case Key_6: //选择-->预授权卡
					{						
						gstKeyboard.u8ThreeMenuStep = 7;
						gstKeyboard.bAddInto = 1;						
					}
					break;		
					
					case Key_7: //选择-->清除所有预授权卡
					{						
						gstKeyboard.u8ThreeMenuStep = 8;
						gstKeyboard.bAddInto = 1;						
					}
					break;		
					
					default:
					break;
				}
			}
			break;
			
			case 2:  
			{
				Set_AdminPassword();
			}
			break;

			case 3:
			{
				Set_AdminCard();
			}
			break;

			case 4:
			{
				Set_LockersName();
			}
			break;		

			case 5:
			{
				Set_ClientIdCode();
			}
			break;

			case 6:
			{
				Set_CardSector();
			}
			break;
			
			case 7:
			{
				Add_ValidCard();
			}
			break;
			
			case 8:
			{
				Clr_AllValidCard();
			}
			break;
			
			default:
			{
				gstKeyboard.u8ThreeMenuStep = 0;
			}
			break;
		}
	}
}

/***********************************************************************************************
* Function Name  : Admin_BoxMenu
* Description	 : 用户管理员--><箱门>操作菜单
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : None
***********************************************************************************************/
void Admin_BoxMenu(void)
{
	if((Key_Cancel == gstKeyboard.u8KeyValue)&&(1 == gstKeyboard.u8ThreeMenuStep))
	{		
		gstKeyboard.u8SecondMenuStep = 0;
		gstKeyboard.u8ThreeMenuStep = 0;
		gstKeyboard.u8FourMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8ThreeMenuStep)
		{
			case 0: //显示选择界面
			{
				gstKeyboard.bAddInto = 0;
				GUI_Admin_BoxMenu();
				gstKeyboard.u8FourMenuStep = 0;
				gstKeyboard.u8ThreeMenuStep ++;
			}
			break;
			
			case 1: //选择要设置的参数选项
			{
				switch(gstKeyboard.u8KeyValue)
				{
					case Key_0: //退出
					{
						gstKeyboard.u8SecondMenuStep = 0;
						gstKeyboard.u8ThreeMenuStep = 0;
						gstKeyboard.u8FourMenuStep = 0;
						gstKeyboard.bAddInto = 1;
					}
					break;
					
					case Key_1: //选择-->单开 
					{						
						gstKeyboard.u8ThreeMenuStep = 2;
						gstKeyboard.bAddInto = 1;						
					}
					break;
				
					case Key_2: //选择-->全开 
					{
						gstKeyboard.u8ThreeMenuStep = 3;
						gstKeyboard.bAddInto = 1;	
					}
					break;

					case Key_3: //选择-->单清 
					{
						gstKeyboard.u8ThreeMenuStep = 4;
						gstKeyboard.bAddInto = 1;	
					}
					break;

					case Key_4: //选择-->全清 
					{
						gstKeyboard.u8ThreeMenuStep = 5;
						gstKeyboard.bAddInto = 1;	
					}
					break;

					case Key_5: //选择-->锁箱 
					{
						gstKeyboard.u8ThreeMenuStep = 6;
						gstKeyboard.bAddInto = 1;	
					}
					break;

					case Key_6: //选择-->解锁 
					{
						gstKeyboard.u8ThreeMenuStep = 7;
						gstKeyboard.bAddInto = 1;	
					}
					break;

					default:
					break;
				}
			}
			break;
			
			case 2:  
			{
				Open_OneBox();
			}
			break;

			case 3: 
			{
				Open_AllBox();
			}
			break;

			case 4: 
			{
				Clean_OneBox();
			}
			break;

			case 5: 
			{
				Clean_AllBox();
			}
			break;

			case 6: 
			{
				Lockin_Box();
			}
			break;

			case 7: 
			{
				Unlockin_Box();
			}
			break;

			default:
			{
				gstKeyboard.u8ThreeMenuStep = 0;
			}
			break;
		}
	}
}

/***********************************************************************************************
* Function Name  : Admin_LockersMenu
* Description	 : 用户管理员--><寄存>操作菜单
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : None
***********************************************************************************************/
void Admin_LockersMenu(void)
{
	if((Key_Cancel == gstKeyboard.u8KeyValue)&&(1 == gstKeyboard.u8ThreeMenuStep))
	{		
		gstKeyboard.u8SecondMenuStep = 0;
		gstKeyboard.u8ThreeMenuStep = 0;
		gstKeyboard.u8FourMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8ThreeMenuStep)
		{
			case 0: //显示选择界面
			{
				gstKeyboard.bAddInto = 0;
				GUI_Admin_LockersMenu();
				gstKeyboard.u8FourMenuStep = 0;
				gstKeyboard.u8ThreeMenuStep ++;
			}
			break;
			
			case 1: //选择要设置的参数选项
			{
				switch(gstKeyboard.u8KeyValue)
				{
					case Key_0: //退出
					{
						gstKeyboard.u8SecondMenuStep = 0;
						gstKeyboard.u8ThreeMenuStep = 0;
						gstKeyboard.u8FourMenuStep = 0;
						gstKeyboard.bAddInto = 1;
					}
					break;
					
					case Key_1: //选择-->开箱数 
					{						
						gstKeyboard.u8ThreeMenuStep = 2;
						gstKeyboard.bAddInto = 1;						
					}
					break;
				
					case Key_2: //选择-->超时(最大寄存时间)
					{						
						gstKeyboard.u8ThreeMenuStep = 3;
						gstKeyboard.bAddInto = 1;						
					}
					break;
				
					case Key_3: //选择-->延时取(最小寄存时间)
					{						
						gstKeyboard.u8ThreeMenuStep = 4;
						gstKeyboard.bAddInto = 1;						
					}
					break;
				
					case Key_4: //选择-->超时提醒时间
					{						
						gstKeyboard.u8ThreeMenuStep = 5;
						gstKeyboard.bAddInto = 1;						
					}
					break;
				
					default:
					break;
				}
			}
			break;
			
			case 2:  
			{
				Set_AllowOpenBoxNum();
			}
			break;
			
			case 3:  
			{
				Set_PickUpTimeout();
			}
			break;

			case 4:
			{
				Set_PickUpAwaitTime();
			}
			break;

			case 5:
			{
				Set_HandleTimeout();
			}
			break;
			
			default:
			{
				gstKeyboard.u8ThreeMenuStep = 0;
			}
			break;
		}
	}
}

/***********************************************************************************************
* Function Name  : Admin_QueryMenu
* Description	 : 用户管理员--><查询>操作菜单
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : None
***********************************************************************************************/
void Admin_QueryMenu(void)
{
	if((Key_Cancel == gstKeyboard.u8KeyValue)&&(1 == gstKeyboard.u8ThreeMenuStep))
	{		
		gstKeyboard.u8SecondMenuStep = 0;
		gstKeyboard.u8ThreeMenuStep = 0;
		gstKeyboard.u8FourMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8ThreeMenuStep)
		{
			case 0: //显示选择界面
			{
				gstKeyboard.bAddInto = 0;
				GUI_Admin_QueryMenu();
				gstKeyboard.u8FourMenuStep = 0;
				gstKeyboard.u8ThreeMenuStep ++;
			}
			break;
			
			case 1:  
			{
				switch(gstKeyboard.u8KeyValue)
				{
					case Key_0: //退出
					{
						gstKeyboard.u8SecondMenuStep = 0;
						gstKeyboard.u8ThreeMenuStep = 0;
						gstKeyboard.u8FourMenuStep = 0;
						gstKeyboard.bAddInto = 1;
					}
					break;
					
					case Key_1: //选择-->(开箱)记录 
					{						
						gstKeyboard.u8ThreeMenuStep = 2;
						gstKeyboard.bAddInto = 1;						
					}
					break;

					case Key_2: //选择-->版本
					{
						gstKeyboard.u8ThreeMenuStep = 3;
						gstKeyboard.bAddInto = 1;	
					}
					break;

					case Key_3: //选择-->机号
					{
						gstKeyboard.u8ThreeMenuStep = 4;
						gstKeyboard.bAddInto = 1;	
					}
					break;
					
					case Key_4: //选择-->柜名称
					{
						gstKeyboard.u8ThreeMenuStep = 5;
						gstKeyboard.bAddInto = 1;	
					}
					break;
					
					case Key_5: //选择-->系统标识码
					{
						gstKeyboard.u8ThreeMenuStep = 6;
						gstKeyboard.bAddInto = 1;	
					}
					break;
					
					case Key_6: //选择-->锁箱
					{
						gstKeyboard.u8ThreeMenuStep = 7;
						gstKeyboard.bAddInto = 1;	
					}
					break;
					
					case Key_7: //选择-->箱状态
					{
						gstKeyboard.u8ThreeMenuStep = 8;
						gstKeyboard.bAddInto = 1;	
					}
					break;
					
					default:
					break;
				}
			}
			break;
			
			case 2: //查询-->记录
			{
				Query_BoxRecord();
			}
			break;

			case 3: //查询-->版本
			{
				gstKeyboard.bAddInto = 0;
				if(Key_Cancel == gstKeyboard.u8KeyValue)
				{		
					GUI_Admin_QueryMenu();
					gstKeyboard.u8ThreeMenuStep =1;
				}
				else
				{	
					GUI_ShowSysVersion();
				}
			}
			break;
			
			case 4: //查询-->机号
			{
				gstKeyboard.bAddInto = 0;
				if(Key_Cancel == gstKeyboard.u8KeyValue)
				{		
					GUI_Admin_QueryMenu();
					gstKeyboard.u8ThreeMenuStep =1;
				}
				else
				{	
					memset(cStr_Message, 0x0, sizeof(cStr_Message));
					sprintf(cStr_Message, "%d", SysConfig.stHardware.u32MachineNum);
					GUI_ShowMessage(cStr_Message, LCD_Color_Red);
				}
			}
			break;

			case 5:
			{
				gstKeyboard.bAddInto = 0;
				if(Key_Cancel == gstKeyboard.u8KeyValue)
				{		
					GUI_Admin_QueryMenu();
					gstKeyboard.u8ThreeMenuStep =1;
				}
				else
				{	
					GUI_ShowTowRowMessage("Name:", (char*)SysConfig.stLockers.Name, LCD_Color_Blue);
				}
			}
			break;

			case 6:
			{
				gstKeyboard.bAddInto = 0;
				if(Key_Cancel == gstKeyboard.u8KeyValue)
				{		
					GUI_Admin_QueryMenu();
					gstKeyboard.u8ThreeMenuStep =1;
				}
				else
				{	
					GUI_ShowTowRowMessage("IdCode:", (char*)SysConfig.stClientInfo.IdCode, LCD_Color_Blue);
				}
			}
			break;

			case 7:
			{
				Query_LockBox();
			}
			break;

			case 8:
			{
				Query_BoxStatus();
			}
			break;
			
			default:
			{
				gstKeyboard.u8ThreeMenuStep = 0;
			}
			break;
		}
	}
}

/***********************************************************************************************
* Function Name  : Admin_LoginMenu
* Description	 : 用户管理员登录操作
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : None
***********************************************************************************************/
void Admin_LoginMenu(void)
{
	if((Key_Cancel == gstKeyboard.u8KeyValue)&&(0 != gstKeyboard.u8SecondMenuStep)&&(0 == gstKeyboard.u8ThreeMenuStep))
	{		
		gstKeyboard.u8SecondMenuStep = 0;
		gstKeyboard.u8AdminMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8SecondMenuStep)
		{
			case 0: //显示登录界面
			{
				gstKeyboard.bAddInto = 0;
				GUI_AdminMenu();	
				gstKeyboard.u8SecondMenuStep ++;
			}
			break;

			case 1: //选择设置项
			{				
				switch(gstKeyboard.u8KeyValue)
				{
					case Key_0: //选择-->退出
					{
						gstKeyboard.u8SecondMenuStep = 0;
						gstKeyboard.u8AdminMenuStep = 0;
						gstKeyboard.bAddInto = 1;
					}
					break;
					
					case Key_1: //选择-->系统 
					{
						gstKeyboard.u8SecondMenuStep = 2;
						gstKeyboard.u8ThreeMenuStep = 0;
						gstKeyboard.bAddInto = 1;
					}
					break;
				
					case Key_2: //选择-->管理 
					{						
						gstKeyboard.u8SecondMenuStep = 3;
						gstKeyboard.u8ThreeMenuStep = 0;
						gstKeyboard.bAddInto = 1;
					}
					break;
				
					case Key_3: //选择-->箱门
					{						
						gstKeyboard.u8SecondMenuStep = 4;
						gstKeyboard.u8ThreeMenuStep = 0;
						gstKeyboard.bAddInto = 1;
					}
					break;
				
					case Key_4: //选择-->寄存
					{						
						gstKeyboard.u8SecondMenuStep = 5;
						gstKeyboard.u8ThreeMenuStep = 0;
						gstKeyboard.bAddInto = 1;
					}
					break;

					case Key_5: //选择-->查询
					{						
						gstKeyboard.u8SecondMenuStep = 6;
						gstKeyboard.u8ThreeMenuStep = 0;
						gstKeyboard.bAddInto = 1;
					}
					break;
					
					default:
					{
					}
					break;
				}
			}
			break;
			
			case 2:  
			{
				Admin_SysSetMenu();	 
			}
			break;

			case 3: 
			{
				 Admin_ManageMenu();
			}
			break;

			case 4: 
			{
				Admin_BoxMenu();	 				
			}
			break;
		
			case 5: 
			{
				Admin_LockersMenu();
			}
			break;

			case 6: 
			{
				Admin_QueryMenu(); 
			}
			break;

			default:
			{
				gstKeyboard.u8SecondMenuStep = 0;
				gstKeyboard.u8AdminMenuStep = 0;
			}
			break;
		}
	}
}

/***********************************************************************************************
* Function Name  : Admin_Login
* Description	 : 用户帐号登录操作
* Input 		 : 0--按键操作, 0xCC--刷卡进入
* Output		 : None
* Return		 : None 
* Notes          : 按键流程: #-->*-->000-->Enter-->PWD
                   刷卡流程: Card-->Enter-->PWD
***********************************************************************************************/
void Admin_Login(uchar8 Type)
{
	uchar8 Len = 0;
	
	if((Key_Cancel == gstKeyboard.u8KeyValue)&&(0 != gstKeyboard.u8SecondMenuStep)&&(0 == gstKeyboard.u8ThreeMenuStep))
	{		
		gstKeyboard.u8SecondMenuStep = 0;
		gstKeyboard.u8AdminMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8SecondMenuStep)
		{
			case 0:
			{
				if(Key_Star == gstKeyboard.u8KeyValue)	
				{
					gstKeyboard.bAddInto = 0;
					gstKeyboard.u8SecondMenuStep ++;	
				}
				else
				{
					memset((uchar8*)&gstKeyboard, 0x0, sizeof(gstKeyboard));
					gstKeyboard.bAddInto = 1;
				}
			}
			break;
		
			case 1:
			case 2:
			case 3:
			{
				if(Key_0 == gstKeyboard.u8KeyValue) 
				{
					gstKeyboard.u8SecondMenuStep ++;	
				}
				else
				{
					memset((uchar8*)&gstKeyboard, 0x0, sizeof(gstKeyboard));
					gstKeyboard.bAddInto = 1;
				}
			}
			break;
		
			case 4:
			{
				if((Key_Enter == gstKeyboard.u8KeyValue)||(0xCC == Type)) 
				{
					//提示输入密码
					Buzzer(800);
					Voice_Play(YY_QinShuRuMiMa);
					GUI_ShowPassword(0);

					memset((uchar8*)&gstKeyboard.KeyBuf[0], 0x0, sizeof(gstKeyboard.KeyBuf));
					gstKeyboard.u8KeyCount = 0;
					gstKeyboard.u8SecondMenuStep ++;	
				}
				else
				{
					memset((uchar8*)&gstKeyboard, 0x0, sizeof(gstKeyboard));
					gstKeyboard.bAddInto = 1;
				}

			}
			break;

			case 5: //输入登录密码
			{
				if(Key_Enter == gstKeyboard.u8KeyValue)
				{
					Len = strlen((char*)SysConfig.stLockers.AdminPassword);
					if(gstKeyboard.u8KeyCount > Len)
					{
						Len = gstKeyboard.u8KeyCount;
					}
					
					if(0 == memcmp((uchar8*)&SysConfig.stLockers.AdminPassword[0], (uchar8*)&gstKeyboard.KeyBuf[0], Len))
					{
						gstKeyboard.u8SecondMenuStep =0;
						gstKeyboard.bAddInto = 1;
						gstKeyboard.u8AdminMenuStep = 5;
					}
					else if(0 == memcmp((uchar8*)&SysConfig.stLockers.SeniorPassword[0], (uchar8*)&gstKeyboard.KeyBuf[0], 6))
					{
						gstKeyboard.u8SecondMenuStep =0;
						gstKeyboard.bAddInto = 1;
						gstKeyboard.u8AdminMenuStep = 3;
					}
					else
					{
						GUI_ShowNotice(NOTICE_PWD_ERR);
						Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 5);
						delay_ms(1234);
						gstKeyboard.u8SecondMenuStep = 0;
						gstKeyboard.u8AdminMenuStep = 0;
						gstKeyboard.bAddInto = 1;
					}
				}
				else if((Key_Star == gstKeyboard.u8KeyValue)||((Key_Jin == gstKeyboard.u8KeyValue)))
				{			
					if(gstKeyboard.u8KeyCount)
					{
						gstKeyboard.u8KeyCount --;
						gstKeyboard.KeyBuf[gstKeyboard.u8KeyCount] = 0;
						GUI_ShowPassword(gstKeyboard.u8KeyCount);
					}
				}
				else if((gstKeyboard.u8KeyValue >= Key_0)&&(gstKeyboard.u8NumValue <= Key_9)&&(gstKeyboard.u8KeyCount < 6))
				{
					gstKeyboard.KeyBuf[gstKeyboard.u8KeyCount] = gstKeyboard.u8NumValue + 0x30;
					gstKeyboard.u8KeyCount ++;
					GUI_ShowPassword(gstKeyboard.u8KeyCount);
				}
				else
				{
					//
				}
			}
			break;

			default:
			{
				gstKeyboard.u8SecondMenuStep = 0;
				gstKeyboard.u8AdminMenuStep = 0;
			}
			break;
		}
	}
}

/***********************************************************************************************
* Function Name  : TopAdmin_Login
* Description	 : 帐号登录操作
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : Y-->N-->000-->PWD
***********************************************************************************************/
void TopAdmin_Login(void)
{
	if((Key_Cancel == gstKeyboard.u8KeyValue)&&(0 != gstKeyboard.u8SecondMenuStep)&&(0 == gstKeyboard.u8ThreeMenuStep))
	{		
		gstKeyboard.u8SecondMenuStep = 0;
		gstKeyboard.u8AdminMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8SecondMenuStep)
		{
			case 0:
			{
				if(Key_Cancel == gstKeyboard.u8KeyValue)	
				{
					gstKeyboard.u8SecondMenuStep ++;	
				}
				else
				{
					memset((uchar8*)&gstKeyboard, 0x0, sizeof(gstKeyboard));
					gstKeyboard.bAddInto = 1;
				}
			}
			break;
		
			case 1:
			case 2:
			case 3:
			{
				if(Key_0 == gstKeyboard.u8KeyValue) 
				{
					gstKeyboard.u8SecondMenuStep ++;	
				}
				else
				{
					memset((uchar8*)&gstKeyboard, 0x0, sizeof(gstKeyboard));
					gstKeyboard.bAddInto = 1;
				}
			}
			break;
		
			case 4:
			{
				if(Key_Enter == gstKeyboard.u8KeyValue) 
				{
					//提示输入密码
					Buzzer(800);
					Voice_Play(YY_QinShuRuMiMa);
					GUI_ShowPassword(0);

					memset((uchar8*)&gstKeyboard.KeyBuf[0], 0x0, sizeof(gstKeyboard.KeyBuf));
					gstKeyboard.u8KeyCount = 0;
					gstKeyboard.u8SecondMenuStep ++;	
				}
				else
				{
					memset((uchar8*)&gstKeyboard, 0x0, sizeof(gstKeyboard));
					gstKeyboard.bAddInto = 1;
				}

			}
			break;

			case 5: //输入登录密码
			{
				if(Key_Enter == gstKeyboard.u8KeyValue)
				{
					if(0 == memcmp(SuperPassword, (uchar8*)&gstKeyboard.KeyBuf[0], 6))
					{
						gstKeyboard.u8SecondMenuStep =0;
						gstKeyboard.bAddInto = 1;
						gstKeyboard.u8AdminMenuStep = 2;
					}
					else if(0 == memcmp((uchar8*)&SysConfig.stLockers.SeniorPassword[0], (uchar8*)&gstKeyboard.KeyBuf[0], 6))
					{
						gstKeyboard.u8SecondMenuStep =0;
						gstKeyboard.bAddInto = 1;
						gstKeyboard.u8AdminMenuStep = 3;
					}
					else
					{
						GUI_ShowNotice(NOTICE_PWD_ERR);
						Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 5);
						delay_ms(1234);
						gstKeyboard.u8SecondMenuStep = 0;
						gstKeyboard.u8AdminMenuStep = 0;
						gstKeyboard.bAddInto = 1;
					}
				}
				else if((Key_Star == gstKeyboard.u8KeyValue)||((Key_Jin == gstKeyboard.u8KeyValue)))
				{			
					if(gstKeyboard.u8KeyCount)
					{
						gstKeyboard.u8KeyCount --;
						gstKeyboard.KeyBuf[gstKeyboard.u8KeyCount] = 0;
						GUI_ShowPassword(gstKeyboard.u8KeyCount);
					}
				}
				else if((gstKeyboard.u8KeyValue >= Key_0)&&(gstKeyboard.u8NumValue <= Key_9)&&(gstKeyboard.u8KeyCount < 6))
				{
					gstKeyboard.KeyBuf[gstKeyboard.u8KeyCount] = gstKeyboard.u8NumValue + 0x30;
					gstKeyboard.u8KeyCount ++;
					GUI_ShowPassword(gstKeyboard.u8KeyCount);
				}
				else
				{
					//
				}
			}
			break;

			default:
			{
				gstKeyboard.u8SecondMenuStep = 0;
				gstKeyboard.u8AdminMenuStep = 0;
			}
			break;
		}
	}
}

/***********************************************************************************************
* Function Name  : TopAdmin_SysSetMenu
* Description	 : 高级管理员设置菜单-->系统设置
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : None
***********************************************************************************************/
void TopAdmin_SysSetMenu(void)
{
	if((Key_Cancel == gstKeyboard.u8KeyValue)&&(1 == gstKeyboard.u8ThreeMenuStep))
	{		
		gstKeyboard.u8SecondMenuStep = 0;
		gstKeyboard.u8ThreeMenuStep = 0;
		gstKeyboard.u8FourMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8ThreeMenuStep)
		{
			case 0: //显示选择界面
			{
				gstKeyboard.bAddInto = 0;
				GUI_TopAdmin_SysSetMenu();
				gstKeyboard.u8ThreeMenuStep ++;
			}
			break;
			
			case 1: //选择要设置的参数选项
			{
				switch(gstKeyboard.u8KeyValue)
				{
					case Key_0: //退出
					{
						gstKeyboard.u8SecondMenuStep = 0;
						gstKeyboard.u8ThreeMenuStep = 0;
						gstKeyboard.bAddInto = 1;
					}
					break;
					
					case Key_1: //选择-->密码
					{						
						gstKeyboard.u8ThreeMenuStep = 2;
						gstKeyboard.bAddInto = 1;						
					}
					break;
				
					case Key_2: //选择-->管理卡
					{
						gstKeyboard.u8ThreeMenuStep = 3;
						gstKeyboard.bAddInto = 1;	
					}
					break;
				
					case Key_3: //选择-->模式
					{
						gstKeyboard.u8ThreeMenuStep = 4;
						gstKeyboard.bAddInto = 1;
					}
					break;
					
					case Key_4: //选择-->调试开关
					{
						gstKeyboard.u8ThreeMenuStep = 5;
						gstKeyboard.bAddInto = 1;
					}
					break;
					
					case Key_5: //选择-->箱数
					{
						gstKeyboard.u8ThreeMenuStep = 6;
						gstKeyboard.bAddInto = 1;
					}
					break;
					
					case Key_6: //选择-->开门检测
					{
						gstKeyboard.u8ThreeMenuStep = 7;
						gstKeyboard.bAddInto = 1;
					}
					break;
					
					case Key_7: //选择-->ID卡
					{
						gstKeyboard.u8ThreeMenuStep = 8;
						gstKeyboard.bAddInto = 1;
					}
					break;
					
					default:
					break;
				}
			}
			break;
			
			case 2:  
			{
				TopAdmin_SetPassword();	
			}
			break;

			case 3:  
			{
				Set_AdminCard();
			}
			break;

			case 4: 
			{
				Set_WorkMode();
			}
			break;
			
			case 5:  
			{
				Set_DebugMode();
			}
			break;
			
			case 6:  
			{
				Set_BoxNum();
			}
			break;
			
			case 7:  
			{
				Set_BoxDoorStatusCheck();
			}
			break;

			case 8:
			{
				Set_IDCardSwitch();
			}
			break;
			
			default:
			{
				gstKeyboard.u8ThreeMenuStep = 0;
			}
			break;
		}
	}
}

/***********************************************************************************************
* Function Name  : TopAdmin_LoginMenu
* Description	 : 高级管理员登录操作
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : None
***********************************************************************************************/
void TopAdmin_LoginMenu(void)
{
	if((Key_Cancel == gstKeyboard.u8KeyValue)&&(0 == gstKeyboard.u8ThreeMenuStep))
	{		
		gstKeyboard.u8SecondMenuStep = 0;
		gstKeyboard.u8AdminMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8SecondMenuStep)
		{
			case 0: //显示登录界面
			{
				gstKeyboard.bAddInto = 0;
				GUI_TopAdminMenu();	
				gstKeyboard.u8SecondMenuStep ++;
			}
			break;

			case 1: //选择设置项
			{				
				switch(gstKeyboard.u8KeyValue)
				{
					case Key_0: //退出
					{
						gstKeyboard.u8SecondMenuStep = 0;
						gstKeyboard.u8AdminMenuStep = 0;
						gstKeyboard.bAddInto = 1;
					}
					break;
					
					case Key_1: //系统
					{
						gstKeyboard.u8SecondMenuStep = 2;
						gstKeyboard.u8ThreeMenuStep = 0;
						gstKeyboard.bAddInto = 1;
					}
					break;
				
					default:
					{
						gstKeyboard.u8SecondMenuStep = 0;
					}
					break;
				}
			}
			break;
			
			case 2: 
			{
				TopAdmin_SysSetMenu();	
			}
			break;

			default:
			{
				gstKeyboard.u8SecondMenuStep = 0;
				gstKeyboard.u8AdminMenuStep = 0;
			}
			break;
		}
	}
}

/***********************************************************************************************
* Function Name  : SuperAdmin_SysSetMenu
* Description	 : 超级管理员设置菜单-->系统设置
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : None
***********************************************************************************************/
void SuperAdmin_SysSetMenu(void)
{
	if((Key_Cancel == gstKeyboard.u8KeyValue)&&(1 == gstKeyboard.u8ThreeMenuStep))
	{		
		gstKeyboard.u8SecondMenuStep = 0;
		gstKeyboard.u8ThreeMenuStep = 0;
		gstKeyboard.u8FourMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8ThreeMenuStep)
		{
			case 0: //显示选择界面
			{
				gstKeyboard.bAddInto = 0;
				GUI_SuperAdmin_SysSetMenu();
				gstKeyboard.u8ThreeMenuStep ++;
			}
			break;
			
			case 1: //选择要设置的参数选项
			{
				switch(gstKeyboard.u8KeyValue)
				{
					case Key_0: //退出
					{
						gstKeyboard.u8SecondMenuStep = 0;
						gstKeyboard.u8ThreeMenuStep = 0;
						gstKeyboard.bAddInto = 1;
					}
					break;
					
					case Key_1: //选择-->密码
					{						
						gstKeyboard.u8ThreeMenuStep = 2;
						gstKeyboard.bAddInto = 1;						
					}
					break;
				
					case Key_2: //选择-->期限
					{
						gstKeyboard.u8ThreeMenuStep = 3;
						gstKeyboard.bAddInto = 1;	
					}
					break;
				
					case Key_3: //选择-->键盘
					{
						gstKeyboard.u8ThreeMenuStep = 4;
						gstKeyboard.bAddInto = 1;	
					}
					break;
					
					case Key_4: //选择-->初始化
					{
						gstKeyboard.u8ThreeMenuStep = 5;
						gstKeyboard.bAddInto = 1;	
					}
					break;
					
					case Key_5: //选择-->箱数
					{
						gstKeyboard.u8ThreeMenuStep = 6;
						gstKeyboard.bAddInto = 1;	
					}
					break;
					
					default:
					break;
				}
			}
			break;
			
			case 2:  
			{
				TopAdmin_SetPassword();
			}
			break;

			case 3:  
			{
				Set_LockersValidityDate();
			}
			break;

			case 4:  
			{
			    Set_KeyboardType();
			}
			break;

			case 5:  
			{
				Set_SysConfigInit();
			}
			break;

			case 6:
			{
				Set_BoxNum();
			}
			break;
			
			default:
			{
				gstKeyboard.u8ThreeMenuStep = 0;
			}
			break;
		}
	}
}

/***********************************************************************************************
* Function Name  : SuperAdmin_LoginMenu
* Description	 : 超级管理员登录操作
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : None
***********************************************************************************************/
void SuperAdmin_LoginMenu(void)
{
	if((Key_Cancel == gstKeyboard.u8KeyValue)&&(0 == gstKeyboard.u8ThreeMenuStep))
	{		
		gstKeyboard.u8SecondMenuStep = 0;
		gstKeyboard.u8AdminMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8SecondMenuStep)
		{
			case 0: //显示登录界面
			{
				gstKeyboard.bAddInto = 0;
				GUI_TopAdminMenu();	
				gstKeyboard.u8SecondMenuStep ++;
			}
			break;

			case 1: //选择设置项
			{				
				switch(gstKeyboard.u8KeyValue)
				{
					case Key_0: //退出
					{
						gstKeyboard.u8SecondMenuStep = 0;
						gstKeyboard.u8AdminMenuStep = 0;
						gstKeyboard.bAddInto = 1;
					}
					break;
					
					case Key_1: //系统
					{
						gstKeyboard.u8SecondMenuStep = 2;
						gstKeyboard.u8ThreeMenuStep = 0;
						gstKeyboard.bAddInto = 1;
					}
					break;
				
					default:
					{
						gstKeyboard.u8SecondMenuStep = 0;
					}
					break;
				}
			}
			break;
			
			case 2: //高级管理员设置菜单-->系统设置
			{
				SuperAdmin_SysSetMenu();	
			}
			break;

			default:
			{
				gstKeyboard.u8SecondMenuStep = 0;
				gstKeyboard.u8AdminMenuStep = 0;
			}
			break;
		}
	}
}

/***********************************************************************************************
* Function Name  : KeyboardFunction
* Description	 : 键盘操作程序
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : None
***********************************************************************************************/
void  KeyboardFunction(void)
{	
	gstKeyboard.u8KeyValue = Key_Get_KeyDown();	
	gstKeyboard.u8NumValue = NumKeyToNumValue(gstKeyboard.u8KeyValue);
	
	if((Key_NoDown == gstKeyboard.u8KeyValue)&&(!gstKeyboard.bAddInto))
	{
		if(SYS_STATUS_KEY == gstSysRunInfo.CurrentStatus)
		{
			if((1 == gstKeyboard.u8AdminMenuStep)||(4 == gstKeyboard.u8AdminMenuStep))
			{
				if(gstSysRunInfo.u32TimeCount_ms - gstKeyboard.iLastKeyTime > 2500)
				{
					gstKeyboard.iLastKeyTime = gstSysRunInfo.u32TimeCount_ms;
					gstKeyboard.u8KeyValue = Key_Cancel;
				}
				else
				{
					ArgCardOperation();
					return;
				}
			}
			else 
			{
				if(gstSysRunInfo.u32TimeCount_ms - gstKeyboard.iLastKeyTime > 11000)
				{
					gstKeyboard.iLastKeyTime = gstSysRunInfo.u32TimeCount_ms;
					if(VALIDITY_USING == SysConfig.stHardware.bDebugMode)
					{
						return;
					}
					else
					{
						gstKeyboard.u8KeyValue = Key_Cancel;
					}
				}
				else
				{
					return;
				}
			}
		}
		else
		{
			return;
		}
	}

	if((SYS_STATUS_IDLE != gstSysRunInfo.CurrentStatus)&&(SYS_STATUS_KEY != gstSysRunInfo.CurrentStatus))
	{
		//从别的系统状态中切换过来,先刷下屏
		if(SYS_STATUS_CARD == gstSysRunInfo.CurrentStatus)
		{
			CardOperation(0);
			GUI_Admin();
		}
		else if(SYS_STATUS_Message == gstSysRunInfo.CurrentStatus)
		{
			GUI_Admin();
		}
	}
	gstSysRunInfo.CurrentStatus = SYS_STATUS_KEY;
	gstKeyboard.iLastKeyTime = gstSysRunInfo.u32TimeCount_ms;
	 
	switch(gstKeyboard.u8AdminMenuStep)
	{
		case 0:   //主菜单
		{
			if(Key_Cancel == gstKeyboard.u8KeyValue)									
			{
				GUI_Admin();
				//
				memset((uchar8*)&gstKeyboard, 0x0, sizeof(gstKeyboard));
				gstSysRunInfo.CurrentStatus = SYS_STATUS_IDLE;
			}
			else if(Key_Enter == gstKeyboard.u8KeyValue)	
			{		
				gstKeyboard.u8AdminMenuStep = 1;
				gstKeyboard.u8SecondMenuStep = 0;
			}
			else if(Key_Jin == gstKeyboard.u8KeyValue)	
			{		
				gstKeyboard.u8AdminMenuStep = 4;
				gstKeyboard.u8SecondMenuStep = 0;
			}
			else if(Key_Pwd == gstKeyboard.u8KeyValue)
			{
				gstKeyboard.u8AdminMenuStep = 6;
				gstKeyboard.u8SecondMenuStep = 0;
				gstKeyboard.bAddInto = 1;
			}
			else
			{	
				if(gstKeyboard.bAddInto)
				{
					gstKeyboard.bAddInto = 0;
					GUI_Admin();
				}
				memset((uchar8*)&gstKeyboard, 0x0, sizeof(gstKeyboard));
				gstSysRunInfo.CurrentStatus = SYS_STATUS_IDLE;
			}
		}
		break;

		case 1:  //识别账号登录
		{	
			TopAdmin_Login();
		}
		break;

		case 2: //超级管理员登录操作
		{
			SuperAdmin_LoginMenu();
		}
		break;

		case 3: //高级管理员登录操作
		{
			TopAdmin_LoginMenu();
		}
		break;
		
		case 4: //识别用户管理员帐号登录
		{
			Admin_Login(0);
		}
		break;

		case 5: //用户管理员登录操作
		{
	 		Admin_LoginMenu();
		}
		break;

		case 6: //按"寄存"键进入寄存操作流程
		{
			User_Consign();
		} 
		break;
		
		default:
		{
			gstKeyboard.u8AdminMenuStep = 0;
		}
		break;
	}

	gstKeyboard.u8KeyValue = Key_NoDown;
}

/***********************************************************************************************
* Function Name  : DepositKeyDown
* Description	 : 寄存键按下
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : 系统空闲状态下才扫描[寄存]独立按键
***********************************************************************************************/
void DepositKeyDown(void)
{
	if(SYS_STATUS_IDLE == gstSysRunInfo.CurrentStatus)
	{
		if(0 == GET_MKey_STATE)
		{
			delay_ms(30);
			if(0 == GET_MKey_STATE)
			{
				gstKeyboard.u8AdminMenuStep = 6;
				gstKeyboard.u8SecondMenuStep = 0;
				gstKeyboard.bAddInto = 1;
				gstSysRunInfo.CurrentStatus = SYS_STATUS_KEY;
				gstKeyboard.iLastKeyTime = gstSysRunInfo.u32TimeCount_ms;
			}
		}
	}
}

/*===================================File end===================================================*/

