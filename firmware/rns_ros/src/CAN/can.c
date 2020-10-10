
/************************************************/
/*		 	 	Include Header	       		  	*/
/************************************************/
#include "can.h"

/************************************************/
/*		 	 	Variables	      	 		  	*/
/************************************************/

/************************************************/
/*		 	 	Functions		       		  	*/
/************************************************/

/* Function Name		: CANx_Init
 * Function Description : This function is called to initialise bxCAN.
 * Function Remarks		:
 * Function Arguments	: CANx							CANx can be CAN1 or CAN2
 * 						  CANx_Tx_GPIOx					CANx_Tx_GPIOx can be GPIOx where x is A to I.
 * 						  CANx_Tx_pin					The desired pin that you want to configure
 * 						  								in group GPIOx. This parameter can be
 * 						  								combination of GPIO_Pin_x where x can be(0..15)
 * 						  CANx_Rx_GPIOx					CANx_Tx_GPIOx can be GPIOx where x is A to I.
 * 						  CANx_Rx_pin					The desired pin that you want to configure
 * 						  								in group GPIOx. This parameter can be
 * 						  								combination of GPIO_Pin_x where x can be(0..15)
 * 						  CAN_FilterFIFOAssignment		CAN_Filter_FIFO0 or CAN_Filter_FIFO1
 * 						  CAN_FilterId_11bits			Filter identification number(0...0x7FF)
 * 						  CAN_FilterMaskId_11bits		Filter mask number(0...0x7FF)
 * 						  CAN_FilterNumber				Filter number. There have 28 filter banks shared between
 * 						  								CAN1 and CAN2. CAN_FilterNumber for CAN1 must less
 * 						  								than CAN2 since initialise CAN2 must call function
 * 						  								CAN_SlaveStartBank which cause the onward filter numbers
 * 						  								cannot be used in CAN1. For example: if 24 is set as
 * 						  								CAN_FilterNumber in initialise CAN2, the filter number which
 * 						  								can only be chosen as CAN_FilterNumber in initialise CAN1
 * 						  								is 0 to 23.
 * 						  CAN_Prescaler					Specifies the length of a time quantum.It ranges from 1 to 1024.
 * 						  								CAN Baudrate = APB1_CLK/
 * 						  											  ((CAN_SJW_tq+CAN_BS1_tq+CAN_BS2_tq)*CAN_Prescaler);
 * 						  								Example: CAN Baudrate = 42000000/((1+12+8)*2);
 * 						  N_PPriority					Preemption priority for the IRQ channel. A lower priority value
 * 						  								indicates a higher priority.
 * 						  N_SPriority					Subpriority level for the IRQ channel. A lower priority value
 * 						  								indicates a higher priority.
 * Function Return		: CAN_InitStatus				Constant indicates initialization succeed which will be
  *        											    CAN_InitStatus_Failed or CAN_InitStatus_Success.
 * Function Example		: CANx_Init(CAN1,GPIOB, GPIO_Pin_9,GPIOB, GPIO_Pin_8,CAN_Filter_FIFO0,0,0,2,1,1);
 */
uint8_t CANx_Init(CAN_TypeDef* CANx, GPIO_TypeDef * CANx_Tx_GPIOx, uint16_t CANx_Tx_pin,
		GPIO_TypeDef * CANx_Rx_GPIOx, uint16_t CANx_Rx_pin,uint16_t CAN_FilterFIFOAssignment,
		uint16_t CAN_FilterId_11bits,uint16_t CAN_FilterMaskId_11bits,uint8_t CAN_FilterNumber,
		uint16_t CAN_Prescaler,uint8_t N_PPriority,uint8_t N_SPriority)
{
	uint8_t CAN_InitStatus;
	if(CANx == CAN1){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
		AFConfig(CAN1,CANx_Tx_GPIOx,CANx_Tx_pin);
		AFConfig(CAN1,CANx_Rx_GPIOx,CANx_Rx_pin);

	}else if(CANx == CAN2){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);
		AFConfig(CAN2,CANx_Tx_GPIOx,CANx_Tx_pin);
		AFConfig(CAN2,CANx_Rx_GPIOx,CANx_Rx_pin);
	}

	GPIOPinsInit (CANx_Tx_GPIOx, CANx_Tx_pin, GPIO_Mode_AF, GPIO_OType_PP, GPIO_Speed_50MHz,  GPIO_PuPd_UP);
	GPIOPinsInit (CANx_Rx_GPIOx, CANx_Rx_pin, GPIO_Mode_AF, GPIO_OType_PP, GPIO_Speed_50MHz,  GPIO_PuPd_UP);

	CAN_InitStruct.CAN_TTCM = DISABLE;
	CAN_InitStruct.CAN_ABOM = ENABLE;
	CAN_InitStruct.CAN_AWUM = DISABLE;
	CAN_InitStruct.CAN_NART = DISABLE;
	CAN_InitStruct.CAN_RFLM = DISABLE;
	CAN_InitStruct.CAN_TXFP = ENABLE;
	CAN_InitStruct.CAN_Mode = CAN_Mode_Normal;
	CAN_InitStruct.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStruct.CAN_BS1 = CAN_BS1_12tq;
	CAN_InitStruct.CAN_BS2 = CAN_BS2_8tq;
//	CAN_InitStruct.CAN_SJW = CAN_SJW_1tq;
//	CAN_InitStruct.CAN_BS1 = CAN_BS1_5tq;
//	CAN_InitStruct.CAN_BS2 = CAN_BS2_2tq;
	CAN_InitStruct.CAN_Prescaler = CAN_Prescaler;
	CAN_InitStatus = CAN_Init(CANx, &CAN_InitStruct);

	CAN_FilterInitStruct.CAN_FilterActivation = ENABLE;
	CAN_FilterInitStruct.CAN_FilterFIFOAssignment = CAN_FilterFIFOAssignment;
	CAN_FilterInitStruct.CAN_FilterIdHigh = CAN_FilterId_11bits << 5;
	CAN_FilterInitStruct.CAN_FilterIdLow = 0x0;
	CAN_FilterInitStruct.CAN_FilterMaskIdHigh = CAN_FilterMaskId_11bits << 5;
	CAN_FilterInitStruct.CAN_FilterMaskIdLow = 0x0;
	CAN_FilterInitStruct.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStruct.CAN_FilterNumber = CAN_FilterNumber;
	CAN_FilterInitStruct.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInit(&CAN_FilterInitStruct);

	if(CANx == CAN2){
		CAN_SlaveStartBank(CAN_FilterNumber);
	}
	CAN_NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = N_PPriority;
	CAN_NVIC_InitStructure.NVIC_IRQChannelSubPriority = N_SPriority;
	CAN_NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	if ((CANx == CAN1)&&(CAN_FilterFIFOAssignment == CAN_Filter_FIFO0)){
		CAN_NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
		NVIC_Init(&CAN_NVIC_InitStructure);
		CAN_ITConfig(CANx, CAN_IT_FMP0, ENABLE);
	}
	if ((CANx == CAN1)&&(CAN_FilterFIFOAssignment == CAN_Filter_FIFO1)){
		CAN_NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX1_IRQn;
		NVIC_Init(&CAN_NVIC_InitStructure);
		CAN_ITConfig(CANx, CAN_IT_FMP1, ENABLE);
	}
	if ((CANx == CAN2)&&(CAN_FilterFIFOAssignment == CAN_Filter_FIFO0)){
		CAN_NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
		NVIC_Init(&CAN_NVIC_InitStructure);
		CAN_ITConfig(CANx, CAN_IT_FMP0, ENABLE);
	}
	if ((CANx == CAN2)&&(CAN_FilterFIFOAssignment == CAN_Filter_FIFO1)){
		CAN_NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX1_IRQn;
		NVIC_Init(&CAN_NVIC_InitStructure);
		CAN_ITConfig(CANx, CAN_IT_FMP1, ENABLE);
	}
	return CAN_InitStatus;
}

/* Function Name		: CAN_TxMsg
 * Function Description : This function is called to transmit data with bxCAN.
 * Function Remarks		:
 * Function Arguments	: CANx				CANx can be CAN1 or CAN2
 * 						  StdId_11bits		The standard identifier for the CAN frame message. This parameter
 * 						  					can be a value between 0 to 0x7FF.
 * 						  Msg				a pointer used to store data to CAN frame
 * 						  datalen				length of data
 * Function Return		: mbox				The number of the mailbox that is used for transmission or
  *         								CAN_TxStatus_NoMailBox if there is no empty mailbox.
 * Function Example		: CAN_TxMsg(CAN1,1,&data,1);
 */
uint8_t CAN_TxMsg(CAN_TypeDef* CANx,uint32_t StdId_11bits,uint8_t * Msg,uint8_t datalen)
{
	uint8_t mbox;
	uint8_t* buf = Msg;
	uint16_t i=0;
	CanTxMsg TxMessage;

	TxMessage.StdId = StdId_11bits;
	TxMessage.ExtId = 0;
	TxMessage.IDE = CAN_Id_Standard ;
	TxMessage.RTR = CAN_RTR_Data  ;
	TxMessage.DLC = datalen;

	while(datalen--)
		TxMessage.Data[i++]= *(uint8_t*)buf++;

	mbox = CAN_Transmit(CANx, &TxMessage);

	while((CAN_TransmitStatus(CANx, mbox)!= CAN_TxStatus_Ok));

	return mbox;
}





