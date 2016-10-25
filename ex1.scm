(define (pause n)
   (sleep 1000)
   (if (> n 0) (pause (- n 1)))) 


(define (loop)
	(set-led! 0)
	;;(led2-color 1)
  (pause 1)
  ;;(led2-color 0)
  (set-led! 1)
  (pause 1)
  (loop))

(loop)
