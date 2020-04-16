#include "include.h"


//串口接收缓存区 	
u8 UART5_RX_BUF[UART5_MAX_RECV_LEN]; 			//接收缓冲,最大UART5_MAX_RECV_LEN个字节.
u8 UART5_TX_BUF[UART5_MAX_SEND_LEN]; 			  //发送缓冲,最大UART5_MAX_SEND_LEN字节

//通过判断接收连续2个字符之间的时间差不大于10ms来决定是不是一次连续的数据.
//如果2个字符接收间隔超过10ms,则认为不是1次连续数据.也就是超过10ms没有接收到
//任何数据,则表示此次接收完毕.
//接收到的数据状态
//[15]:0,没有接收到数据;1,接收到了一批数据.
//[14:0]:接收到的数据长度
vu16 UART5_RX_STA=0;  

void UART5_IRQHandler(void)
{
	u8 res;	      
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)//接收到数据
	{	 
		res =USART_ReceiveData(UART5);		 
		if((UART5_RX_STA&(1<<15))==0)//接收完的一批数据,还没有被处理,则不再接收其他数据
		{ 
			if(UART5_RX_STA<UART5_MAX_RECV_LEN)	//还可以接收数据
			{
				TIM_SetCounter(TIM4,0);//计数器清空          				//计数器清空
				if(UART5_RX_STA==0) 				//使能定时器4的中断 
				{
					TIM_Cmd(TIM4,ENABLE);//使能定时器4
				}
				UART5_RX_BUF[UART5_RX_STA++]=res;	//记录接收到的值	 
			}else 
			{
				UART5_RX_STA|=1<<15;				//强制标记接收完成
			} 
		}
	}  				 											 
}   

//初始化IO 串口5
//pclk1:PCLK1时钟频率(Mhz)
//bound:波特率	  
void uart5_init(u32 bound)
{  

	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);	// GPIOC,GPIOD时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE); //串口5时钟使能

 	USART_DeInit(UART5);  //复位串口5
		 //UART5_TX   PC12
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //PC12
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOC, &GPIO_InitStructure); //初始化PC12

	//UART5_RX	  PD2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//浮空输入
	GPIO_Init(GPIOD, &GPIO_InitStructure);  //初始化PD2
	
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  
	USART_Init(UART5, &USART_InitStructure); //初始化串口5
  

	USART_Cmd(UART5, ENABLE);                    //使能串口 
	
	//使能接收中断
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//开启中断   
	
	//设置中断优先级
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	
	TIM4_Int_Init(99,7199);		//10ms中断
	UART5_RX_STA=0;		//清零
	TIM_Cmd(TIM4,DISABLE);			//关闭定时器4

}

//串口5,printf 函数
//确保一次发送数据不超过UART5_MAX_SEND_LEN字节
void u5_printf(char* fmt,...)  
{  
	u16 i,j; 
	va_list ap; 
	va_start(ap,fmt);
	vsprintf((char*)UART5_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)UART5_TX_BUF);		//此次发送数据的长度
	for(j=0;j<i;j++)							//循环发送数据
	{
	  while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET); //循环发送,直到发送完毕   
		USART_SendData(UART5,UART5_TX_BUF[j]); 
	} 
}
