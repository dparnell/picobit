(define (main)
  (GPIO_init GPIOC 'Input 'Analog SPEED_in Pin_4)
  (GPIO_init GPIOC 'Output 'Push-pull SPEED_50 Pin_8)

  (IO_write GPIOC Pin_8 #t)

  (let ( (readValue_ch14 (ADC_configSingle enable disable enable ADC_Channel_14  ADC_SampleTime_55Cycles5)) )
    (let loop ( (value-ad (readValue_ch14 disable)) )
      (if (< value-ad 2000)
          (IO_write GPIOC Pin_8 #f)
          (IO_write GPIOC Pin_8 #t))
      (loop (ADC_readValue disable)) )) )

(main)
