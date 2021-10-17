#include "SPI.h"
#include "systick.h"


/****************************************************************************
												SPI������ȫ����

****************************************************************************/
void All_SPI_INI(void)
{
		CS_Config();			//Ƭѡ����
		RET_Config();		//��λ��������
		RDY_Config();		//RDY��������

		SPI1_Config();
		SPI2_Config();
}






/****************************************************************************
* Function Name  : SPI1_Config     
* Description    : ��ʼ��SPI1
* Input          : None
* Output         : None
* Return         : None
����STM32IO��ģʽ��ѡ��

typedef enum
{ GPIO_Mode_AIN = 0x0,									//ģ������ģʽ
  GPIO_Mode_IN_FLOATING = 0x04,
  GPIO_Mode_IPD = 0x28,									//��������ģʽ
  GPIO_Mode_IPU = 0x48,									//��������ģʽ
  GPIO_Mode_Out_OD = 0x14,							// ��©���ģʽ
  GPIO_Mode_Out_PP = 0x10,						// �������ģʽ
  GPIO_Mode_AF_OD = 0x1C,
  GPIO_Mode_AF_PP = 0x18
}GPIOMode_TypeDef;

****************************************************************************/

static void SPI1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;

    /* SPI��IO�ں�SPI�����ʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    /* SPI��IO������ */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7); //PA5.6.7����#################################
/***************************************************************************/
/************************* ����SPI�Ĳ��� ***********************************/
/***************************************************************************/
/*****�����ֲ����֪�����ֵ�λ����	CPOL = 0, CPHA = 1������
								����ʱ��ʱ�ӵ�ƽΪ�͵�ƽ��
								ʱ����λ��ʾ�ڶ�������
								��Ϊ�˴�CPOLΪ0������ʱ��ʱ��Ϊ�͵�ƽ����ʾ���ڶ�������Ϊ�ɸߵ��͵��½���
	
	*****************************************************************/
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//ѡ��ȫ˫��SPIģʽ
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;     //����ģʽ
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b; //16λSPI      SPI_DataSize_16b
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;       //ʱ�����ոߵ�ƽ  CPOL=0;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;      //�ڵڶ���ʱ�Ӳɼ�����CPHA=1;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		  //Nssʹ���������
	/* ѡ������Ԥ��ƵΪ256 */
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//�����λ��ʼ����
//	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_CalculateCRC(SPI1, DISABLE);
	SPI_Cmd(SPI1, ENABLE);
	SPI_Init(SPI1, &SPI_InitStructure);
}      


/****************************************************************************
* Function Name  : SPI1_SetSpeed
* Description    : ����SPI1�Ĵ����ٶȡ�
* Input          : �ٶȲ����ʷ�Ƶ
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
* Description    : ʹ��SPI1д��һ���ֽ�����ͬʱ��ȡһ���ֽ����ݡ�
* Input          : dat��Ҫд��8λ����
* Output         : None
* Return         : ��ȡ����8λ����
****************************************************************************/

uint16_t SPI1_WriteReadData(uint16_t dat)
{
	uint16_t i = 0;

    /* �����ͻ������� */	
 	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
	{
		i++;
		if(i > 10000)
		{
			return 0xFFFF;
		}
	}
	
    /* �������� */
   	SPI_I2S_SendData(SPI1, dat);
	
	/* �ȴ����ջ�����Ϊ�ǿ� */
while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
			{

			}	
	
 	/* ����ȡ������ֵ���� */
 	return SPI_I2S_ReceiveData(SPI1);		
}







/****************************************************************************
* Function Name  : SPI2_Config     
* Description    : ��ʼ��SPI2
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

static void SPI2_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;

    /* SPI��IO�ں�SPI�����ʱ�� */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

    /* SPI��IO������ */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOB, &GPIO_InitStructure);
//	GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7); //PA5.6.7����#################################
/***************************************************************************/
/************************* ����SPI�Ĳ��� ***********************************/
/***************************************************************************/
/*****�����ֲ����֪�����ֵ�λ����	CPOL = 0, CPHA = 1������
								����ʱ��ʱ�ӵ�ƽΪ�͵�ƽ��
								ʱ����λ��ʾ�ڶ�������
								��Ϊ�˴�CPOLΪ0������ʱ��ʱ��Ϊ�͵�ƽ����ʾ���ڶ�������Ϊ�ɸߵ��͵��½���
	
	*****************************************************************/
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//ѡ��ȫ˫��SPIģʽ
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;     //����ģʽ
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b; //16λSPI      SPI_DataSize_16b
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;       //ʱ�����ոߵ�ƽ  CPOL=0;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;      //�ڵڶ���ʱ�Ӳɼ�����CPHA=1;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		  //Nssʹ���������
	/* ѡ������Ԥ��ƵΪ256 */
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//�����λ��ʼ����
//	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_CalculateCRC(SPI2, DISABLE);
	SPI_Cmd(SPI2, ENABLE);
	SPI_Init(SPI2, &SPI_InitStructure);
}      


/****************************************************************************
* Function Name  : SPI1_SetSpeed
* Description    : ����SPI1�Ĵ����ٶȡ�
* Input          : �ٶȲ����ʷ�Ƶ
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
* Description    : ʹ��SPI1д��һ���ֽ�����ͬʱ��ȡһ���ֽ����ݡ�
* Input          : dat��Ҫд��8λ����
* Output         : None
* Return         : ��ȡ����8λ����
****************************************************************************/

uint16_t SPI2_WriteReadData(uint16_t dat)
{
	uint16_t i = 0;

    /* �����ͻ������� */	
 	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET)
	{
		i++;
		if(i > 10000)
		{
			return 0xFFFF;
		}
	}
	
    /* �������� */
   	SPI_I2S_SendData(SPI2, dat);
	
	/* �ȴ����ջ�����Ϊ�ǿ� */
while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)
			{

			}	
	
 	/* ����ȡ������ֵ���� */
 	return SPI_I2S_ReceiveData(SPI2);		
}


/****************************************************************************
* Function Name  : CS_Config
* Description    : ƬѡIO���úͳ�ʼ��
****************************************************************************/

static void CS_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;               //����һ���ṹ�������������ʼ��GPIO
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;        //ѡ����Ҫ���õ�IO��
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //���ô�������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //�����������ģʽ
    GPIO_Init(GPIOE, &GPIO_InitStructure);	           //��ʼ��GPIO_LED
		GPIO_SetBits(GPIOE, GPIO_Pin_All);                 //Ƭѡȫ���ر�
}

/****************************************************************************
* Function Name  :RET_Config 
* Description    : ����Ӳ����λ���ţ�
* Input          : stateValue��LED��״̬
* Output         : None
* Return         : None
****************************************************************************/

static void RET_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;               //����һ���ṹ�������������ʼ��GPIO
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    GPIO_InitStructure.GPIO_Pin = RET_ALL;        //ѡ����Ҫ���õ�IO��
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //���ô�������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //�����������ģʽ
    GPIO_Init(GPIOD, &GPIO_InitStructure);	           //��ʼ���ṹ��
    GPIO_SetBits(GPIOD,RET_ALL);                 //�������費��λ
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
    GPIO_InitTypeDef GPIO_InitStructure;               //����һ���ṹ�������������ʼ��GPIO
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;        //ѡ����Ҫ���õ�IO��
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //���ô�������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD ;   //��������
    GPIO_Init(GPIOG, &GPIO_InitStructure);	           //��ʼ��GPIO_LED

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
	GPIO_ResetBits(GPIOE,PIN_x);	//����Ƭѡ����
	delay_us(10);	
	returnx=SPI1_WriteReadData(Parm);		
	delay_us(10);		
	GPIO_SetBits(GPIOE,PIN_x);		//����Ƭѡ���ţ�ʹ1803ָ�оƬ�������
	delay_us(10);
	return returnx;
		
}









