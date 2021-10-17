#ifndef _printf_H
#define _printf_H
#include "stm32f10x.h"
#include "stdio.h"
/*===========================����������=======================*/
extern __IO u8		RxCounter1;
extern __IO u8		RxCounter2;
extern __IO u8 RxBuffer1[29];
extern __IO u8 RxBuffer2[10];
extern __IO FlagStatus FLAG_Rx;
extern __IO FlagStatus RECEIVE_flag2;

extern __IO FlagStatus IP_ask_flag;	


extern __IO u8 RxBuffer2[10];
extern __IO u8 RxBuffer2N[10];
extern __IO FlagStatus FLAG_Rx;
extern __IO FlagStatus RECEIVE_flag2;

/*===========================������������Ĭ�ϴ�extern��=======================*/
int fputc(int ch,FILE *p);

void USART1_INI(u32 baud);//Usart1 INi
void My_USART1_IRQHandler(void);

void USART1_Send_Char(u8 temp);				
void USART1_Send_Char_Buff(u8 buf[],u32 len) ;//����1���鷢��
/*=============About Usart2====================================*/	
void USART2_INI(u32 baud);
void My_USART2_IRQHandler(void);						//�����жϺ���
void USART2_Send_Char(u8 temp);				
void USART2_Send_Char_Buff(u8 buf[],u32 len) ;//����3���鷢��
/*=============About Usart3====================================*/
void USART3_INI(u32 baud);
void USART3_Send_Char(u8 temp);				
void USART3_Send_Char_Buff(u8 buf[],u32 len) ;//����3���鷢��
void My_USART3_IRQHandler(void);

static void Comand_Rx(uint8_t buffer);
static void IP_pack_Rx(uint8_t buffer);



#endif
