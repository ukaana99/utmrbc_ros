#ifndef _ROS_rosbridge_library_TestDurationArray_h
#define _ROS_rosbridge_library_TestDurationArray_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "ros/duration.h"

namespace rosbridge_library
{

  class TestDurationArray : public ros::Msg
  {
    public:
      uint32_t durations_length;
      typedef ros::Duration _durations_type;
      _durations_type st_durations;
      _durations_type * durations;

    TestDurationArray():
      durations_length(0), durations(NULL)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->durations_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->durations_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->durations_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->durations_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->durations_length);
      for( uint32_t i = 0; i < durations_length; i++){
      *(outbuffer + offset + 0) = (this->durations[i].sec >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->durations[i].sec >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->durations[i].sec >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->durations[i].sec >> (8 * 3)) & 0xFF;
      offset += sizeof(this->durations[i].sec);
      *(outbuffer + offset + 0) = (this->durations[i].nsec >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->durations[i].nsec >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->durations[i].nsec >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->durations[i].nsec >> (8 * 3)) & 0xFF;
      offset += sizeof(this->durations[i].nsec);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      uint32_t durations_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      durations_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      durations_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      durations_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->durations_length);
      if(durations_lengthT > durations_length)
        this->durations = (ros::Duration*)realloc(this->durations, durations_lengthT * sizeof(ros::Duration));
      durations_length = durations_lengthT;
      for( uint32_t i = 0; i < durations_length; i++){
      this->st_durations.sec =  ((uint32_t) (*(inbuffer + offset)));
      this->st_durations.sec |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->st_durations.sec |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->st_durations.sec |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->st_durations.sec);
      this->st_durations.nsec =  ((uint32_t) (*(inbuffer + offset)));
      this->st_durations.nsec |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->st_durations.nsec |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->st_durations.nsec |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->st_durations.nsec);
        memcpy( &(this->durations[i]), &(this->st_durations), sizeof(ros::Duration));
      }
     return offset;
    }

    const char * getType(){ return "rosbridge_library/TestDurationArray"; };
    const char * getMD5(){ return "8b3bcadc803a7fcbc857c6a1dab53bcd"; };

  };

}
#endif
