#ifndef _ROS_rosbridge_library_TestHeaderArray_h
#define _ROS_rosbridge_library_TestHeaderArray_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace rosbridge_library
{

  class TestHeaderArray : public ros::Msg
  {
    public:
      uint32_t header_length;
      typedef std_msgs::Header _header_type;
      _header_type st_header;
      _header_type * header;

    TestHeaderArray():
      header_length(0), header(NULL)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->header_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->header_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->header_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->header_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->header_length);
      for( uint32_t i = 0; i < header_length; i++){
      offset += this->header[i].serialize(outbuffer + offset);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      uint32_t header_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      header_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      header_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      header_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->header_length);
      if(header_lengthT > header_length)
        this->header = (std_msgs::Header*)realloc(this->header, header_lengthT * sizeof(std_msgs::Header));
      header_length = header_lengthT;
      for( uint32_t i = 0; i < header_length; i++){
      offset += this->st_header.deserialize(inbuffer + offset);
        memcpy( &(this->header[i]), &(this->st_header), sizeof(std_msgs::Header));
      }
     return offset;
    }

    const char * getType(){ return "rosbridge_library/TestHeaderArray"; };
    const char * getMD5(){ return "d7be0bb39af8fb9129d5a76e6b63a290"; };

  };

}
#endif
