

#include "pwm.h"

/************************************************/
/*		 	 	Functions		       		  	*/
/************************************************/

/*
 * Function Name		: PWMTimeBaseInit
 * Function Description : This function is called to initialize time base of timer for PWM .
 * Function Remarks		: User can set the frequency of PWM by using the formula:
 *
 * 						  			frequency in Hz = (Timer_Clock)/((period)*(prescaler))
 *
 * Function Arguments	: Timer 		Timer use for PWM generation
 * 						  Period		Period value (1 to 65535)
 * 						  Prescaler 	Prescaler value to divide TIM clock (1 to 65535)
 * Function Return		: None
 * Function Example		: PWMTimeBaseInit(TIM1, 20000, 84);
 */

void PWMTimeBaseInit(TIM_TypeDef* Timer, uint32_t Period, uint32_t Prescaler){

	TIM_TimeBaseInitTypeDef     	PWM_TIM_TimeBaseStructure;

	switch((uint32_t)Timer){

			case TIM1_BASE: RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
							break;

			case TIM2_BASE: RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
							break;

			case TIM3_BASE: RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
							break;

			case TIM4_BASE: RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
							break;

			case TIM5_BASE: RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
							break;

			case TIM8_BASE: RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
							break;

			case TIM9_BASE: RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
							break;

			case TIM10_BASE: RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);
							break;

			case TIM11_BASE: RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11, ENABLE);
							break;

			case TIM12_BASE: RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);
							break;

			case TIM13_BASE: RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE);
							break;

			case TIM14_BASE: RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
							break;

			default: break;

		}

	PWM_TIM_TimeBaseStructure.TIM_Period = Period-1;
	PWM_TIM_TimeBaseStructure.TIM_Prescaler = Prescaler-1;
	PWM_TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	PWM_TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	PWM_TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(Timer, &PWM_TIM_TimeBaseStructure);
	TIM_Cmd(Timer, ENABLE);

}

/*
 * Function Name		: PWMChannelConfig
 * Function Description : This function is called to configure the channel of PWM.
 * Function Remarks		: None
 * Function Arguments	: Timer 		Timer use for PWM generation
 * 						  Channel		Channel of PWM:
 * 						  				PWM_CHANNEL_1
 * 						  				PWM_CHANNEL_2
 * 						  				PWM_CHANNEL_3
 * 						  				PWM_CHANNEL_4
 * 						  GPIOx			GPIOx group of PWM pin(x = A,B,C,D or E)
 * 						  GPIO_Pin_x	GPIO_Pin_x of PWM pin(x = 0,1,2,...or 15)
 * Function Return		: None
 * Function Example		: PWMChannelConfig(TIM1, PWM_CHANNEL_1, GPIOE, GPIO_Pin_9);
 */

void PWMChannelConfig(TIM_TypeDef* Timer, PWMChannel Channel , GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_x){

	TIM_OCInitTypeDef		PWM_TIM_OCInitStructure;

	GPIOPinsInit(GPIOx, GPIO_Pin_x, GPIO_Mode_AF, GPIO_OType_PP, GPIO_Speed_100MHz, GPIO_PuPd_UP);

	if(Timer == TIM1){
		AFConfig(TIM1, GPIOx, GPIO_Pin_x);
		PWM_TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
		TIM_CtrlPWMOutputs(TIM1, ENABLE);
	}else if(Timer == TIM2){
		AFConfig(TIM2, GPIOx, GPIO_Pin_x);
		PWM_TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	}else if(Timer == TIM3){
		AFConfig(TIM3, GPIOx, GPIO_Pin_x);
		PWM_TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	}else if(Timer == TIM4){
		AFConfig(TIM4, GPIOx, GPIO_Pin_x);
		PWM_TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	}else if(Timer == TIM5){
		AFConfig(TIM5, GPIOx, GPIO_Pin_x);
		PWM_TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	}else if(Timer == TIM8){
		AFConfig(TIM8, GPIOx, GPIO_Pin_x);
		PWM_TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
		TIM_CtrlPWMOutputs(TIM8, ENABLE);
	}else if(Timer == TIM9){
		AFConfig(TIM9, GPIOx, GPIO_Pin_x);
		PWM_TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	}else if(Timer == TIM10){
		AFConfig(TIM10, GPIOx, GPIO_Pin_x);
		PWM_TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	}else if(Timer == TIM11){
		AFConfig(TIM11, GPIOx, GPIO_Pin_x);
		PWM_TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	}else if(Timer == TIM12){
		AFConfig(TIM12, GPIOx, GPIO_Pin_x);
		PWM_TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	}else if(Timer == TIM13){
		AFConfig(TIM13, GPIOx, GPIO_Pin_x);
		PWM_TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	}else if(Timer == TIM14){
		AFConfig(TIM14, GPIOx, GPIO_Pin_x);
		PWM_TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	}


	PWM_TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;

	switch(Channel){

		case PWM_CHANNEL_1 :	TIM_OC1Init(Timer, &PWM_TIM_OCInitStructure);
								TIM_OC1PreloadConfig(Timer, TIM_OCPreload_Enable);
								break;

		case PWM_CHANNEL_2 :	TIM_OC2Init(Timer, &PWM_TIM_OCInitStructure);
								TIM_OC2PreloadConfig(Timer, TIM_OCPreload_Enable);
								break;

		case PWM_CHANNEL_3 :	TIM_OC3Init(Timer, &PWM_TIM_OCInitStructure);
								TIM_OC3PreloadConfig(Timer, TIM_OCPreload_Enable);
								break;

		case PWM_CHANNEL_4 :	TIM_OC4Init(Timer, &PWM_TIM_OCInitStructure);
								TIM_OC4PreloadConfig(Timer, TIM_OCPreload_Enable);
								break;

	}

	TIM_ARRPreloadConfig(Timer, ENABLE);

}

