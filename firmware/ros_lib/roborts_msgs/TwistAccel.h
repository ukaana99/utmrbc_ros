#ifndef _ROS_roborts_msgs_TwistAccel_h
#define _ROS_roborts_msgs_TwistAccel_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "geometry_msgs/Twist.h"
#include "geometry_msgs/Accel.h"

namespace roborts_msgs
{

  class TwistAccel : public ros::Msg
  {
    public:
      typedef geometry_msgs::Twist _twist_type;
      _twist_type twist;
      typedef geometry_msgs::Accel _accel_type;
      _accel_type accel;

    TwistAccel():
      twist(),
      accel()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += this->twist.serialize(outbuffer + offset);
      offset += this->accel.serialize(outbuffer + offset);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += this->twist.deserialize(inbuffer + offset);
      offset += this->accel.deserialize(inbuffer + offset);
     return offset;
    }

    const char * getType(){ return "roborts_msgs/TwistAccel"; };
    const char * getMD5(){ return "644e3af95741e8ff8b733f2fd1e014d8"; };

  };

}
#endif
