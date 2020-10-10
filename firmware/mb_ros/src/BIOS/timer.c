

/***************************************
 * 		Include Libraries 			   *
 **************************************/
#include "timer.h"

/************************************************/
/*		 	 	Functions		       		  	*/
/************************************************/

/*
 * Function Name		: BIOSTIMx_Init
 * Function Description : This function is called to configure the TIMx's interrupt duration.
 * Function Remarks		: TIM2 - TIM7 & TIM 12 - 14 -> Tclk = 84Mhz
 * 						  TIM1, TIM8, TIM9, TIM10, TIM11 -> Tclk = 168MHz
 *
 * 						  ***************TO FIND Tout********************
 *
 * 						     Tout = ((period)*(prescaler))/Tclk
 *
 * 						  ***********************************************
 *
 * Function Arguments	: TIMx					TIMx can be TIM1 to TIM14
 * 						  period				period value (1 to 65535)
 * 						  prescaler 			prescaler value to divide TIM clock (1 to 65535)
 * 						  preemptionpriority    interrupt with higher preemption priority executed first
 * 						  subpriority			when 2 interrupts have similar preemption priorities, interrupt with higher
 * 						  						priority will be executed first. If 2 interrupts have similar preemption
 * 						  						subpriority, then the one comes first in the program will be executed first.
 * Function Return		: None
 * Function Example		: BIOSTIMx_Init (TIM1, 1000, 168, 1, 0); //1ms  interrupt
 * 						  BIOSTIMx_Init (TIM2, 1000, 84, 2, 0);  //1ms  interrupt
 * 						  BIOSTIMx_Init (TIM3, 20000, 84, 2, 1); //20ms interrupt
 */

void BIOSTIMx_Init(TIM_TypeDef* TIMx, uint16_t period, uint16_t prescaler,uint16_t preemptionpriority,uint16_t subpriority)
{
	switch((uint32_t)TIMx)
	{
		//APB2 TIM
		case TIM1_BASE:
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
			BIOS_NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn ;

			break;
		}
		case TIM8_BASE:
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
			BIOS_NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_TIM13_IRQn ;

			break;
		}
		case TIM9_BASE:
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
			BIOS_NVIC_InitStructure.NVIC_IRQChannel = TIM1_BRK_TIM9_IRQn ;

			break;
		}
		case TIM10_BASE:
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);
			BIOS_NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn ;

			break;
		}
		case TIM11_BASE:
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11, ENABLE);
			BIOS_NVIC_InitStructure.NVIC_IRQChannel = TIM1_TRG_COM_TIM11_IRQn ;

			break;
		}
		//APB1 TIM
		case TIM2_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
			BIOS_NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn ;

			break;
		}
		case TIM3_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
			BIOS_NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn ;

			break;
		}
		case TIM4_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
			BIOS_NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn ;

			break;
		}
		case TIM5_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
			BIOS_NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn ;

			break;
		}
		case TIM6_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
			BIOS_NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn ;

			break;
		}
		case TIM7_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
			BIOS_NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn ;

			break;
		}
		case TIM12_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);
			BIOS_NVIC_InitStructure.NVIC_IRQChannel = TIM8_BRK_TIM12_IRQn ;

			break;
		}
		case TIM13_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE);
			BIOS_NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_TIM13_IRQn ;

			break;
		}
		case TIM14_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
			BIOS_NVIC_InitStructure.NVIC_IRQChannel = TIM8_TRG_COM_TIM14_IRQn ;

			break;
		}

		default: break;
	}

	//Initialize TIMx
	BIOS_TIM_TimeBaseStructure.TIM_Period = period - 1;
	BIOS_TIM_TimeBaseStructure.TIM_Prescaler = prescaler - 1;
	BIOS_TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	BIOS_TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIMx, &BIOS_TIM_TimeBaseStructure);
	TIM_ClearITPendingBit(TIMx, TIM_IT_Update);
	TIM_ITConfig(TIMx,TIM_IT_Update,ENABLE);
	//NVIC Configuration
	BIOS_NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemptionpriority;
	BIOS_NVIC_InitStructure.NVIC_IRQChannelSubPriority = subpriority;
	BIOS_NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&BIOS_NVIC_InitStructure);
	//Enable TIMx
	TIM_Cmd(TIMx,ENABLE);
}

/*
 * Function Name		: BIOSTIMx_Delayus
 * Function Description : Delay using timer to produce a delay in microseconds
 * Function Remarks		: More accurate than BIOSDelay() and BIOSDelayLoop()
 * 						  TIMx				TIMx can be TIM1 to TIM14
 * 						  					Take note than TIM2 and TIM15 are 32 bits Timer,
 * 						  					can produces longer delay.
 * Function Arguments	: Delayus 			time length of delay in microseconds (1 to 65535)
 * Function Return		: None
 * Function Example		: BIOSTIMx_Delayus (TIM2, 2); //Delay 2us
 */

void BIOSTIMx_Delayus(TIM_TypeDef * TIMx, uint16_t Delayus)
{
    uint16_t  TIMCounter = Delayus;

	switch((uint32_t)TIMx)
	{
		//APB2 TIM
		case TIM1_BASE:
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
			BIOS_TIM_TimeBaseStructure.TIM_Prescaler = 168;
			break;
		}
		case TIM8_BASE:
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
			BIOS_TIM_TimeBaseStructure.TIM_Prescaler = 168;
			break;
		}
		case TIM9_BASE:
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
			BIOS_TIM_TimeBaseStructure.TIM_Prescaler = 168;
			break;
		}
		case TIM10_BASE:
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);
			BIOS_TIM_TimeBaseStructure.TIM_Prescaler = 168;
			break;
		}
		case TIM11_BASE:
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11, ENABLE);
			BIOS_TIM_TimeBaseStructure.TIM_Prescaler = 168;
			break;
		}
		//APB1 TIM
		case TIM2_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
			BIOS_TIM_TimeBaseStructure.TIM_Prescaler = 84;
			break;
		}
		case TIM3_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
			BIOS_TIM_TimeBaseStructure.TIM_Prescaler = 84;
			break;
		}
		case TIM4_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
			BIOS_TIM_TimeBaseStructure.TIM_Prescaler = 84;
			break;
		}
		case TIM5_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
			BIOS_TIM_TimeBaseStructure.TIM_Prescaler = 84;
			break;
		}
		case TIM6_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
			BIOS_TIM_TimeBaseStructure.TIM_Prescaler = 84;
			break;
		}
		case TIM7_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
			BIOS_TIM_TimeBaseStructure.TIM_Prescaler = 84;
			break;
		}
		case TIM12_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);
			BIOS_TIM_TimeBaseStructure.TIM_Prescaler = 84;
			break;
		}
		case TIM13_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE);
			BIOS_TIM_TimeBaseStructure.TIM_Prescaler = 84;
			break;
		}
		case TIM14_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
			BIOS_TIM_TimeBaseStructure.TIM_Prescaler = 84;
			break;
		}
		default: break;
	}

	BIOS_TIM_TimeBaseStructure.TIM_Period = 1;
	BIOS_TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	BIOS_TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIMx, &BIOS_TIM_TimeBaseStructure);

	TIM_Cmd(TIMx,ENABLE);
	TIM_SetCounter(TIMx, 65535 - TIMCounter);

	while (TIMCounter < 65535)
	{
		TIMCounter = TIM_GetCounter(TIMx);
	}

	TIM_Cmd(TIMx, DISABLE);
}

/*
 * Function Name		: BIOSTIMx_Delay100us
 * Function Description : Delay using timer to produce a delay in microseconds
 * Function Remarks		: More accurate than BIOSDelay() and BIOSDelayLoop()
 * 						  TIMx				TIMx can be TIM1 to TIM14
 * 						  					Take note than TIM2 and TIM15 are 32 bits Timer,
 * 						  					can produces longer delay.
 * Function Arguments	: Delay100us 			time length of delay in 100 microseconds (1 to 65535)
 * Function Return		: None
 * Function Example		: BIOSTIMx_Delay100us (TIM2, 2); //Delay 200us
 */

void BIOSTIMx_Delay100us(TIM_TypeDef * TIMx, uint16_t Delay100us)
{
    uint16_t  TIMCounter = Delay100us;

	switch((uint32_t)TIMx)
	{
		//APB2 TIM
		case TIM1_BASE:
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
			BIOS_TIM_TimeBaseStructure.TIM_Prescaler = 16800;
			break;
		}
		case TIM8_BASE:
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
			BIOS_TIM_TimeBaseStructure.TIM_Prescaler = 16800;
			break;
		}
		case TIM9_BASE:
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
			BIOS_TIM_TimeBaseStructure.TIM_Prescaler = 16800;
			break;
		}
		case TIM10_BASE:
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);
			BIOS_TIM_TimeBaseStructure.TIM_Prescaler = 16800;
			break;
		}
		case TIM11_BASE:
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11, ENABLE);
			BIOS_TIM_TimeBaseStructure.TIM_Prescaler = 16800;
			break;
		}
		//APB1 TIM
		case TIM2_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
			BIOS_TIM_TimeBaseStructure.TIM_Prescaler = 8400;
			break;
		}
		case TIM3_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
			BIOS_TIM_TimeBaseStructure.TIM_Prescaler = 8400;
			break;
		}
		case TIM4_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
			BIOS_TIM_TimeBaseStructure.TIM_Prescaler = 8400;
			break;
		}
		case TIM5_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
			BIOS_TIM_TimeBaseStructure.TIM_Prescaler = 8400;
			break;
		}
		case TIM6_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
			BIOS_TIM_TimeBaseStructure.TIM_Prescaler = 8400;
			break;
		}
		case TIM7_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
			BIOS_TIM_TimeBaseStructure.TIM_Prescaler = 8400;
			break;
		}
		case TIM12_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);
			BIOS_TIM_TimeBaseStructure.TIM_Prescaler = 8400;
			break;
		}
		case TIM13_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE);
			BIOS_TIM_TimeBaseStructure.TIM_Prescaler = 8400;
			break;
		}
		case TIM14_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
			BIOS_TIM_TimeBaseStructure.TIM_Prescaler = 8400;
			break;
		}
		default: break;
	}

	BIOS_TIM_TimeBaseStructure.TIM_Period = 1;
	BIOS_TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	BIOS_TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIMx, &BIOS_TIM_TimeBaseStructure);

	TIM_Cmd(TIMx,ENABLE);
	TIM_SetCounter(TIMx, 65535 - TIMCounter);

	while (TIMCounter < 65535)
	{
		TIMCounter = TIM_GetCounter(TIMx);
	}

	TIM_Cmd(TIMx, DISABLE);
}

/*
 * Function Name		: BIOSTIMx_Delayms
 * Function Description : Delay using timer to produce a delay in microseconds
 * Function Remarks		: More accurate than BIOSDelay() and BIOSDelayLoop()
 * 						  TIMx				TIMx can be TIM1 to TIM14
 * 						  					Take note than TIM2 and TIM15 are 32 bits Timer,
 * 						  					can produces longer delay.
 * Function Arguments	: Delayms 			time length of delay in milliseconds (1 to 65535)
 * Function Return		: None
 * Function Example		: BIOSTIMx_Delayms (TIM2, 20); //Delay 20ms
 */

void BIOSTIMx_Delayms(TIM_TypeDef * TIMx, uint32_t Delayms)
{
    uint32_t i=0;
    uint32_t nCount = Delayms;
	for(i=0;i<nCount;i++)
	{
		BIOSTIMx_Delay100us(TIMx,10);
	}
}


