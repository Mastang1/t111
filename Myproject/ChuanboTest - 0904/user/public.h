#ifndef _public_H
#define _public_H

#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "misc.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_tim.h"

/**********************�궨��**************************/
#define RecSuccess 0x11
#define RecData    0x22
#define UartCnt    0x33
#define FLAG_1		 1;
#define FLAG_0		 0;



/**********************ȫ�ֱ�������**************************/
//ȫ�ֱ�������
extern unsigned int  Flag_HMICMD;
extern unsigned int  Flag_UART1;
extern unsigned char glRecuart3Buff[10];
extern unsigned char FlagTaskHMI;
extern unsigned int flag_YN;
extern unsigned int test_YN;
extern __IO FlagStatus RECEIVE_flag2;
extern u32 Banttesla;
extern unsigned int flag_RSuc;
/*==================���������ʼ������===================*/
void Base_INI(void);
void PWR_UP(void);
extern int HexToString(unsigned char *psHex, char *pszStr,
                unsigned int uiHexLen, unsigned int uiBytesPerLine);










#endif
