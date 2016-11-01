
(define (pause n)
   (sleep 1000)
   (if (> n 0) (pause (- n 1)))) 

(define (config)
  (GPIO-output 9))

(define (loop)
  (set-led! #t)
  (sleep 400)
  (set-led! #f)
  (pause 2)
  (loop))

(define (main)
  (config)
  (loop))

(main)
