#include "public.h"
#include "printf.h"
#include "systick.h"
#include "relay.h"
#include "SPI.h"
#include "Fun.h"
#include "time.h"
#include "LED.h"
#include "adc.h"
/**********************全局变量定义并初始化**************************/
//
unsigned int  Flag_HMICMD = 0;
unsigned int  Flag_UART1 = 0;

unsigned char glRecuart3Buff[10] = {0};
unsigned char flagHMI = RecSuccess;
unsigned char FlagTaskHMI = 0;
unsigned int flag_YN = 0;


/*********************共有函数初始化**************************/
void Base_INI(void)
{
	 //系统所用三个串口初始化
	SystemInit();	//时钟初始化
	USART1_INI(115200);  	
	USART2_INI(115200);
	USART3_INI(115200);					//根据串口屏的要求配置为9600
	time_init();
	GPIO_initial();
		/* enable adc1 and config adc1 to dma mode */
	Init_ADC_Periph();
}
/**********************16进制转字符串***************************/
int HexToString(unsigned char *psHex, char *pszStr,
                unsigned int uiHexLen, unsigned int uiBytesPerLine)
{
    int i = 0;
    int j = 0;

    if (psHex == NULL || pszStr == NULL) {
        return -1;
    }

/////////////////////////////////////////////////////////
    while (i < uiHexLen)
		{
        sprintf(pszStr+j, "%02X ", psHex[i]);
        i++;
        j += 3;
    }


    return 0;
}

