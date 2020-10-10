#ifndef _ROS_ros_threading_example_Pose2D_h
#define _ROS_ros_threading_example_Pose2D_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace ros_threading_example
{

  class Pose2D : public ros::Msg
  {
    public:
      typedef float _xPose_type;
      _xPose_type xPose;
      typedef float _yPose_type;
      _yPose_type yPose;

    Pose2D():
      xPose(0),
      yPose(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += serializeAvrFloat64(outbuffer + offset, this->xPose);
      offset += serializeAvrFloat64(outbuffer + offset, this->yPose);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->xPose));
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->yPose));
     return offset;
    }

    const char * getType(){ return "ros_threading_example/Pose2D"; };
    const char * getMD5(){ return "d402bbb5a81624685655b53f88cd22de"; };

  };

}
#endif
