
(define (pause n)
   (sleep 1000)
   (if (> n 0) (pause (- n 1)))) 


(define (loop)
  (set-led! #t)
  (sleep 400)
  (set-led! #f)
  (pause 2)
  (loop))

(loop)
