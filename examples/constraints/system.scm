(define (main)
  (let ( (led-verde (led! Pin_9))
         (led-azul  (led! Pin_8)) )
    (let ( (a (make-connector))
           (b (make-connector))
           (c (make-connector))
           (d (make-connector))
           (e (make-connector)) )
      (adder a b c)
      (multiplier a c d)
      ;;(divider d b e)
      (if (= (/ 1 3) 0)
          (led-azul #t)
          (led-azul #f) )
      ;;(forget-value! a )
      (set-value! a 10 )
      (set-value! b 20 )
      (if (= (get-value c) 30)
          (led-verde #t)
          (led-verde #f) )
      (sleep 40000)
      (if (= (get-value d) 300)
          (led-verde #f)
          (led-verde #t) )
      (sleep 40000)
      (if (= (get-value e) 15)
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
                     ( else 'adder ) ) ))
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

(define (multiplier m1 m2 product)
  (letrec ( (process-new-value
             (lambda ()
               (cond ( (or (and (has-value? m1) (= (get-value m1) 0))
                           (and (has-value? m2) (= (get-value m2) 0)))
                       (set-value! product 0) )
                     ( (and (has-value? m1) (has-value? m2))
                       (set-value! product
                                   (* (get-value m1) (get-value m2)) ) )
                     ( (and (has-value? m1) (has-value? product))
                       (set-value! m2
                                   (/ (get-value product) (get-value m1)) ) )
                     ( (and (has-value? m2) (has-value? product))
                       (set-value! m1
                                   (/ (get-value product) (get-value m2)) ) )
                     ( else 'multiplier ) ) ))
            (process-forget-value
             (lambda ()
               (forget-value! product)
               (forget-value! m1)
               (forget-value! m2)
               (process-new-value) ))
            (me
             (lambda (request)
               (cond ( (eq? request 'I-have-a-value)
                       (process-new-value) )
                     ( (eq? request 'I-lost-my-value)
                       (process-forget-value) )
                     ;;ERROR
                     ) )) )
    (connect m1      me)
    (connect m2      me)
    (connect product me)
    me
    )
  )

(define (divider d1 d2 q)
  (letrec ( (process-new-value
             (lambda ()
               (cond
                ( (and (has-value? d1) 
                       ;;(and (has-value? d2) (not (= (get-value d2) 0))))
                       (has-value? d2))
                  (let ( (a (get-value d1)) )
                    (if (number? a)
                        (IO_write GPIOC Pin_8 #t)
                        (IO_write GPIOC Pin_8 #f) )
                    (set-value! q (quotient a 1))
                    ;;(set-value! q
                    ;;            (/ ;;(get-value d1)
                    ;;             45
                    ;;             a);;(get-value d2))
                                ;;(#%mul-non-neg (get-value d1) (get-value d2))
                              ;;(#%div-non-neg 4 2)
                                ) )
                ;;( (and (has-value? d1) (has-value? d2))
                ;;  (set-value! q
                ;;              (* (get-value d1) (get-value d2)) ) )
                ( (and (has-value? d1) (has-value? q))
                  (set-value! d2
                              (/ (get-value q) (get-value d1)) ) )
                ( (and (has-value? d2) (has-value? q))
                  (set-value! d1
                              (/ (get-value q) (get-value d2)) ) )
                ( else 'divider ) ) ))
            (process-forget-value
             (lambda ()
               (forget-value! q)
               (forget-value! d1)
               (forget-value! d2)
               (process-new-value) ))
            (me
             (lambda (request)
               (cond ( (eq? request 'I-have-a-value)
                       (process-new-value) )
                     ( (eq? request 'I-lost-my-value)
                       (process-forget-value) )
                     ;;ERROR
                     ) )) )
    (connect d1       me)
    (connect d2       me)
    (connect q me)
    me
    )
  )



;; make-connector, in process-new-value don't use for-eacg-except
(define (make-connector)
  (let ( (value #f)
         (constraints '())
         (min #f)
         (max #f) )
    (letrec ( (set-my-value
               (lambda (newval)
                 (cond ( (not (has-value? me))
                         (cond ( (and (not min) (not max))
                                 (begin
                                   (set! value newval)
                                   (let loop ( (items constraints) )
                                     (if (null? items) 'done
                                         (begin (inform-about-value (car items))
                                                (loop (cdr items)) )) )
                                   )  )
                               ( (and (not min) max)
                                 (if (< newval max)
                                   (begin
                                     (set! value newval)
                                     (let loop ( (items constraints) )
                                       (if (null? items) 'done
                                           (begin (inform-about-value (car items))
                                                  (loop (cdr items)) )) ) )
                                   'ignored)  )
                               ( (and min (not max))
                                 (if (> newval min)
                                   (begin
                                     (set! value newval)
                                     (let loop ( (items constraints) )
                                       (if (null? items) 'done
                                           (begin (inform-about-value (car items))
                                                  (loop (cdr items)) )) ) )
                                   'ignored)  )
                               ( (and min max)
                                 (if (and (< newval max) (> newval min))
                                   (begin
                                     (set! value newval)
                                     (let loop ( (items constraints) )
                                       (if (null? items) 'done
                                           (begin (inform-about-value (car items))
                                                  (loop (cdr items)) )) ) )
                                   'ignored)  )
                               
                               )
                         )
                       (else 'ignored ) )) )
              (forget-my-value
               (lambda ()
                 (set! value #f)
                 (for-each-except inform-about-no-value
                                          constraints)) )
              (set-my-min
               (lambda (minval)
                 (set! min minval)))
              (set-my-max
               (lambda (maxval)
                 (set! max maxval)))
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
                       ( (eq? request 'connect) connect )
                       ( (eq? request 'set-min!) set-my-min )
                       ( (eq? request 'set-max!) set-my-max ) ) )) )
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
(define (set-min! connector new-min)
  ((connector 'set-min!) new-min))
(define (set-max! connector new-max)
  ((connector 'set-max!) new-max))

