#ifndef __UART5_H
#define __UART5_H	 
#include "include.h"     

#define UART5_MAX_RECV_LEN		400					//�����ջ����ֽ���
#define UART5_MAX_SEND_LEN		400					//����ͻ����ֽ���
#define UART5_RX_EN 			1					//0,������;1,����.

extern u8  UART5_RX_BUF[UART5_MAX_RECV_LEN]; 		//���ջ���,���UART5_MAX_RECV_LEN�ֽ�
extern u8  UART5_TX_BUF[UART5_MAX_SEND_LEN]; 		//���ͻ���,���UART5_MAX_SEND_LEN�ֽ�
extern vu16 UART5_RX_STA;   						//��������״̬

void uart5_init(u32 bound);				//����5��ʼ�� 
void u5_printf(char* fmt,...);
#endif
