#ifndef BOT1_RNS_CORE_H_
#define BOT1_RNS_CORE_H_

#include "ROS/bot1.h"
#include <ros_lib/ros.h>
#include <ros_lib/ros/time.h>
#include <ros_lib/std_msgs/Bool.h>
#include <ros_lib/std_msgs/String.h>
#include <ros_lib/std_msgs/Empty.h>
#include <ros_lib/std_msgs/Int32.h>
#include <ros_lib/std_msgs/Float64.h>

#include <ros_lib/geometry_msgs/Twist.h>
#include <ros_lib/geometry_msgs/Vector3.h>
#include <ros_lib/nav_msgs/Odometry.h>
#include <ros_lib/sensor_msgs/Imu.h>
#include <ros_lib/tf/tf.h>
#include <ros_lib/tf/transform_broadcaster.h>
// #include <ros_lib/sensor_msgs/JointState.h>
// #include <ros_lib/sensor_msgs/BatteryState.h>
// #include <ros_lib/sensor_msgs/MagneticField.h>

#include "include.h"

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
void ROS_Init(void);
void ROS_Update(void);
void initOdom(void);
// void initJointStates(void);
void updateOdometry(void);
void updateTF(geometry_msgs::TransformStamped &odom_tf);
void updateTFPrefix(void);

// Publisher function prototypes
void publishImuMsg(void);
void publishDriveInformation(void);

// Callback function prototypes
void cmdVelCallback(const geometry_msgs::Twist &cmd_vel_msg);

// ROS timer
ros::Time rosNow(void);
void updateTime(void);

#endif // BOT1_RNS_CORE_H_
