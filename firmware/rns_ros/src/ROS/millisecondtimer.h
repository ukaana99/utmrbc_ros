#ifndef _MILLISECONDTIMER_H_
#define _MILLISECONDTIMER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"

//void initialise(void);
void delay(uint32_t millis_);
uint32_t millis();
void reset();

#ifdef __cplusplus
}
#endif

#endif // _MILLISECONDTIMER_H_
