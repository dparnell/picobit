(define set-led!
  (lambda (value)
    (#%set-led! value)))

(define acende-led!
  (lambda (value)
     (#%acende-led! value)))

(define sleep
  (lambda (value)
    (#%sleep value)))

(define GPIO-output
  (lambda (ch port)
    (#%GPIO-output ch port)))
