#ifndef HARDWARESERIAL_H_
#define HARDWARESERIAL_H_

#include <ROS/config.h>

class HardwareSerial {
private:
Serial_TypeDef Serial;

public:
	HardwareSerial(Serial_TypeDef _Serial=SERIAL2){
		Serial = _Serial;
	}

	void begin(uint32_t baud);

	uint32_t available(void);
	uint8_t read(void);
	void flush(void);
	uint32_t write(uint8_t ch);
	void print(const char *format, ...);
	void putstr(const char *str);
	//uint32_t send(void *buf, uint32_t len);
	
	//static ring_buf_t rb_send;
	//static ring_buf_t rb_recv;

};

#endif
