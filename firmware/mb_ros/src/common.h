
#ifndef COMMON_H_
#define COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "adapter.h"


#define IP1  			GPIO_ReadInputDataBit(IP1_PIN)
#define IP2  			GPIO_ReadInputDataBit(IP2_PIN)
#define IP3  			GPIO_ReadInputDataBit(IP3_PIN)
#define IP4  			GPIO_ReadInputDataBit(IP4_PIN)
#define IP5  			GPIO_ReadInputDataBit(IP5_PIN)
#define IP6 			GPIO_ReadInputDataBit(IP6_PIN)
#define IP7  			GPIO_ReadInputDataBit(IP7_PIN)
#define IP8  			GPIO_ReadInputDataBit(IP8_PIN)
#define IP9  			GPIO_ReadInputDataBit(IP9_PIN)
#define IP10  			GPIO_ReadInputDataBit(IP10_PIN)
#define IP11  			GPIO_ReadInputDataBit(IP11_PIN)
#define IP12  			GPIO_ReadInputDataBit(IP12_PIN)
#define IP13  			GPIO_ReadInputDataBit(IP13_PIN)
#define IP14  			GPIO_ReadInputDataBit(IP14_PIN)
#define IP15			GPIO_ReadInputDataBit(IP15_PIN)

#define hspm1s2			SR.cast[1].bit7
#define hspm1s1			SR.cast[1].bit6
#define hspm2s2			SR.cast[1].bit5
#define hspm2s1			SR.cast[1].bit4
#define hspm3s2			SR.cast[1].bit3
#define hspm3s1			SR.cast[1].bit2
#define hspm4s2			SR.cast[1].bit1
#define hspm4s1			SR.cast[1].bit0

#define hspm5s2			SR.cast[0].bit7
#define hspm5s1			SR.cast[0].bit6
#define hspm6s2			SR.cast[0].bit5
#define hspm6s1			SR.cast[0].bit4
#define hspm7s2			SR.cast[0].bit3
#define hspm7s1			SR.cast[0].bit2
#define hspm8s2			SR.cast[0].bit1
#define hspm8s1			SR.cast[0].bit0

#define hspm7s1_On()	    	hspm7s1=1
#define hspm7s1_Off()	 		hspm7s1=0

//ANALOG PIN//
#define IP16	GPIO_ReadInputDataBit(IP16_Analog1_PIN)
#define IP17	GPIO_ReadInputDataBit(IP17_Analog2_PIN)
#define IP18	GPIO_ReadInputDataBit(IP18_Analog3_PIN)
#define IP19	GPIO_ReadInputDataBit(IP19_Analog4_PIN)
#define IP20	GPIO_ReadInputDataBit(IP20_Analog7_PIN)
#define IP21	GPIO_ReadInputDataBit(IP21_Analog8_PIN)


#define Mux1		 MUX.mux_data.bit0
#define Mux2		 MUX.mux_data.bit1
#define Mux3		 MUX.mux_data.bit2
#define Mux4		 MUX.mux_data.bit3
#define Mux5		 MUX.mux_data.bit4
#define Mux6		 MUX.mux_data.bit5
#define Mux7		 MUX.mux_data.bit6
#define Mux8		 MUX.mux_data.bit7

#define PB1 		GPIOB_IN->bit7
#define PB2 		GPIOE_IN->bit0

#define led1		GPIOC_OUT->bit13
#define led2		GPIOC_OUT->bit14
#define led3		GPIOC_OUT->bit15





typedef union{
	uint16_t flags;
	struct{
		unsigned flag1		:1;
		unsigned flag2 		:1;
		unsigned flag3		:1;
		unsigned flag4		:1;
		unsigned flag5		:1;
		unsigned flag6		:1;
		unsigned flag7 		:1;
		unsigned flag8		:1;
		unsigned flag9		:1;
		unsigned flag10		:1;
		unsigned flag11		:1;
		unsigned flag12		:1;
		unsigned flag13		:1;
		unsigned flag14		:1;
		unsigned flag15		:1;
		unsigned flag16		:1;
	};
}sys_t;

sys_t sys;


#define PS3Request(a)	I2CMasterReadII(a, 0x44, 10)


void System_Start(void);
void System_Run(void);
void RNS_config(CAN_TypeDef* CANx);

#ifdef __cplusplus
}
#endif

#endif /* COMMON_H_ */
