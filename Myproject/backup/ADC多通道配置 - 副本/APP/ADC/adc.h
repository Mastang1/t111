#ifndef _adc_H
#define _adc_H
#include "stm32f10x.h"


#define N 10 //ÿͨ����50��
#define M 6 //Ϊ12��ͨ��

void Init_All_Periph(void);
void filter(void);
u16 GetVolt(u16 advalue);
void filter(void);




#endif
