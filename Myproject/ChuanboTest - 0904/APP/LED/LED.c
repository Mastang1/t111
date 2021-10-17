#include "LED.h"
void GPIO_initial(void)//定时器初始化
{
	GPIO_InitTypeDef GPIO_InitStructure; //声明一个结构体变量，用来初始化GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);

	
	
	
	
	
	
	
	/*  配置GPIO的模式和IO口 */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;  //用作实现IO状态识别
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;	 //
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //设置传输速率
	GPIO_Init(GPIOE,&GPIO_InitStructure); 	   /* 初始化GPIO */

		/*  配置GPIO的模式和IO口 */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;  //选择你要设置的IO口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //设置传输速率
	GPIO_Init(GPIOG,&GPIO_InitStructure); 	   /* 初始化GPIO */
	
	
	GPIO_ResetBits(GPIOG, GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5);
	GPIO_SetBits(GPIOG, GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8);
}



