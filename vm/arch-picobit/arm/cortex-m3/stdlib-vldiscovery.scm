
(define set-led!
  (lambda (value)
    (#%set-led! value)))

(define sleep
  (lambda (value)
    (#%sleep value)))

;;(define GPIO-output
;;  (lambda (channel port)
;;    (#%GPIO-output channel port)))
