#lang racket

(require libserialport)
(require "defs.rkt")
(require "crc16_calc.rkt")

(displayln "Use: ")
(displayln "> (define c (device \"USB0\"))")
(displayln "> (c 'config)")
(displayln ";;Example: ")
(displayln "> (c 'read 'bot)")
(displayln "> (c 'write 'led-verde 1)")


(define (device port-name)
  (let ( (port-list (serial-ports))
         (baudrate "9600") )
    (let ( (port (port-exists? port-list port-name)) )
      (cond (port ;;port exists and the device is connected
             (let ( (call-string (string-append  "stty -F " port " cs8 " baudrate " ignbrk -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke noflsh -ixon -crtscts"))
                    (out (open-output-file port #:mode 'binary #:exists 'append))
                    (in  (open-input-file  port #:mode 'binary))

                    (read-thread null)
                    (config_hash (make-hash))
                    (decode-key  '((none "default"))) )

               (define (read-begin)
                 (set! read-thread (thread (lambda() (read-frame))) )
                 (displayln "Master Beginning...") )
               
               (define (read-frame)
                 (let loop ( (frame '())
                             (u8-byte (read-byte in)) )
                   (cond ( (equal? u8-byte FRAME_FLAG) ;end of the frame
                           (decode-frame (reverse frame))
                           (set! read-thread (thread (lambda() (read-frame))))
                           (flush-output) )
                         (else 
                          (loop (cons u8-byte frame) (read-byte in)) )) ))

               (define (decode-frame frame)
                 ;;(displayln (list->bytes frame))
                 (match frame
                   ;;GPIO_config
                   ( (list source master_add f_config operation f_gpio gpiox pinH pinL in_out)
                     (let ( (frame-key (string-join (list
                                                     (number->string operation)
                                                     (number->string gpiox)
                                                     (number->string pinH)
                                                     (number->string pinL)
                                                     (number->string in_out)) ",")) )
                       (if (not (hash-has-key? config_hash frame-key))
                           (hash-set! config_hash
                                      frame-key
                                      (cond ( (= operation o_READ)
                                              (lambda() (GPIO_request o_READ source gpiox pinH pinL in_out out 0)) )
                                            ( (= operation o_WRITE)
                                              (lambda(v) (GPIO_request o_WRITE source gpiox pinH pinL in_out out v)) )
                                            ) )
                           (displayln "You already registered this peripheral!"))
                       ) )

                   ;;GPIO
                   ( (list source master_add operation f_IO gpiox pinH pinL in_out valueWrite valueH valueL crcH crcL)
                     (let ( (symbol (string-join (list
                                                     (number->string operation)
                                                     (number->string gpiox)
                                                     (number->string pinH)
                                                     (number->string pinL)
                                                     (number->string in_out)) ",")) )
                       (if (= operation o_READ)
                           (displayln (~a "Value of " (symbol->name symbol decode-key) " is " valueL))
                           (displayln (~a "You wrote " valueWrite " in " (symbol->name symbol decode-key) ))  ) )   )

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
               (lambda(option [name 'none] [value null])
                 (cond ( (or (equal? option 'config) (equal? option "config"))
                         (let ( (keys (hash-keys config_hash)) )
                           (for ( (key (in-list keys)) )
                             (display "Digite o nome do periférico: ")
                             (displayln (_code->string key))
                             (set! decode-key (cons (list (read) key) decode-key))
                             )
                           (hash-set! config_hash "default"
                                      (lambda ( (x null) ) (displayln "Not configured use: (<device-name> 'config) or name wrong")))
                           ))
                       ( (equal? option 'read)  (apply (hash-ref config_hash (name->symbol name decode-key) ) '())  )
                       ( (equal? option 'write) (apply (hash-ref config_hash (name->symbol name decode-key) ) (list value))  ) )
                 ))   )
            (else ;;port doesn't exists or the device isn't connected
             (displayln "Port doesn't exists or your microcontroller isn't plugged correctly!") ))
      ))
  )

(define (port-exists? port-list port-name)
  (let ( (port (filter-map (lambda(x) (and (string-contains? x port-name) x)) port-list)) )
    (if (null? port)
        #f
        (car port)) ))

(define (_code->string code)
  (let ( (code-match (string-split code ",")) )
    (match code-match
      ( (list operation gpiox pinH pinL in_out)
        (let ( (operation-n (string->number operation))
               (gpiox-n  (string->number gpiox))
               (pinH-n   (string->number pinH))
               (pinL-n   (string->number pinL)) 
               (in_out-n (string->number in_out)) )
          (string-join
           (list
            (cond ( (= operation-n o_READ) "READ")
                  ( (= operation-n o_WRITE) "WRITE") )
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
               

(define (GPIO_request operation dst gpiox pinH pinL in_out out value)
  (let ( (source       master_add)
         (destination  dst)
         (periph       f_IO) )
    (let* ( (frame_list (list source destination operation periph gpiox pinH pinL in_out value))
            (crc        (crc16_calc frame_list (length frame_list)))
            (crcH       (arithmetic-shift crc -8))
            (crcL       (bitwise-and crc #xff))
            (frame      (list->bytes (append frame_list (list crcH crcL FRAME_FLAG))))  )
      (write-bytes frame out)
      ))
  )

(define (name->symbol name decode-key)
  (let ( (symbol (filter-map (lambda(x)
                               (match x
                                 ( (list a b) (if (equal? a name) b #f) )
                                 ( _ #f )))  decode-key))  )
    (if (not (null? symbol)) (car symbol)
        "default") ))

(define (symbol->name symbol decode-key)
  (let ( (name (filter-map (lambda(x)
                               (match x
                                 ( (list a b) (if (equal? b symbol) a #f) )
                                 ( _ #f )))  decode-key))  )
    (if (not (null? name)) (car name)
        'none) ))
                
  