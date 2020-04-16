#ifndef __KEY_H
#define __KEY_H	 
#include "include.h"


#define KEY0  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)//读取按键0
#define KEY1  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)//读取按键1
#define WK_UP   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//读取按键2 
 

#define KEY0_PRES	1		//KEY0  
#define KEY1_PRES	2		//KEY1 
#define WKUP_PRES	4		//WK_UP  

#define Read_Lie1 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)
#define Read_Lie2 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)
#define Read_Lie3 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)



void KEY_Init(void);//IO初始化
u8 KEY_Scan(void);  	//按键扫描函数		
void Deal_KEY(u8 data);
u8 KEY_Scan3x4(void);

#endif
