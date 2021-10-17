#include "SPI.h"
#include "systick.h"


/****************************************************************************
												SPI外设完全配置

****************************************************************************/
void All_SPI_INI(void)
{
		CS_Config();			//片选配置
		RET_Config();		//复位引脚配置
		RDY_Config();		//RDY引脚配置

		SPI1_Config();
		SPI2_Config();
}






/****************************************************************************
* Function Name  : SPI1_Config     
* Description    : 初始化SPI1
* Input          : None
* Output         : None
* Return         : None
关于STM32IO口模式的选择

typedef enum
{ GPIO_Mode_AIN = 0x0,									//模拟输入模式
  GPIO_Mode_IN_FLOATING = 0x04,
  GPIO_Mode_IPD = 0x28,									//下拉输入模式
  GPIO_Mode_IPU = 0x48,									//上拉输入模式
  GPIO_Mode_Out_OD = 0x14,							// 开漏输出模式
  GPIO_Mode_Out_PP = 0x10,						// 推挽输出模式
  GPIO_Mode_AF_OD = 0x1C,
  GPIO_Mode_AF_PP = 0x18
}GPIOMode_TypeDef;

****************************************************************************/

static void SPI1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;

    /* SPI的IO口和SPI外设打开时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    /* SPI的IO口设置 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7); //PA5.6.7上拉#################################
/***************************************************************************/
/************************* 设置SPI的参数 ***********************************/
/***************************************************************************/
/*****根据手册可以知道数字电位器的	CPOL = 0, CPHA = 1。即：
								空闲时，时钟电平为低电平；
								时钟相位表示第二个边沿
								因为此处CPOL为0，空闲时，时钟为低电平，所示，第二个边沿为由高到低的下降沿
	
	*****************************************************************/
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//选择全双工SPI模式
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;     //主机模式
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b; //16位SPI      SPI_DataSize_16b
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;       //时钟悬空高电平  CPOL=0;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;      //在第二个时钟采集数据CPHA=1;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		  //Nss使用软件控制
	/* 选择波特率预分频为256 */
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//从最高位开始传输
//	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_CalculateCRC(SPI1, DISABLE);
	SPI_Cmd(SPI1, ENABLE);
	SPI_Init(SPI1, &SPI_InitStructure);
}      


/****************************************************************************
* Function Name  : SPI1_SetSpeed
* Description    : 设置SPI1的传输速度。
* Input          : 速度波特率分频
* Output         : None
* Return         : None
****************************************************************************/

void SPI1_SetSpeed(uint8_t speed)
{
	SPI1->CR1 &= 0xFFC7; 
	SPI1->CR1 |= speed;
	SPI_Cmd(SPI1, ENABLE);
	
}


/****************************************************************************
* Function Name  : SPI1_WriteReadData
* Description    : 使用SPI1写入一个字节数据同时读取一个字节数据。
* Input          : dat：要写的8位数据
* Output         : None
* Return         : 读取到的8位数据
****************************************************************************/

uint16_t SPI1_WriteReadData(uint16_t dat)
{
	uint16_t i = 0;

    /* 当发送缓冲器空 */	
 	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
	{
		i++;
		if(i > 10000)
		{
			return 0xFFFF;
		}
	}
	
    /* 发送数据 */
   	SPI_I2S_SendData(SPI1, dat);
	
	/* 等待接收缓冲器为非空 */
while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
			{

			}	
	
 	/* 将读取到的数值返回 */
 	return SPI_I2S_ReceiveData(SPI1);		
}







/****************************************************************************
* Function Name  : SPI2_Config     
* Description    : 初始化SPI2
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

static void SPI2_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;

    /* SPI的IO口和SPI外设打开时钟 */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

    /* SPI的IO口设置 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOB, &GPIO_InitStructure);
//	GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7); //PA5.6.7上拉#################################
/***************************************************************************/
/************************* 设置SPI的参数 ***********************************/
/***************************************************************************/
/*****根据手册可以知道数字电位器的	CPOL = 0, CPHA = 1。即：
								空闲时，时钟电平为低电平；
								时钟相位表示第二个边沿
								因为此处CPOL为0，空闲时，时钟为低电平，所示，第二个边沿为由高到低的下降沿
	
	*****************************************************************/
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//选择全双工SPI模式
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;     //主机模式
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b; //16位SPI      SPI_DataSize_16b
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;       //时钟悬空高电平  CPOL=0;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;      //在第二个时钟采集数据CPHA=1;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		  //Nss使用软件控制
	/* 选择波特率预分频为256 */
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//从最高位开始传输
//	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_CalculateCRC(SPI2, DISABLE);
	SPI_Cmd(SPI2, ENABLE);
	SPI_Init(SPI2, &SPI_InitStructure);
}      


/****************************************************************************
* Function Name  : SPI1_SetSpeed
* Description    : 设置SPI1的传输速度。
* Input          : 速度波特率分频
* Output         : None
* Return         : None
****************************************************************************/

void SPI2_SetSpeed(uint8_t speed)
{
	SPI1->CR1 &= 0xFFC7; 
	SPI1->CR1 |= speed;
	SPI_Cmd(SPI1, ENABLE);
	
}


/****************************************************************************
* Function Name  : SPI1_WriteReadData
* Description    : 使用SPI1写入一个字节数据同时读取一个字节数据。
* Input          : dat：要写的8位数据
* Output         : None
* Return         : 读取到的8位数据
****************************************************************************/

uint16_t SPI2_WriteReadData(uint16_t dat)
{
	uint16_t i = 0;

    /* 当发送缓冲器空 */	
 	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET)
	{
		i++;
		if(i > 10000)
		{
			return 0xFFFF;
		}
	}
	
    /* 发送数据 */
   	SPI_I2S_SendData(SPI2, dat);
	
	/* 等待接收缓冲器为非空 */
while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)
			{

			}	
	
 	/* 将读取到的数值返回 */
 	return SPI_I2S_ReceiveData(SPI2);		
}


/****************************************************************************
* Function Name  : CS_Config
* Description    : 片选IO设置和初始化
****************************************************************************/

static void CS_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;               //声明一个结构体变量，用来初始化GPIO
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;        //选择你要设置的IO口
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //设置传输速率
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //设置推挽输出模式
    GPIO_Init(GPIOE, &GPIO_InitStructure);	           //初始化GPIO_LED
		GPIO_SetBits(GPIOE, GPIO_Pin_All);                 //片选全部关闭
}

/****************************************************************************
* Function Name  :RET_Config 
* Description    : 配置硬件复位引脚，
* Input          : stateValue：LED的状态
* Output         : None
* Return         : None
****************************************************************************/

static void RET_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;               //声明一个结构体变量，用来初始化GPIO
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    GPIO_InitStructure.GPIO_Pin = RET_ALL;        //选择你要设置的IO口
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //设置传输速率
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //设置推挽输出模式
    GPIO_Init(GPIOD, &GPIO_InitStructure);	           //初始化结构体
    GPIO_SetBits(GPIOD,RET_ALL);                 //所有外设不复位
}
/****************************************************************************
* Function Name  : RDY_Config
* Description    : 
* Input          :
* Output         : None
* Return         : None
****************************************************************************/

static void RDY_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;               //声明一个结构体变量，用来初始化GPIO
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;        //选择你要设置的IO口
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //设置传输速率
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD ;   //下拉输入
    GPIO_Init(GPIOG, &GPIO_InitStructure);	           //初始化GPIO_LED

}
/****************************************************************************
* Function Name  : Write_CNRG()
* Description    : 
* Input          :
* Output         : None
* Return         : None
****************************************************************************/
uint16_t Write_CMD(uint16_t PIN_x,uint16_t Parm)
{
	uint16_t returnx;
	GPIO_ResetBits(GPIOE,PIN_x);	//拉低片选引脚
	delay_us(10);	
	returnx=SPI1_WriteReadData(Parm);		
	delay_us(10);		
	GPIO_SetBits(GPIOE,PIN_x);		//拉高片选引脚，使1803指令被芯片配置完成
	delay_us(10);
	return returnx;
		
}









