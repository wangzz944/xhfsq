/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/main.c 
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   Main program body
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
#include "main.h"
#include "stm32f4xx.h"
#include "stdio.h"
#include "stm32f4xx_spi.h"
#include "lcd.h"
#include "stm32f4xx_tim.h"
//#include "includes.h"

/** @addtogroup Template_Project
  * @{
  */ 

unsigned char ParamBuf[20] = {0};



/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define USARTx_TX_GPIO_PORT   GPIOA   
#define USARTx_RX_GPIO_PORT   GPIOA
#define USARTx_TX_PIN         GPIO_Pin_9  
#define USARTx_RX_PIN         GPIO_Pin_10

#define SPI_CS_ENABLE         GPIO_ResetBits(GPIOA,GPIO_Pin_4);
//#define SPI_CS_ENABLE         GPIO_ResetBits(GPIOB,GPIO_Pin_12);
#define SPI_CS_DISABLE        GPIO_SetBits(GPIOA,GPIO_Pin_4);


#define SPI_RS_LOW            GPIO_ResetBits(GPIOC,GPIO_Pin_8);
#define SPI_RS_HIGH           GPIO_SetBits(GPIOC,GPIO_Pin_8);

#define SPI_LDAC_LOW            GPIO_ResetBits(GPIOC,GPIO_Pin_10);
#define SPI_LDAC_HIGH           GPIO_SetBits(GPIOC,GPIO_Pin_10);


 #define CS_PIN                          GPIO_Pin_4
 #define LDC_PIN                         GPIO_Pin_10

		

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t uwTimingDelay;
RCC_ClocksTypeDef RCC_Clocks;

/* Private function prototypes -----------------------------------------------*/
//static void Delay(__IO uint32_t nTime);


void TIM2_Init(void)
{
		TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  ///??TIM9??        
		TIM_TimeBaseInitStructure.TIM_Period = 10 - 1 ;         //??????
		TIM_TimeBaseInitStructure.TIM_Prescaler=(168 -1);  //?????
		TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //??????
		TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
		TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//???TIM9
		TIM_ClearFlag(TIM2,TIM_FLAG_Update);
		TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //?????9????

		NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;//???9??
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00; //?????0
		NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //????3
		NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
		NVIC_Init(&NVIC_InitStructure);        
	
		TIM_Cmd(TIM2,ENABLE);
}

void SPI_Configuration(void)
{

		//PA4,PA5,P46,PA7
		SPI_InitTypeDef  SPI_InitStructure;
		GPIO_InitTypeDef  GPIO_InitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);//??SPI_1??
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC ,ENABLE);	


		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
		GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6 | GPIO_Pin_5;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_Init(GPIOA,&GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
		GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_Init(GPIOA,&GPIO_InitStructure);
		
		//GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_SPI1);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);
		
		SPI_I2S_DeInit(SPI1);

		SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
		SPI_InitStructure.SPI_CRCPolynomial = 7;
		SPI_Init(SPI1,&SPI_InitStructure);
		
		//SPI_SSOutputCmd(SPI1, ENABLE);
		SPI_Cmd(SPI1,ENABLE);
		SPI_CS_DISABLE;
		
//zjf 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);	
#if 0

			//PA4,PA5,P46,PA7
		SPI_InitTypeDef  SPI_InitStructure;
		GPIO_InitTypeDef  GPIO_InitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI4,ENABLE);//??SPI_1??
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOA  ,ENABLE);	
	
		GPIO_PinAFConfig(GPIOE, GPIO_PinSource12, GPIO_AF_SPI4);
		GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_SPI4);

		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
		GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_14;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_Init(GPIOE,&GPIO_InitStructure);
		
		SPI_I2S_DeInit(SPI4);
		
		SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
		SPI_InitStructure.SPI_CRCPolynomial = 7;
		SPI_Init(SPI4,&SPI_InitStructure);
		SPI_Cmd(SPI4,ENABLE);
			
		
		//SPIµÄCSÒý½Å
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
		GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_Init(GPIOE,&GPIO_InitStructure);
		
			//SPIµÄCSÒý½Å
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
		GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_Init(GPIOA,&GPIO_InitStructure);
		
		
				//SPIµÄCSÒý½Å
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
		GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_Init(GPIOA,&GPIO_InitStructure);
		
#endif				
}

void Signal_Sendout(char* Databuf, char* SendFlag)
{
		char FlagLoop = 0;
	
		for(FlagLoop=0;FlagLoop<4;FlagLoop++)
		{
				if(SendFlag[FlagLoop] == 1)
				{
						//SPI_I2S_SendData(Databuf);
				}	
		}
}	

void LCD_Play_CFGInfrom(char* Databuf, char* ConfFlag)
{
		char FlagLoop = 0;
	
		for(FlagLoop=0;FlagLoop<4;FlagLoop++)
		{
				if(ConfFlag[FlagLoop] == 1)
				{
						//LCD_SHOWS_Inform(Databuf);
				}	
		}
}	

unsigned int GetDivNumber(unsigned int uiTime_Amp_Number)
{/*½«20000×÷ÎªÒ»¸öÊ±¼ä»òÕßµçÑ¹µÄµ¥Î»*/
		uiTime_Amp_Number = (uiTime_Amp_Number/20000)*20000;
	
		return uiTime_Amp_Number ;
}


void Key_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef   EXTI_InitStructure;
  NVIC_InitTypeDef   NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  
	//key1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//key2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	
	//key3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	
	//key4
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	
	//key5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	
	//key6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	/* Connect EXTI Line3 to PA3 pin */
  //SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource3);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource0);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource14);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource3);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource15);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource13);
	
  /* Configure EXTI Line3 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line3;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource0|EXTI_PinSource14|EXTI_PinSource3|EXTI_PinSource15|EXTI_PinSource13);
		  
		/* Configure EXTI Line0 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	 
	 /* Configure EXTI Line14 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line14;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	  
		/* Configure EXTI Line15 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line15;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	  
		/* Configure EXTI Line13 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line13;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	
	 /* Enable and set EXTI Line0 Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	/* Enable and set EXTI Line3 Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0e;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	/* Enable and set EXTI Line0 Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0d;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}	


/* Private functions ---------------------------------------------------------*/
void USART_Config(void)
{

	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	/* ??GPIO_D??? */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	/* ????3??? */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);


	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	USART_InitStructure.USART_BaudRate   = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits   = USART_StopBits_1;
	USART_InitStructure.USART_Parity     = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;

	USART_Init(USART1, &USART_InitStructure);

	/* ????3 */
	USART_Cmd(USART1, ENABLE);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
		/* Enable and set USART1 Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0a;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}

void delay_nop(void)
{
	#if 0
		unsigned int i = 0;
		for (i=0;i<2;i++)
		{
				__NOP();
		}
	#else
	//delay_us(1);
	 delay_us(10);
	 //delay_us(30);
	//delay_us(200);
	//delay_us(100);
	//delay_ms(1);
	#endif
}


void test_dac(uint16_t Data)
{
			uint16_t val = 0;
	#if 0
			val = Data;
			val &= 0x3FFF;
			val |= 0x0000;
		
			SPI_LDAC_HIGH;
			SPI_CS_ENABLE;
			while(0 == SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE));
			SPI_I2S_SendData(SPI1,val);		//00
			//1110 0000 0000 0000
			while(0 == SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE));
			delay_nop();
			SPI_CS_DISABLE;
			SPI_LDAC_LOW;
	delay_nop();
			//delay_ms(20);
	
	
			val = Data;
			val &= 0x3FFF;
			val |= 0x4000;
			
			SPI_LDAC_HIGH;
			SPI_CS_ENABLE;
			while(0 == SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE));
			SPI_I2S_SendData(SPI1,val);		//01
			//1110 0000 0000 0000
			while(0 == SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE));
			delay_nop();
			SPI_CS_DISABLE;
			//SPI_LDAC_LOW; //zjf
				delay_nop();
			//delay_ms(20);
			
			val = Data;
		  val &= 0x3FFF;
			val |= 0x8000;
			SPI_LDAC_HIGH;
			SPI_CS_ENABLE;
			while(0 == SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE));
			SPI_I2S_SendData(SPI1,val);		//10
			//1110 0000 0000 0000
			while(0 == SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE));
			/delay_nop();
			SPI_CS_DISABLE;
			SPI_LDAC_LOW;
				delay_nop();
			//delay_ms(20);
		#endif	
//½öµ÷ÊÔµÚ4Â·¡		
			val = Data;
			val &= 0x3FFF;
			val |= 0xC000;
			
			SPI_LDAC_HIGH;
			SPI_CS_ENABLE;
			
			// GPIOA->BSRRL = CS_PIN;
      // GPIOC->BSRRH = LDC_PIN;
			//while(0 == SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE));
			SPI_I2S_SendData(SPI1,val);			//11
			//1110 0000 0000 0000
			//while(0 == SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE));
		 	delay_nop();
			SPI_CS_DISABLE;
			SPI_LDAC_LOW;
			
			#if 0
			 GPIOA->BSRRH = CS_PIN;
			 	__NOP();
				__NOP();
			 GPIOC->BSRRL = LDC_PIN;
			 #endif
			//delay_nop();
			
			#if 0
				__NOP();
				__NOP();
				__NOP();
				__NOP();
				__NOP();
				__NOP();
				__NOP();
				__NOP();
				__NOP();
				__NOP();
				__NOP();
				SPI_LDAC_HIGH;
			#endif
				//delay_nop();
}


/**
  * @brief  Main program
  * @param  None
  * @retval None
  */


int main(void)
{
	uint32_t count = 0;
		delay_init();
		
	
		USART_Config();
	
	SPI_Configuration();

	//USART_SendData(USART1,0x40);

	//	SPI_I2S_SendData(SPI1,0xFFFF);
	//SPI_I2S_SendData(SPI1,0xDFFF);

		LCD_Init();		
		
		Key_Config();
		TIM2_Init();

	
	 LCD_play_init();
	
	 LCD_ShowString(60,50,200,16,16,"AZGK");
	
	
	SPI_RS_LOW;
	delay_ms(5);
	SPI_RS_HIGH;
	
		while (1)
		{
			//signal sends out from requirement
			//Signal_Sendout();
			
//			LCD shows the signal output information
			//LCD_Play_CFGInfrom();

//			test_dac(0xccd);
//			test_dac(0x199a);
//			test_dac(0x2667);
//			//delay_nop();
//			test_dac(0x3334);
//			test_dac(0x3fff);
		

//D
//			SPI_CS_ENABLE;
//			SPI_LDAC_HIGH;
		 GPIOA->BSRRL = CS_PIN;
						 	__NOP();
				__NOP();
     GPIOC->BSRRH = LDC_PIN;
		 			 	__NOP();
				__NOP();
			SPI_I2S_SendData(SPI1,0xd000);			

				delay_nop();
			 GPIOA->BSRRH = CS_PIN;
			 	__NOP();
				__NOP();
			 GPIOC->BSRRL = LDC_PIN;
			 			 	__NOP();
				__NOP();
//			SPI_CS_DISABLE;
//			SPI_LDAC_LOW;			
		
#if 0
//			SPI_CS_ENABLE;
//			SPI_LDAC_HIGH;
		  GPIOA->BSRRL = CS_PIN;
						 	__NOP();
				__NOP();
     GPIOC->BSRRH = LDC_PIN;
		 			 	__NOP();
				__NOP();
			SPI_I2S_SendData(SPI1,0xffff);			

				delay_nop();
			 GPIOA->BSRRH = CS_PIN;
			 	__NOP();
				__NOP();
			 GPIOC->BSRRL = LDC_PIN;			 	__NOP();
				__NOP();
//			SPI_CS_DISABLE;
//			SPI_LDAC_LOW;
#endif
		}
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (uwTimingDelay != 0x00)
  { 
    uwTimingDelay--;
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
