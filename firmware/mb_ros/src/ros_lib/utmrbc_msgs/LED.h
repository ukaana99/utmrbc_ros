#ifndef _ROS_utmrbc_msgs_LED_h
#define _ROS_utmrbc_msgs_LED_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ros_lib/ros/msg.h>

namespace utmrbc_msgs
{

  class LED : public ros::Msg
  {
    public:
      typedef int32_t _led_type;
      _led_type led;
      typedef bool _signal_type;
      _signal_type signal;

    LED():
      led(0),
      signal(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      union {
        int32_t real;
        uint32_t base;
      } u_led;
      u_led.real = this->led;
      *(outbuffer + offset + 0) = (u_led.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_led.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_led.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_led.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->led);
      union {
        bool real;
        uint8_t base;
      } u_signal;
      u_signal.real = this->signal;
      *(outbuffer + offset + 0) = (u_signal.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->signal);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      union {
        int32_t real;
        uint32_t base;
      } u_led;
      u_led.base = 0;
      u_led.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_led.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_led.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_led.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->led = u_led.real;
      offset += sizeof(this->led);
      union {
        bool real;
        uint8_t base;
      } u_signal;
      u_signal.base = 0;
      u_signal.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->signal = u_signal.real;
      offset += sizeof(this->signal);
     return offset;
    }

    const char * getType(){ return "utmrbc_msgs/LED"; };
    const char * getMD5(){ return "0d48f2ba0b312d97024acd5dc86ebcb1"; };

  };

}
#endif
