#ifndef BIOS_PWM_H_
#define BIOS_PWM_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************************
 * 		Include Libraries 			   *
 **************************************/

#include "system.h"
#include "gpio.h"


/**************************************************
 * 		Enumerator							  	  *
 *************************************************/

typedef enum{
	PWM_CHANNEL_1,
	PWM_CHANNEL_2,
	PWM_CHANNEL_3,
	PWM_CHANNEL_4
}PWMChannel;

/**************************************************
 * 		Function Prototype						 *
 *************************************************/

void PWMTimeBaseInit(TIM_TypeDef* Timer, uint32_t Period, uint32_t Prescaler);
void PWMChannelConfig(TIM_TypeDef* Timer, PWMChannel Channel , GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_x);

#ifdef __cplusplus
}
#endif

#endif /* BIOS_PWM_H_ */
