#ifndef _STM32_ADC_H_
#define _STM32_ADC_H_

#include <arch/stm32/bits.h>

struct regs_GPIO {

  //status register
  reg SR;
#define		STRT		BIT(4)
#define		JSTRT       	BIT(3)
#define	        JEOC            BIT(2)
#define         EOC             BIT(1)
#define         AWD             BIT(0)

  //control register 1
  reg CR1;
#define         AWDEN           BIT(23)
#define         JAWDEN          BIT(22)
#define         DUALMODE(x)     BITS(x, 16, 19)
#define         DISCNUM(x)      BITS(x, 13, 15)
#define         JDISCEN         BIT(12)
#define         DISCEN          BIT(11)
#define         JAUTO           BIT(10)
#define         AWDSGL          BIT(9)
#define         SCAN            BIT(8)
#define         JEOCIE          BIT(7)
#define         AWDIE           BIT(6)
#define         EOCIE           BIT(5)
#define         AWDCH(x)        BITS(x, 0, 4)

  //control register 2 - temperature sensor
  reg CR2;
#define         TSVREFE         BIT(23)
#define         SWSTART         BIT(22)
#define         JSWSTART        BIT(21)
#define         EXTTRIG         BIT(20)
#define         EXTSEL(x)       BITS(x, 17, 19)
#define         JEXTTRIG        BIT(15)
#define         JEXTSEL(x)      BITS(x, 12, 14)
#define         ALIGN           BIT(11)
#define         DMA             BIT(8)
#define         RSTCAL          BIT(3)
#define         CAL             BIT(2)
#define         CONT            BIT(1)
#define         ADON            BIT(0)

  //sample time register 1
  reg SMPR1;

#define         SMP17(x)        BITS(x, 21, 23)
#define         SMP16(x)        BITS(x, 18, 20)
#define         SMP15(x)        BITS(x, 15, 17)
#define         SMP14(x)        BITS(x, 12, 14)
#define         SMP13(x)        BITS(x, 9, 11)
#define         SMP12(x)        BITS(x, 6, 8)
#define         SMP11(x)        BITS(x, 3, 5)
#define         SMP10(x)        BITS(x, 0, 2)

  //sample time register 2
  reg SMPR2;
#define         SMP9(x)         BITS(x, 27, 29)
#define         SMP8(x)         BITS(x, 24, 26)
#define         SMP7(x)         BITS(x, 21, 23)
#define         SMP6(x)         BITS(x, 18, 20)
#define         SMP5(x)         BITS(x, 15, 17)
#define         SMP4(x)         BITS(x, 12, 14)
#define         SMP3(x)         BITS(x, 9, 11)
#define         SMP2(x)         BITS(x, 6, 8)
#define         SMP1(x)         BITS(x, 3, 5)
#define         SMP0(x)         BITS(x, 0, 2)

  //injected channel data offset register x
  reg JOFR1;
  reg JOFR2;
  reg JOFR3;
  reg JOFR4;
#define         JOFFSET(x)      BITS(x, 0, 11)

  //watchdog high threshold register
  reg HTR;
#define         HT(x)           BITS(x, 0, 11)

  //watchdog low threshold register
  reg LTR;
#define         LT(x)           BITS(x, 0, 11)

  //regular sequence register 1
  reg SQR1;
#define         L(x)            BITS(x, 20, 23)
#define         SQ16(x)         BITS(x, 15, 19)
#define         SQ15(x)         BITS(x, 10, 14)
#define         SQ14(x)         BITS(x, 5, 9)
#define         SQ13(x)         BITS(x, 0, 4)

  //regular sequence register 2
  reg SQR2;
#define         SQ12(x)         BITS(x, 25, 29)
#define         SQ11(x)         BITS(x, 20, 24)
#define         SQ10(x)         BITS(x, 15, 19)
#define         SQ9(x)          BITS(x, 10, 14)
#define         SQ8(x)          BITS(x, 5, 9)
#define         SQ7(x)          BITS(x, 0, 4)

  //regular sequence register 3
  reg SQR3;
#define        SQ6(x)           BITS(x, 25, 29)
#define        SQ5(x)           BITS(X, 20, 24)
#define        SQ4(x)           BITS(x, 15, 19)
#define        SQ3(x)           BITS(x, 10, 14)
#define        SQ2(x)           BITS(x, 5, 9)
#define        SQ1(x)           BITS(x, 0, 4)

  //injected sequence register
  reg JSQR;
#define        JL(x)            BITS(x, 20, 21)
#define        JSQ4(x)          BITS(x, 15, 19)
#define        JSQ3(x)          BITS(x, 10, 14)
#define        JSQ2(x)          BITS(x, 5, 9)
#define        JSQ1(x)          BITS(x, 0, 4)

  //injected data register x
  reg JDR;
#define        JDATA(x)         BITS(x, 0, 15)

  //regular data register
  reg DR;
#define        ADC2DATA(x)      BITS(x, 16, 31)
#define        DATA(x)          BITS(x, 0, 15)
};

CONFIG_AREA(regs_ADC, ADC1, 0x40012400);
CONFIG_AREA(regs_ADC, ADC2, 0x40012800);
CONFIG_AREA(regs_ADC, ADC3, 0x40013C00);

#endif
