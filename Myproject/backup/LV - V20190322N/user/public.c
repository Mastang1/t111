#include "public.h"
#include "printf.h"
#include "systick.h"
#include "relay.h"
#include "SPI.h"
#include "Fun.h"
#include "time.h"
#include "LED.h"
void Base_INI(void)
{
	 //系统所用三个串口初始化
	SystemInit();	//时钟初始化
	USART1_INI(115200);  	
	USART2_INI(115200);
	USART3_INI(9600);					//根据串口屏的要求配置为9600
	All_SPI_INI();
	Relay_INI();
	time_init();
	LED_init();
}
/**********************系统加电后默认配置***************************/
void PWR_UP(void)
{	
	delay_s(3);							//配置系统启动延时为3S，以保证网卡模块启动完成
	Sys_reset();
}

