#ifndef BIOS_MULTIPLEXER_H_
#define BIOS_MULTIPLEXER_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************************
 * 		Include Libraries 			   *
 **************************************/
#include "gpio.h"

/**************************************************
 * 		Structure							  	  *
 *************************************************/
typedef struct{

	GPIO_TypeDef *GPIOx_Input;
	uint16_t GPIO_Pin_Input;

	GPIO_TypeDef *GPIOx_S0;
	uint16_t GPIO_Pin_S0;

	GPIO_TypeDef *GPIOx_S1;
	uint16_t GPIO_Pin_S1;

	GPIO_TypeDef *GPIOx_S2;
	uint16_t GPIO_Pin_S2;

	byte_t mux_data;

	union{
		uint8_t select;
		struct{
			uint8_t s0:1;
			uint8_t s1:1;
			uint8_t s2:1;
		};
	};

}MUX_t;
/**************************************************
 * 		Function Prototype			  			  *
 *************************************************/
void MUXInit(MUX_t* mux,GPIO_TypeDef *GPIOx_Input,uint16_t GPIO_Pin_Input,GPIO_TypeDef *GPIOx_S0,
		uint16_t GPIO_Pin_S0,GPIO_TypeDef *GPIOx_S1,uint16_t GPIO_Pin_S1,GPIO_TypeDef *GPIOx_S2,
		uint16_t GPIO_Pin_S2);
uint8_t MUXSelect(MUX_t* mux, uint8_t select);
void MUXUpdate(MUX_t* mux);

#ifdef __cplusplus
}
#endif

#endif /* BIOS_MULTIPLEXER_H_ */
