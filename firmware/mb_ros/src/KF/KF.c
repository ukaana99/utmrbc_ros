/* 
 * SimpleKalmanFilter - a Kalman Filter implementation for single variable models.
 * Created by Denys Sene, January, 1, 2017.
 * Released under MIT License - see LICENSE file for details.
 */
#include <KF/KF.h>
#include <math.h>
//#include "include.h"
/*
 * 	uint16_t adcValue[6];
	KALMANFILTER_t adckf[6];
	float filtValue[6];
	uint16_t filtered[6];
 *
 * 	for(int i = 0; i < 6; i++)
		KalmanFilterInit(5.0, 2.0, 0.65, &adckf[i]);
 *
 *
 * ***************************** interrupt ******************************
 *
 * for(int i = 0; i < 6; i++){
			filtValue[i] = adcValue[i]/1000.0;
			filtValue[i] = updateEstimate(filtValue[i], &adckf[i]);
			filtered[i] = filtValue[i]*1000;
	}
 *
 * */

void KalmanFilterInit(float mea_e, float est_e, float q, KALMANFILTER_t *kalmanfilter)
{
	kalmanfilter->_err_measure = mea_e;
	kalmanfilter->_err_estimate = est_e;
	kalmanfilter->_q = q;
}

float updateEstimate(float mea, KALMANFILTER_t *kalmanfilter)
{
	kalmanfilter->_kalman_gain = kalmanfilter->_err_estimate / (kalmanfilter->_err_estimate + kalmanfilter->_err_measure);
	kalmanfilter->_current_estimate = kalmanfilter->_last_estimate + kalmanfilter->_kalman_gain * (mea - kalmanfilter->_last_estimate);
	kalmanfilter->_err_estimate = (1.0 - kalmanfilter->_kalman_gain) * kalmanfilter->_err_estimate + fabs(kalmanfilter->_last_estimate - kalmanfilter->_current_estimate) * kalmanfilter->_q;
	kalmanfilter->_last_estimate = kalmanfilter->_current_estimate;

	return kalmanfilter->_current_estimate;
}

void setMeasurementError(float mea_e, KALMANFILTER_t *kalmanfilter)
{
	kalmanfilter->_err_measure = mea_e;
}

void setEstimateError(float est_e, KALMANFILTER_t *kalmanfilter)
{
	kalmanfilter->_err_estimate = est_e;
}

void setProcessNoise(float q, KALMANFILTER_t *kalmanfilter)
{
	kalmanfilter->_q = q;
}

float getKalmanGain(KALMANFILTER_t *kalmanfilter) {
	return kalmanfilter->_kalman_gain;
}

