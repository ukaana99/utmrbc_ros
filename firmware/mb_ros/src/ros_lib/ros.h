#ifndef _ROS_H_
#define _ROS_H_

#include "ros_lib/ros/node_handle.h"
#include "ros_lib/STM32Hardware.h"

namespace ros
{
	typedef NodeHandle_<STM32Hardware, 25, 25, 1024, 1024> NodeHandle;
}

#endif
