(define GPIO-output
  (lambda (channel)
    (#%GPIO-output channel)))

(define set-led!
  (lambda (value)
    (#%set-led! value)))

(define sleep
  (lambda (value)
    (#%sleep value)))
