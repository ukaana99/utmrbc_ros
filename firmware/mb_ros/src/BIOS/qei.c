#ifdef __cplusplus
extern "C" {
#endif

#include "qei.h"

/************************************************/
/*		 	 	Declarations		  		  	*/
/************************************************/

qei_TypeDef BIOS_QEI1;
qei_TypeDef BIOS_QEI2;
qei_TypeDef BIOS_QEI3;
qei_TypeDef BIOS_QEI4;
qei_TypeDef BIOS_QEI5;
qei_TypeDef BIOS_QEI6;
enc_TypeDef qei;


/************************************************/
/*		 	 	Functions		       		  	*/
/************************************************/

/*
 * Function Name		: QEIInit
 * Function Description : This function is called to initialize desired QEIx, where x can be 1 to 5.
 * Function Remarks		: QEI1(TIM1)
 * 						  QEI2(TIM2)
 * 						  QEI3(TIM3)
 * 						  QEI4(TIM4)
 * 						  QEI5(TIM5)
 * 						  QEI6(TIM8)
 * Function Arguments	: QEIx, where x can be 1 to 5.
 * 						  preemptionpriority    interrupt with higher preemption priority executed first
 * 						  subpriority			when 2 interrupts have similar preemption priorities, interrupt with higher
 * 						  						priority will be executed first. If 2 interrupts have similar preemption
 * 						  						subpriority, then the one comes first in the program will be executed first.
 * 						  GPIOx_pulseA			GPIOx group of QEI pulse A pin(x = A,B,C,D or E)
 * 						  GPIO_Pin_pulseA		GPIO_Pin_x of QEI pulse A pin(x = 0,1,2,...or 15)
 * 						  GPIOx	_pulseA			GPIOx group of QEI pulse B pin(x = A,B,C,D or E)
 * 						  GPIO_Pin_pulseA		GPIO_Pin_x of QEI pulse B pin(x = 0,1,2,...or 15)
 * Function Return		: None
 * Function Example		: QEIInit(QEI1, 0, 1, GPIOE, GPIO_Pin_9, GPIOE, GPIO_Pin_11);  	//Initialize QEI1
 * 						  QEIInit(QEI2, 2, 2, GPIOE, GPIO_Pin_9, GPIOE, GPIO_Pin_11);	//Initialize QEI2
 * 						  QEIInit(QEI3, 5, 4, GPIOE, GPIO_Pin_9, GPIOE, GPIO_Pin_11);	//Initialize QEI3
 * 						  QEIInit(QEI4, 7, 6, GPIOE, GPIO_Pin_9, GPIOE, GPIO_Pin_11);  	//Initialize QEI4
 * 						  QEIInit(QEI5, 9, 7, GPIOE, GPIO_Pin_9, GPIOE, GPIO_Pin_11);	//Initialize QEI5
 * 						  QEIInit(QEI6, 3, 8, GPIOE, GPIO_Pin_9, GPIOE, GPIO_Pin_11);	//Initialize QEI6
 */

void QEIInit(QEI_TypeDef QEIx, uint16_t preemptionpriority,uint16_t subpriority,
		  GPIO_TypeDef* GPIOx_pulseA, uint16_t GPIO_Pin_pulseA, GPIO_TypeDef* GPIOx_pulseB, uint16_t GPIO_Pin_pulseB)
{
	switch(QEIx)
	{
	case QEI1:
		QEI1Init(preemptionpriority,subpriority, GPIOx_pulseA, GPIO_Pin_pulseA, GPIOx_pulseB, GPIO_Pin_pulseB);
		break;
	case QEI2:
		QEI2Init(preemptionpriority,subpriority, GPIOx_pulseA, GPIO_Pin_pulseA, GPIOx_pulseB, GPIO_Pin_pulseB);
		break;
	case QEI3:
		QEI3Init(preemptionpriority,subpriority, GPIOx_pulseA, GPIO_Pin_pulseA, GPIOx_pulseB, GPIO_Pin_pulseB);
		break;
	case QEI4:
		QEI4Init(preemptionpriority,subpriority, GPIOx_pulseA, GPIO_Pin_pulseA, GPIOx_pulseB, GPIO_Pin_pulseB);
		break;
	case QEI5:
		QEI5Init(preemptionpriority,subpriority, GPIOx_pulseA, GPIO_Pin_pulseA, GPIOx_pulseB, GPIO_Pin_pulseB);
		break;
	case QEI6:
		QEI6Init(preemptionpriority,subpriority, GPIOx_pulseA, GPIO_Pin_pulseA, GPIOx_pulseB, GPIO_Pin_pulseB);
		break;
	default: break;
	}
}

/*
 * Function Name		: QEIPrintValueStore
 * Function Description : Print QEIs' Values.
 * Function Remarks		: Called this function in while(1) to print QEIs' values, x can be 1 to 5.
 * Function Arguments	: USARTx, where x can be 1 to 6.
 * Function Return		: None
 * Function Example		: void main(main)
 * 							{
 * 								while(1)
 * 								{
 * 									 QEIPrintValueStore(UART5);	//Print QEIs' Value using UART5
 * 								}
 * 							}
 */

void QEIPrintValueStore(USART_TypeDef* USARTx)
{
	qei.l[0].LONG = TIM1-> CNT;
	qei.l[1].LONG = TIM2-> CNT;
	qei.l[2].LONG = TIM3-> CNT;
	qei.l[3].LONG = TIM4-> CNT;
	qei.l[4].LONG = TIM5-> CNT;
	qei.l[5].LONG = TIM6-> CNT;
	char QEIbuf[100];
	sprintf(QEIbuf,"QEI1=%d\tQEI2=%d\tQEI3=%d\tQEI4=%d\tQEI5=%d\tQEI6=%d\r\n",qei.l[0].LONG, qei.l[1].LONG,qei.l[2].LONG,qei.l[3].LONG,qei.l[4].LONG,qei.l[5].LONG);
	UARTPrintString(USARTx, QEIbuf);
}

/*
 * Function Name		: QEIWrite
 * Function Description : This function is called to write value QEI.
 * Function Remarks		: None
 * Function Arguments	: QEIx		where x can be 1 to 5.
 * 						  value		desired value to be set
 * Function Return		: None
 * Function Example		: QEIWrite(QEI1, 10000);
 */

void QEIWrite(QEI_TypeDef QEIx, int32_t value)
{

	switch (QEIx){

		case QEI1:	BIOS_QEI1.count = value;
					TIM1->CNT = BIOS_QEI1.poscnt;
					break;

		case QEI2:	BIOS_QEI2.count = value;
					TIM2->CNT = BIOS_QEI2.poscnt;
					break;

		case QEI3:  BIOS_QEI3.count = value;
					TIM3->CNT = BIOS_QEI3.poscnt;
					break;

		case QEI4:  BIOS_QEI4.count = value;
					TIM4->CNT = BIOS_QEI4.poscnt;
					break;

		case QEI5:	BIOS_QEI5.count = value;
					TIM5->CNT = BIOS_QEI5.poscnt;
					break;

		case QEI6:	BIOS_QEI6.count = value;
					TIM8->CNT = BIOS_QEI6.poscnt;
					break;

	}

}

/*
 * Function Name		: QEIReset
 * Function Description : This function is called to reset value of QEI.
 * Function Remarks		: None
 * Function Arguments	: QEIx		where x can be 1 to 5
 * Function Return		: None
 * Function Example		: QEIReset(QEI1);
 */

void QEIReset(QEI_TypeDef QEIx)
{

	switch (QEIx){

		case QEI1:	TIM1->CNT = 0;
					BIOS_QEI1.count = 0;
					break;

		case QEI2:	TIM2->CNT = 0;
					BIOS_QEI2.count = 0;
					break;

		case QEI3:	TIM3->CNT = 0;
					BIOS_QEI3.count = 0;
					break;

		case QEI4:	TIM4->CNT = 0;
					BIOS_QEI4.count = 0;
					break;

		case QEI5:	TIM5->CNT = 0;
					BIOS_QEI5.count = 0;
					break;

		case QEI6:	TIM8->CNT = 0;
					BIOS_QEI6.count = 0;
					break;

	}
}

/*
 * Function Name		: QEISwap
 * Function Description : This function is called to swap the direction of QEI's value.
 * Function Remarks		: None
 * Function Arguments	: QEIx		where x can be 1 to 5.
 * 						  swap		can be QEI_No_Swap or QEI_Swap
 * Function Return		: None
 * Function Example		: QEISwap(QEI1, QEI_Swap);
 */

void QEISwap(QEI_TypeDef QEIx, QEI_Direction_TypeDef swap )
{

	switch(QEIx){

		case QEI1:	if(swap == QEI_No_Swap)
						TIM_EncoderInterfaceConfig(TIM1, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
					else if(swap == QEI_Swap)
						TIM_EncoderInterfaceConfig(TIM1, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Rising);

					break;

		case QEI2:	if(swap == QEI_No_Swap)
						TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
					else if(swap == QEI_Swap)
						TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Rising);

					break;

		case QEI3:	if(swap == QEI_No_Swap)
						TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
					else if(swap == QEI_Swap)
						TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Rising);

					break;

		case QEI4:	if(swap == QEI_No_Swap)
						TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
					else if(swap == QEI_Swap)
						TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Rising);

					break;

		case QEI5:	if(swap == QEI_No_Swap)
						TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
					else if(swap == QEI_Swap)
						TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Rising);

					break;

		case QEI6:	if(swap == QEI_No_Swap)
						TIM_EncoderInterfaceConfig(TIM8, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
					else if(swap == QEI_Swap)
						TIM_EncoderInterfaceConfig(TIM8, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Rising);

					break;

		default: break;

	}

}

/*
 * Function Name		: QEIDelay
 * Function Description : This function is called to induce delay in timer interrupt.
 * Function Remarks		: This function is called by developer in corresponding timer interrupt for respective QEIx,
 * 						  x can be 1 to 5.
 * Function Arguments	: value							100 for TIM2 to TIM5
 * 														200 for TIM1 and TIM8
 * Function Return		: None
 * Function Example		: QEIDelay(100);
 */

void QEIDelay(uint8_t value)
{
	uint8_t currentvalues;
	value = value * 8400;
	while(currentvalues < value)
	{
		currentvalues++;
	}
	currentvalues = 0;
}

/*
 * Function Name		: QEIRead
 * Function Description : This function is called to raed of QEI's value.
 * Function Remarks		: None
 * Function Arguments	: QEIx		where x can be 1 to 5.
 * Function Return		: QEI's count with 32 bits with sign.
 * Function Example		: QEIRead(QEI1);
 */

int32_t QEIRead(QEI_TypeDef QEIx){

	int32_t value;
	switch(QEIx){

		case QEI1:	BIOS_QEI1.poscnt = TIM1-> CNT;
					value = BIOS_QEI1.count;
					break;

		case QEI2:	BIOS_QEI2.poscnt = TIM2-> CNT;
					value = BIOS_QEI2.count;
					break;

		case QEI3:	BIOS_QEI3.poscnt = TIM3-> CNT;
					value = BIOS_QEI3.count;
					break;

		case QEI4:	BIOS_QEI4.poscnt = TIM4-> CNT;
					value = BIOS_QEI4.count;
					break;

		case QEI5:	BIOS_QEI5.poscnt = TIM5-> CNT;
					value = BIOS_QEI5.count;
					break;

		case QEI6:	BIOS_QEI6.poscnt = TIM8-> CNT;
					value = BIOS_QEI6.count;
					break;

		default: break;
	}

	return value;
}

/*
 * Function Name		: QEIReadRaw
 * Function Description : This function is called to raed of QEI's raw value.
 * Function Remarks		: None
 * Function Arguments	: QEIx		where x can be 1 to 5.
 * Function Return		: QEI's raw count wuth range from 0 until 65535
 * Function Example		: QEIRead(QEI1);
 */

uint32_t QEIReadRaw(QEI_TypeDef QEIx){

	uint32_t value;
	switch(QEIx){

		case QEI1:	value = TIM1->CNT;
					break;

		case QEI2:	value = TIM2->CNT;
					break;

		case QEI3:	value = TIM3->CNT;
					break;

		case QEI4:	value = TIM4->CNT;
					break;

		case QEI5:	value = TIM5->CNT;
					break;

		case QEI6:	value = TIM8->CNT;
					break;

		default: break;
	}

	return value;
}

/*
 * Function Name		: QEI1Init
 * Function Description : This function is called initialize QEI1.
 * Function Remarks		: QEI1 is using PE9 and PE11 as Pulse A and Pulse B output.
 * Function Arguments	: preemptionpriority    interrupt with higher preemption priority executed first
 * 						  subpriority			when 2 interrupts have similar preemption priorities, interrupt with higher
 * 						  						priority will be executed first. If 2 interrupts have similar preemption
 * 						  						subpriority, then the one comes first in the program will be executed first.
 * 						  GPIOx_pulseA			GPIOx group of QEI pulse A pin(x = A,B,C,D or E)
 * 						  GPIO_Pin_pulseA		GPIO_Pin_x of QEI pulse A pin(x = 0,1,2,...or 15)
 * 						  GPIOx	_pulseA			GPIOx group of QEI pulse B pin(x = A,B,C,D or E)
 * 						  GPIO_Pin_pulseA		GPIO_Pin_x of QEI pulse B pin(x = 0,1,2,...or 15)
 * Function Return		: None
 * Function Example		: QEI1Init();
 */

void QEI1Init(uint16_t preemptionpriority, uint16_t subpriority,
				  GPIO_TypeDef* GPIOx_pulseA, uint16_t GPIO_Pin_pulseA, GPIO_TypeDef* GPIOx_pulseB, uint16_t GPIO_Pin_pulseB)
{

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	GPIOPinsInit (GPIOx_pulseA, GPIO_Pin_pulseA, GPIO_Mode_AF, GPIO_OType_PP,GPIO_Speed_50MHz,GPIO_PuPd_UP);
	GPIOPinsInit (GPIOx_pulseB, GPIO_Pin_pulseB, GPIO_Mode_AF, GPIO_OType_PP,GPIO_Speed_50MHz,GPIO_PuPd_UP);
	AFConfig(TIM1,GPIOx_pulseA, GPIO_Pin_pulseA);
	AFConfig(TIM1,GPIOx_pulseB, GPIO_Pin_pulseB);

	BIOS_QEI1_TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
	BIOS_QEI1_TIM_TimeBaseStructure.TIM_Prescaler = 0;
	BIOS_QEI1_TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	BIOS_QEI1_TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1, &BIOS_QEI1_TIM_TimeBaseStructure);
	TIM_EncoderInterfaceConfig(TIM1, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_ARRPreloadConfig(TIM1, ENABLE);

	TIM_ClearITPendingBit(TIM1, TIM_IT_Update );
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);

	BIOS_QEI1_NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;
	BIOS_QEI1_NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemptionpriority;
	BIOS_QEI1_NVIC_InitStructure.NVIC_IRQChannelSubPriority = subpriority;
	BIOS_QEI1_NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&BIOS_QEI1_NVIC_InitStructure);
	TIM_Cmd(TIM1, ENABLE);
}

/*
 * Function Name		: QEI2Init
 * Function Description : This function is called initialize QEI2.
 * Function Remarks		: QEI2 is using PA15 and PB3 as Pulse A and Pulse B output.
 * Function Arguments	: preemptionpriority    interrupt with higher preemption priority executed first
 * 						  subpriority			when 2 interrupts have similar preemption priorities, interrupt with higher
 * 						  						priority will be executed first. If 2 interrupts have similar preemption
 * 						  						subpriority, then the one comes first in the program will be executed first.
 * 						  GPIOx_pulseA			GPIOx group of QEI pulse A pin(x = A,B,C,D or E)
 * 						  GPIO_Pin_pulseA		GPIO_Pin_x of QEI pulse A pin(x = 0,1,2,...or 15)
 * 						  GPIOx	_pulseA			GPIOx group of QEI pulse B pin(x = A,B,C,D or E)
 * 						  GPIO_Pin_pulseA		GPIO_Pin_x of QEI pulse B pin(x = 0,1,2,...or 15)
 * Function Return		: None
 * Function Example		: QEI2Init();
 */

void QEI2Init(uint16_t preemptionpriority, uint16_t subpriority,
		  GPIO_TypeDef* GPIOx_pulseA, uint16_t GPIO_Pin_pulseA, GPIO_TypeDef* GPIOx_pulseB, uint16_t GPIO_Pin_pulseB)
{

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	GPIOPinsInit (GPIOx_pulseA, GPIO_Pin_pulseA, GPIO_Mode_AF, GPIO_OType_PP,GPIO_Speed_50MHz,GPIO_PuPd_UP);
	GPIOPinsInit (GPIOx_pulseB, GPIO_Pin_pulseB, GPIO_Mode_AF, GPIO_OType_PP,GPIO_Speed_50MHz,GPIO_PuPd_UP);
	AFConfig(TIM2,GPIOx_pulseA, GPIO_Pin_pulseA);
	AFConfig(TIM2,GPIOx_pulseB, GPIO_Pin_pulseB);

	BIOS_QEI2_TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
	BIOS_QEI2_TIM_TimeBaseStructure.TIM_Prescaler = 0;
	BIOS_QEI2_TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	BIOS_QEI2_TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &BIOS_QEI2_TIM_TimeBaseStructure);
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_ARRPreloadConfig(TIM2, ENABLE);

	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	BIOS_QEI2_NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	BIOS_QEI2_NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemptionpriority;
	BIOS_QEI2_NVIC_InitStructure.NVIC_IRQChannelSubPriority = subpriority;
	BIOS_QEI2_NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&BIOS_QEI2_NVIC_InitStructure);

	TIM_Cmd(TIM2, ENABLE);
}

/*
 * Function Name		: QEI3Init
 * Function Description : This function is called initialize QEI3.
 * Function Remarks		: QEI3 is using PC6 and PC7 as Pulse A and Pulse B output.
 * Function Arguments	: preemptionpriority    interrupt with higher preemption priority executed first
 * 						  subpriority			when 2 interrupts have similar preemption priorities, interrupt with higher
 * 						  						priority will be executed first. If 2 interrupts have similar preemption
 * 						  						subpriority, then the one comes first in the program will be executed first.
 * 						  GPIOx_pulseA			GPIOx group of QEI pulse A pin(x = A,B,C,D or E)
 * 						  GPIO_Pin_pulseA		GPIO_Pin_x of QEI pulse A pin(x = 0,1,2,...or 15)
 * 						  GPIOx	_pulseA			GPIOx group of QEI pulse B pin(x = A,B,C,D or E)
 * 						  GPIO_Pin_pulseA		GPIO_Pin_x of QEI pulse B pin(x = 0,1,2,...or 15)
 * Function Return		: None
 * Function Example		: QEI3Init();
 */

void QEI3Init(uint16_t preemptionpriority, uint16_t subpriority,
		  GPIO_TypeDef* GPIOx_pulseA, uint16_t GPIO_Pin_pulseA, GPIO_TypeDef* GPIOx_pulseB, uint16_t GPIO_Pin_pulseB)
{

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	GPIOPinsInit (GPIOx_pulseA, GPIO_Pin_pulseA, GPIO_Mode_AF, GPIO_OType_PP,GPIO_Speed_50MHz,GPIO_PuPd_UP);
	GPIOPinsInit (GPIOx_pulseB, GPIO_Pin_pulseB, GPIO_Mode_AF, GPIO_OType_PP,GPIO_Speed_50MHz,GPIO_PuPd_UP);
	AFConfig(TIM3,GPIOx_pulseA, GPIO_Pin_pulseA);
	AFConfig(TIM3,GPIOx_pulseB, GPIO_Pin_pulseB);

	BIOS_QEI3_TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
	BIOS_QEI3_TIM_TimeBaseStructure.TIM_Prescaler = 0;
	BIOS_QEI3_TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	BIOS_QEI3_TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &BIOS_QEI3_TIM_TimeBaseStructure);
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_ARRPreloadConfig(TIM3, ENABLE);

	TIM_ClearITPendingBit(TIM3, TIM_IT_Update );
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

	BIOS_QEI3_NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	BIOS_QEI3_NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemptionpriority;
	BIOS_QEI3_NVIC_InitStructure.NVIC_IRQChannelSubPriority = subpriority;
	BIOS_QEI3_NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&BIOS_QEI3_NVIC_InitStructure);
	TIM_Cmd(TIM3, ENABLE);
}

/*
 * Function Name		: QEI4Init
 * Function Description : This function is called initialize QEI4.
 * Function Remarks		: QEI4 is using PB6 and PB7 as Pulse A and Pulse B output.
 * Function Arguments	: preemptionpriority    interrupt with higher preemption priority executed first
 * 						  subpriority			when 2 interrupts have similar preemption priorities, interrupt with higher
 * 						  						priority will be executed first. If 2 interrupts have similar preemption
 * 						  						subpriority, then the one comes first in the program will be executed first.
 * 						  GPIOx_pulseA			GPIOx group of QEI pulse A pin(x = A,B,C,D or E)
 * 						  GPIO_Pin_pulseA		GPIO_Pin_x of QEI pulse A pin(x = 0,1,2,...or 15)
 * 						  GPIOx	_pulseA			GPIOx group of QEI pulse B pin(x = A,B,C,D or E)
 * 						  GPIO_Pin_pulseA		GPIO_Pin_x of QEI pulse B pin(x = 0,1,2,...or 15)
 * Function Return		: None
 * Function Example		: QEI4Init();
 */

void QEI4Init(uint16_t preemptionpriority, uint16_t subpriority,
		  GPIO_TypeDef* GPIOx_pulseA, uint16_t GPIO_Pin_pulseA, GPIO_TypeDef* GPIOx_pulseB, uint16_t GPIO_Pin_pulseB)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	GPIOPinsInit (GPIOx_pulseA, GPIO_Pin_pulseA, GPIO_Mode_AF, GPIO_OType_PP,GPIO_Speed_50MHz,GPIO_PuPd_UP);
	GPIOPinsInit (GPIOx_pulseB, GPIO_Pin_pulseB, GPIO_Mode_AF, GPIO_OType_PP,GPIO_Speed_50MHz,GPIO_PuPd_UP);
	AFConfig(TIM4,GPIOx_pulseA, GPIO_Pin_pulseA);
	AFConfig(TIM4,GPIOx_pulseB, GPIO_Pin_pulseB);

	BIOS_QEI4_TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
	BIOS_QEI4_TIM_TimeBaseStructure.TIM_Prescaler = 0;
	BIOS_QEI4_TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	BIOS_QEI4_TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &BIOS_QEI4_TIM_TimeBaseStructure);
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_ARRPreloadConfig(TIM4, ENABLE);

	TIM_ClearITPendingBit(TIM4, TIM_IT_Update );
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

	BIOS_QEI4_NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	BIOS_QEI4_NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemptionpriority;
	BIOS_QEI4_NVIC_InitStructure.NVIC_IRQChannelSubPriority = subpriority;
	BIOS_QEI4_NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&BIOS_QEI4_NVIC_InitStructure);
	TIM_Cmd(TIM4, ENABLE);
}

/*
 * Function Name		: QEI5Init
 * Function Description : This function is called initialize QEI5.
 * Function Remarks		: QEI5 is using PA0 and PA1 as Pulse A and Pulse B output.
 * Function Arguments	: preemptionpriority    interrupt with higher preemption priority executed first
 * 						  subpriority			when 2 interrupts have similar preemption priorities, interrupt with higher
 * 						  						priority will be executed first. If 2 interrupts have similar preemption
 * 						  						subpriority, then the one comes first in the program will be executed first.
 * 						  GPIOx_pulseA			GPIOx group of QEI pulse A pin(x = A,B,C,D or E)
 * 						  GPIO_Pin_pulseA		GPIO_Pin_x of QEI pulse A pin(x = 0,1,2,...or 15)
 * 						  GPIOx	_pulseA			GPIOx group of QEI pulse B pin(x = A,B,C,D or E)
 * 						  GPIO_Pin_pulseA		GPIO_Pin_x of QEI pulse B pin(x = 0,1,2,...or 15)
 * Function Return		: None
 * Function Example		: QEI5Init();
 */

void QEI5Init(uint16_t preemptionpriority, uint16_t subpriority,
		  GPIO_TypeDef* GPIOx_pulseA, uint16_t GPIO_Pin_pulseA, GPIO_TypeDef* GPIOx_pulseB, uint16_t GPIO_Pin_pulseB)
{

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

	GPIOPinsInit (GPIOx_pulseA, GPIO_Pin_pulseA, GPIO_Mode_AF, GPIO_OType_PP,GPIO_Speed_50MHz,GPIO_PuPd_UP);
	GPIOPinsInit (GPIOx_pulseB, GPIO_Pin_pulseB, GPIO_Mode_AF, GPIO_OType_PP,GPIO_Speed_50MHz,GPIO_PuPd_UP);
	AFConfig(TIM5,GPIOx_pulseA, GPIO_Pin_pulseA);
	AFConfig(TIM5,GPIOx_pulseB, GPIO_Pin_pulseB);

	BIOS_QEI5_TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
	BIOS_QEI5_TIM_TimeBaseStructure.TIM_Prescaler = 0;
	BIOS_QEI5_TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	BIOS_QEI5_TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM5, &BIOS_QEI5_TIM_TimeBaseStructure);
	TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_ARRPreloadConfig(TIM5, ENABLE);

	TIM_ClearITPendingBit(TIM5, TIM_IT_Update );
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);

	BIOS_QEI5_NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	BIOS_QEI5_NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemptionpriority;
	BIOS_QEI5_NVIC_InitStructure.NVIC_IRQChannelSubPriority = subpriority;
	BIOS_QEI5_NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&BIOS_QEI5_NVIC_InitStructure);
	TIM_Cmd(TIM5, ENABLE);
}

/*
 * Function Name		: QEI6Init
 * Function Description : This function is called initialize QEI6.
 * Function Remarks		: QEI6 is using PC6 and PC7 as Pulse A and Pulse B output.
 * Function Arguments	: preemptionpriority    interrupt with higher preemption priority executed first
 * 						  subpriority			when 2 interrupts have similar preemption priorities, interrupt with higher
 * 						  						priority will be executed first. If 2 interrupts have similar preemption
 * 						  						subpriority, then the one comes first in the program will be executed first.
 * 						  GPIOx_pulseA			GPIOx group of QEI pulse A pin(x = A,B,C,D or E)
 * 						  GPIO_Pin_pulseA		GPIO_Pin_x of QEI pulse A pin(x = 0,1,2,...or 15)
 * 						  GPIOx	_pulseA			GPIOx group of QEI pulse B pin(x = A,B,C,D or E)
 * 						  GPIO_Pin_pulseA		GPIO_Pin_x of QEI pulse B pin(x = 0,1,2,...or 15)
 * Function Return		: None
 * Function Example		: QEI6Init(0,1);
 */

void QEI6Init(uint16_t preemptionpriority, uint16_t subpriority,
		  GPIO_TypeDef* GPIOx_pulseA, uint16_t GPIO_Pin_pulseA, GPIO_TypeDef* GPIOx_pulseB, uint16_t GPIO_Pin_pulseB)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);

	GPIOPinsInit (GPIOx_pulseA, GPIO_Pin_pulseA, GPIO_Mode_AF, GPIO_OType_PP,GPIO_Speed_50MHz,GPIO_PuPd_UP);
	GPIOPinsInit (GPIOx_pulseB, GPIO_Pin_pulseB, GPIO_Mode_AF, GPIO_OType_PP,GPIO_Speed_50MHz,GPIO_PuPd_UP);
	AFConfig(TIM8,GPIOx_pulseA, GPIO_Pin_pulseA);
	AFConfig(TIM8,GPIOx_pulseB, GPIO_Pin_pulseB);

	BIOS_QEI6_TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
	BIOS_QEI6_TIM_TimeBaseStructure.TIM_Prescaler = 0;
	BIOS_QEI6_TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	BIOS_QEI6_TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM8, &BIOS_QEI6_TIM_TimeBaseStructure);
	TIM_EncoderInterfaceConfig(TIM8, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_ARRPreloadConfig(TIM8, ENABLE);

	TIM_ClearITPendingBit(TIM8, TIM_IT_Update );
	TIM_ITConfig(TIM8, TIM_IT_Update, ENABLE);

	BIOS_QEI6_NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_TIM13_IRQn;
	BIOS_QEI6_NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemptionpriority;
	BIOS_QEI6_NVIC_InitStructure.NVIC_IRQChannelSubPriority = subpriority;
	BIOS_QEI6_NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&BIOS_QEI6_NVIC_InitStructure);
	TIM_Cmd(TIM8, ENABLE);
}

#ifdef __cplusplus
}
#endif
