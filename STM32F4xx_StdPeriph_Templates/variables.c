

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "includes.h"

/* Global variables ---------------------------------------------------------*/
unsigned char USART_RXBuf[2][400] = {0}; 
unsigned int USART_ContIndex = 0;
unsigned int USART_BufIndex = 0;
unsigned int OutPut_count = 0;
unsigned int OutPut_flag = 0;
unsigned int DAC_out[4][20] = {0}; 

