#lang racket

(require libserialport)
(require "defs.rkt")
(require "crc16_calc.rkt")

(define (device port-name)
  (let ( (port-list (serial-ports))
         (baudrate "9600") )
    (let ( (port (port-exists? port-list port-name)) )
      (cond (port ;;port exists and the device is connected
             (let ( (call-string (string-append  "stty -F " port " cs8 " baudrate " ignbrk -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke noflsh -ixon -crtscts"))
                    (out (open-output-file port #:mode 'binary #:exists 'append))
                    (in  (open-input-file  port #:mode 'binary))
                    (buffer '())
                    (read-thread null)
                    (IO_hash (make-hash))
                    (decode-key '()) )

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
                   ( (list source master_add f_config f_gpio gpiox pinH pinL in_out)
                     (let ( (frame-key (string-join (list (number->string gpiox)
                                                   (number->string pinH)
                                                   (number->string pinL)
                                                   (number->string in_out)) ",")) )
                       (if (not (hash-has-key? IO_hash frame-key))
                           (hash-set! IO_hash
                                      frame-key
                                      (lambda() (GPIO_request source gpiox pinH pinL in_out out)))
                           (displayln "You already registered this peripheral!"))
                       ) )
                   ( (list source master_add f_IO gpiox pinH pinL in_out valueH valueL crcH crcL)
                     (displayln source)
                     (displayln master_add)
                     (displayln f_IO)
                     (displayln gpiox)
                     (displayln pinH)
                     (displayln pinL)
                     (displayln in_out)
                     (displayln valueH)
                     (displayln valueL)
                     (displayln crcH)
                     (displayln crcL) )
                   (a (displayln "displayln nao decodificavel!")
                      (displayln a) )
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
               (lambda(option [periph ""])
                 ;;(displayln buffer)
                 (cond ( (or (equal? option 'config) (equal? option "config"))
                         (let ( (keys (hash-keys IO_hash)) )
                           (for ( (key (in-list keys)) )
                             ;;(displayln key)
                             (display "Digite o nome do periferico ")
                             (displayln (GPIO_code->string key))
                             (set! decode-key (cons (list (read) key) decode-key))
                             )
                           )
                         decode-key
                         )
                       ( (equal? option 'use) (apply (hash-ref IO_hash (cadr (assoc periph decode-key)) ) '())  ) )
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

(define (GPIO_code->string code)
  (let ( (code-match (string-split code ",")) )
    (match code-match
      ( (list gpiox pinH pinL in_out)
        (let ( (gpiox-n  (string->number gpiox))
               (pinH-n   (string->number pinH))
               (pinL-n   (string->number pinL)) 
               (in_out-n (string->number in_out)) )
          (string-join
           (list
            (cond ( (= gpiox-n GPIOA) "GPIOA" )
                  ( (= gpiox-n GPIOB) "GPIOB" )
                  ( (= gpiox-n GPIOC) "GPIOC" )
                  ( (= gpiox-n GPIOD) "GPIOD" )
                  ( (= gpiox-n GPIOE) "GPIOE" )
                  (else "notFound" ) )
            (number->string (+ (arithmetic-shift pinH-n 8) pinL-n)) ;;decodificar com GPIO_Pin_x
            (cond ( (= in_out-n p_IN) "input" )
                  ( (= in_out-n p_OUT) "output" )
                  (else "notFound" ) ) )
           "_") )   )
      (_ "ERROR"))
    ) )
               

(define (GPIO_request dst gpiox pinH pinL in_out out)
  (let ( (source       master_add)
         (destination  dst)
         (periph       f_IO) )
    (let* ( (frame_list (list source destination periph gpiox pinH pinL in_out))
            (crc        (crc16_calc frame_list (length frame_list)))
            (crcH       (arithmetic-shift crc -8))
            (crcL       (bitwise-and crc #xff))
            (frame      (list->bytes (append frame_list (list crcH crcL FRAME_FLAG))))  )
      (write-bytes frame out)
      ))
  )

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