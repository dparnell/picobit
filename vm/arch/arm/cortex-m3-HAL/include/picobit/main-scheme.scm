;;File: main-scheme.scm - function in scheme
;;Cecilia Carneiro e Silva

;;SLEEP
;;sleep
(define (sleep time)
  (#%sleep time))

;;GPIO - defines
;;GPIOs
(define GPIOA 0)
(define GPIOB 1)
(define GPIOC 2)
(define GPIOD 3)
(define GPIOE 4)
;;Direction
(define DIR_output 'Output)
(define DIR_input  'Input)
(define DIR_af     'Af)
;;Mode
(define MODE_pp        'Push-pull)
(define MODE_od        'Open-drain)
(define MODE_analog    'Analog)
(define MODE_floating  'Floation)
(define MODE_pull-down 'Pull-down)
(define MODE_pull-up   'Pull-up)
;;Speed
(define SPEED_2  1)
(define SPEED_10 1)
(define SPEED_50 1)
(define SPEED_in 0)
;;Pin
(define Pin_0           #x0001) 
(define Pin_1           #x0002) 
(define Pin_2           #x0004) 
(define Pin_3           #x0008) 
(define Pin_4           #x0010) 
(define Pin_5           #x0020) 
(define Pin_6           #x0040) 
(define Pin_7           #x0080) 
(define Pin_8           #x0100) 
(define Pin_9           #x0200) 
(define Pin_10          #x0400) 
(define Pin_11          #x0800) 
(define Pin_12          #x1000) 
(define Pin_13          #x2000) 
(define Pin_14          #x4000) 
(define Pin_15          #x8000) 
(define Pin_All         #xFFFF) 
;;configure mode_direction
(define GPIO_Mode_AIN         #x0)
(define GPIO_Mode_IN_FLOATING #x04)
(define GPIO_Mode_IPD         #x28)
(define GPIO_Mode_IPU         #x48)
(define GPIO_Mode_Out_OD      #x14)
(define GPIO_Mode_Out_PP      #x10)
(define GPIO_Mode_AF_OD       #x1C)
(define GPIO_Mode_AF_PP       #x18)

;;Configure GPIO
(define (GPIO_init GPIOx direction mode speed pin)
  ;;enable clock
  (cond [(equal? GPIOx GPIOA)
         (GPIOA_clock)]
        [(equal? GPIOx GPIOB)
         (GPIOB_clock)]
        [(equal? GPIOx GPIOC)
         (GPIOC_clock)]
        [(equal? GPIOx GPIOD)
         (GPIOD_clock)]
        [(equal? GPIOx GPIOE)
         (GPIOE_clock)])
  ;;configure 
  (cond [(equal? direction DIR_output)
         (if (equal? mode MODE_pp)
             (GPIO_config GPIOx GPIO_Mode_Out_PP pin speed)
             (GPIO_config GPIOx GPIO_Mode_Out_OD pin speed))]
        
        [(equal? direction DIR_af)
         (if (equal? mode MODE_pp)
             (GPIO_config GPIOx GPIO_Mode_AF_PP pin speed)
             (GPIO_config GPIOx GPIO_Mode_AF_OD pin speed))]

        [(equal? direction DIR_input)
         (cond [(equal? mode MODE_analog)
                (GPIO_config GPIOx GPIO_Mode_AIN pin speed)]
               [(equal? mode MODE_floating)
                (GPIO_config GPIOx GPIO_Mode_IN_FLOATING pin speed)]
               [(equal? mode MODE_pull-down)
                (GPIO_config GPIOx GPIO_Mode_IPD pin speed)]
               [(equal? mode MODE_pull-up)
                (GPIO_config GPIOx GPIO_Mode_IPU pin speed)] )]) )

;;IO_read and IO_write
(define (IO_read gpiox pin)
 (#%IO_read gpiox pin))
(define (IO_write gpiox pin value)
  (#%IO_write gpiox pin value))


(define GPIO_example
  (lambda ()
    (#%GPIO_example)) )
