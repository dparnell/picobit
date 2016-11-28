#lang racket

(require syntax/parse racket/syntax)

(define-syntax-class table
  (pattern ((key value) ...)
           #:attr hashtable
           (for/hash [(k (syntax->datum #'(key ...)))
                      (v (syntax->datum #'(value ...)))]
             (values k v))
           #:attr [sorted-kv 1]
                    (delay
                     (printf "sorting!\n")
                     (sort (syntax->list #'((key value) ...))
                           <
                           #:key (lambda (kv) (cadr (syntax->datum kv)))))))

(syntax-parse #'((a 3) (b 2) (c 1))
    [t:table
     (map syntax->datum (syntax->list #'(t.key ...)))])

(syntax-parse #'((a 3) (b 2) (c 1))
    [t:table
     #'(t.sorted-kv ...)])

(syntax-parse #'((a 1) (b 2) (c 3))
    [t:table
     (attribute t.hashtable)])