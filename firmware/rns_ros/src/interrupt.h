#ifndef INTERRUPT_H_
#define INTERRUPT_H_
#ifdef __cplusplus
extern "C" {
#endif

/************************************************
 * @Title   : INTERRUPT
 * @Author  : Leonard Chin (Chin Hung Vui)
 * @Version : 1.00
 * @Date    : 26 June 2016
 * **********************************************
 * @Descriptions:
 *
 * - Handling all IRQs of MUC.
 * - Handling data stored by QEI.
 *
 * @Version History:
 *
 *  V1.00:
 *  + V1.00 consists of following IRQs:
 *    a. SysTick_Handler() , call BIOSQEIHandler() in SysTick_Handler to handling QEI Values.
 *    b. TIM1_UP_TIM10_IRQHandler() handling data of QEI1 if #define USED_QEI1
 *    c. TIM2_IRQHandler() handling data of QEI2 if #define USED_QEI2
 *    e. TIM3_IRQHandler() handling data of QEI3 if #define USED_QEI3
 *    f. TIM4_IRQHandler() handling data of QEI4 if #define USED_QEI4
 *    g. TIM5_IRQHandler() handling data of QEI5 if #define USED_QEI5
 *    h. TIM6_DAC_IRQHandler()
 *    i. TIM7_IRQHandler()
 *    j. TIM8_UP_TIM13_IRQHandler()
 *    k. TIM1_BRK_TIM9_IRQHandler()
 *    l. TIM1_TRG_COM_TIM11_IRQHandler()
 *    m. TIM8_BRK_TIM12_IRQHandler()
 *    n. TIM8_TRG_COM_TIM14_IRQHandler()
 *
 * Bugs:
 *
 *	+ Worked Perfectly for V1.00
 ************************************************/


#ifdef __cplusplus
}
#endif
#endif /* INTERRUPT_H_ */
