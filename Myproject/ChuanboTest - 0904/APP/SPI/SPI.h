#ifndef __SPI_H
#define __SPI_H

#include "stm32f10x.h"//ͷ�ļ�����


void All_SPI_INI(void);
static void CS_Config(void);			//Ƭѡ����
static void RET_Config(void);		//��λ��������
static void RDY_Config(void);		//RDY��������

static void SPI1_Config(void);
void SPI1_SetSpeed(uint8_t speed);
uint16_t SPI1_WriteReadData(uint16_t dat);

static void SPI2_Config(void);
void SPI2_SetSpeed(uint8_t speed);
uint16_t SPI2_WriteReadData(uint16_t dat);

uint16_t Write_CMD(uint16_t PIN_x,uint16_t Parm);



/* �����ͨ����Ƭѡ���� */
#define CH1_CS GPIO_Pin_0 
#define CH2_CS GPIO_Pin_1 
#define CH3_CS GPIO_Pin_2 
#define CH4_CS GPIO_Pin_3 
#define CH5_CS GPIO_Pin_4 
#define CH6_CS GPIO_Pin_5 
#define CS_ALL (CH1_CS|CH2_CS|CH3_CS|CH4_CS|CH5_CS|CH6_CS)
/* �����ͨ����Ӳ����λ���� */
#define CH1_RET GPIO_Pin_10 
#define CH2_RET GPIO_Pin_11 
#define CH3_RET GPIO_Pin_12 
#define CH4_RET GPIO_Pin_13
#define CH5_RET GPIO_Pin_14
#define CH6_RET GPIO_Pin_15
#define RET_ALL (CH1_RET|CH2_RET|CH3_RET|CH4_RET|CH5_RET|CH6_RET)
/* �����ͨ����RDY���ţ���ΪRDY��ʱδ�ã�ֻ����ȫ������ */
#define RDY_ALL GPIO_Pin_All



















#endif
