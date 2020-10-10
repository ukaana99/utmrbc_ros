#ifndef _ROS_SERVICE_FricWhl_h
#define _ROS_SERVICE_FricWhl_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace roborts_msgs
{

static const char FRICWHL[] = "roborts_msgs/FricWhl";

  class FricWhlRequest : public ros::Msg
  {
    public:
      typedef bool _open_type;
      _open_type open;

    FricWhlRequest():
      open(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      union {
        bool real;
        uint8_t base;
      } u_open;
      u_open.real = this->open;
      *(outbuffer + offset + 0) = (u_open.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->open);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      union {
        bool real;
        uint8_t base;
      } u_open;
      u_open.base = 0;
      u_open.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->open = u_open.real;
      offset += sizeof(this->open);
     return offset;
    }

    const char * getType(){ return FRICWHL; };
    const char * getMD5(){ return "b06b243ca7fb95b6d7d5a6ac2b1f7c85"; };

  };

  class FricWhlResponse : public ros::Msg
  {
    public:
      typedef bool _received_type;
      _received_type received;

    FricWhlResponse():
      received(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      union {
        bool real;
        uint8_t base;
      } u_received;
      u_received.real = this->received;
      *(outbuffer + offset + 0) = (u_received.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->received);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      union {
        bool real;
        uint8_t base;
      } u_received;
      u_received.base = 0;
      u_received.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->received = u_received.real;
      offset += sizeof(this->received);
     return offset;
    }

    const char * getType(){ return FRICWHL; };
    const char * getMD5(){ return "dd4152e077925db952c78baadb1e48b7"; };

  };

  class FricWhl {
    public:
    typedef FricWhlRequest Request;
    typedef FricWhlResponse Response;
  };

}
#endif
