#ifndef MODN_H
#define MODN_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************/
/*          Include Header                   */
/*********************************************/

/*********************************************/




/*********************************************/
/*          Define                           */
/*********************************************/

/*********************************************/




/*********************************************/
/*          Enumarator                       */
/*********************************************/
enum{MODN_FWD_OMNI = 0, MODN_TRI_OMNI, MODN_PI_OMNI,MODN_MECANUM};
/*********************************************/




/*********************************************/
/*          Variable                         */
/*********************************************/
typedef struct {
	float *x_vel;
	float *y_vel;
	float *w_vel;
	float *vel1;
	float *vel2;
	float *vel3;
	float *vel4;
	float d;
	float e;
	unsigned char base;
}MODN_t;

/*********************************************/




/*********************************************/
/*           Function Prototype              */
/*********************************************/
void MODNRobotBaseInit(unsigned char base, float d, float e, MODN_t *modn);
void MODNRobotVelInit(float *x_vel, float *y_vel, float *w_vel, MODN_t *modn);
void MODNWheelVelInit(float *vel1, float *vel2, float *vel3, float *vel4, MODN_t *modn);
void MODN(MODN_t *modn);
/*********************************************/

#ifdef __cplusplus
}
#endif

#endif
