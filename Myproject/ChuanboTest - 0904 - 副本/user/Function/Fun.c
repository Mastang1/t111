/*******************************************************************************             
*            ÊµÏÖÏµÍ³¸´Î»/ÏµÍ³×Ô¼ì/·ù¶ÈÉèÖÃºÍÍ¨µÀÑ¡ÔñµÈÖ¸ÁîµÄ´¦Àí
*******************************************************************************/
#include "Fun.h"

#include "systick.h"
#include "printf.h"
#include "relay.h"
#include "LCDdv.h"
//#include "stdbool.h"
/*=================±äÁ¿µÄÉùÃ÷=====================*/

uint8_t Buffer_last =0;
extern uint8_t Reset_Fback;	//Òº¾§¸´Î»
extern uint8_t Self_Chack_Fback;	//Òº¾§¸´Î»
extern uint8_t __IP[15];
extern __IO u8 RxBuffer1[29];
extern uint8_t Reset_LCD[];	//Òº¾§¸´Î»
extern uint8_t Clear_LCD[];	//Òº¾§ÇåÆÁ
extern uint8_t CSR_Pos[];		//¹â±êÎ»ÖÃÉèÖÃÖ¸Áî£¬ºóĞø·¢ËÍÊı¾İ

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
/*==========´®¿Ú1¿ØÖÆÒÔÌ«ÍøÄ£¿éÏà¹ØÊı×é===================*/
uint8_t Usart_ON[]={0x2a,0x2b,0x2c};									//´ò¿ª´®ĞĞÄ£Ê½Ö¸Áî
uint8_t Usart_OFF[]={0x3e,0x57,0x42,0x52,0x31,0x0d};	//¹Ø±Õ´®ĞĞÄ£Ê½Ö¸Áî
uint8_t Send_IP_Com[]={0x3e,0x52,0x4e,0x50,0x0d};			//·¢ËÍIPÇëÇóÖ¸Áî

/*******************************************************************************             
*           									 ÊµÏÖÏµÍ³¸´Î»
*******************************************************************************/
void Sys_reset(void)
{
		/*1.¼ÌµçÆ÷´¦ÓÚÖ±Í¨Î»ÖÃ
			2.AD 5292¸´Î»
			3.Òº¾§ÆÁ¸´Î»
			4.Ä£Äâ¿ª¹Ø¹Ø±Õ	
	*/
	uint16_t SPI_R;																			//¶¨Òå¾Ö²¿±äÁ¿
	uint8_t i=0;																					//¶¨Òå¾Ö²¿±äÁ¿
	uint8_t j=0;																					//¶¨Òå¾Ö²¿±äÁ¿	
	
/*=================¼ÌµçÆ÷Ö±Í¨==============*/
	for(j=0;j<6;j++)																			//¹Ø±ÕËùÓĞµÄ¼ÌµçÆ÷
	{
		Relay_OFF (0x0400<<j);									//LVDTÖĞÊÇPF10-PF15,ÆäÖĞ0x400±íÊ¾PF10£¬Ñ­»·×óÒÆµ½PF15
		delay_ms(50);
	}

///*=================Ä£Äâ¿ª¹Ø±ÕºÏÊ¹µçÈİ·Åµç£¨PF10-PF15£©===============*/
//	Analog_sw_ON (0xFC00);
//	delay_ms(200);																		//ÑÓÊ±Ò»¶ÎÊ±¼ä£¬
//	Analog_sw_OFF (0xFC00);														//¶Ï¿ªÄ£Äâ¿ª¹Ø
	
/*=================AD5292¸´Î»¿ØÖÆ===============*/	
	for(i=0;i<6;i++)
	{

		SPI_R=Write_CMD(0x0001<<i,0x1803);								//ËùÓĞÍ¨µÀµÄAD5292¸´Î»(CS from PE0-PE5)
		SPI_R=Write_CMD(0x0001<<i,0x0500);								//RDAC ¸´Î»
		SPI_R=SPI_R<<1;																		//·ÀÖ¹±àÒëÆ÷±¨´í
	}
/*=================Òº¾§¸´Î»¿ØÖÆ===============*/	
	LCD_Comand(Clear_LCD);
	//delay_ms(100);
	LCD_display(NORMAL,LCD_Display_CMD,6,1,0);			//ÔÚÒº¾§ÆÁÏÔÊ¾¡°NORMAL¡±
//	LCD_display(__IP,LCD_Display_CMD,15,1,1);			//ÔÚÒº¾§ÆÁÏÔÊ¾¡°NORMAL¡±
/*=================¸´Î»³É¹¦·´À¡==============*/	
	USART1_Send_Char(Reset_Fback);
}

/*******************************************************************************             
*           									 ÊµÏÖÏµÍ³×Ô¼ì
*******************************************************************************/
void AUTO_Check(void)	//ÏµÍ³×Ô¼ìÖ´ĞĞ³ÌĞò
{
	uint8_t j;
	uint8_t i;
	/*=================¼ÌµçÆ÷Ö±Í¨==============*/
	LCD_Comand(Clear_LCD); 
	LCD_display(checking,LCD_Display_CMD,8,0,0);
	for(j=0;j<6;j++)																			//¹Ø±ÕËùÓĞµÄ¼ÌµçÆ÷
	{
		Relay_ON (0x0400<<j);
		delay_ms(500);
	}
	delay_ms(1000);
		for(i=0;i<6;i++)																			//¹Ø±ÕËùÓĞµÄ¼ÌµçÆ÷
	{
		Relay_OFF (0x0400<<i);
		delay_ms(500);
	}
/*=================×Ô¼ì³É¹¦·´À¡==============*/	
	USART1_Send_Char(Self_Chack_Fback);	
	LCD_Comand(Clear_LCD); 
	LCD_display(NORMAL,LCD_Display_CMD,6,1,0);
	LCD_display(__IP,LCD_Display_CMD,15,0,0);
}


/*==========================================================================
Function:USART1CMD_exe();
Description:Ö¸ÁîÖ´ĞĞº¯Êı,

===========================================================================*/
void USART1CMD_exe(void)
{
/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&×Ô¼ìºÍ¸´Î»ÅĞ¶Ï&&&&&&&&&&&&&&&&&&&&&&&&&&*/		
			if((RxBuffer1[1]==0x3c)||(RxBuffer1[15]==0x30)||(RxBuffer1[1]==0x3a))
			{
				Buffer_last=RxBuffer1[2];
				if(RxBuffer1[1]==0x3c)
				{
					Sys_reset();																		//ÏµÍ³¸´Î»Ö´ĞĞ³ÌĞò
					LCD_display(__IP,LCD_Display_CMD,15,0,0);				//ºóÌí¼Ó£¬×÷ÓÃÓÚÏÔÊ¾IP
				} else if(RxBuffer1[15]==0x30)
				{
					AUTO_Check();//Ö´ĞĞ×Ô¼ì²Ù×÷
					LCD_display(__IP,LCD_Display_CMD,15,0,0);				//ºóÌí¼Ó£¬×÷ÓÃÓÚÏÔÊ¾IP
				} else
				{
					FB_state();
				}
			}
/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&´Ë´¦Òª×¢Òâ¶Ë¿ÚÊÇ·ñ¶ÔÓ¦&&&&&&&&&&&&&&&&&&&&&&&&&&&*/				
			else //ÅĞ¶Ï±»´®ÈëµÄÍ¨µÀ
			{			
				u8 i=0;
				u8 SPI_R=0;
				uint8_t Current_channel=0;
				LCD_Comand(Clear_LCD);
				LCD_display(channel,LCD_Display_CMD,16,0,0);    		 //¹ÊÕÏ×¢ÈëµÚÒ»ĞĞÒº¾§ÏÔÊ¾
				
				Current_channel=Buffer_last ^ RxBuffer1[2];					//Òì»ò
				Buffer_last=RxBuffer1[2];
				
				for(i=0;i<6;i++)																		//forÑ­»·£¬ÂÖÑ¯6¸öÍ¨µÀµÄÊı¾İ
				{
					if(Current_channel & (0x40>>i))										//ÁùÎ»Êı¾İ£¬´Ó×óµ½ÓÒ·Ö±ğÊÇ1-6Í¨µÀ
					{
							if(RxBuffer1[2] & (0x40>>i))
							{
								uint8_t Rx;																	//¶¨Òå½ÓÊÕµ½µÄ¸ßÎ»Öµ£¨RxBuffer1[2i+3]µÄµÍËÄÎ»£©
								uint16_t Set_unit;													//¶¨ÒåSet_unit´æ·Å¸öÎ»Êı
								uint8_t Set_dec;														//¶¨ÒåSet_dec´æ·ÅĞ¡Êı²¿·Ö
								uint16_t Set_Value;
								uint16_t Digipot;
								float ratio;
								
								uint8_t P_N;	
								
								uint16_t Souce_H_bit;
								uint16_t Souce_L_bit;
								uint16_t Souce_Value;
		/***********************Í¨µÀ±ê×¢******************************************/
//								if(RxBuffer1[2] & (0x40>>i))
//								{
//									LCD_display(Ch_ON,LCD_Display_CMD,1,1,5+(i<<1)); 
//								}
		/*************************************************************************/						
								Relay_ON (0x0400<<i);
								delay_ms(1);
																																			
								SPI_R=Write_CMD(0x0001<<i,0x1803);					//´ò¿ª¶ÔÓ¦Í¨µÀµÄĞ´Èë£¨Ã»ÎÊÌâ£©
								SPI_R++;																
								
		/****************************Õı¸ºÅĞ¶Ï(Ã»ÎÊÌâ)*********************************************/						
								
								P_N=(RxBuffer1[(2*i)+3])>>6;
								if(P_N!=0)
								{								//ËµÃ÷Îª1£¬ÔòÎª¸º
									Analog_sw_ON(0x400<<i);
								}
								else if (P_N==0)
								{
									Analog_sw_OFF(0x400<<i);
								}
								else{}
					//			P_N=0;	
		/*************************ÊäÈëÖµ¼ÆËã**********************£¿£¿£¿£¿£¿£¿£¿**********************/							
								Souce_H_bit=RxBuffer1[(i*2)+16];
								Souce_H_bit=(u16)(Souce_H_bit*100);		//¸ßÎ»³ËÒÔ100
								Souce_L_bit=RxBuffer1[(i*2)+17];					//»ñÈ¡Ğ¡Êı²¿·ÖµÄÖµ						
								Souce_Value=(uint16_t)(Souce_H_bit+Souce_L_bit);											//´ËÊ±µÄSet_ValueÖµÊÇ¸öÈıÎ»ÊıÕûÊı°
									
									
		/****************************Êä³öÖµ¼ÆËã***************************************************/
								Rx=(RxBuffer1[(i*2)+3]) & 0x3f;	//°Ñ¸ßÎ»µÄ6¸öÎ»µÄÖµ¸³Öµ¸øRx0
								Set_unit=(uint16_t)(Rx*100);		//¸öÎ»³ËÒÔ100,³ıÒÔ2ËùµÃ¼´ÎªÊä³öÎªVPP
								Set_dec=RxBuffer1[(i*2)+4];					//»ñÈ¡Ğ¡Êı²¿·ÖµÄÖµ
								Set_Value=(uint16_t)(Set_unit+Set_dec);											//´ËÊ±µÄSet_ValueÖµÊÇ¸öÈıÎ»ÊıÕûÊı

								ratio=((float)Set_Value)/((float)Souce_Value)*14;				//14K,´ËÊ±Îªµç×èÖµ

																			
								ratio=1-ratio/20.00;							//ratioÊÇ¸¡µãĞÍÊı¾İ	
								ratio=ratio*1024;								//´ËÊ±µÄratioÊÇRDAC¼Ä´æÆ÷ĞèÒªµÄÖµ£¨¸¡µãĞÍ£© 
								Digipot=(uint16_t)(ratio+0.5);	//´ËÊ±µÄDigipotÊÇÊı×ÖµçÎ»Æ÷Ó¦¸ÃĞ´ÈëµÄÖµ,Ó¦ÓÃËÄÉáÎåÈë
									if(Digipot>1023) Digipot=1023;
		/*=====================Ğ´Êı×ÖµçÎ»Æ÷============================================*/
																																	//ĞÎ³ÉÖ¸Áî
								SPI_R=Write_CMD(0x01<<i,0x0400|Digipot);		//¾­²âÊÔ£¬0x0500Ê±ºò£¬ÊıÖµÎª15K£»RAW½Ó·¨£¬
		/*=====================Õı¸ºÖµÅĞ¶Ï²¢ÉèÖÃ============================================*/	
						
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
			//	i=0;//iÇåÁã
				USART1_Send_Char(0x55);	
			}
			
			
			RxCounter1=0;
			FLAG_Rx=RESET;
//	}
}



 /*==========================================================================
Function:USART2CMD_exe();
Description:Ö¸ÁîÖ´ĞĞº¯Êı,µ±»ñÈ¡ÍêÕûÊı¾İ°üºó£¬¼ÆËãÎÂ¶È£¬È»ºóÅĞ¶ÏÊÇ·ñ³¬¹ıãĞÖµ£¬½ø¶ø¿ØÖÆÖ¸Ê¾µÆµÄ×´Ì¬

===========================================================================*/
void USART2CMD_exe(void)
{
	if(RECEIVE_flag2==SET)//RECEIVE_flag2
	{
			if((RxBuffer2[5]*256+RxBuffer2[6])>5500)
			{
					GPIO_SetBits(GPIOA, GPIO_Pin_1);																				 //ºìÉ«Ö¸Ê¾µÆÁÁ£¨LVDTÖĞµÄÖ¸Ê¾µÆ¿ØÖÆÎ»ÊÇPF7Òı½Å£©¼´P4 Òı½Å
			}
			else  GPIO_ResetBits(GPIOA, GPIO_Pin_1);																		 //ÂÌÉ«Ö¸Ê¾µÆÁÁ	
			RxCounter2=0;
			RECEIVE_flag2=RESET;
	}	
}
 /*==========================================================================
Function:Usart_mode_ON();
Description:´ò¿ª´®¿Ú¿ØÖÆÒÔÌ«ÍøÄ£Ê½
===========================================================================*/
void Usart_mode_ON(void)
{
	USART1_Send_Char_Buff(Usart_ON,3);
	
}
 /*==========================================================================
Function:Usart_mode_OFF();
Description:¹Ø±Õ´®¿Ú¿ØÖÆÒÔÌ«ÍøÄ£Ê½
===========================================================================*/
void Usart_mode_OFF(void)
{
	USART1_Send_Char_Buff(Usart_OFF,6);
}
 /*==========================================================================
Function:Send_ask_IP_com();
Description:·¢ËÍIPÇëÇóÖ¸Áî
===========================================================================*/
void Send_ask_IP_com(void)
{
	USART1_Send_Char_Buff(Send_IP_Com,5);
}

void FB_state(void)
{
//	USART1_Send_Char_Buff(RxBuffer1,29);
}

