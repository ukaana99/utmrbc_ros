#ifndef _ROS_utmrbc_msgs_AnalogInput_h
#define _ROS_utmrbc_msgs_AnalogInput_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace utmrbc_msgs
{

  class AnalogInput : public ros::Msg
  {
    public:
      typedef int32_t _pin_type;
      _pin_type pin;
      typedef int32_t _value_type;
      _value_type value;

    AnalogInput():
      pin(0),
      value(0)
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
        int32_t real;
        uint32_t base;
      } u_value;
      u_value.real = this->value;
      *(outbuffer + offset + 0) = (u_value.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_value.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_value.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_value.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->value);
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
        int32_t real;
        uint32_t base;
      } u_value;
      u_value.base = 0;
      u_value.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_value.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_value.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_value.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->value = u_value.real;
      offset += sizeof(this->value);
     return offset;
    }

    const char * getType(){ return "utmrbc_msgs/AnalogInput"; };
    const char * getMD5(){ return "0d2137c3882cd2c55a0c95412d364629"; };

  };

}
#endif
