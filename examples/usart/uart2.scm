(define (main)
  (GPIO_init GPIOC 'Input 'Analog SPEED_in Pin_4)
  (GPIO_init GPIOC 'Output 'Push-pull SPEED_50 Pin_8)

  (IO_write GPIOC Pin_8 #t)

  (UART_AD 14)
  (let ( (readValue_ch14 (ADC_config 14)) )
    (let loop ( (value-ad (readValue_ch14)) )
      (if (< value-ad 2000)
          (IO_write GPIOC Pin_8 #f)
          (IO_write GPIOC Pin_8 #t))
      (sleep 10000)
      (loop (readValue_ch14)) )) )

(main)
