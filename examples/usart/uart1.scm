(define (main)
  (GPIO_init GPIOA 'Input  'Pull-down SPEED_in Pin_0)
  (let ( (led-verde (led! Pin_9))
         (led-azul  (led! Pin_8))
         (ledd (led! Pin_4)) )
    (ledd #t)
    (led-azul #t)
    (let loop ( (pressionado? (IO_read  GPIOA Pin_0)) )
      (if pressionado?
          (begin (led-azul #t) (UART_GPIO 'read GPIOA Pin_4 p_IN) (UART_GPIO "read" GPIOC Pin_8 p_OUT)
                 (UART_GPIO "write" GPIOC Pin_9 p_OUT)
                 )
          (led-azul #f))
      (sleep 10000)
      (loop (IO_read  GPIOA Pin_0)) )) )

(main)
