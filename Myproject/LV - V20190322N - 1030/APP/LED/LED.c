#include "LED.h"
void GPIO_initial(void)//��ʱ����ʼ��
{
	GPIO_InitTypeDef GPIO_InitStructure; //����һ���ṹ�������������ʼ��GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);

	
	
	
	
	
	
	
	/*  ����GPIO��ģʽ��IO�� */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;  //����ʵ��IO״̬ʶ��
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;	 //
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //���ô�������
	GPIO_Init(GPIOE,&GPIO_InitStructure); 	   /* ��ʼ��GPIO */

		/*  ����GPIO��ģʽ��IO�� */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;  //ѡ����Ҫ���õ�IO��
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //���ô�������
	GPIO_Init(GPIOG,&GPIO_InitStructure); 	   /* ��ʼ��GPIO */
	
	
	GPIO_ResetBits(GPIOG, GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5);
	GPIO_SetBits(GPIOG, GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8);
}



