/*
 * adc.c
 *
 *  Created on: 24 Jul 2017
 *      Author: user
 */

#include "adc.h"

void ADC_DMA_init(uint8_t ADC_Number, uint32_t ADC_Resolution,
		uint16_t *DMA_Memory0BaseAddr, uint16_t dmapreemptionpriority,
		uint16_t dmasubpriority) {
	/* DMA init */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	DMA_DeInit(DMA2_Stream2);
	while (DMA_GetCmdStatus(DMA2_Stream2) != DISABLE);

	ADC2_DMA_InitStructure.DMA_Channel = DMA_Channel_1;
	ADC2_DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &(ADC2->DR);
	ADC2_DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) DMA_Memory0BaseAddr;
	ADC2_DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	ADC2_DMA_InitStructure.DMA_BufferSize = ADC_Number;
	ADC2_DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	ADC2_DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	ADC2_DMA_InitStructure.DMA_PeripheralDataSize =
	DMA_PeripheralDataSize_HalfWord;
	ADC2_DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_HalfWord;
	ADC2_DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	ADC2_DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	ADC2_DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	ADC2_DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	ADC2_DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_INC4;
	ADC2_DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_INC4;
	DMA_Init(DMA2_Stream2, &ADC2_DMA_InitStructure);

	DMA_NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream2_IRQn;
	DMA_NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	DMA_NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = dmapreemptionpriority;
	DMA_NVIC_InitStructure.NVIC_IRQChannelSubPriority = dmasubpriority;
	NVIC_Init(&DMA_NVIC_InitStructure);
	DMA_ITConfig(DMA2_Stream2, DMA_IT_TC, ENABLE);
	DMA_Cmd(DMA2_Stream2,ENABLE);

	/* ADC init */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
	ADC2_InitStructure.ADC_Resolution = ADC_Resolution;
	ADC2_InitStructure.ADC_ScanConvMode = ENABLE;			/* ENABLE to make ADC to check for other input */
	ADC2_InitStructure.ADC_ContinuousConvMode = ENABLE;		/* ENABLE to make ADC auto sampling */
	ADC2_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC2_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC2_InitStructure.ADC_NbrOfConversion = ADC_Number;
	ADC_Init(ADC2, &ADC2_InitStructure);

	/* ADC Common Init */
	ADC2_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC2_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div8;
	ADC2_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_3;
	ADC2_CommonInitStructure.ADC_TwoSamplingDelay =
			ADC_TwoSamplingDelay_20Cycles;
	ADC_CommonInit(&ADC2_CommonInitStructure);

	ADC_Channel_Config(GPIOC, GPIO_Pin_1,ADC2, ADC_Channel_11, 1);
	ADC_Channel_Config(GPIOC, GPIO_Pin_2,ADC2, ADC_Channel_12, 2);

	ADC_DMACmd(ADC2, ENABLE);
	ADC_DMARequestAfterLastTransferCmd(ADC2, ENABLE);
	ADC_MultiModeDMARequestAfterLastTransferCmd(ENABLE);
	ADC_EOCOnEachRegularChannelCmd(ADC2, ENABLE);
	ADC_Cmd(ADC2, ENABLE);
	ADC_SoftwareStartConv(ADC2);
}

void ADC_Channel_Config(GPIO_TypeDef * GPIOx, uint16_t GPIO_Pin, ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t Rank) {
	GPIOPinsInit(GPIOx, GPIO_Pin, GPIO_Mode_AN, GPIO_OType_PP, GPIO_Speed_50MHz, GPIO_PuPd_NOPULL);
	ADC_RegularChannelConfig(ADCx, ADC_Channel, Rank, ADC_SampleTime_144Cycles);
}
