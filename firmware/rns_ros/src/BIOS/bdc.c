

#include "bdc.h"

/************************************************/
/*		 	 	Functions		       		  	*/
/************************************************/

/*
 * Function Name		: BDCInit
 * Function Description : This function is called to initialize desired BDC.
 * Function Remarks		: User need to initialize PWM first before calling this function.
 * Function Arguments	: bdc					pointer to structure BDC_t
 * 						  pwmaddress			adress of pwm duty cycle register
 * 						  Dirpintype 			type of direction pin (GPIO or SHIFTREG)
 * 						  ...					direction pin
 * Function Return		: None
 * Function Example		: BDCInit(&BDC1, &TIM1->CCR1, SHIFTREG, &(SR.cast[0]), Bit1, Bit0);
 * 						  BDCInit(&BDC2, &TIM1->CCR2, GPIO, GPIOA, GPIO_Pin_9, GPIOA, GPIO_Pin_10);
 */

void BDCInit(BDC_t* bdc, __IO uint32_t* pwmaddress, BCDDIRPINType Dirpintype, ...){

	bdc->speed = pwmaddress;
	bdc->Dirpintype =  Dirpintype;

	va_list pinconfig;
	va_start(pinconfig, Dirpintype);

	if( bdc->Dirpintype == GPIO){

		bdc->GPIOx_DIR1 = va_arg(pinconfig, GPIO_TypeDef*);
		bdc->GPIO_Pin_DIR1	= va_arg(pinconfig, int);
		bdc->GPIOx_DIR2 = va_arg(pinconfig, GPIO_TypeDef*);
		bdc->GPIO_Pin_DIR2	= va_arg(pinconfig, int);

		GPIOPinsInit(bdc->GPIOx_DIR1, bdc->GPIO_Pin_DIR1, GPIO_Mode_OUT,GPIO_OType_PP,GPIO_Speed_50MHz, GPIO_PuPd_UP);
		GPIOPinsInit(bdc->GPIOx_DIR2, bdc->GPIO_Pin_DIR2, GPIO_Mode_OUT,GPIO_OType_PP,GPIO_Speed_50MHz, GPIO_PuPd_UP);

	}else if(bdc->Dirpintype == SHIFTREG){

		bdc->shiftreg = va_arg(pinconfig, byte_t *);
		bdc->dir1 = va_arg(pinconfig, int);
		bdc->dir2 = va_arg(pinconfig, int);
	}

	va_end(pinconfig);

}

/*
 * Function Name		: WriteBDC
 * Function Description : This function is called to write value to desired BDC.
 * Function Remarks		: None
 * Function Arguments	: bdc					pointer to structure BDC_t
 * 						  pwm					value from -20000 to 20000
 * Function Return		: None
 * Function Example		: WriteBDC(&BDC1,20000);
 */

void WriteBDC(BDC_t* bdc, int32_t pwm)
{

	if(pwm >= 0){

		*(bdc-> speed) = pwm;

		if( bdc->Dirpintype == GPIO){

			GPIO_WriteBit(bdc->GPIOx_DIR1 , bdc->GPIO_Pin_DIR1, Bit_SET);
			GPIO_WriteBit(bdc->GPIOx_DIR2 , bdc->GPIO_Pin_DIR2, Bit_RESET);

		}else if(bdc->Dirpintype == SHIFTREG){

			bdc->shiftreg->Byte |= bdc->dir1;
			bdc->shiftreg->Byte &= (uint8_t)~((uint8_t)bdc->dir2);
		}

	}else if (pwm < 0){

		*(bdc-> speed) = pwm*(-1);

		if( bdc->Dirpintype == GPIO){

			GPIO_WriteBit(bdc->GPIOx_DIR1 , bdc->GPIO_Pin_DIR1, Bit_RESET);
			GPIO_WriteBit(bdc->GPIOx_DIR2 , bdc->GPIO_Pin_DIR2, Bit_SET);

		}else if(bdc->Dirpintype == SHIFTREG){

			bdc->shiftreg->Byte &= (uint8_t)~((uint8_t)bdc->dir1);
			bdc->shiftreg->Byte |= bdc->dir2;

		}
	}
}

/*
 * Function Name		: StopBDC
 * Function Description : This function is called to stop desired BDC.
 * Function Remarks		: None
 * Function Arguments	: bdc		pointer to structure BDC_t
 * Function Return		: None
 * Function Example		: StopBDC(&BDC1);
 */

void StopBDC(BDC_t* bdc)
{
	*(bdc-> speed) = 0;

	if(bdc->Dirpintype == GPIO){

		GPIO_WriteBit(bdc->GPIOx_DIR1 , bdc->GPIO_Pin_DIR1, Bit_RESET);
		GPIO_WriteBit(bdc->GPIOx_DIR2 , bdc->GPIO_Pin_DIR2, Bit_RESET);

	}else if(bdc->Dirpintype == SHIFTREG){

		bdc->shiftreg->Byte &= (uint8_t)~((uint8_t)bdc->dir1);
		bdc->shiftreg->Byte &= (uint8_t)~((uint8_t)bdc->dir2);
	}
}

