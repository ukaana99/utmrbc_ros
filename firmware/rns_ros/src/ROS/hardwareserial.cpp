#include "ROS/hardwareserial.h"

USART_TypeDef*  SERIAL_USART[SERIALn] = {USART2, USART3, UART4};

static uint8_t rxbuf[RXBUF_SIZE];
static ring_buffer rb1_recv;
static ring_buffer rb2_recv;
static ring_buffer rb3_recv;

void HardwareSerial::begin(uint32_t baud)
{
	/** init ringbuf **/
	if(this->Serial == SERIAL2)
		rb_init(&rb1_recv, sizeof(rxbuf), rxbuf);
	if(this->Serial == SERIAL3)
		rb_init(&rb2_recv, sizeof(rxbuf), rxbuf);
	if(this->Serial == SERIAL4)
		rb_init(&rb3_recv, sizeof(rxbuf), rxbuf);
}

uint32_t HardwareSerial::available(void)
{
	if(this->Serial == SERIAL2)
		return rb_full_count(&rb1_recv);
	if(this->Serial == SERIAL3)
		return rb_full_count(&rb2_recv);
	if(this->Serial == SERIAL4)
		return rb_full_count(&rb3_recv);

	return 0;

}

uint8_t HardwareSerial::read(void)
{
	while(!this->available())
		;
	if(this->Serial == SERIAL2)
		return rb_remove(&rb1_recv);
	if(this->Serial == SERIAL3)
		return rb_remove(&rb2_recv);
	if(this->Serial == SERIAL4)
		return rb_remove(&rb3_recv);

	return 0;
}

uint32_t HardwareSerial::write(uint8_t ch)
{
	USART_SendData(SERIAL_USART[this->Serial], ch);
	while(USART_GetFlagStatus(SERIAL_USART[this->Serial], USART_FLAG_TXE) == RESET);

	return 1;
}

void HardwareSerial::flush()
{
	if(this->Serial == SERIAL2)
		rb_reset(&rb1_recv);
	if(this->Serial == SERIAL3)
		rb_reset(&rb2_recv);
	if(this->Serial == SERIAL4)
		rb_reset(&rb3_recv);
}

void HardwareSerial::print(const char *format, ...)
{
	va_list args;
	char buf[256];
	va_start (args, format);
	vsprintf (buf, format, args);
	va_end (args);
	putstr(buf);
}

void HardwareSerial::putstr(const char *str)
{
//	for(int i = 0; i < strlen(str); i++){
//		write(str[i]);
//	}
}

#ifdef __cplusplus
extern "C" {
#endif

void USART2_IRQHandler(void)
{
	unsigned char data;
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
		data = USART_ReceiveData(USART2);
		rb_push_insert(&rb1_recv, data);
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
}
void USART3_IRQHandler(void)
{
	unsigned char data;
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {
		data = USART_ReceiveData(USART3);
		rb_push_insert(&rb2_recv, data);
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}
}

void UART4_IRQHandler(void)
{
	unsigned char data;
	if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET) {
		data = USART_ReceiveData(UART4);
		rb_push_insert(&rb3_recv, data);
		USART_ClearITPendingBit(UART4, USART_IT_RXNE);
	}
}

#ifdef __cplusplus
}
#endif

