#include "LED.h"
#include "public.h"
void GPIO_initial(void)//��ʱ����ʼ��
{
	GPIO_InitTypeDef GPIO_InitStructure; //����һ���ṹ�������������ʼ��GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	
	
	
	
	
	
	
	/*  ����GPIO��ģʽ��IO�� */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;  //����ʵ��IO״̬ʶ��
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;	 //
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //���ô�������
	GPIO_Init(GPIOE,&GPIO_InitStructure); 	   /* ��ʼ��GPIO */

		/*  ����GPIO��ģʽ��IO�� */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_All;  //����IO״̬led����IO
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //���ô�������
	GPIO_Init(GPIOD,&GPIO_InitStructure); 	   /* ��ʼ��GPIO */
	GPIO_ResetBits(GPIOD, GPIO_Pin_BLUE_All);
	GPIO_SetBits(GPIOD, GPIO_Pin_RED_All);
	
	//PB5 config
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;  //����IO״̬led����IO
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //���ô�������
	GPIO_Init(GPIOB,&GPIO_InitStructure); 	   /* ��ʼ��GPIO */

	GPIO_ResetBits(GPIOB, GPIO_Pin_5);
}




void LEDLight(int CHx , int color)
{
	int blue_bit = 1 << CHx;
	int red_bit = 0x0040 << CHx;
	
	if(color == BLUE)
	{
		GPIO_ResetBits(GPIOD, blue_bit);
		GPIO_SetBits(GPIOD, red_bit);
	}
	else if(color == RED)
	{
		GPIO_ResetBits(GPIOD, red_bit);
		GPIO_SetBits(GPIOD, blue_bit);		
	}
	else
	{
		GPIO_ResetBits(GPIOD, red_bit);
		GPIO_ResetBits(GPIOD, blue_bit);			
	}
}

