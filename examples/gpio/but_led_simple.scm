(define (main)
  (let ( (led-azul   (led! Pin_8))
         (botao-read (BUTTON-user)) )
    (let loop ( (pressionado? (botao-read)) )
      (if pressionado?
          (led-azul #t)
          (led-azul #f))
      (loop (botao-read)) )) )

(main)


