#ifndef _LCDDV_H
#define _LCDDV_H
#include "stm32f10x.h"
#include "stdio.h"


void LCD_Comand(uint8_t _Comand[]);																//2�ֽ�����ִ��
void LCD_display(uint8_t _parm[],uint8_t _Comand[],uint8_t len,uint8_t Line,uint8_t Rank) ;	//������ʾ
static  void CSRPos_set(uint8_t Line,uint8_t Rank);	
void IP_display(void) ;//IP��ַ������ʾ


extern uint8_t Reset_Fback;	//Һ����λ
extern uint8_t Self_Chack_Fback;	//
extern uint8_t Reset_LCD[2];	//Һ����λ
extern uint8_t Clear_LCD[2];	//Һ������
extern uint8_t CSR_Pos[2];		//���λ������ָ�������������
extern uint8_t __IP[15];

extern uint8_t LCD_Display_CMD[2];

extern uint8_t NORMAL[];//"NORMAL"
extern uint8_t WERROR[];//"ERROR"
extern uint8_t FAULT[];//"FAULT"
#endif

