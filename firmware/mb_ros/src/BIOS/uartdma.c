
#include "uartdma.h"

/************************************************/
/*		 	 	Functions		       		  	*/
/************************************************/

/*
 * Function Name		: USART1TxDMAInit
 * Function Description : This function is called to initialize USART1 Transmit DMA.
 * Function Remarks		: User need to initialize USART1 first before calling this function.
 * 						  DMA2_Stream7 channel 4 was used for USART1 Transmit DMA.
 * Function Arguments	: DMA_Priority			Specifies the software priority for the DMA2_Stream7.
 * 						  state					ENABLE or DISABLE the DMA2_Stream7 Transfer Complete interrupt
 * 						  preemptionpriority    interrupt with higher preemption priority executed first
 * 						  subpriority			when 2 interrupts have similar preemption priorities, interrupt with higher
 * 						  						priority will be executed first. If 2 interrupts have similar preemption
 * 						  						subpriority, then the one comes first in the program will be executed first.
 * Function Return		: None
 * Function Example		: USART1TxDMAInit(DMA_Priority_Medium, ENABLE, 2, 1);
 */
void USART1TxDMAInit(uint32_t DMA_Priority, FunctionalState state, uint16_t preemptionpriority, uint16_t subpriority)
{
	NVIC_InitTypeDef 	NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

	DMA_DeInit(DMA2_Stream7);
	while (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE);

	USART1_Tx_DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	USART1_Tx_DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &USART1->DR;
	USART1_Tx_DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	USART1_Tx_DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	USART1_Tx_DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	USART1_Tx_DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	USART1_Tx_DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	USART1_Tx_DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	USART1_Tx_DMA_InitStructure.DMA_Priority = DMA_Priority;
	USART1_Tx_DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	USART1_Tx_DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	USART1_Tx_DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	USART1_Tx_DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream7, &USART1_Tx_DMA_InitStructure);
	DMA_Cmd(DMA2_Stream7, ENABLE);
    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);

	DMA_ITConfig(DMA2_Stream7, DMA_IT_TC, state);

	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemptionpriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = subpriority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = state;
	NVIC_Init(&NVIC_InitStructure);

}

/*
 * Function Name		: USART2TxDMAInit
 * Function Description : This function is called to initialize USART2 Transmit DMA.
 * Function Remarks		: User need to initialize USART2 first before calling this function.
 * 						  DMA1_Stream6 channel 4 was used for USART2 Transmit DMA.
 * Function Arguments	: DMA_Priority			Specifies the software priority for the DMA1_Stream6.
 * 						  state					ENABLE or DISABLE the DMA1_Stream6 Transfer Complete interrupt
 * 						  preemptionpriority    interrupt with higher preemption priority executed first
 * 						  subpriority			when 2 interrupts have similar preemption priorities, interrupt with higher
 * 						  						priority will be executed first. If 2 interrupts have similar preemption
 * 						  						subpriority, then the one comes first in the program will be executed first.
 * Function Return		: None
 * Function Example		: USART2TxDMAInit(DMA_Priority_Medium, ENABLE, 2, 1);
 */

void USART2TxDMAInit(uint32_t DMA_Priority, FunctionalState state, uint16_t preemptionpriority, uint16_t subpriority)
{
	NVIC_InitTypeDef 	NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

	DMA_DeInit(DMA1_Stream6);
	while (DMA_GetCmdStatus(DMA1_Stream6) != DISABLE);

	USART2_Tx_DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	USART2_Tx_DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &USART2->DR;
	USART2_Tx_DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	USART2_Tx_DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	USART2_Tx_DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	USART2_Tx_DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	USART2_Tx_DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	USART2_Tx_DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	USART2_Tx_DMA_InitStructure.DMA_Priority = DMA_Priority;
	USART2_Tx_DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	USART2_Tx_DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	USART2_Tx_DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	USART2_Tx_DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA1_Stream6, &USART2_Tx_DMA_InitStructure);
	DMA_Cmd(DMA1_Stream6, ENABLE);
    USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);

	DMA_ITConfig(DMA1_Stream6, DMA_IT_TC, state);

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemptionpriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = subpriority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = state;
	NVIC_Init(&NVIC_InitStructure);

}

/*
 * Function Name		: USART3TxDMAInit
 * Function Description : This function is called to initialize USART3 Transmit DMA.
 * Function Remarks		: User need to initialize USART3 first before calling this function.
 * 						  DMA1_Stream3 channel 4 was used for USART3 Transmit DMA.
 * Function Arguments	: DMA_Priority			Specifies the software priority for the DMA1_Stream3.
 * 						  state					ENABLE or DISABLE the DMA1_Stream3 Transfer Complete interrupt
 * 						  preemptionpriority    interrupt with higher preemption priority executed first
 * 						  subpriority			when 2 interrupts have similar preemption priorities, interrupt with higher
 * 						  						priority will be executed first. If 2 interrupts have similar preemption
 * 						  						subpriority, then the one comes first in the program will be executed first.
 * Function Return		: None
 * Function Example		: USART3TxDMAInit(DMA_Priority_Medium, ENABLE, 2, 1);
 */
void USART3TxDMAInit(uint32_t DMA_Priority, FunctionalState state, uint16_t preemptionpriority, uint16_t subpriority)
{
	NVIC_InitTypeDef 	NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

	DMA_DeInit(DMA1_Stream3);
	while (DMA_GetCmdStatus(DMA1_Stream3) != DISABLE);

	USART3_Tx_DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	USART3_Tx_DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &USART3->DR;
	USART3_Tx_DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	USART3_Tx_DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	USART3_Tx_DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	USART3_Tx_DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	USART3_Tx_DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	USART3_Tx_DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	USART3_Tx_DMA_InitStructure.DMA_Priority = DMA_Priority;
	USART3_Tx_DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	USART3_Tx_DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	USART3_Tx_DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	USART3_Tx_DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA1_Stream3, &USART3_Tx_DMA_InitStructure);
	DMA_Cmd(DMA1_Stream3, ENABLE);
    USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);

	DMA_ITConfig(DMA1_Stream3, DMA_IT_TC, state);

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemptionpriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = subpriority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = state;
	NVIC_Init(&NVIC_InitStructure);

}

/*
 * Function Name		: UART4TxDMAInit
 * Function Description : This function is called to initialize UART4 Transmit DMA.
 * Function Remarks		: User need to initialize UART4 first before calling this function.
 * 						  DMA1_Stream4 channel 4 was used for UART4 Transmit DMA.
 * Function Arguments	: DMA_Priority			Specifies the software priority for the DMA1_Stream4.
 * 						  state					ENABLE or DISABLE the DMA1_Stream4 Transfer Complete interrupt
 * 						  preemptionpriority    interrupt with higher preemption priority executed first
 * 						  subpriority			when 2 interrupts have similar preemption priorities, interrupt with higher
 * 						  						priority will be executed first. If 2 interrupts have similar preemption
 * 						  						subpriority, then the one comes first in the program will be executed first.
 * Function Return		: None
 * Function Example		: UART4TxDMAInit(DMA_Priority_Medium, ENABLE, 2, 1);
 */
void UART4TxDMAInit(uint32_t DMA_Priority, FunctionalState state, uint16_t preemptionpriority, uint16_t subpriority)
{
	NVIC_InitTypeDef 	NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

	DMA_DeInit(DMA1_Stream4);
	while (DMA_GetCmdStatus(DMA1_Stream4) != DISABLE);

	UART4_Tx_DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	UART4_Tx_DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &UART4->DR;
	UART4_Tx_DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	UART4_Tx_DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	UART4_Tx_DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	UART4_Tx_DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	UART4_Tx_DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	UART4_Tx_DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	UART4_Tx_DMA_InitStructure.DMA_Priority = DMA_Priority;
	UART4_Tx_DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	UART4_Tx_DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	UART4_Tx_DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	UART4_Tx_DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA1_Stream4, &UART4_Tx_DMA_InitStructure);
	DMA_Cmd(DMA1_Stream4, ENABLE);
    USART_DMACmd(UART4, USART_DMAReq_Tx, ENABLE);

	DMA_ITConfig(DMA1_Stream4, DMA_IT_TC, state);

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemptionpriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = subpriority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = state;
	NVIC_Init(&NVIC_InitStructure);

}

/*
 * Function Name		: UART5TxDMAInit
 * Function Description : This function is called to initialize UART5 Transmit DMA.
 * Function Remarks		: User need to initialize UART5 first before calling this function.
 * 						  DMA1_Stream7 channel 4 was used for UART5 Transmit DMA.
 * Function Arguments	: DMA_Priority			Specifies the software priority for the DMA1_Stream7.
 * 						  state					ENABLE or DISABLE the DMA1_Stream7 Transfer Complete interrupt
 * 						  preemptionpriority    interrupt with higher preemption priority executed first
 * 						  subpriority			when 2 interrupts have similar preemption priorities, interrupt with higher
 * 						  						priority will be executed first. If 2 interrupts have similar preemption
 * 						  						subpriority, then the one comes first in the program will be executed first.
 * Function Return		: None
 * Function Example		: UART5TxDMAInit(DMA_Priority_Medium, ENABLE, 2, 1);
 */
void UART5TxDMAInit(uint32_t DMA_Priority, FunctionalState state, uint16_t preemptionpriority, uint16_t subpriority)
{
	NVIC_InitTypeDef 	NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

	DMA_DeInit(DMA1_Stream7);
	while (DMA_GetCmdStatus(DMA1_Stream7) != DISABLE);

	UART5_Tx_DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	UART5_Tx_DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &UART5->DR;
	UART5_Tx_DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	UART5_Tx_DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	UART5_Tx_DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	UART5_Tx_DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	UART5_Tx_DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	UART5_Tx_DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	UART5_Tx_DMA_InitStructure.DMA_Priority = DMA_Priority;
	UART5_Tx_DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	UART5_Tx_DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	UART5_Tx_DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	UART5_Tx_DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA1_Stream7, &UART5_Tx_DMA_InitStructure);
	DMA_Cmd(DMA1_Stream7, ENABLE);
    USART_DMACmd(UART5, USART_DMAReq_Tx, ENABLE);

	DMA_ITConfig(DMA1_Stream7, DMA_IT_TC, state);

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemptionpriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = subpriority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = state;
	NVIC_Init(&NVIC_InitStructure);

}

/*
 * Function Name		: USART6TxDMAInit
 * Function Description : This function is called to initialize USART6 Transmit DMA.
 * Function Remarks		: User need to initialize USART6 first before calling this function.
 * 						  DMA2_Stream6 channel 5 was used for USART6 Transmit DMA.
 * Function Arguments	: DMA_Priority			Specifies the software priority for the DMA2_Stream6.
 * 						  state					ENABLE or DISABLE the DMA2_Stream6 Transfer Complete interrupt
 * 						  preemptionpriority    interrupt with higher preemption priority executed first
 * 						  subpriority			when 2 interrupts have similar preemption priorities, interrupt with higher
 * 						  						priority will be executed first. If 2 interrupts have similar preemption
 * 						  						subpriority, then the one comes first in the program will be executed first.
 * Function Return		: None
 * Function Example		: USART6TxDMAInit(DMA_Priority_Medium, ENABLE, 2, 1);
 */
void USART6TxDMAInit(uint32_t DMA_Priority, FunctionalState state, uint16_t preemptionpriority, uint16_t subpriority)
{
	NVIC_InitTypeDef 	NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

	DMA_DeInit(DMA2_Stream6);
	while (DMA_GetCmdStatus(DMA2_Stream6) != DISABLE);

	USART6_Tx_DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	USART6_Tx_DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &USART6->DR;
	USART6_Tx_DMA_InitStructure.DMA_Channel = DMA_Channel_5;
	USART6_Tx_DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	USART6_Tx_DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	USART6_Tx_DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	USART6_Tx_DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	USART6_Tx_DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	USART6_Tx_DMA_InitStructure.DMA_Priority = DMA_Priority;
	USART6_Tx_DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	USART6_Tx_DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	USART6_Tx_DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	USART6_Tx_DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream6, &USART6_Tx_DMA_InitStructure);
	DMA_Cmd(DMA2_Stream6, ENABLE);
    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);

	DMA_ITConfig(DMA2_Stream6, DMA_IT_TC, state);

	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemptionpriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = subpriority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = state;
	NVIC_Init(&NVIC_InitStructure);

}

/*
 * Function Name		: USART1_DMA_Send
 * Function Description : This function is called to Transmit using DMA.
 * Function Remarks		: None
 * Function Arguments	: DataArray			pointer to data to be transmit
 * 						  size				size of data to be transmit
 * Function Return		: None
 * Function Example		: char uarttransmitbuff[100];
 * 						  sprintf(uarttransmitbuff, "Data To Be Transmit");
 * 						  USART1_DMA_Send(uarttransmitbuff, strlen(uarttransmitbuff));
 */
uint8_t USART1_DMA_Send(uint8_t* DataArray, uint32_t size)
{

	if (DMA2_Stream7->NDTR)
		return 0;

	USART1_Tx_DMA_InitStructure.DMA_BufferSize = size;
	USART1_Tx_DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) DataArray;
	DMA_DeInit(DMA2_Stream7);
	DMA_Init(DMA2_Stream7, &USART1_Tx_DMA_InitStructure);

	DMA_Cmd(DMA2_Stream7, ENABLE);
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);

	return 1;

}

/*
 * Function Name		: USART2_DMA_Send
 * Function Description : This function is called to Transmit using DMA.
 * Function Remarks		: None
 * Function Arguments	: DataArray			pointer to data to be transmit
 * 						  size				size of data to be transmit
 * Function Return		: None
 * Function Example		: char uarttransmitbuff[100];
 * 						  sprintf(uarttransmitbuff, "Data To Be Transmit");
 * 						  USART2_DMA_Send(uarttransmitbuff, strlen(uarttransmitbuff));
 */
uint8_t USART2_DMA_Send(uint8_t* DataArray, uint32_t size)
{

	if (DMA1_Stream6->NDTR)
		return 0;

	USART2_Tx_DMA_InitStructure.DMA_BufferSize = size;
	USART2_Tx_DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) DataArray;
	DMA_DeInit(DMA1_Stream6);
	DMA_Init(DMA1_Stream6, &USART2_Tx_DMA_InitStructure);

	DMA_Cmd(DMA1_Stream6, ENABLE);
	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);

	return 1;

}

/*
 * Function Name		: USART3_DMA_Send
 * Function Description : This function is called to Transmit using DMA.
 * Function Remarks		: None
 * Function Arguments	: DataArray			pointer to data to be transmit
 * 						  size				size of data to be transmit
 * Function Return		: None
 * Function Example		: char uarttransmitbuff[100];
 * 						  sprintf(uarttransmitbuff, "Data To Be Transmit");
 * 						  USART3_DMA_Send(uarttransmitbuff, strlen(uarttransmitbuff));
 */
uint8_t USART3_DMA_Send(uint8_t* DataArray, uint32_t size)
{

	if (DMA1_Stream3->NDTR)
		return 0;

	USART3_Tx_DMA_InitStructure.DMA_BufferSize = size;
	USART3_Tx_DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) DataArray;
	DMA_DeInit(DMA1_Stream3);
	DMA_Init(DMA1_Stream3, &USART3_Tx_DMA_InitStructure);

	DMA_Cmd(DMA1_Stream3, ENABLE);
	USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);

	return 1;

}

/*
 * Function Name		: UART4_DMA_Send
 * Function Description : This function is called to Transmit using DMA.
 * Function Remarks		: None
 * Function Arguments	: DataArray			pointer to data to be transmit
 * 						  size				size of data to be transmit
 * Function Return		: None
 * Function Example		: char uarttransmitbuff[100];
 * 						  sprintf(uarttransmitbuff, "Data To Be Transmit");
 * 						  UART4_DMA_Send(uarttransmitbuff, strlen(uarttransmitbuff));
 */
uint8_t UART4_DMA_Send(uint8_t* DataArray, uint32_t size)
{

	if (DMA1_Stream4->NDTR)
		return 0;

	UART4_Tx_DMA_InitStructure.DMA_BufferSize = size;
	UART4_Tx_DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) DataArray;
	DMA_DeInit(DMA1_Stream4);
	DMA_Init(DMA1_Stream4, &UART4_Tx_DMA_InitStructure);

	DMA_Cmd(DMA1_Stream4, ENABLE);
	USART_DMACmd(UART4, USART_DMAReq_Tx, ENABLE);

	return 1;

}

/*
 * Function Name		: UART5_DMA_Send
 * Function Description : This function is called to Transmit using DMA.
 * Function Remarks		: None
 * Function Arguments	: DataArray			pointer to data to be transmit
 * 						  size				size of data to be transmit
 * Function Return		: None
 * Function Example		: char uarttransmitbuff[100];
 * 						  sprintf(uarttransmitbuff, "Data To Be Transmit");
 * 						  UART5_DMA_Send(uarttransmitbuff, strlen(uarttransmitbuff));
 */
uint8_t UART5_DMA_Send(uint8_t* DataArray, uint32_t size)
{

	if (DMA1_Stream7->NDTR)
		return 0;

	UART5_Tx_DMA_InitStructure.DMA_BufferSize = size;
	UART5_Tx_DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) DataArray;
	DMA_DeInit(DMA1_Stream7);
	DMA_Init(DMA1_Stream7, &UART5_Tx_DMA_InitStructure);

	DMA_Cmd(DMA1_Stream7, ENABLE);
	USART_DMACmd(UART5, USART_DMAReq_Tx, ENABLE);

	return 1;

}

/*
 * Function Name		: USART6_DMA_Send
 * Function Description : This function is called to Transmit using DMA.
 * Function Remarks		: None
 * Function Arguments	: DataArray			pointer to data to be transmit
 * 						  size				size of data to be transmit
 * Function Return		: None
 * Function Example		: char uarttransmitbuff[100];
 * 						  sprintf(uarttransmitbuff, "Data To Be Transmit");
 * 						  USART6_DMA_Send(uarttransmitbuff, strlen(uarttransmitbuff));
 */
uint8_t USART6_DMA_Send(uint8_t* DataArray, uint32_t size)
{

	if (DMA2_Stream6->NDTR)
		return 0;

	USART6_Tx_DMA_InitStructure.DMA_BufferSize = size;
	USART6_Tx_DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) DataArray;
	DMA_DeInit(DMA2_Stream6);
	DMA_Init(DMA2_Stream6, &USART6_Tx_DMA_InitStructure);

	DMA_Cmd(DMA2_Stream6, ENABLE);
	USART_DMACmd(USART6, USART_DMAReq_Tx, ENABLE);

	return 1;

}

/*
 * Function Name		: USART1RxDMAInit
 * Function Description : This function is called to initialize USART1 Receive DMA.
 * Function Remarks		: User need to initialize USART1 first before calling this function.
 * 						  DMA2_Stream5 channel 4 was used for USART1 Receive DMA.
 * Function Arguments	: DMA_Priority			Specifies the software priority for the DMA2_Stream5.
 * 						  state					ENABLE or DISABLE the DMA2_Stream5 Transfer Complete interrupt
 * 						  preemptionpriority    interrupt with higher preemption priority executed first
 * 						  subpriority			when 2 interrupts have similar preemption priorities, interrupt with higher
 * 						  						priority will be executed first. If 2 interrupts have similar preemption
 * 						  						subpriority, then the one comes first in the program will be executed first.
 * 						  DataArray				pointer to buffer to store the receive data
 * 						  size					size of buffer to store the receive data
 * Function Return		: None
 * Function Example		: uint8_t DMA_RX_Buffer[8];
 * 						  USART1RxDMAInit(DMA_Priority_Medium, DISABLE, 2, 1, DMA_RX_Buffer, 8);
 */
void USART1RxDMAInit(uint32_t DMA_Priority, FunctionalState state, uint16_t preemptionpriority, uint16_t subpriority, uint8_t* DataArray, uint32_t size)
{
	NVIC_InitTypeDef 	NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);

	DMA_StructInit(&USART1_Rx_DMA_InitStructure);
	USART1_Rx_DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	USART1_Rx_DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)DataArray;
	USART1_Rx_DMA_InitStructure.DMA_BufferSize = size;
	USART1_Rx_DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	USART1_Rx_DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;
	USART1_Rx_DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	USART1_Rx_DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	USART1_Rx_DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	USART1_Rx_DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	USART1_Rx_DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	USART1_Rx_DMA_InitStructure.DMA_Priority = DMA_Priority;
	DMA_Init(DMA2_Stream5, &USART1_Rx_DMA_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = state;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemptionpriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = subpriority;
	NVIC_Init(&NVIC_InitStructure);

	DMA_ITConfig(DMA2_Stream5, DMA_IT_TC, state);

	DMA_Cmd(DMA2_Stream5, ENABLE);
}

/*
 * Function Name		: USART2RxDMAInit
 * Function Description : This function is called to initialize USART2 Receive DMA.
 * Function Remarks		: User need to initialize USART2 first before calling this function.
 * 						  DMA1_Stream5 channel 4 was used for USART2 Receive DMA.
 * Function Arguments	: DMA_Priority			Specifies the software priority for the DMA1_Stream5.
 * 						  state					ENABLE or DISABLE the DMA1_Stream5 Transfer Complete interrupt
 * 						  preemptionpriority    interrupt with higher preemption priority executed first
 * 						  subpriority			when 2 interrupts have similar preemption priorities, interrupt with higher
 * 						  						priority will be executed first. If 2 interrupts have similar preemption
 * 						  						subpriority, then the one comes first in the program will be executed first.
 * 						  DataArray				pointer to buffer to store the receive data
 * 						  size					size of buffer to store the receive data
 * Function Return		: None
 * Function Example		: uint8_t DMA_RX_Buffer[8];
 * 						  USART2RxDMAInit(DMA_Priority_Medium, DISABLE, 2, 1, DMA_RX_Buffer, 8);
 */
void USART2RxDMAInit(uint32_t DMA_Priority, FunctionalState state, uint16_t preemptionpriority, uint16_t subpriority, uint8_t* DataArray, uint32_t size)
{
	NVIC_InitTypeDef 	NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

	USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);

	DMA_StructInit(&USART2_Rx_DMA_InitStructure);
	USART2_Rx_DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	USART2_Rx_DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)DataArray;
	USART2_Rx_DMA_InitStructure.DMA_BufferSize = size;
	USART2_Rx_DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	USART2_Rx_DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART2->DR;
	USART2_Rx_DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	USART2_Rx_DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	USART2_Rx_DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	USART2_Rx_DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	USART2_Rx_DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	USART2_Rx_DMA_InitStructure.DMA_Priority = DMA_Priority;
	DMA_Init(DMA1_Stream5, &USART2_Rx_DMA_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = state;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemptionpriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = subpriority;
	NVIC_Init(&NVIC_InitStructure);

	DMA_ITConfig(DMA1_Stream5, DMA_IT_TC, state);

	DMA_Cmd(DMA1_Stream5, ENABLE);
}

/*
 * Function Name		: USART3RxDMAInit
 * Function Description : This function is called to initialize USART3 Receive DMA.
 * Function Remarks		: User need to initialize USART3 first before calling this function.
 * 						  DMA1_Stream1 channel 4 was used for USART3 Receive DMA.
 * Function Arguments	: DMA_Priority			Specifies the software priority for the DMA1_Stream1.
 * 						  state					ENABLE or DISABLE the DMA1_Stream1 Transfer Complete interrupt
 * 						  preemptionpriority    interrupt with higher preemption priority executed first
 * 						  subpriority			when 2 interrupts have similar preemption priorities, interrupt with higher
 * 						  						priority will be executed first. If 2 interrupts have similar preemption
 * 						  						subpriority, then the one comes first in the program will be executed first.
 * 						  DataArray				pointer to buffer to store the receive data
 * 						  size					size of buffer to store the receive data
 * Function Return		: None
 * Function Example		: uint8_t DMA_RX_Buffer[8];
 * 						  USART3RxDMAInit(DMA_Priority_Medium, DISABLE, 2, 1, DMA_RX_Buffer, 8);
 */
void USART3RxDMAInit(uint32_t DMA_Priority, FunctionalState state, uint16_t preemptionpriority, uint16_t subpriority, uint8_t* DataArray, uint32_t size)
{
	NVIC_InitTypeDef 	NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

	USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);

	DMA_StructInit(&USART3_Rx_DMA_InitStructure);
	USART3_Rx_DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	USART3_Rx_DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)DataArray;
	USART3_Rx_DMA_InitStructure.DMA_BufferSize = size;
	USART3_Rx_DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	USART3_Rx_DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART3->DR;
	USART3_Rx_DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	USART3_Rx_DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	USART3_Rx_DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	USART3_Rx_DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	USART3_Rx_DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	USART3_Rx_DMA_InitStructure.DMA_Priority = DMA_Priority;
	DMA_Init(DMA1_Stream1, &USART3_Rx_DMA_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = state;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemptionpriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = subpriority;
	NVIC_Init(&NVIC_InitStructure);

	DMA_ITConfig(DMA1_Stream1, DMA_IT_TC, state);

	DMA_Cmd(DMA1_Stream1, ENABLE);
}

/*
 * Function Name		: UART4RxDMAInit
 * Function Description : This function is called to initialize UART4 Receive DMA.
 * Function Remarks		: User need to initialize UART4 first before calling this function.
 * 						  DMA1_Stream2 channel 4 was used for UART4 Receive DMA.
 * Function Arguments	: DMA_Priority			Specifies the software priority for the DMA1_Stream2.
 * 						  state					ENABLE or DISABLE the DMA1_Stream2 Transfer Complete interrupt
 * 						  preemptionpriority    interrupt with higher preemption priority executed first
 * 						  subpriority			when 2 interrupts have similar preemption priorities, interrupt with higher
 * 						  						priority will be executed first. If 2 interrupts have similar preemption
 * 						  						subpriority, then the one comes first in the program will be executed first.
 * 						  DataArray				pointer to buffer to store the receive data
 * 						  size					size of buffer to store the receive data
 * Function Return		: None
 * Function Example		: uint8_t DMA_RX_Buffer[8];
 * 						  UART4RxDMAInit(DMA_Priority_Medium, DISABLE, 2, 1, DMA_RX_Buffer, 8);
 */
void UART4RxDMAInit(uint32_t DMA_Priority, FunctionalState state, uint16_t preemptionpriority, uint16_t subpriority, uint8_t* DataArray, uint32_t size)
{
	NVIC_InitTypeDef 	NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

	USART_DMACmd(UART4, USART_DMAReq_Rx, ENABLE);

	DMA_StructInit(&UART4_Rx_DMA_InitStructure);
	UART4_Rx_DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	UART4_Rx_DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)DataArray;
	UART4_Rx_DMA_InitStructure.DMA_BufferSize = size;
	UART4_Rx_DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	UART4_Rx_DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&UART4->DR;
	UART4_Rx_DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	UART4_Rx_DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	UART4_Rx_DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	UART4_Rx_DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	UART4_Rx_DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	UART4_Rx_DMA_InitStructure.DMA_Priority = DMA_Priority;
	DMA_Init(DMA1_Stream2, &UART4_Rx_DMA_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = state;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemptionpriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = subpriority;
	NVIC_Init(&NVIC_InitStructure);

	DMA_ITConfig(DMA1_Stream2, DMA_IT_TC, state);

	DMA_Cmd(DMA1_Stream2, ENABLE);
}

/*
 * Function Name		: UART5RxDMAInit
 * Function Description : This function is called to initialize UART5 Receive DMA.
 * Function Remarks		: User need to initialize UART5 first before calling this function.
 * 						  DMA1_Stream0 channel 4 was used for UART4 Receive DMA.
 * Function Arguments	: DMA_Priority			Specifies the software priority for the DMA1_Stream0.
 * 						  state					ENABLE or DISABLE the DMA1_Stream0 Transfer Complete interrupt
 * 						  preemptionpriority    interrupt with higher preemption priority executed first
 * 						  subpriority			when 2 interrupts have similar preemption priorities, interrupt with higher
 * 						  						priority will be executed first. If 2 interrupts have similar preemption
 * 						  						subpriority, then the one comes first in the program will be executed first.
 * 						  DataArray				pointer to buffer to store the receive data
 * 						  size					size of buffer to store the receive data
 * Function Return		: None
 * Function Example		: uint8_t DMA_RX_Buffer[8];
 * 						  UART5RxDMAInit(DMA_Priority_Medium, DISABLE, 2, 1, DMA_RX_Buffer, 8);
 */
void UART5RxDMAInit(uint32_t DMA_Priority, FunctionalState state, uint16_t preemptionpriority, uint16_t subpriority, uint8_t* DataArray, uint32_t size)
{
	NVIC_InitTypeDef 	NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

	USART_DMACmd(UART5, USART_DMAReq_Rx, ENABLE);

	DMA_StructInit(&UART5_Rx_DMA_InitStructure);
	UART5_Rx_DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	UART5_Rx_DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)DataArray;
	UART5_Rx_DMA_InitStructure.DMA_BufferSize = size;
	UART5_Rx_DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	UART5_Rx_DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&UART5->DR;
	UART5_Rx_DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	UART5_Rx_DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	UART5_Rx_DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	UART5_Rx_DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	UART5_Rx_DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	UART5_Rx_DMA_InitStructure.DMA_Priority = DMA_Priority;
	DMA_Init(DMA1_Stream0, &UART5_Rx_DMA_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = state;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemptionpriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = subpriority;
	NVIC_Init(&NVIC_InitStructure);

	DMA_ITConfig(DMA1_Stream0, DMA_IT_TC, state);

	DMA_Cmd(DMA1_Stream0, ENABLE);
}

/*
 * Function Name		: USART6RxDMAInit
 * Function Description : This function is called to initialize USART6 Receive DMA.
 * Function Remarks		: User need to initialize USART6 first before calling this function.
 * 						  DMA2_Stream1 channel 5 was used for USART3 Receive DMA.
 * Function Arguments	: DMA_Priority			Specifies the software priority for the DMA2_Stream1.
 * 						  state					ENABLE or DISABLE the DMA2_Stream1 Transfer Complete interrupt
 * 						  preemptionpriority    interrupt with higher preemption priority executed first
 * 						  subpriority			when 2 interrupts have similar preemption priorities, interrupt with higher
 * 						  						priority will be executed first. If 2 interrupts have similar preemption
 * 						  						subpriority, then the one comes first in the program will be executed first.
 * 						  DataArray				pointer to buffer to store the receive data
 * 						  size					size of buffer to store the receive data
 * Function Return		: None
 * Function Example		: uint8_t DMA_RX_Buffer[8];
 * 						  USART6RxDMAInit(DMA_Priority_Medium, DISABLE, 2, 1, DMA_RX_Buffer, 8);
 */
void USART6RxDMAInit(uint32_t DMA_Priority, FunctionalState state, uint16_t preemptionpriority, uint16_t subpriority, uint8_t* DataArray, uint32_t size)
{
	NVIC_InitTypeDef 	NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

	USART_DMACmd(USART6, USART_DMAReq_Rx, ENABLE);

	DMA_StructInit(&USART6_Rx_DMA_InitStructure);
	USART6_Rx_DMA_InitStructure.DMA_Channel = DMA_Channel_5;
	USART6_Rx_DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)DataArray;
	USART6_Rx_DMA_InitStructure.DMA_BufferSize = size;
	USART6_Rx_DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	USART6_Rx_DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART6->DR;
	USART6_Rx_DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	USART6_Rx_DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	USART6_Rx_DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	USART6_Rx_DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	USART6_Rx_DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	USART6_Rx_DMA_InitStructure.DMA_Priority = DMA_Priority;
	DMA_Init(DMA2_Stream1, &USART6_Rx_DMA_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = state;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemptionpriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = subpriority;
	NVIC_Init(&NVIC_InitStructure);

	DMA_ITConfig(DMA2_Stream1, DMA_IT_TC, state);

	DMA_Cmd(DMA2_Stream1, ENABLE);
}
