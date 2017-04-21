#include <stdint.h>
#include <picobit.h>
#include <dispatch.h>
#include <primitives.h>
#include <bignum.h>
#include <stm32f10x.h>
#include <stm32f10x_usart.h>
#include <misc.h>
#include "usart.h"

#define FRAME_FLAG   0x7E
#define FRAME_MAX    12
volatile uint8_t frame[FRAME_MAX+1];
volatile uint8_t frame_response[FRAME_MAX+1];

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

void USART_puts(USART_TypeDef* USARTx, volatile uint8_t *s)
{
  while(*s != FRAME_FLAG){
    while( !(USARTx->SR & 0x00000040) ); 
    //USART_SendData(USARTx, *s);
    USARTx->DR = *s;
    s++;
  }
  //USART_SendData(USARTx, FRAME_FLAG);
}

void decode_io()
{
  GPIO_TypeDef* GPIOx; 
  uint8_t gpiox, pinH, pinL, in_out;
  uint16_t pinx;
 
  gpiox  = frame[3];
  pinH   = frame[4];
  pinL   = frame[5];
  in_out = frame[6];

  frame_response[3] = gpiox;
  frame_response[4] = pinH;
  frame_response[5] = pinL;

  pinx = (pinH << 8) + pinL;
  
  GPIOx = GPIOA;
  if(gpiox == 0){
    GPIOx = GPIOA;
  }
  else if(gpiox == 1){
    GPIOx = GPIOB;
  }
  else if(gpiox == 2){
    GPIOx = GPIOC;
  }
  else if(gpiox == 3){
    GPIOx = GPIOD;
  }
  else if(gpiox == 4){
    GPIOx = GPIOE;
  }
  
  if(in_out == p_IN){
    frame_response[6] = ((GPIOx->IDR & pinx) != 0) ? 1 : 0;
  }
  else{
    frame_response[6] = ((GPIOx->ODR & pinx) != 0) ? 1 : 0;
  }

  frame_response[7] = FRAME_FLAG;
}

uint8_t decode_frame()
{
  uint8_t src, dst, periph;

  src    = frame[0];
  dst    = frame[1];
  periph = frame[2];

  if(dst != my_add){
    return 0;
  }

  frame_response[0] = my_add;
  frame_response[1] = src;
  frame_response[2] = periph;
  
  switch(periph){
  case f_IO:
    decode_io();
    break;
    
  case f_ADC:
    //decode_adc();
    break;

  case f_PWM:
    //decode_pwm();
    break;

  case f_DAC:
    //decode_dac();
    break;

  default:
    return 0;    
  }

  return 1;
}

void USART1_IRQHandler(void)
{
  if( USART_GetITStatus(USART1, USART_IT_RXNE) ){
    
    static uint8_t cnt = 0;
    uint8_t t = USART1->DR;
    
    if( (t != 0x7e) && (cnt < FRAME_MAX) ){ 
      frame[cnt] = t;
      cnt++;
    }
    else{ 
      cnt = 0;
      if (decode_frame()){
	USART_puts(USART1, frame_response);
      }
    }
  }
}

void usart_init(uint32_t baudrate)
{
  init_USART1(baudrate); 
}
