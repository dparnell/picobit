/**
  ******************************************************************************
  * @file    system_stm32f1xx.c
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    16-December-2014
  * @brief   CMSIS Cortex-M3 Device Peripheral Access Layer System Source File.
  * 
  * 1.  This file provides two functions and one global variable to be called from 
  *     user application:
  *      - SystemInit(): Setups the system clock (System clock source, PLL Multiplier
  *                      factors, AHB/APBx prescalers and Flash settings). 
  *                      This function is called at startup just after reset and 
  *                      before branch to main program. This call is made inside
  *                      the "startup_stm32f1xx_xx.s" file.
  *
  *      - SystemCoreClock variable: Contains the core clock (HCLK), it can be used
  *                                  by the user application to setup the SysTick 
  *                                  timer or configure other parameters.
  *                                     
  *      - SystemCoreClockUpdate(): Updates the variable SystemCoreClock and must
  *                                 be called whenever the core clock is changed
  *                                 during program execution.
  *
  * 2. After each device reset the HSI (8 MHz) is used as system clock source.
  *    Then SystemInit() function is called, in "startup_stm32f1xx_xx.s" file, to
  *    configure the system clock before to branch to main program.
  *
  * 4. The default value of HSE crystal is set to 8 MHz (or 25 MHz, depending on
  *    the product used), refer to "HSE_VALUE". 
  *    When HSE is used as system clock source, directly or through PLL, and you
  *    are using different crystal you have to adapt the HSE value to your own
  *    configuration.
  *        
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup stm32f1xx_system
  * @{
  */  
  
/** @addtogroup STM32F1xx_System_Private_Includes
  * @{
  */

#include <stdint.h> /* I changed, including stdint */
//#include "include/stm32/stm32f1xx.h" /* I changed, path  */

//#include "include/stm32-hal/inc/stm32f10x.h" /* I changed */
//#include "include/stm32-hal/inc/misc.h" /* I changed, path */
//#include "include/stm32-hal/inc/stm32f10x_gpio.h" /* I changed here */
//#include "include/stm32-hal/inc/stm32f10x_rcc.h" /* I changed here */
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>

#include <picobit.h>
#include <dispatch.h>
#include <primitives.h>
#include <bignum.h>


/**
  * @}
  */

/** @addtogroup STM32F1xx_System_Private_TypesDefinitions
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32F1xx_System_Private_Defines
  * @{
  */
#if !defined  (HSE_VALUE) 
  #define HSE_VALUE    ((uint32_t)8000000) /*!< Default value of the External oscillator in Hz.
                                                This value can be provided and adapted by the user application. */
#endif /* HSE_VALUE */

#if !defined  (HSI_VALUE)
  #define HSI_VALUE    ((uint32_t)8000000) /*!< Default value of the Internal oscillator in Hz.
                                                This value can be provided and adapted by the user application. */
#endif /* HSI_VALUE */

/*!< Uncomment the following line if you need to use external SRAM  */ 
#if defined(STM32F100xE) || defined(STM32F101xE) || defined(STM32F101xG) || defined(STM32F103xE) || defined(STM32F103xG)
/* #define DATA_IN_ExtSRAM */
#endif /* STM32F100xE || STM32F101xE || STM32F101xG || STM32F103xE || STM32F103xG */

/*!< Uncomment the following line if you need to relocate your vector Table in
     Internal SRAM. */ 
/* #define VECT_TAB_SRAM */
#define VECT_TAB_OFFSET  0x0 /*!< Vector Table base offset field. 
                                  This value must be a multiple of 0x200. */

static uint32_t SystemClock = 6;

void SystemInit(){
  static uint8_t ab = 56;
  ab = ab;
  
  SystemClock = 34;
  
}

void Delay(__IO uint32_t nCount) {
  while(nCount--) {
  }
}

/* This funcion shows how to initialize 
 * the GPIO pins on GPIOC / GPIOA and how to configure
 * them as inputs and outputs 
 */
void GPIO_init(void){
	
	/* This TypeDef is a structure defined in the
	 * ST's library and it contains all the properties
	 * the corresponding peripheral has, such as output mode,
	 * pullup / pulldown resistors etc.
	 * 
	 * These structures are defined for every peripheral so 
	 * every peripheral has it's own TypeDef. The good news is
	 * they always work the same so once you've got a hang
	 * of it you can initialize any peripheral.
	 * 
	 * The properties of the periperals can be found in the corresponding
	 * header file e.g. stm32f10x_gpio.h and the source file stm32f10x_gpio.c
	 */
	GPIO_InitTypeDef GPIO_InitStruct;
	
	/* This enables the peripheral clock to the GPIOD IO module
	 * Every peripheral's clock has to be enabled 
	 * 
	 * The STM32F10x documentation file (*.chm) and
	 * datasheet contain the information which peripheral clock has to be used.
	 * 
	 * It is also mentioned at the beginning of the peripheral library's 
	 * source file, e.g. stm32f10x_gpio.c
	 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	/* In this block of instructions all the properties
	 * of the peripheral, the GPIO port in this case,
	 * are filled with actual information and then 
	 * given to the Init function which takes care of 
	 * the low level stuff (setting the correct bits in the 
	 * peripheral's control register)
	 * PC8 and PC9
	 */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_8; // we want to configure all LED GPIO pins
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; 	// we want the pins to be a push-pull output
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; 	// this sets the GPIO modules clock speed
	GPIO_Init(GPIOC, &GPIO_InitStruct); 			// this finally passes all the values to the GPIO_Init function which takes care of setting the corresponding bits.
		
	/* Here the GPIOD module is initialized.
	 * We want to use PA0 as an input because
	 * the USER button on the board is connected
	 * between this pin and VCC.
	 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;		  // we want to configure PA0
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD; 	  // we want it to be an input with a pull-down resistor enabled
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//this sets the GPIO modules clock speed
	GPIO_Init(GPIOA, &GPIO_InitStruct);			  // this passes the configuration to the Init function which takes care of the low level stuff
}

void main(void){
  
  // initialize the GPIO pins we need
  GPIO_init();

  /* This flashes the LEDs on the board once
   * Two registers are used to set the pins (pin level is VCC)
   * or to reset the pins (pin level is GND)
   */
  GPIOC->ODR = GPIO_Pin_8 | GPIO_Pin_9; // set PD8 and PC9

  while (1){  
		GPIOC->ODR |= GPIO_Pin_8; 		// set PC8 without affecting other bits
		
		if (GPIOA->IDR & GPIO_Pin_0)	// check if PA0 button is pressed
		{	
			GPIOC->ODR |= GPIO_Pin_9;	// turn on PC9
		}
		Delay(1000000);				// wait a little
		
		GPIOC->ODR &= ~GPIO_Pin_9;		// clear PC8 and PC9
		Delay(1000000);				// wait a little
	}

  interpreter();

}
