#include <picobit.h>
#include <dispatch.h>
#include <arch/stm32/rcc.h>
#include <arch/stm32/gpio.h>
#include <primitives.h>
#include <bignum.h>

void halt_with_error ()
{
  GPIOC->ODR |= BIT(8);

  while(1);
}

PRIMITIVE_UNSPEC(#%sleep, arch_sleep, 1)
{
  static int a, b;

  a1 = decode_int (arg1);

  for(a = 0; a < a1; a++) {
    for(b = 0; b < 100; b++) {
#if defined(CONFIG_ARM_COMPILER_GCC)
      __asm__ __volatile__("nop");
                              #endif

#if defined(CONFIG_ARM_COMPILER_KEIL)
      __nop();
                              #endif
    }
  }

  arg1 = OBJ_FALSE;
}

PRIMITIVE_UNSPEC(#%set-led!, arch_set_led, 1)
{
  if (arg1 == OBJ_FALSE) {
    GPIOC->ODR &= ~BIT(8);
  } else {
    GPIOC->ODR |= BIT(8);
  }

  arg1 = OBJ_FALSE;
}

/*
PRIMITIVE_UNSPEC(#%GPIO-output, arch_GPIO_output, 2)
{
  //arg1 => Channel
  //arg2 => Port

  int ch, port;
  
  ch   = decode_int (arg1);
  port = decode_int (arg2);

  
  //GPIOC->CRH   &= ~BIT(3) & ~BIT(2) & ~BIT(1);
  //GPIOC->CRH   |=  BIT(0);

  switch(ch)
    {
    case 1:
      if(port>7){
	GPIOA->CRH |= BIT(0) << 4*(port - 8);
      }
      else{
	GPIOA->CRL |= BIT(0) << 4*(port); 
      }
      break;

    case 2:
      if(port>7){
	GPIOB->CRH |= BIT(0) << 4*(port - 8);
      }
      else{
	GPIOB->CRL |= BIT(0) << 4*(port); 
      }
      break;

    case 3:
      if(port>7){
	GPIOC->CRH |= BIT(0) << 4*(port - 8);
      }
      else{
	GPIOC->CRL |= BIT(0) << 4*(port); 
      }
      break;

    case 4:
      if(port>7){
	GPIOD->CRH |= BIT(0) << 4*(port - 8);
      }
      else{
	GPIOD->CRL |= BIT(0) << 4*(port); 
      }
      break;

    case 5:
      if(port>7){
	GPIOA->CRH |= BIT(0) << 4*(port - 8);
      }
      else{
	GPIOA->CRL |= BIT(0) << 4*(port); 
      }
      break;
      }
}
*/

void main ()
{
  RCC->APB2ENR |= IOPCEN;

  GPIOC->CRH   &= ~BIT(3) & ~BIT(2) & ~BIT(1);
  GPIOC->CRH   |=  BIT(0);

  //so esta funcionando a main
  //GPIOC->ODR   |= BIT(8);
  
  interpreter();
}
