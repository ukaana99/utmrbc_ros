
/*********************************************/
/*          Include Header                   */
/*********************************************/

#include "include.h"

/*********************************************/
/*          Variable                         */
/*********************************************/



/*********************************************/
/*           Subroutine Function             */
/*********************************************/

void APPResetPos(void)
{
	sys.flag = 0;

	QEIReset(QEI1);
	QEIReset(QEI2);
	QEIReset(QEI3);
	QEIReset(QEI4);
	QEIReset(QEI5);
	QEIReset(QEI6);

	QEIWrite(QEI1, MIN_POSCNT);
	QEIWrite(QEI2, MIN_POSCNT);
	QEIWrite(QEI3, MIN_POSCNT);
	QEIWrite(QEI4, MIN_POSCNT);
	QEIWrite(QEI5, MIN_POSCNT);
	QEIWrite(QEI6, MIN_POSCNT);

	fFLeftPosData = 0.0;
	fFRightPosData = 0.0;

	fBLeftPosData = 0.0;
	fBRightPosData = 0.0;

	fXEncData = 0.0;
	fYEncData = 0.0;

	ABTEstimateInit(&fleft_pos_data);
	ABTEstimateInit(&fright_pos_data);

	ABTEstimateInit(&bleft_pos_data);
	ABTEstimateInit(&bright_pos_data);

	ABTEstimateInit(&x_data);
	ABTEstimateInit(&y_data);


}

void APPSet(param_t *par)
{
	static int s = 0, NumPoint = 0;

	switch (par->parameter){

	case RNS_CONTROLLER_BASE_SPEED:
		x_base_vel =  par->param_buffer[0].data;
		y_base_vel =  par->param_buffer[1].data;
		w_base_vel =  par->param_buffer[2].data;
		break;

	case RNS_XYZfeedback:
		xDis = par->param_buffer[0].data;
		yDis = par->param_buffer[1].data;
		zDis = par->param_buffer[2].data;



		sys.feedback = 1;
		break;

	case RNS_Printing:
		data = (uint8_t)par->param_buffer[0].data;
		status = par->param_buffer[1].data;
		LED4=0;
		break;



	case RNS_F_LEFT_ABT :
		fFLeftPosGain[0] = par->param_buffer[0].data;
		fFLeftPosGain[1] = par->param_buffer[1].data;
		fFLeftPosGain[2] = par->param_buffer[2].data;
		ABTInit(SAMPLE_TIME, fFLeftPosGain[0], fFLeftPosGain[1], fFLeftPosGain[2],
				&fFLeftPosData, &fFLeftPos, &fFLeftVel, &fFLeftAcc, &fleft_pos_data);
		break;

	case RNS_B_LEFT_ABT :
		fBLeftPosGain[0] = par->param_buffer[0].data;
		fBLeftPosGain[1] = par->param_buffer[1].data;
		fBLeftPosGain[2] = par->param_buffer[2].data;
		ABTInit(SAMPLE_TIME, fBLeftPosGain[0], fBLeftPosGain[1], fBLeftPosGain[2],
				&fBLeftPosData, &fBLeftPos, &fBLeftVel, &fBLeftAcc, &bleft_pos_data);
		break;

	case RNS_F_RIGHT_ABT :
		fFRightPosGain[0] = par->param_buffer[0].data;
		fFRightPosGain[1] = par->param_buffer[1].data;
		fFRightPosGain[2] = par->param_buffer[2].data;
		ABTInit(SAMPLE_TIME, fFRightPosGain[0], fFRightPosGain[1], fFRightPosGain[2],
				&fFRightPosData, &fFRightPos, &fFRightVel, &fFRightAcc, &fright_pos_data);
		break;

	case RNS_B_RIGHT_ABT :
		fBRightPosGain[0] = par->param_buffer[0].data;
		fBRightPosGain[1] = par->param_buffer[1].data;
		fBRightPosGain[2] = par->param_buffer[2].data;
		ABTInit(SAMPLE_TIME, fBRightPosGain[0], fBRightPosGain[1], fBRightPosGain[2],
				&fBRightPosData, &fBRightPos, &fBRightVel, &fBRightAcc, &bright_pos_data);
		break;

	case RNS_F_LEFT_VEL_SATEU :
		fFLeftVG[0] = par->param_buffer[0].data;
		fFLeftVG[1] = par->param_buffer[1].data;
		fFLeftVG[2] = par->param_buffer[2].data;
		PIDGainSet(SAT, fFLeftVG[0], &fleft_vel);
		PIDGainSet(KE, fFLeftVG[1], &fleft_vel);
		PIDGainSet(KU, fFLeftVG[2], &fleft_vel);
		break;

	case RNS_F_LEFT_VEL_PID :
		fFLeftVG[3] = par->param_buffer[0].data;
		fFLeftVG[4] = par->param_buffer[1].data;
		fFLeftVG[5] = par->param_buffer[2].data;
		PIDGainSet(KP, fFLeftVG[3], &fleft_vel);
		PIDGainSet(KI, fFLeftVG[4], &fleft_vel);
		PIDGainSet(KD, fFLeftVG[5], &fleft_vel);
		break;

	case RNS_B_LEFT_VEL_SATEU :
		fBLeftVG[0] = par->param_buffer[0].data;
		fBLeftVG[1] = par->param_buffer[1].data;
		fBLeftVG[2] = par->param_buffer[2].data;
		PIDGainSet(SAT, fBLeftVG[0], &bleft_vel);
		PIDGainSet(KE, fBLeftVG[1], &bleft_vel);
		PIDGainSet(KU, fBLeftVG[2], &bleft_vel);
		break;

	case RNS_B_LEFT_VEL_PID :
		fBLeftVG[3] = par->param_buffer[0].data;
		fBLeftVG[4] = par->param_buffer[1].data;
		fBLeftVG[5] = par->param_buffer[2].data;
		PIDGainSet(KP, fBLeftVG[3], &bleft_vel);
		PIDGainSet(KI, fBLeftVG[4], &bleft_vel);
		PIDGainSet(KD, fBLeftVG[5], &bleft_vel);
		break;

	case RNS_F_RIGHT_VEL_SATEU :
		fFRightVG[0] = par->param_buffer[0].data;
		fFRightVG[1] = par->param_buffer[1].data;
		fFRightVG[2] = par->param_buffer[2].data;
		PIDGainSet(SAT, fFRightVG[0], &fright_vel);
		PIDGainSet(KE, fFRightVG[1], &fright_vel);
		PIDGainSet(KU, fFRightVG[2], &fright_vel);
		break;

	case RNS_F_RIGHT_VEL_PID :
		fFRightVG[3] = par->param_buffer[0].data;
		fFRightVG[4] = par->param_buffer[1].data;
		fFRightVG[5] = par->param_buffer[2].data;
		PIDGainSet(KP, fFRightVG[3], &fright_vel);
		PIDGainSet(KI, fFRightVG[4], &fright_vel);
		PIDGainSet(KD, fFRightVG[5], &fright_vel);
		break;

	case RNS_B_RIGHT_VEL_SATEU :
		fBRightVG[0] = par->param_buffer[0].data;
		fBRightVG[1] = par->param_buffer[1].data;
		fBRightVG[2] = par->param_buffer[2].data;
		PIDGainSet(SAT, fBRightVG[0], &bright_vel);
		PIDGainSet(KE, fBRightVG[1], &bright_vel);
		PIDGainSet(KU, fBRightVG[2], &bright_vel);
		break;

	case RNS_B_RIGHT_VEL_PID :
		fBRightVG[3] = par->param_buffer[0].data;
		fBRightVG[4] = par->param_buffer[1].data;
		fBRightVG[5] = par->param_buffer[2].data;
		PIDGainSet(KP, fBRightVG[3], &bright_vel);
		PIDGainSet(KI, fBRightVG[4], &bright_vel);
		PIDGainSet(KD, fBRightVG[5], &bright_vel);
		break;


	case RNS_F_KCD_PTD :
		fFKcd = par->param_buffer[0].data;
		fFPtd = par->param_buffer[1].data;

		break;

	case RNS_B_KCD_PTD :
		fBKcd = par->param_buffer[0].data;
		fBPtd = par->param_buffer[1].data;
		break;

	case RNS_X_Y_ENC_CONFIG :
		xPtd = par->param_buffer[0].data;
		yPtd = par->param_buffer[2].data;

		if(par->param_buffer[1].data == 1.0)
			QEISwap(QEI2,QEI_No_Swap);
		else if(par->param_buffer[1].data == 2.0)
			QEISwap(QEI2,QEI_Swap);

		if(par->param_buffer[3].data == 1.0)
			QEISwap(QEI5,QEI_No_Swap);
		else if(par->param_buffer[3].data == 2.0)
			QEISwap(QEI5,QEI_Swap);

		break;

	case RNS_RESET_POS:
		APPResetPos();
		break;

	case RNS_LF_DIST_SATEU:
		fLFDistG[0] = par->param_buffer[0].data;
		fLFDistG[1] = par->param_buffer[1].data;
		fLFDistG[2] = par->param_buffer[2].data;
		PIDGainSet(SAT, fLFDistG[0], &lf_dist);
		PIDGainSet(KE, fLFDistG[1], &lf_dist);
		PIDGainSet(KU, fLFDistG[2], &lf_dist);
		break;

	case RNS_LF_DIST_PID:
		fLFDistG[3] = par->param_buffer[0].data;
		fLFDistG[4] = par->param_buffer[1].data;
		fLFDistG[5] = par->param_buffer[2].data;
		PIDGainSet(KP, fLFDistG[3], &lf_dist);
		PIDGainSet(KI, fLFDistG[4], &lf_dist);
		PIDGainSet(KD, fLFDistG[5], &lf_dist);
		break;

	case RNS_ROTATE_SATEU:
		fAngleG[0] = par->param_buffer[0].data;
		fAngleG[1] = par->param_buffer[1].data;
		fAngleG[2] = par->param_buffer[2].data;
		PIDGainSet(SAT, fAngleG[0], &imu_rotate);
		PIDGainSet(KE, fAngleG[1], &imu_rotate);
		PIDGainSet(KU, fAngleG[2], &imu_rotate);
		break;

	case RNS_ROTATE_PID:
		fAngleG[3] = par->param_buffer[0].data;
		fAngleG[4] = par->param_buffer[1].data;
		fAngleG[5] = par->param_buffer[2].data;
		PIDGainSet(KP, fAngleG[3], &imu_rotate);
		PIDGainSet(KI, fAngleG[4], &imu_rotate);
		PIDGainSet(KD, fAngleG[5], &imu_rotate);
		break;

	case RNS_LF_ROTATE_SATEU:
		fRotateG[0] = par->param_buffer[0].data;
		fRotateG[1] = par->param_buffer[1].data;
		fRotateG[2] = par->param_buffer[2].data;
		PIDGainSet(SAT, fRotateG[0], &lf_rotate);
		PIDGainSet(KE, fRotateG[1], &lf_rotate);
		PIDGainSet(KU, fRotateG[2], &lf_rotate);
		break;

	case RNS_LF_ROTATE_PID:
		fRotateG[3] = par->param_buffer[0].data;
		fRotateG[4] = par->param_buffer[1].data;
		fRotateG[5] = par->param_buffer[2].data;
		PIDGainSet(KP, fRotateG[3], &lf_rotate);
		PIDGainSet(KI, fRotateG[4], &lf_rotate);
		PIDGainSet(KD, fRotateG[5], &lf_rotate);
		break;

	case RNS_LF_FWD_SATEU:
		fFwdG[0] = par->param_buffer[0].data;
		fFwdG[1] = par->param_buffer[1].data;
		fFwdG[2] = par->param_buffer[2].data;
		PIDGainSet(SAT, fFwdG[0], &lf_fwd);
		PIDGainSet(KE, fFwdG[1], &lf_fwd);
		PIDGainSet(KU, fFwdG[2], &lf_fwd);
		break;

	case RNS_LF_FWD_PID:
		fFwdG[3] = par->param_buffer[0].data;
		fFwdG[4] = par->param_buffer[1].data;
		fFwdG[5] = par->param_buffer[2].data;
		PIDGainSet(KP, fFwdG[3], &lf_fwd);
		PIDGainSet(KI, fFwdG[4], &lf_fwd);
		PIDGainSet(KD, fFwdG[5], &lf_fwd);
		break;

//	case RNS_LF_LSA_POS :
//
//		if((int)par->param_buffer[0].data == AT_FRONT)
//			 LSA_FRONT = &LSA_A;
//		else if((int)par->param_buffer[0].data == AT_BACK)
//			 LSA_BACK = &LSA_A;
//		else if((int)par->param_buffer[0].data == AT_LEFT)
//			 LSA_LEFT = &LSA_A;
//		else if((int)par->param_buffer[0].data == AT_RIGHT)
//			 LSA_RIGHT = &LSA_A;
//
//		if((int)par->param_buffer[1].data == AT_FRONT)
//			LSA_FRONT = &LSA_B;
//		else if((int)par->param_buffer[1].data == AT_BACK)
//			LSA_BACK = &LSA_B;
//		else if((int)par->param_buffer[1].data == AT_LEFT)
//			LSA_LEFT = &LSA_B ;
//		else if((int)par->param_buffer[1].data == AT_RIGHT)
//			LSA_RIGHT = &LSA_B;
//
//		if((int)par->param_buffer[2].data == AT_FRONT)
//			LSA_FRONT = &LSA_C;
//		else if((int)par->param_buffer[2].data == AT_BACK)
//			LSA_BACK = &LSA_C;
//		else if((int)par->param_buffer[2].data == AT_LEFT)
//			LSA_LEFT = &LSA_C;
//		else if((int)par->param_buffer[2].data == AT_RIGHT)
//			LSA_RIGHT = &LSA_C;
//
//		if((int)par->param_buffer[3].data == AT_FRONT)
//			LSA_FRONT = &LSA_D;
//		else if((int)par->param_buffer[3].data == AT_BACK)
//			LSA_BACK = &LSA_D ;
//		else if((int)par->param_buffer[3].data == AT_LEFT)
//			LSA_LEFT = &LSA_D;
//		else if((int)par->param_buffer[3].data == AT_RIGHT)
//			LSA_RIGHT = &LSA_D;
//
//		break;

	case RNS_X_ABT:
		fXPosGain[0] = par->param_buffer[0].data;
		fXPosGain[1] = par->param_buffer[1].data;
		fXPosGain[2] = par->param_buffer[2].data;
		ABTInit(SAMPLE_TIME, fXPosGain[0], fXPosGain[1], fXPosGain[2], &fXEncData ,&fXPos, &fXVel, &fXAcc, &x_data);
		break;

	case RNS_Y_ABT:
		fYPosGain[0] = par->param_buffer[0].data;
		fYPosGain[1] = par->param_buffer[1].data;
		fYPosGain[2] = par->param_buffer[2].data;
		ABTInit(SAMPLE_TIME, fYPosGain[0], fYPosGain[1], fYPosGain[2], &fYEncData, &fYPos, &fYVel, &fYAcc, &y_data);
		break;

	case RNS_DEVICE_CONFIG :
		dev_cfg.motor_enc_dir = (unsigned char) par->param_buffer[0].data;
		dev_cfg.base_type = (unsigned char) par->param_buffer[1].data;
		dev_cfg.PID_type = (unsigned char) par->param_buffer[2].data;

		if (dev_cfg.motor1_enc == 0)
			QEISwap(QEI6,QEI_No_Swap);
		else
			QEISwap(QEI6,QEI_Swap);

		if (dev_cfg.motor2_enc == 0)
			QEISwap(QEI4,QEI_No_Swap);
		else
			QEISwap(QEI4,QEI_Swap);

		if (dev_cfg.motor3_enc == 0)
			QEISwap(QEI1,QEI_No_Swap);
		else
			QEISwap(QEI1,QEI_Swap);

		if (dev_cfg.motor4_enc == 0)
			QEISwap(QEI3,QEI_No_Swap);
		else
			QEISwap(QEI3,QEI_Swap);

		break;

	default:
		break;
	}
}

void APPApply(ins_t *pins)
{
	switch (pins->instruction){

	case RNS_CONTROLLER:
		joy_x = main_board_1_data_receive.common_buffer[0].byte1;
		joy_y = main_board_1_data_receive.common_buffer[0].byte2;
		an_L2 = main_board_1_data_receive.common_buffer[0].byte3;
		an_R2 = main_board_1_data_receive.common_buffer[0].byte4;
		break;


	case RNS_VELOCITY:
		fFLeftVelR 	= pins->ins_buffer[0].data;
		fFRightVelR = pins->ins_buffer[1].data;
		fBLeftVelR 	= pins->ins_buffer[2].data;
		fBRightVelR = pins->ins_buffer[3].data;
		break;

	case RNS_PDC:
		fFLeftVelU 	= pins->ins_buffer[0].data;
		fFRightVelU = pins->ins_buffer[1].data;
		fBLeftVelU 	= pins->ins_buffer[2].data;
		fBRightVelU = pins->ins_buffer[3].data;
		break;

	case RNS_ROTATE:
		AngleTargetDeg = (int)fyaw + (int)pins->ins_buffer[0].data; //+ve:clkwise,-ve:anti-clkwise,max:+-180
		break;

	case RNS_LF_DIST:
		Dir = (int)pins->ins_buffer[0].data;
		LF_vel = pins->ins_buffer[1].data;
		LF_dist = pins->ins_buffer[2].data;

		if (Dir == DIR_LEFT )
			LFtargetDist = fXEncData - LF_dist;
		else if ( Dir == DIR_RIGHT)
			LFtargetDist = fXEncData + LF_dist;
		else if (Dir == DIR_FRONT)
			LFtargetDist = fYEncData + LF_dist;
		else if (Dir == DIR_BACK)
			LFtargetDist = fYEncData - LF_dist;

		if (Dir == DIR_LEFT || Dir == DIR_BACK)
			PIDGainSet(KE,-1/LF_dist,&lf_dist);
		else if(Dir == DIR_RIGHT || Dir == DIR_FRONT)
			PIDGainSet(KE,1/LF_dist,&lf_dist);
		PIDGainSet(KU,LF_vel,&lf_dist);
		break;

	case RNS_LF_JUNC:
		Dir = (int)pins->ins_buffer[0].data;
		LF_vel = pins->ins_buffer[1].data;
		LF_dist = pins->ins_buffer[2].data;
		LF_junc = pins->ins_buffer[3].data;
		LFtargetJunc = junction_count + (int)LF_junc;

		if (Dir == DIR_LEFT || Dir == DIR_BACK)
			PIDGainSet(KE,-1/LF_dist,&lf_dist);
		else if(Dir == DIR_RIGHT || Dir == DIR_FRONT)
			PIDGainSet(KE,1/LF_dist,&lf_dist);
		PIDGainSet(KU,LF_vel,&lf_dist);
		break;

	case RNS_ODNStart:
	//	ODNStart(odn[0], odn[1], odn[2], odn[3], odn[4], odn[5], odn[6], odn[7],(int)odn[8] ,&PathPlan);
		break;

	default:
		break;
	}
}

void APPStart(ins_t *pins)
{

	switch (pins->instruction){

	case RNS_CONTROLLER:
		sys.controller = 1;
		sys.vel_flag = 1;
		break;

	case RNS_VELOCITY:
		sys.vel_flag = 1;
		break;

	case RNS_PDC:
		break;

	case RNS_LF_DIST:

		if(Dir == DIR_FRONT){
			sys.lf_front = 1;
		}else if(Dir == DIR_BACK){
			sys.lf_back = 1;
		}else if(Dir == DIR_LEFT){
			sys.lf_left = 1;
		}else if(Dir == DIR_RIGHT){
			sys.lf_right = 1;
		}

		sys.lf_flag= 1;
		sys.dist_flag = 1;
		sys.vel_flag = 1;
		sys.busy_flag = 1;

		break;

	case RNS_LF_JUNC:

		if(Dir == DIR_FRONT){
			sys.lf_front = 1;
		}else if(Dir == DIR_BACK){
			sys.lf_back = 1;
		}else if(Dir == DIR_LEFT){
			sys.lf_left = 1;
		}else if(Dir == DIR_RIGHT){
			sys.lf_right = 1;
		}
		sys.lf_flag= 1;
		sys.lf_junc = 1;
		sys.vel_flag = 1;
		sys.busy_flag = 1;
		break;

	case RNS_ROTATE:
		sys.rotate = 1;
		sys.vel_flag = 1;
		sys.busy_flag = 1;
		break;

	case RNS_ODNStart:
		sys.odnstart_flag = 1;
		sys.vel_flag = 1;
		sys.busy_flag = 1;
		break;

	default:
		break;
	}
}

void APPStop(void)
{
	sys.flag = 0;	/* Reset system flag */

	PIDDelayInit(&fleft_vel);
	PIDDelayInit(&fright_vel);

	PIDDelayInit(&bleft_vel);
	PIDDelayInit(&bright_vel);

	fFLeftVelR = 0.0;
	fFLeftVelU = 0.0;

	fFRightVelR = 0.0;
	fFRightVelU = 0.0;

	fBLeftVelR = 0.0;
	fBLeftVelU = 0.0;

	fBRightVelR = 0.0;
	fBRightVelU = 0.0;

	PIDDelayInit(&lf_dist);
	PIDDelayInit(&lf_rotate);
	PIDDelayInit(&lf_fwd);
	PIDDelayInit(&imu_rotate);

	fLFDistErr = 0;
	fLFDistU = 0;

	fRotateErr = 0;
	fRotateU = 0;

	fFwdErr = 0;
	fFwdU = 0;

	fAngleErr = 0;
	fAngleU = 0;
}

unsigned char APPBusy(ins_t *pins)
{
	switch (pins->instruction){

	case RNS_LF_DIST:
	case RNS_LF_JUNC:
	case RNS_ROTATE:
		return sys.busy_flag;
		break;

	case RNS_ODNStart:
		//return ODNBusy(&PathPlan);
		break;

	default:
		break;
	}

	return 0;
}

void APPEnquire(enq_t *penq)
{
	switch (penq->enquiry){

	case RNS_POS_LEFT :
		penq->enq_buffer[0].data = fFLeftPos;
		penq->enq_buffer[1].data = fBLeftPos;
		break;

	case RNS_POS_RIGHT :
		penq->enq_buffer[0].data = fFRightPos;
		penq->enq_buffer[1].data = fBRightPos;
		break;

	case RNS_POS_AVE :
		penq->enq_buffer[0].data = (fFLeftPos + fFRightPos) / 2;
		penq->enq_buffer[1].data = (fBLeftPos + fBRightPos) / 2;
		break;

	case RNS_POS_BOTH :
		penq->enq_buffer[0].data = fFLeftPos;
		penq->enq_buffer[1].data = fFRightPos;
		penq->enq_buffer[2].data = fBLeftPos;
		penq->enq_buffer[3].data = fBRightPos;
		penq->enq_buffer[4].data = fXPos;
		penq->enq_buffer[5].data = fYPos;
		break;

	case RNS_VEL_LEFT :
		penq->enq_buffer[0].data = fFLeftVel;
		penq->enq_buffer[1].data = fBLeftVel;
		break;

	case RNS_VEL_RIGHT :
		penq->enq_buffer[0].data = fFRightVel;
		penq->enq_buffer[1].data = fBRightVel;
		break;

	case RNS_VEL_AVE :
		penq->enq_buffer[0].data = (fFLeftVel + fFRightVel) / 2;
		penq->enq_buffer[1].data = (fBLeftVel + fBRightVel) / 2;
		break;

	case RNS_VEL_BOTH :
		penq->enq_buffer[0].data = fFLeftVel;
		penq->enq_buffer[1].data = fFRightVel;
		penq->enq_buffer[2].data = fBLeftVel;
		penq->enq_buffer[3].data = fBRightVel;
		break;

	case RNS_PDC_LEFT :
		penq->enq_buffer[0].data = fFLeftVelU;
		penq->enq_buffer[1].data = fBLeftVelU;
		break;

	case RNS_PDC_RIGHT :
		penq->enq_buffer[0].data = fFRightVelU;
		penq->enq_buffer[1].data = fBRightVelU;
		break;

	case RNS_PDC_AVE :
		penq->enq_buffer[0].data = (fFLeftVelU + fFRightVelU) / 2;
		penq->enq_buffer[1].data = (fBLeftVelU + fBRightVelU) / 2;
		break;

	case RNS_PDC_BOTH :
		penq->enq_buffer[0].data = fFLeftVelU;
		penq->enq_buffer[1].data = fFRightVelU;
		penq->enq_buffer[2].data = fBLeftVelU;
		penq->enq_buffer[3].data = fBRightVelU;
		break;

	case RNS_X_POS:
		penq->enq_buffer[0].data = fXPos;
		break;

	case RNS_Y_POS:
		penq->enq_buffer[0].data = fYPos;
		break;

	case RNS_X_Y_POS:
		penq->enq_buffer[0].data = fXPos;
		penq->enq_buffer[1].data = fYPos;
		break;

	case RNS_X_RAW:
		penq->enq_buffer[0].data = (float)BIOS_QEI2.count;
		break;

	case RNS_Y_RAW:
		penq->enq_buffer[0].data = (float)BIOS_QEI5.count;
		break;

	case RNS_X_Y_RAW:
		penq->enq_buffer[0].data = (float)BIOS_QEI2.count;
		penq->enq_buffer[1].data = (float)BIOS_QEI5.count;
		break;

	case RNS_X_Y_IMU_LSA:
//		LSA_read(LSA_FRONT);
//		LsaData.lsa_F = LSA_FRONT->LSA_bits.Byte;
//		LSA_read(LSA_BACK);
//		LsaData.lsa_B = LSA_BACK->LSA_bits.Byte;
//		LSA_read(LSA_LEFT);
//		LsaData.lsa_L = LSA_LEFT->LSA_bits.Byte;
//		LSA_read(LSA_RIGHT);
//		LsaData.lsa_R = LSA_RIGHT->LSA_bits.Byte;
		penq->enq_buffer[0].data = fyaw;
		penq->enq_buffer[1].data = fXPos;
		penq->enq_buffer[2].data = fYPos;
		penq->enq_buffer[3].data = LsaData.LSA_ALL;
		break;

//	case RNS_LSA_FRONT:
//		LSA_read(LSA_FRONT);
//		penq->enq_buffer[0].data = (float)LSA_FRONT->LSA_bits.Byte;
//		break;
//
//	case RNS_LSA_BACK:
//		LSA_read(LSA_BACK);
//		penq->enq_buffer[0].data = (float)LSA_BACK->LSA_bits.Byte;
//		break;
//
//	case RNS_LSA_LEFT:
//		LSA_read(LSA_LEFT);
//		penq->enq_buffer[0].data = (float)LSA_LEFT->LSA_bits.Byte;
//		break;
//
//	case RNS_LSA_RIGHT:
//		LSA_read(LSA_RIGHT);
//		penq->enq_buffer[0].data = (float)LSA_RIGHT->LSA_bits.Byte;
//		break;

//	case RNS_LSA_ALL:
//		LSA_read(LSA_FRONT);
//		LSA_read(LSA_BACK);
//		LSA_read(LSA_LEFT);
//		LSA_read(LSA_RIGHT);
//		penq->enq_buffer[0].data = (float)LSA_FRONT->LSA_bits.Byte;
//		penq->enq_buffer[1].data = (float)LSA_BACK->LSA_bits.Byte;
//		penq->enq_buffer[2].data = (float)LSA_LEFT->LSA_bits.Byte;
//		penq->enq_buffer[2].data = (float)LSA_RIGHT->LSA_bits.Byte;
//		break;
//
//	case RNS_LF_JUNCTION:
//		penq->enq_buffer[0].data = (float)junction_count;
//		break;
//
//	case RNS_ANGLE:
//		penq->enq_buffer[0].data = fyaw;
//	break;
//
//	case RNS_COORDINATE_X_Y_Z:
//		penq->enq_buffer[0].data = x_axis;	//PathPlan.point3_p1.p.x;
//		penq->enq_buffer[1].data = y_axis;	//PathPlan.point3_p1.p.y;
//		penq->enq_buffer[2].data = z_axis;	//PathPlan.point3_p1.p.z;
////		LED5 = 0;
//		break;
//
//	case RNS_PathPlan_VELOCITY:
//		penq->enq_buffer[0].data = PathPlan.lc1.v1;
//		penq->enq_buffer[1].data = PathPlan.lc1.v2;
//		penq->enq_buffer[2].data = PathPlan.lc1.v3;
//		penq->enq_buffer[3].data = PathPlan.lc1.v4;
//		break;

	}

}

uint8_t APPPrintstatus(void){

	return status;
}

void APPPrinting(char buff[]){

//	switch (data){
//
//		case triomni_velocity: //veocity calculated by LC (PathPlan)
//			sprintf(buff, "v1=%.2f v2=%.2f, v3=%.2f\r\n", velo1, velo2, velo3);
//			break;
//
//		case coordinate_and_Velocity_triomni :
//			sprintf(buff, "x=%.2f y=%.2f, z=%.2f, zrad=%.2f Imuraw=%.2f v1=%.2f v2=%.2f, v3=%.2f \r\n ",
//					x_axis, y_axis, z_axis, z_rad, fyaw ,velo1, velo2, velo3);
//			break;
//		case coordinate_and_Velocity_fwdomni :
//			sprintf(buff, "x=%.2f y=%.2f, z=%.2f, zrad=%.2f Imuraw=%.2f  v1=%.2f v2=%.2f, v3=%.2f v4=%.2f \r\n ",
//					x_axis, y_axis, z_axis, z_rad, fyaw,velo1, velo2, velo3,velo4);
//			break;
//		case fwdomni_velocity:  //veocity calculated by LC (PathPlan)
//			sprintf(buff, "v1=%.2f v2=%.2f, v3=%.2f, v4=%.2f\r\n", velo1, velo2, velo3,velo4);
//			break;
//		case coordinate_and_ref:
//			sprintf(buff, "x=%.2f y=%.2f, z=%.2f, zrad=%.2f Imuraw=%.2f x1=%.2f x0=%.2f, y1=%.2f, y0=%.2f z1=%.2f z0=%.2f\r\n",
//					x_axis, y_axis, z_axis, z_rad, fyaw, x_ref,x_ref_p, y_ref, y_ref_p, z_ref, z_ref_p);
//			break;
//
//		case coordinate:
//			sprintf(buff, "x=%.2f y=%.2f, z=%.2f, zrad=%.2f Imuraw=%.2f\r\n",
//					x_axis, y_axis, z_axis, z_rad, fyaw);
//			break;
//		case ref_point:
//			sprintf(buff, "x1=%.2f x0=%.2f, y1=%.2f, y0=%.2f z1=%.2f z0=%.2f\r\n", x_ref,
//					x_ref_p, y_ref, y_ref_p, z_ref, z_ref_p);
//			break;
//		case u1234:
//			sprintf(buff, "vux=%.2f vuy=%.2f, vvx=%.2f, vvy=%.2f u1=%.2f u2=%.2f u3=%.2f u4=%.2f\r\n",
//					vux, vuy, vvx, vvy, u1, u2, u3, u4);
//			break;
//
//		case LSA_front:
//			LSA_read(LSA_FRONT);
//			sprintf(buff,"%d %d %d %d %d %d %d %d\r\n",LSA_FRONT->LSA_bits.bit0,LSA_FRONT->LSA_bits.bit1,LSA_FRONT->LSA_bits.bit2,
//					LSA_FRONT->LSA_bits.bit3,LSA_FRONT->LSA_bits.bit4,LSA_FRONT->LSA_bits.bit5,LSA_FRONT->LSA_bits.bit6,LSA_FRONT->LSA_bits.bit7);
//			break;
//		case LSA_back:
//			LSA_read(LSA_BACK);
//			sprintf(buff,"%d %d %d %d %d %d %d %d\r\n",LSA_BACK->LSA_bits.bit0,LSA_BACK->LSA_bits.bit1,LSA_BACK->LSA_bits.bit2,
//					LSA_BACK->LSA_bits.bit3,LSA_BACK->LSA_bits.bit4,LSA_BACK->LSA_bits.bit5,LSA_BACK->LSA_bits.bit6,LSA_BACK->LSA_bits.bit7);
//			break;
//		case LSA_left:
//			LSA_read(LSA_LEFT);
//			sprintf(buff,"%d %d %d %d %d %d %d %d\r\n",LSA_LEFT->LSA_bits.bit0,LSA_LEFT->LSA_bits.bit1,LSA_LEFT->LSA_bits.bit2,
//					LSA_LEFT->LSA_bits.bit3,LSA_LEFT->LSA_bits.bit4,LSA_LEFT->LSA_bits.bit5,LSA_LEFT->LSA_bits.bit6,LSA_LEFT->LSA_bits.bit7);
//			break;
//		case LSA_right:
//			LSA_read(LSA_RIGHT);
//			sprintf(buff,"%d %d %d %d %d %d %d %d\r\n",LSA_RIGHT->LSA_bits.bit0,LSA_RIGHT->LSA_bits.bit1,LSA_RIGHT->LSA_bits.bit2,
//					LSA_RIGHT->LSA_bits.bit3,LSA_RIGHT->LSA_bits.bit4,LSA_RIGHT->LSA_bits.bit5,LSA_RIGHT->LSA_bits.bit6,LSA_RIGHT->LSA_bits.bit7);
//			break;
//		default:
//			break;
//	}

}

uint8_t APPFeedback(void){

	if(xstate == 1){
		xstate = 0;
		xDis = 0;
		x_case = 0;
		return x_reach;
	} else if(ystate == 1){
		ystate  =0;
		yDis = 0;
		y_case = 0;
		return y_reach;
	} else if(zstate == 1){
		zstate = 0;
		zDis = 0;
		z_case = 0;
		return z_reach;
	}

	return 0;

}
