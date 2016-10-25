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
      __asm__ __volatile__("nop");
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

void main ()
{
  RCC->APB2ENR |= IOPCEN;
  GPIOC->CRH   &= ~BIT(3) & ~BIT(2) & ~BIT(1);
  GPIOC->CRH   |=  BIT(0);

  interpreter();
}
