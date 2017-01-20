#include <stdint.h>
#include <picobit.h>
#include <dispatch.h>
#include <arch/stm32/rcc.h>
//#include <arch/stm32/gpio.h>
//#include <arch/stm32/adc.h>
#include <arch/stm32/stm32f100xb.h>
#include <primitives.h>
#include <bignum.h>

//uint32_t tim_arr, tim_cnt;

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
  uint16_t data;

  //while(ADC1->SR){}
  data = ADC1->DR;

  arg1 = encode_int(data);
}

PRIMITIVE_UNSPEC(#%set-duty, arch_set_duty, 1)
{
  uint16_t adc_value;
  uint16_t duty_cicle;
    
  adc_value  = decode_int(arg1);

  duty_cicle = (uint16_t) adc_value*19999/4095;

  TIM3->CCR4 = duty_cicle; 
}

PRIMITIVE_UNSPEC(#%set-arr, arch_set_arr, 1)
{
  uint16_t arr_value;
    
  arr_value  = decode_int(arg1);

  TIM3->ARR = arr_value; 
}

PRIMITIVE_UNSPEC(#%set-psc, arch_set_psc, 1)
{
  uint16_t psc_value;
    
  psc_value  = decode_int(arg1);

  TIM3->ARR = psc_value; 
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

  //ADC1->CR2   |= ADC_CR2_TSVREFE; //sensor-temp
  //ADC1->SQR3  |= ADC_SQR3_SQ1_4; //IN16

  //ADC1->SQR3  |= ADC_SQR3_SQ1_0 | ADC_SQR3_SQ1_2; //PA5-IN5

  ADC1->SQR3  |= ADC_SQR3_SQ1_3 | ADC_SQR3_SQ1_1; //PC0-IN10
  
  ADC1->CR2   |= ADC_CR2_SWSTART;
}

PRIMITIVE_UNSPEC(#%PWM-config, arch_PWM_config, 0)
{
  GPIOB->CRL   |= 0x000003B3;
  GPIOB->ODR   |= GPIO_ODR_ODR0 | GPIO_ODR_ODR2;

  AFIO->MAPR   |= AFIO_MAPR_TIM3_REMAP_NOREMAP;
  
  TIM3->CR1    |= TIM_CR1_ARPE;
  TIM3->CR1    |= TIM_CR1_CEN;
  
  TIM3->ARR     = 19999; //50Hz
  //TIM3->ARR     = 49999; //20Hz
  
  TIM3->PSC     = 24-1;

  TIM3->CCMR2  |= TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_0 | TIM_CCMR2_OC4PE;
  //TIM3->CCMR2  |= TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_0 | TIM_CCMR2_OC4PE;

  TIM3->CCER   |= TIM_CCER_CC4E;

  TIM3->CCR4    = 10000;

  TIM3->EGR    |= TIM_EGR_UG;
  //IM3->SR     &= ~TIM_SR_UIF;
}

void main ()
{
  volatile uint8_t data;
  RCC->APB2ENR |= IOPCEN | ADC1EN | AFIOEN | IOPBEN | IOPAEN;
  RCC->APB1ENR |= TIM3EN | USART2EN;
  
  GPIOC->CRL = 0x00000000;
  GPIOC->CRH = 0x00000000;
  GPIOC->ODR = 0x00000000;
  
  GPIOB->CRL = 0x00000000;
  GPIOB->CRH = 0x00000000;
  GPIOB->ODR = 0x00000000;

  GPIOA->CRL = 0x00000000;
  GPIOA->CRH = 0x00000000;
  GPIOA->ODR = 0x00000000;

  //USART2 - transmission
  // Put PA2  (TX) to alternate function output push-pull at 50 MHz
  // Put PA3 (RX) to floating input
  GPIOA->CRL |= 0x0004B00;
  GPIOC->CRH |= 0x00000003;
  
  AFIO->MAPR &= ~AFIO_MAPR_USART2_REMAP; //remap = 0

  USART2->CR1  |= USART_CR1_UE; //enable usart
  //USART2->GTPR |= USART_GTPR_PSC_0; //divides the source clock by 1
  USART2->CR1  &= ~USART_CR1_M; //word length 8
  USART2->CR2  &= ~USART_CR2_STOP_0 & ~USART_CR2_STOP_1; //1 stop bit
  USART2->CR1  &= ~USART_CR1_OVER8; //oversampling 16
  USART2->BRR   = 0x9C4; //USART_DIV = 156.25 baud_rate = 9600
  USART2->CR1  |= USART_CR1_TE; //enable transmitter
  USART2->CR1  |= USART_CR1_RE; //enable receiver

  while(1){
    //while( !(USART2->SR)) {GPIOD->ODR |= BIT(8);}
    GPIOC->ODR |= BIT(8);
    while(!(USART2->SR & USART_SR_RXNE)) {GPIOC->ODR &= ~BIT(8);}
    data = USART2->DR;
    data = data;

    data = 67;
    GPIOC->ODR |= BIT(8);
    while(!(USART2->SR & USART_SR_TXE)) {GPIOC->ODR &= ~BIT(8);}
    USART2->DR = 0x43;
    
    //GPIOC->ODR &= ~BIT(8);
  }
  //GPIOC->CRH |= 0x0000000B;
  //GPIOC->ODR |= BIT(8);

  //GPIOC->CRH |= 0x000000B0;
  //GPIOC->ODR |= BIT(9);
  
  /*
  tim_arr = GPIOC->CRH;
  tim_cnt = GPIOC->ODR;
  //AFIO->MAPR  |= AFIO_MAPR_TIM3_REMAP_FULLREMAP; //tim3_ch4 pc9

  TIM3->ARR    = 19999;
  TIM3->PSC    = 23;
  TIM3->CCMR2 |= TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4PE;

  TIM3->CR1   |= TIM_CR1_ARPE;

  TIM3->CCER  |= TIM_CCER_CC4E;
  TIM3->CCR4   = 18000;

  TIM3->CR1   |= TIM_CR1_CEN;
 
  tim_arr = TIM3->ARR;
  tim_cnt = TIM3->CNT;

  ADC1->CR2   |= ADC_CR2_ADON;
  ADC1->CR2   |= ADC_CR2_RSTCAL;
  ADC1->CR2   |= ADC_CR2_CAL;

  ADC1->CR2   |= ADC_CR2_EXTTRIG;
  ADC1->CR2   |= ADC_CR2_EXTSEL_0 | ADC_CR2_EXTSEL_1 | ADC_CR2_EXTSEL_2;

  ADC1->CR2   |= ADC_CR2_CONT;

  ADC1->SQR3  |= ADC_SQR3_SQ1_1 | ADC_SQR3_SQ1_3;

  ADC1->CR2   |= ADC_CR2_SWSTART;
  */
  interpreter();
}
