#ifndef _ROS_roborts_msgs_ShootState_h
#define _ROS_roborts_msgs_ShootState_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace roborts_msgs
{

  class ShootState : public ros::Msg
  {
    public:
      typedef int32_t _single_shoot_type;
      _single_shoot_type single_shoot;
      typedef int32_t _continue_shoot_type;
      _continue_shoot_type continue_shoot;
      typedef int32_t _run_friction_whell_type;
      _run_friction_whell_type run_friction_whell;
      typedef int32_t _friction_whell_speed_type;
      _friction_whell_speed_type friction_whell_speed;

    ShootState():
      single_shoot(0),
      continue_shoot(0),
      run_friction_whell(0),
      friction_whell_speed(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      union {
        int32_t real;
        uint32_t base;
      } u_single_shoot;
      u_single_shoot.real = this->single_shoot;
      *(outbuffer + offset + 0) = (u_single_shoot.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_single_shoot.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_single_shoot.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_single_shoot.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->single_shoot);
      union {
        int32_t real;
        uint32_t base;
      } u_continue_shoot;
      u_continue_shoot.real = this->continue_shoot;
      *(outbuffer + offset + 0) = (u_continue_shoot.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_continue_shoot.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_continue_shoot.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_continue_shoot.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->continue_shoot);
      union {
        int32_t real;
        uint32_t base;
      } u_run_friction_whell;
      u_run_friction_whell.real = this->run_friction_whell;
      *(outbuffer + offset + 0) = (u_run_friction_whell.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_run_friction_whell.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_run_friction_whell.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_run_friction_whell.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->run_friction_whell);
      union {
        int32_t real;
        uint32_t base;
      } u_friction_whell_speed;
      u_friction_whell_speed.real = this->friction_whell_speed;
      *(outbuffer + offset + 0) = (u_friction_whell_speed.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_friction_whell_speed.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_friction_whell_speed.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_friction_whell_speed.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->friction_whell_speed);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      union {
        int32_t real;
        uint32_t base;
      } u_single_shoot;
      u_single_shoot.base = 0;
      u_single_shoot.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_single_shoot.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_single_shoot.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_single_shoot.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->single_shoot = u_single_shoot.real;
      offset += sizeof(this->single_shoot);
      union {
        int32_t real;
        uint32_t base;
      } u_continue_shoot;
      u_continue_shoot.base = 0;
      u_continue_shoot.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_continue_shoot.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_continue_shoot.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_continue_shoot.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->continue_shoot = u_continue_shoot.real;
      offset += sizeof(this->continue_shoot);
      union {
        int32_t real;
        uint32_t base;
      } u_run_friction_whell;
      u_run_friction_whell.base = 0;
      u_run_friction_whell.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_run_friction_whell.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_run_friction_whell.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_run_friction_whell.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->run_friction_whell = u_run_friction_whell.real;
      offset += sizeof(this->run_friction_whell);
      union {
        int32_t real;
        uint32_t base;
      } u_friction_whell_speed;
      u_friction_whell_speed.base = 0;
      u_friction_whell_speed.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_friction_whell_speed.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_friction_whell_speed.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_friction_whell_speed.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->friction_whell_speed = u_friction_whell_speed.real;
      offset += sizeof(this->friction_whell_speed);
     return offset;
    }

    const char * getType(){ return "roborts_msgs/ShootState"; };
    const char * getMD5(){ return "36f858c917904d9229bf553ee5e21890"; };

  };

}
#endif
