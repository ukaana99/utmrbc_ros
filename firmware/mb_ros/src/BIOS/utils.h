#ifndef BIOS_UTILS_H_
#define BIOS_UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************************
 * 		Include Libraries 			   *
 **************************************/
#include "system.h"

/**************************************************
 * 		Define						  			  *
 *************************************************/
#define BUFSIZE (sizeof(long) * 8 + 1)

/**************************************************
 * 		Function Prototype			  			  *
 *************************************************/
char *ltoa(long N, char *str, int base);

#ifdef __cplusplus
}
#endif

#endif /* BIOS_UTILS_H_ */
