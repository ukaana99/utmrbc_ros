
#ifndef SYSTEM_H_
#define SYSTEM_H_
#ifdef __cplusplus
extern "C" {
#endif

/*********************************************/
/*          Define                           */
/*********************************************/

#define SAMPLE_TIME 0.005f
#define MIN_POSCNT	500

#define P1DC1		TIM9->CCR1
#define P1DC2		TIM9->CCR2
#define P1DC3		TIM12->CCR1
#define P1DC4		TIM12->CCR2

#define psx_low_Rx	115.0
#define psx_high_Rx	140.0

#define psx_low_Ry	115.0
#define psx_high_Ry	140.0

/*********************************************/
/*          Structure                        */
/*********************************************/

typedef union{

	uint16_t flag;
	struct{
		unsigned activate		: 1;
		unsigned busy_flag		: 1;
		unsigned vel_flag 		: 1;
		unsigned lf_flag    	: 1;
		unsigned junc_flag 		: 1;
		unsigned dist_flag		: 1;
		unsigned lf_junc		: 1;
		unsigned lf_front 		: 1;
		unsigned lf_back 		: 1;
		unsigned lf_left 		: 1;
		unsigned lf_right 		: 1;
		unsigned rotate			: 1;
		unsigned odnstart_flag	: 1;
		unsigned odngoto_flag	: 1;
		unsigned feedback		: 1;
		unsigned controller		: 1;
	};
}flag_t;

typedef union{

	uint16_t flag;
	struct{
		unsigned user		: 1;
		unsigned flag_2		: 1;
		unsigned flag_3		: 1;
		unsigned flag_4	   	: 1;
		unsigned flag_5		: 1;
		unsigned flag_6		: 1;
		unsigned flag_7		: 1;
		unsigned flag_8	 	: 1;
		unsigned flag_9	 	: 1;
		unsigned flag_10	: 1;
		unsigned flag_11	: 1;
		unsigned flag_12	: 1;
		unsigned flag_13	: 1;
		unsigned flag_14	: 1;
		unsigned flag_15	: 1;
		unsigned flag_16	: 1;
	};
}User_flag_t;

typedef float_data_t POINT_data_t[40][4];

struct{

	uint8_t base_type;
	uint8_t motor_type;
	uint8_t PID_type;

	union{

		uint8_t motor_enc_dir;
		struct{
			uint8_t motor1_dir :1;
			uint8_t motor2_dir :1;
			uint8_t motor3_dir :1;
			uint8_t motor4_dir :1;
			uint8_t motor1_enc :1;
			uint8_t motor2_enc :1;
			uint8_t motor3_enc :1;
			uint8_t motor4_enc :1;
		};
	};

}dev_cfg;

/*********************************************/
/*          Variable                         */
/*********************************************/


/*********************************************/
/*           Function Prototype              */
/*********************************************/

void SYSSystemInit(void);
void SYSSystem5ms(void);
void SYSSystemInfiniteLoop(void);
void SYSSystemAct(void);

#ifdef __cplusplus
}
#endif
#endif /* SYSTEM_H_ */
