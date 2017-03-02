;;arithmetic-shift nao ta funcionando no lado de scheme
(define (main)
  (GPIO_init GPIOC 'Input 'Analog SPEED_in Pin_4)
  (GPIO_init GPIOC 'Output 'Push-pull SPEED_50 Pin_8)
  (IO_write GPIOC Pin_8 #t)

  (ADC1_clock)
  (DMA1_clock)
  (ADC_config enable enable ADC_ExternalTrigConv_None 1)

  (let loop ( (var-teste (ADC_read)) )
    (if (< var-teste 2000)
        (IO_write GPIOC Pin_8 #f)
        (IO_write GPIOC Pin_8 #t))
    (loop (ADC_read)) )
  )

(main)
