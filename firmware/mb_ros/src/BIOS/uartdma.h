
#ifndef BIOS_UARTDMA_H_
#define BIOS_UARTDMA_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************************
 * 		Include Libraries 			   *
 **************************************/

#include "system.h"

/***************************************
 * 		STRUCTURE DEFINES			   *
 **************************************/

DMA_InitTypeDef USART1_Tx_DMA_InitStructure;
DMA_InitTypeDef USART1_Rx_DMA_InitStructure;

DMA_InitTypeDef USART2_Tx_DMA_InitStructure;
DMA_InitTypeDef USART2_Rx_DMA_InitStructure;

DMA_InitTypeDef USART3_Tx_DMA_InitStructure;
DMA_InitTypeDef USART3_Rx_DMA_InitStructure;

DMA_InitTypeDef UART4_Tx_DMA_InitStructure;
DMA_InitTypeDef UART4_Rx_DMA_InitStructure;

DMA_InitTypeDef UART5_Tx_DMA_InitStructure;
DMA_InitTypeDef UART5_Rx_DMA_InitStructure;

DMA_InitTypeDef USART6_Tx_DMA_InitStructure;
DMA_InitTypeDef USART6_Rx_DMA_InitStructure;

/**************************************************
 * 		Function Prototype			  			  *
 *************************************************/

void USART1TxDMAInit(uint32_t DMA_Priority, FunctionalState state, uint16_t preemptionpriority, uint16_t subpriority);
void USART2TxDMAInit(uint32_t DMA_Priority, FunctionalState state, uint16_t preemptionpriority, uint16_t subpriority);
void USART3TxDMAInit(uint32_t DMA_Priority, FunctionalState state, uint16_t preemptionpriority, uint16_t subpriority);
void UART4TxDMAInit(uint32_t DMA_Priority, FunctionalState state, uint16_t preemptionpriority, uint16_t subpriority);
void UART5TxDMAInit(uint32_t DMA_Priority, FunctionalState state, uint16_t preemptionpriority, uint16_t subpriority);
void USART6TxDMAInit(uint32_t DMA_Priority, FunctionalState state, uint16_t preemptionpriority, uint16_t subpriority);

uint8_t USART1_DMA_Send(uint8_t* DataArray, uint32_t size);
uint8_t USART2_DMA_Send(uint8_t* DataArray, uint32_t size);
uint8_t USART3_DMA_Send(uint8_t* DataArray, uint32_t size);
uint8_t UART4_DMA_Send(uint8_t* DataArray, uint32_t size);
uint8_t UART5_DMA_Send(uint8_t* DataArray, uint32_t size);
uint8_t USART6_DMA_Send(uint8_t* DataArray, uint32_t size);

void USART1RxDMAInit(uint32_t DMA_Priority, FunctionalState state, uint16_t preemptionpriority, uint16_t subpriority, uint8_t* DataArray, uint32_t size);
void USART2RxDMAInit(uint32_t DMA_Priority, FunctionalState state, uint16_t preemptionpriority, uint16_t subpriority, uint8_t* DataArray, uint32_t size);
void USART3RxDMAInit(uint32_t DMA_Priority, FunctionalState state, uint16_t preemptionpriority, uint16_t subpriority, uint8_t* DataArray, uint32_t size);
void UART4RxDMAInit(uint32_t DMA_Priority, FunctionalState state, uint16_t preemptionpriority, uint16_t subpriority, uint8_t* DataArray, uint32_t size);
void UART5RxDMAInit(uint32_t DMA_Priority, FunctionalState state, uint16_t preemptionpriority, uint16_t subpriority, uint8_t* DataArray, uint32_t size);
void USART6RxDMAInit(uint32_t DMA_Priority, FunctionalState state, uint16_t preemptionpriority, uint16_t subpriority, uint8_t* DataArray, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif /* BIOS_UARTDMA_H_ */
