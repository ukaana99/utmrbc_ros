#include <string>
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Joy.h>
#include <utmrbc_msgs/LED.h>
#include <utmrbc_msgs/DigitalInput.h>
#include <utmrbc_msgs/BDC.h>
#include <utmrbc_msgs/HSPM.h>

// #define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

class Bot1_PS4
{
private:
  ros::NodeHandle nh;

  ros::Publisher cmd_vel_pub;
  ros::Publisher led_pub;
  ros::Publisher bdc_pub;
  ros::Publisher hspm_pub;

  ros::Subscriber ps4_sub;
  ros::Subscriber dinp_sub;

  ros::Timer timer;

  std::string pubName;

  /* button states data */
  int stateSquare, stateCross, stateCircle, stateTriangle;

  /* raw data */
  double leftStickY, leftStickX, rightStickX, rightStickY, l2, r2;
  int arrowsX, arrowsY, buttonSquare, buttonCross, buttonCircle, buttonTriangle,
      buttonStart, buttonShare, buttonOption, buttonTouch,
      leftStickButton, rightStickButton, l1, r1;

  /* rosparams */
  double linear_scale, angular_scale;

  /* flags */

  void PS4Callback(const sensor_msgs::Joy::ConstPtr &ps4);
  void DInpCallback(const utmrbc_msgs::DigitalInput::ConstPtr &dinp_msg);
  void timerCallback(const ros::TimerEvent &event);
  void printRaw();

public:
  Bot1_PS4();
  void publishCommandVelocity();
  void publishLED(int led, bool signal);
  void publishBDC(int bdc, int32_t value);
  void publishHSPM(int hspm, bool signal);
};

Bot1_PS4::Bot1_PS4()
{
  stateSquare = stateCross = stateCircle = stateTriangle = 0;

  nh.param("scale_angular", angular_scale, angular_scale);
  nh.param("scale_linear", linear_scale, linear_scale);
  nh.param<std::string>("pub_topic", this->pubName, "cmd_vel");

  cmd_vel_pub = nh.advertise<geometry_msgs::Twist>(pubName.c_str(), 1);
  led_pub = nh.advertise<utmrbc_msgs::LED>("led", 1);
  bdc_pub = nh.advertise<utmrbc_msgs::BDC>("bdc", 1);
  hspm_pub = nh.advertise<utmrbc_msgs::HSPM>("hspm", 1);

  ps4_sub = nh.subscribe<sensor_msgs::Joy>("joy", 1, &Bot1_PS4::PS4Callback, this);
  dinp_sub = nh.subscribe<utmrbc_msgs::DigitalInput>("dinp", 1, &Bot1_PS4::DInpCallback, this);

  timer = nh.createTimer(ros::Duration(0.1), &Bot1_PS4::timerCallback, this);
  timer.stop();
}

void Bot1_PS4::timerCallback(const ros::TimerEvent &event)
{
  // ROS_INFO("Callback 1 triggered");
}

void Bot1_PS4::PS4Callback(const sensor_msgs::Joy::ConstPtr &ps4)
{
  this->buttonSquare = ps4->buttons[0];
  this->buttonCross = ps4->buttons[1];
  this->buttonCircle = ps4->buttons[2];
  this->buttonTriangle = ps4->buttons[3];
  this->buttonStart = ps4->buttons[12];
  this->buttonShare = ps4->buttons[8];
  this->buttonOption = ps4->buttons[9];
  this->buttonTouch = ps4->buttons[13];
  this->l1 = ps4->buttons[4];
  this->r1 = ps4->buttons[5];

  this->arrowsX = -ps4->axes[9];
  this->arrowsY = ps4->axes[10];
  this->l2 = ps4->axes[3];
  this->r2 = ps4->axes[4];

  this->leftStickX = -ps4->axes[0];
  this->leftStickY = ps4->axes[1];
  this->rightStickX = -ps4->axes[2];
  this->rightStickY = ps4->axes[5];
  this->leftStickButton = ps4->buttons[10];
  this->rightStickButton = ps4->buttons[11];

  if (stateSquare != buttonSquare)
  {
    this->stateSquare = this->buttonSquare;
    if (stateSquare)
      publishBDC(7, 20000);
    else
      publishBDC(7, 0);
  }

  if (stateCross != buttonCross)
  {
    this->stateCross = this->buttonCross;
    if (stateCross)
      publishBDC(7, -20000);
    else
      publishBDC(7, 0);
  }

  if (stateCircle != buttonCircle)
  {
    this->stateCircle = this->buttonCircle;
    publishHSPM(16, stateCircle);
  }

  if (stateTriangle != buttonTriangle)
  {
    this->stateTriangle = this->buttonTriangle;
    publishLED(2, stateTriangle);
  }

  // Bot1_PS4::printRaw();
}

void Bot1_PS4::DInpCallback(const utmrbc_msgs::DigitalInput::ConstPtr &dinp_msg)
{
  // ROS_INFO("dinp%d triggered", dinp_msg->pin);
}

void Bot1_PS4::publishCommandVelocity()
{
  geometry_msgs::Twist vel_msg;
  vel_msg.linear.x = linear_scale * leftStickX;
  vel_msg.linear.y = linear_scale * leftStickY;
  vel_msg.angular.z = (angular_scale / 2) * (l2 - r2);
  cmd_vel_pub.publish(vel_msg);
}

void Bot1_PS4::publishLED(int led, bool signal)
{
  utmrbc_msgs::LED led_msg;

  led_msg.led = led;
  led_msg.signal = signal;
  led_pub.publish(led_msg);
}

void Bot1_PS4::publishBDC(int bdc, int32_t value)
{
  utmrbc_msgs::BDC bdc_msg;

  bdc_msg.bdc = bdc;
  bdc_msg.value = value;
  bdc_pub.publish(bdc_msg);
}

void Bot1_PS4::publishHSPM(int hspm, bool signal)
{
  utmrbc_msgs::HSPM hspm_msg;

  hspm_msg.hspm = hspm;
  hspm_msg.signal = signal;
  hspm_pub.publish(hspm_msg);
}

void Bot1_PS4::printRaw()
{
  ROS_INFO("Square        : %i", this->buttonSquare);
  ROS_INFO("Cross         : %i", this->buttonCross);
  ROS_INFO("Circle        : %i", this->buttonCircle);
  ROS_INFO("Triangle      : %i", this->buttonTriangle);
  ROS_INFO("Left/Right    : %i", this->arrowsX);
  ROS_INFO("Up/Down       : %i", this->arrowsY);
  ROS_INFO("Start         : %i", this->buttonStart);
  ROS_INFO("Share         : %i", this->buttonShare);
  ROS_INFO("Option        : %i", this->buttonOption);
  ROS_INFO("Touch         : %i", this->buttonTouch);
  ROS_INFO("L1            : %i", this->l1);
  ROS_INFO("R1            : %i", this->r1);
  ROS_INFO("L2            : %f", this->l2);
  ROS_INFO("R2            : %f", this->r2);
  ROS_INFO("LeftStick X   : %f", this->leftStickX);
  ROS_INFO("LeftStick Y   : %f", this->leftStickY);
  ROS_INFO("LeftStickBtn  : %i", this->leftStickButton);
  ROS_INFO("RightStick X  : %f", this->rightStickX);
  ROS_INFO("RightStick Y  : %f", this->rightStickY);
  ROS_INFO("RightStickBtn : %i", this->rightStickButton);
  ROS_INFO("\n");
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "bot1_teleop_ps4");
  Bot1_PS4 bot1_ps4;

  ros::Rate loop_rate(10);

  while (1)
  {
    bot1_ps4.publishCommandVelocity();
    ros::spinOnce();
    loop_rate.sleep();
  }
}