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
                    (decode-key  '()) )

               (define (read-begin)
                 (set! read-thread (thread (lambda() (read-frame))) )
                 (displayln "Master Beginning...") )
               
               (define (read-frame)
                 (let loop ( (frame '())
                             (u8-byte (read-byte in)) )
                   (cond ( (equal? u8-byte FRAME_FLAG) ;end of the frame
                           (decode-frame (reverse frame))
                           (set! read-thread (thread (lambda() (read-frame))))
                           (flush-output out)
                           )
                         (else 
                          (loop (cons u8-byte frame) (read-byte in)) )) ))

               (define (decode-frame frame)
                 (displayln frame)
                 (match frame
                   ;;GPIO_config
                   ( (list source master_add 10 #|f_config = 10|# operation f_IO gpiox pinH pinL in_out)
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
                           (displayln "You already registered this peripheral! - GPIO"))
                       ) )

                   ;;AD_config
                   ( (list source master_add 10 #|f_config = 10|# f_ADC channel posDMA)
                     (let ( (frame-key (string-join (list
                                                     (number->string f_ADC)
                                                     (number->string channel)) ","))  )
                       (if (not (hash-has-key? config_hash frame-key))
                           (hash-set! config_hash
                                      frame-key
                                      (lambda() (ADC_request o_READ source channel posDMA out)) )
                           (displayln "You already registered this peripheral! - ADC"))  )   )

                   ;;PWM_config
                   ( (list source master_add 10 #|f_config = 10|# operation f_PWM timx channel)
                     (let ( (frame-key (string-join (list
                                                     (number->string operation)
                                                     (number->string timx)
                                                     (number->string channel)) ",")) )
                       (if (not (hash-has-key? config_hash frame-key))
                           (hash-set! config_hash
                                      frame-key
                                      (cond ( (= operation o_READ)
                                              (lambda() (PWM_request o_READ source timx channel out 0)) )
                                            ( (= operation o_WRITE)
                                              (lambda(v) (PWM_request o_WRITE source timx channel out v)) )
                                            ) )
                           (displayln "You already registered this peripheral! - PWM"))
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

                   ;;ADC
                   ( (list source master_add operation 1 channel posDMA valueH valueL crcH crcL)
                     (let ( (symbol (string-join (list
                                                  (number->string f_ADC)
                                                  (number->string channel)) ",")) )
                       (displayln (~a "The value of " (symbol->name symbol decode-key) " - ADC - is "
                                      (bitwise-ior (arithmetic-shift valueH 8)
                                         valueL))) ) )

                   ;;PWM
                   ( (list source master_add operation f_PWM timx channel valueWriteH valueWriteL valueH valueL crcH crcL)
                     (let ( (symbol (string-join (list
                                                     (number->string operation)
                                                     (number->string timx)
                                                     (number->string channel)) ",")) )
                       (if (= operation o_READ)
                           (displayln (~a "Value of " symbol " is " (bitwise-ior (arithmetic-shift valueH 8) valueL)))
                           (displayln (~a "You wrote " (bitwise-ior (arithmetic-shift valueWriteH 8) valueWriteL) " in " symbol ))  ) )   )

                   
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
                       ( (equal? option 'show) config_hash )
                       ( (equal? option 'clean) (flush-output out) )
                       ( (equal? option 'read)
                         (with-handlers ([string? (lambda(v) v)]
                                         [exn:fail? (lambda(v) (displayln "I can't understand, please configure and/or check if the operation is correct."))])
                           (apply (hash-ref config_hash (name->symbol name decode-key) ) '()) )   )
                       ( (equal? option 'write)
                         (with-handlers ([string? (lambda(v) v)]
                                         [exn:fail? (lambda(v) (displayln "I can't understand, please configure and/or check if the operation is correct."))])
                           (apply (hash-ref config_hash (name->symbol name decode-key) ) (list value)) )   ) )
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
      ( (list adc channel)
        (string-join (list "ADC_channel" channel)) )

      ( (list operation timx channel)
        (string-join (list operation timx channel) "_") )
      
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
      (displayln (~a "GPIO " (bytes->list frame)))
      (write-bytes frame out)
      ))
  )

(define (ADC_request operation dst channel posDMA out)
  (let ( (source       master_add)
         (destination  dst)
         (periph       f_ADC) )
    (let* ( (frame_list (list source destination operation periph channel posDMA))
            (crc        (crc16_calc frame_list (length frame_list)))
            (crcH       (arithmetic-shift crc -8))
            (crcL       (bitwise-and crc #xff))
            (frame      (list->bytes (append frame_list (list crcH crcL FRAME_FLAG))))  )
      (displayln (~a "ADC " (bytes->list frame)))
      (write-bytes frame out)
      ))
  )

(define (PWM_request operation dst timx channel out value)
  (let ( (source       master_add)
         (destination  dst)
         (periph       f_PWM) )
    (let* ( (frame_list (list source destination operation periph timx (arithmetic-shift value -8)
                              (bitwise-and value #x00ff)
                              ))
            (crc        (crc16_calc frame_list (length frame_list)))
            (crcH       (arithmetic-shift crc -8))
            (crcL       (bitwise-and crc #xff))
            (frame      (list->bytes (append frame_list (list crcH crcL FRAME_FLAG))))  )
      (displayln (~a "PWM " (bytes->list frame)))
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
                
  