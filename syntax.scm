;;#lang racket

;;(define me 'user)

(define (adder a1 a2 sum)                                                                                                                        
  (letrec ([process-new-value                                                                                                                    
            (lambda()                                                                                                                            
              (cond ((and (has-value? a1) (has-value? a2))                                                                                       
                     (set-value! sum                                                                                                             
                                 (+ (get-value a1) (get-value a2))                                                                               
                                 ))                                                                                                            
                    ((and (has-value? a1) (has-value? sum))                                                                                      
                     (set-value! a2                                                                                                              
                                 (- (get-value sum) (get-value a1))                                                                              
                                 ))                                                                                                            
                    ((and (has-value? a2) (has-value? sum))                                                                                      
                     (set-value! a1                                                                                                              
                                 (- (get-value sum) (get-value a2))                                                                              
                                 ))))]                                                                                                         
           [process-forget-value                                                                                                                 
            (lambda()                                                                                                                            
              (forget-value! sum)                                                                                                             
              (forget-value! a1)
              (forget-value! a2)
              (process-new-value))]                                                                                                              
           [me                                                                                                                                   
            (lambda (request)                                                                                                                    
              (cond ((eq? request 'I-have-a-value)                                                                                               
                     (process-new-value))                                                                                                        
                    ((eq? request 'I-lost-my-value)                                                                                              
                     (process-forget-value))                                                                                                     
                    (else                                                                                                                        
                     (error "Unknown request -- ADDER" request))))])                                                                             
    (connect a1)
    (connect a2)
    (connect sum)
    'user
    )) 

(define (has-value? connector)
  (connector 'has-value?))
(define (get-value connector)
  (connector 'value))
(define (set-value! connector new-value )
  ((connector 'set-value!) new-value ))
(define (forget-value! connector retractor)
  ((connector 'forget) retractor))
(define (connect connector )
  ((connector 'connect) ))
(define (set-min! connector minval)
  ((connector 'set-min!) minval))
(define (set-max! connector maxval)
  ((connector 'set-max!) maxval))

(define (for-each-except exception procedure list)
  (let loop ((items list))
    (cond ((null? items) 'done)
          ((eq? (car items) exception) (loop (cdr items)))
          (else (procedure (car items))
                (loop (cdr items)))))
  )

(define (inform-about-value constraint)
  (constraint 'I-have-a-value))
(define (inform-about-no-value constraint)
  (constraint 'I-lost-my-value))

#|
(define (adder a1 a2 sum)
  (letrec ([process-new-value
            (lambda()
              (cond ((and (has-value? a1) (has-value? a2))
                     (set-value! sum
                                 (+ (get-value a1) (get-value a2))
                                 me))
                    ((and (has-value? a1) (has-value? sum))
                     (set-value! a2
                                 (- (get-value sum) (get-value a1))
                                 me))
                    ((and (has-value? a2) (has-value? sum))
                     (set-value! a1
                                 (- (get-value sum) (get-value a2))
                                 me))))]
           [process-forget-value
            (lambda()
              (forget-value! sum me)
              (forget-value! a1 me)
              (forget-value! a2 me)
              (process-new-value))]
           [me
            (lambda (request)
              (cond ((eq? request 'I-have-a-value)
                     (process-new-value))
                    ((eq? request 'I-lost-my-value)
                     (process-forget-value))
                    (else
                     (error "Unknown request -- ADDER" request))))])
    (connect a1 me)
    (connect a2 me)
    (connect sum me)
    me))
            
(define (adder a1 a2 sum)
  (define (process-new-value)
    (cond ((and (has-value? a1) (has-value? a2))
           (set-value! sum
                       (+ (get-value a1) (get-value a2))
                       me))
          ((and (has-value? a1) (has-value? sum))
           (set-value! a2
                       (- (get-value sum) (get-value a1))
                       me))
          ((and (has-value? a2) (has-value? sum))
           (set-value! a1
                       (- (get-value sum) (get-value a2))
                       me))))
  (define (process-forget-value)
    (forget-value! sum me)
    (forget-value! a1 me)
    (forget-value! a2 me)
    (process-new-value))
  (define (me request)
    (cond ((eq? request 'I-have-a-value)
           (process-new-value))
          ((eq? request 'I-lost-my-value)
           (process-forget-value))
          (else
           (error "Unknown request -- ADDER" request))))
  (connect a1 me)
  (connect a2 me)
  (connect sum me)
  me)
|#

;;(define (loop)
;;  (letrec ((b 3)) 
;;    (let loop2 ((a '()))
;;      (GPIO-output "C" 8)
;;      (set-led! 8 #t)
;;      (b)
;;      (loop2))) )
