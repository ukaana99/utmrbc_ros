/*
 * adc.h
 *
 *  Created on: 24 Jul 2017
 *      Author: user
 */
/*				Pin		Channel
 * ADC1/2/3		PA0		0
 * 				PA1		1
 * 				PA2		2
 * 				PA3		3
 * 				PC0		10
 * 				PC1		11
 * 				PC2		12
 * 				PC3		13
 * ADC1/2		PA4		4
 * 				PA5		5
 * 				PA6		6
 * 				PA7		7
 * 				PB0		8
 * 				PB1		9
 * 				PC4		14
 * 				PC5		15
 *
 *				Pin		Channel
 * ADC1			PA0		0
 *				PA1		1
 *				PA2		2
 *				PA3		3
 *				PA4		4
 *				PA5		5
 *				PA6		6
 *				PA7		7
 *				PB0		8
 *				PB1		9
 *				PC0		10
 *				PC1		11
 *				PC2		12
 *				PC3		13
 *				PC4		14
 *				PC5		15
 *
 *
 *
 * */

#ifndef ADC_ADC_H_
#define ADC_ADC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "BIOS/bios.h"

ADC_InitTypeDef       ADC2_InitStructure;
ADC_CommonInitTypeDef ADC2_CommonInitStructure;
DMA_InitTypeDef       ADC2_DMA_InitStructure;
NVIC_InitTypeDef 	  DMA_NVIC_InitStructure;

void ADC_DMA_init(uint8_t ADC_Number, uint32_t ADC_Resolution,
		uint16_t *DMA_Memory0BaseAddr, uint16_t dmapreemptionpriority,
		uint16_t dmasubpriority) ;
void ADC_DMA_init(uint8_t, uint32_t, uint16_t*, uint16_t, uint16_t);
void ADC_Channel_Config(GPIO_TypeDef *, uint16_t,ADC_TypeDef*, uint8_t, uint8_t);
uint16_t TM_ADC_Read(ADC_TypeDef*, uint8_t);

#ifdef __cplusplus
}
#endif

#endif /* ADC_ADC_H_ */
