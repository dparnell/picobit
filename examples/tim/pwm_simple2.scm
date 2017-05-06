(define (main)
  (setup)
  )

(define (setup)
  (GPIO_init GPIOC 'Input 'Analog SPEED_in Pin_4)
  (GPIO_init GPIOC 'Alternative-function 'Push-pull SPEED_50 Pin_8)
  (GPIO_init GPIOC 'Output 'Push-pull SPEED_50 Pin_9)
  
  (AFIO_clock)
  (GPIO_FullRemap_TIM3)
  (TIM_config TIM_3 (- 24 1) (- 20000 1) TIM_CounterMode_Up)

  (IO_write GPIOC Pin_9 #t)
  (let ( (led-duty (PWM_config TIM_3 (- 20000 1) TIM_Channel_3 TIM_OCMode_PWM1 TIM_OCPolarity_High))
         (readValue_ch14 (ADC_single 14 #f)) )
    (let loop2 ( (value-ad (readValue_ch14)) )
      (led-duty 1000 4096)
      (if (< value-ad 2000)
          (IO_write GPIOC Pin_9 #f)
          (IO_write GPIOC Pin_9 #t))

      (sleep 1000)
      (loop2 (readValue_ch14) )
      )
    )
  )

(main)
