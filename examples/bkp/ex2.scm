;;Read AD

(define (config)
  (GPIO-output "C" 9)
  (ADC-config))

(define (main)
  (config)
  (set-led! 9 #t)
  (sleep 4000)
  
  (ADC-read)
  )

(main)
