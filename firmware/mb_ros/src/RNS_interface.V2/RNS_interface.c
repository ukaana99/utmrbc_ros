/*********************************************/
/*          Include                          */
/*********************************************/
#include "RNS_interface.V2/RNS_interface.h"


/*********************************************/
/*          Variable                         */
/*********************************************/

uint8_t insData_send[2];

/*********************************************/
/*           Subroutine Function             */
/*********************************************/

/*
 * Function Name		: RNSInit
 * Function Description : This function is called to initialize the Robot Navigation System Module.
 * Function Remarks		: NONE
 * Function Arguments	: -if user define USED_CAN
 * 						   		CANx 		Select CAN peripheral (CAN1 or CAN2)
 * 						 		rns 		pointer to a RNS data structure with RNS_interface _t type
 * 						  -if user define USED_I2C
 * 						  		id			I2C address of RNS
 * 						  		I2Cx		Select I2C peripheral (I2C1, I2C2 or I2C3)
 * 						  		rns 		pointer to a RNS data structure with RNS_interface _t type
 * Function Return		: None
 * Function Example		: RNSInit(CAN1, &RNS);
 * 						  RNSInit(Robot_navi_system, I2C1,  &RNS);
 */

#if defined USED_CAN
void RNSInit(CAN_TypeDef* CANx, RNS_interface_t* rns)
{
	rns->rns_CANx = CANx;

	insData_send[0] = 1;
	insData_send[1] = RNS_PENDING;
	CAN_TxMsg(rns->rns_CANx,mainboard_TO_RNS,&insData_send,2);

	rns->RNS_data.common_instruction = RNS_PENDING;

	insData_send[0] = 1;
	insData_send[1] = RNS_RESET_POS;
	CAN_TxMsg(rns->rns_CANx,mainboard_TO_RNS,&insData_send,2);

#elif defined USED_I2C
void RNSInit(char id, I2C_TypeDef *I2Cx, RNS_interface_t* rns)
{
	rns->rns_address = id;
	rns->rns_I2Cx = I2Cx;

	I2CSendV(rns->rns_I2Cx,rns->rns_address, 1, RNS_PENDING);

	ucRNSSent = 0;

	rns->RNS_data.common_instruction = RNS_PENDING;

	I2CSendV(rns->rns_I2Cx,rns->rns_address, 1, RNS_RESET_POS);

#endif
	rns->RNS_data.common_instruction = RNS_WAITING;
}

/*
 * Function Name		: RNSStop
 * Function Description : Command the RNS board to stop and reset the position count.
 * Function Remarks		: NONE
 * Function Arguments	: rns 		pointer to a RNS data structure with RNS_interface _t type
 * Function Return		: None
 * Function Example		: RNSStop(&RNS);
 */

void RNSStop(RNS_interface_t* rns)
{
	rns->ins.instruction = RNS_STOP;
	rns->ins.ins_buffer[0].data = 0.0;
	rns->ins.ins_buffer[1].data = 0.0;
	rns->ins.ins_buffer[2].data = 0.0;
	rns->ins.ins_buffer[3].data = 0.0;

	//RNSSendIns(rns);
	rns->RNS_data.common_instruction = RNS_WAITING;

	#if defined USED_CAN
		insData_send[0] = 17;
		insData_send[1] = rns->ins.instruction;
		CAN_TxMsg(rns->rns_CANx,mainboard_TO_RNS,&insData_send,2);
		CAN_TxMsg(rns->rns_CANx,mainboard_TO_RNS_buf1,&(rns->ins.ins_buffer[0]),8);
		CAN_TxMsg(rns->rns_CANx,mainboard_TO_RNS_buf2,&(rns->ins.ins_buffer[2]),8);

	#elif defined USED_I2C
		I2CSend(rns->rns_I2Cx,rns->rns_address, sizeof(rns->ins), &(rns->ins));
	#endif

		while(rns->RNS_data.common_instruction == RNS_WAITING);

}

/*
 * Function Name		: RNSVelocity
 * Function Description : Command the RNS to move with specified velocity without any position control.
 * Function Remarks		: NONE
 * Function Arguments	: f_left_velocity	speed of front left motor in meter per second
 * 						  f_right_velocity	speed of front right motor in meter per second
 * 						  b_left_velocity 	speed of back left motor in meter per second
 * 						  b_left_velocity	speed of back right motor in meter per second
 * 						  rns 				pointer to a RNS data structure with RNS_interface _t type
 * Function Return		: None
 * Function Example		: RNSVelocity(1.0, 1.5 , 1.5 , 1.0, &RNS);
 */

void RNSVelocity(float fFLeftVelR, float fFRightVelR, float fBLeftVelR, float fBRightVelR, RNS_interface_t* rns)
{
	rns->ins.instruction = RNS_VELOCITY;
	rns->ins.ins_buffer[0].data = fFLeftVelR;
	rns->ins.ins_buffer[1].data = fFRightVelR;
	rns->ins.ins_buffer[2].data = fBLeftVelR;
	rns->ins.ins_buffer[3].data = fBRightVelR;

	RNSSendIns(rns);
}

/*
 * Function Name		: RNSPDC
 * Function Description : Command the RNS to move with give pulse width modulation duty cycle.
 * Function Remarks		: NONE
 * Function Arguments	: f_left_pdc	speed of front left motor in meter per second
 * 						  f_right_pdc	speed of front right motor in meter per second
 * 						  b_left_pdc 	speed of back left motor in meter per second
 * 						  b_left_pdc	speed of back right motor in meter per second
 * 						  rns 			pointer to a RNS data structure with RNS_interface _t type
 * Function Return		: None
 * Function Example		: RNSVelocity(1.0, 1.5 , 1.5 , 1.0, &RNS);
 */
void RNSPDC(float fFLeftPDC, float fFRightPDC, float fBLeftPDC, float fBRightPDC, RNS_interface_t* rns)
{
	rns->ins.instruction = RNS_PDC;
	rns->ins.ins_buffer[0].data = fFLeftPDC;
	rns->ins.ins_buffer[1].data = fFRightPDC;
	rns->ins.ins_buffer[2].data = fBLeftPDC;
	rns->ins.ins_buffer[3].data = fBRightPDC;

	RNSSendIns(rns);
}

/*
 * Function Name		: RNSLFDist
 * Function Description : Command the RNS to line follow with given distance and direction.
 * Function Remarks		: NONE
 * Function Arguments	: MoveDir	    enum type with the members of x_ax, y_ax
 * 						  Dir			enum type with the members of
 * 						     			DIR_FRONT, DIR_BACK,DIR_LEFT,DIR_RIGHT
 * 						  LF_dist 		Distance for line follow
 * 						  rns 			pointer to a RNS data structure with RNS_interface _t type
 * Function Return		: None
 * Function Example		: RNSLFDist(x_ax, DIR_LEFT, 100.0 ,&RNS);
 */
void RNSLFDist( dir_t Dir, float LF_vel, float LF_dist,RNS_interface_t* rns){

	rns->ins.instruction = RNS_LF_DIST;
	rns->ins.ins_buffer[0].data = (float)Dir;
	rns->ins.ins_buffer[1].data = LF_vel;
	rns->ins.ins_buffer[2].data = LF_dist;
	rns->ins.ins_buffer[3].data = 0;

	RNSSendIns(rns);
}

/*
 * Function Name		: RNSLFJunc
 * Function Description : Command the RNS to line follow with given direction and junction.
 * Function Remarks		: NONE
 * Function Arguments	: Dir			enum type with the members of
 * 						     			DIR_FRONT, DIR_BACK,DIR_LEFT,DIR_RIGHT
 * 						  LF_junc 		number of junction to be pass through with line follow
 * 						  rns 			pointer to a RNS data structure with RNS_interface _t type
 * Function Return		: None
 * Function Example		: RNSLFJunc(DIR_LEFT,1.5,2.5, 2.0 ,&RNS);
 */
void RNSLFJunc(dir_t Dir, float LF_vel, float LF_dist, float LF_junc,RNS_interface_t* rns){

	rns->ins.instruction = RNS_LF_JUNC;
	rns->ins.ins_buffer[0].data = (float)Dir;
	rns->ins.ins_buffer[1].data = LF_vel;
	rns->ins.ins_buffer[2].data = LF_dist;
	rns->ins.ins_buffer[3].data = LF_junc;

	RNSSendIns(rns);
}
/*
 * Function Name		: RNSIMURotate
 * Function Description : Command the RNS to rotate with given angle of max:+-180 degree.
 * Function Remarks		: NONE
 * Function Arguments	: AngleDeg		Rotate angle. Max:+-180degree, +:clkwise, -:anticlkwise
 * 						  rns 			pointer to a RNS data structure with RNS_interface _t type
 * Function Return		: None
 * Function Example		: RNSIMURotate(70,&RNS);
 */
void RNSIMURotate(int AngleDeg,RNS_interface_t* rns){
	rns->ins.instruction = RNS_ROTATE;
	rns->ins.ins_buffer[0].data = (float)AngleDeg;
	rns->ins.ins_buffer[1].data = 0;
	rns->ins.ins_buffer[2].data = 0;
	rns->ins.ins_buffer[3].data = 0;

	RNSSendIns(rns);
}

/*
 * Function Name		: RNSODNStart
 * Function Description : This function is called to start the path
 * Function Remarks		: This function will send the first and final point of the path to RNS
 * Function Arguments	: velocity1 	speed to go to the first point
 * 						  x1			X-coordinate of the first point	(in meter)
 * 						  y1			y-coordinate of the first point	(in meter)
 * 						  z1			z-coordinate of the first point (rotation angle in degree)
 						  velocity2     speed to go to final point
 * 						  xf			X-coordinate of the final point (in meter)
 * 						  yf			y-coordinate of the final point (in meter)
 * 						  zf			z-coordinate of the final point (rotation angle in degree)
 * 						  no_point		number of points in the path excluded the first an final point
 * 						  rns 			pointer to a RNS data structure with RNS_interface _t type
 * Function Return		: None
 * Function Example		: RNSODNStart(1.5, 1.5,2.5,160,1.0,4.8,5.6,120,3,&RNS);
 */
void RNSODNStart(float velocity1, float x1, float y1, float z1, float velocity2, float xf, float yf, float zf, int no_point, RNS_interface_t* rns){
	RNSSet(rns, RNS_ODNStartData1, velocity1,  x1,  y1,  z1);
	RNSSet(rns, RNS_ODNStartData2, velocity2, xf, yf, zf);
	RNSSet(rns, RNS_ODNStartData3, (float)no_point);

	rns->ins.instruction = RNS_ODNStart;
	rns->ins.ins_buffer[0].data = 0;
	rns->ins.ins_buffer[1].data = 0;
	rns->ins.ins_buffer[2].data = 0;
	rns->ins.ins_buffer[3].data = 0;

	RNSSendIns(rns);
}


/*
 * Function Name		: RNSODNKSet
 * Function Description : This function is called to set the pid of path plan on rns( PATH OR FINAL)
 * Function Remarks		: This function can only be called after RNSOdnStart is called
 * Function Arguments	: path_ctrl		path_control(either pid set on Path or Final point)
 * 						  pos_ctrl		ONAV_X/Y/Z gain
 * 						  a				Parameter of PID to be set
 * 						  rns 			pointer to a RNS data structure with RNS_interface _t type
 * Function Return		: None
 * Function Example		: RNSODNKSet(FINAL, ONAV_X_GAIN, KE, 0.5,&rns);
 * 							or 	RNSODNKSet(PATH, ONAV_Y_GAIN, KU, 0.5,&rns);
 */
void RNSODNKSet(unsigned char path_ctrl, unsigned char pos_ctrl, unsigned char a, float value,RNS_interface_t* rns){
	RNSSet(rns, RNS_ODNKSet, path_ctrl,  pos_ctrl,  a,  value);
}
/*
 * Function Name		: RNSOdnGoto
 * Function Description : This function is called to send all the points  for the path to rns for calculation
 * Function Remarks		: This function can only be called after RNSOdnStart is called
 * Function Arguments	: allpoints[][4]	array for all the points in the path
 * 						  no_point			Number of points to be sent (excluded the first and final point)
 * 						  rns 			pointer to a RNS data structure with RNS_interface _t type
 * Function Return		: None
 * Function Example		: float fZoneL1[2][4] = {{0.5, 1.5, 1.5, 180},					//for coordinates use this type of naming
					   	   	   	   	   	   	    {1.0, 3.9, 1.0, 180}}
 * 							RNSOdnGoto(fZoneL1,2,&rns);
 */

void RNSODNGoto(float allpoints[][4],int no_point,RNS_interface_t* rns){
	RNSSet(rns, RNS_ODNSend_num_Point,(float)no_point);

	for(int k=0;k<no_point;k++)
		RNSSet(rns, RNS_ODNSendPoint, allpoints[k][0],allpoints[k][1],allpoints[k][2],allpoints[k][3]);

	rns->ins.instruction = RNS_ODNGoTo;
	rns->ins.ins_buffer[0].data = 0;
	rns->ins.ins_buffer[1].data = 0;
	rns->ins.ins_buffer[2].data = 0;
	rns->ins.ins_buffer[3].data = 0;

	RNSSendIns(rns);
}

/*
 * Function Name		: RNSSendVelIns
 * Function Description : Checks the status of the RNS and sends the instruction to RNS after the previous instruction is sent.
 * Function Remarks		: Not intended to be used by user
 * Function Arguments	: rns 			pointer to a RNS data structure with RNS_interface _t type
 * Function Return		: None
 * Function Example		: None
 */

void RNSSendIns(RNS_interface_t* rns)
{

	while(rns->RNS_data.common_instruction == RNS_BUSY);
	rns->RNS_data.common_instruction = RNS_WAITING;

#if defined USED_CAN
	insData_send[0] = 17;
	insData_send[1] = rns->ins.instruction;
	CAN_TxMsg(rns->rns_CANx,mainboard_TO_RNS,&insData_send,2);
	CAN_TxMsg(rns->rns_CANx,mainboard_TO_RNS_buf1,&(rns->ins.ins_buffer[0]),8);
	CAN_TxMsg(rns->rns_CANx,mainboard_TO_RNS_buf2,&(rns->ins.ins_buffer[2]),8);

#elif defined USED_I2C
	I2CSend(rns->rns_I2Cx,rns->rns_address, sizeof(rns->ins), &(rns->ins));
#endif

	while(rns->RNS_data.common_instruction == RNS_WAITING);

}

/*
 * Function Name		: RNSSet
 * Function Description : Configure the RNS in terms of system function and gains.
 * Function Remarks		: Can be called during an instruction is running or during pending. Every value must cast to float type.
 * Function Arguments	: rns 			pointer to a RNS data structure with RNS_interface _t type
 * 						  parameter 	Enumeration of parameters
 * 						  ...			Values to set for the RNS, depending on parameters
 * Function Return		: None
 * Function Example		: RNSSet(&RNS, RNS_F_KCD_PTD, 0.9956, 0.01/2000);
 */

void RNSSet(RNS_interface_t* rns, unsigned char parameter, ...)
{
	va_list value;
	rns->param.parameter = parameter;
	va_start(value, parameter);
	while(rns->RNS_data.common_instruction == RNS_WAITING);
	if (parameter > RNS_PARAM_1){
		rns->param.param_buffer[0].data = va_arg(value, double);
		rns->param.param_buffer[1].data = 0;
		rns->param.param_buffer[2].data = 0;
		rns->param.param_buffer[3].data = 0;
	}
	if (parameter > RNS_PARAM_2){
		rns->param.param_buffer[1].data = va_arg(value, double);
		rns->param.param_buffer[2].data = 0;
		rns->param.param_buffer[3].data = 0;
	}
	if (parameter > RNS_PARAM_3){
		rns->param.param_buffer[2].data = va_arg(value, double);
		rns->param.param_buffer[3].data = 0;
	}
	if (parameter > RNS_PARAM_4){
		rns->param.param_buffer[3].data = va_arg(value, double);
	}
	va_end(value);
#if defined USED_CAN
	insData_send[0] = 17;
	insData_send[1] = rns->param.parameter;

	CAN_TxMsg(rns->rns_CANx,mainboard_TO_RNS,&insData_send,2);
	CAN_TxMsg(rns->rns_CANx,mainboard_TO_RNS_buf1,&(rns->param.param_buffer[0]),8);
	CAN_TxMsg(rns->rns_CANx,mainboard_TO_RNS_buf2,&(rns->param.param_buffer[2]),8);

#elif defined USED_I2C
	I2CSend(rns->rns_I2Cx,rns->rns_address, sizeof(rns->param), &(rns->param));
#endif
	rns->RNS_data.common_instruction = RNS_WAITING;
	while(rns->RNS_data.common_instruction == RNS_WAITING);
}

/*
 * Function Name		: RNSEnquire
 * Function Description : This function is classified under enquiry type instruction.
 * Function Remarks		: Should only be called during an instruction is running (the RNS is in motion).
 * 						  Failing to do so will return gibberish data or previously returned value.
 * Function Arguments	: parameter 	Enumeration to parameter to enquire
 * 						  rns 			pointer to a RNS data structure with RNS_interface _t type
 * Function Return		: None
 * Function Example		: RNSSet(&RNS, RNS_F_KCD_PTD, 0.9956, 0.01/2000);
 */

uint8_t RNSEnquire(unsigned char parameter, RNS_interface_t* rns)
{
	rns->ins.instruction = parameter;
	rns->ins.ins_buffer[0].data = 0.0;
	rns->ins.ins_buffer[1].data = 0.0;
	rns->ins.ins_buffer[2].data = 0.0;
	rns->ins.ins_buffer[3].data = 0.0;

//	RNSSendIns(rns);

	rns->RNS_data.common_instruction = RNS_WAITING;

	#if defined USED_CAN
		insData_send[0] = 17;
		insData_send[1] = rns->ins.instruction;
		CAN_TxMsg(rns->rns_CANx,mainboard_TO_RNS,&insData_send,2);
		CAN_TxMsg(rns->rns_CANx,mainboard_TO_RNS_buf1,&(rns->ins.ins_buffer[0]),8);
		CAN_TxMsg(rns->rns_CANx,mainboard_TO_RNS_buf2,&(rns->ins.ins_buffer[2]),8);

	#elif defined USED_I2C
		I2CSend(rns->rns_I2Cx,rns->rns_address, sizeof(rns->ins), &(rns->ins));
	#endif

	while(rns->RNS_data.common_instruction == RNS_WAITING);

	rns->enq.enquiry = parameter;
	rns->enq.enq_buffer[0].data = rns->RNS_data.common_buffer[0].data;
	rns->enq.enq_buffer[1].data = rns->RNS_data.common_buffer[1].data;
	rns->enq.enq_buffer[2].data = rns->RNS_data.common_buffer[2].data;
	rns->enq.enq_buffer[3].data = rns->RNS_data.common_buffer[3].data;

	return 1;
}

void RNS_CONT(RNS_interface_t* rns, uint8_t psxJoy_x, uint8_t psxJoy_y, uint8_t psxJoyL_2,uint8_t psxJoyR_2)
{
	rns->cont.buf1 = psxJoy_x;
	rns->cont.buf2 = psxJoy_y;
	rns->cont.buf3 = psxJoyL_2;
	rns->cont.buf4 = psxJoyR_2;

	while(rns->RNS_data.common_instruction == RNS_WAITING);
	while(rns->RNS_data.common_instruction == RNS_BUSY);
	rns->RNS_data.common_instruction = RNS_WAITING;

	CAN_TxMsg(rns->rns_CANx,CONT_data,&(rns->cont),4);
	while(rns->RNS_data.common_instruction == RNS_WAITING);
}
/*********************************************/



