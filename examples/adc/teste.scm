(define (main)
  (GPIO_init GPIOC 'Input 'Analog SPEED_in Pin_4)
  (GPIO_init GPIOC 'Output 'Push-pull SPEED_50 Pin_8)

  (GPIO_init GPIOC 'Input 'Analog SPEED_in Pin_5)
  (GPIO_init GPIOC 'Output 'Push-pull SPEED_50 Pin_9)
  
  ;;(IO_write GPIOC Pin_8 #t)
  ;;(IO_write GPIOC Pin_9 #t)

  (let ( (readValues (ADC_configMulti enable enable (list ADC_Channel_14 ADC_Channel_15)
                                      (list ADC_SampleTime_55Cycles5 ADC_SampleTime_55Cycles5)
                                      '(ad2 ad1))) )
    (let loop ( (value-ad  (cadr (assoc 'ad1 (readValues))))
                (value-ad2 (cadr (assoc 'ad2 (readValues)))) )
      (if (equal? (assoc 'ad1 (readValues)) (list 'ad1 value-ad))
          (IO_write GPIOC Pin_8 #t)
          (IO_write GPIOC Pin_8 #f))
      (if (equal? (assoc 'ad2 (readValues)) (list 'ad2 value-ad2))
          (IO_write GPIOC Pin_9 #t)
          (IO_write GPIOC Pin_9 #f))
      (loop (cadr (assoc 'ad1 (readValues))) (cadr (assoc 'ad2 (readValues)))) )) )

(main)
