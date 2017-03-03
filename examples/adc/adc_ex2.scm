(define (main)
  (GPIO_init GPIOC 'Input 'Analog SPEED_in Pin_4)
  (GPIO_init GPIOC 'Output 'Push-pull SPEED_50 Pin_8)

  (IO_write GPIOC Pin_8 #t)

  (let ( (readValues (ADC_configMulti disable disable '(ADC_Channel_14) '(ADC_SampleTime_55Cycles5) '(ad1))) )
    (readValues)
    (let loop ( (value-ad (assoc 'ad1 (readValues))) )
      (if (< value-ad 2000)
          (IO_write GPIOC Pin_8 #f)
          (IO_write GPIOC Pin_8 #t))
      (loop (assoc 'ad1 (readValues))) )
    ))

(main)

    
