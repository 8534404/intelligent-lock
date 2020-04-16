#ifndef __AS608_H
#define __AS608_H
#include "include.h"

#define PS_Sta   PCin(4)//读指纹模块状态引脚
#define FigOnFlag GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4) //高电平说明有指纹

#define CharBuffer1 0x01
#define CharBuffer2 0x02

#define MODULE_DBSize 100
#define ERR_OK					0x00		// 成功
#define ERR_DATARECEIVE			0x01		//表示数据包接收错误;
#define ERR_NOFINGER			0x02		//表示传感器上没有手指;
#define ERR_READIMAGE			0x03		//表示录入指纹图像失败;
#define ERR_DRYFINGER			0x04		//表示指纹图像太干、太淡而生不成特征;
#define ERR_WETFINGER			0x05		//表示指纹图像太湿、太糊而生不成特征;
#define ERR_MIXFINGER			0x06		//表示指纹图像太乱而生不成特征;
#define ERR_LACKCHAR			0x07		//表示指纹图像正常,但特征点太少（或面积太小）而生不成特征;
#define ERR_MACTHCHAR			0x08  		//表示指纹不匹配;
#define ERR_SEARCHCHAR			0x09  		//表示没搜索到指纹;
#define ERR_MERGECHAR			0x0a  		//表示特征合并失败;
#define ERR_OUTOFINDEX			0x0b  		//表示访问指纹库时地址序号超出指纹库范围;
#define ERR_READTEMPLATE		0x0c  		//表示从指纹库读模板出错或无效;
#define ERR_UPCHAR				0x0d  		//表示上传特征失败;
#define ERR_RECEIVENEXT			0x0e  		//表示模块不能接收后续数据包;
#define ERR_UPIMAGE				0x0f  		//表示上传图像失败;
#define ERR_DELETETEMPLATE		0x10  		//表示删除模板失败;
#define ERR_CLEARDATABASE		0x11  		//表示清空指纹库失败;
#define ERR_SLEEP				0x12  		//表示不能进入低功耗状态;
#define ERR_PIN					0x13  		//表示口令不正确;
#define ERR_RESET				0x14  		//表示系统复位失败;
#define ERR_NORAWIMAGE			0x15  		//表示缓冲区内没有有效原始图而生不成图像;
#define ERR_IAP					0x16  		//表示在线升级失败;
#define ERR_SAMEIMAGE			0x17  		//表示残留指纹或两次采集之间手指没有移动过;
#define ERR_ACCESSFLASH			0x18  		//表示读写FLASH出错;
#define ERR_GENERATERANDOM		0x19  		//随机数生成失败;
#define ERR_INVALIDREGISTER		0x1a  		//无效寄存器号;
#define ERR_FULLDATABASE		0x1f  		//指纹库满;
#define ERR_PASSWORD			0x21  		//密码有误;
#define ERR_TEMPLATENOTNULL		0x22  		//指纹模板非空;
#define ERR_TEMPLATEISNULL		0x23  		//指纹模板为空;
#define ERR_DATABASEISNULL		0x24  		//指纹库为空;
#define ERR_ENROLLCOUNT			0x25  		//录入次数设置错误;
#define ERR_OVERTIME			0x26  		//超时;
#define ERR_TEMPLATEEXIST		0x27  		//指纹已存在;
#define ERR_TEMPLATERELATED		0x28  		//指纹模板有关联;


#define ERR_ADMINOVER        0X2A //管理员已满
#define ERR_FIGNOLEAVE     0X2B//指纹没有离开模块
#define ERR_MODECOMFAIL   0X2C//模板合并失败
#define ERR_GETINDEXFAIL  0X2D//获取第一个可注册索引失败
#define ERR_SAVEFINGFAIL  0X2E //存储失败
#define NoIdFIG           0X2F  //不进行指纹识别
#define ERR_ISPUBLIC     0X30//将要录入的管理员指纹已经属于普通指纹
//2AH—EFH									//Reserved;
#define ERR_F0RESPOND			0xf0  		//有后续数据包的指令,正确接收后用0xf0应答;
#define ERR_F1RESPOND			0xf1  		//有后续数据包的指令,命令包用0xf1应答;
#define ERR_FLASHPACKAGECHECK	0xf2  		//表示烧写内部FLASH时,校验和错误;
#define ERR_FLASHPACKAGEFLAG	0xf3  		//表示烧写内部FLASH时,包标识错误;
#define ERR_FLASHPACKAGELENGTH	0xf4  		//表示烧写内部FLASH时,包长度错误;
#define ERR_FLASHPACKAGECODE	0xf5		//表示烧写内部FLASH时,代码长度太长;
#define ERR_FLASHPACKAGEBURN	0xf6		//表示烧写内部FLASH时,烧写FLASH失败;
//F7-FF									    //Reserved;

#define CMD_RUNEXT 0x00
#define CMD_COMPLETE 0x01


extern u32 AS608Addr;//模块地址

typedef struct  
{
	u16 pageID;//指纹ID
	u16 mathscore;//匹配得分
}SearchResult;

typedef struct
{
	u16 PS_max;//指纹最大容量
	u8  PS_level;//安全等级
	u32 PS_addr;
	u8  PS_size;//通讯数据包大小
	u8  PS_N;//波特率基数N
}SysPara;

void PS_StaGPIO_Init(void);//初始化PA6读状态引脚
	
u8 PS_GetImage(void); //录入图像 
 
u8 PS_GenChar(u8 BufferID);//生成特征 

u8 PS_Match(void);//精确比对两枚指纹特征 

u8 PS_Search(u8 BufferID,u16 StartPage,u16 PageNum,unsigned short *nPageID, unsigned short *nScore);//搜索指纹 
 
u8 PS_RegModel(void);//合并特征（生成模板） 
 
u8 PS_StoreChar(u8 BufferID,u16 PageID);//储存模板 

u8 PS_DeletChar(u16 PageID,u16 N);//删除模板 

u8 PS_Empty(void);//清空指纹库 

u8 PS_WriteReg(u8 RegNum,u8 DATA);//写系统寄存器 
 
u8 PS_ReadSysPara(SysPara *p); //读系统基本参数 

u8 PS_SetAddr(u32 addr);  //设置模块地址 

u8 PS_HandShake(u32 *PS_Addr); //与AS608模块握手 

int AutoEnrollPro(unsigned char intype); 
int AutoIdentifyPro(unsigned char identype);
int DeelOnFigAndUSERPro(unsigned char usernum);
int EmptyUserFig(void);
#endif

