/************************************************
 * Title   : I2C
 * Author  : Kai Sheng and Qiu Hui
 * Version : 1.30
 * Date    : 13 JULY 2017
 * **********************************************
 * Descriptions:
 *
 *
 * Version History:
 * V1.30: I2C master transmit via interrupt is
 * 		  added
 * V1.20: Improve error handling of I2C(Edited by
 * 		  Yew Ji Hui 25/1/2018)
 * V1.10: I2CxMasterread function is added to read
 * 		  data without using interrupt( Edited by
 * 		  Yew Ji Hui 21/1/2018)
 *
 * Bugs:
 *
 ************************************************/

#ifndef I2C_I2C_H_
#define I2C_I2C_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************************
 * 		Include Libraries 			   *
 **************************************/

#include "BIOS/bios.h"
//#include "uCOS-ii/Source/ucos_ii.h"
//#include "uCOS-ii/Port/cpu.h"


/**************************************************
 * 		STRUCTURE DEFINES					  	  *
 *************************************************/

I2C_InitTypeDef  				I2C1_InitStructure;
GPIO_InitTypeDef				I2C1_GPIO_InitStructure;
NVIC_InitTypeDef				I2C1_NVIC_InitStructure;

I2C_InitTypeDef  				I2C2_InitStructure;
GPIO_InitTypeDef				I2C2_GPIO_InitStructure;
NVIC_InitTypeDef				I2C2_NVIC_InitStructure;

I2C_InitTypeDef  				I2C3_InitStructure;
GPIO_InitTypeDef				I2C3_GPIO_InitStructure;
NVIC_InitTypeDef				I2C3_NVIC_InitStructure;

/**************************************************
 * 		DEFINES								  	  *
 *************************************************/

#define CLOCK_SPEED_100KHz	100000
#define CLOCK_SPEED_400KHz	400000

#define main_board_1						8
#define main_board_2						9
#define main_board_3						10
#define main_board_4						11
#define motor_control_module_1				16
#define motor_control_module_2				17
#define motor_control_module_3				18
#define motor_control_module_4				19
#define robot_navi_system					30

#define I2C_SHORT_TIMEOUT            		((uint32_t)0x10000)
#define I2C_LONG_TIMEOUT             		((uint32_t)(350000 * I2C_SHORT_TIMEOUT))

#define I2C1ReInit()	I2C_DeInit(I2C1);\
						RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);\
						RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);\
						I2C_Init(I2C1, &I2C1_InitStructure);\
						I2C_ITConfig(I2C1, I2C_IT_BUF|I2C_IT_EVT|I2C_IT_ERR, ENABLE);\

#define I2C2ReInit()	I2C_DeInit(I2C2);\
						RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);\
						RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);\
						I2C_Init(I2C2, &I2C2_InitStructure);\
						I2C_ITConfig(I2C2, I2C_IT_BUF|I2C_IT_EVT|I2C_IT_ERR, ENABLE);\

#define I2C3ReInit()	I2C_DeInit(I2C3);\
						RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C3, ENABLE);\
						RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);\
						RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);\
						I2C_Init(I2C3, &I2C3_InitStructure);\
						I2C_ITConfig(I2C3, I2C_IT_BUF|I2C_IT_EVT|I2C_IT_ERR, ENABLE);\


/************************************************/
/*		 	 	Typedef	       		  			*/
/************************************************/

typedef void (*i2c_rcv_handler)(uint8_t length, const uint8_t *pdata);
typedef void (*i2c_trans_handler)(uint8_t *length, uint8_t *pdata);

i2c_rcv_handler srcvhandlerI2C1,srcvhandlerI2C2,srcvhandlerI2C3;
i2c_rcv_handler mrcvhandlerI2C1,mrcvhandlerI2C2,mrcvhandlerI2C3;
i2c_trans_handler stranshandlerI2C1, stranshandlerI2C2, stranshandlerI2C3;

/**************************************************
 * 		Structure							  	  *
 *************************************************/


/**************************************************
 * 		Enumerator							  	  *
 *************************************************/

/**************************************************
 * 		variables							  	  *
 *************************************************/
uint8_t I2C1rxlen, I2C1txlen, I2C1slaveadress, I2C1_direction;
uint8_t I2C2rxlen, I2C2txlen, I2C2slaveadress, I2C2_direction;
uint8_t I2C3rxlen, I2C3txlen, I2C3slaveadress, I2C3_direction;

uint8_t I2C1_Master_Flag, I2C2_Master_Flag, I2C3_Master_Flag;

uint8_t I2C1_master_snd_buf[256], I2C1_master_rcv_buf[256], I2C1_slave_snd_buf[256], I2C1_slave_rcv_buf[256], I2C1_Txlength, I2C1_Rxlength;
uint8_t I2C2_master_snd_buf[256], I2C2_master_rcv_buf[256], I2C2_slave_snd_buf[256], I2C2_slave_rcv_buf[256], I2C2_Txlength, I2C2_Rxlength;
uint8_t I2C3_master_snd_buf[256], I2C3_master_rcv_buf[256], I2C3_slave_snd_buf[256], I2C3_slave_rcv_buf[256], I2C3_Txlength, I2C3_Rxlength;

uint16_t i2c1ErrorCount;
uint16_t i2c2ErrorCount;
uint16_t i2c3ErrorCount;

/**************************************************
 * 		Function Prototype			  			  *
 *************************************************/
/*MASTER MODE*/

bool I2CMasterSendI(I2C_TypeDef *I2Cx,uint8_t slave_addr, uint8_t len, uint8_t* data);  // without interrupt
bool I2CMasterReadI(I2C_TypeDef *I2Cx,uint8_t slave_addr, uint8_t len, uint8_t* data); 	// without interrupt

bool I2CMasterSendII(I2C_TypeDef *I2Cx,uint8_t slave_addr, uint8_t len, uint8_t* data); // with interrupt
bool I2CMasterReadII(I2C_TypeDef *I2Cx,uint8_t slave_addr, uint8_t len);				// with interrupt

/*ROBOCON UTM I2C PROTOCOL*/
bool I2CSend(I2C_TypeDef *I2Cx, uint32_t slave_addr, uint8_t len, const void *data);
bool I2CSendV(I2C_TypeDef *I2Cx, uint8_t slave_addr, uint8_t len, ...);

/*I2C PERIPHERAL INITIALIZE*/
void I2C1Init (uint8_t OwnAdress,uint32_t ClockSpeed,i2c_rcv_handler rh,i2c_rcv_handler mh, i2c_trans_handler st);
void I2C2Init (uint8_t OwnAdress,uint32_t ClockSpeed,i2c_rcv_handler rh,i2c_rcv_handler mh, i2c_trans_handler st);
void I2C3Init (uint8_t OwnAdress,uint32_t ClockSpeed,i2c_rcv_handler rh,i2c_rcv_handler mh, i2c_trans_handler st);

/*TIMEOUT AND DEBUG*/
uint32_t I2CTimeoutCallback(I2C_TypeDef *I2Cx);
uint16_t I2CGetErrorCounter(I2C_TypeDef *I2Cx);

#ifdef __cplusplus
}
#endif

#endif /* I2C_I2C_H_ */
