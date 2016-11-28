#lang racket
(require racket/match)
(require racket/fixnum)

(define assert
  (lambda (msg b)
    (if (not b)
	(begin
	  (display "ERROR: ")
	  (display msg)
	  (newline))
	(void))))

(define (lookup table s)
  (cond
    [(empty? table) (error (string-append s " not found"))]
    [else (if (string=? (first (first table)) s)
              (second (first table))
              (lookup (rest table) s))]))

(define (interp-R1 env)
  (lambda(e)
    (define recur (interp-R1 env))
    (match e
      [(? symbol?) (lookup e env)]
      [`(let [(,x ,(app recur v))] ,body)
       (define new-env (cons (cons x v) env))
       ((interp-R1 new-env) body)]
      [(? fixnum?) e]
      [`(read)
       (define r (read))
       (cond [(fixnum? r) r]
             [else (error 'interp-R1 "expected an integer")])]
      [`(- ,(app recur v))
       (- 0 v)]
      [`(+ ,(app recur v1) ,(app recur v2))
       (+ v1 v2)]
      [`(program ,e) ((interp-R1 '()) e)] )))

(define sample-table
  '(("matthias" 10)
    ("matthew"  20)
    ("robby"    -1)
    ("shriram"  18)))