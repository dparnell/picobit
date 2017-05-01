#ifndef __FRAMES__
#define __FRAMES__

#define FRAME_FLAG   0x7E
#define FRAME_MAX    14

#define buffer_next(pos) ( (pos) + 1)
#define buffer_prev(pos) ( (pos) - 1)

typedef struct s_IO_point{
  uint8_t gpiox;
  uint8_t pinH;
  uint8_t pinL;
  uint8_t in_out;  
  uint8_t value;
}IO_point;

typedef struct s_ADC_point{
  uint8_t channel;
}ADC_point;

typedef struct s_PWM_point{
  uint8_t timx;
  uint8_t channel;
  uint8_t period_duty;
}PWM_point;

typedef union s_point{
  IO_point  t_IO;
  ADC_point t_ADC;
  PWM_point t_PWM;
}t_point;

typedef struct s_frame{
  uint8_t pos;
  
  uint8_t src;
  uint8_t dst;
  uint8_t read_write;
  uint8_t periph;

  t_point point;

  uint16_t crc;
}t_frame;

typedef struct s_frame_response{
  uint8_t pos;
  
  uint8_t src;
  uint8_t dst;
  uint8_t read_write;
  uint8_t periph;

  t_point point;

  uint8_t valueH;
  uint8_t valueL;
  
  uint16_t crc;
}t_frame_response;

#endif 
