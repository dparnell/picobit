#ifndef _STM32_TIM_H_
#define _STM32_TIM_H_

#include <arch/stm32/bits.h>

struct regs_TIM {
  reg CR1;
#define    TIM_CKD(x)    BITS(x, 8, 9)
#define    TIM_ARPE      BIT(7)
#define    TIM_CMS(x)    BITS(x, 5, 6)
#define    TIM_DIR       BIT(4)
#define    TIM_OPM       BIT(3)
#define    TIM_URS       BIT(2)
#define    TIM_UDIS      BIT(1)
#define    TIM_CEN       BIT(0)

  reg CR2;
#define    TIM_TL1S      BIT(7)
#define    TIM_MMS(x)    BITS(x, 4, 6)
#define    TIM_CCDS      BIT(3)

  reg SMCR;
#define    TIM_ETP       BIT(15)
#define    TIM_ECE       BIT(14)
#define    TIM_ETPS(x)   BITS(x, 12,13)
#define    TIM_ETF(x)    BITS(x, 8, 11)
#define    TIM_MSM       BIT(7)
#define    TIM_TS(x)     BITS(x, 4, 6)
#define    TIM_SMS(x)    BITS(x, 0, 2)

  reg DIER;
#define    TIM_TDE       BIT(14)
#define    TIM_CC4DE     BIT(12)
#define    TIM_CC3DE     BIT(11)
#define    TIM_CC2DE     BIT(10)
#define    TIM_CC1DE     BIT(9)
#define    TIM_UDE       BIT(8)
#define    TIM_TIE       BIT(6)
#define    TIM_CC4IE     BIT(4)
#define    TIM_CC3IE     BIT(3)
#define    TIM_CC2IE     BIT(2)
#define    TIM_CC1IE     BIT(1)
#define    TIM_UIE       BIT(0)
  
  reg SR;
#define    TIM_CC4OF     BIT(12)
#define    TIM_CC3OF     BIT(11)
#define    TIM_CC2OF     BIT(10)
#define    TIM_CC1OF     BIT(9)
#define    TIM_TIF       BIT(6)
#define    TIM_CC4IF     BIT(4)
#define    TIM_CC3IF     BIT(3)
#define    TIM_CC2IF     BIT(2)
#define    TIM_CC1IF     BIT(1)
#define    TIM_UIF       BIT(0)
  
  reg EGR;
#define    TIM_TG        BIT(6)
#define    TIM_CC4G      BIT(4)
#define    TIM_CC3G      BIT(3)
#define    TIM_CC2G      BIT(2)
#define    TIM_CC1G      BIT(1)
#define    TIM_UG        BIT(0)
  
  reg CCMR1;
#define    TIM_OC2CE     BIT(15)
#define    TIM_OC2M(x)   BITS(x, 12, 14)
#define    TIM_OC2PE     BIT(11)
#define    TIM_OC2FE     BIT(10)
#define    TIM_CC2S(x)   BITS(x, 8, 9)
#define    TIM_OC1CE     BIT(7)
#define    TIM_OC1M(x)   BITS(x, 4, 6)
#define    TIM_OC1PE     BIT(3)
#define    TIM_OC1FE     BIT(2)
#define    TIM_CC1S(x)   BITS(x, 0, 1)
  
  reg CCMR2;
#define    TIM_OC4CE     BIT(15)
#define    TIM_OC4M(x)   BITS(x, 12, 14)
#define    TIM_OC4PE     BIT(11)
#define    TIM_OC4FE     BIT(10)
#define    TIM_CC4S(x)   BITS(x, 8, 9)
#define    TIM_OC3CE     BIT(7)
#define    TIM_OC3M(x)   BITS(x, 4, 6)
#define    TIM_OC3PE     BIT(3)
#define    TIM_OC3FE     BIT(2)
#define    TIM_CC3S(x)   BITS(x, 0, 1)

  reg CCER;
#define    TIM_CC4P      BIT(13)
#define    TIM_CC4E      BIT(12)
#define    TIM_CC3P      BIT(9)
#define    TIM_CC3E      BIT(8)
#define    TIM_CC2P      BIT(5)
#define    TIM_CC2E      BIT(4)
#define    TIM_CC1P      BIT(1)
#define    TIM_CC1E      BIT(0)
  
  reg CNT;
  reg PSC;
  reg ARR;
  reg reservedx30;
  reg CCR1;
  reg CCR2;
  reg CCR3;
  reg CCR4;
  reg reservedx44;
  reg DCR;
  reg DMAR;
  
};
  
CONFIG_AREA(regs_TIM, TIM2, 0x40000000);
CONFIG_AREA(regs_TIM, TIM3, 0x40000400);
#endif
