#ifndef __FUN_H
#define __FUN_H
#include "stm32f10x.h"//ͷ�ļ�����

/* ����ȫ�ֺ��� */
void Sys_reset(void);		//ϵͳ��λִ�г���
void AUTO_Check(void);	//ϵͳ�Լ�ִ�г���



void USART1CMD_exe(void);//����1ָ��ִ�г���
void USART2CMD_exe(void);//����2ָ��ִ�г���

void Usart_mode_ON(void);
void Usart_mode_OFF(void);
void Send_ask_IP_com(void);
//uint8_t IP_Receive(void);
void FB_state(void);
/*===============��������======================================*/
extern uint8_t Usart_ON[];
extern uint8_t Usart_OFF[];
extern uint8_t Send_IP_Com[];
extern uint8_t NORMAL[6];//"NORMAL"
extern uint8_t WERROR[5];//"ERROR"


#endif

