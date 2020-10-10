#ifndef CONFIG_H_
#define CONFIG_H_

#include <errno.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>
#include "stm32f4xx.h"
#include "ROS/ring_buffer.h"
#include "ROS/millisecondtimer.h"

/** --------Serial Config-------- **/
typedef enum {
	SERIAL2 = 0,
	SERIAL3 = 1,
	SERIAL4 = 2,
	SERIAL_END = 3
}Serial_TypeDef; 

#define SERIALn							3

#define TXBUF_SIZE        1024
#define RXBUF_SIZE        1024

#endif // CONFIG_H_
