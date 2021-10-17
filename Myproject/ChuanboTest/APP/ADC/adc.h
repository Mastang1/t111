#ifndef __ADC_H
#define	__ADC_H


#include "stm32f10x.h"
#include "public.h"


#define N 100 //每通道采50次
#define M 2 //为12个通道

extern vu16 AD_Value[N][M]; //用来存放ADC转换结果，也是DMA的目标地址
extern vu16 After_filter[M]; //用来存放求平均值之后的结果

void Init_ADC_Periph(void);
u16 GetVolt(u16 advalue);
//void filter(void);


#endif /* __ADC_H */

