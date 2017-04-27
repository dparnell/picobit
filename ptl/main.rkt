#lang racket

(require libserialport)
(require "defs.rkt")

(define (device port-name device-name)
  (let ( (port-list (serial-ports))
         (baudrate "9600") )
    (let ( (port (port-exists? port-list port-name)) )
      (cond (port ;;port exists and the device is connected
             (let ( (call-string (string-append  "stty -F " port " cs8 " baudrate " ignbrk -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke noflsh -ixon -crtscts"))
                    (out (open-output-file port #:mode 'binary #:exists 'append))
                    (in  (open-input-file  port #:mode 'binary))
                    (buffer '())
                    (read-thread null)
                    (IO_hash (make-hash)) )

               (define (read-begin)
                 (set! read-thread (thread (lambda() (read-frame))) )
                 "Master Beginnig..." )
               
               (define (read-frame)
                 (let loop ( (frame '())
                             (u8-byte (read-byte in)) )
                   (cond ( (equal? u8-byte FRAME_FLAG) ;end of the frame
                           (decode-frame (reverse frame))
                           (set! buffer (cons (reverse frame) buffer))
                           ;;(kill-thread read-thread) ;;with this doesn't work
                           (set! read-thread (thread (lambda() (read-frame))))
                           (flush-output) )
                         (else 
                          (loop (cons u8-byte frame) (read-byte in)) )) ))

               (define (decode-frame frame)
                 ;;(displayln (list->bytes frame))
                 (match frame
                   ( (list source destination config periph_config gpiox pinH pinL in_out)
                     (hash-set! IO_hash (number->string (random 100)) (lambda() (GPIO_request source gpiox pinH pinL in_out out)))
                     )
                   (_ (displayln "entreiii"))
                   )
                 )
               
               (file-stream-buffer-mode out 'none)
               (sleep 3)
               (if (system call-string)
                   (begin
                     (sleep 1)
                     (read-begin)
                     )
                   "System Call Error.")
               (lambda(ab)
                 (displayln buffer)
                 IO_hash
                 ;;(cond ( (equal? ab "teste")
                 ;;        (apply (hash-ref IO_hash ab) '()) ))
                 ;;(write-bytes (list->bytes (append (map char->integer (string->list "oi mundo\r\n")) '(#x7E))) out)
                 ;;(write-bytes (bytes #x00 #x01 #x00 #x02 #x01 #x00 #x01 #x7e) out)
                 )
               
               ))
            (else
             (displayln "Port doesn't exists or your microcontroller isn't plugged correctly!") ))
      ))
  )

(define (port-exists? port-list port-name)
  (let ( (port (filter-map (lambda(x) (and (string-contains? x port-name) x)) port-list)) )
    (if (null? port)
        #f
        (car port)) ))

(define (GPIO_request dst gpiox pinH pinL in_out out)
  (let ( (source       master_add)
         (destination  dst)
         (periph       f_IO) )
    (let ( (frame (bytes source destination periph gpiox pinH pinL in_out FRAME_FLAG)) )
      (write-bytes frame out)
      ;;(write-bytes (bytes #x00 #x01 #x00 #x02 #x01 #x00 #x01 #x7e) out)
      )) )

#|(define (get-port)
  (car (serial-ports)) )

(define call-system (string-append  "stty -F " (get-port) " cs8 9600 ignbrk -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke noflsh -ixon -crtscts"))
;;> call-system
;;"stty -F /dev/ttyUSB0 cs8 9600 ignbrk -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke noflsh -ixon -crtscts"
(define port-name (get-port))
(define out (open-output-file port-name #:mode 'binary #:exists 'append))
(define  in  (open-input-file  port-name #:mode 'binary))
(file-stream-buffer-mode out 'none)
(system call-system)

(define abc "")

(define (read-msg)
  (let loop ( (li '())
              (caracter (read-byte in)) )
    (if (equal? caracter 10)
        (displayln (list->bytes (reverse li)))
        (loop (cons caracter li) (read-byte in)) )) )

(define (process-input str)
  (printf "\n------------------------\n")
  (printf "processing input ~a\n" str)
  (set! str (substring str 8))
  (cond 


  	( (string=? (string-upcase str) "MOVE RIGHT")
          (printf "I'm rotating to the right direction for one second\n")
	  )
        ( (string=? (string-upcase str) "MOVE LEFT")
          (printf "I'm rotating to the left direction for one second\n")
	  )
        (else
         (printf "Sorry I cannot understand: ~a\n" str)
         (flush-output)
         )
        )
  )

(define (read-loop)
  (define read-thread (thread (lambda() (read-msg))) )
  (define str (read-line))
  (unless (eof-object? str)
               (process-input str)
    )
  ;;(displayln abc)
  (read-loop))

(define (start-everything)
  (read-loop)
  )
|#