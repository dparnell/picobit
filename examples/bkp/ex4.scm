;;PC0 - ADC1-IN10
;;acima de 2000 apaga LED
;;abaixo desliga o LED

(define (config)
  (GPIO-output "C" 9)
  (ADC-config))

(define (main)
  (config)
  (set-led! 9 #t)
  (sleep 4000)
  
  (let loop ((adc-value (ADC-read)))
    (if (> adc-value 2000)
        (set-led! 9 #t)
        (set-led! 9 #f))
    (loop (ADC-read)) )
  )

(main)
