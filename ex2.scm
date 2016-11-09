(define (config)
  (GPIO-output "C" 9))

(define (main)
  (config)
  (set-led! 9 #t)
  (sleep 4000)
  (set-led! 9 #f)
  (ADC-read)

  (sleep 5000)
  (ADC-read)
  )

(main)
