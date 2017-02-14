#include <stdint.h>
#include <picobit.h>
#include <dispatch.h>
#include <bits.h>
#include <primitives.h>
#include <bignum.h>

#include <rcc.h>
//#include <gpio.h>
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

void Delay(__IO uint32_t nCount) {
  while(nCount--) {
  }
}

PRIMITIVE_UNSPEC(#%GPIO_example, arch_GPIO_example, 0)
{
  GPIOC->ODR = GPIO_Pin_8 | GPIO_Pin_9; // set PD8 and PC9
    
  while (1){
    GPIOC->ODR |= GPIO_Pin_8; // set PC8 without affecting other bits
    
    if (GPIOA->IDR & GPIO_Pin_0)// check if PA0 button is pressed
      {
	GPIOC->ODR |= GPIO_Pin_9;// turn on PC9
      }
    Delay(1000000L);// wait a little

    GPIOC->ODR &= ~GPIO_Pin_9;// clear PC8 and PC9
    Delay(1000000L);// wait a little
  }

}

void main ()
{
  
  /*
  //volatile uint8_t data;
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
  */
  /* USART begin:
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
  */
  
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
