#ifndef _ROS_roborts_msgs_GimbalAngle_h
#define _ROS_roborts_msgs_GimbalAngle_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace roborts_msgs
{

  class GimbalAngle : public ros::Msg
  {
    public:
      typedef bool _yaw_mode_type;
      _yaw_mode_type yaw_mode;
      typedef bool _pitch_mode_type;
      _pitch_mode_type pitch_mode;
      typedef float _yaw_angle_type;
      _yaw_angle_type yaw_angle;
      typedef float _pitch_angle_type;
      _pitch_angle_type pitch_angle;

    GimbalAngle():
      yaw_mode(0),
      pitch_mode(0),
      yaw_angle(0),
      pitch_angle(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      union {
        bool real;
        uint8_t base;
      } u_yaw_mode;
      u_yaw_mode.real = this->yaw_mode;
      *(outbuffer + offset + 0) = (u_yaw_mode.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->yaw_mode);
      union {
        bool real;
        uint8_t base;
      } u_pitch_mode;
      u_pitch_mode.real = this->pitch_mode;
      *(outbuffer + offset + 0) = (u_pitch_mode.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->pitch_mode);
      offset += serializeAvrFloat64(outbuffer + offset, this->yaw_angle);
      offset += serializeAvrFloat64(outbuffer + offset, this->pitch_angle);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      union {
        bool real;
        uint8_t base;
      } u_yaw_mode;
      u_yaw_mode.base = 0;
      u_yaw_mode.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->yaw_mode = u_yaw_mode.real;
      offset += sizeof(this->yaw_mode);
      union {
        bool real;
        uint8_t base;
      } u_pitch_mode;
      u_pitch_mode.base = 0;
      u_pitch_mode.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->pitch_mode = u_pitch_mode.real;
      offset += sizeof(this->pitch_mode);
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->yaw_angle));
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->pitch_angle));
     return offset;
    }

    const char * getType(){ return "roborts_msgs/GimbalAngle"; };
    const char * getMD5(){ return "b3949b2683b910b23b0cb34879284a9b"; };

  };

}
#endif
