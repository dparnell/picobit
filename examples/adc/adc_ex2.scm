;;trava com a chamada de arithmetic-shift
;;nao sei pq
;;nao tenho ideia

(define (main)
  (GPIO_init GPIOC 'Output 'Push-pull SPEED_50 Pin_9)
  (IO_write GPIOC Pin_9 #t)
  (sleep 4000)
  (if (= (#%arithmetic-shift-left 1 20) 1048576)
   ;;(= (#%arithmetic-shift-right #x00E00000 20) 14)
   ;;(= (#%div-non-neg 10 2) 5)
      (IO_write GPIOC Pin_9 #f)
      (IO_write GPIOC Pin_9 #t) )
  ;;(sleep 40000000)
  ;;(IO_write GPIOC Pin_9 #f)
  )

(main)
