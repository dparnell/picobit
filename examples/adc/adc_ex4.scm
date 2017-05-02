(define (main)
  (GPIO_init GPIOC 'Input 'Analog SPEED_in Pin_4)
  (GPIO_init GPIOC 'Output 'Push-pull SPEED_50 Pin_8)

  (IO_write GPIOC Pin_8 #t)

  (let ( (readValue_ch14 (ADC_single 14 #f)) )
    (let loop ( (value-ad (readValue_ch14)) )
      (if (< value-ad 2000)
          (IO_write GPIOC Pin_8 #f)
          (IO_write GPIOC Pin_8 #t))
      (loop (readValue_ch14)) )) )

(main)
