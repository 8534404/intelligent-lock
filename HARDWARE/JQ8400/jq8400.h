#ifndef __JQ8400_H
#define __JQ8400_H
#include "include.h"



#define JQ8400_SDA 		PBout(0)
#define JQ8400_BUSY 	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)

void JQ8400_Init(void);
void JQ8400_SendData(uint8_t addr);
void JQ8400_SendCmd(u8 data1,u8 data2,u8 flag1,u8 CmdorMusic);
void JQ8400_SetVolume(u8 flag);
void JQ8400_VoliceNum(u8 type,u8 mode);





#endif
