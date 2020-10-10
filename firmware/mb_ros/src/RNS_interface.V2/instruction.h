
#ifndef INSTRUCTION_H_
#define INSTRUCTION_H_

/*********************************************/
/*          Enumarator                       */
/*********************************************/

enum {
	x_reach=1,
	y_reach,
	z_reach
};

enum {
	triomni_velocity,
	fwdomni_velocity,
	coordinate,
	ref_point,
	u1234,
	coordinate_and_Velocity_triomni ,
	coordinate_and_Velocity_fwdomni,
	coordinate_and_ref,
	LSA_front,
	LSA_back,
	LSA_left,
	LSA_right
};

typedef enum{
	x_ax = 8,
	y_ax
}movedir_t;

typedef enum{
	AT_FRONT = 10,
	AT_BACK,
	AT_LEFT,
	AT_RIGHT
}lsa_pos_t;

typedef enum{
	DIR_FRONT = 14,
	DIR_BACK,
	DIR_LEFT,
	DIR_RIGHT
}dir_t;

enum {
	tri_omni=1,
	fwd_omni,
	mecanum
};

enum {
	fuzzyPID=1,
	roboconPID
};

enum{PATH = 0, FINAL};

// maximum 255
enum {
	/* Pending */
	RNS_PENDING,
	RNS_BUSY,
	RNS_WAITING,
	/**************************************************************************/

	/* Reset */
	RNS_STOP,			/* Stop everything */

	RNS_INS_RESET,
	/**************************************************************************/

	/* Velocity */
	RNS_VELOCITY,
	RNS_PDC,
	RNS_CONTROLLER,

	RNS_INS_VELOCITY,
	/**************************************************************************/

	/*Line Follow*/
	RNS_LF_DIST,
	RNS_LF_JUNC,

	RNS_INS_LINE_FOLLOW,
	/**************************************************************************/

	/*Path Planning*/

	RNS_ODNStart,
	RNS_ROTATE,

	RNS_INS_PATH_PLAN,
	/**************************************************************************/

	/* Setting */
	RNS_PARAM_0,

	RNS_RESET_POS,		/* 0 */

	RNS_ODNReset, 		/* 0 */
	RNS_ODNGoTo, 		/* 0 */

	RNS_PARAM_1,

	RNS_ODNSetd, 			/* 1 */
	RNS_ODNSend_num_Point, 	/* 1 */
	RNS_ODNStartData3,		/* 1 */

	RNS_PARAM_2,

	RNS_F_KCD_PTD,		/* 2 */
	RNS_B_KCD_PTD,		/* 2 */
	RNS_Printing,  		/* 2 */

	RNS_PARAM_3,

	RNS_DEVICE_CONFIG,	 	/* 3 */

	RNS_F_LEFT_ABT,			/* 3 */
	RNS_F_RIGHT_ABT,		/* 3 */
	RNS_B_LEFT_ABT,			/* 3 */
	RNS_B_RIGHT_ABT,		/* 3 */

	RNS_X_ABT, 				/* 3 */
	RNS_Y_ABT, 				/* 3 */

	RNS_F_LEFT_VEL_SATEU,	/* 3 */
	RNS_F_RIGHT_VEL_SATEU,	/* 3 */
	RNS_B_LEFT_VEL_SATEU,	/* 3 */
	RNS_B_RIGHT_VEL_SATEU,	/* 3 */

	RNS_F_LEFT_VEL_PID,		/* 3 */
	RNS_F_RIGHT_VEL_PID,	/* 3 */
	RNS_B_LEFT_VEL_PID,		/* 3 */
	RNS_B_RIGHT_VEL_PID,	/* 3 */

	RNS_ROTATE_SATEU, 		/* 3 */
	RNS_ROTATE_PID, 		/* 3 */

	RNS_LF_DIST_SATEU,  	/* 3 */
	RNS_LF_DIST_PID,		/* 3 */
	RNS_LF_ROTATE_SATEU, 	/* 3 */
	RNS_LF_ROTATE_PID, 		/* 3 */
	RNS_LF_FWD_SATEU, 		/* 3 */
	RNS_LF_FWD_PID, 		/* 3 */

	RNS_XYZfeedback, 		/* 3 */
	RNS_CONTROLLER_BASE_SPEED, /* 3 */

	RNS_F_LEFT_VEL_FUZZY_PID_BASE, 	/* 3 */
	RNS_F_RIGHT_VEL_FUZZY_PID_BASE, /* 3 */
	RNS_B_LEFT_VEL_FUZZY_PID_BASE, 	/* 3 */
	RNS_B_RIGHT_VEL_FUZZY_PID_BASE, /* 3 */

	RNS_F_LEFT_VEL_FUZZY_PID_PARAM, /* 3 */
	RNS_F_RIGHT_VEL_FUZZY_PID_PARAM,/* 3 */
	RNS_B_LEFT_VEL_FUZZY_PID_PARAM, /* 3 */
	RNS_B_RIGHT_VEL_FUZZY_PID_PARAM,/* 3 */

	RNS_ODNInit,  			/* 3 */


	RNS_ODNSetXYZ, 			/* 3 */
	RNS_ODNSetXYZerror, 	/* 3 */

	RNS_PARAM_4,

	RNS_ODNKSet,							/* 4 */

	RNS_X_Y_ENC_CONFIG,  					/* 4 */

	RNS_F_LEFT_VEL_FUZZY_PID_UEECES_MAX, 	/* 4 */
	RNS_F_RIGHT_VEL_FUZZY_PID_UEECES_MAX, 	/* 4 */
	RNS_B_LEFT_VEL_FUZZY_PID_UEECES_MAX, 	/* 4 */
	RNS_B_RIGHT_VEL_FUZZY_PID_UEECES_MAX, 	/* 4 */

	RNS_LF_LSA_POS, 						/* 4 */

	RNS_ODNStartData1, 						/* 4 */
	RNS_ODNStartData2, 						/* 4 */
	RNS_ODNSendPoint, 						/* 4 */

	RNS_INS_PARAM,
	/**************************************************************************/

	/* Enquiry */
	RNS_POS_LEFT,
	RNS_POS_RIGHT,
	RNS_POS_AVE,
	RNS_POS_BOTH,
	RNS_VEL_LEFT,
	RNS_VEL_RIGHT,
	RNS_VEL_AVE,
	RNS_VEL_BOTH,
	RNS_PDC_LEFT,
	RNS_PDC_RIGHT,
	RNS_PDC_AVE,
	RNS_PDC_BOTH,
	RNS_X_POS,
	RNS_Y_POS,
	RNS_X_Y_POS,
	RNS_X_RAW,
	RNS_Y_RAW,
	RNS_X_Y_RAW,
	RNS_X_Y_IMU_LSA,
	RNS_LSA_FRONT,
	RNS_LSA_BACK,
	RNS_LSA_LEFT,
	RNS_LSA_RIGHT,
	RNS_LSA_ALL,
	RNS_LF_JUNCTION,
	RNS_ANGLE,
	RNS_COORDINATE_X_Y_Z,
	RNS_PathPlan_VELOCITY,
	RNS_PathPlanEND,

	RNS_INS_ENQ,


	//USER
	RNS_INS_USER

	/**************************************************************************/
};
#endif /* INSTRUCTION_H_ */
