#include <stdint.h>
#include <picobit.h>
#include <dispatch.h>
#include <primitives.h>
#include <bignum.h>
#include <stm32f10x.h>

/* GPIOx clock enable */
PRIMITIVE_UNSPEC(GPIOA_clock, arch_GPIOA_clock, 0)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
}
PRIMITIVE_UNSPEC(GPIOB_clock, arch_GPIOB_clock, 0)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
}
PRIMITIVE_UNSPEC(GPIOC_clock, arch_GPIOC_clock, 0)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
}
PRIMITIVE_UNSPEC(GPIOD_clock, arch_GPIOD_clock, 0)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
}
PRIMITIVE_UNSPEC(GPIOE_clock, arch_GPIOE_clock, 0)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
}

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
