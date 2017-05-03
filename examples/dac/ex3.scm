(define (main)
  (GPIO_init GPIOC 'Input 'Analog SPEED_in Pin_4)
  (GPIO_init GPIOC 'Output 'Push-pull SPEED_50 Pin_8)

  (GPIO_init GPIOC 'Input 'Analog SPEED_in Pin_5)
  (GPIO_init GPIOC 'Output 'Push-pull SPEED_50 Pin_9)
  
  (IO_write GPIOC Pin_8 #t)
  (IO_write GPIOC Pin_9 #t)

  ;;(DAC_config DAC_Trigger_None DAC_WaveGeneration_None DAC_OutputBuffer_Enable DAC_Channel_1)
  
  (let ( (readValues (ADC_multi (list ADC_Channel_14 ADC_Channel_15)
                                '(ad2 ad1) #t)) )
    (let loop ( (value-ad  (cadr (assoc 'ad1 (readValues))))
                (value-ad2 (cadr (assoc 'ad2 (readValues)))) )

      ;;(DAC_writeValue DAC_Channel_1 value-ad2)
      (if (< value-ad 2000)
          (IO_write GPIOC Pin_8 #f)
          (IO_write GPIOC Pin_8 #t))

      (sleep 10000)
      (loop (cadr (assoc 'ad1 (readValues))) (cadr (assoc 'ad2 (readValues)))) )) )

(main)
