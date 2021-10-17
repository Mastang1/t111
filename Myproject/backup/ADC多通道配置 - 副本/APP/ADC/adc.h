#ifndef _adc_H
#define _adc_H
#include "stm32f10x.h"


#define N 10 //每通道采50次
#define M 6 //为12个通道

void Init_All_Periph(void);
void filter(void);
u16 GetVolt(u16 advalue);
void filter(void);




#endif
