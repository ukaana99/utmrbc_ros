#include "include.h"

/********************************************/
/*					QEIS					*/
/********************************************/

//void SysTick_Handler(void)
//{
//}

void TIM1_UP_TIM10_IRQHandler(void){

	if (TIM1 -> CR1 == 129)
		BIOS_QEI1.signbit += 1;
	else if (TIM1 ->CR1 == 145)
		BIOS_QEI1.signbit -= 1;

	TIM1 -> SR = 0;
	QEIDelay(200);


}

void TIM2_IRQHandler(void){


	if (TIM2 -> CR1 == 129)
		BIOS_QEI2.signbit += 1;
	else if (TIM2 ->CR1 == 145)
		BIOS_QEI2.signbit -= 1;
	TIM2 -> SR = 0;
	QEIDelay(100);



}

void TIM3_IRQHandler(void){


	if (TIM3 -> CR1 == 129)
		BIOS_QEI3.signbit += 1;
	else if (TIM3 ->CR1 == 145)
		BIOS_QEI3.signbit -= 1;

	TIM3 -> SR = 0;
	QEIDelay(100);


}

void TIM4_IRQHandler(void){


	if (TIM4 -> CR1 == 129)
		BIOS_QEI4.signbit += 1;
	else if (TIM4 ->CR1 == 145)
		BIOS_QEI4.signbit -= 1;

	TIM4 -> SR = 0;
	QEIDelay(100);


}

void TIM5_IRQHandler(void){


	if (TIM5 -> CR1 == 129)
		BIOS_QEI5.signbit += 1;
	else if (TIM5 ->CR1 == 145)
		BIOS_QEI5.signbit -= 1;

	TIM5 -> SR = 0;
	QEIDelay(100);

}

void TIM8_UP_TIM13_IRQHandler(void){


	if (TIM8 -> CR1 == 129)
		BIOS_QEI6.signbit += 1;
	else if (TIM8 ->CR1 == 145)
		BIOS_QEI6.signbit -= 1;

	TIM8 -> SR = 0;
	QEIDelay(200);


}

/********************************************/
/*			TIMERS INTERRUPT				*/
/********************************************/


void TIM7_IRQHandler(void){

	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET){
		SHIFTREGShift(&SR);
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
	}

	return;
}




void TIM8_TRG_COM_TIM14_IRQHandler(void){


	if (TIM_GetITStatus(TIM14, TIM_IT_Update) != RESET){
	  TIM_ClearITPendingBit(TIM14, TIM_IT_Update);

	}

	return;
}

void UART5_IRQHandler(void)
{
	if(USART_GetITStatus(UART5,USART_IT_RXNE)!=RESET){
		rcvdata = ReadUART(UART5);
		if(rcvdata == '\n'){
			for(int i = u5rx_count; i < 25; i++)
				c[i] = '\0';
			u5rx_count = 0;
			sscanf(c, "%u%d%u", &header, &yaw, &lastbyte);
			checksum = header + yaw + lastbyte;
			if(!(checksum & 0xFF)){
				fyaw = yaw/100.0+180.0;
				LED3=!LED3;
			}
		}else if(u5rx_count > 25){
			u5rx_count = 0;
		}else{
			c[u5rx_count++] = rcvdata;
		}
		USART_ClearITPendingBit(UART5,USART_IT_RXNE);
	} else {
		USART_ClearITPendingBit(UART5,USART_IT_RXNE);
	}
}
