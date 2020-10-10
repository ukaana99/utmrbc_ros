

#include "uart.h"

/*
 * Function Name		: UART1Init
 * Function Description : This function is called to initialize USART1 only.
 * Function Remarks		: USART1  is using PB6 as TX and PB7 as RX.
 * Function Arguments	: baudrate				,normally set to 115200 according to UTM ROBOCON UART COMMUNICATION PROTOCOL.
 * 						  rxstate				,can be ENABLE (enable USART1 receive interrupt) or DISBALE
 * 						  preemptionpriority    ,interrupt with higher preemption priority executed first
 * 						  subpriority			,when 2 interrupts have similar preemption priorities, interrupt with higher
 * 						  						 priority will be executed first. If 2 interrupts have similar preemption
 * 						  						 subpriority, then the one comes first in the program will be executed first.
 *						  GPIOx_Tx				GPIOx group of UART Tx pin(x = A,B,C,D or E)
 * 						  GPIO_Pin_Tx 			GPIO_Pin_x of UART Tx pin(x = 0,1,2,...or 15)
 * 						  GPIOx_Rx				GPIOx group of UART Rx pin(x = A,B,C,D or E)
 * 						  GPIO_Pin_Rx 			GPIO_Pin_x of UART Rx pin(x = 0,1,2,...or 15)
 * Function Return		: None
 * Function Example		: UART1Init(115200, ENABLE, 0, 0, GPIOD, GPIO_Pin_0, GPIOD, GPIO_Pin_1);
 */

void UART1Init(uint32_t baudrate, FunctionalState rxstate, uint16_t preemptionpriority,uint16_t subpriority,
		  GPIO_TypeDef* GPIOx_Tx, uint16_t GPIO_Pin_Tx, GPIO_TypeDef* GPIOx_Rx, uint16_t GPIO_Pin_Rx)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	GPIOPinsInit (GPIOx_Tx, GPIO_Pin_Tx, GPIO_Mode_AF, GPIO_OType_PP,GPIO_Speed_50MHz,GPIO_PuPd_NOPULL);
	GPIOPinsInit (GPIOx_Rx, GPIO_Pin_Rx, GPIO_Mode_AF, GPIO_OType_PP,GPIO_Speed_50MHz,GPIO_PuPd_NOPULL);
	AFConfig(USART1,GPIOx_Tx, GPIO_Pin_Tx);
	AFConfig(USART1,GPIOx_Rx, GPIO_Pin_Rx);

	BIOS_USART_InitStructure.USART_BaudRate            = baudrate;
	BIOS_USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	BIOS_USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	BIOS_USART_InitStructure.USART_Parity              = USART_Parity_No;
	BIOS_USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	BIOS_USART_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART1, &BIOS_USART_InitStructure);

	USART_ITConfig(USART1, USART_IT_RXNE, rxstate);

	BIOS_UART_NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	BIOS_UART_NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemptionpriority;
	BIOS_UART_NVIC_InitStructure.NVIC_IRQChannelSubPriority = subpriority;
	BIOS_UART_NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&BIOS_UART_NVIC_InitStructure);

	USART_Cmd(USART1, ENABLE);
}

/*
 * Function Name		: UART2Init
 * Function Description : This function is called to initialize USART2 only.
 * Function Remarks		: USART2  is using PD5 as TX and PD6 as RX.
 * Function Arguments	: baudrate				,normally set to 115200 according to UTM ROBOCON UART COMMUNICATION PROTOCOL.
 * 						  rxstate				,can be ENABLE (enable USART2 receive interrupt) or DISBALE
 * 						  preemptionpriority    ,interrupt with higher preemption priority executed first
 * 						  subpriority			,when 2 interrupts have similar preemption priorities, interrupt with higher
 * 						  						 priority will be executed first. If 2 interrupts have similar preemption
 * 						  						 subpriority, then the one comes first in the program will be executed first.
 * 						  GPIOx_Tx				GPIOx group of UART Tx pin(x = A,B,C,D or E)
 * 						  GPIO_Pin_Tx 			GPIO_Pin_x of UART Tx pin(x = 0,1,2,...or 15)
 * 						  GPIOx_Rx				GPIOx group of UART Rx pin(x = A,B,C,D or E)
 * 						  GPIO_Pin_Rx 			GPIO_Pin_x of UART Rx pin(x = 0,1,2,...or 15)
 * Function Return		: None
 * Function Example		: UART2Init(115200, ENABLE, 0, 0, GPIOD, GPIO_Pin_0, GPIOD, GPIO_Pin_1);
 */

void UART2Init(uint32_t baudrate, FunctionalState rxstate, uint16_t preemptionpriority,uint16_t subpriority,
		  GPIO_TypeDef* GPIOx_Tx, uint16_t GPIO_Pin_Tx, GPIO_TypeDef* GPIOx_Rx, uint16_t GPIO_Pin_Rx)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	GPIOPinsInit (GPIOx_Tx, GPIO_Pin_Tx, GPIO_Mode_AF, GPIO_OType_PP,GPIO_Speed_50MHz,GPIO_PuPd_NOPULL);
	GPIOPinsInit (GPIOx_Rx, GPIO_Pin_Rx, GPIO_Mode_AF, GPIO_OType_PP,GPIO_Speed_50MHz,GPIO_PuPd_NOPULL);
	AFConfig(USART2,GPIOx_Tx, GPIO_Pin_Tx);
	AFConfig(USART2,GPIOx_Rx, GPIO_Pin_Rx);

	BIOS_USART_InitStructure.USART_BaudRate            = baudrate;
	BIOS_USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	BIOS_USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	BIOS_USART_InitStructure.USART_Parity              = USART_Parity_No;
	BIOS_USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	BIOS_USART_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART2, &BIOS_USART_InitStructure);

	USART_ITConfig(USART2, USART_IT_RXNE, rxstate);

	BIOS_UART_NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	BIOS_UART_NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemptionpriority;
	BIOS_UART_NVIC_InitStructure.NVIC_IRQChannelSubPriority = subpriority;
	BIOS_UART_NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&BIOS_UART_NVIC_InitStructure);

	USART_Cmd(USART2, ENABLE);
}

/*
 * Function Name		: UART3Init
 * Function Description : This function is called to initialize USART3 only.
 * Function Remarks		: USART3  is using PC10 as TX and PC11 as RX.
 * Function Arguments	: baudrate				,normally set to 115200 according to UTM ROBOCON UART COMMUNICATION PROTOCOL.
 * 						  rxstate				,can be ENABLE (enable USART3 receive interrupt) or DISBALE
 * 						  preemptionpriority    ,interrupt with higher preemption priority executed first
 * 						  subpriority			,when 2 interrupts have similar preemption priorities, interrupt with higher
 * 						  						 priority will be executed first. If 2 interrupts have similar preemption
 * 						  						 subpriority, then the one comes first in the program will be executed first.
 * 						  GPIOx_Tx				GPIOx group of UART Tx pin(x = A,B,C,D or E)
 * 						  GPIO_Pin_Tx 			GPIO_Pin_x of UART Tx pin(x = 0,1,2,...or 15)
 * 						  GPIOx_Rx				GPIOx group of UART Rx pin(x = A,B,C,D or E)
 * 						  GPIO_Pin_Rx 			GPIO_Pin_x of UART Rx pin(x = 0,1,2,...or 15)
 * Function Return		: None
 * Function Example		: UART3Init(115200, ENABLE, 0, 0, GPIOD, GPIO_Pin_0, GPIOD, GPIO_Pin_1);
 */

void UART3Init(uint32_t baudrate, FunctionalState rxstate, uint16_t preemptionpriority,uint16_t subpriority,
		  GPIO_TypeDef* GPIOx_Tx, uint16_t GPIO_Pin_Tx, GPIO_TypeDef* GPIOx_Rx, uint16_t GPIO_Pin_Rx)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	GPIOPinsInit (GPIOx_Tx, GPIO_Pin_Tx, GPIO_Mode_AF, GPIO_OType_PP,GPIO_Speed_50MHz,GPIO_PuPd_NOPULL);
	GPIOPinsInit (GPIOx_Rx, GPIO_Pin_Rx, GPIO_Mode_AF, GPIO_OType_PP,GPIO_Speed_50MHz,GPIO_PuPd_NOPULL);
	AFConfig(USART3,GPIOx_Tx, GPIO_Pin_Tx);
	AFConfig(USART3,GPIOx_Rx, GPIO_Pin_Rx);

	BIOS_USART_InitStructure.USART_BaudRate            = baudrate;
	BIOS_USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	BIOS_USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	BIOS_USART_InitStructure.USART_Parity              = USART_Parity_No;
	BIOS_USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	BIOS_USART_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART3, &BIOS_USART_InitStructure);

	USART_ITConfig(USART3, USART_IT_RXNE, rxstate);

	BIOS_UART_NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	BIOS_UART_NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemptionpriority;
	BIOS_UART_NVIC_InitStructure.NVIC_IRQChannelSubPriority = subpriority;
	BIOS_UART_NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&BIOS_UART_NVIC_InitStructure);

	USART_Cmd(USART3, ENABLE);
}

/*
 * Function Name		: UART4Init
 * Function Description : This function is called to initialize UART4 only.
 * Function Remarks		: UART4  is using PC10 as TX and PC11 as RX.
 * Function Arguments	: baudrate				,normally set to 115200 according to UTM ROBOCON UART COMMUNICATION PROTOCOL.
 * 						  rxstate				,can be ENABLE (enable UART4 receive interrupt) or DISBALE
 * 						  preemptionpriority    ,interrupt with higher preemption priority executed first
 * 						  subpriority			,when 2 interrupts have similar preemption priorities, interrupt with higher
 * 						  						 priority will be executed first. If 2 interrupts have similar preemption
 * 						  						 subpriority, then the one comes first in the program will be executed first.
 * 						  GPIOx_Tx				GPIOx group of UART Tx pin(x = A,B,C,D or E)
 * 						  GPIO_Pin_Tx 			GPIO_Pin_x of UART Tx pin(x = 0,1,2,...or 15)
 * 						  GPIOx_Rx				GPIOx group of UART Rx pin(x = A,B,C,D or E)
 * 						  GPIO_Pin_Rx 			GPIO_Pin_x of UART Rx pin(x = 0,1,2,...or 15)
 * Function Return		: None
 * Function Example		: UART4Init(115200, ENABLE, 0, 0, GPIOD, GPIO_Pin_0, GPIOD, GPIO_Pin_1);
 */

void UART4Init(uint32_t baudrate, FunctionalState rxstate, uint16_t preemptionpriority,uint16_t subpriority,
		  GPIO_TypeDef* GPIOx_Tx, uint16_t GPIO_Pin_Tx, GPIO_TypeDef* GPIOx_Rx, uint16_t GPIO_Pin_Rx)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

	GPIOPinsInit (GPIOx_Tx, GPIO_Pin_Tx, GPIO_Mode_AF, GPIO_OType_PP,GPIO_Speed_50MHz,GPIO_PuPd_NOPULL);
	GPIOPinsInit (GPIOx_Rx, GPIO_Pin_Rx, GPIO_Mode_AF, GPIO_OType_PP,GPIO_Speed_50MHz,GPIO_PuPd_NOPULL);
	AFConfig(UART4,GPIOx_Tx, GPIO_Pin_Tx);
	AFConfig(UART4,GPIOx_Rx, GPIO_Pin_Rx);

	BIOS_USART_InitStructure.USART_BaudRate            = baudrate;
	BIOS_USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	BIOS_USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	BIOS_USART_InitStructure.USART_Parity              = USART_Parity_No;
	BIOS_USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	BIOS_USART_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(UART4, &BIOS_USART_InitStructure);

	USART_ITConfig(UART4, USART_IT_RXNE, rxstate);

	BIOS_UART_NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	BIOS_UART_NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemptionpriority;
	BIOS_UART_NVIC_InitStructure.NVIC_IRQChannelSubPriority = subpriority;
	BIOS_UART_NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&BIOS_UART_NVIC_InitStructure);

	USART_Cmd(UART4, ENABLE);
}

/*
 * Function Name		: UART5Init
 * Function Description : This function is called to initialize UART5 only.
 * Function Remarks		: UART5  is using PC12 as TX and PD2 as RX.
 * Function Arguments	: baudrate				,normally set to 115200 according to UTM ROBOCON UART COMMUNICATION PROTOCOL.
 * 						  rxstate				,can be ENABLE (enable UART5 receive interrupt) or DISBALE
 * 						  preemptionpriority    ,interrupt with higher preemption priority executed first
 * 						  subpriority			,when 2 interrupts have similar preemption priorities, interrupt with higher
 * 						  						 priority will be executed first. If 2 interrupts have similar preemption
 * 						  						 subpriority, then the one comes first in the program will be executed first.
 * 						  GPIOx_Tx				GPIOx group of UART Tx pin(x = A,B,C,D or E)
 * 						  GPIO_Pin_Tx 			GPIO_Pin_x of UART Tx pin(x = 0,1,2,...or 15)
 * 						  GPIOx_Rx				GPIOx group of UART Rx pin(x = A,B,C,D or E)
 * 						  GPIO_Pin_Rx 			GPIO_Pin_x of UART Rx pin(x = 0,1,2,...or 15)
 * Function Return		: None
 * Function Example		: UART5Init(115200, ENABLE, 0, 0, GPIOD, GPIO_Pin_0, GPIOD, GPIO_Pin_1);
 */

void UART5Init(uint32_t baudrate, FunctionalState rxstate, uint16_t preemptionpriority,uint16_t subpriority,
		  GPIO_TypeDef* GPIOx_Tx, uint16_t GPIO_Pin_Tx, GPIO_TypeDef* GPIOx_Rx, uint16_t GPIO_Pin_Rx)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);

	GPIOPinsInit (GPIOx_Tx, GPIO_Pin_Tx, GPIO_Mode_AF, GPIO_OType_PP,GPIO_Speed_50MHz,GPIO_PuPd_NOPULL);
	GPIOPinsInit (GPIOx_Rx, GPIO_Pin_Rx, GPIO_Mode_AF, GPIO_OType_PP,GPIO_Speed_50MHz,GPIO_PuPd_NOPULL);
	AFConfig(UART5,GPIOx_Tx, GPIO_Pin_Tx);
	AFConfig(UART5,GPIOx_Rx, GPIO_Pin_Rx);

	BIOS_USART_InitStructure.USART_BaudRate            = baudrate;
	BIOS_USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	BIOS_USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	BIOS_USART_InitStructure.USART_Parity              = USART_Parity_No;
	BIOS_USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	BIOS_USART_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(UART5, & BIOS_USART_InitStructure);

	USART_ITConfig(UART5, USART_IT_RXNE, rxstate);

	BIOS_UART_NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	BIOS_UART_NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemptionpriority;
	BIOS_UART_NVIC_InitStructure.NVIC_IRQChannelSubPriority = subpriority;
	BIOS_UART_NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&BIOS_UART_NVIC_InitStructure);

	USART_Cmd(UART5, ENABLE);
}

/*
 * Function Name		: UART6Init
 * Function Description : This function is called to initialize USART6 only.
 * Function Remarks		: USART6  is using PC6 as TX and PC7 as RX.
 * Function Arguments	: baudrate				,normally set to 115200 according to UTM ROBOCON UART COMMUNICATION PROTOCOL.
 * 						  rxstate				,can be ENABLE (enable USART6 receive interrupt) or DISBALE
 * 						  preemptionpriority    ,interrupt with higher preemption priority executed first
 * 						  subpriority			,when 2 interrupts have similar preemption priorities, interrupt with higher
 * 						  						 priority will be executed first. If 2 interrupts have similar preemption
 * 						  						 subpriority, then the one comes first in the program will be executed first.
 * 						  GPIOx_Tx				GPIOx group of UART Tx pin(x = A,B,C,D or E)
 * 						  GPIO_Pin_Tx 			GPIO_Pin_x of UART Tx pin(x = 0,1,2,...or 15)
 * 						  GPIOx_Rx				GPIOx group of UART Rx pin(x = A,B,C,D or E)
 * 						  GPIO_Pin_Rx 			GPIO_Pin_x of UART Rx pin(x = 0,1,2,...or 15)
 * Function Return		: None
 * Function Example		: UART6Init(115200, ENABLE, 0, 0, GPIOD, GPIO_Pin_0, GPIOD, GPIO_Pin_1);
 */

void UART6Init(uint32_t baudrate, FunctionalState rxstate, uint16_t preemptionpriority,uint16_t subpriority,
		  GPIO_TypeDef* GPIOx_Tx, uint16_t GPIO_Pin_Tx, GPIO_TypeDef* GPIOx_Rx, uint16_t GPIO_Pin_Rx)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);

	GPIOPinsInit (GPIOx_Tx, GPIO_Pin_Tx, GPIO_Mode_AF, GPIO_OType_PP,GPIO_Speed_50MHz,GPIO_PuPd_NOPULL);
	GPIOPinsInit (GPIOx_Rx, GPIO_Pin_Rx, GPIO_Mode_AF, GPIO_OType_PP,GPIO_Speed_50MHz,GPIO_PuPd_NOPULL);
	AFConfig(USART6,GPIOx_Tx, GPIO_Pin_Tx);
	AFConfig(USART6,GPIOx_Rx, GPIO_Pin_Rx);

	BIOS_USART_InitStructure.USART_BaudRate            = baudrate;
	BIOS_USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	BIOS_USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	BIOS_USART_InitStructure.USART_Parity              = USART_Parity_No;
	BIOS_USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	BIOS_USART_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART6, &BIOS_USART_InitStructure);

	USART_ITConfig(USART6, USART_IT_RXNE, rxstate);

	BIOS_UART_NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
	BIOS_UART_NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemptionpriority;
	BIOS_UART_NVIC_InitStructure.NVIC_IRQChannelSubPriority = subpriority;
	BIOS_UART_NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&BIOS_UART_NVIC_InitStructure);

	USART_Cmd(USART6, ENABLE);
}

/*
 * Function Name		: UARTPrintChar
 * Function Description : This function is called to print char to desired UARTx, x can be 1 to 6.
 * Function Remarks		: None
 * Function Arguments	: USARTx	,x can be 1 to 6.
 * 						  ch		,4 bits length value
 * Function Return		: None
 * Function Example		: UARTPrintChar(UART4, 'A');		//Print 'A' to UART4
 */

void UARTPrintChar(USART_TypeDef* USARTx, const char ch)
{
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	USART_SendData(USARTx, ch);
}

/*
 * Function Name		: UARTPrintString
 * Function Description : This function is called to print string to desired UARTx, x can be 1 to 6.
 * Function Remarks		: None
 * Function Arguments	: USARTx	,x can be 1 to 6.
 * 						  s			,buffer or string
 * Function Return		: None
 * Function Example		: UARTPrintString(UART4, buffer);
 */

void UARTPrintString(USART_TypeDef* USARTx, volatile char *s)
{
	while(*s)
	{
		while( !(USARTx->SR & 0x00000040) );
		USART_SendData(USARTx, *s);
		s++;
	}
}

/*
 * Function Name		: ReadUART
 * Function Description : This function will return received value of desired USARTx, x can be 1 to 6.
 * Function Remarks		: None
 * Function Arguments	: USARTx	,x can be 1 to 6.
 * Function Return		: USARTx's received data
 * Function Example		: rx = ReadUART(UART4);
 */

char ReadUART(USART_TypeDef* USARTx)
{
	while(USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == 0x00);
		return((char)USART_ReceiveData(USARTx));
}

