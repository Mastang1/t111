#ifndef __FUN_H
#define __FUN_H
#include "stm32f10x.h"//ͷ�ļ�����
#include "SPI.h"
#include "systick.h"
#include "printf.h"
#include "relay.h"



/* ����ȫ�ֺ��� */
void Sys_reset(void);
void Self_check(void);



/* �����ͨ����Ƭѡ���� */
//#define CH1_CS GPIO_Pin_0 



#endif

