#include "stm32f10x.h" //这个头文件包括STM32F10x所有外围寄存器、位、内存映射的定义
#include "adc.h"

vu16 AD_Value[N][M]; //用来存放ADC转换结果，也是DMA的目标地址
vu16 After_filter[M]; //用来存放求平均值之后的结果




void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//?? GPIOA GPIOB GPIOC ??

	//PB0/1 作为模拟通道输入引脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //模拟输入引脚
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
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //使能DMA时钟
 
    DMA_DeInit(DMA1_Channel1);   //将DMA的通道1寄存器重设为缺省值：ADC1连接DMA通道1
    DMA_InitStructure.DMA_PeripheralBaseAddr =  (u32)&ADC1->DR;  //DMA外设ADC基地址
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&AD_Value;      //DMA内存基地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //内存作为数据传输的目的地
    DMA_InitStructure.DMA_BufferSize = N*2;  //此值为完整一轮DMA传输的次数
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址不变
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址递增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //外设数据位宽度16位，即DMA传输尺寸
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //数据宽度16位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  //工作在循环缓存模式，一轮结束后自动开始下轮传输
    DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMA通道 x拥有高优先级
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x禁止内存到内存
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);  //根据DMA_InitStruct中参数DMA通道
}



//配置所有外设
void Init_ADC_Periph(void)
{

	GPIO_Configuration();
	ADC_Multichannel_Config();
	DMA_Configuration();

}



u16 GetVolt(u16 advalue)

{

	return (u16)(advalue * 330 / 4096); //求的结果扩大了100倍，方便下面求出小数

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
	DMA_Cmd(DMA1_Channel1, ENABLE); //启动DMA通道
	while(1)
	{
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);//等待传输完成否则第一位数据容易丢失
		filter();
		for(i=0;i<12;i++)
		{
			value[i]= GetVolt(After_filter[i]);
			printf("value[%d]:t%d.%dvn",i,value[i]/100,value[i]/10) ;
			delay_ms(100);
		}
	}

}*/


