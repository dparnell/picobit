;;FAZER ----- terminar, lembrar REMAP = AFIO

(define (main)
  (setup2)
  )

(define (setup)
  (GPIO_init GPIOB 'Alternative-function 'Push-pull SPEED_50 Pin_6)
  (TIM_config TIM_4 (- 24 1) (- 20000 1) TIM_CounterMode_Up)
  
  (let ( (led-duty (PWM_config TIM_4 TIM_Channel_1 TIM_OCMode_PWM1 TIM_OCPolarity_High)) )
    (led-duty 10000)
    )
  )

(define (setup2)
  (GPIO_init GPIOC 'Alternative-function 'Push-pull SPEED_50 Pin_8)
  (GPIO_init GPIOC 'Output 'Push-pull SPEED_50 Pin_9)
  (#%AFIO_remap)
  (TIM_config TIM_3 (- 24 1) (- 20000 1) TIM_CounterMode_Up)

  (IO_write GPIOC Pin_9 #t)
  (let ( (led-duty (PWM_config TIM_3 TIM_Channel_3 TIM_OCMode_PWM1 TIM_OCPolarity_High)) )
    (led-duty 10000)
    )
  )

(main)
