#include <stdint.h>
#include <picobit.h>
#include <dispatch.h>
#include <primitives.h>
#include <bignum.h>
#include <stm32f10x.h>
#include <stm32f10x_tim.h>

#include <stm32f10x_gpio.h>

PRIMITIVE_UNSPEC(#%TIM_config, arch_TIM_config, 4)
{
  uint16_t u16_prescaler, u16_period, u16_instance, u16_counterMode;
  uint16_t u16_clockDivision = TIM_CKD_DIV1;

  TIM_TimeBaseInitTypeDef TIMx_config;
  TIM_TypeDef* TIMx = TIM2;
    
  u16_instance    = decode_int(arg1);
  u16_prescaler   = decode_int(arg2);
  u16_period      = decode_int(arg3);
  u16_counterMode = decode_int(arg4);

  if(u16_instance == 2){
    TIMx = TIM2;
  }
  else if(u16_instance == 3){
    TIMx = TIM3;
  }
  else if(u16_instance == 4){
    TIMx = TIM4;
  }
  else if(u16_instance == 5){
    TIMx = TIM5;
  }
  
  TIMx_config.TIM_Prescaler     = u16_prescaler;
  TIMx_config.TIM_Period        = u16_period;
  TIMx_config.TIM_CounterMode   = u16_counterMode;
  TIMx_config.TIM_ClockDivision = u16_clockDivision;

  TIM_TimeBaseInit(TIMx, &TIMx_config);

  TIM_ARRPreloadConfig(TIMx, ENABLE);

  /* Enable counting */
  TIM_Cmd(TIMx, ENABLE);
}

PRIMITIVE_UNSPEC(#%PWM_config, arch_PWM_config, 4)
{
  uint16_t u16_channel, u16_OCMode, u16_instance, u16_OCPolarity;
  uint16_t u16_outputState = TIM_OutputState_Enable, u16_pulse = 0;

  TIM_OCInitTypeDef PWM_channelx;
  TIM_TypeDef* TIMx = TIM2;
  
  u16_instance   = decode_int(arg1);
  u16_channel    = decode_int(arg2);
  u16_OCMode     = decode_int(arg3);
  u16_OCPolarity = decode_int(arg4);

  if(u16_instance == 2){
    TIMx = TIM2;
  }
  else if(u16_instance == 3){
    TIMx = TIM3;
  }
  else if(u16_instance == 4){
    TIMx = TIM4;
  }
  else if(u16_instance == 5){
    TIMx = TIM5;
  }
  
  PWM_channelx.TIM_OCMode      = u16_OCMode;
  PWM_channelx.TIM_OutputState = u16_outputState;
  PWM_channelx.TIM_Pulse       = u16_pulse;
  PWM_channelx.TIM_OCPolarity  = u16_OCPolarity;

  if(u16_channel == TIM_Channel_1){
    TIM_OC1Init(TIMx, &PWM_channelx);
    TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);
  }
  else if(u16_channel == TIM_Channel_2){
    TIM_OC2Init(TIMx, &PWM_channelx);
    TIM_OC2PreloadConfig(TIMx, TIM_OCPreload_Enable);
  }
  else if(u16_channel == TIM_Channel_3){
    TIM_OC3Init(TIMx, &PWM_channelx);
    TIM_OC3PreloadConfig(TIMx, TIM_OCPreload_Enable);
  }
  else if(u16_channel == TIM_Channel_4){
    TIM_OC4Init(TIMx, &PWM_channelx);
    TIM_OC4PreloadConfig(TIMx, TIM_OCPreload_Enable);
  }
}

PRIMITIVE_UNSPEC(#%PWM_set_duty, arch_PWM_set_duty, 3)
{
  uint16_t u16_instance, u16_channel, u16_value;
  TIM_TypeDef* TIMx = TIM2;
  
  u16_instance = decode_int(arg1);
  u16_channel  = decode_int(arg2);
  u16_value    = decode_int(arg3);

  if(u16_instance == 2){
    TIMx = TIM2;
  }
  else if(u16_instance == 3){
    TIMx = TIM3;
  }
  else if(u16_instance == 4){
    TIMx = TIM4;
  }
  else if(u16_instance == 5){
    TIMx = TIM5;
  }

  if(u16_channel == TIM_Channel_1){
    TIMx->CCR1 = u16_value;
  }
  else if(u16_channel == TIM_Channel_2){
    TIMx->CCR2 = u16_value;
  }
  else if(u16_channel == TIM_Channel_3){
    TIMx->CCR3 = u16_value;
  }
  else if(u16_channel == TIM_Channel_4){
    TIMx->CCR4 = u16_value;
  }
}
