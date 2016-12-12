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
    (#%set-led! led value) ))

(define sleep
  (lambda (value)
    (#%sleep value) ))

#|(define ADC-read
  (lambda ()
    (let ((value (#%ADC-read)))
      (if (= value 0)
          (set-led! 9 #t)
          (set-led! 9 #f)) )))|#

(define ADC-read
  (lambda()
    (let ((value (#%ADC-read)))
      value) ))

(define ADC-config
  (lambda ()
    (#%ADC-config) ))

(define PWM-config
  (lambda ()
    (#%PWM-config) ))

(define set-duty
  (lambda (value)
    (#%set-duty value) ))
