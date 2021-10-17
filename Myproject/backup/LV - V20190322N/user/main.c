/****************************************************************************************************************             
																												_/^\_
																											 <     >
																			*                 /.-.\         *
																							 *        `/&\`                   *
																											 ,@.*;@,
																											/_o.I %_\    *
																				 *           (`'--:o(_@;
																										/`;--.,__ `')             *
																									;@`o % O,*`'`&\ 
																						 *    (`'--)_@ ;o %'()\      *
																									/`;--._`'--._O'@;
																								 /&*,()~o`;-.,_ `""`)
																			*          /`,@ ;+& () o*;-';\
																								(`""--.,_0o*`;-'&()\
																								/-.,_    ``''-...-'`)  *
																					 *   /@%;o`:;'--,._   _.]'\
																							 ;*,&();@%&^;~`"'`o;@();         *
																							/()Emily & ().o@Robin%OCF\
																							`"="==""==,,,.,="=="==="`
																					 __.----.(\-''#####---...___...-----._
																				 '`         \)_`"""""`
																								 .--' ')
																							 o(  )_-\
																								 `"""` ` Version：20190312
************************************************************************************************************/
#include "public.h"
#include "printf.h"
#include "systick.h"
#include "relay.h"
#include "SPI.h"
#include "Fun.h"
#include "LCDdv.h"
#include "time.h"
#include "LED.h"
//#include "stdbool.h"						//对于布尔型的变量的定义在该头文件下边，注释掉会出错 

extern __IO FlagStatus IP_ask_flag;			//声明已发送获取IP指令的标志位

uint8_t NOe[]={0x4E,0x4f,0x52,0x4D,0x41,0x4C,0x0d};//"NORMAL"
extern __IO u8		RxCounter1;
extern __IO u8		RxCounter2;
extern __IO u8 RxBuffer1[29];
extern __IO u8 RxBuffer2[10];
extern __IO FlagStatus FLAG_Rx;
extern __IO FlagStatus RECEIVE_flag2;

extern void My_USART1_IRQHandler(void);
				uint8_t Com[]={0xaa,0x00};
				uint8_t Res[]={0xaa,0x01};
				uint16_t SPIreceive;
extern void My_USART2_IRQHandler(void);
u8 ComandU1[]={0xFF ,0x11 ,0x22 ,0x33 ,0x44 ,0x55 ,0x66 ,0x77 ,0x88 ,0x99 
									,0xAA ,0xBB ,0xcc ,0xdd ,0xee ,0xe1 ,0x11 ,0x12 ,0x13 ,0x14 
										,0x15 ,0x16 ,0x22 ,0x21 ,0x23 ,0x24 ,0x25 ,0x26 ,0xAB};
 u8 temp;
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
* Function Name  : main
* Description    : Main program.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
int main()				//已经改成了两种数据包解析函数
{	
	Base_INI();				//外设初始化	
	PWR_UP();
/****************IP获取**********************************/	
	delay_s(1);
	Usart_mode_ON();
	delay_ms(1);
	Usart_mode_ON();
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE );		
	while(1)
	{
			delay_ms(1000);
		printf("Hello world\n");
	}
}


