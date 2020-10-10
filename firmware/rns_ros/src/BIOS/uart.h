#ifndef BIOS_UART_H_
#define BIOS_UART_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************************
 * 		Include Libraries 			   *
 **************************************/
#include "system.h"
#include "gpio.h"

/**************************************************
 * 		STRUCTURE DEFINES					  	  *
 *************************************************/

NVIC_InitTypeDef 	BIOS_UART_NVIC_InitStructure;

/**************************************************
 * 		Function Prototype					  	  *
 *************************************************/
void UART1Init(uint32_t baudrate, FunctionalState rxstate, uint16_t preemptionpriority,uint16_t subpriority,
		  GPIO_TypeDef* GPIOx_Tx, uint16_t GPIO_Pin_Tx, GPIO_TypeDef* GPIOx_Rx, uint16_t GPIO_Pin_Rx);
void UART2Init(uint32_t baudrate, FunctionalState rxstate, uint16_t preemptionpriority,uint16_t subpriority,
		  GPIO_TypeDef* GPIOx_Tx, uint16_t GPIO_Pin_Tx, GPIO_TypeDef* GPIOx_Rx, uint16_t GPIO_Pin_Rx);
void UART3Init(uint32_t baudrate, FunctionalState rxstate, uint16_t preemptionpriority,uint16_t subpriority,
		  GPIO_TypeDef* GPIOx_Tx, uint16_t GPIO_Pin_Tx, GPIO_TypeDef* GPIOx_Rx, uint16_t GPIO_Pin_Rx);
void UART4Init(uint32_t baudrate, FunctionalState rxstate, uint16_t preemptionpriority,uint16_t subpriority,
		  GPIO_TypeDef* GPIOx_Tx, uint16_t GPIO_Pin_Tx, GPIO_TypeDef* GPIOx_Rx, uint16_t GPIO_Pin_Rx);
void UART5Init(uint32_t baudrate, FunctionalState rxstate, uint16_t preemptionpriority,uint16_t subpriority,
		  GPIO_TypeDef* GPIOx_Tx, uint16_t GPIO_Pin_Tx, GPIO_TypeDef* GPIOx_Rx, uint16_t GPIO_Pin_Rx);
void UART6Init(uint32_t baudrate, FunctionalState rxstate, uint16_t preemptionpriority,uint16_t subpriority,
		  GPIO_TypeDef* GPIOx_Tx, uint16_t GPIO_Pin_Tx, GPIO_TypeDef* GPIOx_Rx, uint16_t GPIO_Pin_Rx);
void UARTPrintChar(USART_TypeDef* USARTx, const char ch);
void UARTPrintString(USART_TypeDef* USARTx, volatile char *s);
char ReadUART(USART_TypeDef* USARTx);

#ifdef __cplusplus
}
#endif

#endif /* BIOS_UART_H_ */
