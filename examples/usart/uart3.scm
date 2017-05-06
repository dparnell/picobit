(define (main)
  (setup)
  )

(define (setup)
  (GPIO_init GPIOC 'Alternative-function 'Push-pull SPEED_50 Pin_8)
  (GPIO_init GPIOC 'Output 'Push-pull SPEED_50 Pin_9)
  
  (AFIO_clock)
  (GPIO_FullRemap_TIM3)
  (TIM_config TIM_3 (- 24 1) (- 20000 1) TIM_CounterMode_Up)

  (IO_write GPIOC Pin_9 #t)
  (let ( (led-duty (PWM_config TIM_3 (- 20000 1) TIM_Channel_3 TIM_OCMode_PWM1 TIM_OCPolarity_High)) )
    (led-duty 30 100)
    )
  )

(main)
