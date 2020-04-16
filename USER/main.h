#ifndef __MAIN_H
#define __MAIN_H
#include "include.h"

#define START  0X01
#define END  	 0X02

#define AgainSpeak '#'
#define AgainSpeakLed  0xf7ff;

#define  Management 0x00
#define  User			  0x01
#define  NONE		  	0x02

#define  ADD 				0x01
#define  DEL			  0x00

#define FINPT			 	0x01
#define PASSWORD 		0x02
#define ICCARD			0x03
#define	ALL					0x04
#define	DELUSER			0x06
#define	PHONE				0x05
#define	RESETCMD				0x07

#define FINPT_Saveaddr 		FINPT*1024*1024
#define PASSWORD_Saveaddr PASSWORD*1024*1024
#define ICCARD_Saveaddr 	ICCARD*1024*1024

#define  FLASH_APP1_START_ADDR		0X08040000 	// 0x0800F000
#define  STM_PAGE_SIZE   					1024*2

#define Man_FINPT_MAX				5 	
#define Man_PASSWORD_MAX 		2

#define Man_MAX							7 	
#define User_MAX 						430

#define PWD_USER_MAX 28
#define FIN_USER_MAX 95
#define IC_USER_MAX	100

#define  Single_Validation  0x00
#define  Fin_Plus_Pwd  			0x01
#define  Fin_Plus_IC	 		  0x02
#define  Pwd_Plus_IC 			  0x03
#define  Fin_Plus_Fin			  0x04

#define ON 0x00
#define OFF 0x01

#define Open 	0x00
#define Close 0x01

#define Chinese 0x00
#define English 0x01
#define SumMax 320
#define TOUCH_Timeout           1000 

__packed typedef struct
{
	uint32_t  write_data[260];
	uint32_t  read_data[260];
}flash;

__packed typedef struct
{
	u8	Flag;
	u8	Value[30][11];
	u8 	Users_Num;
	u8	Status;
	u8	Error;
	u8 	Man_Num;
	u8	Man_Error;
	u8  Users_Save_Num;
	u8  Man_Save_Num;
	u8  Read_Num;
	u8 	UserFullFlag;
	u8 	ManFullFlag;
}pwd;

__packed typedef struct
{
	u8	Flag;
	u8	Value[100];
	u8 	Users_Num;
	u8	Status;
	u8	Error;
	u8 	Man_Num;
	u8	Man_Error;
	u8  Users_Save_Num;
	u8  Man_Save_Num;
	u8  Read_Num;
	u8 	UserFullFlag;
	u8 	ManFullFlag;
}fin;

__packed typedef struct
{
	u8	 Flag;
	u32  Value[200];
	u8	 Error;
	u8	 Status;
	u8   Users_Num;
	u8 	 Save_Num;
	u32  Read_Num;
	u8 	 UserFullFlag;
}ic;

__packed typedef struct
{
	u8	Flag;
	u16	Value[320]; //001-009为管理员 010-299为普通用户  高八位存储用户类型  1指纹用户 2 密码用户 3 IC用户 第八位存储密码或指纹或者IC卡的存储位置
	u16 Users_Num;//已注册的用户数量
	u8	Status;//状态
	u8	Error;//错误
	u8 	Man_Num;//已注册的管理员数量
	u8	Man_Error;
	u16  Users_Save_Num;//普通用户存储地址
	u8  Man_Save_Num;//管理员存储地址
	u16  Read_Num;//读出的用户值
	u8  UserFullFlag;//用户已经到达满标志
	u8  ManFullFlag;//管理员已经到达满标志
	u16 Save_Num;
	u16 DisplayUsers_Save_Num;//普通用户存储地址
	u8  DisplayMan_Save_Num;//管理员存储地址
}SumStr;

extern SumStr SumGen;
extern flash Flash;
extern fin Fin;
extern pwd Pwd;
extern ic IC;
extern u8 FIN_FLAG;
extern unsigned int menu_now;
extern u8 Touch_flag;
extern u8	re_flag;
extern u8 Mode;
extern u8	re_flag;
extern u8 ic_switch;
extern u8 Verify_Flag;
extern u8 Language;
extern u8 Page0;
extern u8 Page1;
extern u8 Page2;
extern u8 Page3;
extern u8 Page0_0;
extern u8 Page1_1;
extern u8 Page2_2;
extern u8 Page3_3;
extern u8 voice_flag;
extern unsigned int lowerpower_now;
extern unsigned int menu_now,Low_Timeout;
extern unsigned char touch_flag;
extern unsigned int timingdelay, TimingDelay;
extern u8 Volume_Flag;
extern u8 Temp_Pwd[22];
extern u8 value_num;
extern unsigned int pwd_timeout;
extern unsigned short nIndex_temp[2];
extern unsigned short nIndex_num;
extern u16 man_num,user_num;
extern u8 show_flag;
extern u8 page_ic;
extern u8 reset_sw;
extern u8 Error_Flag;
extern u8 timer3_open,TimingDelay_flag;
extern u16 time_sec,time_min;
extern u8 Remote_Error;
extern u8 Remote_status;
extern u8 Remote_flag;
#endif 
