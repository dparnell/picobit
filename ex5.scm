;;PC0 - ADC1-IN10
;;ADC dimerizando PB1
;;lendo PC0 controlando PB1

(define (config)
  (GPIO-output "C" 9)
  (ADC-config)
  (PWM-config))

(define (main)
  (config)
  (set-led! 9 #t)
  (sleep 4000)
  
  (let loop ((adc-value (ADC-read)))
    (set-duty adc-value)
    (loop (ADC-read)) )
  )

(main)
