
#include "shiftreg.h"

/************************************************/
/*		 	 	Functions		       		  	*/
/************************************************/

/*
 * Function Name		: SHIFTREGInit
 * Function Description : Configure shift register pins.
 * Function Remarks		: GPIO Mapping for shift register can be set
 * Function Arguments	: shiftreg			pointer to structure
 * 						  cascade			Enumeration of shift register type as below:
 *											NO_CASCADE	Using 1 shift register
 *											CASCADE_1	Using 2 shift register
 *											CASCADE_2	Using 3 shift register
 *											CASCADE_3	Using 4 shift register
 *											CASCADE_4	Using 5 shift register
 *											CASCADE_5	Using 6 shift register
 * 						  GPIOx_sck			GPIOx group of shift register sck pin(x = A,B,C,D or E)
 * 						  GPIO_Pin_sck 		GPIO_Pin_x of shift register sck pin(x = 0,1,2,...or 15)
 * 						  GPIOx_rck			GPIOx group of shift register rck pin(x = A,B,C,D or E)
 * 						  GPIO_Pin_rck		GPIO_Pin_x of shift register rck pin(x = 0,1,2,...or 15)
 * 						  GPIOx_si			GPIOx group of shift register si pin(x = A,B,C,D or E)
 * 						  GPIO_Pin_si		GPIO_Pin_x of shift register si pin(x = 0,1,2,...or 15)
 * Function Return		: None
 * Function Example		: SHIFTREGInit(&shiftreg, CASCADE_1, GPIOD, GPIO_Pin_0, GPIOD, GPIO_Pin_1, GPIOD, GPIO_Pin_2);
 */

void SHIFTREGInit (shiftreg_t* shiftreg, fSR cascade, GPIO_TypeDef *GPIOx_sck, uint16_t GPIO_Pin_sck,
		           GPIO_TypeDef *GPIOx_rck , uint16_t GPIO_Pin_rck,GPIO_TypeDef *GPIOx_si, uint16_t GPIO_Pin_si){

	shiftreg->flag = cascade;

	shiftreg->GPIOx_sck = GPIOx_sck;
	shiftreg->GPIO_Pin_sck = GPIO_Pin_sck;

	shiftreg->GPIOx_rck = GPIOx_rck;
	shiftreg->GPIO_Pin_rck = GPIO_Pin_rck;

	shiftreg->GPIOx_si = GPIOx_si;
	shiftreg->GPIO_Pin_si = GPIO_Pin_si;

	GPIOPinsInit(shiftreg->GPIOx_sck, shiftreg->GPIO_Pin_sck, GPIO_Mode_OUT,GPIO_OType_PP,GPIO_Speed_50MHz, GPIO_PuPd_UP);
	GPIOPinsInit(shiftreg->GPIOx_rck, shiftreg->GPIO_Pin_rck, GPIO_Mode_OUT,GPIO_OType_PP,GPIO_Speed_50MHz, GPIO_PuPd_UP);
	GPIOPinsInit(shiftreg->GPIOx_si, shiftreg->GPIO_Pin_si , GPIO_Mode_OUT,GPIO_OType_PP,GPIO_Speed_50MHz, GPIO_PuPd_UP);

}

/*
 * Function Name		: SHIFTREGShift
 * Function Description : Shift the data to the output
 * Function Remarks		: None
 * Function Arguments	: shiftreg			pointer to structure
 * Function Return		: None
 * Function Example		: SHIFTREGShift (&shiftreg);
 */

void SHIFTREGShift(shiftreg_t* shiftreg){

	uint8_t out, i, j;

	i = shiftreg->flag;

	GPIO_WriteBit(shiftreg->GPIOx_sck , shiftreg->GPIO_Pin_sck, Bit_RESET);
	GPIO_WriteBit(shiftreg->GPIOx_rck , shiftreg->GPIO_Pin_rck, Bit_RESET);

	do{
		out = shiftreg->cast[i].Byte;
		for (j = 8; j; j--){
			(out & 0x80) ? GPIO_WriteBit(shiftreg->GPIOx_si , shiftreg->GPIO_Pin_si, Bit_SET) : GPIO_WriteBit(shiftreg->GPIOx_si , shiftreg->GPIO_Pin_si, Bit_RESET);
			out <<= 1;
			GPIO_WriteBit(shiftreg->GPIOx_sck , shiftreg->GPIO_Pin_sck, Bit_SET);
			GPIO_WriteBit(shiftreg->GPIOx_sck , shiftreg->GPIO_Pin_sck, Bit_RESET);
		}
	}while(i--);

	GPIO_WriteBit(shiftreg->GPIOx_rck , shiftreg->GPIO_Pin_rck, Bit_SET);
	GPIO_WriteBit(shiftreg->GPIOx_rck , shiftreg->GPIO_Pin_rck, Bit_RESET);

}

void WriteSR(shiftreg_t* shiftreg, int32_t hspm, bool signal, uint8_t byte)
{
//	int index = (hspm > 8) ? 0 : 1;
//
//	if(signal)
//		shiftreg->cast[index].Byte |= byte;
//	else
//		shiftreg->cast[index].Byte &= (uint8_t)~((uint8_t)byte);

	if(hspm >= 1 && hspm <= 8)
	{
		if(signal)
			shiftreg->cast[1].Byte |= byte;
		else
			shiftreg->cast[1].Byte &= (uint8_t)~((uint8_t)byte);
	}
	else if(hspm >= 9 && hspm <= 16)
	{
		if(signal)
			shiftreg->cast[0].Byte |= byte;
		else
			shiftreg->cast[0].Byte &= (uint8_t)~((uint8_t)byte);
	}

}
