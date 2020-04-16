#include "include.h"
u8 Hold_Open_Door = 1;
unsigned char TempPWD[12]; 
u8 Status_Lock = 0;
u8 Double_Verify = 0;
u8 Open_Way_flag = 0;
unsigned int pwd_timeout = TOUCH_Timeout;

void Reset_Error_Flag(u8 mode)
{
		if(mode == User||mode == ALL)
		{
			Fin.Error = 0;
			Pwd.Error = 0;
			IC.Error = 0;
			Remote_Error = 0;
		}
		if(Management||mode == ALL)
		{
			Pwd.Man_Error = 0;
			Fin.Man_Error = 0;
			Page0 = '1';
			Page1 = 0;
			Page2 = 0;
			Page3 = 0;
			Page0_0 = 0;
			Page1_1 = 0;
			Page2_2 = 0;
			Page3_3 = 0;
			show_flag = 0;
		}
	 time_sec = 0;
	 time_min = 0;
	 timer3_open = 0;
}


//返回值：0：成功 1：失败 2：长度错误 3:进入菜单 4：双验证里没有密码 5:退出
u8 Enter_pwd(u8 mode)
{
	u8 return_num = 0xff;
	u16 iret;
	unsigned char Key_Value;
	Key_Value = KEY_Scan3x4();
	if((Key_Value>='0'&&Key_Value<='9')||Key_Value == '#'||Key_Value == '*')
	{
		menu_now = TimingDelay;
		lowerpower_now=TimingDelay;
		JQ8400_SendCmd(2,9,1,1);
		delay_ms(300);
		
		if(Key_Value == '*')//按 *键返回
		{ 
			Mode = 3;
			Touch_flag=1;
			Low_Timeout=100; 
			Key_Value = 0;
			show_flag = 0;
			lowerpower_now=TimingDelay;   
			delay_ms(200);			
			return_num = 5;
		}	
		Temp_Pwd[value_num] = Key_Value;	 
		if(Key_Value == '#')
		{
			if(value_num >= 2)
					Pwd.Flag = 0;
			if(value_num == 2 && mode == User)
			{   
				if(Temp_Pwd[0] == '0' && Temp_Pwd[1] == '0')
				{
					return_num = 3;
				}
				else  // 不是00#都操作失败
				{
					return_num = 2;
				}
			}
			else if(value_num < 6)
			{
				return_num = 2;
			}
			else if(Error_Flag == 1)
			{
				JQ8400_SendCmd(1,5,1,1);//系统锁定
				delay_ms(1000);
				Mode = 3;
				show_flag = 0;
			}
			else  if(Pwd.Flag == 0 && value_num <= 19)
			{
				iret = pwd_verify(0, Temp_Pwd);//密码识别
				if(mode == User)
				{
					if(iret != 0 && iret != 3)
						iret = pwd_verify(1, Temp_Pwd);
				}
				if(iret == 0 || iret == 3)
				{
						return_num = 0;
				}
				else 
				{						
					return_num = 1;
				}
			}
			else
			{
				return_num = 2;
			}
		}
		else if(Key_Value == '*')
		{
			return_num = 5;
		}
		value_num++;
		if((value_num > 20) && (Key_Value != '#'))
		{
			Pwd.Error++;
			return_num = 2;
		}
		else if(value_num > 2 && Pwd.Flag == 1 && mode == User)
		{
			return_num = 4;
		}
	}
	if(return_num == 0)//0：成功 
	{				
		if(mode == Management)
		{
			Mode = 1;
			Reset_Error_Flag(ALL);
			voice_flag = 0;
		}
		else if(mode == User)
		{
			Pwd.Status = 1;
			DistinguishSum(2);//密码
		}
	}	
	else if(return_num ==1)//1：失败 
	{
		//密码错误
		JQ8400_SendCmd(1,4,1,1);
		delay_ms(1000);
		if(mode == Management)
			Pwd.Man_Error++;
		else if(mode == User)
			Pwd.Error++;
	}
	else if(return_num == 2)
	{
		//2：长度错误 
		JQ8400_SendCmd(2,0,1,1);
		delay_ms(1000);
		if(mode == Management)
			Pwd.Man_Error++;
		else if(mode == User)
			Pwd.Error++;
	}
	else if(return_num ==3)//3:进入菜单 
	{
		Mode = 4;	
    show_flag = 0;						
	}
	else if(return_num ==4)//4：双验证里没有密码
	{
		JQ8400_SendCmd(3,4,1,1);
		delay_ms(1000);
		Mode = 3;
		Low_Timeout=100;							 
	}
	else if(return_num ==5)//返回
	{
		Mode = 3;
		show_flag = 0;
	}
	if(return_num<6)
	{
			memset(Temp_Pwd, 0, sizeof(Temp_Pwd));
			return_num = 0xff;
			Key_Value = 0;
			value_num = 0;
	}	
	return 0xff;
}
/*
函数：添加密码
参数：mode：0:管理员 1;普通用户 2 清空标志位和密码数据 Key_Value：键入值
返回值：
			 0:成功
			 1:失败
			 2:超时
			 3:长度错误
			 4:退出
			 5:密码重复
*/

u8 pwd_add(u8 mode,u8 Key_Value)
{
	u8 buf_num,tempi,iret=0xff,return_num = 0xff;
	static u8 PWD_Num = 0, PWD_Time = 0;
	u16 i;
	int now;
	if(mode == Management)
	{
		buf_num = Pwd.Man_Save_Num;
	}
	else if(mode == User)
	{
		buf_num = Pwd.Users_Save_Num;
	}
	else if(mode == NONE)
	{
		PWD_Time = 0;
		PWD_Num = 0;
		memset(Pwd.Value[buf_num],0,sizeof(*Pwd.Value));
		memset(TempPWD,0,sizeof(TempPWD));
		return 0xff;
	}
	if(Key_Value >= '0' && Key_Value <= '9')
	{
		if(PWD_Num > 10)
		{
			return_num = 1;
		}
		menu_now = TimingDelay;
		TempPWD[PWD_Num]=Key_Value;
		JQ8400_SendCmd(2,9,1,1);
		delay_ms(300);
		PWD_Num++;	
		now=TimingDelay;
	}
	else if(Key_Value == '#')
	{
		menu_now = TimingDelay;
		TempPWD[PWD_Num]=Key_Value;
		JQ8400_SendCmd(2,9,1,1);
		delay_ms(300);
		if(PWD_Num < 6)
		{
			return_num = 1;					
		}
		else if (PWD_Time == 0)
		{
			iret=pwd_verify(Management,TempPWD);
			if(iret==0)
			{
				return_num = 5;
			}
			iret=pwd_verify(User,TempPWD);
			if(iret==0)
			{
				return_num = 5;
			}
			PWD_Time++;
			for(i=0;i<11;i++)
			{
				Pwd.Value[buf_num][i]=TempPWD[i];
				if(TempPWD[i]=='#')
					break;
			}
			PWD_Num=0;
			JQ8400_SendCmd(1,8,1,1);
			memset(TempPWD,0,sizeof(TempPWD));
		}		
		else if (PWD_Time == 1)
		{
			for(i=0;i<11;i++)
			{
				if(Pwd.Value[buf_num][i]!=TempPWD[i])
				{
					return_num = 1;
					break;
				}
				if(Pwd.Value[buf_num][i]=='#'&&TempPWD[i]=='#')
				{
					save_flash(PASSWORD,buf_num);
					SumGen.Save_Num = buf_num;
					if(buf_num<2)
					{
						Pwd.Man_Num++;
						if(Pwd.ManFullFlag==0)
						{
							Pwd.Man_Save_Num++;
							if(Pwd.Man_Save_Num>1)
							{
								Pwd.ManFullFlag=1;
								for(tempi=0;tempi<2;tempi++)
								{
									if(Pwd.Value[tempi][0]==0)
									{
										Pwd.Man_Save_Num=tempi;
										break;
									}
								}  
								flash_save(42,Pwd.ManFullFlag);			
							}
						}				
						flash_save(32,Pwd.Man_Num);
						flash_save(40, Pwd.Man_Save_Num);
					}
					else 
					{
						Pwd.Users_Num++;	
						if(Pwd.UserFullFlag==0)	
						{
							Pwd.Users_Save_Num++;
							if(Pwd.Users_Save_Num>29)
							{
								Pwd.UserFullFlag=1;
								for(tempi=2;tempi<100;tempi++)
								{
									if(Pwd.Value[tempi][0]==0)
									{  
										Pwd.Users_Save_Num=tempi;
										break;
									}
								}      
								flash_save(43,Pwd.UserFullFlag);
							}
						}
						flash_save(34,Pwd.Users_Num);
						flash_save(38,Pwd.Users_Save_Num);
					}
					return_num = 0;
				}
			}		
		}	
	}
	else if(Key_Value == '*')
	{
		JQ8400_SendCmd(2,9,1,1);
		delay_ms(300);	
		return_num =  4;
	}
	if(Mode==11)
		return_num = 4;
	if(return_num!=0xff)
	{
		if(return_num == 0)
		{
			
		}
		else 	if(return_num == 1)//添加失败
		{
			JQ8400_SendCmd(2,0,1,1);
		  delay_ms(800);
		}
		else 	if(return_num == 3)//长度错误
		{
			JQ8400_SendCmd(2,0,1,1);
		  delay_ms(800);
		}
		else 	if(return_num == 5)//已存在
		{
			JQ8400_SendCmd(2,4,1,1);
		  delay_ms(1000);
		}
		PWD_Num = 0;
		PWD_Time = 0;
		memset(Pwd.Value[buf_num],0,sizeof(*Pwd.Value));
		memset(TempPWD,0,sizeof(TempPWD));
	}
	return return_num;	
}


/*
识别密码
参数：mode:模式(0:管理模式1：普通用户模式)
			buf：需要识别的密码
返回值：0：成功
				1：失败
				2：密码太短或太长
				3: 以虚拟密码形式成功
				4:	还没有检测到密码或#
*/

u8 len_test,len_testi,len_testj,test_nums=1;
u8	pwd_verify(u8 mode,unsigned char *buf)
{
	u8 i,j=0,k=0,num=0,len,page,buf_num,verify_hash=0,verify;
	u8 len_;
	buf_num = buf_num;
	page = page;
	num = num;
	
	len=strlen((const char*)buf);
	
	if( mode == Management)
		buf_num=2;
	else 
		buf_num=15;
	if(len < 7 || len > 20)
	{
			return 2;
	}
	
	for(i=0;i<(len-6);i++)
	{
		if(*(buf+6)=='#')
		{
			verify_hash=1;
			read_flash(PASSWORD);
			break;
		}
		else
		{
			buf++;
		}
	}
	
	if(verify_hash==1)
	{
		buf -= i;
		
		for(i=0;i<100;i++)
		{
			if(Pwd.Value[i][0]==0)
			{
				continue;
			}
			for(k=0;*(buf+5)!='#';k++)
			{
				verify=0;
				for(j=0;Pwd.Value[i][j]!='#';j++)
				{
					if(Pwd.Value[i][j]!=*buf)
					{
						verify=1;
						buf=buf-j;
						break;
					}
					buf++;
					if(j>21)
						break;
				}
				if(verify==0)
				{
					Pwd.Read_Num=i;
					if((mode==0&&Pwd.Read_Num<2)||(mode==1&&Pwd.Read_Num>=2))
					{
						len_= strlen((const char*)Pwd.Value[i]);
						if(k==0&&(len==len_))
							return 0;
						else 
							return 3;    
					}
				}
				buf++;
			}
			buf-=k;
		}
		verify_hash=0;
	}
	else 
	{
		return 4;
	}
	return 1;
}

void Register_Admins(void)
{
	static u8  Fin_time_flag = 0;
	u8 iret = 0xff;
	unsigned char Key_Value = 0;
	Key_Value = KEY_Scan3x4();
	if(re_flag == 1)
	{
		enterpwdpage(1);
		OLED_Refresh_Gram();
		
		iret = pwd_add(Management,Key_Value);	
		if(iret == 0)
		{
			re_flag = 2;
			flash_save(30, 1);//存储已经注册过管理员的标识位
			AddSum(2,0);//指纹1 密码2 IC 3   管理员0 普通用户1
			JQ8400_SendCmd(1,9,1,1);
			delay_ms(1100);
			flash_save(32, Pwd.Man_Num);
			flash_save(40, Pwd.Man_Save_Num); 
			menu_now = TimingDelay;
		}
//		else if(TimingDelay - menu_now > TOUCH_Timeout || iret == 4)
//		{
//			Mode = 3;	
//			delay_ms(300);
//			Touch_flag=1;
//		}
		iret = 0xff;
	}
	else if(re_flag == 2)
	{
		JQ8400_SendCmd(3,8,1,1);  //请输入指纹
		if(show_flag == 0)
		{
			OLED_Clear();
			enterpwdpage(0);
			OLED_Refresh_Gram();
			show_flag = 1;
		}
		delay_ms(1200);
		
		iret=AutoEnrollPro(Management);
		menu_now = TimingDelay;
		if(iret==ERR_OVERTIME)
		{						
			re_flag = 3;
			Mode = 3;		 
			OLED_Clear();
			OLED_Refresh_Gram();
		}
		else if(Key_Value == '*'||iret == 0xFE)
		{
			re_flag = 3;
			Mode = 1;	
			Reset_Error_Flag(ALL);
		}
		else if(iret == ERR_OK)
		{					 
			flash_save(33, Fin.Users_Num);
			flash_save(37, Fin.Users_Save_Num);
			flash_save(44,Fin.ManFullFlag);
			AddSum(1,0);//指纹1 密码2 IC 3   管理员0 普通用户1
			
			JQ8400_SendCmd(1,9,1,1);
			delay_ms(1100);
			re_flag = 3;
			Mode = 1;
			Reset_Error_Flag(ALL);
		}
		else 
		{
			Fin_time_flag++;
			if(Fin_time_flag>3)
			{
				Fin_time_flag=0;
				re_flag = 3;
				Mode = 1;
				Reset_Error_Flag(ALL);
			}
		}
	}
}

//添加一个密码或者指纹或者IC卡	
//输入
//idtype ：卡的类型 1指纹用户 2 密码用户 3 IC用户 4遥控器
	//usertype : 用户类型 0 管理员 1 普通用户
void AddSum(unsigned char idtype,unsigned char usertype)
{
	unsigned short tempcnt = 0;
	unsigned short temomid = 0;//中间变量
	if(usertype == Management)//管理员
	{
		SumGen.Man_Num++;
		if(SumGen.ManFullFlag == 1)
		{
			for(tempcnt=1;tempcnt<10;tempcnt++)
			{
				if(SumGen.Value[tempcnt] == 0)
				{
					SumGen.Man_Save_Num = tempcnt;
					break;
				}
			}
		}
		if(idtype == FINPT)//指纹用户
		{
			temomid = 0x0100;
			temomid += ((unsigned short)(SumGen.Save_Num)); 
			SumGen.Value[SumGen.Man_Save_Num] = temomid;
		}
		else if(idtype == PASSWORD)//密码用户
		{
			temomid = 0x0200;
			temomid += ((unsigned short)(SumGen.Save_Num)); 
			SumGen.Value[SumGen.Man_Save_Num] = temomid;
		}
		SumStr_Save(SumGen.Man_Save_Num);
		SumGen.DisplayMan_Save_Num = SumGen.Man_Save_Num;
		//判断存储位置是否溢出
		if(SumGen.ManFullFlag == 0)
		{
			SumGen.Man_Save_Num++;
			if(SumGen.Man_Save_Num > 9)//最大299
			{
				SumGen.ManFullFlag = 1;
			}
		}
		if(SumGen.ManFullFlag == 1)
		{
			for(tempcnt = 1;tempcnt < 10;tempcnt++)
			{
				if(SumGen.Value[tempcnt] == 0)
				{
					SumGen.Man_Save_Num = tempcnt;
					break;
				}
			}
		}
		flash_save(49, SumGen.Man_Num);
		flash_save(51, SumGen.Man_Save_Num);
		flash_save(53, SumGen.ManFullFlag);
	}
	else if(usertype == User)
	{
		SumGen.Users_Num++;
		if(SumGen.UserFullFlag == 1)
		{
			for(tempcnt = 10;tempcnt < 320;tempcnt++)
			{
				if(SumGen.Value[tempcnt] == 0)
				{
					SumGen.Users_Save_Num = tempcnt;
					break;
				}
			}
		}
		if(idtype == 1)//指纹用户
		{
			temomid = 0x0100;
			temomid += ((unsigned short)(SumGen.Save_Num)); 
			SumGen.Value[SumGen.Users_Save_Num] = temomid;
		}
		else if(idtype == 2)//密码用户
		{
			temomid = 0x0200;
			temomid += ((unsigned short)(SumGen.Save_Num)); 
			SumGen.Value[SumGen.Users_Save_Num] = temomid;
		}
		else if(idtype == 3)//IC用户
		{
			temomid = 0x0300;
			temomid += ((unsigned short)(SumGen.Save_Num)); 
			SumGen.Value[SumGen.Users_Save_Num] = temomid;
		}
		else if(idtype == 4)//遥控器
		{
			temomid = 0x0400;
			temomid += ((unsigned short)(SumGen.Save_Num)); 
			SumGen.Value[SumGen.Users_Save_Num] = temomid;
		}
		SumStr_Save(SumGen.Users_Save_Num);	
		SumGen.DisplayUsers_Save_Num  = SumGen.Users_Save_Num;
		//判断存储位置是否溢出
		if(SumGen.UserFullFlag == 0)
		{
			SumGen.Users_Save_Num++;
			if(SumGen.Users_Save_Num >= 320)//最大319
			{
				SumGen.UserFullFlag = 1;
			}
		}
		if(SumGen.UserFullFlag == 1)
		{
			for(tempcnt = 10;tempcnt < 320;tempcnt++)
			{
				if(SumGen.Value[tempcnt] == 0)
				{
					SumGen.Users_Save_Num = tempcnt;
					break;
				}
			}
		}
		flash_save(50, SumGen.Users_Num);
		flash_save(52, SumGen.Users_Save_Num);
		flash_save(54, SumGen.UserFullFlag);
	}
}


//识别信息  识别到的号码放在SumGen.Read_Num 中
//输入
//idtype ：卡的类型 1指纹用户 2 密码用户 3 IC用户
	//usertype : 用户类型 0 管理员 1 普通用户
//0 成功 将要删除的实际数据放到Read_Num中去
//1 失败 --不存在或者无效
unsigned char  DistinguishSum(unsigned char idtype)
{
	unsigned short tempcnt = 0;
	if(idtype == 1)//指纹用户
	{
			for(tempcnt = 0;tempcnt < SumMax;tempcnt++)
			{
				if((SumGen.Value[tempcnt] & 0xff00) == 0x0100)//是这个类型的用户
				{
					if((unsigned char)(SumGen.Value[tempcnt]) == Fin.Read_Num)
					{
						SumGen.Read_Num = tempcnt;
						return 0;
					}
				}
			}
	}
	else if(idtype == 2)//密码用户
	{
			for(tempcnt = 0;tempcnt < SumMax;tempcnt++)
			{
				if((SumGen.Value[tempcnt] & 0xff00) == 0x0200)//是这个类型的用户
				{
					if((unsigned char)(SumGen.Value[tempcnt]) == Pwd.Read_Num)
					{
						SumGen.Read_Num = tempcnt;
						return 0;
					}
				}
			}
	}
	else if(idtype == 3)//IC用户
	{
		for(tempcnt = 0;tempcnt < SumMax;tempcnt++)
			{
				if((SumGen.Value[tempcnt] & 0xff00) == 0x0300)//是这个类型的用户
				{
					if((unsigned char)(SumGen.Value[tempcnt]) == IC.Read_Num)
					{
						SumGen.Read_Num = tempcnt;
						return 0;
					}
				}
			}
	}
//	else if(idtype == 4)//遥控器用户
//	{
//		for(tempcnt = 0;tempcnt < SumMax;tempcnt++)
//			{
//				if((SumGen.Value[tempcnt] & 0xff00) == 0x0400)//是这个类型的用户
//				{
//					if((unsigned char)(SumGen.Value[tempcnt]) == Telecontrol.Read_Num)
//					{
//						SumGen.Read_Num = tempcnt;
//						return 0;
//					}
//				}
//			}
//	}
	return 1;
}

//删除一个密码或者指纹或者IC卡	按识别到的标识删除  此时已经识别到Read_Num了
//输入
//idtype ：卡的类型 1指纹用户 2 密码用户 3 IC用户
	//usertype : 用户类型 0 管理员 1 普通用户
//0 成功 将要删除的实际数据放到Read_Num中去
//1 失败 --不存在或者无效
unsigned char DeleteDisSum(unsigned char idtype,unsigned char usertype)
{
  unsigned short tempcnt = 0;

	if(usertype == 0)//管理员
	{
		if(idtype == 1)//指纹用户
		{
			for(tempcnt = 0;tempcnt < 10;tempcnt++)
			{
				if((SumGen.Value[tempcnt] & 0xff00) == 0x0100)//是这个类型的用户
				{
					if((unsigned char)(SumGen.Value[tempcnt]) == Fin.Read_Num)
					{
						SumGen.Value[tempcnt] = 0;
						SumGen.Man_Num--;//数量减少
						SumGen.Read_Num = tempcnt;
						SumGen.DisplayMan_Save_Num = SumGen.Read_Num ;
						SumStr_Save(tempcnt);
						flash_save(49, SumGen.Man_Num);
						return 0;
					}
				}
			}
		}
		else if(idtype == 2)//密码用户
		{
			for(tempcnt = 0;tempcnt < 10;tempcnt++)
			{
				if((SumGen.Value[tempcnt] & 0xff00) == 0x0200)//是这个类型的用户
				{
					if((unsigned char)(SumGen.Value[tempcnt]) == Pwd.Read_Num)
					{
						SumGen.Value[tempcnt] = 0;
						SumGen.Man_Num--;//数量减少
						SumGen.Read_Num = tempcnt;
						SumGen.DisplayMan_Save_Num = SumGen.Read_Num ;
						SumStr_Save(tempcnt);
						flash_save(49, SumGen.Man_Num);
						return 0;
					}
				}
			}
		}	
	}	
	else if(usertype == 1)//普通用户
	{
		if(idtype == 1)//指纹用户
		{
			for(tempcnt = 10;tempcnt < SumMax;tempcnt++)
			{
				if((SumGen.Value[tempcnt] & 0xff00) == 0x0100)//是这个类型的用户
				{
					if((unsigned char)(SumGen.Value[tempcnt]) == Fin.Read_Num)
					{
						SumGen.Value[tempcnt] = 0;
						SumGen.Users_Num--;//数量减少
						SumGen.Read_Num = tempcnt;
						SumGen.DisplayUsers_Save_Num = SumGen.Read_Num ;
						SumStr_Save(tempcnt);
					flash_save(50, SumGen.Users_Num);
						return 0;
					}
				}
			}
		}
		else if(idtype == 2)//密码用户
		{
			for(tempcnt = 10;tempcnt < SumMax;tempcnt++)
			{
				if((SumGen.Value[tempcnt] & 0xff00) == 0x0200)//是这个类型的用户
				{
					if((unsigned char)(SumGen.Value[tempcnt]) == Pwd.Read_Num)
					{
						SumGen.Value[tempcnt] = 0;
						SumGen.Users_Num--;//数量减少
						SumGen.Read_Num = tempcnt;
						SumGen.DisplayUsers_Save_Num = SumGen.Read_Num ;
						SumStr_Save(tempcnt);
					flash_save(50, SumGen.Users_Num);
						return 0;
					}
				}
			}
		}
		else if(idtype == 3)//IC用户
		{
			for(tempcnt = 10;tempcnt < SumMax;tempcnt++)
			{
				if((SumGen.Value[tempcnt] & 0xff00) == 0x0300)//是这个类型的用户
				{
					if((unsigned char)(SumGen.Value[tempcnt]) == IC.Read_Num)
					{
						SumGen.Value[tempcnt] = 0;
						SumGen.Users_Num--;//数量减少
						SumGen.Read_Num = tempcnt;
						SumGen.DisplayUsers_Save_Num = SumGen.Read_Num ;
						SumStr_Save(tempcnt);
						flash_save(50, SumGen.Users_Num);
						return 0;
					}
				}
			}
		}
	}
	return 1;
}


//删除一个密码或者指纹或者IC卡	按输入的数字删除
//输入
//idtype ：卡的类型 1指纹用户 2 密码用户 3 IC用户  4 遥控器
	//usertype : 用户类型 0 管理员 1 普通用户
//delnum : 要删除的 用户编号
//0 成功 将要删除的实际数据放到Read_Num中去
//1 失败 --不存在或者无效
unsigned char DeletenumSum(unsigned char idtype,unsigned char usertype,unsigned short delnum)
{
	unsigned char tempmid = 0;
	if(usertype == 0)//管理员
	{
		if((delnum < 1) || (delnum > 9)) //判断取值范围
		{
			return 1;
		}
		if(idtype == 1)//指纹用户
		{
			tempmid = (unsigned char)(SumGen.Value[delnum] >> 8); //先取类型判断是否为正确的
			if(tempmid == idtype)
			{
				tempmid = (unsigned char)(SumGen.Value[delnum]);//读取要删除的编号
				if((tempmid < 5) )//Fin的0-4存储管理员指纹
				{
					Fin.Read_Num = tempmid;//
					SumGen.Man_Num--;//数量减少
					SumGen.Value[delnum] = 0;//清空已经删除的信息
					
					flash_save(49, SumGen.Man_Num);
					SumStr_Save(delnum);
					SumGen.Read_Num = delnum;
					return 0;
				}
				else
				{
					return 1;
				}
			}
			else
			{
				return 1;
			}
		}
		else if(idtype == 2)//密码用户
		{
			tempmid = (unsigned char)(SumGen.Value[delnum] >> 8); //先取类型判断是否为正确的
			if(tempmid == idtype)
			{
				tempmid = (unsigned char)(SumGen.Value[delnum]);//读取要删除的编号
				if((tempmid < 2) )//Pwd的0-1存储管理员密码
				{
					Pwd.Read_Num = tempmid;//
					SumGen.Man_Num--;//数量减少
					SumGen.Value[delnum] = 0;//清空已经删除的信息
					SumGen.Read_Num = delnum;
					flash_save(49, SumGen.Man_Num);
					SumStr_Save(delnum);
					return 0;
				}
				else
				{
					return 1;
				}
			}
			else
			{
				return 1;
			}
		}	
	}
	else if(usertype == 1)//普通用户
	{
		if((delnum < 10) || (delnum >= SumMax))//判断取值范围
		{
			return 1;
		}
		if(idtype == 1)//指纹用户
		{	
			tempmid = (unsigned char)(SumGen.Value[delnum] >> 8); //先取类型判断是否为正确的
			if(tempmid == idtype)
			{
				tempmid = (unsigned char)(SumGen.Value[delnum]);//读取要删除的编号
				if((tempmid < 99) && (tempmid >= 5))//Fin的5-99存储普通用户
				{
					Fin.Read_Num = tempmid;//
					SumGen.Users_Num--;//数量减少
					SumGen.Value[delnum] = 0;//清空已经删除的信息
					SumGen.Read_Num = delnum;
					SumStr_Save(delnum);
					flash_save(50, SumGen.Users_Num);
					return 0;
				}
				else
				{
					return 1;
				}
			}
			else
			{
				return 1;
			}
		}
		else if(idtype == 2)//密码用户
		{
			tempmid = (unsigned char)(SumGen.Value[delnum] >> 8); //先取类型判断是否为正确的
			if(tempmid == idtype)
			{
				tempmid = (unsigned char)(SumGen.Value[delnum]);//读取要删除的编号
				if((tempmid < 30) && (tempmid >= 2))//的5-99存储普通用户
				{
					Pwd.Read_Num = tempmid;//
					SumGen.Users_Num--;//数量减少
					SumGen.Value[delnum] = 0;//清空已经删除的信息
					SumGen.Read_Num = delnum;
					SumStr_Save(delnum);
					flash_save(50, SumGen.Users_Num);
					return 0;
				}
				else
				{
					return 1;
				}
			}
			else
			{
				return 1;
			}
		}
		else if(idtype == 3)//IC用户
		{
			tempmid = (unsigned char)(SumGen.Value[delnum] >> 8); //先取类型判断是否为正确的
			if(tempmid == idtype)
			{
				tempmid = (unsigned char)(SumGen.Value[delnum]);//读取要删除的编号
				if((tempmid < 100) )//的0-99存储普通用户
				{
					IC.Read_Num = tempmid;//
					SumGen.Users_Num--;//数量减少
					SumGen.Value[delnum] = 0;//清空已经删除的信息
					SumGen.Read_Num = delnum;
					SumStr_Save(delnum);
					flash_save(50, SumGen.Users_Num);
					return 0;
				}
				else
				{
					return 1;
				}
			}
			else
			{
				return 1;
			}
		}
			
	}
	return 1;
}

 //清空某个库 
//输入
//idtype ：卡的类型 1指纹用户 2 密码用户 3 IC用户 
	//usertype : 用户类型 0 管理员 1 普通用户 2 清空所有
//0 成功 将要删除的实际数据放到Read_Num中去
//1 失败 --不存在或者无效
unsigned char ClearSum(unsigned char idtype,unsigned char usertype)
{
	unsigned short tempcnt = 0;
	if(usertype == 0)//管理员
	{
		if(idtype == 1)//指纹用户
		{
			for(tempcnt = 0;tempcnt < 10;tempcnt++)
			{
				if((SumGen.Value[tempcnt] & 0xff00) == 0x0100)//是这个类型的用户
				{
						SumGen.Value[tempcnt] = 0;
						SumGen.Man_Num--;//数量减少			
						SumStr_Save(tempcnt);
						flash_save(49, SumGen.Man_Num);					
				}				
			}
			return 0;
		}
		else if(idtype == 2)//密码用户
		{
			for(tempcnt = 0;tempcnt < 10;tempcnt++)
			{
				if((SumGen.Value[tempcnt] & 0xff00) == 0x0200)//是这个类型的用户
				{
						SumGen.Value[tempcnt] = 0;
						SumGen.Man_Num--;//数量减少		
						SumStr_Save(tempcnt);
						flash_save(49, SumGen.Man_Num);						
				}				
			}
			return 0;
		}			
	}
	else if(usertype == 1)//普通用户
	{
		if(idtype == 1)//指纹用户
		{
			for(tempcnt = 10;tempcnt < SumMax;tempcnt++)
			{
				if((SumGen.Value[tempcnt] & 0xff00) == 0x0100)//是这个类型的用户
				{
						SumGen.Value[tempcnt] = 0;
						SumGen.Users_Num--;//数量减少	
						SumStr_Save(tempcnt);
						flash_save(50, SumGen.Users_Num);					
				}
			}
			return 0;
		}
		else if(idtype == 2)//密码用户
		{
			for(tempcnt = 10;tempcnt < SumMax;tempcnt++)
			{
				if((SumGen.Value[tempcnt] & 0xff00) == 0x0200)//是这个类型的用户
				{
						SumGen.Value[tempcnt] = 0;
						SumGen.Users_Num--;//数量减少	
						SumStr_Save(tempcnt);
						flash_save(50, SumGen.Users_Num);					
				}
			}
			return 0;
		}
		else if(idtype == 3)//IC用户
		{
			for(tempcnt = 10;tempcnt < SumMax;tempcnt++)
			{
				if((SumGen.Value[tempcnt] & 0xff00) == 0x0300)//是这个类型的用户
				{
						SumGen.Value[tempcnt] = 0;
						SumGen.Users_Num--;//数量减少	
						SumStr_Save(tempcnt);
						flash_save(50, SumGen.Users_Num);					
				}
			}
			return 0;
		}
		else if(idtype == 4)//遥控用户
		{
			for(tempcnt = 10;tempcnt < SumMax;tempcnt++)
			{
				if((SumGen.Value[tempcnt] & 0xff00) == 0x0400)//是这个类型的用户
				{
						SumGen.Value[tempcnt] = 0;
						SumGen.Users_Num--;//数量减少		
						SumStr_Save(tempcnt);
						flash_save(50, SumGen.Users_Num);					
				}
			}
			return 0;
		} 
			
	}
	else if(usertype == 2)//所有用户
	{		
		memset((void*)SumGen.Value,0,sizeof((void*)SumGen.Value));
		SumGen.Man_Num = 0;
		SumGen.Users_Num = 0;
		SumGen.Man_Save_Num = 1;
		SumGen.Users_Save_Num = 10;
		SumGen.ManFullFlag = 0;
		SumGen.UserFullFlag = 0;
		
		flash_save(49, SumGen.Man_Num);
		flash_save(50, SumGen.Users_Num);
		flash_save(51, SumGen.Man_Save_Num);
		flash_save(52, SumGen.Users_Save_Num);
		flash_save(53, SumGen.ManFullFlag);
		flash_save(54, SumGen.UserFullFlag);
		
		SumStr_Savezero();
		return 0;
	}
	return 1;
}


//开门方式
void Open_Way(u8 mode)
{
	if(mode == 0) //单验证
	{
			Pwd.Flag = 0;
			Fin.Flag = 0;
			IC.Flag = 0;

			if(Pwd.Status == 1 || IC.Status == 1 || Fin.Status == 1 || Remote_status == 1)
			{
					Status_Lock = 1;
					Mode = 3;
					IC.Status = 0;
					Pwd.Status = 0;
					Fin.Status = 0;		
					Remote_status = 0;
			}
	}
	else if (mode == 1) //指纹加密码
	{
		IC.Flag = 1;
		Pwd.Flag = 0;
		Fin.Flag = 0;
		if(Pwd.Status == 1&&Double_Verify!=2)
		{
				//语音
				Pwd.Flag = 1;
				Pwd.Error = 0;
				if(Open_Way_flag == 0)
				{
						JQ8400_SendCmd(3,8,1,1);
						delay_ms(1200);
						Open_Way_flag = 1;
				}
				if(Fin.Status == 1&&Double_Verify==1)
				{
					Double_Verify=0;
					Pwd.Status = 0;
					Fin.Status = 0;
					Fin.Flag = 0;
					Pwd.Flag = 0;
					Mode = 3;
					Double_Verify=0;
					Status_Lock = 1;
				}
				else if(Fin.Status == 0)
					Double_Verify = 1;
		}
		else if(Fin.Status == 1)
		{
				//语音
				Fin.Flag = 1;
				Fin.Error = 0;

				if(Open_Way_flag == 0)
				{
						JQ8400_SendCmd(7,8,1,1);
						delay_ms(1000);
						Open_Way_flag = 1;
				}
			
				if(Pwd.Status == 1&&Double_Verify==2)
				{
						Double_Verify=0;
						Pwd.Status = 0;
						Fin.Status = 0;
						Mode = 3;
						 
						Status_Lock = 1;
				}
				else if(Pwd.Status == 0)
				 Double_Verify = 2;
		}
		else
		{
				Open_Way_flag = 0;
		}
	}
	else if (mode == 2) //指纹加IC卡
	{
		Pwd.Flag = 1;
		IC.Flag = 0;
		Fin.Flag = 0;
		if(Fin.Status == 1&&Double_Verify!=2)
		{
			//语音
			if(Open_Way_flag == 0)
			{
					//请输入IC卡
				  JQ8400_SendCmd(3,7,1,1);
					delay_ms(1000);
					Open_Way_flag = 1;
					Fin.Error = 0;
			}
			Fin.Flag = 1;
			if(IC.Status == 1&&Double_Verify==1)
			{
					Double_Verify=0;
					IC.Status = 0;
					Fin.Status = 0;
					IC.Flag = 0;
					Fin.Flag = 0;				 
					Status_Lock = 1;
			}
			else if(IC.Status == 0)
				Double_Verify = 1;
		}
		else if(IC.Status == 1)
		{
			//语音
			if(Open_Way_flag == 0)
			{
					IC.Error = 0;
					//请输入指纹
				  JQ8400_SendCmd(3,8,1,1);
					delay_ms(1000);
					Open_Way_flag = 1;
			}

			IC.Flag = 1;

			if(Fin.Status == 1&&Double_Verify==2)
			{
					Double_Verify=0;
					IC.Status = 0;
					Fin.Status = 0;
					IC.Flag = 0;
					Fin.Flag = 0;
					Status_Lock = 1;
			}
			else if(Fin.Status == 0)
				Double_Verify = 2;
		}
		else
		{
			Open_Way_flag = 0;
		}
	}

	if(Status_Lock > 0)
	{		
		Reset_Error_Flag(User);
		value_num = 0;
		if(Hold_Open_Door == ON)
		{		 
//			Voice_1A_WT588S(0x0014);
//			Close_Door();
			Hold_Open_Door=OFF;
			OLED_Clear();
			Display_timefun(calendar.min,calendar.hour,calendar.w_date,calendar.w_month,calendar.w_year,0);
			OLED_Refresh_Gram();
			delay_ms(1000);
			delay_ms(1000);
		}
		else if(Hold_Open_Door == OFF)
		{ 
			//开门音效
			JQ8400_SendCmd(2,1,1,1);
//			Open_Door();
			delay_ms(500);
			show_flag = 0;	
//			Close_Door();
			OLED_Clear();
			Display_timefun(calendar.min,calendar.hour,calendar.w_date,calendar.w_month,calendar.w_year,0);
			OLED_Refresh_Gram();
			delay_ms(1000);
			delay_ms(1000);
			Mode = 3;
			Pwd.Flag = 0;
		}
		Status_Lock = 0;
	}
}
