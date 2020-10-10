#include "interrupt.h"

#define USED_QEI1
//#define USED_QEI2
//#define USED_QEI3
#define USED_QEI4
//#define USED_QEI5
#define USED_QEI6

int count = 0;
int count2 = 0;
int count3 = 0;
char uartbuff[200];
void SysTick_Handler(void){

	BIOSTimingDelay_Decrement();
	MUXUpdate(&MUX);
	SHIFTREGShift(&SR);

}

void TIM1_UP_TIM10_IRQHandler(void)
{
#ifdef USED_QEI1
	if (TIM1 -> CR1 == 129)
		{
			BIOS_QEI1.signbit += 1;
		}
	else if (TIM1 ->CR1 == 145)
	{
		BIOS_QEI1.signbit -= 1;
	}
	TIM1 -> SR = 0;
	QEIDelay(200);
#else
  if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
	   {
		  TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	   }
#endif
  if (TIM_GetITStatus(TIM10, TIM_IT_Update) != RESET)
	   {
		  TIM_ClearITPendingBit(TIM10, TIM_IT_Update);

	   }
   return;
}


void TIM2_IRQHandler(void)
{
#ifdef USED_QEI2
	if (TIM2 -> CR1 == 129)
	{
		BIOS_QEI2.signbit += 1;
	}
	else if (TIM2 ->CR1 == 145)
	{
		BIOS_QEI2.signbit -= 1;
	}
	TIM2 -> SR = 0;
	QEIDelay(100);
#else
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	   {
		  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	   }


   return;
#endif
}


void TIM3_IRQHandler(void)
{
#ifdef USED_QEI3
	if (TIM3 -> CR1 == 129)
	{
		BIOS_QEI3.signbit += 1;
	}
	else if (TIM3 ->CR1 == 145)
	{
		BIOS_QEI3.signbit -= 1;
	}
	TIM3 -> SR = 0;
	QEIDelay(100);
#else
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	   {
		  TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

	   }
   return;
#endif
}

void TIM4_IRQHandler(void)
{
#ifdef USED_QEI4
  if (TIM4 -> CR1 == 129)
  {
		BIOS_QEI4.signbit += 1;
  }
  else if (TIM4 ->CR1 == 145)
  {
		BIOS_QEI4.signbit -= 1;
  }
  TIM4 -> SR = 0;
  QEIDelay(100);
#else
  if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	   {
	  	  TIM_ClearITPendingBit(TIM4, TIM_IT_Update);

	   }
   return;
#endif
}

void TIM5_IRQHandler(void)
{
#ifdef USED_QEI5
   if (TIM5 -> CR1 == 129)
	  {
			BIOS_QEI5.signbit += 1;
	  }
	  else if (TIM5 ->CR1 == 145)
	  {
			BIOS_QEI5.signbit -= 1;
	  }
	  TIM5 -> SR = 0;
	  QEIDelay(100);
#else
  if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
	   {
		  TIM_ClearITPendingBit(TIM5, TIM_IT_Update);

	   }
   return;
#endif
}



void TIM7_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
		{
			TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
		}
   return;
}

void TIM8_UP_TIM13_IRQHandler(void)
{
#ifdef USED_QEI6
	if (TIM8 -> CR1 == 129)
		{
			BIOS_QEI6.signbit += 1;
		}
	else if (TIM8 ->CR1 == 145)
	{
		BIOS_QEI6.signbit -= 1;
	}
	TIM8 -> SR = 0;
	QEIDelay(200);
#else
  if (TIM_GetITStatus(TIM8, TIM_IT_Update) != RESET)
		{
		  TIM_ClearITPendingBit(TIM8, TIM_IT_Update);

		}
#endif


	if (TIM_GetITStatus(TIM13, TIM_IT_Update) != RESET)
		{
		  TIM_ClearITPendingBit(TIM13, TIM_IT_Update);
		}
return;

}

void TIM1_BRK_TIM9_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM9, TIM_IT_Update) != RESET)
	   {
		  TIM_ClearITPendingBit(TIM9, TIM_IT_Update);

	   }
	   return;
}


void TIM1_TRG_COM_TIM11_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM11, TIM_IT_Update) != RESET)
	   {
		  TIM_ClearITPendingBit(TIM11, TIM_IT_Update);

	   }
	return;
}

void TIM8_BRK_TIM12_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM12, TIM_IT_Update) != RESET)
	   {
		  TIM_ClearITPendingBit(TIM12, TIM_IT_Update);

	   }
	return;
}

void TIM8_TRG_COM_TIM14_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM14, TIM_IT_Update) != RESET)
	   {
		  TIM_ClearITPendingBit(TIM14, TIM_IT_Update);

	   }
    return;
}


void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line15) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line15);
	}
}
