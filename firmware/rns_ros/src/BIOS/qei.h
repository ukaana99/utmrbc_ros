#ifndef BIOS_QEI_H_
#define BIOS_QEI_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************************
 * 		Include Libraries 			   *
 **************************************/

#include "system.h"
#include "gpio.h"
#include "uart.h"


/**************************************************
 * 		STRUCTURE DEFINES					  	  *
 *************************************************/

TIM_TimeBaseInitTypeDef     	BIOS_QEI1_TIM_TimeBaseStructure;
NVIC_InitTypeDef				BIOS_QEI1_NVIC_InitStructure;

TIM_TimeBaseInitTypeDef     	BIOS_QEI2_TIM_TimeBaseStructure;
NVIC_InitTypeDef				BIOS_QEI2_NVIC_InitStructure;

TIM_TimeBaseInitTypeDef     	BIOS_QEI3_TIM_TimeBaseStructure;
NVIC_InitTypeDef				BIOS_QEI3_NVIC_InitStructure;

TIM_TimeBaseInitTypeDef     	BIOS_QEI4_TIM_TimeBaseStructure;
NVIC_InitTypeDef				BIOS_QEI4_NVIC_InitStructure;

TIM_TimeBaseInitTypeDef     	BIOS_QEI5_TIM_TimeBaseStructure;
NVIC_InitTypeDef				BIOS_QEI5_NVIC_InitStructure;

TIM_TimeBaseInitTypeDef     	BIOS_QEI6_TIM_TimeBaseStructure;
NVIC_InitTypeDef				BIOS_QEI6_NVIC_InitStructure;

/**************************************************
 * 		DEFINES								  	  *
 *************************************************/


/**************************************************
 * 		Structure							  	  *
 *************************************************/

typedef struct{

	union{
		int32_t count;
		struct{
			uint16_t poscnt;
			int16_t  signbit;
		};
	};

}qei_TypeDef;

typedef struct{
	union{
		signed long LONG;
		struct{
			char BYTE1;
			char BYTE2;
			char BYTE3;
			signed char BYTE4;
		};
	}l[6];
}enc_TypeDef;

/**************************************************
 * 		Enumerator							  	  *
 *************************************************/
typedef enum {
	QEI1 = 1,
	QEI2 = 2,
	QEI3 = 3,
	QEI4 = 4,
	QEI5 = 5,
	QEI6 = 6
}QEI_TypeDef;

typedef enum{
	QEI_No_Swap = 0,
	QEI_Swap
}QEI_Direction_TypeDef;

/**************************************************
 * 		Extern	variables					  	  *
 *************************************************/
extern qei_TypeDef BIOS_QEI1;
extern qei_TypeDef BIOS_QEI2;
extern qei_TypeDef BIOS_QEI3;
extern qei_TypeDef BIOS_QEI4;
extern qei_TypeDef BIOS_QEI5;
extern qei_TypeDef BIOS_QEI6;
extern enc_TypeDef qei;

/**************************************************
 * 		Function Prototype			  			  *
 *************************************************/
void QEI1Init(uint16_t preemptionpriority, uint16_t subpriority,
		  GPIO_TypeDef* GPIOx_pulseA, uint16_t GPIO_Pin_pulseA, GPIO_TypeDef* GPIOx_pulseB, uint16_t GPIO_Pin_pulseB);
void QEI2Init(uint16_t preemptionpriority, uint16_t subpriority,
		  GPIO_TypeDef* GPIOx_pulseA, uint16_t GPIO_Pin_pulseA, GPIO_TypeDef* GPIOx_pulseB, uint16_t GPIO_Pin_pulseB);
void QEI3Init(uint16_t preemptionpriority, uint16_t subpriority,
		  GPIO_TypeDef* GPIOx_pulseA, uint16_t GPIO_Pin_pulseA, GPIO_TypeDef* GPIOx_pulseB, uint16_t GPIO_Pin_pulseB);
void QEI4Init(uint16_t preemptionpriority, uint16_t subpriority,
		  GPIO_TypeDef* GPIOx_pulseA, uint16_t GPIO_Pin_pulseA, GPIO_TypeDef* GPIOx_pulseB, uint16_t GPIO_Pin_pulseB);
void QEI5Init(uint16_t preemptionpriority, uint16_t subpriority,
		  GPIO_TypeDef* GPIOx_pulseA, uint16_t GPIO_Pin_pulseA, GPIO_TypeDef* GPIOx_pulseB, uint16_t GPIO_Pin_pulseB);
void QEI6Init(uint16_t preemptionpriority, uint16_t subpriority,
		  GPIO_TypeDef* GPIOx_pulseA, uint16_t GPIO_Pin_pulseA, GPIO_TypeDef* GPIOx_pulseB, uint16_t GPIO_Pin_pulseB);
void QEIInit(QEI_TypeDef QEIx, uint16_t preemptionpriority,uint16_t subpriority,
		  GPIO_TypeDef* GPIOx_pulseA, uint16_t GPIO_Pin_pulseA, GPIO_TypeDef* GPIOx_pulseB, uint16_t GPIO_Pin_pulseB);
void QEIPrintValueStore(USART_TypeDef* USARTx);
void QEIDelay(uint8_t value);
void QEIWrite(QEI_TypeDef QEIx, int32_t value);
void QEIReset(QEI_TypeDef QEIx);
void QEISwap(QEI_TypeDef QEIx, QEI_Direction_TypeDef swap);
int32_t QEIRead(QEI_TypeDef QEIx);
uint32_t QEIReadRaw(QEI_TypeDef QEIx);

#ifdef __cplusplus
}
#endif

#endif /* BIOS_QEI_H_ */
