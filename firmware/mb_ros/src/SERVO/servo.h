/************************************************
 * Title   : SERVO
 * Author  : Kai Sheng and Qiu Hui
 * Version : 1.00
 * Date    : 13 JULY 2017
 * **********************************************
 * Descriptions:
 *	- provide the function for controlling the servo.
 *
 * Version History:
 *
 *
 * Bugs:
 *
 ************************************************/

#ifndef SERVO_H_
#define SERVO_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************************
 * 		Include Libraries 			   *
 **************************************/

#include "BIOS/bios.h"

/**************************************************
 * 		STRUCTURE DEFINES					  	  *
 *************************************************/

TIM_TimeBaseInitTypeDef 	SERVO_TIM_TimeBaseStructure;
TIM_OCInitTypeDef 			SERVO_TIM_OCInitStructure;

/**************************************************
 * 		DEFINES								  	  *
 *************************************************/


/************************************************/
/*		 	 	Typedef	       		  			*/
/************************************************/


/**************************************************
 * 		Structure							  	  *
 *************************************************/


/**************************************************
 * 		Enumerator							  	  *
 *************************************************/
typedef enum {
	channel1 = 1,
	channel2,
	channel3,
	channel4
}TIM_ChDef;

typedef struct{
	TIM_TypeDef* TIMx;
	TIM_ChDef TIMx_Channel;
	uint32_t SERVO_pulse0degree;
	uint32_t SERVO_pulse1degree;
	uint32_t TIMx_Compare;
}SERVO_t;

/**************************************************
 * 		Extern	variables					  	  *
 *************************************************/

/**************************************************
 * 		Function Prototype			  			  *
 *************************************************/
void ServoInit(SERVO_t* servo, TIM_TypeDef* Timer, GPIO_TypeDef * SERVO_GPIOx,uint16_t SERVO_GPIO_Pin, uint8_t channel);
void ServoSetPulse(SERVO_t* servo, uint32_t pulse);
void ServoInitAngle(SERVO_t* servo, uint32_t pulse0degree , uint32_t pulse180degree);
void ServoSetAngle(SERVO_t* servo,uint8_t angle);

#ifdef __cplusplus
}
#endif

#endif /* SERVO_H_ */
