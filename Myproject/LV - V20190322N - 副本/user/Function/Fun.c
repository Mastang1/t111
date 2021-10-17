/*******************************************************************************             
*            ʵ��ϵͳ��λ/ϵͳ�Լ�/�������ú�ͨ��ѡ���ָ��Ĵ���
*******************************************************************************/
#include "Fun.h"

#include "systick.h"
#include "printf.h"
#include "relay.h"
#include "LCDdv.h"
//#include "stdbool.h"
/*=================����������=====================*/

uint8_t Buffer_last =0;
extern uint8_t Reset_Fback;	//Һ����λ
extern uint8_t Self_Chack_Fback;	//Һ����λ
extern uint8_t __IP[15];
extern __IO u8 RxBuffer1[29];
extern uint8_t Reset_LCD[];	//Һ����λ
extern uint8_t Clear_LCD[];	//Һ������
extern uint8_t CSR_Pos[];		//���λ������ָ�������������

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
/*==========����1������̫��ģ���������===================*/
uint8_t Usart_ON[]={0x2a,0x2b,0x2c};									//�򿪴���ģʽָ��
uint8_t Usart_OFF[]={0x3e,0x57,0x42,0x52,0x31,0x0d};	//�رմ���ģʽָ��
uint8_t Send_IP_Com[]={0x3e,0x52,0x4e,0x50,0x0d};			//����IP����ָ��

/*******************************************************************************             
*           									 ʵ��ϵͳ��λ
*******************************************************************************/
void Sys_reset(void)
{
		/*1.�̵�������ֱͨλ��
			2.AD 5292��λ
			3.Һ������λ
			4.ģ�⿪�عر�	
	*/
	uint16_t SPI_R;																			//����ֲ�����
	uint8_t i=0;																					//����ֲ�����
	uint8_t j=0;																					//����ֲ�����	
	
/*=================�̵���ֱͨ==============*/
	for(j=0;j<6;j++)																			//�ر����еļ̵���
	{
		Relay_OFF (0x0400<<j);									//LVDT����PF10-PF15,����0x400��ʾPF10��ѭ�����Ƶ�PF15
		delay_ms(50);
	}

///*=================ģ�⿪�رպ�ʹ���ݷŵ磨PF10-PF15��===============*/
//	Analog_sw_ON (0xFC00);
//	delay_ms(200);																		//��ʱһ��ʱ�䣬
//	Analog_sw_OFF (0xFC00);														//�Ͽ�ģ�⿪��
	
/*=================AD5292��λ����===============*/	
	for(i=0;i<6;i++)
	{

		SPI_R=Write_CMD(0x0001<<i,0x1803);								//����ͨ����AD5292��λ(CS from PE0-PE5)
		SPI_R=Write_CMD(0x0001<<i,0x0500);								//RDAC ��λ
		SPI_R=SPI_R<<1;																		//��ֹ����������
	}
/*=================Һ����λ����===============*/	
	LCD_Comand(Clear_LCD);
	//delay_ms(100);
	LCD_display(NORMAL,LCD_Display_CMD,6,1,0);			//��Һ������ʾ��NORMAL��
//	LCD_display(__IP,LCD_Display_CMD,15,1,1);			//��Һ������ʾ��NORMAL��
/*=================��λ�ɹ�����==============*/	
	USART1_Send_Char(Reset_Fback);
}

/*******************************************************************************             
*           									 ʵ��ϵͳ�Լ�
*******************************************************************************/
void AUTO_Check(void)	//ϵͳ�Լ�ִ�г���
{
	uint8_t j;
	uint8_t i;
	/*=================�̵���ֱͨ==============*/
	LCD_Comand(Clear_LCD); 
	LCD_display(checking,LCD_Display_CMD,8,0,0);
	for(j=0;j<6;j++)																			//�ر����еļ̵���
	{
		Relay_ON (0x0400<<j);
		delay_ms(500);
	}
	delay_ms(1000);
		for(i=0;i<6;i++)																			//�ر����еļ̵���
	{
		Relay_OFF (0x0400<<i);
		delay_ms(500);
	}
/*=================�Լ�ɹ�����==============*/	
	USART1_Send_Char(Self_Chack_Fback);	
	LCD_Comand(Clear_LCD); 
	LCD_display(NORMAL,LCD_Display_CMD,6,1,0);
	LCD_display(__IP,LCD_Display_CMD,15,0,0);
}


/*==========================================================================
Function:USART1CMD_exe();
Description:ָ��ִ�к���,

===========================================================================*/
void USART1CMD_exe(void)
{
/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&�Լ�͸�λ�ж�&&&&&&&&&&&&&&&&&&&&&&&&&&*/		
			if((RxBuffer1[1]==0x3c)||(RxBuffer1[15]==0x30)||(RxBuffer1[1]==0x3a))
			{
				Buffer_last=RxBuffer1[2];
				if(RxBuffer1[1]==0x3c)
				{
					Sys_reset();																		//ϵͳ��λִ�г���
					LCD_display(__IP,LCD_Display_CMD,15,0,0);				//����ӣ���������ʾIP
				} else if(RxBuffer1[15]==0x30)
				{
					AUTO_Check();//ִ���Լ����
					LCD_display(__IP,LCD_Display_CMD,15,0,0);				//����ӣ���������ʾIP
				} else
				{
					FB_state();
				}
			}
/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&�˴�Ҫע��˿��Ƿ��Ӧ&&&&&&&&&&&&&&&&&&&&&&&&&&&*/				
			else //�жϱ������ͨ��
			{			
				u8 i=0;
				u8 SPI_R=0;
				uint8_t Current_channel=0;
				LCD_Comand(Clear_LCD);
				LCD_display(channel,LCD_Display_CMD,16,0,0);    		 //����ע���һ��Һ����ʾ
				
				Current_channel=Buffer_last ^ RxBuffer1[2];					//���
				Buffer_last=RxBuffer1[2];
				
				for(i=0;i<6;i++)																		//forѭ������ѯ6��ͨ��������
				{
					if(Current_channel & (0x40>>i))										//��λ���ݣ������ҷֱ���1-6ͨ��
					{
							if(RxBuffer1[2] & (0x40>>i))
							{
								uint8_t Rx;																	//������յ��ĸ�λֵ��RxBuffer1[2i+3]�ĵ���λ��
								uint16_t Set_unit;													//����Set_unit��Ÿ�λ��
								uint8_t Set_dec;														//����Set_dec���С������
								uint16_t Set_Value;
								uint16_t Digipot;
								float ratio;
								
								uint8_t P_N;	
								
								uint16_t Souce_H_bit;
								uint16_t Souce_L_bit;
								uint16_t Souce_Value;
		/***********************ͨ����ע******************************************/
//								if(RxBuffer1[2] & (0x40>>i))
//								{
//									LCD_display(Ch_ON,LCD_Display_CMD,1,1,5+(i<<1)); 
//								}
		/*************************************************************************/						
								Relay_ON (0x0400<<i);
								delay_ms(1);
																																			
								SPI_R=Write_CMD(0x0001<<i,0x1803);					//�򿪶�Ӧͨ����д�루û���⣩
								SPI_R++;																
								
		/****************************�����ж�(û����)*********************************************/						
								
								P_N=(RxBuffer1[(2*i)+3])>>6;
								if(P_N!=0)
								{								//˵��Ϊ1����Ϊ��
									Analog_sw_ON(0x400<<i);
								}
								else if (P_N==0)
								{
									Analog_sw_OFF(0x400<<i);
								}
								else{}
					//			P_N=0;	
		/*************************����ֵ����**********************��������������**********************/							
								Souce_H_bit=RxBuffer1[(i*2)+16];
								Souce_H_bit=(u16)(Souce_H_bit*100);		//��λ����100
								Souce_L_bit=RxBuffer1[(i*2)+17];					//��ȡС�����ֵ�ֵ						
								Souce_Value=(uint16_t)(Souce_H_bit+Souce_L_bit);											//��ʱ��Set_Valueֵ�Ǹ���λ�������
									
									
		/****************************���ֵ����***************************************************/
								Rx=(RxBuffer1[(i*2)+3]) & 0x3f;	//�Ѹ�λ��6��λ��ֵ��ֵ��Rx0
								Set_unit=(uint16_t)(Rx*100);		//��λ����100,����2���ü�Ϊ���ΪVPP
								Set_dec=RxBuffer1[(i*2)+4];					//��ȡС�����ֵ�ֵ
								Set_Value=(uint16_t)(Set_unit+Set_dec);											//��ʱ��Set_Valueֵ�Ǹ���λ������

								ratio=((float)Set_Value)/((float)Souce_Value)*14;				//14K,��ʱΪ����ֵ

																			
								ratio=1-ratio/20.00;							//ratio�Ǹ���������	
								ratio=ratio*1024;								//��ʱ��ratio��RDAC�Ĵ�����Ҫ��ֵ�������ͣ� 
								Digipot=(uint16_t)(ratio+0.5);	//��ʱ��Digipot�����ֵ�λ��Ӧ��д���ֵ,Ӧ����������
									if(Digipot>1023) Digipot=1023;
		/*=====================д���ֵ�λ��============================================*/
																																	//�γ�ָ��
								SPI_R=Write_CMD(0x01<<i,0x0400|Digipot);		//�����ԣ�0x0500ʱ����ֵΪ15K��RAW�ӷ���
		/*=====================����ֵ�жϲ�����============================================*/	
						
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
			//	i=0;//i����
				USART1_Send_Char(0x55);	
			}
			
			
			RxCounter1=0;
			FLAG_Rx=RESET;
//	}
}



 /*==========================================================================
Function:USART2CMD_exe();
Description:ָ��ִ�к���,����ȡ�������ݰ��󣬼����¶ȣ�Ȼ���ж��Ƿ񳬹���ֵ����������ָʾ�Ƶ�״̬

===========================================================================*/
void USART2CMD_exe(void)
{
	if(RECEIVE_flag2==SET)//RECEIVE_flag2
	{
			if((RxBuffer2[5]*256+RxBuffer2[6])>5500)
			{
					GPIO_SetBits(GPIOA, GPIO_Pin_1);																				 //��ɫָʾ������LVDT�е�ָʾ�ƿ���λ��PF7���ţ���P4 ����
			}
			else  GPIO_ResetBits(GPIOA, GPIO_Pin_1);																		 //��ɫָʾ����	
			RxCounter2=0;
			RECEIVE_flag2=RESET;
	}	
}
 /*==========================================================================
Function:Usart_mode_ON();
Description:�򿪴��ڿ�����̫��ģʽ
===========================================================================*/
void Usart_mode_ON(void)
{
	USART1_Send_Char_Buff(Usart_ON,3);
	
}
 /*==========================================================================
Function:Usart_mode_OFF();
Description:�رմ��ڿ�����̫��ģʽ
===========================================================================*/
void Usart_mode_OFF(void)
{
	USART1_Send_Char_Buff(Usart_OFF,6);
}
 /*==========================================================================
Function:Send_ask_IP_com();
Description:����IP����ָ��
===========================================================================*/
void Send_ask_IP_com(void)
{
	USART1_Send_Char_Buff(Send_IP_Com,5);
}

void FB_state(void)
{
//	USART1_Send_Char_Buff(RxBuffer1,29);
}

