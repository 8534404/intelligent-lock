#include "include.h"

void HC_SR501_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;		 //PB2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD ; //���ó���������
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOC5
}


u8 HC_SR501_Statue(void)
{
	if(PBin(2) == 1)
	{
		return 1;
		
	}
	return 0;
}
