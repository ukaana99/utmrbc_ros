/* 
 * SimpleKalmanFilter - a Kalman Filter implementation for single variable models.
 * Created by Denys Sene, January, 1, 2017.
 * Released under MIT License - see LICENSE file for details.
 */

#ifndef SimpleKalmanFilter_h
#define SimpleKalmanFilter_h

#ifdef __cplusplus
extern "C" {
#endif

#include "BIOS/bios.h"
  
typedef struct{
  float _err_measure;
  float _err_estimate;
  float _q;
  float _current_estimate;
  float _last_estimate;
  float _kalman_gain;
}KALMANFILTER_t;

void KalmanFilterInit(float, float, float, KALMANFILTER_t*);
float updateEstimate(float, KALMANFILTER_t*);
void setMeasurementError(float, KALMANFILTER_t*);
void setEstimateError(float, KALMANFILTER_t*);
void setProcessNoise(float, KALMANFILTER_t*);
float getKalmanGain(KALMANFILTER_t*);

#ifdef __cplusplus
}
#endif

#endif
