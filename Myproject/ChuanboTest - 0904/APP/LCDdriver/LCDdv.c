#include "LCDdv.h"
#include "printf.h"

/*==========================================================================
Function:USART3_Sned_Char_Buff();
Description:Һ���ַ���ʾ
дһ���ĵã�
��װ�⺯�����������������ʲô
��һ���ļ��з�װ�⺯�����������������б�����
===========================================================================*/
void LCD_display(uint8_t _parm[],uint8_t _Comand[],uint8_t len)  
{  
		USART3_Sned_Char_Buff( Reset_LCD,2);//Һ����λLCD_Display_CMD
		delay_ms(10);
	
		USART3_Sned_Char_Buff( _Comand,2);	//�����
		USART3_Sned_Char_Buff( _parm,len);  //uint8_t Display_Char//"NORMAL" 
		delay_ms(10);  
		
}

/*==========================================================================
Function:USART3_Sned_Char_Buff();
Description:LCD����
===========================================================================*/
void LCD_display(uint8_t _parm[],uint8_t _Comand[],uint8_t len)  
{  
		USART3_Sned_Char_Buff( Reset_LCD,2);//Һ����λLCD_Display_CMD
		delay_ms(10);

		
}