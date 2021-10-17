#include "adc.h"



u16 AD_Value[10][6]; //�������ADCת�������Ҳ��DMA��Ŀ���ַ
u16 After_filter[6]; //���������ƽ��ֵ֮��Ľ��
int i;


static void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE); /* ����GPIOʱ�� */
	//PC0/1/2/3/4/5 ��Ϊģ��ͨ����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //ģ����������
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}



static void RCC_Configuration(void)
{
	ErrorStatus HSEStartUpStatus;

	RCC_DeInit(); //RCC ϵͳ��λ
	RCC_HSEConfig(RCC_HSE_ON); //����HSE
	HSEStartUpStatus = RCC_WaitForHSEStartUp(); //�ȴ�HSE׼����
	if(HSEStartUpStatus == SUCCESS)
	{
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); //Enable Prefetch Buffer
		FLASH_SetLatency(FLASH_Latency_2); //Set 2 Latency cycles
		RCC_HCLKConfig(RCC_SYSCLK_Div1); //AHB clock = SYSCLK
		RCC_PCLK2Config(RCC_HCLK_Div1); //APB2 clock = HCLK
		RCC_PCLK1Config(RCC_HCLK_Div2); //APB1 clock = HCLK/2
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_6); //PLLCLK = 12MHz * 6 = 72 MHz
		RCC_PLLCmd(ENABLE); //Enable PLL
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET); //Wait till PLL is ready
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); //Select PLL as system clock source
		while(RCC_GetSYSCLKSource() != 0x08); //Wait till PLL is used as system clock source

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB
		| RCC_APB2Periph_GPIOC |RCC_APB2Periph_ADC1 | RCC_APB2Periph_AFIO |RCC_APB2Periph_USART1, ENABLE ); //ʹ��ADC1ͨ��ʱ�ӣ������ܽ�ʱ��

		RCC_ADCCLKConfig(RCC_PCLK2_Div6); //72M/6=12,ADC���ʱ�䲻�ܳ���14M
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //ʹ��DMA����
	}
}


static void ADC1_Configuration(void)
{
		ADC_InitTypeDef ADC_InitStructure;

		ADC_DeInit(ADC1); //������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ


		ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
		ADC_InitStructure.ADC_ScanConvMode =ENABLE; //ģ��ת��������ɨ��ģʽ
		ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //ģ��ת������������ת��ģʽ
		ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //�ⲿ����ת���ر�
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //ADC�����Ҷ���
		ADC_InitStructure.ADC_NbrOfChannel = 6; //˳����й���ת����ADCͨ������Ŀ
		ADC_Init(ADC1, &ADC_InitStructure); //����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���


		//����ָ��ADC�Ĺ�����ͨ�����������ǵ�ת��˳��Ͳ���ʱ��
		//ADC1,ADCͨ��x,�������˳��ֵΪy,����ʱ��Ϊ239.5����
	
		ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_239Cycles5 );
		ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 2, ADC_SampleTime_239Cycles5 );
		ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 3, ADC_SampleTime_239Cycles5 );
		ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 4, ADC_SampleTime_239Cycles5 );
		ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 5, ADC_SampleTime_239Cycles5 );
		ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 6, ADC_SampleTime_239Cycles5 );

		// ����ADC��DMA֧�֣�Ҫʵ��DMA���ܣ������������DMAͨ���Ȳ�����
		ADC_DMACmd(ADC1, ENABLE);


		ADC_Cmd(ADC1, ENABLE); //ʹ��ָ����ADC1

		ADC_ResetCalibration(ADC1); //��λָ����ADC1��У׼�Ĵ���

		while(ADC_GetResetCalibrationStatus(ADC1)); //��ȡADC1��λУ׼�Ĵ�����״̬,����״̬��ȴ�


		ADC_StartCalibration(ADC1); //��ʼָ��ADC1��У׼״̬

		while(ADC_GetCalibrationStatus(ADC1)); //��ȡָ��ADC1��У׼����,����״̬��ȴ�


}


static void DMA_Configuration(void)
{

		DMA_InitTypeDef DMA_InitStructure;
		DMA_DeInit(DMA1_Channel1); //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
		DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR; //DMA����ADC����ַ
		DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&AD_Value; //DMA�ڴ����ַ
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //�ڴ���Ϊ���ݴ����Ŀ�ĵ�
		DMA_InitStructure.DMA_BufferSize = 60; //DMAͨ����DMA����Ĵ�С
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //�����ַ�Ĵ�������
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //�ڴ��ַ�Ĵ�������
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //���ݿ��Ϊ16λ
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //���ݿ��Ϊ16λ
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //������ѭ������ģʽ
		DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMAͨ�� xӵ�и����ȼ�
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
		DMA_Init(DMA1_Channel1, &DMA_InitStructure); //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��

}


//������������
void Init_All_Periph(void)
{

	RCC_Configuration();

	GPIO_Configuration();

	ADC1_Configuration();

	DMA_Configuration();

	//USART1_Configuration();

}



u16 GetVolt(u16 advalue)
{
	return (u16)(advalue * 249 / 4096); //��Ľ��������100���������������С��
}



void filter(void)
{
	int sum = 0;
	u8 count;		
	for(i=0;i<12;i++)
		{
				for ( count=0;count<10;count++)
				{
					sum += AD_Value[count][i];
				}
				After_filter[i]=sum/10;
				sum=0;
		}
}





