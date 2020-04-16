#include "include.h"

u32 AS608Addr = 0XFFFFFFFF; //默认
SearchResult match;
unsigned char g_flagloop; 
u16 man_num = 0,user_num = 5;

//初始化PA6为下拉输入		    
//读摸出感应状态(触摸感应时输出高电平信号)
void PS_StaGPIO_Init(void)
{   
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能GPIOA时钟
  //初始化读状态引脚GPIOA
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//输入下拉模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIO	
}

//串口发送一个字节
static void MYUSART_SendData(u8 data)
{
	while((UART5->SR&0X40)==0); 
	UART5->DR = data;
}
//发送包头
static void SendHead(void)
{
	MYUSART_SendData(0xEF);
	MYUSART_SendData(0x01);
}
//发送地址
static void SendAddr(void)
{
	MYUSART_SendData(AS608Addr>>24);
	MYUSART_SendData(AS608Addr>>16);
	MYUSART_SendData(AS608Addr>>8);
	MYUSART_SendData(AS608Addr);
}
//发送包标识,
static void SendFlag(u8 flag)
{
	MYUSART_SendData(flag);
}
//发送包长度
static void SendLength(int length)
{
	MYUSART_SendData(length>>8);
	MYUSART_SendData(length);
}
//发送指令码
static void Sendcmd(u8 cmd)
{
	MYUSART_SendData(cmd);
}
//发送校验和
static void SendCheck(u16 check)
{
	MYUSART_SendData(check>>8);
	MYUSART_SendData(check);
}
//判断中断接收的数组有没有应答包
//waittime为等待中断接收数据的时间（单位1ms）
//返回值：数据包首地址
static u8 *JudgeStr(u16 waittime)
{
	char *data;
	u8 str[8];
	str[0]=0xef;str[1]=0x01;str[2]=AS608Addr>>24;
	str[3]=AS608Addr>>16;str[4]=AS608Addr>>8;
	str[5]=AS608Addr;str[6]=0x07;str[7]='\0';
	UART5_RX_STA=0;
	while(--waittime)
	{
		delay_ms(1);
		if(UART5_RX_STA&0X8000)//接收到一次数据
		{
			UART5_RX_STA=0;
			data=strstr((const char*)UART5_RX_BUF,(const char*)str);
			if(data)
				return (u8*)data;	
		}
	}
	return 0;
}
//录入图像 PS_GetImage
//功能:探测手指，探测到后录入指纹图像存于ImageBuffer。 
//模块返回确认字
u8 PS_GetImage(void)
{
  u16 temp;
  u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x03);
	Sendcmd(0x01);
  temp =  0x01+0x03+0x01;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;
}
//生成特征 PS_GenChar
//功能:将ImageBuffer中的原始图像生成指纹特征文件存于CharBuffer1或CharBuffer2			 
//参数:BufferID --> charBuffer1:0x01	charBuffer1:0x02												
//模块返回确认字
u8 PS_GenChar(u8 BufferID)
{
	u16 temp;
  u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x04);
	Sendcmd(0x02);
	MYUSART_SendData(BufferID);
	temp = 0x01+0x04+0x02+BufferID;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;
}
//精确比对两枚指纹特征 PS_Match
//功能:精确比对CharBuffer1 与CharBuffer2 中的特征文件 
//模块返回确认字
u8 PS_Match(void)
{
	u16 temp;
  u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x03);
	Sendcmd(0x03);
	temp = 0x01+0x03+0x03;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;
}
//搜索指纹 PS_Search
//功能:以CharBuffer1或CharBuffer2中的特征文件搜索整个或部分指纹库.若搜索到，则返回页码。			
//参数:  BufferID @ref CharBuffer1	CharBuffer2
//说明:  模块返回确认字，页码（相配指纹模板）
u8 PS_Search(u8 BufferID,u16 StartPage,u16 PageNum,unsigned short *nPageID, unsigned short *nScore)
{
	u16 temp;
  u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x08);
	Sendcmd(0x04);
	MYUSART_SendData(BufferID);
	MYUSART_SendData(StartPage>>8);
	MYUSART_SendData(StartPage);
	MYUSART_SendData(PageNum>>8);
	MYUSART_SendData(PageNum);
	temp = 0x01+0x08+0x04+BufferID
	+(StartPage>>8)+(u8)StartPage
	+(PageNum>>8)+(u8)PageNum;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
	{
		ensure = data[9];
		*nPageID  =(data[10]<<8)+data[11];
		*nScore=(data[12]<<8)+data[13];	
	}
	else
		ensure = 0xff;
	return ensure;	
}
//合并特征（生成模板）PS_RegModel
//功能:将CharBuffer1与CharBuffer2中的特征文件合并生成 模板,结果存于CharBuffer1与CharBuffer2	
//说明:  模块返回确认字
u8 PS_RegModel(void)
{
	u16 temp;
  u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x03);
	Sendcmd(0x05);
	temp = 0x01+0x03+0x05;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;		
}
//储存模板 PS_StoreChar
//功能:将 CharBuffer1 或 CharBuffer2 中的模板文件存到 PageID 号flash数据库位置。			
//参数:  BufferID @ref charBuffer1:0x01	charBuffer1:0x02
//       PageID（指纹库位置号）
//说明:  模块返回确认字
u8 PS_StoreChar(u8 BufferID,u16 PageID)
{
	u16 temp;
  u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x06);
	Sendcmd(0x06);
	MYUSART_SendData(BufferID);
	MYUSART_SendData(PageID>>8);
	MYUSART_SendData(PageID);
	temp = 0x01+0x06+0x06+BufferID
	+(PageID>>8)+(u8)PageID;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;	
}
//删除模板 PS_DeletChar
//功能:  删除flash数据库中指定ID号开始的N个指纹模板
//参数:  PageID(指纹库模板号)，N删除的模板个数。
//说明:  模块返回确认字
u8 PS_DeletChar(u16 PageID,u16 N)
{
	u16 temp;
  u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x07);
	Sendcmd(0x0C);
	MYUSART_SendData(PageID>>8);
	MYUSART_SendData(PageID);
	MYUSART_SendData(N>>8);
	MYUSART_SendData(N);
	temp = 0x01+0x07+0x0C
	+(PageID>>8)+(u8)PageID
	+(N>>8)+(u8)N;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;
}
//清空指纹库 PS_Empty
//功能:  删除flash数据库中所有指纹模板
//参数:  无
//说明:  模块返回确认字
u8 PS_Empty(void)
{
	u16 temp;
  u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x03);
	Sendcmd(0x0D);
	temp = 0x01+0x03+0x0D;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;
}
//写系统寄存器 PS_WriteReg
//功能:  写模块寄存器
//参数:  寄存器序号RegNum:4\5\6
//说明:  模块返回确认字
u8 PS_WriteReg(u8 RegNum,u8 DATA)
{
	u16 temp;
  u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x05);
	Sendcmd(0x0E);
	MYUSART_SendData(RegNum);
	MYUSART_SendData(DATA);
	temp = RegNum+DATA+0x01+0x05+0x0E;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	if(ensure==0)
		printf("\r\n设置参数成功！");
	return ensure;
}

//设置模块地址 PS_SetAddr
//功能:  设置模块地址
//参数:  PS_addr
//说明:  模块返回确认字
u8 PS_SetAddr(u32 PS_addr)
{
	u16 temp;
  u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x07);
	Sendcmd(0x15);
	MYUSART_SendData(PS_addr>>24);
	MYUSART_SendData(PS_addr>>16);
	MYUSART_SendData(PS_addr>>8);
	MYUSART_SendData(PS_addr);
	temp = 0x01+0x07+0x15
	+(u8)(PS_addr>>24)+(u8)(PS_addr>>16)
	+(u8)(PS_addr>>8) +(u8)PS_addr;				
	SendCheck(temp);
	AS608Addr=PS_addr;//发送完指令，更换地址
  data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;	
		AS608Addr = PS_addr;
	if(ensure==0x00)
		printf("\r\n设置地址成功！");
	return ensure;
}

//与AS608握手 PS_HandShake
//参数: PS_Addr地址指针
//说明: 模块返新地址（正确地址）	
u8 PS_HandShake(u32 *PS_Addr)
{
	SendHead();
	SendAddr();
	MYUSART_SendData(0X01);
	MYUSART_SendData(0X00);
	MYUSART_SendData(0X00);	
	delay_ms(200);
	if(UART5_RX_STA&0X8000)//接收到数据
	{		
		if(//判断是不是模块返回的应答包				
				UART5_RX_BUF[0]==0XEF
				&&UART5_RX_BUF[1]==0X01
				&&UART5_RX_BUF[6]==0X07
			)
			{
				*PS_Addr=(UART5_RX_BUF[2]<<24) + (UART5_RX_BUF[3]<<16)
								+(UART5_RX_BUF[4]<<8) + (UART5_RX_BUF[5]);
				UART5_RX_STA=0;
				return 0;
			}
		UART5_RX_STA=0;					
	}
	return 1;		
}


int AutoEnrollPro(unsigned char intype)
{
	unsigned short tempcnt = 0,Key_Value,i;
	unsigned char circnt = 0;//循环次数
	unsigned short uMatchIndex, uMatchScore;    // 搜索的索引值，分数
	unsigned char tempdelaycnt = 20; 
	
	int iRet;
	u8 test_cir = 0;
	if(intype == 1)
	{
		if(Fin.Users_Num >= (MODULE_DBSize - 5))
		{
			//用户指纹满
			return ERR_OUTOFINDEX;
		}
		
		if(Fin.UserFullFlag == 1)
		{
				for(i=5;i<MODULE_DBSize;i++)
				{
					if(Fin.Value[i]==0)
					{
						Fin.Users_Save_Num=i;
						break;
					}
				}
		}
	}
	else
	{
		if(Fin.Man_Num >= 5 || man_num >= 5)   
		{  
			//管理员已满
			delay_ms(1000);
			return ERR_ADMINOVER;
		}	
		if(Fin.ManFullFlag == 1)
		{
			for(i=0;i<5;i++)
			{
				if(Fin.Value[i]==0)
				{
					Fin.Man_Save_Num=i;
					break;
				}
			}
		}	
		
	}
	g_flagloop = true;
	//需要2次注册
	for(circnt = 1;circnt <= 2;circnt++)
	{	
		while(g_flagloop)	
		{
			//获取注册图像
			if((iRet = PS_GetImage()) == ERR_OK)	
			{   //  如不支持可换成cmd_GetImage
					break;
			}
		
			//注册超时
			if((TimingDelay-menu_now) > TOUCH_Timeout/2)	
			{
					g_flagloop = false; 			  
					return ERR_OVERTIME;
			}
			Key_Value = KEY_Scan3x4();  			
			if(Key_Value=='*')
			{
				return 0xFE;
			}
		}
			
		//生产特征文件
		test_cir = (u8)circnt;
		iRet = PS_GenChar(test_cir);
		if(iRet != ERR_OK)
		{
			//添加失败
			return iRet;
		}
		
		//搜索指纹库中是否有现有指纹
		iRet = PS_Search(circnt, 0, MODULE_DBSize,&uMatchIndex, &uMatchScore);
		if(iRet == ERR_OK	)//搜索到了指纹
		{
			for(tempcnt = 0;tempcnt < 5;tempcnt++)
			{
				//和管理员指纹重复
				if(Fin.Value[tempcnt] == (uMatchIndex))
				{
					g_flagloop = false;										
					JQ8400_SendCmd(2,4,1,1);
					delay_ms(1500);
					return ERR_TEMPLATEEXIST;
				}
			}
			if(intype == 0)//如果添加的时管理员密码 并且此指纹已经录入了 
			{
					g_flagloop = false;
					//指纹已存在
					return ERR_ISPUBLIC;
			}		
		}
		else if( (iRet != ERR_SEARCHCHAR))//搜索错误ERR_SEARCHCHAR没有搜索到
		{
			//添加失败
			return iRet;
		}	
			
		//判断手指是否离开
		if(circnt != 2)
		{
			menu_now = TimingDelay;//清除计数值
			if(circnt!=2)
			{
				JQ8400_SendCmd(1,8,1,1);
			}
			 g_flagloop = true;
			 menu_now = TimingDelay;
			 while(g_flagloop)	
			 {
				if((iRet = PS_GetImage()) == ERR_NOFINGER)	
				{
						tempdelaycnt = 20;
						
						break;
				}
				else 
				{
					JQ8400_SendCmd(1,2,1,1);
					delay_ms(1000);
				}
				if((TimingDelay - menu_now) > TOUCH_Timeout)	
				{
					return ERR_OVERTIME;
				}
				tempdelaycnt++;
				if(tempdelaycnt >= 15)//3s
				{
					tempdelaycnt = 0;
				}
				Key_Value = KEY_Scan3x4(); 
				if(Key_Value=='*')
				{
					return 0xFE;
				}
			 }
		}
	}	
	iRet = PS_RegModel();
	if(iRet != ERR_OK)
	{
		//添加失败
		return iRet;
	}
	//存储模版
	if(intype == 1)
	{
		if(user_num >= 99)
		{
			user_num = 0;
		}
		iRet = PS_StoreChar(1,user_num);
	}
	else
	{
		if(man_num >= 5)
		{
			man_num = 0;
		}
		iRet = PS_StoreChar(1,man_num);
	}
	if(iRet != ERR_OK)
	{
		//添加失败
		return iRet;
	}
	if(intype == 0)
	{
		Fin.Man_Num++;
		if(Fin.ManFullFlag == 0)//还没有满
		{
			SumGen.Save_Num = Fin.Man_Save_Num;
			Fin.Value[Fin.Man_Save_Num] = (man_num);
			save_flash(FINPT,Fin.Man_Save_Num);
			if(Fin.Man_Save_Num == 4)//如果已经到达了最高数字
			{
				Fin.ManFullFlag = 1;//已经存储满了
				flash_save(44,Fin.ManFullFlag);
				for(i=0;i<5;i++)
				{
					if(Fin.Value[i]==0)
					{
						Fin.Man_Save_Num=i;
						break;
					}
				}
			}
			else
			{
				Fin.Man_Save_Num++;
				man_num++;
			}			
		}
		else  //已经存储满过
		{
			for(i=0;i<5;i++)
			{
				if(Fin.Value[i]==0)
				{
					Fin.Man_Save_Num=i;
					break;
				}
			}
			SumGen.Save_Num  = Fin.Man_Save_Num;
			Fin.Value[Fin.Man_Save_Num] = (man_num);
			save_flash(FINPT,Fin.Man_Save_Num);	
		}
	}
	else//普通用户
	{
		Fin.Users_Num = user_num;			
		if(Fin.UserFullFlag == 0)//还没有满
		{
			SumGen.Save_Num  = Fin.Users_Save_Num;
			Fin.Value[Fin.Users_Save_Num] = (user_num);
			save_flash(FINPT,Fin.Users_Save_Num);
			if(Fin.Users_Save_Num == (MODULE_DBSize-1))//如果已经到达了最高数字
			{
				Fin.UserFullFlag = 1;//已经存储满了
				flash_save(45,Fin.UserFullFlag);
				for(i=5;i<MODULE_DBSize;i++)
				{
					if(Fin.Value[i]==0)
					{
						Fin.Users_Save_Num=i;
						break;
					}
				}
			}
			else
			{
				Fin.Users_Save_Num++;
				user_num++;
			}
		}
		else //已经存储满过
		{
			for(i=5;i<MODULE_DBSize;i++)
			{
				if(Fin.Value[i]==0)
				{
					Fin.Users_Save_Num=i;
					break;
				}
			}
			SumGen.Save_Num  = Fin.Users_Save_Num;
			Fin.Value[Fin.Users_Save_Num] = (user_num);
			save_flash(FINPT,Fin.Users_Save_Num);
		}
	}
	return 0;
}

//=================================================================================
// 实现指纹识别功能, 区分主动识别和被动识别 
// 参数:
//  identype 识别类型 0 被动识别  1 主动识别(带有用户已存在语音) 2主动识别 不带用户已存在语音

//figidadd 识别到指纹的地址
// 返回值:
//     0xFF : 不进行识别
//     0xFE : 失败多次
//     0  : 成功
//     正数: 失败
//=================================================================================
unsigned char startIdentifyflag = 0;//开始进行指纹识别标记
int AutoIdentifyPro(unsigned char identype)
{
	int iRet = 0;
	unsigned tempcnt = 0;
	unsigned short uMatchIndex, uMatchScore;    // 搜索的索引值，分数
	if(identype > 0)
	{
		startIdentifyflag = 1;
	}
	else if(identype == 0)
	{
		if(startIdentifyflag == 0)
		{
			if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4))//如果有手指在
			{
					startIdentifyflag = 1;
			}
		}
	}
	if(startIdentifyflag == 1)//识别开始
	{
		FIN_FLAG=3;
		g_flagloop = true;
//		TimingDelay_flag = 0;//开定时器
		do
		{
			menu_now = TimingDelay;
			while(g_flagloop)
			{
				 if((iRet = PS_GetImage()) == ERR_OK)
				 {
						break;
				 }
				 if((TimingDelay-menu_now) > TOUCH_Timeout/2)	
				 {
						if(identype == 1)
						{
								//操作失败 请重试
						}
						iRet = ERR_OVERTIME;
						g_flagloop = false;
						startIdentifyflag = 0;
						return  iRet;
				 }
				 delay_ms(100);
			}
			iRet =  PS_GenChar(CharBuffer1);
			if(iRet != ERR_OK)
			{
				if(identype != 0)
				{
					//操作失败 请重试
				}
				startIdentifyflag = 0;
				return iRet;
			}
			
			iRet = PS_Search(CharBuffer1, 0, MODULE_DBSize,&uMatchIndex, &uMatchScore);
			if(iRet == ERR_OK)
			{
				if(identype == 1)
				{
					//用户已存在
				}
				for(tempcnt =0;tempcnt <sizeof(Fin.Value);tempcnt++)
				{
					if(Fin.Value[tempcnt] == (uMatchIndex))
					{
						Fin.Read_Num = uMatchIndex;
						if(uMatchIndex<5)
							FIN_FLAG=0; //管理员
						else 
							FIN_FLAG=1; //普通用户
						break;							
					}
				}
				startIdentifyflag = 0;
				return ERR_OK;
			}
			else if(iRet == ERR_SEARCHCHAR)
			{
				if(identype == 0)
				{
					//用户bu存在
				}
				startIdentifyflag = 0;
				return ERR_SEARCHCHAR;
			}
			else
			{
				if(identype != 0)
				{
					//操作失败,请重试
				}
				startIdentifyflag = 0;
				
				return iRet;
			}
		}while(0);	
	}
	return NoIdFIG;
}

int DeelOnFigAndUSERPro(unsigned char usernum)
{
	int iRet;
	do{
		iRet = PS_DeletChar(Fin.Read_Num,1);
	  if(iRet != 0)break;
	}while(0);
	
	if(iRet == 0)
	{
		if(Fin.Read_Num < 5)
		{
			Fin.Man_Num--;
		}
		else 
		{
			Fin.Users_Num--;
		}
	  
		Fin.Value[usernum] = 0x00;
	}
	else
	{
		//删除失败
	  delay_ms(2000);
	}
	return iRet;
}

//=================================================================================
// 清空用户指纹
// 参数:
// 
// 返回值:
//     
//     0  : 成功
//     正数: 失败
//=================================================================================
int EmptyUserFig(void)
{
	int iRet;
	unsigned char tempcnt = 0;
	do{
		for(tempcnt = 5;tempcnt < MODULE_DBSize;tempcnt++)	
		{
			if(Fin.Value[tempcnt] != 0)//地址上有指纹存在
			{
				iRet = PS_DeletChar(Fin.Value[tempcnt]-1,1);//对地址进行删除
				if(iRet != 0)   break;
			}
		}
	} while(0);
    
	if(iRet == 0)
	{	
		delay_ms(200);
	}
	else
	{
		//操作失败
		delay_ms(200);
	}
	
	return iRet;
}





























