#include <stdint.h>
#include <picobit.h>
#include <dispatch.h>
#include <primitives.h>
#include <bignum.h>
#include <stm32f10x.h>
#include <stm32f10x_gpio.h>

PRIMITIVE_UNSPEC(GPIO_config, arch_GPIO_config, 4)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  uint16_t u16_gpiox, u16_mode, u16_pin, u16_speed;

  u16_gpiox = decode_int(arg1);
  u16_mode  = decode_int(arg2);
  u16_pin   = decode_int(arg3);
  u16_speed = decode_int(arg4);

  GPIO_InitStruct.GPIO_Pin   = u16_pin;
  GPIO_InitStruct.GPIO_Mode  = u16_mode;
  GPIO_InitStruct.GPIO_Speed = u16_speed;

  if (u16_gpiox == 0){
    GPIO_Init(GPIOA, &GPIO_InitStruct);
  }
  else if (u16_gpiox == 1){
    GPIO_Init(GPIOB, &GPIO_InitStruct);
  }
  else if (u16_gpiox == 2){
    GPIO_Init(GPIOC, &GPIO_InitStruct);
  }
  else if (u16_gpiox == 3){
    GPIO_Init(GPIOD, &GPIO_InitStruct);
  }
  else if (u16_gpiox == 4){
    GPIO_Init(GPIOE, &GPIO_InitStruct);
  }
}

PRIMITIVE(#%IO_read, arch_IO_read, 2)
{
  GPIO_TypeDef* GPIOx; 
  uint8_t  u8_gpiox;
  uint16_t u16_pin;

  u8_gpiox = decode_int(arg1);
  u16_pin  = decode_int(arg2);
  GPIOx = GPIOA;
  if(u8_gpiox == 0){
    GPIOx = GPIOA;
  }
  else if(u8_gpiox == 1){
    GPIOx = GPIOB;
  }
  else if(u8_gpiox == 2){
    GPIOx = GPIOC;
  }
  else if(u8_gpiox == 3){
    GPIOx = GPIOD;
  }
  else if(u8_gpiox == 4){
    GPIOx = GPIOE;
  }
  
  if(GPIOx->IDR & u16_pin){
    arg1 = OBJ_TRUE;
  }
  else{
    arg1 = OBJ_FALSE;
  }
}

PRIMITIVE_UNSPEC(#%IO_write, arch_IO_write, 3)
{
  GPIO_TypeDef* GPIOx; 
  uint8_t  u8_gpiox;
  uint16_t u16_pin;

  u8_gpiox = decode_int(arg1);
  u16_pin  = decode_int(arg2);
  
  GPIOx = GPIOA;
  if(u8_gpiox == 0){
    GPIOx = GPIOA;
  }
  else if(u8_gpiox == 1){
    GPIOx = GPIOB;
  }
  else if(u8_gpiox == 2){
    GPIOx = GPIOC;
  }
  else if(u8_gpiox == 3){
    GPIOx = GPIOD;
  }
  else if(u8_gpiox == 4){
    GPIOx = GPIOE;
  }

  if(arg3 == OBJ_FALSE){
    GPIOx->ODR &= ~u16_pin;
  }
  else {
    GPIOx->ODR |= u16_pin;
  }
}

PRIMITIVE_UNSPEC(AFIO_remap, arch_AFIO_remap, 2)
{
  uint16_t u16_afioH, u16_afioL;
  uint32_t u32_afio;

  u16_afioH = decode_int(arg1);
  u16_afioL = decode_int(arg2);
  u32_afio = (u16_afioH << 16) + u16_afioL;
  
  GPIO_PinRemapConfig(u32_afio, ENABLE);
}

