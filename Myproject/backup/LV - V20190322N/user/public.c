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
	 //ϵͳ�����������ڳ�ʼ��
	SystemInit();	//ʱ�ӳ�ʼ��
	USART1_INI(115200);  	
	USART2_INI(115200);
	USART3_INI(9600);					//���ݴ�������Ҫ������Ϊ9600
	All_SPI_INI();
	Relay_INI();
	time_init();
	LED_init();
}
/**********************ϵͳ�ӵ��Ĭ������***************************/
void PWR_UP(void)
{	
	delay_s(3);							//����ϵͳ������ʱΪ3S���Ա�֤����ģ���������
	Sys_reset();
}

