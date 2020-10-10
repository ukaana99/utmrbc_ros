#ifndef BIOS_TIMER_H_
#define BIOS_TIMER_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************************
 * 		Include Libraries 			   *
 **************************************/
#include "system.h"

/**************************************************
 * 		Function Prototype			  			  *
 *************************************************/
void BIOSTIMx_Init(TIM_TypeDef* TIMx, uint16_t period, uint16_t prescaler,uint16_t preemptionpriority,uint16_t subpriority);
void BIOSTIMx_Delayus(TIM_TypeDef * TIMx, uint16_t Delayus);
void BIOSTIMx_Delayms(TIM_TypeDef * TIMx, uint32_t Delayms);
void BIOSTIMx_Delay100us(TIM_TypeDef * TIMx, uint16_t Delay100us);

#ifdef __cplusplus
}
#endif

#endif /* BIOS_TIMER_H_ */
