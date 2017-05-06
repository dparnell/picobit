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
  (let ( (led-duty (PWM_config TIM_3 (- 20000 1) TIM_Channel_3 TIM_OCMode_PWM1 TIM_OCPolarity_High))
         (readValue_ch14 (ADC_single 14 #f)) )
    (UART_PWM TIM_3 TIM_Channel_3 o_READ)
    (let loop ( (value-ad (readValue_ch14)) )
      (sleep 10000)
      ;;(if (< value-ad 2000)
        ;;  (begin
            (IO_write GPIOC Pin_9 #f)
            (led-duty value-ad 4096)
            ;;)
          ;;(begin
          ;;  (IO_write GPIOC Pin_9 #t)
          ;;  (led-duty 70 100)) )
      (loop (readValue_ch14) )        
      )
    )
  )

(main)
