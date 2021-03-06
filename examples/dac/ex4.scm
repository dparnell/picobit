(define (main)
  (GPIO_init GPIOC 'Input 'Analog SPEED_in Pin_4)
  (GPIO_init GPIOC 'Output 'Push-pull SPEED_50 Pin_8)

  (GPIO_init GPIOC 'Input 'Analog SPEED_in Pin_5)
  (GPIO_init GPIOC 'Output 'Push-pull SPEED_50 Pin_9)
  
  (IO_write GPIOC Pin_8 #t)
  (IO_write GPIOC Pin_9 #t)

  (let ( (readValues (ADC_multi (list ADC_Channel_14 ADC_Channel_15)
                                '(ad1 ad2) #t))
         (dac-control (DAC_single DAC_Channel_1 #t))
         (botao-read (BUTTON-user)) )
    (let loop ( (value-ad  (cadr (assoc 'ad1 (readValues))))
                (value-ad2 (cadr (assoc 'ad2 (readValues))))
                (pressionado? (botao-read))  )
      (sleep 10000)

      (if pressionado?
          (begin
            (dac-control value-ad)
            (if (< value-ad2 2000)
                (IO_write GPIOC Pin_8 #f)
                (IO_write GPIOC Pin_8 #t)) )
          (if (< value-ad2 2000)
                (IO_write GPIOC Pin_8 #f)
                (IO_write GPIOC Pin_8 #t)) )

      (loop (cadr (assoc 'ad1 (readValues))) (cadr (assoc 'ad2 (readValues))) (botao-read) )   ))
  (main)
  )

(main)
