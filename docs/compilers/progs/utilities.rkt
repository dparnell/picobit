#lang racket

(define ast1.4 `(- 8))
(define ast1.1 `(+ (read) ,ast1.4))

;;match `(+ ,op1 ,op2)
(match ast1.1
  [`(,op ,child1 ,child2)
   (print op) (newline)
   (print child1) (newline)
   (print child2) (newline)])

;;is leaf?
;;> (leaf? '(- 8))
;;> (leaf? 3)
(define (leaf? exp)
  (match exp
    [(? fixnum?)  #t]
    [`(read)      #t]
    [`(- ,c1)     #f]
    [`(+ ,c1 ,c2) #f]
    [_ #f]))

;;is R0?
;;given an s-exp is R0
;;> (R0? '(+ (read) (- 8)))
;;> (R0? '(- (read) (+ 8)))
(define (R0? sexp)
  (match sexp
    [(? leaf?) #t]
    [`(- ,e) (R0? e)]
    [`(+ ,e1 ,e2)
     (and (R0? e1) (R0? e2))]
    [`(program ,e) (R0? e)]
    [_ #f]))

;;interpleter to R0
(define (interp-R0 e)
  (match e
    [(? fixnum?) e]
    [`(read)
     (let [(r (read))]
       (cond [(fixnum? r) r]
             [else (error 'interp-R0 "input not an integer" r)] ))]
    [`(- ,(app interp-R0 v))
     (- 0 v)]
    [`(+ ,(app interp-R0 v1) ,(app interp-R0 v2))
     (+ v1 v2)]
    [`(program ,(app interp-R0 v)) v]))

;;partial evaluator
;;optimizer R0->R0+
;;compute all that do not depend on inputs
(define (pe-arith e)
  (match e
    [(? fixnum?) e]
    [`(read) `(read)]
    [`(- ,(app pe-arith r))
     (pe-neg r)]
    [`(+ ,(app pe-arith r1) ,(app pe-arith r2))
     (pe-pos r1 r2)]
    [_
     (error 'pe-aritm "input not an R0" e)]))

(define (pe-neg r)
  (cond [(fixnum? r) (- 0 r)]
        [else `(- r)]) )
(define (pe-pos r1 r2)
  (cond [(and (fixnum? r1) (fixnum? r2)) (+ r1 r2)]
        [else `(+ ,r1 ,r2)]))

