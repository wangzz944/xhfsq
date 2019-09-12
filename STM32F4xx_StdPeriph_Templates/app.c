
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "includes.h"
#include "usart.h"

/* Private define ------------------------------------------------------------*/


# if 0
void test(void)
{
	if(1 == (USART_RX_STA&0x8000))
	{
		USART_RX_STA = 0;
		if ((0xEB != USART_RX_BUF[0]) || (0x90 != USART_RX_BUF[1]) || (0x0D != USART_RX_BUF[14]) || (0x0A != USART_RX_BUF[15]))
			return;
		switch(USART_RX_BUF[2])
		{
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				break;
		}	
	}
}
#endif