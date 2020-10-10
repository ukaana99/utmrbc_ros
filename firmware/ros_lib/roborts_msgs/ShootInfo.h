#ifndef _ROS_roborts_msgs_ShootInfo_h
#define _ROS_roborts_msgs_ShootInfo_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace roborts_msgs
{

  class ShootInfo : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef int16_t _remain_bullet_type;
      _remain_bullet_type remain_bullet;
      typedef int16_t _sent_bullet_type;
      _sent_bullet_type sent_bullet;
      typedef bool _fric_wheel_run_type;
      _fric_wheel_run_type fric_wheel_run;

    ShootInfo():
      header(),
      remain_bullet(0),
      sent_bullet(0),
      fric_wheel_run(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      union {
        int16_t real;
        uint16_t base;
      } u_remain_bullet;
      u_remain_bullet.real = this->remain_bullet;
      *(outbuffer + offset + 0) = (u_remain_bullet.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_remain_bullet.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->remain_bullet);
      union {
        int16_t real;
        uint16_t base;
      } u_sent_bullet;
      u_sent_bullet.real = this->sent_bullet;
      *(outbuffer + offset + 0) = (u_sent_bullet.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_sent_bullet.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->sent_bullet);
      union {
        bool real;
        uint8_t base;
      } u_fric_wheel_run;
      u_fric_wheel_run.real = this->fric_wheel_run;
      *(outbuffer + offset + 0) = (u_fric_wheel_run.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->fric_wheel_run);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      union {
        int16_t real;
        uint16_t base;
      } u_remain_bullet;
      u_remain_bullet.base = 0;
      u_remain_bullet.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_remain_bullet.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->remain_bullet = u_remain_bullet.real;
      offset += sizeof(this->remain_bullet);
      union {
        int16_t real;
        uint16_t base;
      } u_sent_bullet;
      u_sent_bullet.base = 0;
      u_sent_bullet.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_sent_bullet.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->sent_bullet = u_sent_bullet.real;
      offset += sizeof(this->sent_bullet);
      union {
        bool real;
        uint8_t base;
      } u_fric_wheel_run;
      u_fric_wheel_run.base = 0;
      u_fric_wheel_run.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->fric_wheel_run = u_fric_wheel_run.real;
      offset += sizeof(this->fric_wheel_run);
     return offset;
    }

    const char * getType(){ return "roborts_msgs/ShootInfo"; };
    const char * getMD5(){ return "c71a8c5d03eafbb3be3f4aa0410b8278"; };

  };

}
#endif
