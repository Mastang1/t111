#include "printf.h"
#include "systick.h"
#include "public.h"
/*变量定义--------------------------------------------------------------------------------*/
__IO FlagStatus IP_ask_flag=SET;		//定义一个布尔型的全局volatile型变量，用于指示主程序是否正在等待获取IP地址

__IO u8		RxCounter1=0;//__IO的意义是volatile
__IO u8		RxCounter2=0;
__IO u8 RxBuffer1[29];
__IO u8 RxBuffer2[10];
__IO u8 RxBuffer2N[10];
__IO FlagStatus FLAG_Rx=RESET;
__IO FlagStatus RECEIVE_flag2=RESET;
u32 Banttesla = 0;




int fputc(int ch,FILE *p)  //函数默认的，在使用printf函数时自动调用
{
	USART_SendData(USART3,(u8)ch);	
	while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==RESET);
	return ch;
}

/*******************************************************************************
* 函 数 名         : printf_init
* 函数功能		   : IO端口及串口1，时钟初始化函数	   
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void USART1_INI(u32 baud) 	//printf初始化
{
	GPIO_InitTypeDef GPIO_InitStructure;	//声明一个结构体变量，用来初始化GPIO
	USART_InitTypeDef  USART_InitStructure;	  //串口结构体定义
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1|RCC_APB2Periph_AFIO,ENABLE);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;//TX
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//RX
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
 

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);


	USART_InitStructure.USART_BaudRate=baud;   
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_Init(USART1,&USART_InitStructure);
	USART_Cmd(USART1, ENABLE);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//使能或者失能指定的USART中断 接收中断
	USART_ClearFlag(USART1,USART_FLAG_TC);//清除USARTx的待处理标志位
}

/*===============================================================================
Copyright: AMC
Version:V 1.0
Author:   Renegade 
Date: 2018/11/3
Description:串口2配置
===============================================================================*/
void USART2_INI(u32 baud) 
{  
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE); 

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;    //PA2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;     
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

//    RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,ENABLE); 
//    RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,DISABLE); 
	
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; 
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
		NVIC_Init(&NVIC_InitStructure);

    USART_InitStructure.USART_BaudRate = baud; 
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; 
    USART_InitStructure.USART_StopBits = USART_StopBits_1; 
    USART_InitStructure.USART_Parity = USART_Parity_No; 
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 

    USART_Init(USART2, &USART_InitStructure); 
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); 
    USART_Cmd(USART2, ENABLE);                     

}



/*===============================================================================
Copyright:
Version:1.0
Author:    Renegade
Date: 2018/12/01
Description:  Usart3配置

===============================================================================*/


void USART3_INI(u32 baud)   
{  
    USART_InitTypeDef USART_InitStructure;  
//    NVIC_InitTypeDef NVIC_InitStructure;   
    GPIO_InitTypeDef GPIO_InitStructure;   
		NVIC_InitTypeDef NVIC_InitStructure;
	
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE); 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);  
 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
    GPIO_Init(GPIOB, &GPIO_InitStructure);  

    // Configure USART3 Tx (PB.10) as alternate function push-pull  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
    GPIO_Init(GPIOB, &GPIO_InitStructure);  
    //
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 
		NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn; 
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
		NVIC_Init(&NVIC_InitStructure);
	
	
    USART_InitStructure.USART_BaudRate = baud;  
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;  
    USART_InitStructure.USART_StopBits = USART_StopBits_1;  
    USART_InitStructure.USART_Parity = USART_Parity_No;  
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;  
    // Configure USART3   
    USART_Init(USART3, &USART_InitStructure);//????3 
		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); 
    USART_Cmd(USART3, ENABLE);//????3  
} 

/*==========================================================================
Function:USART1_Sned_Char();
Description:串口1数据发送函数

===========================================================================*/
void USART1_Send_Char(u8 temp)        
{  
    USART_SendData(USART1,(u8)temp);      
    while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);  
      
}
/*==========================================================================
Function:USART1_Sned_Char_Buff();
Description:串口1数据串发送函数

===========================================================================*/
void USART1_Send_Char_Buff(u8 buf[],u32 len)  
{  
    u32 i;  
    for(i=0;i<len;i++)  
    USART1_Send_Char(buf[i]);           
}


/*==========================================================================
Function:USART2_Sned_Char();
Description:串口2数据发送函数

===========================================================================*/
void USART2_Send_Char(u8 temp)        
{  
    USART_SendData(USART2,(u8)temp);      
    while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);  
      
}
/*==========================================================================
Function:USART2_Sned_Char_Buff();
Description:串口2数据串发送函数

===========================================================================*/
void USART2_Send_Char_Buff(u8 buf[],u32 len)  
{  
    u32 i;  
    for(i=0;i<len;i++)  
    USART2_Send_Char(buf[i]);  
          
}



/*==========================================================================
Function:USART3_Sned_Char();
Description:串口3数据发送函数

===========================================================================*/
void USART3_Send_Char(u8 temp)        
{  
    USART_SendData(USART3,(u8)temp);      
    while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==RESET);  
      
}
/*==========================================================================
Function:USART3_Sned_Char_Buff();
Description:串口3数据串发送函数

===========================================================================*/
void USART3_Send_Char_Buff(u8 buf[],u32 len)  
{  
    u32 i;  
    for(i=0;i<len;i++)  
    USART3_Send_Char(buf[i]);  
          
}


 /*==========================================================================
Function:USART3_IRQHandler();
Description:串口1中断函数用于与上位机通信，上位机发送数据包解析
#define RecSuccess 0x11
#define RecData    0x22
#define UartCnt    0x33

===========================================================================*/
void My_USART3_IRQHandler(void)
{
	
		if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
		{
			__IO uint8_t buffer;
		  unsigned char SN = 0;
			u32 BautVal_32 = 0;
			static unsigned char Uart3Cnt = 0;

			buffer = USART_ReceiveData(USART3);
			
/*****************帧头判断*********************************************/			
			if(buffer == 0x71)
			{
				Uart3Cnt = 1;
			}
			else if(Uart3Cnt == 1)
			{
				SN = buffer;
				
				switch(SN)
				{
					case 0:
						BautVal_32 = 921600;
					break;
					
					case 1:
						BautVal_32 = 4800;
					break;
				
					case 2:
						BautVal_32 = 9600;
					break;
										
					case 3:
						BautVal_32 = 19200;
					break;
					case 4:
						BautVal_32 = 115200;
					break;
					case 5:
						BautVal_32 = 38400;
					break;
					
					default:
						break;
				}
				Banttesla = BautVal_32;
				Uart3Cnt = 0;
			}
			else
			{
				Uart3Cnt = 0;
			}
			
		}	
}	





 /*==========================================================================
Function:USART1_IRQHandler();
Description:串口1中断函数用于与上位机通信，上位机发送数据包解析

===========================================================================*/
void My_USART1_IRQHandler(void)
{
	
		if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
		{
				__IO uint8_t buffer;
/*===========定义一个变量用来存储接收到的一个字节的数据==================*/			
				buffer = USART_ReceiveData(USART1);
				if(IP_ask_flag==SET)			//如果IP_ask_flag置位，说明主函数正在等待接收IP地址
				{												//所以此时要用IP地址数据包解析函数
					IP_pack_Rx(buffer);					
				}
				else
				{
					Comand_Rx(buffer);	
				}
			
			
			
		}
//其他计数归零			
			else				
					RxCounter1 = 0;


	

}	


 /*==========================================================================
Function:USART2_IRQHandler();
Description:串口2中断函数，串口2数据包解析

===========================================================================*/
void My_USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
		{
				uint8_t buffer;
	//0 假 1真
				/* Read one byte from the receive data register */
				buffer = USART_ReceiveData(USART2);//每次都定义一个局部变量，来赋值为接收到的数据
			
//判断是否为帧头			
			if (RxCounter2 == 0)		//RxCounter 是当前计数值
			{
					if (buffer==0xAA )
					{
						RxBuffer2[0] = buffer;
						RxBuffer2N[0] = buffer;
						RxCounter2 = 1;
					}
					else
					{
						flag_YN = 0;
						RxBuffer2N[0] = buffer;
						RxCounter2 = 1;
					}
			}
			else if(RxCounter2 == 1)
			{
					if (buffer==0x55 )
					{
						if(RxBuffer2[0] == 0xAA)
						{
							RxBuffer2[1] = buffer;
							RxBuffer2N[0] = 0;
							RxCounter2 = 2;
							flag_YN = 1;
						}
						else
						{
							flag_YN = 0;
							RxBuffer2[0] = 0;
							RxBuffer2N[1] = buffer;
						}

					}
					else
					{
						flag_YN = 0;
						RxBuffer2N[1] = buffer;
					}
			}
			
			
			
//判断接收数据个数是否为10			
			else if ((RxCounter2 > 1) && (RxCounter2 < 8))
			{
				if(flag_YN == 0)
				{
					RxBuffer2[RxCounter2] = buffer;
					RxCounter2++;
				}
				else
				{
					RxBuffer2N[RxCounter2] = buffer;
					RxCounter2++;
				}
			}
//判断是否接收全一个完整的数据帧			
			else if (RxCounter2 == 8)
			{
				USART2_Send_Char(0x11);
				if(flag_YN == 0)
				{
					RxBuffer2[RxCounter2] = buffer;
					RxCounter2++;
				}
				else
				{
					RxBuffer2N[RxCounter2] = buffer;
					RxCounter2++;
				}
					RECEIVE_flag2  =	SET;				
			}
//其他计数归零			
			else				
					RxCounter2 = 0;
  }

}
 /*==========================================================================
Function:void IP_pack_Rx(void);
Description:串口1获取IP地址

===========================================================================*/
static void IP_pack_Rx(uint8_t buffer)
{
/*========================判断帧头=========================================*/
	if (RxCounter1 == 0)
			{
					if (buffer==0x59 )
					{
						RxBuffer1[0] = buffer;
						RxCounter1 = 1;
					}
					else
							RxCounter1 = 0;
			}
	else if(RxCounter1 == 1)
			{
					if (buffer==0x4e )
					{
						RxBuffer1[1] = buffer;
						RxCounter1 = 2;
					}
					else
					{
						RxCounter1 = 0;
					}
			}
/*=========================================================================*/			
			
			
//判断接收数据个数是否为19	(RxCounter1 < 19) &&	
			else if( (RxCounter1 < 19) &&(buffer!=0x0d) )
			{
				
					RxBuffer1[RxCounter1] = buffer;
					RxCounter1++;
			} 
			else if(buffer==0x0d)
			{
				RxBuffer1[RxCounter1] = buffer;
				FLAG_Rx  =	SET;	
			}

//其他计数归零			
			else				
					RxCounter1 = 0;
}
 /*==========================================================================
Function:Comand_Rx(uint8_t buffer)
Description:串口1获取IP地址

===========================================================================*/
static void Comand_Rx(uint8_t buffer)
{
	if (RxCounter1 == 0)
			{
					if (buffer==0xff )
					{
						RxBuffer1[0] = buffer;
						RxCounter1 = 1;
					}
					else
							RxCounter1 = 0;
			}
//判断接收数据个数是否为17		
			else if (RxCounter1 < 28)
			{
					RxBuffer1[RxCounter1] = buffer;
					RxCounter1++;
			}
//判断是否接收全一个完整的数据帧			
			else if (RxCounter1 == 28)
			{

				RxBuffer1[RxCounter1] = buffer;
				if(buffer==0xab)
				{
					FLAG_Rx  =	SET;	
				}else 
					RxCounter1 = 0;	
			}
//其他计数归零			
			else				
					RxCounter1 = 0;
}



