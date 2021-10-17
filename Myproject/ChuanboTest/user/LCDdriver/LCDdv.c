#include "LCDdv.h"
#include "printf.h"
#include "systick.h"

/*==========================================================================
Function:USART3_Sned_Char_Buff();
Description:液晶字符显示
写一下心得：
封装库函数的意义和作用在于什么
在一个文件中封装库函数，用于在主函数中被调用
===========================================================================*/
/*================液晶控制指令字==============*/
uint8_t __IP[15];

uint8_t Reset_Fback=0x55;	//液晶复位
uint8_t Self_Chack_Fback=0x55;	//
uint8_t Reset_LCD[]={0xaa,0x01};	//液晶复位
uint8_t Clear_LCD[]={0xaa,0x10};	//液晶清屏
uint8_t CSR_Pos[]={0xaa,0x20};		//光标位置设置指令，后续发送数据

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
Description:LCD光标位置设置，
===========================================================================*/
static void CSRPos_set(uint8_t Line,uint8_t Rank)  		//共有两个字节的指令需要完成，除了液晶亮度和数值显示
{  
		USART3_Send_Char_Buff( CSR_Pos,2);//液晶复位LCD_Display_CMD
		USART3_Send_Char(Line);//液晶复位LCD_Display_CMD
		USART3_Send_Char(Rank);//液晶复位LCD_Display_CMD
		delay_ms(1);  
		
}
/*==========================================================================
Function:LCD_display();
Description:LCD字符显示  LCD_Display_CMD
===========================================================================*/
void LCD_display(uint8_t _parm[],uint8_t _Comand[],uint8_t len,uint8_t Line,uint8_t Rank)  
{  
		CSRPos_set(Line,Rank);
		USART3_Send_Char_Buff( _Comand,2);	//命令发送
		USART3_Send_Char_Buff( _parm,len);  //uint8_t Display_Char//"NORMAL" 
		USART3_Send_Char(0x0d);							//数据发送最后要以0x0d结尾
		delay_us(10);  	
}

/*==========================================================================
Function:USART3_Sned_Char_Buff();
Description:LCD清屏
===========================================================================*/
void LCD_Comand(uint8_t _Comand[])  		//共有两个字节的指令需要完成，除了液晶亮度和数值显示
{  
		USART3_Send_Char_Buff( _Comand,2);//液晶复位LCD_Display_CMD
		delay_ms(1);		
}


/*==========================================================================
Function:IP_display();
Description:LCD清屏
===========================================================================*/
void IP_display(void)  		//共有两个字节的指令需要完成，除了液晶亮度和数值显示
{  
				uint8_t i;
	
				for(i=0;i<15;i++)
				{
					if(__IP[i]!=0x0d)
					{
						__IP[i]=RxBuffer1[i+3];
					} else
					{
						__IP[i]=0x0d;					//此处可使数据显示为空白
					}
				}
				LCD_Comand(Clear_LCD);	
			//	CSRPos_set(0,2);
				LCD_display(__IP,LCD_Display_CMD,15,0,0);			//在液晶屏显示“NORMAL”
				LCD_display(NORMAL,LCD_Display_CMD,6,1,0);
}
