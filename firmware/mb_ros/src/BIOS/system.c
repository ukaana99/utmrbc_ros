
#include "system.h"

/*
 * Function Name		: BIOSInit
 * Function Description : This function is called to initialize the mcu and configure the SYSTICK. It also responsible to
 * 						  initialize the system tick timer its interrupt and start the system tick timer.
 * Function Remarks		: This function will configure the SYSTICK counter clock source to be Core Clock Source (HCLK)
 * 						  which is 168MHz.
 * Function Arguments	: None
 * Function Return		: None
 * Function Example		: BIOSInit();
 */

void BIOSInit(void)
{
	SystemInit();
	SystemCoreClockUpdate();
	RCC_GetClocksFreq(&BIOS_RCC_Clocks);
	SysTick_Config(BIOS_RCC_Clocks.HCLK_Frequency / 1000);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

}

/*
 * Function Name		: BIOSDelay
 * Function Description : Delay using internal timer.
 * Function Remarks		: The SYSTICK_IRQ runs at a priority level of 6, thus avoid calling function with priority 6 or more as
 * 						  it will cause STM32F4 to hang.
 * Function Arguments	: nTime				  time length of delay in milliseconds
 * Function Return		: None
 * Function Example		: BIOSDelay (1000);   //Delay for 1s
 */

void BIOSDelay (__IO uint32_t nTime)
{
	BIOSTimingDelay = nTime;
	while(BIOSTimingDelay != 0);
}

/*
 * Function Name		: BIOSDelayLoop
 * Function Description : Delay using instruction loop.
 * Function Remarks		: This function is independent of priority level, thus does not hang in interrupt. The drawback is the delay
 * 						  is not accurate. For instance, BIOSDelayLoop(16800000) produces a delay of nearly 1 second.
 * Function Arguments	: nTime			time length of delay
 * Function Return		: None
 * Function Example		: BIOSDelayLoop (16800000);   //Delay for 1s
 */

void BIOSDelayLoop (__IO uint32_t nTime)
{
	while(nTime--)
	{

	}
}

/*
 * Function Name		: BIOSTimingDelay_Decrement
 * Function Description : Handle the decrement of BIOSTimingDelay variable.
 * Function Remarks		: None
 * Function Arguments	: None
 * Function Return		: None
 * Function Example		: BIOSTimingDelay_Decrement();
 */
void BIOSTimingDelay_Decrement(void)
{
  if (BIOSTimingDelay != 0x00)
  {
    BIOSTimingDelay--;
  }
}

/*
 * Function Name		: BIOSFPUEnable
 * Function Description : This function is called to enable FPU.
 * Function Remarks		: None
 * Function Arguments	: None
 * Function Return		: None
 * Function Example		: BIOSFPUEnable();
 */

void BIOSFPUEnable(void)
{
	SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));
}

