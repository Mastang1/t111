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
#include "time.h"
#include "LED.h"
#include "adc.h"
extern __IO u8		RxCounter1;
extern __IO u8		RxCounter2;
extern __IO u8 RxBuffer1[29];
extern __IO u8 RxBuffer2[10];
vu16 CH1Adc[N];
vu16 CH2Adc[N];

// ADC1转换的电压值通过MDA方式传到SRAM
extern __IO uint16_t ADC_ConvertedValue;

// 局部变量，用于保存转换计算后的电压值			 

float ADC_ConvertedValueLocal;        


u8 gl_IOstate1 = 0;
u8 gl_IOstate2 = 0;
u8 MyHex[18];
char MyTxt[100] = "hello";
char ArrString[40] = "";
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
* Function Name  : main
* Description    : Main program.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
int main()				
{	
	
	int i = 0;
	vu16 CH1MaxVal = 0;
	vu16 CH2MaxVal = 0;
	float f_MaxVal =.0;
	Base_INI();				//外设初始化	
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	DMA_Cmd(DMA1_Channel1, ENABLE); //启动DMA通道
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE );
	
	delay_ms(2000);
	BeepLoud(3);

	while(1)
	{

		//delay
		delay_ms(10);   
		
		if(flag_RSuc == 1)										//Rs422数据判断*********1
		{
			if(test_YN)
			{
				printf("t0.txt=\"%s\"\xff\xff\xff","TRUE FRAME");
				printf("t4.txt=\"%s\"\xff\xff\xff","");
				printf("t5.txt=\"%s\"\xff\xff\xff","");
				sprintf(ArrString, "%3x%3x%3x%3x%3x%3x%3x%3x", RxBuffer2[0], RxBuffer2[1], RxBuffer2[2], RxBuffer2[3], RxBuffer2[4], RxBuffer2[5], RxBuffer2[6], RxBuffer2[7]);
				printf("t1.txt=\"%s\"\xff\xff\xff",ArrString);
			}else
			{
				printf("t4.txt=\"%s\"\xff\xff\xff","ERROR FRAME");
				printf("t0.txt=\"%s\"\xff\xff\xff","");
				printf("t1.txt=\"%s\"\xff\xff\xff","");
				sprintf(ArrString, "%3x%3x%3x%3x%3x%3x%3x%3x", RxBuffer2N[0], RxBuffer2N[1], RxBuffer2N[2], RxBuffer2N[3], RxBuffer2N[4], RxBuffer2N[5], RxBuffer2N[6], RxBuffer2N[7]);
				printf("t5.txt=\"%s\"\xff\xff\xff",ArrString);
				BeepLoud(3)	;		//DiDiDi
			}

			flag_RSuc = 0;
		}
///////////////////													实现波特率的更改****2
		if(Banttesla != 0)
		{
			USART_InitTypeDef USART_InitStructure;
			USART_Cmd(USART2, DISABLE);
			USART_InitStructure.USART_BaudRate =Banttesla;
			USART_InitStructure.USART_WordLength = USART_WordLength_8b;
			USART_InitStructure.USART_StopBits = USART_StopBits_1;
			USART_InitStructure.USART_Parity = USART_Parity_No;
			USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
			USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
			USART_Init(USART2, &USART_InitStructure);
			USART_Cmd(USART2, ENABLE);
			Banttesla = 0;
			//添加执行完成的声音
			BeepLoud(2);//DiDiDi			//DiDiDi
		}
////////////////////数字IO检测CH1************////////////////*********************3
		if(1 == GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_11) ) 			//输入为高电平
		{
			printf("p0.pic=4\xff\xff\xff");
			LEDLight(0, BLUE);
		}else																											//输入为低电平
		{
			printf("p0.pic=5\xff\xff\xff");
			LEDLight(0, RED);
			if(IOBeerState1 == 1) {BeepLoud(1);} 
		}
		////////////////////数字IO检测CH2************////////////////////////////////////////	
///////////////////////////////////////////////////////////////////////////////////		
		if(1 == GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_12) ) 
		{
			printf("p1.pic=4\xff\xff\xff");
			LEDLight(1, BLUE);
		}else
		{
			printf("p1.pic=5\xff\xff\xff");
			LEDLight(1, RED);
			if(IOBeerState2 == 1) {BeepLoud(1);} 
		}
		////////////////////数字IO检测CH3************////////////////////////////////////////	
///////////////////////////////////////////////////////////////////////////////////		
		if(1 == GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_13) ) 
		{
			printf("p2.pic=4\xff\xff\xff");
			LEDLight(2, BLUE);
		}else
		{
			printf("p2.pic=5\xff\xff\xff");
			LEDLight(2, RED);
			if(IOBeerState3 == 1) {BeepLoud(1);} 
		}
		////////////////////数字IO检测CH4************////////////////////////////////////////	
///////////////////////////////////////////////////////////////////////////////////		
		if(1 == GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_14) ) 
		{
			printf("p3.pic=4\xff\xff\xff");
			LEDLight(3, BLUE);
		}else
		{
			printf("p3.pic=5\xff\xff\xff");
			LEDLight(3, RED);
			if(IOBeerState4 == 1) {BeepLoud(1);} 
		}
///////////////////////////////////////////////////////////////////////////////////		
	//	ADC_ConvertedValueLocal =(float) ADC_ConvertedValue/4096*3.3; // 读取转换的AD值********4
///////////////////////////////////////////////////////////////////////////////
		
		for(i = 0; i < 100; i++)
		{
			if(AD_Value[i][0] < CH1MaxVal)
			{
				CH1MaxVal = CH1MaxVal;
			}
			else
			{
				CH1MaxVal = AD_Value[i][0];
			}
		}
		
		f_MaxVal = CH1MaxVal*3.3/4096*3;
	//	printf("tv1.txt=\"%.3fV\"\xff\xff\xff",f_MaxVal);
		if( CH1MaxVal > 2068)						//大于给定值***************报错***************5
		{
			
			printf("p4.pic=5\xff\xff\xff");
			LEDLight(4, RED);
			BeepLoud(1);
		}
		else
		{
			printf("p4.pic=4\xff\xff\xff");
			LEDLight(4, BLUE);
		}
		
///////////////////////////////////////////////////////////////////////////////
		for(i = 0; i < 100; i++)
		{
			if(AD_Value[i][1] < CH1MaxVal)
			{
				CH1MaxVal = CH1MaxVal;
			}
			else
			{
				CH1MaxVal = AD_Value[i][1];
			}
		}
		
		f_MaxVal = CH1MaxVal*3.3/4095*3;
		printf("tv2.txt=\"%.3fV\"\xff\xff\xff",f_MaxVal);
		if( CH1MaxVal > 2069)
		{
			
			printf("p5.pic=5\xff\xff\xff");
			LEDLight(5, RED);
			BeepLoud(1);
		}
		else
		{
			printf("p5.pic=4\xff\xff\xff");
			LEDLight(5, BLUE);
		}
		CH1MaxVal = 0;
		f_MaxVal = 0;
///////////////////////////////////////////////////////////////////////////////


		
	}
	
}







/*


//      printf("n0.val=666"); 
//      printf("\xff\xff\xff");
    temp = 0xff;
		printf("t0.txt=\"%x\"\xff\xff\xff",temp);
		
		printf("t3.txt=\"helloo\"\xff\xff\xff");
		
		printf("t4.txt=\"%s\"\xff\xff\xff",MyTxt);
		delay_ms(1000);
		delay_ms(1000);	
*/
