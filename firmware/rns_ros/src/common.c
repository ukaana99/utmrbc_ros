
/*********************************************/
/*          Include Header                   */
/*********************************************/

#include "include.h"

/*********************************************/
/*          Variables	            	     */
/*********************************************/

/*********************************************/
/*          Functions	            	     */
/*********************************************/

void Initialize(void)
{
	BIOSInit();

	GPIOPinsInit(GPIOA, GPIO_Pin_5, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_Speed_50MHz, GPIO_PuPd_UP);				 //LED
	GPIOPinsInit(GPIOC, GPIO_Pin_5, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_Speed_50MHz, GPIO_PuPd_UP);				 //LED
	GPIOPinsInit(GPIOE, GPIO_Pin_7 | GPIO_Pin_14, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_Speed_50MHz, GPIO_PuPd_UP); //LED
	GPIOPinsInit(GPIOD, GPIO_Pin_10, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_Speed_50MHz, GPIO_PuPd_UP);				 //LED
	GPIOPinsInit(GPIOC, GPIO_Pin_4, GPIO_Mode_IN, GPIO_OType_PP, GPIO_Speed_50MHz, GPIO_PuPd_UP);				 //PB
	GPIOPinsInit(GPIOE, GPIO_Pin_10, GPIO_Mode_IN, GPIO_OType_PP, GPIO_Speed_50MHz, GPIO_PuPd_UP);				 //PB

	LED1 = 1;
	LED2 = 1;
	LED3 = 1;
	LED4 = 1;
	LED5 = 1;

	SHIFTREGInit(&SR, NO_CASCADE, SR_SCK_PIN, SR_RCK_PIN, SR_SI_PIN);

	QEIInit(QEI1, 1, 2, QEI1_PLUSEA_PIN, QEI1_PLUSEB_PIN);
	QEIInit(QEI2, 2, 0, QEI2_PLUSEA_PIN, QEI2_PLUSEB_PIN);
	QEIInit(QEI3, 1, 3, QEI3_PLUSEA_PIN, QEI3_PLUSEB_PIN);
	QEIInit(QEI4, 1, 1, QEI4_PLUSEA_PIN, QEI4_PLUSEB_PIN);
	QEIInit(QEI5, 2, 1, QEI5_PLUSEA_PIN, QEI5_PLUSEB_PIN);
	QEIInit(QEI6, 1, 0, QEI6_PLUSEA_PIN, QEI6_PLUSEB_PIN);

	UART2Init(115200, ENABLE, 0, 0, UART2_Tx_PIN, UART2_Rx_PIN);
	UART5Init(115200, ENABLE, 1, 0, UART5_Tx_PIN, UART5_Rx_PIN);

	//	UART5TxDMAInit(DMA_Priority_Medium, DISABLE, 2, 1);

	PWMTimeBaseInit(TIM9, 20000, 4);
	PWMTimeBaseInit(TIM12, 20000, 2);

	PWMChannelConfig(TIM9, PWM_CHANNEL_1, PWM_TIM9_CH1_PIN);
	PWMChannelConfig(TIM9, PWM_CHANNEL_2, PWM_TIM9_CH2_PIN);
	PWMChannelConfig(TIM12, PWM_CHANNEL_1, PWM_TIM12_CH1_PIN);
	PWMChannelConfig(TIM12, PWM_CHANNEL_2, PWM_TIM12_CH2_PIN);
}

void wheel_config()
{

	PIDGainSet(SAT, 1.0, &fleft_vel);
	PIDGainSet(KE, 1 / 6.8, &fleft_vel);
	PIDGainSet(KU, 19999.0, &fleft_vel);
	//case RNS_F_LEFT_VEL_PID :
	PIDGainSet(KP, 3.5, &fleft_vel);
	PIDGainSet(KI, 10.5, &fleft_vel);
	PIDGainSet(KD, 0.0, &fleft_vel);
	//case RNS_B_LEFT_VEL_SATEU :
	PIDGainSet(SAT, 1.0, &bleft_vel);
	PIDGainSet(KE, 1 / 7.00, &bleft_vel);
	PIDGainSet(KU, 19999.0, &bleft_vel);
	//case RNS_B_LEFT_VEL_PID :
	PIDGainSet(KP, 4.30, &bleft_vel);
	PIDGainSet(KI, 9.80, &bleft_vel);
	PIDGainSet(KD, 0.0, &bleft_vel);
	//case RNS_F_RIGHT_VEL_SATEU :
	PIDGainSet(SAT, 1.0, &fright_vel);
	PIDGainSet(KE, 1 / 6.50, &fright_vel);
	PIDGainSet(KU, 19999.0, &fright_vel);
	//case RNS_F_RIGHT_VEL_PID :
	PIDGainSet(KP, 3.80, &fright_vel);
	PIDGainSet(KI, 9.0, &fright_vel);
	PIDGainSet(KD, 0.0, &fright_vel);
	//case RNS_B_RIGHT_VEL_SATEU :

	PIDGainSet(SAT, 1.0, &bright_vel);
	PIDGainSet(KE, 1 / 6.65, &bright_vel);
	PIDGainSet(KU, 19999.0, &bright_vel);

	//case RNS_B_RIGHT_VEL_PID :
	PIDGainSet(KP, 3.5, &bright_vel);
	PIDGainSet(KI, 13.80, &bright_vel);
	PIDGainSet(KD, 0.0, &bright_vel);

	dev_cfg.motor_enc_dir = 0b00000101;
	if (dev_cfg.motor1_enc == 0)
		QEISwap(QEI6, QEI_No_Swap);
	else
		QEISwap(QEI6, QEI_Swap);

	if (dev_cfg.motor2_enc == 0)
		QEISwap(QEI4, QEI_No_Swap);
	else
		QEISwap(QEI4, QEI_Swap);

	if (dev_cfg.motor3_enc == 0)
		QEISwap(QEI1, QEI_No_Swap);
	else
		QEISwap(QEI1, QEI_Swap);

	if (dev_cfg.motor4_enc == 0)
		QEISwap(QEI3, QEI_No_Swap);
	else
		QEISwap(QEI3, QEI_Swap);

	xPtd = 0.05 / 4000.0 * 3.142;
	yPtd = 0.05 / 4000.0 * 3.142;

	QEISwap(QEI2, QEI_Swap);
	QEISwap(QEI5, QEI_No_Swap);

	fFKcd = 517.5 / 516.5;
	fFPtd = 0.125 * 3.142 / 517.5;
	fBKcd = 521.1 / 520.7;
	fBPtd = 0.125 * 3.142 / 521.1;
}

void button(void)
{

	if (BUTTON1 == 0)
	{
		LED2 = 0;
		mmode++;
		while (BUTTON1 == 0)
		{
			LED2 = 0;
		}
		LED2 = 1;
	}
	else if (BUTTON2 == 0)
	{
		while (BUTTON2 == 0)
		{
			LED2 = 0;
		}
		LED2 = 1;
		switch (mmode)
		{
		case 0:
			LED2 = 1;
			break;

		case 1:
			ins.instruction = RNS_PDC;
			ins.ins_buffer[0].data = 4000;
			ins.ins_buffer[1].data = 4000;
			ins.ins_buffer[2].data = 4000;
			ins.ins_buffer[3].data = 4000;

			APPApply(&ins);
			APPStart(&ins);
			sys.activate = 1;
			break;

		case 2:
			ins.instruction = RNS_PDC;
			ins.ins_buffer[0].data = -4000;
			ins.ins_buffer[1].data = -4000;
			ins.ins_buffer[2].data = -4000;
			ins.ins_buffer[3].data = -4000;

			APPApply(&ins);
			APPStart(&ins);
			sys.activate = 1;
			break;

		case 3:
			APPStop();
			break;

		case 4:
			while (BUTTON2 == 1)
			{
				//				sprintf(uartbuf,"FL = %f\tFR = %f\tBL = %f\tBR = %f\tx_enc = %f\ty_enc = %f\r\n",
				//						fFLeftPosData, fFRightPosData, fBLeftPosData,fBRightPosData, fXEncData, fYEncData);
				//				sprintf(uartbuf,"FL = %f\tFR = %f\tBL = %f\tBR = %f\tx_enc = %f\ty_enc = %f\r\n",
				//									fFLeftVel, fFRightVel, fBLeftVel,fBRightVel, fXEncData, fYEncData);
				//				UART5_DMA_Send(uartbuf, strlen(uartbuf));
				//				UARTPrintString(USART2,uartbuf);
			}
			while (BUTTON2 == 0)
				;
			mmode = 0;
			break;

		default:
			mmode = 0;
			break;
		}
	}
}
