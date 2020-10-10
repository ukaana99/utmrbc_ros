#ifndef BIOS_GPIO_H_
#define BIOS_GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************************
 * 		Include Libraries 			   *
 **************************************/
#include "system.h"

/***************************************
 * 		Defines			 			   *
 **************************************/

#define AFConfig(a,b,c) 	switch((uint32_t)c){\
								case GPIO_Pin_0: GPIO_PinAFConfig(b, GPIO_PinSource0, GPIO_AF_##a);break;\
								case GPIO_Pin_1: GPIO_PinAFConfig(b, GPIO_PinSource1, GPIO_AF_##a);break;\
								case GPIO_Pin_2: GPIO_PinAFConfig(b, GPIO_PinSource2, GPIO_AF_##a);break;\
								case GPIO_Pin_3: GPIO_PinAFConfig(b, GPIO_PinSource3, GPIO_AF_##a);break;\
								case GPIO_Pin_4: GPIO_PinAFConfig(b, GPIO_PinSource4, GPIO_AF_##a);break;\
								case GPIO_Pin_5: GPIO_PinAFConfig(b, GPIO_PinSource5, GPIO_AF_##a);break;\
								case GPIO_Pin_6: GPIO_PinAFConfig(b, GPIO_PinSource6, GPIO_AF_##a);break;\
								case GPIO_Pin_7: GPIO_PinAFConfig(b, GPIO_PinSource7, GPIO_AF_##a);break;\
								case GPIO_Pin_8: GPIO_PinAFConfig(b, GPIO_PinSource8, GPIO_AF_##a);break;\
								case GPIO_Pin_9: GPIO_PinAFConfig(b, GPIO_PinSource9, GPIO_AF_##a);break;\
								case GPIO_Pin_10: GPIO_PinAFConfig(b, GPIO_PinSource10, GPIO_AF_##a);break;\
								case GPIO_Pin_11: GPIO_PinAFConfig(b, GPIO_PinSource11, GPIO_AF_##a);break;\
								case GPIO_Pin_12: GPIO_PinAFConfig(b, GPIO_PinSource12, GPIO_AF_##a);break;\
								case GPIO_Pin_13: GPIO_PinAFConfig(b, GPIO_PinSource13, GPIO_AF_##a);break;\
								case GPIO_Pin_14: GPIO_PinAFConfig(b, GPIO_PinSource14, GPIO_AF_##a);break;\
								case GPIO_Pin_15: GPIO_PinAFConfig(b, GPIO_PinSource15, GPIO_AF_##a);break;\
								default:break;\
							}

#define GPIOA_OUT			((word_t*) &GPIOA->ODR)
#define GPIOB_OUT			((word_t*) &GPIOB->ODR)
#define GPIOC_OUT			((word_t*) &GPIOC->ODR)
#define GPIOD_OUT			((word_t*) &GPIOD->ODR)
#define GPIOE_OUT			((word_t*) &GPIOE->ODR)
#define GPIOF_OUT			((word_t*) &GPIOF->ODR)
#define GPIOG_OUT			((word_t*) &GPIOG->ODR)

#define GPIOA_IN			((word_t*) &GPIOA->IDR)
#define GPIOB_IN			((word_t*) &GPIOB->IDR)
#define GPIOC_IN			((word_t*) &GPIOC->IDR)
#define GPIOD_IN			((word_t*) &GPIOD->IDR)
#define GPIOE_IN			((word_t*) &GPIOE->IDR)
#define GPIOF_IN			((word_t*) &GPIOF->IDR)
#define GPIOG_IN			((word_t*) &GPIOG->IDR)

/**************************************************
 * 		Function Prototype			  			  *
 *************************************************/

void GPIOClockSet (void);
void GPIOPinsInit (GPIO_TypeDef * GPIOx, uint16_t GPIO_Pin, GPIOMode_TypeDef GPIO_Mode, GPIOOType_TypeDef GPIO_OType, GPIOSpeed_TypeDef GPIO_Speed,  GPIOPuPd_TypeDef GPIO_PuPd);

#ifdef __cplusplus
}
#endif

#endif /* BIOS_GPIO_H_ */
