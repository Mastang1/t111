/*******************************************************************************             
*                实现：
								1. 继电器的初始化；
								2. 继电器的操作函数；
								3. 模拟开关的初始化和操作函数；
								4. LED的初始化和开关控制函数
*******************************************************************************/
#include "relay.h"
#include "systick.h"
/*==============================================================================
*function:void relay_ini(void);
*description: 继电器初始化为推挽输出模式，并把16通道IO清零
*==============================================================================*/
void Relay_INI(void)
{
	Relay_Pin_Ini();
	Lat_Clk_Ini();
}



static void Relay_Pin_Ini(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //声明一个结构体变量，用来初始化GPIO

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);

	/*  配置GPIO的模式和IO口 */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_All;  //选择你要设置的IO口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //设置推挽输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //设置传输速率
	GPIO_Init(GPIOF,&GPIO_InitStructure); 	   /* 初始化GPIO */
	GPIOF->BRR = 0xffff; //全部清零
}



/*==============================================================================
*function:void relay_handle(void);
*description: 锁存器时钟控制引脚初始化,并置1
*==============================================================================*/

static void Lat_Clk_Ini(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //声明一个结构体变量，用来初始化GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

	/*  配置GPIO的模式和IO口 */
	GPIO_InitStructure.GPIO_Pin=LACK_CLK;  //选择你要设置的IO口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //设置推挽输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //设置传输速率
	GPIO_Init(GPIOC,&GPIO_InitStructure); 	   /* 初始化GPIO */
	GPIO_ResetBits(GPIOC, LACK_CLK);
}


/*==============================================================================
*function:void relay_handle(void); 	GPIOF->BSRR = (uint32_t)0xfc00; 
*description: 来个一上升沿
*==============================================================================*/

static void P_edge(void)
{
//	delay_ms(10);
//	GPIO_SetBits(GPIOC, LACK_CLK);
//	//GPIOF->BSRR = GPIO_Pin_9; //全部置1
//	delay_ms(10);
//	GPIO_ResetBits(GPIOC, LACK_CLK);
////	GPIOF->BRR = GPIO_Pin_9; //全部清零
//	delay_ms(10);
	//	delay_ms(1);
	GPIO_SetBits(GPIOF, GPIO_Pin_9);
	//GPIOF->BSRR = GPIO_Pin_9; //全部置1
	delay_us(10);
	GPIO_ResetBits(GPIOF, GPIO_Pin_9);
//	GPIOF->BRR = GPIO_Pin_9; //全部清零
	delay_us(10);
}

/*==============================================================================
*function:void Relay_ON (u16 relay_x)	GPIOF->BSRR = (uint32_t)0xfc00; 
*description: 开继电器
*==============================================================================*/

void Relay_ON (u16 RELA_x)
{
	GPIOF->BSRR = RELA_x; 
	P_edge();
}

/*==============================================================================
*function:void Relay_ON (u16 relay_x);	GPIOF->BSRR = (uint32_t)0xfc00; 
*description: 关继电器
*==============================================================================*/

void Relay_OFF (u16 RELA_x)
{
	GPIOF->BRR = RELA_x; 
	P_edge();
}

/*==============================================================================
*function:void Analog_sw_ON (u16 CH_x); 
*description: 打开模拟开关
*==============================================================================*/
void Analog_sw_ON (u16 SW_x)			//模拟开关控制引脚对应Pe10-PF15（LVDT信号调理）
{
	GPIOE->BRR = SW_x; //后期根据测试结果修改
}
/*==============================================================================
*function:void Analog_sw_OFF (u16 CH_x); 
*description: 关闭模拟开关
*==============================================================================*/
void Analog_sw_OFF (u16 SW_x)			//模拟开关控制引脚对应Pe10-PF15
{
	GPIOE->BSRR = SW_x; //之后要考虑测试并修改
}

