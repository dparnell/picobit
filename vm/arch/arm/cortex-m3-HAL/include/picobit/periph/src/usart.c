#include <stdint.h>
#include <picobit.h>
#include <dispatch.h>
#include <primitives.h>
#include <bignum.h>
#include <stm32f10x.h>
#include <stm32f10x_usart.h>
#include <misc.h>
#include <usart.h>

#include <frames.h>
#include <crc16.h>

uint16_t crc;
volatile uint8_t buffer[12+1];
volatile t_frame frame;
volatile t_frame_response frame_response;

PRIMITIVE_UNSPEC(#%UART_putByte, arch_UART_putByte, 1)
{
  uint8_t u8_byte = decode_int(arg1);
  while( !(USART1->SR & 0x00000040) ); 
  USART1->DR = u8_byte;
}

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
  USART_SendData(USARTx, FRAME_FLAG);
}

void USART_frame_response(USART_TypeDef* USARTx)
{
  uint8_t buffer_out[FRAME_MAX+1];
  uint8_t cnt_out = 0;
  
  while( !(USARTx->SR & 0x00000040) );
  USARTx->DR = frame_response.src;
  buffer_out[cnt_out] = frame_response.src;

  while( !(USARTx->SR & 0x00000040) );
  USARTx->DR = frame_response.dst;
  cnt_out += 1;
  buffer_out[cnt_out] = frame_response.dst;

  while( !(USARTx->SR & 0x00000040) );
  USARTx->DR = frame_response.read_write;
  cnt_out += 1;
  buffer_out[cnt_out] = frame_response.read_write;
  
  while( !(USARTx->SR & 0x00000040) );
  USARTx->DR = frame_response.periph;
  cnt_out += 1;
  buffer_out[cnt_out] = frame_response.periph;


  if(frame_response.periph == f_IO){
    while( !(USARTx->SR & 0x00000040) );
    USARTx->DR = frame_response.point.t_IO.gpiox;
    cnt_out += 1;
    buffer_out[cnt_out] = frame_response.point.t_IO.gpiox;

    while( !(USARTx->SR & 0x00000040) );
    USARTx->DR = frame_response.point.t_IO.pinH;
    cnt_out += 1;
    buffer_out[cnt_out] = frame_response.point.t_IO.pinH;

    while( !(USARTx->SR & 0x00000040) );
    USARTx->DR = frame_response.point.t_IO.pinL;
    cnt_out += 1;
    buffer_out[cnt_out] = frame_response.point.t_IO.pinL;

    while( !(USARTx->SR & 0x00000040) );
    USARTx->DR = frame_response.point.t_IO.in_out;
    cnt_out += 1;
    buffer_out[cnt_out] = frame_response.point.t_IO.in_out;

    while( !(USARTx->SR & 0x00000040) );
    USARTx->DR = frame_response.point.t_IO.value;
    cnt_out += 1;
    buffer_out[cnt_out] = frame_response.point.t_IO.value;
  }

  //others peripherals

  while( !(USARTx->SR & 0x00000040) );
  USARTx->DR = frame_response.valueH;
  cnt_out += 1;
  buffer_out[cnt_out] = frame_response.valueH;
  
  while( !(USARTx->SR & 0x00000040) );
  USARTx->DR = frame_response.valueL;
  cnt_out += 1;
  buffer_out[cnt_out] = frame_response.valueL;

  //IO_size + 2 => valueH and valueL
  frame_response.crc = crc16_calc(buffer_out, (IO_size+2));
  
  while( !(USARTx->SR & 0x00000040) );
  USARTx->DR = (frame_response.crc >> 8);

  //aux = frame_response.crc & 0xFF;
  while( !(USARTx->SR & 0x00000040) );
  USARTx->DR = frame_response.crc & 0xFF;
  

  while( !(USARTx->SR & 0x00000040) );
  USARTx->DR = FRAME_FLAG;
}

uint8_t decode_io_read()
{
  GPIO_TypeDef* GPIOx; 
  uint16_t pinx;
  uint8_t buffer_in[FRAME_MAX+1], i;
  
  frame.pos = buffer_next(frame.pos);
  frame.point.t_IO.gpiox = buffer[frame.pos];

  frame.pos = buffer_next(frame.pos);
  frame.point.t_IO.pinH = buffer[frame.pos];

  frame.pos = buffer_next(frame.pos);
  frame.point.t_IO.pinL = buffer[frame.pos];

  frame.pos = buffer_next(frame.pos);
  frame.point.t_IO.in_out = buffer[frame.pos];

  frame.pos = buffer_next(frame.pos);
  frame.point.t_IO.value = buffer[frame.pos];

  frame.pos = buffer_next(frame.pos);
  frame.crc = (buffer[frame.pos] << 8);

  frame.pos = buffer_next(frame.pos);
  frame.crc |= (buffer[frame.pos]);
  
  for(i = 0; i < IO_size; i++){
    buffer_in[i] = buffer[i];
  }
  
  crc = crc16_calc(buffer_in, IO_size);
  if(frame.crc != crc)
  {
    return 0;
  }
  
  frame_response.point.t_IO.gpiox  = frame.point.t_IO.gpiox;
  frame_response.point.t_IO.pinH   = frame.point.t_IO.pinH;
  frame_response.point.t_IO.pinL   = frame.point.t_IO.pinL;
  frame_response.point.t_IO.in_out = frame.point.t_IO.in_out;
  frame_response.point.t_IO.value  = frame.point.t_IO.value;
  
  pinx = (frame.point.t_IO.pinH << 8) + frame.point.t_IO.pinL;

  GPIOx = GPIOA;
  if(frame.point.t_IO.gpiox == 0){
    GPIOx = GPIOA;
  }
  else if(frame.point.t_IO.gpiox == 1){
    GPIOx = GPIOB;
  }
  else if(frame.point.t_IO.gpiox == 2){
    GPIOx = GPIOC;
  }
  else if(frame.point.t_IO.gpiox == 3){
    GPIOx = GPIOD;
  }
  else if(frame.point.t_IO.gpiox == 4){
    GPIOx = GPIOE;
  }
  
  if(frame.point.t_IO.in_out == p_IN){
    frame_response.valueL = ((GPIOx->IDR & pinx) != 0) ? 1 : 0;
  }
  else{
    frame_response.valueL = ((GPIOx->ODR & pinx) != 0) ? 1 : 0;
  }

  frame_response.valueH = 0;

  USART_frame_response(USART1);

  return 1;
}

uint8_t decode_io_write()
{
  GPIO_TypeDef* GPIOx; 
  uint16_t pinx;
  uint8_t buffer_in[FRAME_MAX+1], i;
  
  frame.pos = buffer_next(frame.pos);
  frame.point.t_IO.gpiox = buffer[frame.pos];

  frame.pos = buffer_next(frame.pos);
  frame.point.t_IO.pinH = buffer[frame.pos];

  frame.pos = buffer_next(frame.pos);
  frame.point.t_IO.pinL = buffer[frame.pos];

  frame.pos = buffer_next(frame.pos);
  frame.point.t_IO.in_out = buffer[frame.pos];

  frame.pos = buffer_next(frame.pos);
  frame.point.t_IO.value = buffer[frame.pos];
  
  frame.pos = buffer_next(frame.pos);
  frame.crc = (buffer[frame.pos] << 8);

  frame.pos = buffer_next(frame.pos);
  frame.crc |= (buffer[frame.pos]);
  
  for(i = 0; i < IO_size; i++){
    buffer_in[i] = buffer[i];
  }
  
  crc = crc16_calc(buffer_in, IO_size);
  if(frame.crc != crc)
  {
    return 0;
  }
  
  frame_response.point.t_IO.gpiox  = frame.point.t_IO.gpiox;
  frame_response.point.t_IO.pinH   = frame.point.t_IO.pinH;
  frame_response.point.t_IO.pinL   = frame.point.t_IO.pinL;
  frame_response.point.t_IO.in_out = frame.point.t_IO.in_out;
  frame_response.point.t_IO.value  = frame.point.t_IO.value;
  
  pinx = (frame.point.t_IO.pinH << 8) + frame.point.t_IO.pinL;

  GPIOx = GPIOA;
  if(frame.point.t_IO.gpiox == 0){
    GPIOx = GPIOA;
  }
  else if(frame.point.t_IO.gpiox == 1){
    GPIOx = GPIOB;
  }
  else if(frame.point.t_IO.gpiox == 2){
    GPIOx = GPIOC;
  }
  else if(frame.point.t_IO.gpiox == 3){
    GPIOx = GPIOD;
  }
  else if(frame.point.t_IO.gpiox == 4){
    GPIOx = GPIOE;
  }

  if(frame.point.t_IO.in_out == p_OUT){
    if(frame.point.t_IO.value == 0){
      GPIOx->ODR &= ~pinx;
    }
    else{
      GPIOx->ODR |= pinx;
    }
    frame_response.valueL = ((GPIOx->ODR & pinx) != 0) ? 1 : 0;
  }

  frame_response.valueH = 0;

  USART_frame_response(USART1);

  return 1;
}


uint8_t decode_frame()
{
  frame.pos = 0;
  frame.src = buffer[frame.pos];
  
  frame.pos = buffer_next(frame.pos);
  frame.dst = buffer[frame.pos];

  frame.pos = buffer_next(frame.pos);
  frame.read_write = buffer[frame.pos];
  
  frame.pos = buffer_next(frame.pos);
  frame.periph = buffer[frame.pos];

  if(frame.dst != my_add){
    return 0;
  }

  frame_response.pos        = 0;
  frame_response.src        = my_add;
  frame_response.dst        = frame.src;
  frame_response.read_write = frame.read_write;
  frame_response.periph     = frame.periph;
  
  switch(frame.periph){
  case f_IO:
    if(frame.read_write == o_READ)
    {
      decode_io_read();
    }
    else if(frame.read_write == o_WRITE)
    {
      decode_io_write();
    }
    
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
    
    if( (t != FRAME_FLAG) && (cnt < FRAME_MAX) ){ 
      buffer[cnt] = t;
      cnt++;
    }
    else{ 
      cnt = 0;
      decode_frame();
    }
  }
}

void usart_init(uint32_t baudrate)
{
  init_USART1(baudrate); 
}
