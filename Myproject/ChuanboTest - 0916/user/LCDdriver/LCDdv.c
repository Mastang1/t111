#include "LCDdv.h"
#include "printf.h"
#include "systick.h"

/*==========================================================================
Function:USART3_Sned_Char_Buff();
Description:Һ���ַ���ʾ
дһ���ĵã�
��װ�⺯�����������������ʲô
��һ���ļ��з�װ�⺯�����������������б�����
===========================================================================*/
/*================Һ������ָ����==============*/
uint8_t __IP[15];

uint8_t Reset_Fback=0x55;	//Һ����λ
uint8_t Self_Chack_Fback=0x55;	//
uint8_t Reset_LCD[]={0xaa,0x01};	//Һ����λ
uint8_t Clear_LCD[]={0xaa,0x10};	//Һ������
uint8_t CSR_Pos[]={0xaa,0x20};		//���λ������ָ�������������

uint8_t LCD_Display_CMD[]={0xaa,0x25};

uint8_t NORMAL[]={0x4E,0x4f,0x52,0x4D,0x41,0x4C};//"NORMAL"
uint8_t WERROR[]={0x45,0x52,0x52,0x4f,0x52};//"ERROR"
uint8_t FAULT[]={0x46,0x41,0x55,0x4c,0x54};//"FAULT"

uint8_t CODE[]={0x43,0x4f,0x44,0x45,0x3a};//"CODE:"
uint8_t check[]={0x30,0x30,0x30,0x31};//"0001"
uint8_t Injec[]={0x30,0x30,0x30,0x32};//"0002"
uint8_t channel[]={0x43,0x48,0x41,0x4e,0x3a,0x31,0xa0,0x32,0xa0,0x33,0xa0,0x34,0xa0,0x35,0xa0,0x36};//"chan:"16
uint8_t checking[]={0x43,0x48,0x45,0x43,0x4b,0x49,0x4e,0x47};//"checking:"
/*==========================================================================
Function:USART3_Sned_Char_Buff();
Description:LCD���λ�����ã�
===========================================================================*/
static void CSRPos_set(uint8_t Line,uint8_t Rank)  		//���������ֽڵ�ָ����Ҫ��ɣ�����Һ�����Ⱥ���ֵ��ʾ
{  
		USART3_Send_Char_Buff( CSR_Pos,2);//Һ����λLCD_Display_CMD
		USART3_Send_Char(Line);//Һ����λLCD_Display_CMD
		USART3_Send_Char(Rank);//Һ����λLCD_Display_CMD
		delay_ms(1);  
		
}
/*==========================================================================
Function:LCD_display();
Description:LCD�ַ���ʾ  LCD_Display_CMD
===========================================================================*/
void LCD_display(uint8_t _parm[],uint8_t _Comand[],uint8_t len,uint8_t Line,uint8_t Rank)  
{  
		CSRPos_set(Line,Rank);
		USART3_Send_Char_Buff( _Comand,2);	//�����
		USART3_Send_Char_Buff( _parm,len);  //uint8_t Display_Char//"NORMAL" 
		USART3_Send_Char(0x0d);							//���ݷ������Ҫ��0x0d��β
		delay_us(10);  	
}

/*==========================================================================
Function:USART3_Sned_Char_Buff();
Description:LCD����
===========================================================================*/
void LCD_Comand(uint8_t _Comand[])  		//���������ֽڵ�ָ����Ҫ��ɣ�����Һ�����Ⱥ���ֵ��ʾ
{  
		USART3_Send_Char_Buff( _Comand,2);//Һ����λLCD_Display_CMD
		delay_ms(1);		
}


/*==========================================================================
Function:IP_display();
Description:LCD����
===========================================================================*/
void IP_display(void)  		//���������ֽڵ�ָ����Ҫ��ɣ�����Һ�����Ⱥ���ֵ��ʾ
{  
				uint8_t i;
	
				for(i=0;i<15;i++)
				{
					if(__IP[i]!=0x0d)
					{
						__IP[i]=RxBuffer1[i+3];
					} else
					{
						__IP[i]=0x0d;					//�˴���ʹ������ʾΪ�հ�
					}
				}
				LCD_Comand(Clear_LCD);	
			//	CSRPos_set(0,2);
				LCD_display(__IP,LCD_Display_CMD,15,0,0);			//��Һ������ʾ��NORMAL��
				LCD_display(NORMAL,LCD_Display_CMD,6,1,0);
}
