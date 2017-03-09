#include <stdint.h>
#include <picobit.h>
#include <dispatch.h>
#include <primitives.h>
#include <bignum.h>
#include <stm32f10x.h>
#include <stm32f10x_adc.h>
#include <stm32f10x_dma.h>

#define ADC1_DR_Address    ((uint32_t)0x4001244C)

__IO uint16_t ADCConvertedValue[16];


PRIMITIVE_UNSPEC(ADC1_clock, arch_ADC1_clock, 0)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
}

PRIMITIVE_UNSPEC(DMA1_clock, arch_DMA1_clock, 0)
{
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
}

PRIMITIVE_UNSPEC(#%DMA_config, arch_DMA_config, 1)
{
  DMA_InitTypeDef DMA_InitStructure;
  uint16_t u16_bufSize;

  u16_bufSize = decode_int(arg1);
  
  DMA_DeInit(DMA1_Channel1);

  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
  DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t)ADCConvertedValue;
  DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize         = u16_bufSize;
  DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  DMA_Cmd(DMA1_Channel1, ENABLE);
}

PRIMITIVE_UNSPEC(#%ADC_config, arch_ADC_config, 4)
{
  ADC_InitTypeDef ADC_InitStructure;
 
  uint32_t u32_scanMode, u32_contMode, u32_dma, u32_nChannel;
  uint32_t u32_mode = ADC_Mode_Independent, u32_dataAlign = ADC_DataAlign_Right, u32_externalTrig = ADC_ExternalTrigConv_None;
  
  u32_scanMode     = decode_int(arg1);
  u32_contMode     = decode_int(arg2);
  u32_dma          = decode_int(arg3);
  u32_nChannel     = decode_int(arg4);

  ADC_InitStructure.ADC_Mode               = u32_mode;
  ADC_InitStructure.ADC_ScanConvMode       = u32_scanMode;
  ADC_InitStructure.ADC_ContinuousConvMode = u32_contMode;
  ADC_InitStructure.ADC_ExternalTrigConv   = u32_externalTrig;
  ADC_InitStructure.ADC_DataAlign          = u32_dataAlign;
  ADC_InitStructure.ADC_NbrOfChannel       = u32_nChannel;
  ADC_Init(ADC1, &ADC_InitStructure);

  if(u32_dma == ENABLE){
    ADC_DMACmd(ADC1, ENABLE);
  }
 
  ADC_Cmd(ADC1, ENABLE);

  ADC_ResetCalibration(ADC1);

  while(ADC_GetResetCalibrationStatus(ADC1));
  ADC_StartCalibration(ADC1);

  while(ADC_GetCalibrationStatus(ADC1));

  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

PRIMITIVE(ADC_configChannel, arch_ADC_configChannel, 3)
{
  uint16_t u16_channel, u16_sampleTime, u16_pos;

  u16_channel    = decode_int(arg1);
  u16_sampleTime = decode_int(arg2);
  u16_pos        = decode_int(arg3);
  
  ADC_RegularChannelConfig(ADC1, u16_channel, u16_pos, u16_sampleTime);
}  

PRIMITIVE(ADC_startConversion, arch_ADC_startConversion, 0)
{
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

PRIMITIVE(ADC_readValue-DMA, arch_ADC_readValue_DMA, 2)
{
  uint16_t u16_cont, u16_pos;
  
  u16_cont = decode_int(arg1);
  u16_pos  = decode_int(arg2);

  if(u16_cont == DISABLE){
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
  }

  arg1 = encode_int(ADCConvertedValue[u16_pos]);
}

PRIMITIVE(ADC_readValue, arch_ADC_readValue, 1)
{  
  uint16_t u16_cont;
  
  u16_cont = decode_int(arg1);
 
  if(u16_cont == DISABLE){
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
  }
  
  arg1 = encode_int(ADC_GetConversionValue(ADC1));
}
