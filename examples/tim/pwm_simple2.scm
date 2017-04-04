(define (main)
  (setup)
  
  (let ( (readValue (ADC_configSingle enable disable enable ADC_Channel_14  ADC_SampleTime_55Cycles5))
         (led-duty (PWM_config TIM_3 (- 20000 1) TIM_Channel_3 TIM_OCMode_PWM1 TIM_OCPolarity_High)) )
    (let loop ( (value (readValue)) )
      (led-duty value 4095)
      (loop (readValue)) )
    )
  )

(define (setup)
  (AFIO_clock)
  
  (GPIO_init GPIOC 'Input 'Analog SPEED_in Pin_4)
  (GPIO_init GPIOC 'Alternative-function 'Push-pull SPEED_50 Pin_8)
  (GPIO_init GPIOC 'Output 'Push-pull SPEED_50 Pin_9)
  
  (GPIO_FullRemap_TIM3)
  (TIM_config TIM_3 (- 24 1) (- 20000 1) TIM_CounterMode_Up) )

(main)
