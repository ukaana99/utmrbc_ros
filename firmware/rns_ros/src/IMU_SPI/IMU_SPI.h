/***************************************************************************************
 * Title   : IMU SPI
 * Author  : Kai Sheng
 * Version : 1.00
 * Date    : 4 MAY 2018
 * *************************************************************************************
 * Descriptions:
 *		This library is use to obtain the yaw value from arduino via SPI comunication.
 *
 * Version History:
 *
 * Bugs:
 *
 ****************************************************************************************/


#ifndef IMU_SPI_H_
#define IMU_SPI_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************************
 * 		Include Libraries 			   *
 **************************************/

#include "BIOS/bios.h"

/***************************************
 * 		DEFINES						   *
 ***************************************/

#define SPI_FLAG_TIMEOUT	((uint32_t)0x1000)

/***************************************
 * 		STRUCTURE					   *
 ***************************************/

typedef struct{

	SPI_TypeDef* SPIx;

	GPIO_TypeDef* GPIOx_NSS;
	uint16_t GPIO_Pin_NSS;

	union{
		float temp_imudata;
		struct{
			uint8_t byte1;
			uint8_t byte2;
			uint8_t byte3;
			uint8_t byte4;
		};
	};

	FunctionalState read_enable;
	uint8_t imu_count;
	float* yaw;

}IMU_SPI_t;

/**************************************************
 * 		Function Prototype			  			  *
 *************************************************/

void IMU_SPI_Init(IMU_SPI_t* IMU, float* yaw, SPI_TypeDef* SPIx,
		  GPIO_TypeDef* GPIOx_NSS, uint16_t GPIO_Pin_NSS,
		  GPIO_TypeDef* GPIOx_SCK, uint16_t GPIO_Pin_SCK,
		  GPIO_TypeDef* GPIOx_MISO, uint16_t GPIO_Pin_MISO,
		  GPIO_TypeDef* GPIOx_MOSI, uint16_t GPIO_Pin_MOSI);

void IMUEnquire(IMU_SPI_t* IMU);
void IMUReadCmd(IMU_SPI_t* IMU, FunctionalState state);


uint8_t SPITransmit(SPI_TypeDef* SPIx, uint8_t data);

#ifdef __cplusplus
}
#endif

#endif /* IMU_SPI_H_ */
