#ifndef BIOS_BDC_H_
#define BIOS_BDC_H_

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
	SHIFTREG,
	GPIO
}BCDDIRPINType;


/**************************************************
 * 		Structure							  	  *
 *************************************************/

typedef struct{

	/*Type of direction pin*/

	BCDDIRPINType Dirpintype;

	/*GPIO DIRECTION PIN*/

	GPIO_TypeDef *GPIOx_DIR1;
	uint16_t GPIO_Pin_DIR1;

	GPIO_TypeDef *GPIOx_DIR2;
	uint16_t GPIO_Pin_DIR2;

	/*Shift register direction pin*/

	byte_t* shiftreg;
	uint8_t dir1;
	uint8_t dir2;

	/*register for PWM duty cycle*/

	__IO uint32_t* speed;

}BDC_t;

/**************************************************
 * 		Function Prototype			  			  *
 *************************************************/

void BDCInit(BDC_t* bdc, __IO uint32_t* pwmaddress, BCDDIRPINType Dirpintype, ...);
void WriteBDC(BDC_t* bdc, int32_t pwm);
void StopBDC(BDC_t* bdc);

#ifdef __cplusplus
}
#endif

#endif /* BIOS_BDC_H_ */
