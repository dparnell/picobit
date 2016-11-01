(define GPIO-output
  (lambda (port channel)
    (cond [(equal? port "A")
           (#%GPIO-output 0 channel)]
          [(equal? port "B")
           (#%GPIO-output 1 channel)]
          [(equal? port "C")
           (#%GPIO-output 2 channel)] )) )

(define set-led!
  (lambda (led value)
    (#%set-led! led value)))

(define sleep
  (lambda (value)
    (#%sleep value)))
