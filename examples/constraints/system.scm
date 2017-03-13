(define (main)
  (let ( (led-verde (led! Pin_9)) )
    (let ( (a (make-connector))
           (b (make-connector))
           (c (make-connector)) )
      (adder a b c)
      ;;(forget-value! a )
      (set-value! a 10 )
      (set-value! b 20 )
      (if (= (get-value c) 30)
          ;;(= (get-value a) 10) ;;(= (get-value c) 30)
          (led-verde #t)
          (led-verde #f) )
      ))
  )

(main)

(define (adder a1 a2 sum)
  (letrec ( (process-new-value
             (lambda ()
               (cond ( (and (has-value? a1) (has-value? a2))
                       (set-value! sum
                                   (+ (get-value a1) (get-value a2)) ) )
                     ( (and (has-value? a1) (has-value? sum))
                       (set-value! a2
                                   (- (get-value sum) (get-value a1)) ) )
                     ( (and (has-value? a2) (has-value? sum))
                       (set-value! a1
                                   (- (get-value sum) (get-value a2)) ) )
                     ( else 'oi ) ) ))
            (process-forget-value
             (lambda ()
               (forget-value! sum)
               (forget-value! a1)
               (forget-value! a2)
               (process-new-value) ))
            (me
             (lambda (request)
               (cond ( (eq? request 'I-have-a-value)
                       (process-new-value) )
                     ( (eq? request 'I-lost-my-value)
                       (process-forget-value) )
                     ;;ERROR
                     ) )) )
    (connect a1  me)
    (connect a2  me)
    (connect sum me)
    me
    ) )

(define (make-connector)
  (let ( (value #f)
         (constraints '()) )
    (letrec ( (set-my-value
               (lambda (newval)
                 (cond ( (not (has-value? me))
                         (begin
                           (set! value newval)
                           (let loop ( (items constraints) )
                             (if (null? items) 'done
                                 (begin (inform-about-value (car items))
                                        (loop (cdr items)) )) )
                           )
                         )
                       (else 'ignored ) )) )
              (forget-my-value
               (lambda ()
                 (set! value #f)
                 (for-each-except inform-about-no-value
                                          constraints)) )
              (connect
               (lambda (new-constraint)
                 (if (not (memq new-constraint constraints))
                     (set! constraints
                           (cons new-constraint constraints))
                     (set! constraints constraints) )
                 (if (has-value? me)
                     (inform-about-value new-constraint)
                     'done)
                 'done ))
              (me
               (lambda (request)
                 (cond ( (eq? request 'has-value?)
                         (if value #t #f) )
                       ( (eq? request 'value) value )
                       ( (eq? request 'set-value!) set-my-value )
                       ( (eq? request 'forget) forget-my-value )
                       ( (eq? request 'connect) connect )) )) )
      me) ) )
    
(define (for-each-except procedure list)
  (letrec ( (loop
             (lambda (items)
               (cond ( (null? items) 'done )
                     ( else (procedure (car items))
                            (loop (cdr items)) )) )) )
  (loop list)) )

;;geral fucntions
(define (inform-about-value constraint)
  (constraint 'I-have-a-value))
(define (inform-about-no-value constraint)
  (constraint 'I-lost-my-value))

(define (connect connector new-constraint)
  ((connector 'connect) new-constraint))
(define (has-value? connector)
  (connector 'has-value?))
(define (get-value connector)
  (connector 'value))
(define (set-value! connector new-value)
  ((connector 'set-value!) new-value))
(define (forget-value! connector)
  (connector 'forget))
