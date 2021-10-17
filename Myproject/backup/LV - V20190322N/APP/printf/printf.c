#include "printf.h"
#include "systick.h"

/*��������--------------------------------------------------------------------------------*/
__IO FlagStatus IP_ask_flag=SET;		//����һ�������͵�ȫ��volatile�ͱ���������ָʾ�������Ƿ����ڵȴ���ȡIP��ַ

__IO u8		RxCounter1=0;//__IO��������volatile
__IO u8		RxCounter2=0;
__IO u8 RxBuffer1[29];
__IO u8 RxBuffer2[10];
__IO FlagStatus FLAG_Rx=RESET;
__IO FlagStatus RECEIVE_flag2=RESET;





int fputc(int ch,FILE *p)  //����Ĭ�ϵģ���ʹ��printf����ʱ�Զ�����
{
	USART_SendData(USART3,(u8)ch);	
	while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==RESET);
	return ch;
}

/*******************************************************************************
* �� �� ��         : printf_init
* ��������		   : IO�˿ڼ�����1��ʱ�ӳ�ʼ������	   
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void USART1_INI(u32 baud) 	//printf��ʼ��
{
	GPIO_InitTypeDef GPIO_InitStructure;	//����һ���ṹ�������������ʼ��GPIO
	USART_InitTypeDef  USART_InitStructure;	  //���ڽṹ�嶨��
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
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//ʹ�ܻ���ʧ��ָ����USART�ж� �����ж�
	USART_ClearFlag(USART1,USART_FLAG_TC);//���USARTx�Ĵ������־λ
}

/*===============================================================================
Copyright: AMC
Version:V 1.0
Author:   Renegade 
Date: 2018/11/3
Description:����2����
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
Description:  Usart3����

===============================================================================*/


void USART3_INI(u32 baud)   
{  
    USART_InitTypeDef USART_InitStructure;  
//    NVIC_InitTypeDef NVIC_InitStructure;   
    GPIO_InitTypeDef GPIO_InitStructure;   
 
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
    USART_InitStructure.USART_BaudRate = baud;  
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;  
    USART_InitStructure.USART_StopBits = USART_StopBits_1;  
    USART_InitStructure.USART_Parity = USART_Parity_No;  
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;  
    // Configure USART3   
    USART_Init(USART3, &USART_InitStructure);//????3 
    USART_Cmd(USART3, ENABLE);//????3  
} 

/*==========================================================================
Function:USART1_Sned_Char();
Description:����1���ݷ��ͺ���

===========================================================================*/
void USART1_Send_Char(u8 temp)        
{  
    USART_SendData(USART1,(u8)temp);      
    while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);  
      
}
/*==========================================================================
Function:USART1_Sned_Char_Buff();
Description:����1���ݴ����ͺ���

===========================================================================*/
void USART1_Send_Char_Buff(u8 buf[],u32 len)  
{  
    u32 i;  
    for(i=0;i<len;i++)  
    USART1_Send_Char(buf[i]);           
}


/*==========================================================================
Function:USART2_Sned_Char();
Description:����2���ݷ��ͺ���

===========================================================================*/
void USART2_Send_Char(u8 temp)        
{  
    USART_SendData(USART2,(u8)temp);      
    while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);  
      
}
/*==========================================================================
Function:USART2_Sned_Char_Buff();
Description:����2���ݴ����ͺ���

===========================================================================*/
void USART2_Send_Char_Buff(u8 buf[],u32 len)  
{  
    u32 i;  
    for(i=0;i<len;i++)  
    USART2_Send_Char(buf[i]);  
          
}



/*==========================================================================
Function:USART3_Sned_Char();
Description:����3���ݷ��ͺ���

===========================================================================*/
void USART3_Send_Char(u8 temp)        
{  
    USART_SendData(USART3,(u8)temp);      
    while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==RESET);  
      
}
/*==========================================================================
Function:USART3_Sned_Char_Buff();
Description:����3���ݴ����ͺ���

===========================================================================*/
void USART3_Send_Char_Buff(u8 buf[],u32 len)  
{  
    u32 i;  
    for(i=0;i<len;i++)  
    USART3_Send_Char(buf[i]);  
          
}
























 /*==========================================================================
Function:USART1_IRQHandler();
Description:����1�жϺ�����������λ��ͨ�ţ���λ���������ݰ�����

===========================================================================*/
void My_USART1_IRQHandler(void)
{
	
		if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
		{
				__IO uint8_t buffer;
/*===========����һ�����������洢���յ���һ���ֽڵ�����==================*/			
				buffer = USART_ReceiveData(USART1);
				if(IP_ask_flag==SET)			//���IP_ask_flag��λ��˵�����������ڵȴ�����IP��ַ
				{												//���Դ�ʱҪ��IP��ַ���ݰ���������
					IP_pack_Rx(buffer);					
				}
				else
				{
					Comand_Rx(buffer);	
				}
			
			
			
		}
//������������			
			else				
					RxCounter1 = 0;


	

}	


 /*==========================================================================
Function:USART2_IRQHandler();
Description:����2�жϺ���������2���ݰ�����

===========================================================================*/
void My_USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
		{
				uint8_t buffer;
				/* Read one byte from the receive data register */
				buffer = USART_ReceiveData(USART2);//ÿ�ζ�����һ���ֲ�����������ֵΪ���յ�������
			
//�ж��Ƿ�Ϊ֡ͷ			
			if (RxCounter2 == 0)
			{
					if (buffer==0xff )
					{
						RxBuffer2[0] = buffer;
						RxCounter2 = 1;
					}
					else
							RxCounter2 = 0;
			}
//�жϽ������ݸ����Ƿ�Ϊ10			
			else if (RxCounter2 < 10)
			{
					RxBuffer2[RxCounter2] = buffer;
					RxCounter2++;
			}
//�ж��Ƿ����ȫһ������������֡			
			else if (RxCounter2 == 10)
			{
					RxBuffer2[RxCounter2] = buffer;
					RECEIVE_flag2  =	SET;				
			}
//������������			
			else				
					RxCounter2 = 0;
  }

}
 /*==========================================================================
Function:void IP_pack_Rx(void);
Description:����1��ȡIP��ַ

===========================================================================*/
static void IP_pack_Rx(uint8_t buffer)
{
/*========================�ж�֡ͷ=========================================*/
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
			
			
//�жϽ������ݸ����Ƿ�Ϊ19	(RxCounter1 < 19) &&	
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

//������������			
			else				
					RxCounter1 = 0;
}
 /*==========================================================================
Function:Comand_Rx(uint8_t buffer)
Description:����1��ȡIP��ַ

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
//�жϽ������ݸ����Ƿ�Ϊ17		
			else if (RxCounter1 < 28)
			{
					RxBuffer1[RxCounter1] = buffer;
					RxCounter1++;
			}
//�ж��Ƿ����ȫһ������������֡			
			else if (RxCounter1 == 28)
			{

				RxBuffer1[RxCounter1] = buffer;
				if(buffer==0xab)
				{
					FLAG_Rx  =	SET;	
				}else 
					RxCounter1 = 0;	
			}
//������������			
			else				
					RxCounter1 = 0;
}
