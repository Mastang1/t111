#ifndef __FUN_H
#define __FUN_H
#include "stm32f10x.h"//头文件引用

/* 声明全局函数 */
void Sys_reset(void);		//系统复位执行程序
void AUTO_Check(void);	//系统自检执行程序



void USART1CMD_exe(void);//串口1指令执行程序
void USART2CMD_exe(void);//串口2指令执行程序

void Usart_mode_ON(void);
void Usart_mode_OFF(void);
void Send_ask_IP_com(void);
//uint8_t IP_Receive(void);
void FB_state(void);
/*===============变量声明======================================*/
extern uint8_t Usart_ON[];
extern uint8_t Usart_OFF[];
extern uint8_t Send_IP_Com[];
extern uint8_t NORMAL[6];//"NORMAL"
extern uint8_t WERROR[5];//"ERROR"


#endif

