#include "LED.h"
#include "public.h"
void GPIO_initial(void)//定时器初始化
{
	GPIO_InitTypeDef GPIO_InitStructure; //声明一个结构体变量，用来初始化GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	
	
	
	
	
	
	
	/*  配置GPIO的模式和IO口 */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;  //用作实现IO状态识别
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;	 //
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //设置传输速率
	GPIO_Init(GPIOE,&GPIO_InitStructure); 	   /* 初始化GPIO */

		/*  配置GPIO的模式和IO口 */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_All;  //用作IO状态led驱动IO
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //设置传输速率
	GPIO_Init(GPIOD,&GPIO_InitStructure); 	   /* 初始化GPIO */
	GPIO_ResetBits(GPIOD, GPIO_Pin_BLUE_All);
	GPIO_SetBits(GPIOD, GPIO_Pin_RED_All);
	
	//PB5 config
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;  //用作IO状态led驱动IO
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //设置传输速率
	GPIO_Init(GPIOB,&GPIO_InitStructure); 	   /* 初始化GPIO */

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

