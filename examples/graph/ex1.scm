(define G2 '(((A B) . 8) ((B E) . 9) ((E H) . 10)
             ((A C) . 1) ((C F) . 5) ((F H) . 7)
             ((A D) . 2) ((D G) . 3) ((G H) . 6)) )

(define (main)
  (GPIO_init GPIOA 'Input  'Pull-down SPEED_in Pin_0)

  (let ( (led-verde (led! Pin_9))
         (led-azul  (led! Pin_8))
         (ledd (led! Pin_4)) )

    (if (equal? (path-short 'A 'H G2) '(A D G H))
        (led-verde #t)
        (led-verde #f) )
    (sleep 100000)
    
    (ledd #t)
    (led-azul #t)
    (let loop ( (pressionado? (IO_read  GPIOA Pin_0)) )
      (if pressionado?
          (led-verde #t)
          (led-verde #f))
      (sleep 10000)
      (led-verde #f)
      (sleep 10000)
      (loop (IO_read  GPIOA Pin_0)) )) )

(main)

(define (path-short A B graph)
  (let loop ( (paths (paths-all A B graph)) (shorter '()) (short-value 1000000) )
    (if (null? paths) shorter
        (let ( (cost (path->cost (car paths) graph)) )
          (if (< cost short-value) (loop (cdr paths) (car paths) cost)
              (loop (cdr paths) shorter short-value) ))  ))
  )

(define (path->cost path graph)
  (if (or (null? path) (null? (cdr path))) 0
      (let loop ( (node1 (car path)) (node2 (cadr path)) (p (cdr path)) (acc 0))
        (if (null? (cdr p)) (+ acc (edge->cost (list node1 node2) graph))
            (let ( (edge (list node1 node2)) )
              (loop (car p) (cadr p) (cdr p) (+ acc (edge->cost edge graph)) )
              )
            ))
      ) )

(define (paths-all A B G)
  (intersect? (paths-beginning A G)
              (paths-ending B G)) )

(define (paths-beginning x graph)
  (loop-node x
             (let loop-link ( (linked-edges '()) (g graph) )
               (if (null? g)  linked-edges
                   (let ( (edge (caar g)) )
                     (if (equal? x (car edge))
                         (loop-link (cons (cadr edge) linked-edges) (cdr g))
                         (loop-link linked-edges (cdr g)) ))   )  )
             graph))

(define (loop-node x linked-edges graph)
  (let loop ( (l linked-edges) (acc '()) )
    (if (null? l) (begin
                    (cons (list x)
                          (map (lambda(e) (cons x e)) (apply-append acc))))
        (loop (cdr l) (cons (paths-beginning (car l) graph) acc)) )  ))


(define (paths-ending x graph)
  (let ( (graph-reverse (map
                         (lambda(a) (cons (reverse (car a)) (cdr a)))
                         graph)) )
    (map reverse (paths-beginning x graph-reverse))  ))

(define (member? element l)
  (cond ( (null? l) #f )
        ( else (or (equal? element (car l))
                   (member? element (cdr l))) )   ))

(define (intersect? paths1 paths2)
  (cond ( (null? paths1) '() )
        ( (member? (car paths1) paths2)
          (cons (car paths1)
                (intersect? (cdr paths1) paths2)) )
        ( else (intersect? (cdr paths1) paths2) )  ))

(define (edge->cost edge graph)
  (if (null? graph) 0
      (if (equal? edge (caar graph))
          (cdar graph)
          (edge->cost edge (cdr graph)) )) )

(define (apply-append path)
  (let loop ( (l path) (acc '()) )
    (if (null? l) acc
        (loop (cdr l) (append acc (car l))) ))
  )
