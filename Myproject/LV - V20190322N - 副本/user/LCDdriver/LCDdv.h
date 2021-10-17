#ifndef _LCDDV_H
#define _LCDDV_H
#include "stm32f10x.h"
#include "stdio.h"


void LCD_Comand(uint8_t _Comand[]);																//2字节命令执行
void LCD_display(uint8_t _parm[],uint8_t _Comand[],uint8_t len,uint8_t Line,uint8_t Rank) ;	//数据显示
static  void CSRPos_set(uint8_t Line,uint8_t Rank);	
void IP_display(void) ;//IP地址接收显示


extern uint8_t Reset_Fback;	//液晶复位
extern uint8_t Self_Chack_Fback;	//
extern uint8_t Reset_LCD[2];	//液晶复位
extern uint8_t Clear_LCD[2];	//液晶清屏
extern uint8_t CSR_Pos[2];		//光标位置设置指令，后续发送数据
extern uint8_t __IP[15];

extern uint8_t LCD_Display_CMD[2];

extern uint8_t NORMAL[];//"NORMAL"
extern uint8_t WERROR[];//"ERROR"
extern uint8_t FAULT[];//"FAULT"
#endif

