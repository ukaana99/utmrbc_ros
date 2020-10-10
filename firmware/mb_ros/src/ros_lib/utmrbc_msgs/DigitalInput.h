#ifndef _ROS_utmrbc_msgs_DigitalInput_h
#define _ROS_utmrbc_msgs_DigitalInput_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros_lib/ros/msg.h"

namespace utmrbc_msgs
{

  class DigitalInput : public ros::Msg
  {
    public:
      typedef int32_t _pin_type;
      _pin_type pin;
      typedef bool _signal_type;
      _signal_type signal;

    DigitalInput():
      pin(0),
      signal(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      union {
        int32_t real;
        uint32_t base;
      } u_pin;
      u_pin.real = this->pin;
      *(outbuffer + offset + 0) = (u_pin.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_pin.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_pin.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_pin.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->pin);
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
      } u_pin;
      u_pin.base = 0;
      u_pin.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_pin.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_pin.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_pin.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->pin = u_pin.real;
      offset += sizeof(this->pin);
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

    const char * getType(){ return "utmrbc_msgs/DigitalInput"; };
    const char * getMD5(){ return "359ffdef4df401a3c881b8043c9e8293"; };

  };

}
#endif
