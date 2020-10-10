#ifndef BIOS_BIOS_H_
#define BIOS_BIOS_H_

#ifdef __cplusplus
extern "C" {
#endif

/************************************************
 * @Title   : BASIC INPUT OUTPUT SYSTEM (BIOS)
 * @Author  : Leonard Chin (Chin Hung Vui)
 * @Version : 1.40
 * @Date    : 26 June 2017
 * **********************************************
 * @Descriptions:
 *
 * 	+ A middle layer libraries for robot programmers to deal with basic settings of GPIOs, LEDs, TIMERs, PWMs, BDCs,
 *    SHIFT REGISTER, UARTs and QEIs.
 *
 * @Version History:
 *  V1.40 (edited by Kai Sheng on 11 August 2018)
 *  + modify the QEI libary (remove QEIHandler. QEI value will automatically update when calling the function QEIRead().)
 *  + add UARTDMA as one of the low-level object.
 *	V1.30 (edited by Kai Sheng on 7 May 2018)
 *	+ modify the QEI library (change the method of changing the direction of QEI from using flag to change settings.)
 * 	V1.20 (edited by Kai Sheng and Qiu Hui on 17 January 2018):
 * 	+ making the object SHIFT REGISTER support cascaded operation.
 * 	+ remove led.h and led.c as it support only the STM32F4Discovery Board.
 * 	+ make all GPIO pins of each object configurable by user.
 * 	+ make the frequency of PWM configurable by user.
 * 	+ make the the direction pin of BDC configurable by using either SHIFT REGISTER or GPIO.
 * 	+ make library more user friendly
 *  V1.10 (edited by Kai Sheng and Qiu Hui on 18 August 2017):
 * 	+ adding multiplexer as one of the low-level object.
 *  V1.00:
 * 	+ Contains libraries to process the following low-level objects:
 * 		a. GPIO
 *		b. LED
 * 		c. TIMER
 * 		d. PWM
 * 		e. BDC
 * 		f. SHIFT REGISTER
 * 		g. UART
 * 		h. QEI
 *
 * @Bugs:
 *
 *
 ************************************************/

#include "system.h"
#include "gpio.h"
#include "timer.h"
#include "pwm.h"
#include "bdc.h"
#include "uart.h"
#include "uartdma.h"
#include "qei.h"
#include "shiftreg.h"
#include "multiplexer.h"

#ifdef __cplusplus
}
#endif

#endif /* BIOS_BIOS_H_ */
