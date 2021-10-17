#ifndef _relay_H
#define _relay_H
#include "stm32f10x.h"


#define RELAY_x GPIO_Pin_All	//�ܽź궨��
#define LACK_CLK GPIO_Pin_6	//������ʱ������ΪPC6


#define SW_ALL (uint16_t)0x3f00
/*=============LED���ض���=========================*/
#define LED_GREEN GPIO_SetBits(GPIOF, GPIO_Pin_7)
#define LED_RED GPIO_ResetBits(GPIOF, GPIO_Pin_7)


	
/*=============�̵�����صĳ�ʼ������=========================*/
void Relay_INI(void);
static void Relay_Pin_Ini(void);				//�̵���������ʼ��
static void Lat_Clk_Ini(void);
static void P_edge(void);		//��һ��������
/*=============�̵�����������==========================*/
void Relay_ON (u16  RELA_x);
void Relay_OFF (u16 RELA_x);
/*=============ģ�⿪�ز�������==========================*/
void Analog_sw_ON (u16 SW_x);
void Analog_sw_OFF (u16 SW_x);


#endif

