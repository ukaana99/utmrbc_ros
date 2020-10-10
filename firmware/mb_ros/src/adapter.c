
#include "adapter.h"
#include <common.h>

char uartbuff[500];
uint8_t mailbox = 0, buf2_flag = 0, buf2_flagC2 = 0;


void Initialize (void){

	BIOSInit();

	BIOSTIMx_Init(TIM6, 20000, 84, 0, 0);
	BIOSTIMx_Init(TIM7, 20000, 84, 0, 0);

	GPIOPinsInit (LED1_PIN, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_Speed_50MHz, GPIO_PuPd_UP);
	GPIOPinsInit (LED2_PIN, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_Speed_50MHz, GPIO_PuPd_UP);
	GPIOPinsInit (LED3_PIN, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_Speed_50MHz, GPIO_PuPd_UP);

	GPIOPinsInit (PB1_PIN, GPIO_Mode_IN, GPIO_OType_PP, GPIO_Speed_50MHz, GPIO_PuPd_UP);
	GPIOPinsInit (PB2_PIN, GPIO_Mode_IN, GPIO_OType_PP, GPIO_Speed_50MHz, GPIO_PuPd_UP);

	/*Normal IOs*/
	GPIOPinsInit (IP1_PIN, GPIO_Mode_IN, GPIO_OType_PP, GPIO_Speed_50MHz, GPIO_PuPd_UP);
	GPIOPinsInit (IP2_PIN, GPIO_Mode_IN, GPIO_OType_PP, GPIO_Speed_50MHz, GPIO_PuPd_UP);
	GPIOPinsInit (IP3_PIN, GPIO_Mode_IN, GPIO_OType_PP, GPIO_Speed_50MHz, GPIO_PuPd_UP);
	GPIOPinsInit (IP4_PIN, GPIO_Mode_IN, GPIO_OType_PP, GPIO_Speed_50MHz, GPIO_PuPd_UP);
	GPIOPinsInit (IP5_PIN, GPIO_Mode_IN, GPIO_OType_PP, GPIO_Speed_50MHz, GPIO_PuPd_UP);
	GPIOPinsInit (IP6_PIN, GPIO_Mode_IN, GPIO_OType_PP, GPIO_Speed_50MHz, GPIO_PuPd_UP);
	GPIOPinsInit (IP7_PIN, GPIO_Mode_IN, GPIO_OType_PP, GPIO_Speed_50MHz, GPIO_PuPd_UP);
	GPIOPinsInit (IP8_PIN, GPIO_Mode_IN, GPIO_OType_PP, GPIO_Speed_50MHz, GPIO_PuPd_UP);
	GPIOPinsInit (IP9_PIN, GPIO_Mode_IN, GPIO_OType_PP, GPIO_Speed_50MHz, GPIO_PuPd_UP);
	GPIOPinsInit (IP10_PIN, GPIO_Mode_IN, GPIO_OType_PP, GPIO_Speed_50MHz, GPIO_PuPd_UP);
	GPIOPinsInit (IP11_PIN, GPIO_Mode_IN, GPIO_OType_PP, GPIO_Speed_50MHz, GPIO_PuPd_UP);
	GPIOPinsInit (IP12_PIN, GPIO_Mode_IN, GPIO_OType_PP, GPIO_Speed_50MHz, GPIO_PuPd_UP);
	GPIOPinsInit (IP13_PIN, GPIO_Mode_IN, GPIO_OType_PP, GPIO_Speed_50MHz, GPIO_PuPd_UP);
	GPIOPinsInit (IP14_PIN, GPIO_Mode_IN, GPIO_OType_PP, GPIO_Speed_50MHz, GPIO_PuPd_UP);
	GPIOPinsInit (IP15_PIN, GPIO_Mode_IN, GPIO_OType_PP, GPIO_Speed_50MHz, GPIO_PuPd_UP);

	/*IOs with ADC*/
	//GPIOPinsInit (IP16_Analog1_PIN, GPIO_Mode_IN, GPIO_OType_PP, GPIO_Speed_50MHz, GPIO_PuPd_UP);
	GPIOPinsInit (IP17_Analog2_PIN, GPIO_Mode_IN, GPIO_OType_PP, GPIO_Speed_50MHz, GPIO_PuPd_UP);
	GPIOPinsInit (IP18_Analog3_PIN, GPIO_Mode_IN, GPIO_OType_PP, GPIO_Speed_50MHz, GPIO_PuPd_UP);
	GPIOPinsInit (IP19_Analog4_PIN, GPIO_Mode_IN, GPIO_OType_PP, GPIO_Speed_50MHz, GPIO_PuPd_UP);
	GPIOPinsInit (IP20_Analog7_PIN, GPIO_Mode_IN, GPIO_OType_PP, GPIO_Speed_50MHz, GPIO_PuPd_UP);
	GPIOPinsInit (IP21_Analog8_PIN, GPIO_Mode_IN, GPIO_OType_PP, GPIO_Speed_50MHz, GPIO_PuPd_UP);

	MUXInit(&MUX, MUX1_INPUT_PIN, MUX1_S0_PIN, MUX1_S1_PIN, MUX1_S2_PIN);
	SHIFTREGInit (&SR, CASCADE_1, SR_SCK_PIN, SR_RCK_PIN, SR_SI_PIN);

//	I2C1Init (main_board_1, CLOCK_SPEED_400KHz, slave_rcv, PS3_rcv, 0);
//	I2C2Init (main_board_1, CLOCK_SPEED_400KHz, slave_rcv, PS3_rcv, 0);
//	I2C3Init (main_board_1, CLOCK_SPEED_400KHz, slave_rcv, PS3_rcv, 0);

	CANx_Init(CAN1, CAN1_Tx, CAN1_Rx, CAN_Filter_FIFO0,0,0,26,4,1,1);
	CANx_Init(CAN2, CAN2_Tx, CAN2_Rx, CAN_Filter_FIFO1,0,0,27,4,1,2);

	UART2Init(115200, ENABLE, 0,0, UART2_Tx, UART2_Rx);
	UART3Init(115200, DISABLE, 0,0, UART3_Tx, UART3_Rx);
	UART4Init(115200, DISABLE, 0,0, UART4_Tx, UART4_Rx);
	UART5Init(115200, DISABLE, 0,0, UART5_Tx, UART5_Rx);

	QEIInit(QEI1,5,5,QEI1_PLUSEA_PIN, QEI1_PLUSEB_PIN);
	QEIInit(QEI4,5,5,QEI4_PLUSEA_PIN, QEI4_PLUSEB_PIN);
	QEIInit(QEI6,5,5,QEI6_PLUSEA_PIN, QEI6_PLUSEB_PIN);

	PWMTimeBaseInit(TIM3, 20000, 84);//50Hz PWM
	PWMChannelConfig(TIM3, PWM_CHANNEL_3, TIM3_CHANNEL3_PIN);
	PWMChannelConfig(TIM3, PWM_CHANNEL_4, TIM3_CHANNEL4_PIN);

	PWMTimeBaseInit(TIM5, 20000, 84);//50Hz PWM
	PWMChannelConfig(TIM5, PWM_CHANNEL_1, TIM5_CHANNEL1_PIN);
	PWMChannelConfig(TIM5, PWM_CHANNEL_2, TIM5_CHANNEL2_PIN);
	PWMChannelConfig(TIM5, PWM_CHANNEL_3, TIM5_CHANNEL3_PIN);
	PWMChannelConfig(TIM5, PWM_CHANNEL_4, TIM5_CHANNEL4_PIN);

	PWMTimeBaseInit(TIM9, 20000, 168);//50Hz PWM
	PWMChannelConfig(TIM9, PWM_CHANNEL_1, TIM9_CHANNEL1_PIN);
	PWMChannelConfig(TIM9, PWM_CHANNEL_2, TIM9_CHANNEL2_PIN);

	BDCInit(&BDC1, &(TIM3->CCR4), SHIFTREG, &(SR.cast[1]), Bit6, Bit7);
	BDCInit(&BDC2, &(TIM3->CCR3), SHIFTREG, &(SR.cast[1]), Bit4, Bit5);
	BDCInit(&BDC3, &(TIM9->CCR1), SHIFTREG, &(SR.cast[1]), Bit2, Bit3);
	BDCInit(&BDC4, &(TIM9->CCR2), SHIFTREG, &(SR.cast[1]), Bit0, Bit1);
	BDCInit(&BDC5, &(TIM5->CCR4), SHIFTREG, &(SR.cast[0]), Bit6, Bit7);
	BDCInit(&BDC6, &(TIM5->CCR3), SHIFTREG, &(SR.cast[0]), Bit4, Bit5);
	BDCInit(&BDC7, &(TIM5->CCR2), SHIFTREG, &(SR.cast[0]), Bit2, Bit3);
	BDCInit(&BDC8, &(TIM5->CCR1), SHIFTREG, &(SR.cast[0]), Bit0, Bit1);

}

//void slave_rcv(unsigned char length, unsigned char *pdata)
//{
//
//	switch (pdata[0]) {
//
//		case main_board_1:
//
//			break;
//
//	}
//}
//
//
//void PS3_rcv(unsigned char length, unsigned char *pdata){
//
////	memcpy(&ps3x.ReceiveBuffer[0], &pdata[0], length);
//
//
//}


void CAN1_RX0_IRQHandler()
{
	if(CAN_GetITStatus(CAN1, CAN_IT_FMP0) == SET){
		CAN_Receive(CAN1, CAN_FIFO0, &CAN1_RxMessage);
	}
	switch(CAN1_RxMessage.StdId){
		case RNS_TO_mainboard:
			memcpy(&insData_receive, &CAN1_RxMessage.Data, CAN1_RxMessage.DLC);
			buf2_flag = 0;

			break;
		case RNS_TO_mainboard_buf1:
			memcpy(&buf1_receive, &CAN1_RxMessage.Data, CAN1_RxMessage.DLC);

			break;
		case RNS_TO_mainboard_buf2:
			memcpy(&buf2_receive, &CAN1_RxMessage.Data, CAN1_RxMessage.DLC);
			buf2_flag = 1;
			break;
		case XY_feedback_state :
			break;
		default:
			break;
	}
	CAN_PROCESS();
	CAN_ClearITPendingBit(CAN1, CAN_IT_FF0);
	CAN_ClearFlag(CAN1, CAN_FLAG_EWG);
	CAN_ClearFlag(CAN1, CAN_FLAG_EPV);
	CAN_ClearFlag(CAN1, CAN_FLAG_BOF);
	CAN_ClearFlag(CAN1, CAN_FLAG_LEC);

	CAN_ClearFlag(CAN1, CAN_FLAG_FMP0);
	CAN_ClearFlag(CAN1, CAN_FLAG_FF0);
	CAN_ClearFlag(CAN1, CAN_FLAG_FOV0);
	CAN_ClearFlag(CAN1, CAN_FLAG_FMP1);
	CAN_ClearFlag(CAN1, CAN_FLAG_FF1);
	CAN_ClearFlag(CAN1, CAN_FLAG_FOV1);
}
//using
void CAN2_RX1_IRQHandler()
{
	if(CAN_GetITStatus(CAN2, CAN_IT_FMP0) == SET){
		CAN_Receive(CAN2, CAN_FIFO1, &CAN2_RxMessage);
	}
	switch(CAN2_RxMessage.StdId){
		case RNS_TO_mainboard:
			memcpy(&insData_receive, &CAN2_RxMessage.Data, CAN2_RxMessage.DLC);
			buf2_flag = 0;

			break;
		case RNS_TO_mainboard_buf1:
			memcpy(&buf1_receive, &CAN2_RxMessage.Data, CAN2_RxMessage.DLC);

			break;
		case RNS_TO_mainboard_buf2:
			memcpy(&buf2_receive, &CAN2_RxMessage.Data, CAN2_RxMessage.DLC);
			buf2_flag = 1;
			break;

		case XY_feedback_state :

			break;
		default:
			break;
	}
	CAN_PROCESS();
	CAN_ClearITPendingBit(CAN2, CAN_IT_FF1);
	CAN_ClearFlag(CAN2, CAN_FLAG_EWG);
	CAN_ClearFlag(CAN2, CAN_FLAG_EPV);
	CAN_ClearFlag(CAN2, CAN_FLAG_BOF);
	CAN_ClearFlag(CAN2, CAN_FLAG_LEC);

	CAN_ClearFlag(CAN2, CAN_FLAG_FMP0);
	CAN_ClearFlag(CAN2, CAN_FLAG_FF0);
	CAN_ClearFlag(CAN2, CAN_FLAG_FOV0);
	CAN_ClearFlag(CAN2, CAN_FLAG_FMP1);
	CAN_ClearFlag(CAN2, CAN_FLAG_FF1);
	CAN_ClearFlag(CAN2, CAN_FLAG_FOV1);
}

void CAN_PROCESS(void){
	if(insData_receive[0] == 1){
		rns.RNS_data.common_instruction = insData_receive[1];
		insData_receive[0]=2;
	}
	if(insData_receive[0] == 17){
		if(buf2_flag == 1){
			rns.RNS_data.common_instruction = insData_receive[1];
			rns.RNS_data.common_buffer[0].data = buf1_receive[0].data;
			rns.RNS_data.common_buffer[1].data = buf1_receive[1].data;
			rns.RNS_data.common_buffer[2].data = buf2_receive[0].data;
			rns.RNS_data.common_buffer[3].data = buf2_receive[1].data;
			insData_receive[0]=3;
		}
	}
}
