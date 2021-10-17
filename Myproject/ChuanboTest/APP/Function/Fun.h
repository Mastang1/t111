#ifndef __FUN_H
#define __FUN_H
#include "stm32f10x.h"//头文件引用
#include "SPI.h"
#include "systick.h"
#include "printf.h"
#include "relay.h"



/* 声明全局函数 */
void Sys_reset(void);
void Self_check(void);



/* 定义各通道的片选引脚 */
//#define CH1_CS GPIO_Pin_0 



#endif

