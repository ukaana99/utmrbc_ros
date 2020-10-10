
#ifndef COMMON_C_
#define COMMON_C_


#include "common.h"
#include "math.h"

void System_Start(void)
{
//	USBH_Init(&USB_OTG_Core, USB_OTG_FS_CORE_ID, &USB_Host, &USBH_BTD_cb, &BTD_USR_cb);
//	PS4BTInit(&PS4BT, 1); //1 for pairing mode and 0 for non-pairing mode
//	while(!PS4BT.connected){
//		USBH_Process(&USB_OTG_Core, &USB_Host);
//	}
//	QEIReset(QEI1);
//	QEIReset(QEI4);
//	QEIReset(QEI6);
//	BIOSDelay(500);
//	RNSInit(CAN1, &rns);
//	BIOSDelay(500);
//	RNSStop(&rns);
//	RNSSet(&rns, RNS_RESET_POS);
}

void System_Run(void)
{

}

void RNS_config(CAN_TypeDef* CANx)
{
//	RNSInit(CANx, &rns);
////	BIOSDelay(500);
//	RNSSet(&rns, RNS_DEVICE_CONFIG,(float)0b00000011 ,(float)fwd_omni,(float)roboconPID);
//	RNSSet(&rns,RNS_X_Y_ENC_CONFIG ,  0.05/2000.0*3.142, 2.0,0.05/2000.0*3.142,2.0); //1.0 for nonswap , 2.0 for swap
//	RNSSet(&rns, RNS_F_KCD_PTD, 268.2/259.9,0.125*3.142/268.2);
//	RNSSet(&rns, RNS_B_KCD_PTD, 258.9/260.4,0.125*3.142/258.9);
//
//	RNSSet(&rns, RNS_F_LEFT_VEL_SATEU, 1.0, 1.0/7.217, 19999.0);
//	RNSSet(&rns, RNS_F_LEFT_VEL_PID, 0.59, 7.0,0.015);
//
//	RNSSet(&rns, RNS_F_RIGHT_VEL_SATEU, 1.0, 1.0/7.574, 19999.0);
//	RNSSet(&rns, RNS_F_RIGHT_VEL_PID, 0.45, 6.0, 0.011);
//
//	RNSSet(&rns, RNS_B_LEFT_VEL_SATEU, 1.0, 1.0/7.76, 19999.0);
//	RNSSet(&rns, RNS_B_LEFT_VEL_PID, 0.66, 8.0, 0.013);
//
//	RNSSet(&rns, RNS_B_RIGHT_VEL_SATEU, 1.0, 1.0/7.913, 19999.0);
//	RNSSet(&rns, RNS_B_RIGHT_VEL_PID, 0.4, 6.0, 0.018);
//
//	RNSSet(&rns,RNS_LF_LSA_POS,(float)AT_FRONT,(float)AT_LEFT,(float)AT_BACK,(float)AT_RIGHT);
//
//	RNSSet(&rns,RNS_PPInit);

//--------------------------------------------------------------------------------------------------
//		Default
//--------------------------------------------------------------------------------------------------
//	RNSSet(&rns,RNS_PPPathPID,1.0,0.0,0.4,&rns);
//	RNSSet(&rns,RNS_PPEndPID,1.0,0.0,0.4,&rns);
//	RNSSet(&rns,RNS_PPZPID,1.0,0.0,0.4,&rns);

//	RNSSet(&rns,RNS_PPPathPID,0.7,0.6,0.3,&rns);
//	RNSSet(&rns,RNS_PPEndPID,0.7,0.6,0.3,&rns);
//	RNSSet(&rns,RNS_PPZPID,0.07,0.0,0.0,&rns);

}

#endif /* COMMON_C_ */
