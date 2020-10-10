#ifndef _ROS_SERVICE_ShootCmd_h
#define _ROS_SERVICE_ShootCmd_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace roborts_msgs
{

static const char SHOOTCMD[] = "roborts_msgs/ShootCmd";

  class ShootCmdRequest : public ros::Msg
  {
    public:
      typedef uint8_t _mode_type;
      _mode_type mode;
      typedef uint8_t _number_type;
      _number_type number;
      enum { STOP =  0 };
      enum { ONCE =  1 };
      enum { CONTINUOUS =  2 };

    ShootCmdRequest():
      mode(0),
      number(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->mode >> (8 * 0)) & 0xFF;
      offset += sizeof(this->mode);
      *(outbuffer + offset + 0) = (this->number >> (8 * 0)) & 0xFF;
      offset += sizeof(this->number);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      this->mode =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->mode);
      this->number =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->number);
     return offset;
    }

    const char * getType(){ return SHOOTCMD; };
    const char * getMD5(){ return "e60d1d1007f97ae14e2d2f584fbf2502"; };

  };

  class ShootCmdResponse : public ros::Msg
  {
    public:
      typedef bool _received_type;
      _received_type received;

    ShootCmdResponse():
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

    const char * getType(){ return SHOOTCMD; };
    const char * getMD5(){ return "dd4152e077925db952c78baadb1e48b7"; };

  };

  class ShootCmd {
    public:
    typedef ShootCmdRequest Request;
    typedef ShootCmdResponse Response;
  };

}
#endif
