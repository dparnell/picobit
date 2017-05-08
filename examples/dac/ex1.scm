(define (main)
  (GPIO_init GPIOC 'Input 'Analog SPEED_in Pin_4)
  (GPIO_init GPIOC 'Output 'Push-pull SPEED_50 Pin_8)

  (IO_write GPIOC Pin_8 #t)

  (DAC_config DAC_Trigger_None DAC_WaveGeneration_None DAC_OutputBuffer_Enable DAC_Channel_1)
  
  (let ( (readValue_ch14 (ADC_single 14 #t)) )
    (let loop ( (value-ad (readValue_ch14)) )
      (sleep 10000)
      (DAC_writeValue DAC_Channel_1 value-ad)
      
      (if (< value-ad 2000)
          (IO_write GPIOC Pin_8 #f)
          (IO_write GPIOC Pin_8 #t))
      (loop (readValue_ch14)) )) )

(main)
