/*******************************************************************************             
*                ʵ�֣�
								1. �̵����ĳ�ʼ����
								2. �̵����Ĳ���������
								3. ģ�⿪�صĳ�ʼ���Ͳ���������
								4. LED�ĳ�ʼ���Ϳ��ؿ��ƺ���
*******************************************************************************/
#include "relay.h"
#include "systick.h"
/*==============================================================================
*function:void relay_ini(void);
*description: �̵�����ʼ��Ϊ�������ģʽ������16ͨ��IO����
*==============================================================================*/
void Relay_INI(void)
{
	Relay_Pin_Ini();
	Lat_Clk_Ini();
}



static void Relay_Pin_Ini(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //����һ���ṹ�������������ʼ��GPIO

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);

	/*  ����GPIO��ģʽ��IO�� */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_All;  //ѡ����Ҫ���õ�IO��
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //�����������ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //���ô�������
	GPIO_Init(GPIOF,&GPIO_InitStructure); 	   /* ��ʼ��GPIO */
	GPIOF->BRR = 0xffff; //ȫ������
}



/*==============================================================================
*function:void relay_handle(void);
*description: ������ʱ�ӿ������ų�ʼ��,����1
*==============================================================================*/

static void Lat_Clk_Ini(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //����һ���ṹ�������������ʼ��GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

	/*  ����GPIO��ģʽ��IO�� */
	GPIO_InitStructure.GPIO_Pin=LACK_CLK;  //ѡ����Ҫ���õ�IO��
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //�����������ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //���ô�������
	GPIO_Init(GPIOC,&GPIO_InitStructure); 	   /* ��ʼ��GPIO */
	GPIO_ResetBits(GPIOC, LACK_CLK);
}


/*==============================================================================
*function:void relay_handle(void); 	GPIOF->BSRR = (uint32_t)0xfc00; 
*description: ����һ������
*==============================================================================*/

static void P_edge(void)
{
//	delay_ms(10);
//	GPIO_SetBits(GPIOC, LACK_CLK);
//	//GPIOF->BSRR = GPIO_Pin_9; //ȫ����1
//	delay_ms(10);
//	GPIO_ResetBits(GPIOC, LACK_CLK);
////	GPIOF->BRR = GPIO_Pin_9; //ȫ������
//	delay_ms(10);
	//	delay_ms(1);
	GPIO_SetBits(GPIOF, GPIO_Pin_9);
	//GPIOF->BSRR = GPIO_Pin_9; //ȫ����1
	delay_us(10);
	GPIO_ResetBits(GPIOF, GPIO_Pin_9);
//	GPIOF->BRR = GPIO_Pin_9; //ȫ������
	delay_us(10);
}

/*==============================================================================
*function:void Relay_ON (u16 relay_x)	GPIOF->BSRR = (uint32_t)0xfc00; 
*description: ���̵���
*==============================================================================*/

void Relay_ON (u16 RELA_x)
{
	GPIOF->BSRR = RELA_x; 
	P_edge();
}

/*==============================================================================
*function:void Relay_ON (u16 relay_x);	GPIOF->BSRR = (uint32_t)0xfc00; 
*description: �ؼ̵���
*==============================================================================*/

void Relay_OFF (u16 RELA_x)
{
	GPIOF->BRR = RELA_x; 
	P_edge();
}

/*==============================================================================
*function:void Analog_sw_ON (u16 CH_x); 
*description: ��ģ�⿪��
*==============================================================================*/
void Analog_sw_ON (u16 SW_x)			//ģ�⿪�ؿ������Ŷ�ӦPe10-PF15��LVDT�źŵ���
{
	GPIOE->BRR = SW_x; //���ڸ��ݲ��Խ���޸�
}
/*==============================================================================
*function:void Analog_sw_OFF (u16 CH_x); 
*description: �ر�ģ�⿪��
*==============================================================================*/
void Analog_sw_OFF (u16 SW_x)			//ģ�⿪�ؿ������Ŷ�ӦPe10-PF15
{
	GPIOE->BSRR = SW_x; //֮��Ҫ���ǲ��Բ��޸�
}

