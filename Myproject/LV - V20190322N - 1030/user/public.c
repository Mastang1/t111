#include "public.h"
#include "printf.h"
#include "systick.h"
#include "relay.h"
#include "SPI.h"
#include "Fun.h"
#include "time.h"
#include "LED.h"
#include "adc.h"
/**********************ȫ�ֱ������岢��ʼ��**************************/
//
unsigned int  Flag_HMICMD = 0;
unsigned int  Flag_UART1 = 0;

unsigned char glRecuart3Buff[10] = {0};
unsigned char flagHMI = RecSuccess;
unsigned char FlagTaskHMI = 0;
unsigned int flag_YN = 0;


/*********************���к�����ʼ��**************************/
void Base_INI(void)
{
	 //ϵͳ�����������ڳ�ʼ��
	SystemInit();	//ʱ�ӳ�ʼ��
	USART1_INI(115200);  	
	USART2_INI(115200);
	USART3_INI(115200);					//���ݴ�������Ҫ������Ϊ9600
	time_init();
	GPIO_initial();
		/* enable adc1 and config adc1 to dma mode */
	Init_ADC_Periph();
}
/**********************16����ת�ַ���***************************/
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

