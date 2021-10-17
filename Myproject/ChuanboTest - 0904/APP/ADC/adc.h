#ifndef __ADC_H
#define	__ADC_H


#include "stm32f10x.h"
#include "public.h"


#define N 100 //ÿͨ����50��
#define M 2 //Ϊ12��ͨ��

extern vu16 AD_Value[N][M]; //�������ADCת�������Ҳ��DMA��Ŀ���ַ
extern vu16 After_filter[M]; //���������ƽ��ֵ֮��Ľ��

void Init_ADC_Periph(void);
u16 GetVolt(u16 advalue);
//void filter(void);


#endif /* __ADC_H */

