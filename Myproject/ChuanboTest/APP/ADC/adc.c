#include "stm32f10x.h" //���ͷ�ļ�����STM32F10x������Χ�Ĵ�����λ���ڴ�ӳ��Ķ���
#include "adc.h"

vu16 AD_Value[N][M]; //�������ADCת�������Ҳ��DMA��Ŀ���ַ
vu16 After_filter[M]; //���������ƽ��ֵ֮��Ľ��




void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//?? GPIOA GPIOB GPIOC ??

	//PB0/1 ��Ϊģ��ͨ����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //ģ����������
	GPIO_Init(GPIOB, &GPIO_InitStructure);

}


void ADC_Multichannel_Config(void)
{
    ADC_InitTypeDef  ADC_InitStructure; 
    ADC_DeInit(ADC1);  //??? ADC1 ????????????
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);//??ADC1?? 
    /* ADC1 configuration ------------------------------------------------------*/ 
    
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//ADC????:ADC1?ADC2???????
    ADC_InitStructure.ADC_ScanConvMode =ENABLE; //ADC?????
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //ADC???????
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //????????
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //ADC????? 
    ADC_InitStructure.ADC_NbrOfChannel = 2; //?????????ADC?????
    ADC_Init(ADC1, &ADC_InitStructure); //??ADC_InitStruct???????????ADCx????


    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_239Cycles5);
		ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 2, ADC_SampleTime_239Cycles5);
    // ??ADC?DMA??(???DMA??,??????DMA?????)     
    ADC_DMACmd(ADC1, ENABLE); 
    /* Enable ADC1 */    
    ADC_Cmd(ADC1, ENABLE);    //?????ADC1
     /* Enable ADC1 reset calibaration register */      
    ADC_ResetCalibration(ADC1);  //?????ADC1?????? 
    
    /* Enable ADC1 reset calibaration register */      
    ADC_ResetCalibration(ADC1);  //?????ADC1?????? 
    /* Start ADC1 calibaration */
    ADC_StartCalibration(ADC1);  //????ADC1????? 
    /* Check the end of ADC1 calibration */
    while(ADC_GetCalibrationStatus(ADC1)); 
    //????ADC1?????,???????
}

void DMA_Configuration(void) 
{
    /* ADC1  DMA1 Channel Config */  
    DMA_InitTypeDef DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //ʹ��DMAʱ��
 
    DMA_DeInit(DMA1_Channel1);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ��ADC1����DMAͨ��1
    DMA_InitStructure.DMA_PeripheralBaseAddr =  (u32)&ADC1->DR;  //DMA����ADC����ַ
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&AD_Value;      //DMA�ڴ����ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //�ڴ���Ϊ���ݴ����Ŀ�ĵ�
    DMA_InitStructure.DMA_BufferSize = N*2;  //��ֵΪ����һ��DMA����Ĵ���
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ����
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ����
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //��������λ���16λ����DMA����ߴ�
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //���ݿ��16λ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  //������ѭ������ģʽ��һ�ֽ������Զ���ʼ���ִ���
    DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMAͨ�� xӵ�и����ȼ�
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��x��ֹ�ڴ浽�ڴ�
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);  //����DMA_InitStruct�в���DMAͨ��
}



//������������
void Init_ADC_Periph(void)
{

	GPIO_Configuration();
	ADC_Multichannel_Config();
	DMA_Configuration();

}



u16 GetVolt(u16 advalue)

{

	return (u16)(advalue * 330 / 4096); //��Ľ��������100���������������С��

}



//vu16 AD_Value[N][M]; 
void filter(void)
{
	int sum = 0;
	int i;
	u8 count;
	for(i=0;i<M;i++)
	{

		for ( count=0;count<N;count++)
		{
		sum += AD_Value[count][i];
		}
		After_filter[i]=sum/N;
		sum=0;
	}

}



/*
int main(void)
{
	u16 value[M];

	Init_ADC_Periph();

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	DMA_Cmd(DMA1_Channel1, ENABLE); //����DMAͨ��
	while(1)
	{
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);//�ȴ�������ɷ����һλ�������׶�ʧ
		filter();
		for(i=0;i<12;i++)
		{
			value[i]= GetVolt(After_filter[i]);
			printf("value[%d]:t%d.%dvn",i,value[i]/100,value[i]/10) ;
			delay_ms(100);
		}
	}

}*/


