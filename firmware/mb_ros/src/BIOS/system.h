#ifndef BIOS_SYSTEM_H_
#define BIOS_SYSTEM_H_

/***************************************
 * 		Include Libraries 			   *
 **************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include <misc.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>



/***************************************
 * 		Compiler Configuration 		   *
 **************************************/
#ifndef __DEBUG
//#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"		//disable the warning of incompatible pointer type
#pragma GCC diagnostic ignored "-Wformat="							//disable the warning of incompatible format specifier
#pragma GCC diagnostic ignored "-Wunused-variable"					//disable the warning of unused variables
#pragma GCC diagnostic ignored "-Wmisleading-indentation"			//diasble the warning of misleading identation
#endif

/**************************************************
 * 		Global Variables Declaration  			  *
 *************************************************/
__IO uint32_t BIOSTimingDelay;

/**************************************************
 * 		STRUCTURE DEFINES					  	  *
 *************************************************/
RCC_ClocksTypeDef				BIOS_RCC_Clocks;
GPIO_InitTypeDef				BIOS_GPIO_InitStructure;
I2C_InitTypeDef  				I2C_InitStructure;
SPI_InitTypeDef 				SPI_InitStructure;
CAN_InitTypeDef					CAN_InitStructure;
CAN_FilterInitTypeDef			CAN_FilterInitStructure;
USART_InitTypeDef   			BIOS_USART_InitStructure;
TIM_TimeBaseInitTypeDef     	BIOS_TIM_TimeBaseStructure;
TIM_OCInitTypeDef				BIOS_TIM_OCInitStructure;
TIM_ICInitTypeDef 				BIOS_TIM_ICInitStructure;
EXTI_InitTypeDef                BIOS_EXTI_InitStructure;
DMA_InitTypeDef					BIOS_DMA_InitStructure;
NVIC_InitTypeDef				BIOS_NVIC_InitStructure;

/**************************************************
 * 					DEFINES					  	  *
 *************************************************/

#define Bit0 	((uint8_t)0b00000001)
#define Bit1	((uint8_t)0b00000010)
#define Bit2	((uint8_t)0b00000100)
#define Bit3	((uint8_t)0b00001000)
#define Bit4	((uint8_t)0b00010000)
#define Bit5	((uint8_t)0b00100000)
#define Bit6	((uint8_t)0b01000000)
#define Bit7	((uint8_t)0b10000000)

/**************************************************
 * 		Structure							  	  *
 *************************************************/
typedef struct{
	union{
		uint8_t Byte;
		struct{
			unsigned bit0		:	1;
			unsigned bit1		:	1;
			unsigned bit2		:	1;
			unsigned bit3		:	1;
			unsigned bit4		:	1;
			unsigned bit5		:	1;
			unsigned bit6		:	1;
			unsigned bit7		:	1;
		};
	};
}byte_t;

typedef struct{
	union{
		uint16_t Halfword;
		struct{
			unsigned bit0		:	1;
			unsigned bit1		:	1;
			unsigned bit2		:	1;
			unsigned bit3		:	1;
			unsigned bit4		:	1;
			unsigned bit5		:	1;
			unsigned bit6		:	1;
			unsigned bit7		:	1;
			unsigned bit8		:	1;
			unsigned bit9		:	1;
			unsigned bit10		:	1;
			unsigned bit11		:	1;
			unsigned bit12		:	1;
			unsigned bit13		:	1;
			unsigned bit14		:	1;
			unsigned bit15		:	1;
		};
	};
}halfword_t;

typedef struct{
	union{
		uint32_t Word;
		struct{
			unsigned bit0		:	1;
			unsigned bit1		:	1;
			unsigned bit2		:	1;
			unsigned bit3		:	1;
			unsigned bit4		:	1;
			unsigned bit5		:	1;
			unsigned bit6		:	1;
			unsigned bit7		:	1;
			unsigned bit8		:	1;
			unsigned bit9		:	1;
			unsigned bit10		:	1;
			unsigned bit11		:	1;
			unsigned bit12		:	1;
			unsigned bit13		:	1;
			unsigned bit14		:	1;
			unsigned bit15		:	1;
			unsigned bit16		:	1;
			unsigned bit17		:	1;
			unsigned bit18		:	1;
			unsigned bit19		:	1;
			unsigned bit20		:	1;
			unsigned bit21		:	1;
			unsigned bit22		:	1;
			unsigned bit23		:	1;
			unsigned bit24		:	1;
			unsigned bit25		:	1;
			unsigned bit26		:	1;
			unsigned bit27		:	1;
			unsigned bit28		:	1;
			unsigned bit29		:	1;
			unsigned bit30		:	1;
			unsigned bit31		:	1;
		};
	};
}word_t;

/**************************************************
 * 		Enumerators							  	  *
 *************************************************/
typedef enum {
	FAILED = 0,
	PASSED = !FAILED
}Status;

/**************************************************
 * 		Function Prototype			  			  *
 *************************************************/

void BIOSInit(void);
void BIOSDelay (__IO uint32_t nTime);
void BIOSDelayLoop (__IO uint32_t nTime);
void BIOSTimingDelay_Decrement(void);
void BIOSFPUEnable(void);

#ifdef __cplusplus
}
#endif

#endif /* BIOS_SYSTEM_H_ */
