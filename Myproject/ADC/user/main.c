/*******************************************************************************
                                   ADC 
*******************************************************************************/
#include "public.h"
#include "printf.h"
#include "systick.h"
#include "ADCscan.h"

extern u16 AD_Value[M][N]; //�������ADCת�������Ҳ��DMA��Ŀ���ַ
extern u16 After_filter[N]; //���������ƽ��ֵ֮��Ľ��
/****************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Output         : None
* Return         : None  delay_ms(u32 i)
****************************************************************************/


int main(void)
{

	u16 value[N];
	u8 i;
	
	
	SystemInit();
	Init_ADC1_Periph();
	printf_init();

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	DMA_Cmd(DMA1_Channel1, ENABLE); //����DMAͨ��
	while(1)
	{
//		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);//�ȴ�������ɷ����һλ�������׶�ʧ

		filter();
//		for(i=0;i<N;i++)
//		{
//			value[i]= GetVolt(After_filter[i]);

//			printf("value[%d]:\t%d.%dv\n",i,value[i],value[i]) ;
//			delay_ms(100);
//		}
		
		
		
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);//�ȴ�������ɷ����һλ�������׶�ʧ

		for(i=0;i<N;i++)
		{
			printf("value[%d]:%d\n",i,After_filter[i]) ;
			delay_ms(1000);
		}
		
	}

}


