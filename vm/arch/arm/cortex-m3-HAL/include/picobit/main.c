#include <stdint.h>
#include <picobit.h>
#include <dispatch.h>
#include <bits.h>
#include <primitives.h>
#include <bignum.h>

#include <rcc.h>
#include <stm32f10x.h>
#include <system_stm32f10x.h>

void halt_with_error ()
{
  //GPIOC->ODR |= BIT(8);
  GPIOC->ODR |= 256;

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

    }
  }

  arg1 = OBJ_FALSE;
}

void Delay(__IO uint32_t nCount) {
  while(nCount--) {
  }
}

void main ()
{
  usart_init(9600);  

  interpreter();
}
