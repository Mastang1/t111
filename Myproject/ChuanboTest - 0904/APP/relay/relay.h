#ifndef _relay_H
#define _relay_H
#include "stm32f10x.h"


#define RELAY_x GPIO_Pin_All	//管脚宏定义
#define LACK_CLK GPIO_Pin_6	//锁存器时钟设置为PC6


#define SW_ALL (uint16_t)0x3f00
/*=============LED开关定义=========================*/
#define LED_GREEN GPIO_SetBits(GPIOF, GPIO_Pin_7)
#define LED_RED GPIO_ResetBits(GPIOF, GPIO_Pin_7)


	
/*=============继电器相关的初始化函数=========================*/
void Relay_INI(void);
static void Relay_Pin_Ini(void);				//继电器驱动初始化
static void Lat_Clk_Ini(void);
static void P_edge(void);		//来一个上升沿
/*=============继电器操作函数==========================*/
void Relay_ON (u16  RELA_x);
void Relay_OFF (u16 RELA_x);
/*=============模拟开关操作函数==========================*/
void Analog_sw_ON (u16 SW_x);
void Analog_sw_OFF (u16 SW_x);


#endif

