/*********************************************/
/*          Include Header                   */
/*********************************************/


/* Standard C header */
#include <math.h>

/* Microchip Peripheral Library */

/* ROBOCON Library */

/* Internal Header */
#include "MODN.h"

/*********************************************/

/*********************************************/
/*          Variable                         */
/*********************************************/


/*********************************************/





/*********************************************/
/*           Subroutine Function             */
/*********************************************/
void MODNRobotBaseInit(unsigned char base, float d, float e, MODN_t *modn)
{
	modn->base = base;
	modn->d	= d;
	modn->e	= e;
}

void MODNRobotVelInit(float *x_vel, float *y_vel, float *w_vel, MODN_t *modn)
{
	modn->x_vel	= x_vel;
	modn->y_vel	= y_vel;
	modn->w_vel	= w_vel;
}

void MODNWheelVelInit(float *vel1, float *vel2, float *vel3, float *vel4, MODN_t *modn)
{
	modn->vel1	= vel1;
	modn->vel2	= vel2;
	modn->vel3	= vel3;
	modn->vel4	= vel4;
}

void MODN(MODN_t *modn)
{
	if(modn->base == MODN_FWD_OMNI) {
		*(modn->vel1) = *(modn->x_vel) * (0.70711) +  *(modn->y_vel) * (0.70711)  + *(modn->w_vel) * modn->d;
		*(modn->vel2) = *(modn->x_vel) * (-0.70711) +  *(modn->y_vel) * (0.70711) - *(modn->w_vel) * modn->d;
		*(modn->vel3) = *(modn->x_vel) * (-0.70711)  +  *(modn->y_vel) * (0.70711) + *(modn->w_vel) * modn->d;
		*(modn->vel4) = *(modn->x_vel) * (0.70711)  +  *(modn->y_vel) * (0.70711)  - *(modn->w_vel) * modn->d;
	}else if(modn->base == MODN_MECANUM){
		*(modn->vel1) = *(modn->y_vel)*(1.0) + *(modn->x_vel)*(1.0)  + *(modn->w_vel)/*(modn->d + modn->e)*/;
		*(modn->vel2) = *(modn->y_vel)*(1.0) + *(modn->x_vel)*(-1.0) - *(modn->w_vel)/*(modn->d + modn->e)*/;
		*(modn->vel3) = *(modn->y_vel)*(1.0) + *(modn->x_vel)*(-1.0) + *(modn->w_vel)/*(modn->d + modn->e)*/;
		*(modn->vel4) = *(modn->y_vel)*(1.0) + *(modn->x_vel)*(1.0)  - *(modn->w_vel)/*(modn->d + modn->e)*/;
	}
	else if (modn->base == MODN_TRI_OMNI){

		*(modn->vel1) =   *(modn->x_vel) * (1.0)  + *(modn->w_vel) * modn->d;
		*(modn->vel3) = *(modn->y_vel) * (0.866) +  *(modn->x_vel) * (-0.5) + *(modn->w_vel) * modn->d;
		*(modn->vel2) = *(modn->y_vel) * (0.866)  +  *(modn->x_vel) * (0.5) + *(modn->w_vel)*(-1.0) * modn->d;
	}

}

/*********************************************/
