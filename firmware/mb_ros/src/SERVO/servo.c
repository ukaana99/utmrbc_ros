
/************************************************/
/*		 	 	Include Header	       		  	*/
/************************************************/

#include"servo.h"

/************************************************/
/*		 	 	Variables	      	 		  	*/
/************************************************/

/************************************************/
/*		 	 	Functions		       		  	*/
/************************************************/

/*
 * Function Name		: ServoInit
 * Function Description : This function is called to initialize servo.
 * Function Remarks		:
 * Function Arguments	: servo				structure to store data
 * 						  Timer				can be TIM1 to TIM14 except TIM6 and TIM7
 * 						  SERVO_GPIOx		GPIOx where x can be A to I.
 * 						  SERVO_GPIO_Pin	The desired pin that you want to configure in group GPIOx. This parameter
 * 						  					can be combination of GPIO_Pin_x where x can be (0..15)
 * 						  channel			channel1, channel2, channel3, or channel4
 * Function Return		: None
 * Function Example		: ServoInit(&SERVO1, TIM3, GPIOA,GPIO_Pin_6, channel1);
 */
void ServoInit(SERVO_t* servo, TIM_TypeDef* Timer, GPIO_TypeDef * SERVO_GPIOx,uint16_t SERVO_GPIO_Pin, uint8_t channel){
	switch((uint32_t)Timer){

		case TIM1_BASE: RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
						AFConfig(TIM1, SERVO_GPIOx, SERVO_GPIO_Pin);
						SERVO_TIM_TimeBaseStructure.TIM_Prescaler = 167;
						TIM_CtrlPWMOutputs(TIM1, ENABLE);

						break;

		case TIM2_BASE: RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
						AFConfig(TIM2, SERVO_GPIOx, SERVO_GPIO_Pin);
						SERVO_TIM_TimeBaseStructure.TIM_Prescaler = 83;


						break;

		case TIM3_BASE: RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
						AFConfig(TIM3, SERVO_GPIOx, SERVO_GPIO_Pin);
						SERVO_TIM_TimeBaseStructure.TIM_Prescaler = 83;

						break;

		case TIM4_BASE: RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
						AFConfig(TIM4, SERVO_GPIOx, SERVO_GPIO_Pin);
						SERVO_TIM_TimeBaseStructure.TIM_Prescaler = 83;

						break;

		case TIM5_BASE: RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
						AFConfig(TIM5, SERVO_GPIOx, SERVO_GPIO_Pin);
						SERVO_TIM_TimeBaseStructure.TIM_Prescaler = 83;

						break;

		case TIM8_BASE: RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
						AFConfig(TIM8, SERVO_GPIOx, SERVO_GPIO_Pin);
						SERVO_TIM_TimeBaseStructure.TIM_Prescaler = 167;
						TIM_CtrlPWMOutputs(TIM8, ENABLE);

						break;

		case TIM9_BASE: RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
						AFConfig(TIM9, SERVO_GPIOx, SERVO_GPIO_Pin);
						SERVO_TIM_TimeBaseStructure.TIM_Prescaler = 167;

						break;

		case TIM10_BASE: RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);
						AFConfig(TIM10, SERVO_GPIOx, SERVO_GPIO_Pin);
						SERVO_TIM_TimeBaseStructure.TIM_Prescaler = 167;

						break;

		case TIM11_BASE: RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11, ENABLE);
						AFConfig(TIM11, SERVO_GPIOx, SERVO_GPIO_Pin);
						SERVO_TIM_TimeBaseStructure.TIM_Prescaler = 167;

						break;

		case TIM12_BASE: RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);
						AFConfig(TIM12, SERVO_GPIOx, SERVO_GPIO_Pin);
						SERVO_TIM_TimeBaseStructure.TIM_Prescaler = 83;

						break;

		case TIM13_BASE: RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE);
						AFConfig(TIM13, SERVO_GPIOx, SERVO_GPIO_Pin);
						SERVO_TIM_TimeBaseStructure.TIM_Prescaler = 83;

						break;

		case TIM14_BASE: RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
						AFConfig(TIM14, SERVO_GPIOx, SERVO_GPIO_Pin);
						SERVO_TIM_TimeBaseStructure.TIM_Prescaler = 83;

						break;


		default: break;

	}
	GPIOPinsInit(SERVO_GPIOx, SERVO_GPIO_Pin, GPIO_Mode_AF, GPIO_OType_PP, GPIO_Speed_50MHz, GPIO_PuPd_UP);

	servo->TIMx = Timer;
	servo->TIMx_Channel = channel;

	SERVO_TIM_TimeBaseStructure.TIM_Period = 19999; //0.02s
	SERVO_TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	SERVO_TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(servo->TIMx, &SERVO_TIM_TimeBaseStructure);


	SERVO_TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	SERVO_TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	SERVO_TIM_OCInitStructure.TIM_Pulse = 0;
	SERVO_TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;

	if(servo->TIMx_Channel == channel1){
		TIM_OC1Init(servo->TIMx, &SERVO_TIM_OCInitStructure);
		TIM_OC1PreloadConfig(servo->TIMx, TIM_OCPreload_Enable);
	}else if(servo->TIMx_Channel == channel2){
		TIM_OC2Init(servo->TIMx, &SERVO_TIM_OCInitStructure);
		TIM_OC2PreloadConfig(servo->TIMx, TIM_OCPreload_Enable);
	}else if(servo->TIMx_Channel == channel3){
		TIM_OC3Init(servo->TIMx, &SERVO_TIM_OCInitStructure);
		TIM_OC3PreloadConfig(servo->TIMx, TIM_OCPreload_Enable);
	}else if(servo->TIMx_Channel == channel4){
		TIM_OC4Init(servo->TIMx, &SERVO_TIM_OCInitStructure);
		TIM_OC4PreloadConfig(servo->TIMx, TIM_OCPreload_Enable);
	}

	TIM_ARRPreloadConfig(servo->TIMx, ENABLE);
	TIM_Cmd(servo->TIMx, ENABLE);
}

/*
 * Function Name		: ServoSetPulse
 * Function Description : Sets the TIMx Capture Compare1,2,3,4 Register value
 * Function Remarks		: None
 * Function Arguments	: servo		structure to store data
 * 						  pulse		output compare pulse to be set
 * Function Return		: None
 * Function Example		: ServoSetPulse(&SERVO1, 1000);
 */

void ServoSetPulse(SERVO_t* servo, uint32_t pulse){

	if(servo->TIMx_Channel == channel1)

		TIM_SetCompare1(servo->TIMx, pulse);

	else if(servo->TIMx_Channel == channel2)

		TIM_SetCompare2(servo->TIMx, pulse);

	else if(servo->TIMx_Channel == channel3)

		TIM_SetCompare3(servo->TIMx, pulse);

	else if(servo->TIMx_Channel == channel4)

		TIM_SetCompare4(servo->TIMx, pulse);

}

/*
 * Function Name		: ServoInitAngle
 * Function Description : Initialize the angle of servo.
 * Function Remarks		: Support servo with angle 0-180 degree
 * Function Arguments	: servo				structure to store data
 * 						  pulse0degree		value of output compare pulse when servo is at 0 degree
 * 						  pulse180degree	value of output compare pulse when servo is at 180 degree
 * Function Return		: None
 * Function Example		: ServoInitAngle(&SERVO1, 800 , 2600);
 */

void ServoInitAngle(SERVO_t* servo, uint32_t pulse0degree , uint32_t pulse180degree){
	servo->SERVO_pulse0degree = pulse0degree;
	servo->SERVO_pulse1degree = (uint32_t)((pulse180degree - pulse0degree)/180);
}

/*
 * Function Name		: ServoSetAngle
 * Function Description : This function set the servo to desired angle.
 * Function Remarks		: User need to initialize the angle of servo by calling the function ServoInitAngle
 * 						  before using this function.
 * Function Arguments	: servo		structure to store data
 * 						  angle		angle of servo to be set
 * Function Return		: None
 * Function Example		: ServoSetAngle(&SERVO1, 90);
 */

void ServoSetAngle(SERVO_t* servo,uint8_t angle){

	servo->TIMx_Compare = servo->SERVO_pulse0degree + (servo->SERVO_pulse1degree)*angle;

	if(servo->TIMx_Channel == channel1)

		TIM_SetCompare1(servo->TIMx, servo->TIMx_Compare);

	else if(servo->TIMx_Channel == channel2)

		TIM_SetCompare2(servo->TIMx, servo->TIMx_Compare);

	else if(servo->TIMx_Channel == channel3)

		TIM_SetCompare3(servo->TIMx, servo->TIMx_Compare);

	else if(servo->TIMx_Channel == channel4)

		TIM_SetCompare4(servo->TIMx, servo->TIMx_Compare);
}
