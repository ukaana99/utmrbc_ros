
/*********************************************/
/*          Include Header                   */
/*********************************************/

#include "include.h"

/*********************************************/
/*          Variable                         */
/*********************************************/
//float fprevyaw = 0;
//float fdifyaw = 0;

/*********************************************/
/*           Subroutine Function             */
/*********************************************/

void SYSSystemInit(void)
{
	/* Left position ABT*/
	ABTInit(SAMPLE_TIME, fFLeftPosGain[0], fFLeftPosGain[1], fFLeftPosGain[2], &fFLeftPosData, &fFLeftPos, &fFLeftVel, &fFLeftAcc, &fleft_pos_data);
	ABTEstimateInit(&fleft_pos_data);

	ABTInit(SAMPLE_TIME, fBLeftPosGain[0], fBLeftPosGain[1], fBLeftPosGain[2], &fBLeftPosData, &fBLeftPos, &fBLeftVel, &fBLeftAcc, &bleft_pos_data);
	ABTEstimateInit(&bleft_pos_data);

	/* Right position ABT */
	ABTInit(SAMPLE_TIME, fFRightPosGain[0], fFRightPosGain[1], fFRightPosGain[2], &fFRightPosData, &fFRightPos, &fFRightVel, &fFRightAcc, &fright_pos_data);
	ABTEstimateInit(&fright_pos_data);

	ABTInit(SAMPLE_TIME, fBRightPosGain[0], fBRightPosGain[1], fBRightPosGain[2], &fBRightPosData, &fBRightPos, &fBRightVel, &fBRightAcc, &bright_pos_data);
	ABTEstimateInit(&bright_pos_data);

	/* X & Y position ABT */
	ABTInit(SAMPLE_TIME, fXPosGain[0], fXPosGain[1], fXPosGain[2], &fXEncData ,&fXPos, &fXVel, &fXAcc, &x_data);
	ABTEstimateInit(&x_data);

	ABTInit(SAMPLE_TIME, fYPosGain[0], fYPosGain[1], fYPosGain[2], &fYEncData, &fYPos, &fYVel, &fYAcc, &y_data);
	ABTEstimateInit(&y_data);

	//ROBOCONPID
	/* Left velocity PID*/
	PIDSourceInit(&fFLeftVelErr, &fFLeftVelU, &fleft_vel);
	PIDDelayInit(&fleft_vel);
	PIDGainInit(SAMPLE_TIME, fFLeftVG[0], fFLeftVG[1], fFLeftVG[2], fFLeftVG[3], fFLeftVG[4], fFLeftVG[5], fFLeftVG[6], &fleft_vel);

	PIDSourceInit(&fBLeftVelErr, &fBLeftVelU, &bleft_vel);
	PIDDelayInit(&bleft_vel);
	PIDGainInit(SAMPLE_TIME, fBLeftVG[0], fBLeftVG[1], fBLeftVG[2], fBLeftVG[3], fBLeftVG[4], fBLeftVG[5], fBLeftVG[6], &bleft_vel);

	/* Right velocity PID*/
	PIDSourceInit(&fFRightVelErr, &fFRightVelU, &fright_vel);
	PIDDelayInit(&fright_vel);
	PIDGainInit(SAMPLE_TIME, fFRightVG[0], fFRightVG[1], fFRightVG[2], fFRightVG[3], fFRightVG[4], fFRightVG[5], fFRightVG[6], &fright_vel);

	PIDSourceInit(&fBRightVelErr, &fBRightVelU, &bright_vel);
	PIDDelayInit(&bright_vel);
	PIDGainInit(SAMPLE_TIME, fBRightVG[0], fBRightVG[1], fBRightVG[2], fBRightVG[3], fBRightVG[4], fBRightVG[5], fBRightVG[6], &bright_vel);
}

void SYSSystem5ms(void)
{
	/* Obtain position for left and right */
	fFLeftPosData = fFPtd * (QEIRead(QEI6) - MIN_POSCNT);
	fFRightPosData = fFKcd * fFPtd * (QEIRead(QEI4) - MIN_POSCNT);

	fBLeftPosData = fBPtd * (QEIRead(QEI1) -MIN_POSCNT);
	fBRightPosData = fBKcd * fBPtd * (QEIRead(QEI3) - MIN_POSCNT);

	fXEncData = xPtd * (QEIRead(QEI2) - MIN_POSCNT);
	fYEncData = yPtd * (QEIRead(QEI5) - MIN_POSCNT);

	/* ABT filter */
	ABT(&fleft_pos_data);
	ABT(&fright_pos_data);

	ABT(&bleft_pos_data);
	ABT(&bright_pos_data);

	ABT(&x_data);
	ABT(&y_data);

	//	fyaw = Read_IMU();
//	fyaw = (fyaw <= 180.0) ? fyaw : fyaw - 360;
	fyaw = (fyaw >= 0.0) ? fyaw : 360.0 + fyaw;

	if (sys.activate){
		//After finish running your task, reset UF.user flag to
		//enable it to go to UserFinish state

		//UserEnd
		if (sys.vel_flag){
			fFLeftVelErr = fFLeftVelR - fFLeftVel;
			fBLeftVelErr = fBLeftVelR - fBLeftVel;
			fFRightVelErr = fFRightVelR - fFRightVel;
			fBRightVelErr = fBRightVelR - fBRightVel;

			PID(&fleft_vel);
			PID(&bleft_vel);
			PID(&fright_vel);
			PID(&bright_vel);
		}
	}
}

void SYSSystemInfiniteLoop(void)
{

}

void SYSSystemAct(void)
{
	//front left motor
	if (fFLeftVelU > 0){
		if(dev_cfg.motor1_dir == 0){
			SR.cast[0].bit0 = 0;
			SR.cast[0].bit5 = 1;
		} else {
			SR.cast[0].bit0 = 1;
			SR.cast[0].bit5 = 0;
		}
		P1DC1 = (unsigned int) fFLeftVelU;
	} else if (fFLeftVelU < 0){	/* backward */
		if(dev_cfg.motor1_dir == 0){
			SR.cast[0].bit0 = 1;
			SR.cast[0].bit5 = 0;
		} else {
			SR.cast[0].bit0 = 0;
			SR.cast[0].bit5 = 1;
		}
		P1DC1 = (unsigned int) -fFLeftVelU;
	} else {
		SR.cast[0].bit0 = 1;
		SR.cast[0].bit5 = 1;
		P1DC1 = 0;
	}

	//front right motor
	if (fFRightVelU > 0) {
		if (dev_cfg.motor2_dir == 0) {
			SR.cast[0].bit6 = 0;
			SR.cast[0].bit7 = 1;
		} else {
			SR.cast[0].bit6 = 1;
			SR.cast[0].bit7 = 0;
		}
		P1DC2 = (unsigned int) fFRightVelU;
	} else if (fFRightVelU < 0) {
		if (dev_cfg.motor2_dir == 0) {
			SR.cast[0].bit6 = 1;
			SR.cast[0].bit7 = 0;
		} else {
			SR.cast[0].bit6 = 0;
			SR.cast[0].bit7 = 1;
		}
		P1DC2 = (unsigned int) -fFRightVelU;
	} else {	/* stop */
		SR.cast[0].bit6 = 1;
		SR.cast[0].bit7 = 1;
		P1DC2 = 0;
	}

	//back left motor
	if (fBLeftVelU > 0){
		if (dev_cfg.motor3_dir == 0) {
			SR.cast[0].bit4 = 0;
			SR.cast[0].bit3 = 1;
		} else {
			SR.cast[0].bit4 = 1;
			SR.cast[0].bit3 = 0;
		}
		P1DC3 = (unsigned int) fBLeftVelU;
	} else if (fBLeftVelU < 0){
		if (dev_cfg.motor3_dir == 0) {
			SR.cast[0].bit4 = 1;
			SR.cast[0].bit3 = 0;
		} else {
			SR.cast[0].bit4 = 0;
			SR.cast[0].bit3 = 1;
		}
		P1DC3 = (unsigned int) -fBLeftVelU;
	} else {	/* stop */
		SR.cast[0].bit4 = 1;
		SR.cast[0].bit3 = 1;
		P1DC3 = 0;
	}

	//back right motor
	if (fBRightVelU > 0){	/* forward */
		if (dev_cfg.motor4_dir == 0){
			SR.cast[0].bit1 = 0;
			SR.cast[0].bit2 = 1;
		} else {
			SR.cast[0].bit1 = 1;
			SR.cast[0].bit2 = 0;
		}
		P1DC4 = (unsigned int) fBRightVelU;
	} else if (fBRightVelU < 0){	/* backward */
		if (dev_cfg.motor4_dir == 0){
			SR.cast[0].bit1 = 1;
			SR.cast[0].bit2 = 0;
		} else {
			SR.cast[0].bit1 = 0;
			SR.cast[0].bit2 = 1;
		}
		P1DC4 = (unsigned int) -fBRightVelU;
	} else {	/* stop */
		SR.cast[0].bit1 = 1;
		SR.cast[0].bit2 = 1;
		P1DC4 = 0;
	}

}
