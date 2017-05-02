/*
Codigo internet = https://www.mikrocontroller.net/topic/233745
Testar e tentar usar
*/

#include <stdint.h>
#include <picobit.h>
#include <dispatch.h>
#include <primitives.h>
#include <bignum.h>
#include <stm32f10x.h>
#include <stm32f10x_dac.h>
#include <misc.h>


#define DAC1_VOLT  1.2f
#define DAC2_VOLT  3.0f
#define REF_VOLT  3.3f
#define DAC_ACC    4096.0f
const uint16_t _DAC1_DATA = (uint16_t)(((DAC1_VOLT / REF_VOLT) * DAC_ACC) + 0.5f);
const uint16_t _DAC2_DATA = (uint16_t)(((DAC2_VOLT / REF_VOLT) * DAC_ACC) + 0.5f);

void DacOn(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  DAC_InitTypeDef  DAC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
    DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
    DAC_Init(DAC_Channel_1, &DAC_InitStructure);
    DAC_Init(DAC_Channel_2, &DAC_InitStructure);

    DAC_Cmd(DAC_Channel_1, ENABLE);
    DAC_Cmd(DAC_Channel_2, ENABLE);

    DAC_SetChannel1Data(DAC_Align_12b_R, _DAC1_DATA);
    DAC_SetChannel2Data(DAC_Align_12b_R, _DAC2_DATA);

    DAC_SoftwareTriggerCmd(DAC_Channel_1, ENABLE);
    DAC_SoftwareTriggerCmd(DAC_Channel_2, ENABLE);
}
