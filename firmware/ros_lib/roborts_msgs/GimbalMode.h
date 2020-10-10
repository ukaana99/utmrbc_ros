#ifndef _ROS_SERVICE_GimbalMode_h
#define _ROS_SERVICE_GimbalMode_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace roborts_msgs
{

static const char GIMBALMODE[] = "roborts_msgs/GimbalMode";

  class GimbalModeRequest : public ros::Msg
  {
    public:
      typedef uint8_t _gimbal_mode_type;
      _gimbal_mode_type gimbal_mode;

    GimbalModeRequest():
      gimbal_mode(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->gimbal_mode >> (8 * 0)) & 0xFF;
      offset += sizeof(this->gimbal_mode);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      this->gimbal_mode =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->gimbal_mode);
     return offset;
    }

    const char * getType(){ return GIMBALMODE; };
    const char * getMD5(){ return "c87f8568799ca64a2eed82fa85fa02bb"; };

  };

  class GimbalModeResponse : public ros::Msg
  {
    public:
      typedef bool _received_type;
      _received_type received;

    GimbalModeResponse():
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

    const char * getType(){ return GIMBALMODE; };
    const char * getMD5(){ return "dd4152e077925db952c78baadb1e48b7"; };

  };

  class GimbalMode {
    public:
    typedef GimbalModeRequest Request;
    typedef GimbalModeResponse Response;
  };

}
#endif
