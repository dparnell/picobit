#include <stdint.h>
#include <picobit.h>
#include <dispatch.h>
#include <primitives.h>
#include <bignum.h>
#include <stm32f10x.h>
#include <stm32f10x_usart.h>
#include <misc.h>

#define MAX_STRLEN 12 // this is the maximum string length of our string in characters
volatile char received_string[MAX_STRLEN+1]; // this will hold the recieved string

void init_USART1(uint32_t baudrate)
{
  GPIO_InitTypeDef GPIO_InitStruct; 
  USART_InitTypeDef USART_InitStruct;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  //TX
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9; 	
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; 	
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStruct);

  //RX
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;	
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStruct);
		
  USART_InitStruct.USART_BaudRate = baudrate;		
  USART_InitStruct.USART_WordLength = USART_WordLength_8b;
  USART_InitStruct.USART_StopBits = USART_StopBits_1;	
  USART_InitStruct.USART_Parity = USART_Parity_No;	
  USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
  USART_Init(USART1, &USART_InitStruct);
  
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  USART_Cmd(USART1, ENABLE);
}

void USART_puts(USART_TypeDef* USARTx, volatile char *s){

  while(*s){
    // wait until data register is empty
    while( !(USARTx->SR & 0x00000040) ); 
    USART_SendData(USARTx, *s);
    *s++;
  }
}

void USART1_IRQHandler(void){
  if( USART_GetITStatus(USART1, USART_IT_RXNE) ){
    
    static uint8_t cnt = 0;
    char t = USART1->DR;
    
    if( (t != '\n') && (cnt < MAX_STRLEN) ){ 
      received_string[cnt] = t;
      cnt++;
    }
    else{ 
      cnt = 0;
      USART_puts(USART1, received_string);
    }
  }
}

void usart_init(uint32_t baudrate) {
  init_USART1(baudrate); 
}
