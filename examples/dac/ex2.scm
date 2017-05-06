(define (main)
  (GPIO_init GPIOC 'Input 'Analog SPEED_in Pin_4)
  (GPIO_init GPIOC 'Output 'Push-pull SPEED_50 Pin_8)

  (GPIO_init GPIOC 'Input 'Analog SPEED_in Pin_5)
  (GPIO_init GPIOC 'Output 'Push-pull SPEED_50 Pin_9)
  
  (IO_write GPIOC Pin_8 #t)
  (IO_write GPIOC Pin_9 #t)

  ;;(DAC_config DAC_Trigger_None DAC_WaveGeneration_None DAC_OutputBuffer_Enable DAC_Channel_1)

  ;;(UART_GPIO "read" GPIOC Pin_9 p_OUT)
  
  (let ( (readValues (ADC_single ADC_Channel_14 #t )) )
    (let loop ( (values (readValues)) )
      (let ( (value-ad  values) )
        
        ;;(DAC_writeValue DAC_Channel_1 value-ad2)
        (if (< value-ad 2000)
            (IO_write GPIOC Pin_8 #f)
            (IO_write GPIOC Pin_8 #t))
        )
      (loop (readValues) ) )) )

(main)
