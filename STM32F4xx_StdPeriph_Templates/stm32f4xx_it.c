/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "main.h"
#include "includes.h"

/*
PC0 : 按键上
PC14 : 按键下
PC3 : 按键数值增加
PC15 : 按键数值减少
PC13 : 按键使能/失能输出
*/

#define PI 		200

#define Button_Up					Gpio_Pin_0
#define Button_Down				Gpio_Pin_14
#define Button_Add				Gpio_Pin_3
#define Button_Sub				Gpio_Pin_15
#define Button_Enable_DIS	Gpio_Pin_13
#define DAC_accuracy 16384
#define BITSET1(n)				(1<<n)
#define BITRESET0(n)			(0<<n)


#define SET_BIT(val, bit) ((val) | (0x1 << (bit)))
#define CLR_BIT(val, bit) ((val) & ~(0x1 << (bit)))

#define Channel_length  	80



int channel = 0;	        //4路通道号码
unsigned int OUTPUT_ENABLE = 0;				//使能通道
unsigned int TIME_NUMBER = 0;



/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  TimingDelay_Decrement();
}

void EXTI0_IRQHandler(void)
{

	if(EXTI_GetITStatus(EXTI_Line0) != RESET)
	{/*通道增加*/
			EXTI_ClearITPendingBit(EXTI_Line0); 		
			channel++;
			if(channel >= 40)
			{
				channel = 0 ;
			}
	}
}
/*增加1或者减少1对应时间是10微秒，或者电压是10mv*/
void EXTI3_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line3) != RESET)
	{
			EXTI_ClearITPendingBit(EXTI_Line3); 	
		
			DAC_out[channel/10][channel%10] += 1;
	}
}
void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line13) != RESET)
	{/*输出使能/失能*/
		EXTI_ClearITPendingBit(EXTI_Line13); 
		OUTPUT_ENABLE = ~OUTPUT_ENABLE;
		if(OUTPUT_ENABLE == 1)
		{
				TIME_NUMBER = 0;
		}
		
	}
	if(EXTI_GetITStatus(EXTI_Line14) != RESET)
	{/*通道减少*/
			EXTI_ClearITPendingBit(EXTI_Line14); 	
			channel--;
			if(channel < 0)
			{
				channel = 40 ;
			}
	}
	if(EXTI_GetITStatus(EXTI_Line15) != RESET)
	{
			EXTI_ClearITPendingBit(EXTI_Line15); 	

			DAC_out[channel/10][channel%10] -= 1;

	}
}

void USARTx_IRQHandler(void)
{
	uint32_t channel = 0;
	uint32_t count = 0;
	uint32_t offset = 0;
  /* USART in Receiver mode */
  if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
  {
		  USART_ClearFlag(USART1,USART_FLAG_RXNE);
			
			/* Receive Transaction data */
      USART_RXBuf[USART_BufIndex][USART_ContIndex++] = USART_ReceiveData(USART1);
		
			if((USART_RXBuf[USART_BufIndex][0] !=0xEB) && (USART_RXBuf[USART_BufIndex][1] != 0x90) && (USART_ContIndex == 2))
			{
					USART_ContIndex = 0;
			}
			if(USART_RXBuf[USART_BufIndex][USART_ContIndex-1] ==0x0d && USART_RXBuf[USART_BufIndex][USART_ContIndex-2] == 0x0a)
			{
					USART_ContIndex = 0;
					OutPut_count = USART_BufIndex;
					OutPut_flag = 1;
				
				/*串口协议   头  OX48 90   尾：0X0D 0A
				每一通道有82个字节，其中1个字节的通道号   一个字节的结尾标志 0xFF*/
				if((USART_RXBuf[USART_BufIndex][2] == 0x01)&&(USART_RXBuf[USART_BufIndex][83] == 0xFF)&&
					(USART_RXBuf[USART_BufIndex][84] == 0x02)&&(USART_RXBuf[USART_BufIndex][165] == 0xFF)&&
					(USART_RXBuf[USART_BufIndex][166] == 0x03)&&(USART_RXBuf[USART_BufIndex][247] == 0xFF)&&
					(USART_RXBuf[USART_BufIndex][248] == 0x04)&&(USART_RXBuf[USART_BufIndex][329] == 0xFF))
				{
					for(channel = 0;channel < 4 ; channel++)
					{
						offset = 3 + 82*channel ;
						for(count = 0 ; count < 20 ;count++)
						{
								DAC_out[channel][count] = (unsigned int)((USART_RXBuf[USART_BufIndex][offset + count*4] << 24)|(USART_RXBuf[USART_BufIndex][offset + count*4 + 1] << 16)|
								(USART_RXBuf[USART_BufIndex][offset + count*4 + 2] << 8)|(USART_RXBuf[USART_BufIndex][offset + count*4 + 3]));
						}
				}

				}	
				
				
				
					USART_BufIndex++;
					if(USART_BufIndex >= 2)
					{
						USART_BufIndex = 0;
					}	
			}
			else if(USART_ContIndex >= 400)
			{
				USART_ContIndex = 0;
			}			
  }

}
void DAC_Output(unsigned char channel,unsigned char index)
{
		unsigned int output = 0;
		output	= (unsigned int)((float)(DAC_out[channel][index]*0.01)/1.5*DAC_accuracy) ; 
		if(channel == 0)
		{
			output = CLR_BIT(output,14);
			output = CLR_BIT(output,15);
		}
		else if(channel == 1)
		{
		  output = CLR_BIT(output,15);
			output = SET_BIT(output,14);
		}
		else if(channel == 2)
		{
			output = CLR_BIT(output,14);
			output = SET_BIT(output,15);
		}
		else if(channel == 3)
		{
			output = SET_BIT(output,14);
			output = SET_BIT(output,15);
		}
		
		SPI_I2S_SendData(SPI1,(unsigned short int)output);
		return ;
}

void TIM2_IRQHandler(void)        
{
		
	unsigned char channel = 0;
	unsigned char index = 0;
	unsigned int DAC_output = 0;
	unsigned int count = 0;
		if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) 
		{
			TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  
			USART_SendData(USART1,0x40);
			if(OUTPUT_ENABLE == 1)
			{
				/*采用定时器对四个通道进行输出,定时器的时间为10微秒*/
					if(TIME_NUMBER == 0)
					{/*使能按键后的首次输出*/
						for(count = 0;count < 4;count++)
						{
							DAC_Output(count,1);
						}
					}
					
					for(channel = 0;channel < 4;channel++)
					{
							DAC_output = TIME_NUMBER%(DAC_out[channel][0] + DAC_out[channel][2] +DAC_out[channel][4] +DAC_out[channel][6] +DAC_out[channel][8]) ; 
							for(index = 0; index < 10;)
							{
									if(DAC_output == DAC_out[channel][0])
									{
											DAC_Output(index,1);
									}
									else if(DAC_output == (DAC_out[channel][0] + DAC_out[channel][2]))
									{
											DAC_Output(index,3);
									}
									else if(DAC_output == (DAC_out[channel][0] + DAC_out[channel][2] + DAC_out[channel][4]))
									{
											DAC_Output(index,5);
									}
									else if(DAC_output == (DAC_out[channel][0] + DAC_out[channel][2] + DAC_out[channel][4] + DAC_out[channel][6]))
									{
											DAC_Output(index,7);
									}
									else if(DAC_output == (DAC_out[channel][0] + DAC_out[channel][2] + DAC_out[channel][4] + DAC_out[channel][6] + DAC_out[channel][8]))
									{
											DAC_Output(index,9);
									}
									else
									{
										
									}
									index = index + 2;
							}
								
							
					}
				
				TIME_NUMBER++;
			}
			
		}   
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
