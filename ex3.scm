
(define (pause n)
   (sleep 1000)
   (if (> n 0) (pause (- n 1)))) 

(define (config)
  (GPIO-output "C" 8) )

(define (loop)
  (set-led! 8 #t)
  (sleep 400)
  (set-led! 8 #f)
  (pause 2)
  (loop))

(define (main)
  (config)
  (PWM-config)
  (loop))

(main)
