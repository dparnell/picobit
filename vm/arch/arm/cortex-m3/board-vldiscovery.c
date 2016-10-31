#include <picobit.h>
#include <dispatch.h>
#include <arch/stm32/rcc.h>
#include <arch/stm32/gpio.h>
#include <arch/stm32/tim.h>
#include <arch/stm32/afio.h>
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
    }
  }

  arg1 = OBJ_FALSE;
}

PRIMITIVE_UNSPEC(#%set-led!, arch_set_led, 1)
{
  if (arg1 == OBJ_FALSE) {
    GPIOC->ODR &= ~BIT(9);
  } else {
    GPIOC->ODR |= BIT(9);
  }

  arg1 = OBJ_FALSE;
}

PRIMITIVE_UNSPEC(#%acende-led!, arch_acende_led, 1)
{
  if (arg1 == OBJ_FALSE) {
    GPIOC->ODR &= ~BIT(8);
  } else {
    GPIOC->ODR |= BIT(8);
  }

  arg1 = OBJ_FALSE;
}


PRIMITIVE_UNSPEC(#%liga-portas!, arch_liga_portas, 2)
{

  a1 = decode_int (arg1); // port number
 
  // Port bit set/reset register (BSRR)
  if (arg2 == OBJ_FALSE) {
    GPIOA->BSRR = 1 << (a1 + 16); // reset port
  } else {
    GPIOA->BSRR = 1<<a1;  // set port
  }
  arg1 = OBJ_FALSE;
}

/*
  Port bit set/reset register (BSRR)
  GPIOA->BSRR = 1<<24; // reset PA8
  GPIOA->BSRR = 1<<25; // reset PA9
  GPIOA->BSRR = 1<<27; // reset PA11
  GPIOA->BSRR = 1<<28; // reset PA12

  GPIOA->BSRR = 1<<8;  // set PA8
  GPIOA->BSRR = 1<<9;  // set PA9
  GPIOA->BSRR = 1<<11; // set PA11
  GPIOA->BSRR = 1<<12; // set PA12
*/

PRIMITIVE_UNSPEC(#%set-outputA!, arch_set_outputA, 1)
{
  // Set PA8, PA9, PA11, PA12 to output
  // Note the hexadecimal number. The binary equivalent is
  //    0000000000000001000100000010001
  // Each four bits you can configure one port and mode:
  //                                 P12     P11    P10    P9    P8         
  //             00 00 00 00 00 00  00 01   00 01  00 00 00 01  00 01

  a1 = decode_int (arg1); // port number
	
  if (a1 < 8) {
    GPIOA->CRL |= 0x11111111 & (0x0000000F << (4*a1));
  } else if (a1 < 16) {
    GPIOA->CRH |= 0x11111111 & (0x0000000F << (4*(a1-8)));
  } 

  arg1 = OBJ_FALSE;
}


int main ()
{
  RCC->APB2ENR = 0
    // Turn on IO Port A
    //| RCC_APB2ENR_IOPAEN
    // Turn on IO Port B
    //| RCC_APB2ENR_IOPBEN
    // Turn on IO Port C
    //| RCC_APB2ENR_IOPCEN
    | IOPAEN
    //| IOPBEN
    | IOPCEN
    | AFIOEN
    ;
       
  GPIOC->CRH = 0x44444411;
  GPIOA->CRL     |= BIT(7);//CNF1
  GPIOA->CRL     |= BIT(5);
              
  AFIO->EVCR     |= EVOE;
  AFIO->EVCR     |= PORT(000); 
  AFIO->EVCR     |= PIN(0001); 
  AFIO->MAPR     |= TIM2_REMAP(00);
  GPIOA->ODR     |= BIT(1);
  //pwm
  RCC->APB1ENR   |= TIM2EN;

  TIM2->PSC       = 23;
  TIM2->ARR       = 19999; 
  TIM2->CCMR1    |= TIM_OC2M(110); //mode1
  TIM2->CCMR1    |= TIM_OC2PE;
  TIM2->CR1      |= TIM_ARPE;
  TIM2->CCER     |= TIM_CC2P;
  TIM2->CCER     |= TIM_CC2E;
  TIM2->CCR2      = 6000;
  TIM2->EGR      |= TIM_UG;
  TIM2->SR       &= ~TIM_UIF;
  TIM2->CR1      |= TIM_DIR;
  TIM2->CR1      |= TIM_CEN;
       
  //PC9
  GPIOC->ODR     |= BIT(9);
              
  //GPIOA->CRL = 0x00000000;
       
       
  interpreter();
  return 0;
}
