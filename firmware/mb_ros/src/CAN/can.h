
#ifndef CAN_CAN_H_
#define CAN_CAN_H_

#ifdef __cplusplus
extern "C" {
#endif

extern int testn;
/************************************************
 * @Title   : Controller area network (bxCAN)
 * @Author  : See Qiu Hui
 * @Version : 1.00
 * @Date    : 23 January 2018
 * **********************************************
 * @Descriptions:
 * - CAN library used to communicate one to another board.
 * - Connection to CAN Transceiver is necessary to interface the CAN network:
 * 		i. CANx_Rx_pin connected to RXD pin of CAN Transceiver.
 * 	   ii. CANx_Tx_pin connected to TXD pin of CAN Transceiver.
 * - On message reception, a receiver node can decide whether the message copied into the SRAM or not by using filter:
 * 		i. if((CAN_RX_ID & CAN_FILTER_MASK) == (CAN_FILTER_ID & CAN_FILTER_MASK))
 * 				Copy Data in Buffer;
 *		   else
 * 				Discard the message;
 * @Bugs:
 *
 ************************************************/

/***************************************
 * 		Include Libraries 			   *
 **************************************/
#include "BIOS/bios.h"

/**************************************************
 * 		STRUCTURE DEFINES					  	  *
 *************************************************/
CAN_InitTypeDef			CAN_InitStruct;
CAN_FilterInitTypeDef	CAN_FilterInitStruct;
NVIC_InitTypeDef		CAN_NVIC_InitStructure;

CanRxMsg 				CAN1_RxMessage,CAN2_RxMessage;

extern uint8_t status_send;
/**************************************************
 * 		Function Prototype			  			  *
 *************************************************/
uint8_t CANx_Init(CAN_TypeDef* CANx, GPIO_TypeDef * CANx_Tx_GPIOx, uint16_t CANx_Tx_pin,
		GPIO_TypeDef * CANx_Rx_GPIOx, uint16_t CANx_Rx_pin,uint16_t CAN_FilterFIFOAssignment,
		uint16_t CAN_FilterId_11bits,uint16_t CAN_FilterMaskId_11bits,uint8_t CAN_FilterNumber,
		uint16_t CAN_Prescaler,uint8_t N_PPriority,uint8_t N_SPriority);
uint8_t CAN_TxMsg(CAN_TypeDef* CANx,uint32_t StdId_11bits,uint8_t * Msg,uint8_t datalen);

#ifdef __cplusplus
}
#endif


#endif /* CAN_CAN_H_ */
