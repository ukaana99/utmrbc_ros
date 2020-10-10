#include "include.h"

/* ABT gains */
float fFLeftPosGain[3] = {0.8, 0.4, 0.2};
float fFRightPosGain[3] = {0.8, 0.4, 0.2};
float fBLeftPosGain[3] = {0.8, 0.4, 0.2};
float fBRightPosGain[3] = {0.8, 0.4, 0.2};
float fXPosGain[3] = {0.8, 0.4, 0.2};
float fYPosGain[3] = {0.8, 0.4, 0.2};

/* U_MAX,E_MAX,EC_MAX,ES_MAX,KP_B,KI_B,KD_B,KP_P,KI_P,KD_P */
float fFuzFLeftVG[10] = {19999.0, 3.0, 1.5, 6.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
float fFuzFRightVG[10] = {19999.0, 3.0, 1.5, 6.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
float fFuzBLeftVG[10] = {19999.0, 3.0, 1.5, 6.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
float fFuzBRightVG[10] = {19999.0, 3.0, 1.5, 6.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

/* SAT, KE, KU, KP, KI, KD, KN */
float fFLeftVG[7] = {1.0, 1.0, 19999.0, 0.05, 1.0, 0.03, 60.0};
float fFRightVG[7] = {1.0, 1.0, 19999.0, 0.05, 1.0, 0.03, 60.0};
float fBLeftVG[7] = {1.0, 1.0, 19999.0, 0.05, 1.0, 0.03, 60.0};
float fBRightVG[7] = {1.0, 1.0, 19999.0, 0.05, 1.0, 0.03, 60.0};

float fLFDistG[7] = {1.0, 1.0/3.0, 1.0, 1.0, 0.0, 0.4, 30.0};
float fRotateG[7] = {1.0, 1.0/30.0, 0.75, 0.4, 0.0, 0.35, 30.0};
float fFwdG[7] = {1.0, 1.0/13.0, 30.0, 0.95, 0.0, 1.2, 30.0};

float fAngleG[7] = {1.0, 1.0/180.0, 1.0, 0.05, 1.0, 0.03, 60.0};

//float fFPtd = 1.0, fFKcd = 1.0, fBPtd = 1.0, fBKcd = 1.0;
//float xPtd = 1.0, yPtd = 1.0;
//
//int Dir = DIR_FRONT;

