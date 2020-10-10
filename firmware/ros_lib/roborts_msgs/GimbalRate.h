#ifndef _ROS_roborts_msgs_GimbalRate_h
#define _ROS_roborts_msgs_GimbalRate_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace roborts_msgs
{

  class GimbalRate : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef float _pitch_rate_type;
      _pitch_rate_type pitch_rate;
      typedef float _yaw_rate_type;
      _yaw_rate_type yaw_rate;

    GimbalRate():
      header(),
      pitch_rate(0),
      yaw_rate(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      offset += serializeAvrFloat64(outbuffer + offset, this->pitch_rate);
      offset += serializeAvrFloat64(outbuffer + offset, this->yaw_rate);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->pitch_rate));
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->yaw_rate));
     return offset;
    }

    const char * getType(){ return "roborts_msgs/GimbalRate"; };
    const char * getMD5(){ return "525d7f3e8f2df5fe6c116264d2afd49a"; };

  };

}
#endif
