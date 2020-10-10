#ifndef BOT1_MB_CORE_H_
#define BOT1_MB_CORE_H_

#include "ROS/bot1.h"
#include <ros_lib/ros.h>
#include <ros_lib/ros/time.h>
#include <ros_lib/std_msgs/Bool.h>
#include <ros_lib/std_msgs/String.h>
#include <ros_lib/std_msgs/Empty.h>
#include <ros_lib/std_msgs/Int32.h>
#include <ros_lib/std_msgs/Float64.h>

#include <ros_lib/nav_msgs/Odometry.h>
#include <ros_lib/utmrbc_msgs/LED.h>
#include <ros_lib/utmrbc_msgs/InputPin.h>
#include <ros_lib/utmrbc_msgs/BDC.h>
#include <ros_lib/utmrbc_msgs/HSPM.h>

#include "main.h"

#include <math.h>

#define constrain(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))

#define DEG2RAD(x) (x * 0.01745329252) // *PI/180
#define RAD2DEG(x) (x * 57.2957795131) // *180/PI

#define PUBLISH_FREQUENCY 20
#define FIRMWARE_VER "1.0.0"

//#define CONTROL_MOTOR_SPEED_FREQUENCY          30   //hz
//#define IMU_PUBLISH_FREQUENCY                  200  //hz
//#define CMD_VEL_PUBLISH_FREQUENCY              30   //hz
//#define DRIVE_INFORMATION_PUBLISH_FREQUENCY    30   //hz
//#define VERSION_INFORMATION_PUBLISH_FREQUENCY  1    //hz
//#define DEBUG_LOG_FREQUENCY                    10   //hz

/*******************************************************************************
* ROS function protype
*******************************************************************************/
void ROS_Init();
void ROS_Update();

// Publisher function prototypes
void publishInputPin();

// Callback function prototypes
void LEDCallback(const utmrbc_msgs::LED &led_msg);
void BDCCallback(const utmrbc_msgs::BDC &bdc_msg);
void HSPMCallback(const utmrbc_msgs::HSPM &hspm_msg);

// ROS timer
ros::Time rosNow(void);
void updateTime(void);

#endif // BOT1_MB_CORE_H_
