#include "LCDdv.h"
#include "printf.h"

/*==========================================================================
Function:USART3_Sned_Char_Buff();
Description:液晶字符显示
写一下心得：
封装库函数的意义和作用在于什么
在一个文件中封装库函数，用于在主函数中被调用
===========================================================================*/
void LCD_display(uint8_t _parm[],uint8_t _Comand[],uint8_t len)  
{  
		USART3_Sned_Char_Buff( Reset_LCD,2);//液晶复位LCD_Display_CMD
		delay_ms(10);
	
		USART3_Sned_Char_Buff( _Comand,2);	//命令发送
		USART3_Sned_Char_Buff( _parm,len);  //uint8_t Display_Char//"NORMAL" 
		delay_ms(10);  
		
}

/*==========================================================================
Function:USART3_Sned_Char_Buff();
Description:LCD清屏
===========================================================================*/
void LCD_display(uint8_t _parm[],uint8_t _Comand[],uint8_t len)  
{  
		USART3_Sned_Char_Buff( Reset_LCD,2);//液晶复位LCD_Display_CMD
		delay_ms(10);

		
}