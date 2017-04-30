#include <stdint.h>
#include <picobit.h>
#include <dispatch.h>
#include <primitives.h>
#include <bignum.h>
#include <stm32f10x.h>
#include <stm32f10x_rcc.h>

PRIMITIVE_UNSPEC(clock_enable, arch_clock_enable, 3)
{

  uint16_t u16_APBH, u16_APBL, u16_APBp;
  uint32_t u32_APB;
  
  u16_APBH = decode_int(arg1);
  u16_APBL = decode_int(arg2);
  u16_APBp = decode_int(arg3);

  u32_APB  = (u16_APBH << 16) + u16_APBL;

  if(u16_APBp == 1){
    RCC_APB1PeriphClockCmd(u32_APB, ENABLE);
  }
  else if (u16_APBp == 2){
    RCC_APB2PeriphClockCmd(u32_APB, ENABLE);
  }
  else if(u16_APBp == 0){
    RCC_AHBPeriphClockCmd(u32_APB, ENABLE);
  }
}
