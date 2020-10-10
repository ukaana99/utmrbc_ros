#ifndef _ROS_utmrbc_msgs_BDC_h
#define _ROS_utmrbc_msgs_BDC_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace utmrbc_msgs
{

  class BDC : public ros::Msg
  {
    public:
      typedef int32_t _bdc_type;
      _bdc_type bdc;
      typedef int32_t _value_type;
      _value_type value;

    BDC():
      bdc(0),
      value(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      union {
        int32_t real;
        uint32_t base;
      } u_bdc;
      u_bdc.real = this->bdc;
      *(outbuffer + offset + 0) = (u_bdc.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_bdc.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_bdc.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_bdc.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->bdc);
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
      } u_bdc;
      u_bdc.base = 0;
      u_bdc.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_bdc.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_bdc.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_bdc.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->bdc = u_bdc.real;
      offset += sizeof(this->bdc);
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

    const char * getType(){ return "utmrbc_msgs/BDC"; };
    const char * getMD5(){ return "2918c8f84390a5f9c92a1c6a1a2d5dd8"; };

  };

}
#endif
