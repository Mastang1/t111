#ifndef _adcscan_H
#define _adcscan_H
#include "stm32f10x.h"


#define N 6//Ϊ6��ͨ��
#define M 50 	//ÿͨ����10��

void Init_ADC1_Periph(void);

void filter(void);

u16 GetVolt(u16 advalue);

void ADC_RCC_Configuration(void);
void ADC_GPIO_Configuration(void);
void ADC1_Configuration(void);
void ADC_DMA_Configuration(void);


#endif

