

#include "gpio.h"

/************************************************/
/*		 	 	Functions		       		  	*/
/************************************************/

/*
 * Function Name		: GPIOClockSet
 * Function Description : This function is called to enable all the Peripheral clock of GPIOs.
 * Function Remarks		: Do not called this function when not using all the GPIO Groups for power saving purpose.
 * Function Arguments	: None
 * Function Return		: None
 * Function Example		: GPIOClockSet();
 */

void GPIOClockSet (void)
{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
}

/*
 * Function Name		: GPIOPinsInit
 * Function Description : This function is called to configure the GPIO Pins of STM32F4 Discovery Board.
 * Function Remarks		: This function should be called in adapter.c only.
 * Function Arguments	: GPIOx							where x can be A to I.
 * 						  GPIO_Pin						The desired pin that you want to configure in group GPIOx. This parameter
 * 						  								can be combination of GPIO_Pin_x where x can be (0..15)
 * 						  GPIO_Mode						the value can be one of the following value:
 * 						  								GPIO_Mode_IN
 * 						  								GPIO_Mode_OUT
 * 						  								GPIO_Mode_AF
 * 						  								GPIO_Mode_AN
 * 						  GPIO_OType					Specifies the operating output type for the selected pins:
 * 						  								GPIO_OType_PP
 *														GPIO_OType_OD
 *						  GPIO_Speed					Specifies the speed for the selected pins:
 *						  								GPIO_Speed_2MHz
 *														GPIO_Speed_25MHz
 * 														GPIO_Speed_50MHz
 * 														GPIO_Speed_100MHz
 * 						  GPIO_PuPd						operating Pull-up/Pull down for the selected pins
 * 						  								GPIO_PuPd_NOPULL
 *														GPIO_PuPd_UP
 *														GPIO_PuPd_DOWN
 * Function Return		: None
 * Function Example		: GPIOPinsInit(GPIOA, GPIO_Pin_2, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_Speed_50MHz, GPIO_PuPd_UP);
 */

void GPIOPinsInit (GPIO_TypeDef * GPIOx, uint16_t GPIO_Pin, GPIOMode_TypeDef GPIO_Mode, GPIOOType_TypeDef GPIO_OType, GPIOSpeed_TypeDef GPIO_Speed,  GPIOPuPd_TypeDef GPIO_PuPd)
{
	switch((uint32_t)GPIOx){

	    case GPIOA_BASE:	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	    					break;

		case GPIOB_BASE:	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	    					break;

		case GPIOC_BASE:	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	    					break;

		case GPIOD_BASE:	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	    					break;

		case GPIOE_BASE:	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	    					break;

		case GPIOF_BASE:	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	    					break;

		case GPIOG_BASE:	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	    					break;

		case GPIOH_BASE:	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);
	    					break;

		case GPIOI_BASE:	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE);
	    					break;

		default: break;
	  }

		BIOS_GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin;
		BIOS_GPIO_InitStructure.GPIO_Mode	= GPIO_Mode;
		BIOS_GPIO_InitStructure.GPIO_OType	= GPIO_OType;
		BIOS_GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd;
		BIOS_GPIO_InitStructure.GPIO_Speed  = GPIO_Speed;
		GPIO_Init(GPIOx,&BIOS_GPIO_InitStructure);
}



