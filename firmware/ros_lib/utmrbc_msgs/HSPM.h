#ifndef _ROS_utmrbc_msgs_HSPM_h
#define _ROS_utmrbc_msgs_HSPM_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace utmrbc_msgs
{

  class HSPM : public ros::Msg
  {
    public:
      typedef int32_t _hspm_type;
      _hspm_type hspm;
      typedef bool _signal_type;
      _signal_type signal;

    HSPM():
      hspm(0),
      signal(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      union {
        int32_t real;
        uint32_t base;
      } u_hspm;
      u_hspm.real = this->hspm;
      *(outbuffer + offset + 0) = (u_hspm.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_hspm.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_hspm.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_hspm.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->hspm);
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
      } u_hspm;
      u_hspm.base = 0;
      u_hspm.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_hspm.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_hspm.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_hspm.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->hspm = u_hspm.real;
      offset += sizeof(this->hspm);
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

    const char * getType(){ return "utmrbc_msgs/HSPM"; };
    const char * getMD5(){ return "5f0f63db1293a5a3d023345ba3cb3ac7"; };

  };

}
#endif
