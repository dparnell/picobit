(define (make-connector)
  (let ( (value #f)
         (informant  #f)
         (constraints '()) )
    (letrec ( (set-my-value
               (lambda (newval setter)
                 (if #t ;;(not value)
                  ;;(eq? setter informant)
                  ;;(not (has-value? me))
                  (begin
                    (set! value newval)
                    (set! informant setter)
                    (for-each-except setter
                                     inform-about-value
                                     constraints)
                    )
                  'ignored)
                 )
               )
              (forget-my-value
               (lambda (retractor)
                 (if ;;(eq? retractor informant)
                  #t
                     (begin (set! informant #f)
                            (for-each-except retractor
                                             inform-about-no-value
                                             constraints))
                     'ignored)) )
              ;;OLHARRR
              (connect
               (lambda (new-constraint)
                 (if (not (memq new-constraint constraints))
                     (set! constraints
                           (cons new-constraint constraints))
                     (set! constraints constraints) )
                 (if (has-value? me)
                     (inform-about-value new-constraint)
                     'done)
                 'done) )
              (me
               (lambda (request)
                 (cond ( (eq? request 'has-value?)
                         (if informant #t #f) )
                       ( (eq? request 'value) value )
                       ( (eq? request 'set-value!) set-my-value )
                       ( (eq? request 'forget) forget-my-value )
                       ( (eq? request 'connect) connect )
                       ;;( else (printf "Constraint ERROR: CONNECTOR ~a" request ) )
                       ) )) )
      me )) )
        
;;exceptions
(define (for-each-except exception procedure list)
  (letrec ( (loop
             (lambda (items)
               (cond ( (null? items) 'done)
                     ( (eq? (car items) exception)
                       (loop (cdr items)) )
                     ( else
                       (procedure (car items))
                       (loop (cdr items)) )) )) )
    (loop list)) )

