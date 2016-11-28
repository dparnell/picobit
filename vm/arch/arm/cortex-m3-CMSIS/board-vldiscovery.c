#include <stdint.h>

#include <picobit.h>
#include <dispatch.h>
#include <arch/stm32/rcc.h>
//#include <arch/stm32/gpio.h>
//#include <arch/stm32/adc.h>
#include <arch/stm32/stm32f100xb.h>
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

PRIMITIVE_UNSPEC(#%set-led!, arch_set_led, 2)
{
  int led;

  led = decode_int(arg1);

  if(led == 8 || led == 9){
    if (arg2 == OBJ_FALSE) {
      GPIOC->ODR &= ~BIT(led);
    } else {
      GPIOC->ODR |= BIT(led);
    }
  }

}

PRIMITIVE_UNSPEC(#%GPIO-output, arch_GPIO_output, 2)
{
  int ch, port;

  port = decode_int(arg1);
  ch   = decode_int(arg2);

  switch(port){
  case 0:
    if(ch < 8){
      GPIOA->CRL |= 0x11111111 & (0x00000003 << (4*ch));
    }
    else if (ch < 16){
      GPIOA->CRH |= 0x11111111 & (0x00000003 << (4*(ch-8)));
    }
    break;

  case 1:
    if(ch < 8){
      GPIOB->CRL |= 0x11111111 & (0x00000003 << (4*ch));
    }
    else if (ch < 16){
      GPIOB->CRH |= 0x11111111 & (0x00000003 << (4*(ch-8)));
    }
    break;

  case 2:
    if(ch < 8){
      GPIOC->CRL |= 0x11111111 & (0x00000003 << (4*ch));
    }
    else if (ch < 16){
      GPIOC->CRH |= 0x11111111 & (0x00000003 << (4*(ch-8)));
    }
    break;
  }  
}

PRIMITIVE(#%ADC-read, arch_ADC_read, 0)
{
  int data;

  while(ADC1->SR){}
  data = ADC1->DR;

  arg1 = encode_int(data);
}

PRIMITIVE_UNSPEC(#%make-button, arch_make_button, 0)
{
  
}

PRIMITIVE(#%read-button, arch_read_button, 0)
{
  
}

PRIMITIVE_UNSPEC(#%ADC-config, arch_ADC_config, 0)
{
  ADC1->CR2   |= ADC_CR2_ADON;
  ADC1->CR2   |= ADC_CR2_RSTCAL;
  ADC1->CR2   |= ADC_CR2_CAL;

  ADC1->CR2   |= ADC_CR2_EXTTRIG;
  ADC1->CR2   |= ADC_CR2_EXTSEL_0 | ADC_CR2_EXTSEL_1 | ADC_CR2_EXTSEL_2;

  ADC1->CR2   |= ADC_CR2_CONT;
  ADC1->CR2   |= ADC_CR2_TSVREFE;

  ADC1->SQR3  |= ADC_SQR3_SQ1_4;

  ADC1->CR2   |= ADC_CR2_SWSTART;
}

PRIMITIVE_UNSPEC(#%PWM-config, arch_PWM_config, 0)
{
  GPIOB->CRL   |= 0x000003B3;
  //GPIOB->ODR   |= GPIO_ODR_ODR0 | GPIO_ODR_ODR2;

  AFIO->MAPR   |= AFIO_MAPR_TIM3_REMAP_NOREMAP;
  
  TIM3->ARR     = 19999;
  TIM3->PSC     = 23;

  TIM3->CCMR2  |= TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4PE;

  TIM3->CR1    |= TIM_CR1_ARPE;
  TIM3->CCER   |= TIM_CCER_CC4E;

  TIM3->CCR4    = 10000;

  TIM3->EGR    |= TIM_EGR_UG;
  TIM3->SR     &= ~TIM_SR_UIF;
}

void main ()
{
  RCC->APB2ENR |= IOPCEN | ADC1EN | AFIOEN | IOPBEN;
  RCC->APB1ENR |= TIM3EN;
  
  GPIOC->CRL = 0x00000000;
  GPIOC->CRH = 0x00000000;
  GPIOC->ODR = 0x00000000;
  
  GPIOB->CRL = 0x00000000;
  GPIOB->CRH = 0x00000000;
  GPIOB->ODR = 0x00000000;

  ADC1->CR2   |= ADC_CR2_ADON;
  ADC1->CR2   |= ADC_CR2_RSTCAL;
  ADC1->CR2   |= ADC_CR2_CAL;

  ADC1->CR2   |= ADC_CR2_EXTTRIG;
  ADC1->CR2   |= ADC_CR2_EXTSEL_0 | ADC_CR2_EXTSEL_1 | ADC_CR2_EXTSEL_2;

  ADC1->CR2   |= ADC_CR2_CONT;

  ADC1->SQR3  |= ADC_SQR3_SQ1_1 | ADC_SQR3_SQ1_3;

  ADC1->CR2   |= ADC_CR2_SWSTART;

  interpreter();
}
