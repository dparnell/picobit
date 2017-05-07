#lang racket

(provide (all-defined-out))

(define master_add 0)

(define FRAME_FLAG #x7E)

(define f_IO      0)
(define f_ADC     1)
(define f_PWM     2)
(define f_DAC     3)
(define f_config 10)

(define o_READ    0)
(define o_WRITE   1)

;;GPIO
(define GPIOA   0)
(define GPIOB   1)
(define GPIOC   2)
(define GPIOD   3)
(define GPIOE   4)

(define p_IN   0)
(define p_OUT  1)

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

;;TIM
(define TIM_2   2)                                                                                                                                
(define TIM_3   3)                                                                                                                                
(define TIM_4   4)                                                                                                                                
(define TIM_5   5)

(define TIM_Channel_1                      #x0000)                                                                                                
(define TIM_Channel_2                      #x0004)                                                                                                
(define TIM_Channel_3                      #x0008)                                                                                                
(define TIM_Channel_4                      #x000C)
