(define (main)
  (GPIO_init GPIOC 'Output 'Push-pull SPEED_50 Pin_9)
  (GPIO_init GPIOC 'Output 'Push-pull SPEED_50 Pin_8)
  (GPIO_init GPIOA 'Input  'Pull-down SPEED_in Pin_0)
  (GPIO_example))

(main)
