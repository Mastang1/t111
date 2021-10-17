#include "adc.h"



u16 AD_Value[10][6]; //用来存放ADC转换结果，也是DMA的目标地址
u16 After_filter[6]; //用来存放求平均值之后的结果
int i;


static void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE); /* 开启GPIO时钟 */
	//PC0/1/2/3/4/5 作为模拟通道输入引脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //模拟输入引脚
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}



static void RCC_Configuration(void)
{
	ErrorStatus HSEStartUpStatus;

	RCC_DeInit(); //RCC 系统复位
	RCC_HSEConfig(RCC_HSE_ON); //开启HSE
	HSEStartUpStatus = RCC_WaitForHSEStartUp(); //等待HSE准备好
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
		| RCC_APB2Periph_GPIOC |RCC_APB2Periph_ADC1 | RCC_APB2Periph_AFIO |RCC_APB2Periph_USART1, ENABLE ); //使能ADC1通道时钟，各个管脚时钟

		RCC_ADCCLKConfig(RCC_PCLK2_Div6); //72M/6=12,ADC最大时间不能超过14M
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //使能DMA传输
	}
}


static void ADC1_Configuration(void)
{
		ADC_InitTypeDef ADC_InitStructure;

		ADC_DeInit(ADC1); //将外设 ADC1 的全部寄存器重设为缺省值


		ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //ADC工作模式:ADC1和ADC2工作在独立模式
		ADC_InitStructure.ADC_ScanConvMode =ENABLE; //模数转换工作在扫描模式
		ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //模数转换工作在连续转换模式
		ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //外部触发转换关闭
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //ADC数据右对齐
		ADC_InitStructure.ADC_NbrOfChannel = 6; //顺序进行规则转换的ADC通道的数目
		ADC_Init(ADC1, &ADC_InitStructure); //根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器


		//设置指定ADC的规则组通道，设置它们的转化顺序和采样时间
		//ADC1,ADC通道x,规则采样顺序值为y,采样时间为239.5周期
	
		ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_239Cycles5 );
		ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 2, ADC_SampleTime_239Cycles5 );
		ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 3, ADC_SampleTime_239Cycles5 );
		ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 4, ADC_SampleTime_239Cycles5 );
		ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 5, ADC_SampleTime_239Cycles5 );
		ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 6, ADC_SampleTime_239Cycles5 );

		// 开启ADC的DMA支持（要实现DMA功能，还需独立配置DMA通道等参数）
		ADC_DMACmd(ADC1, ENABLE);


		ADC_Cmd(ADC1, ENABLE); //使能指定的ADC1

		ADC_ResetCalibration(ADC1); //复位指定的ADC1的校准寄存器

		while(ADC_GetResetCalibrationStatus(ADC1)); //获取ADC1复位校准寄存器的状态,设置状态则等待


		ADC_StartCalibration(ADC1); //开始指定ADC1的校准状态

		while(ADC_GetCalibrationStatus(ADC1)); //获取指定ADC1的校准程序,设置状态则等待


}


static void DMA_Configuration(void)
{

		DMA_InitTypeDef DMA_InitStructure;
		DMA_DeInit(DMA1_Channel1); //将DMA的通道1寄存器重设为缺省值
		DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR; //DMA外设ADC基地址
		DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&AD_Value; //DMA内存基地址
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //内存作为数据传输的目的地
		DMA_InitStructure.DMA_BufferSize = 60; //DMA通道的DMA缓存的大小
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //外设地址寄存器不变
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //内存地址寄存器递增
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //数据宽度为16位
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //数据宽度为16位
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //工作在循环缓存模式
		DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMA通道 x拥有高优先级
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //DMA通道x没有设置为内存到内存传输
		DMA_Init(DMA1_Channel1, &DMA_InitStructure); //根据DMA_InitStruct中指定的参数初始化DMA的通道

}


//配置所有外设
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
	return (u16)(advalue * 249 / 4096); //求的结果扩大了100倍，方便下面求出小数
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





