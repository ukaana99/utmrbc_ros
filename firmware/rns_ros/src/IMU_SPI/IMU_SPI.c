
/************************************************/
/*		 	 	Include Header	       		  	*/
/************************************************/

#include "IMU_SPI.h"

/************************************************/
/*		 	 	Functions		       		  	*/
/************************************************/

/*
 * Function Name		: IMU_SPI_Init
 * Function Description : This function is called to initialize the SPI peripheral and IMU
 * Function Remarks		: None
 * Function Arguments	: IMU				pointer to structure IMU_SPI_t
 *						  yaw				pointer to variable with data type float for storing the yaw data
 *						  SPIx				select SPI peripheral (SPI1 or SPI2 or SPI3)
 *						  GPIOx_NSS			GPIOx group of NSS pin(x = A,B,C,D or E)
 * 						  GPIO_Pin_NSS		GPIO_Pin_x of NSS pin(x = 0,1,2,...or 15)
 * 						  GPIOx_SCK			GPIOx group of SCK pin(x = A,B,C,D or E)
 * 						  GPIO_Pin_SCK		GPIO_Pin_x of SCK pin(x = 0,1,2,...or 15)
 * 						  GPIOx_MISO		GPIOx group of MISO pin(x = A,B,C,D or E)
 * 						  GPIO_Pin_MISO		GPIO_Pin_x of MISO pin(x = 0,1,2,...or 15)
 * 						  GPIOx_MOSI		GPIOx group of MOSI pin(x = A,B,C,D or E)
 * 						  GPIO_Pin_MOSI		GPIO_Pin_x of MOSI pin(x = 0,1,2,...or 15)
 * Function Return		: None
 * Function Example		: IMU_SPI_Init(&IMU, &yaw, SPI2, GPIOB , GPIO_Pin_12, GPIOB , GPIO_Pin_13,
 * 						  				GPIOC , GPIO_Pin_2, GPIOC , GPIO_Pin_3);
 *
 */
void IMU_SPI_Init(IMU_SPI_t* IMU, float* yaw, SPI_TypeDef* SPIx,
		  GPIO_TypeDef* GPIOx_NSS, uint16_t GPIO_Pin_NSS,
		  GPIO_TypeDef* GPIOx_SCK, uint16_t GPIO_Pin_SCK,
		  GPIO_TypeDef* GPIOx_MISO, uint16_t GPIO_Pin_MISO,
		  GPIO_TypeDef* GPIOx_MOSI, uint16_t GPIO_Pin_MOSI){

	IMU->SPIx = SPIx;
	IMU->GPIOx_NSS = GPIOx_NSS;
	IMU->GPIO_Pin_NSS = GPIO_Pin_NSS;
	IMU->yaw = yaw;

	GPIOPinsInit (GPIOx_SCK, GPIO_Pin_SCK, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_50MHz);
	GPIOPinsInit (GPIOx_MOSI, GPIO_Pin_MOSI,GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_DOWN, GPIO_Speed_50MHz);
	GPIOPinsInit (GPIOx_MISO, GPIO_Pin_MISO, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_DOWN, GPIO_Speed_50MHz);
	GPIOPinsInit (GPIOx_NSS, GPIO_Pin_NSS, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_50MHz);

	switch((uint32_t)SPIx){

		case SPI1_BASE: RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
						AFConfig(SPI1, GPIOx_SCK, GPIO_Pin_SCK);
						AFConfig(SPI1, GPIOx_MOSI,GPIO_Pin_MOSI);
						AFConfig(SPI1, GPIOx_MISO,GPIO_Pin_MISO);
						break;

		case SPI2_BASE:	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
						AFConfig(SPI2, GPIOx_SCK, GPIO_Pin_SCK);
						AFConfig(SPI2, GPIOx_MOSI,GPIO_Pin_MOSI);
						AFConfig(SPI2, GPIOx_MISO,GPIO_Pin_MISO);
						break;

		case SPI3_BASE: RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
						AFConfig(SPI3, GPIOx_SCK, GPIO_Pin_SCK);
						AFConfig(SPI3, GPIOx_MOSI,GPIO_Pin_MOSI);
						AFConfig(SPI3, GPIOx_MISO,GPIO_Pin_MISO);
						break;
	}

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPIx, &SPI_InitStructure);
	SPI_NSSInternalSoftwareConfig(SPIx, SPI_NSSInternalSoft_Set);
	SPI_Cmd(SPIx, ENABLE);

	IMU->read_enable = ENABLE;
}

/*
 * Function Name		: IMUEnquire
 * Function Description : This function is called to update the yaw value
 * Function Remarks		: This function should be called for every 1 ms. The yaw value will be updated for every 4 ms.
 * Function Arguments	: IMU			pointer to structure IMU_SPI_t
 * Function Return		: None
 * Function Example		: IMUEnquire(&IMU);
 */
void IMUEnquire(IMU_SPI_t* IMU){

	if(IMU->read_enable == ENABLE){
		IMU->imu_count++;

		switch(IMU->imu_count){

			case 1:		GPIO_WriteBit(IMU->GPIOx_NSS, IMU->GPIO_Pin_NSS, 0);
						SPITransmit(IMU->SPIx, 'a');
						break;

			case 2:		IMU->byte1 = SPITransmit(IMU->SPIx, 'b');
						break;

			case 3:		IMU->byte2 = SPITransmit(IMU->SPIx, 'c');
						break;

			case 4:		IMU->byte3 = SPITransmit(IMU->SPIx, 'd');
						break;

			case 5:		IMU->byte4 = SPITransmit(IMU->SPIx, 0);
						GPIO_WriteBit(IMU->GPIOx_NSS, IMU->GPIO_Pin_NSS, 1);
						*(IMU->yaw) = IMU->temp_imudata;
						IMU->imu_count = 0;
						break;

			default:	break;
		}
	}
}

/*
 * Function Name		: IMUReadCmd
 * Function Description : This function is called to enable or disable the update of yaw value
 * Function Remarks		: None
 * Function Arguments	: IMU			pointer to structure IMU_SPI_t
 *						  state			ENABLE or DISABLE
 * Function Return		: None
 * Function Example		: IMUReadCmd(&IMU, DISABLE);
 *
 */
void IMUReadCmd(IMU_SPI_t* IMU, FunctionalState state){

	IMU->read_enable = state;
}

/*
 * Function Name		: SPITransmit
 * Function Description : This function is called to handle the SPI data transmission and reception.
 * Function Remarks		: This function is not for the use of user.
 * Function Arguments	: SPIx			select SPI peripheral (SPI1 or SPI2 or SPI3)
 *						  data			data to be sent
 * Function Return		: None
 * Function Example		: None
 *
 */

uint8_t SPITransmit(SPI_TypeDef* SPIx, uint8_t data){

	uint32_t SPITimeout;

	SPITimeout = SPI_FLAG_TIMEOUT;
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET)
	{
		if((SPITimeout--) == 0)
			return false;
	}

	SPIx->DR = data;

	SPITimeout = SPI_FLAG_TIMEOUT;
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET)
	{
		if((SPITimeout--) == 0)
			return false;
	}

	return SPIx->DR;

}

