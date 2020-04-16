#include "include.h"

void JQ8400_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_0);
}

void JQ8400_SendData(uint8_t addr)
{
	uint8_t i;
	
	JQ8400_SDA = 1;
	delay_ms(1);
	JQ8400_SDA = 0;
	delay_ms(3);
	
	for(i=0;i<8;i++)
	{
		JQ8400_SDA = 1;
		if(addr & 0x01)
		{
			delay_us(600);
			JQ8400_SDA = 0;
			delay_us(200);
		}
		else
		{
			delay_us(200);
			JQ8400_SDA = 0;
			delay_us(600);
		}
		addr >>= 1;
	}
	JQ8400_SDA = 1;
}
//flag1 0 = 单个数字  1 = 两个数字 hex 
//CmdorMusic 0 = 发命令    1 = 发歌
void JQ8400_SendCmd(u8 data1,u8 data2,u8 flag1,u8 CmdorMusic)
{
	JQ8400_SendData(0x0a);
	if(flag1 == 0)
	{
		JQ8400_SendData(data1);
	}
	else if(flag1 == 1)
	{
		JQ8400_SendData(data1);
		JQ8400_SendData(data2);
	}
	if(CmdorMusic == 0)
	{
		JQ8400_SendData(0x0c);
	}
	else if(CmdorMusic == 1)
	{
		JQ8400_SendData(0x0b);
	}
}

void JQ8400_SetVolume(u8 flag)
{
	if(flag == 2)//高
	{
		JQ8400_SendCmd(3,0,1,0);
	}
  else if(flag == 1)//中
	{
		JQ8400_SendCmd(2,0,1,0);
	}
	else if(flag == 0)//低
	{
		JQ8400_SendCmd(5,0,0,0);
	}
	else if(flag == 3)//静音
	{
		JQ8400_SendCmd(0,0,0,0);
	}
}
//读用户编号
//type 0 管理员  1 普通用户
//mode 0 添加成功 1 删除成功
u16 testtable[6] = {0};
void JQ8400_VoliceNum(u8 type,u8 mode)
{
	u16 tempvoicecnt = 0;
	if(mode == 0)
	{
		//操作成功
		JQ8400_SendCmd(1,9,1,1);
		delay_ms(800);
	}
	else if(mode == 1)
	{
		//操作失败
		JQ8400_SendCmd(1,9,1,1);
		delay_ms(800);
	}
	delay_ms(300);
	if(type == User)
	{
		//用户编号
		delay_ms(300);
		tempvoicecnt = SumGen.DisplayUsers_Save_Num/100;
		testtable[0] = tempvoicecnt;
		JQ8400_SendCmd(tempvoicecnt,0,0,1);
		delay_ms(300);
		
		tempvoicecnt = SumGen.DisplayUsers_Save_Num/10%10;
		testtable[1] = tempvoicecnt;
		JQ8400_SendCmd(tempvoicecnt,0,0,1);
		delay_ms(300);
		
		tempvoicecnt = SumGen.DisplayUsers_Save_Num%10;
		testtable[2] = tempvoicecnt;
		JQ8400_SendCmd(tempvoicecnt,0,0,1);
		delay_ms(300);
	}
	else if(type == Management)
	{
		//管理员编号
		delay_ms(1000); 
		
		tempvoicecnt = SumGen.DisplayMan_Save_Num/100;
		testtable[3] = tempvoicecnt;
		JQ8400_SendCmd(tempvoicecnt,0,0,1);
		delay_ms(300);
		
		tempvoicecnt = SumGen.DisplayMan_Save_Num/10%10;
		testtable[4] = tempvoicecnt;
		JQ8400_SendCmd(tempvoicecnt,0,0,1);
		delay_ms(300);
		
		tempvoicecnt = SumGen.DisplayMan_Save_Num%10;
		testtable[5] = tempvoicecnt;
		JQ8400_SendCmd(tempvoicecnt,0,0,1);
		delay_ms(300);
	}
}
