
#ifndef COMMON_H_
#define COMMON_H_
#ifdef __cplusplus
extern "C" {
#endif

/*********************************************/
/*          Include Header                   */
/*********************************************/

/*********************************************/
/*          Define			                 */
/*********************************************/

#define BUTTON1				GPIOC_IN->bit4
#define BUTTON2				GPIOE_IN->bit10

#define LED1				GPIOA_OUT->bit5
#define LED2				GPIOC_OUT->bit5
#define LED3				GPIOE_OUT->bit7
#define LED4				GPIOE_OUT->bit14
#define LED5				GPIOD_OUT->bit10

#define BUTTON1_PIN			GPIOC, GPIO_Pin_4
#define BUTTON2_PIN			GPIOE, GPIO_Pin_10

#define LED1_PIN			GPIOA, GPIO_Pin_5
#define LED2_PIN			GPIOC, GPIO_Pin_5
#define LED3_PIN			GPIOE, GPIO_Pin_7
#define LED4_PIN			GPIOE, GPIO_Pin_14
#define LED5_PIN			GPIOD, GPIO_Pin_10

#define SR_SCK_PIN			GPIOA, GPIO_Pin_12
#define SR_RCK_PIN			GPIOA, GPIO_Pin_11
#define SR_SI_PIN			GPIOC, GPIO_Pin_8

#define QEI1_PLUSEA_PIN		GPIOE, GPIO_Pin_9
#define QEI1_PLUSEB_PIN		GPIOE, GPIO_Pin_11

#define QEI2_PLUSEA_PIN		GPIOA, GPIO_Pin_15
#define QEI2_PLUSEB_PIN		GPIOB, GPIO_Pin_3

#define QEI3_PLUSEA_PIN		GPIOA, GPIO_Pin_6
#define QEI3_PLUSEB_PIN		GPIOA, GPIO_Pin_7

#define QEI4_PLUSEA_PIN		GPIOD, GPIO_Pin_12
#define QEI4_PLUSEB_PIN		GPIOD, GPIO_Pin_13

#define QEI5_PLUSEA_PIN		GPIOA, GPIO_Pin_0
#define QEI5_PLUSEB_PIN		GPIOA, GPIO_Pin_1

#define QEI6_PLUSEA_PIN		GPIOC, GPIO_Pin_6
#define QEI6_PLUSEB_PIN		GPIOC, GPIO_Pin_7

#define PWM_TIM9_CH1_PIN	GPIOA, GPIO_Pin_2
#define PWM_TIM9_CH2_PIN	GPIOA, GPIO_Pin_3
#define PWM_TIM12_CH1_PIN	GPIOB, GPIO_Pin_14
#define PWM_TIM12_CH2_PIN	GPIOB, GPIO_Pin_15

#define UART2_Tx_PIN		GPIOD, GPIO_Pin_5
#define UART2_Rx_PIN		GPIOD, GPIO_Pin_6

#define UART5_Tx_PIN		GPIOC, GPIO_Pin_12
#define UART5_Rx_PIN		GPIOD, GPIO_Pin_2

#define SPI2_NSS_PIN		GPIOB, GPIO_Pin_12
#define SPI2_SCK_PIN		GPIOB, GPIO_Pin_13
#define SPI2_MISO_PIN		GPIOC, GPIO_Pin_2
#define SPI2_MOSI_PIN		GPIOC, GPIO_Pin_3

#define CAN1_Tx_PIN			GPIOD, GPIO_Pin_1
#define CAN1_Rx_PIN			GPIOD, GPIO_Pin_0

#define CAN2_Tx_PIN			GPIOB, GPIO_Pin_6
#define CAN2_Rx_PIN			GPIOB, GPIO_Pin_5

#define S1_A_PIN			GPIOE, GPIO_Pin_5
#define S2_A_PIN			GPIOE, GPIO_Pin_4
#define S3_A_PIN			GPIOE, GPIO_Pin_3
#define S4_A_PIN			GPIOE, GPIO_Pin_2
#define S5_A_PIN			GPIOE, GPIO_Pin_1
#define S6_A_PIN			GPIOE, GPIO_Pin_0
#define S7_A_PIN			GPIOB, GPIO_Pin_7
#define S8_A_PIN			GPIOD, GPIO_Pin_7

#define S1_B_PIN			GPIOB, GPIO_Pin_0
#define S2_B_PIN			GPIOA, GPIO_Pin_4
#define S3_B_PIN			GPIOC, GPIO_Pin_1
#define S4_B_PIN			GPIOC, GPIO_Pin_0
#define S5_B_PIN			GPIOC, GPIO_Pin_15
#define S6_B_PIN			GPIOC, GPIO_Pin_14
#define S7_B_PIN			GPIOC, GPIO_Pin_13
#define S8_B_PIN			GPIOE, GPIO_Pin_6

#define S1_C_PIN			GPIOD, GPIO_Pin_15
#define S2_C_PIN			GPIOD, GPIO_Pin_14
#define S3_C_PIN			GPIOD, GPIO_Pin_11
#define S4_C_PIN			GPIOE, GPIO_Pin_15
#define S5_C_PIN			GPIOE, GPIO_Pin_13
#define S6_C_PIN			GPIOE, GPIO_Pin_12
#define S7_C_PIN			GPIOE, GPIO_Pin_8
#define S8_C_PIN			GPIOB, GPIO_Pin_1

#define S1_D_PIN			GPIOA, GPIO_Pin_10
#define S2_D_PIN			GPIOD, GPIO_Pin_9
#define S3_D_PIN			GPIOA, GPIO_Pin_9
#define S4_D_PIN			GPIOD, GPIO_Pin_8
#define S5_D_PIN			GPIOB, GPIO_Pin_8
#define S6_D_PIN			GPIOC, GPIO_Pin_10
#define S7_D_PIN			GPIOD, GPIO_Pin_4
#define S8_D_PIN			GPIOC, GPIO_Pin_11

#define D15p				GPIOD, GPIO_Pin_15
#define D14p				GPIOD, GPIO_Pin_14
#define D11p				GPIOD, GPIO_Pin_11
#define E15p				GPIOE, GPIO_Pin_15
#define E13p				GPIOE, GPIO_Pin_13
#define E12p				GPIOE, GPIO_Pin_12
#define E8p					GPIOE, GPIO_Pin_8
#define B1p					GPIOB, GPIO_Pin_1
#define C10p				GPIOC,GPIO_Pin_10

#define D15 			GPIO_ReadInputDataBit(D15p)
#define D14  			GPIO_ReadInputDataBit(D14p)
#define D11  			GPIO_ReadInputDataBit(D11p)
#define E15  			GPIO_ReadInputDataBit(E15p)
#define E13  			GPIO_ReadInputDataBit(E13p)
#define E12 			GPIO_ReadInputDataBit(E12p)
#define E8  			GPIO_ReadInputDataBit(E8p)
#define B1  			GPIO_ReadInputDataBit(B1p)
#define C10  			GPIO_ReadInputDataBit(C10p)

/*********************************************/
/*          Structure                        */
/*********************************************/

typedef struct{
	unsigned char common_instruction;
	union{
		float data;
		struct{
			char byte1;
			char byte2;
			char byte3;
			char byte4;
		};
	}common_buffer[4];
}comm_t;

typedef union{
	float data;
	struct{
		char byte1;
		char byte2;
		char byte3;
		char byte4;
	};
}float_data_t;

/*********************************************/
/*          Enumeration                      */
/*********************************************/

//Enumeration for can StdId
enum {

	RNS_TO_mainboard = 20,
	RNS_TO_mainboard_buf1,
	RNS_TO_mainboard_buf2,
	RNS_TO_mainboard_buf3,
	mainboard_TO_RNS,
	mainboard_TO_RNS_buf1,
	mainboard_TO_RNS_buf2,
	XY_feedback_state,
	CONT_data

};

/*********************************************/
/*          Extern Variable                  */
/*********************************************/


/*********************************************/
/*          Function Prototype               */
/*********************************************/

void Initialize(void);
void wheel_config();
void slave_rcv(unsigned char length, unsigned char *pdata);
void button (void);
void Master_rcv(unsigned char length, unsigned char *pdata);
int Read_IMU(void);
void slave_rcv2(unsigned char length, unsigned char *pdata);
void slave_rcv3(unsigned char length, unsigned char *pdata);

typedef union{
	unsigned int yawvalue;
 	    	struct {
 			unsigned d1			:1;
 			unsigned d2 		:1;
 			unsigned d3			:1;
 			unsigned d4			:1;
 		    unsigned d5			:1;
 			unsigned d6			:1;
 			unsigned d7			:1;
 			unsigned d8			:1;
 			unsigned d9			:1;
 			unsigned d10		:7;
     };
 }yaww;



#ifdef __cplusplus
}
#endif
#endif /* COMMON_H_ */
