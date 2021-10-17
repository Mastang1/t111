/*******************************************************************************             
*            实现系统复位/系统自检/幅度设置和通道选择等指令的处理
*******************************************************************************/
#include "Fun.h"
#include "SPI.h"
#include "systick.h"
#include "printf.h"
#include "relay.h"
#include "LCDdv.h"
//#include "stdbool.h"
/*=================变量的声明=====================*/

uint8_t Buffer_last =0;
extern uint8_t Reset_Fback;	//液晶复位
extern uint8_t Self_Chack_Fback;	//液晶复位
extern uint8_t __IP[15];
extern __IO u8 RxBuffer1[29];
extern uint8_t Reset_LCD[];	//液晶复位
extern uint8_t Clear_LCD[];	//液晶清屏
extern uint8_t CSR_Pos[];		//光标位置设置指令，后续发送数据

extern uint8_t LCD_Display_CMD[];

extern uint8_t NORMAL[];//"NORMAL"
extern uint8_t WERROR[];//"ERROR"
extern uint8_t FAULT[];//"ERROR"

extern uint8_t CODE[];//"CODE:"
extern uint8_t check[];//"0001"
extern uint8_t Injec[];//"0002"
extern uint8_t channel[];//"chan:"
extern uint8_t checking[];//"checking:"
uint8_t Ch_ON[1]={0x2a};	//79
uint8_t Ch_OFF[1]={0x20};
/*==========串口1控制以太网模块相关数组===================*/
uint8_t Usart_ON[]={0x2a,0x2b,0x2c};									//打开串行模式指令
uint8_t Usart_OFF[]={0x3e,0x57,0x42,0x52,0x31,0x0d};	//关闭串行模式指令
uint8_t Send_IP_Com[]={0x3e,0x52,0x4e,0x50,0x0d};			//发送IP请求指令

/*******************************************************************************             
*           									 实现系统复位
*******************************************************************************/
void Sys_reset(void)
{
		/*1.继电器处于直通位置
			2.AD 5292复位
			3.液晶屏复位
			4.模拟开关关闭	
	*/
	uint16_t SPI_R;																			//定义局部变量
	uint8_t i=0;																					//定义局部变量
	uint8_t j=0;																					//定义局部变量	
	
/*=================继电器直通==============*/
	for(j=0;j<6;j++)																			//关闭所有的继电器
	{
		Relay_OFF (0x0400<<j);									//LVDT中是PF10-PF15,其中0x400表示PF10，循环左移到PF15
		delay_ms(50);
	}

///*=================模拟开关闭合使电容放电（PF10-PF15）===============*/
//	Analog_sw_ON (0xFC00);
//	delay_ms(200);																		//延时一段时间，
//	Analog_sw_OFF (0xFC00);														//断开模拟开关
	
/*=================AD5292复位控制===============*/	
	for(i=0;i<6;i++)
	{

		SPI_R=Write_CMD(0x0001<<i,0x1803);								//所有通道的AD5292复位(CS from PE0-PE5)
		SPI_R=Write_CMD(0x0001<<i,0x0500);								//RDAC 复位
		SPI_R=SPI_R<<1;																		//防止编译器报错
	}
/*=================液晶复位控制===============*/	
	LCD_Comand(Clear_LCD);
	//delay_ms(100);
	LCD_display(NORMAL,LCD_Display_CMD,6,1,0);			//在液晶屏显示“NORMAL”
//	LCD_display(__IP,LCD_Display_CMD,15,1,1);			//在液晶屏显示“NORMAL”
/*=================复位成功反馈==============*/	
	USART1_Send_Char(Reset_Fback);
}

/*******************************************************************************             
*           									 实现系统自检
*******************************************************************************/
void AUTO_Check(void)	//系统自检执行程序
{
	uint8_t j;
	uint8_t i;
	/*=================继电器直通==============*/
	LCD_Comand(Clear_LCD); 
	LCD_display(checking,LCD_Display_CMD,8,0,0);
	for(j=0;j<6;j++)																			//关闭所有的继电器
	{
		Relay_ON (0x0400<<j);
		delay_ms(500);
	}
	delay_ms(1000);
		for(i=0;i<6;i++)																			//关闭所有的继电器
	{
		Relay_OFF (0x0400<<i);
		delay_ms(500);
	}
/*=================自检成功反馈==============*/	
	USART1_Send_Char(Self_Chack_Fback);	
	LCD_Comand(Clear_LCD); 
	LCD_display(NORMAL,LCD_Display_CMD,6,1,0);
	LCD_display(__IP,LCD_Display_CMD,15,0,0);
}


/*==========================================================================
Function:USART1CMD_exe();
Description:指令执行函数,

===========================================================================*/
void USART1CMD_exe(void)
{
/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&自检和复位判断&&&&&&&&&&&&&&&&&&&&&&&&&&*/		
			if((RxBuffer1[1]==0x3c)||(RxBuffer1[15]==0x30)||(RxBuffer1[1]==0x3a))
			{
				Buffer_last=RxBuffer1[2];
				if(RxBuffer1[1]==0x3c)
				{
					Sys_reset();																		//系统复位执行程序
					LCD_display(__IP,LCD_Display_CMD,15,0,0);				//后添加，作用于显示IP
				} else if(RxBuffer1[15]==0x30)
				{
					AUTO_Check();//执行自检操作
					LCD_display(__IP,LCD_Display_CMD,15,0,0);				//后添加，作用于显示IP
				} else
				{
					FB_state();
				}
			}
/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&此处要注意端口是否对应&&&&&&&&&&&&&&&&&&&&&&&&&&&*/				
			else //判断被串入的通道
			{			
				u8 i=0;
				u8 SPI_R=0;
				uint8_t Current_channel=0;
				LCD_Comand(Clear_LCD);
				LCD_display(channel,LCD_Display_CMD,16,0,0);    		 //故障注入第一行液晶显示
				
				Current_channel=Buffer_last ^ RxBuffer1[2];					//异或
				Buffer_last=RxBuffer1[2];
				
				for(i=0;i<6;i++)																		//for循环，轮询6个通道的数据
				{
					if(Current_channel & (0x40>>i))										//六位数据，从左到右分别是1-6通道
					{
							if(RxBuffer1[2] & (0x40>>i))
							{
								uint8_t Rx;																	//定义接收到的高位值（RxBuffer1[2i+3]的低四位）
								uint16_t Set_unit;													//定义Set_unit存放个位数
								uint8_t Set_dec;														//定义Set_dec存放小数部分
								uint16_t Set_Value;
								uint16_t Digipot;
								float ratio;
								
								uint8_t P_N;	
								
								uint16_t Souce_H_bit;
								uint16_t Souce_L_bit;
								uint16_t Souce_Value;
		/***********************通道标注******************************************/
//								if(RxBuffer1[2] & (0x40>>i))
//								{
//									LCD_display(Ch_ON,LCD_Display_CMD,1,1,5+(i<<1)); 
//								}
		/*************************************************************************/						
								Relay_ON (0x0400<<i);
								delay_ms(1);
																																			
								SPI_R=Write_CMD(0x0001<<i,0x1803);					//打开对应通道的写入（没问题）
								SPI_R++;																
								
		/****************************正负判断(没问题)*********************************************/						
								
								P_N=(RxBuffer1[(2*i)+3])>>6;
								if(P_N!=0)
								{								//说明为1，则为负
									Analog_sw_ON(0x400<<i);
								}
								else if (P_N==0)
								{
									Analog_sw_OFF(0x400<<i);
								}
								else{}
					//			P_N=0;	
		/*************************输入值计算**********************？？？？？？？**********************/							
								Souce_H_bit=RxBuffer1[(i*2)+16];
								Souce_H_bit=(u16)(Souce_H_bit*100);		//高位乘以100
								Souce_L_bit=RxBuffer1[(i*2)+17];					//获取小数部分的值						
								Souce_Value=(uint16_t)(Souce_H_bit+Souce_L_bit);											//此时的Set_Value值是个三位数整数�
									
									
		/****************************输出值计算***************************************************/
								Rx=(RxBuffer1[(i*2)+3]) & 0x3f;	//把高位的6个位的值赋值给Rx0
								Set_unit=(uint16_t)(Rx*100);		//个位乘以100,除以2所得即为输出为VPP
								Set_dec=RxBuffer1[(i*2)+4];					//获取小数部分的值
								Set_Value=(uint16_t)(Set_unit+Set_dec);											//此时的Set_Value值是个三位数整数

								ratio=((float)Set_Value)/((float)Souce_Value)*14;				//14K,此时为电阻值

																			
								ratio=1-ratio/20.00;							//ratio是浮点型数据	
								ratio=ratio*1024;								//此时的ratio是RDAC寄存器需要的值（浮点型） 
								Digipot=(uint16_t)(ratio+0.5);	//此时的Digipot是数字电位器应该写入的值,应用四舍五入
									if(Digipot>1023) Digipot=1023;
		/*=====================写数字电位器============================================*/
																																	//形成指令
								SPI_R=Write_CMD(0x01<<i,0x0400|Digipot);		//经测试，0x0500时候，数值为15K；RAW接法，
		/*=====================正负值判断并设置============================================*/	
						
							}
							else
							{
								Relay_OFF (0x0400<<i);
							}
					}
					else{}
					
					if(RxBuffer1[2] & (0x40>>i))
					{
						LCD_display(Ch_ON,LCD_Display_CMD,1,1,5+(i<<1)); 
					}
					
			  }
			//	i=0;//i清零
				USART1_Send_Char(0x55);	
			}
			
			
			RxCounter1=0;
			FLAG_Rx=RESET;
//	}
}



 /*==========================================================================
Function:USART2CMD_exe();
Description:指令执行函数,当获取完整数据包后，计算温度，然后判断是否超过阈值，进而控制指示灯的状态

===========================================================================*/
void USART2CMD_exe(void)
{
	if(RECEIVE_flag2==SET)//RECEIVE_flag2
	{
			if((RxBuffer2[5]*256+RxBuffer2[6])>5500)
			{
					GPIO_SetBits(GPIOA, GPIO_Pin_1);																				 //红色指示灯亮（LVDT中的指示灯控制位是PF7引脚）即P4 引脚
			}
			else  GPIO_ResetBits(GPIOA, GPIO_Pin_1);																		 //绿色指示灯亮	
			RxCounter2=0;
			RECEIVE_flag2=RESET;
	}	
}
 /*==========================================================================
Function:Usart_mode_ON();
Description:打开串口控制以太网模式
===========================================================================*/
void Usart_mode_ON(void)
{
	USART1_Send_Char_Buff(Usart_ON,3);
	
}
 /*==========================================================================
Function:Usart_mode_OFF();
Description:关闭串口控制以太网模式
===========================================================================*/
void Usart_mode_OFF(void)
{
	USART1_Send_Char_Buff(Usart_OFF,6);
}
 /*==========================================================================
Function:Send_ask_IP_com();
Description:发送IP请求指令
===========================================================================*/
void Send_ask_IP_com(void)
{
	USART1_Send_Char_Buff(Send_IP_Com,5);
}

void FB_state(void)
{
//	USART1_Send_Char_Buff(RxBuffer1,29);
}

