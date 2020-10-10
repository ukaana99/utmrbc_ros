/************************************************/
/*		 	 	Include Header	       		  	*/
/************************************************/

#include "I2C/i2c.h"

/************************************************/
/*		 	 	Variables	      	 		  	*/
/************************************************/


/************************************************/
/*		 	 	Functions		       		  	*/
/************************************************/

/*
 * Function Name		: I2CMasterSendI
 * Function Description : send an array of data to target device
 * Function Remarks		: None
 * Function Arguments	: I2Cx			select I2C peripheral (I2C1 or I2C2 or I2C3)
 *						  slave_addr	receiver’s or target’s address
 *						  len 			length of data to be send
 *						  data			data to be sent
 * Function Return		: BOOLEAN
 * Function Example		: unsigned char buffer[3] = { ‘a’,’b’,’c’ };
 *						  I2CMasterSendI(I2C2, main_board_2, sizeof(buffer), &buffer);
 */
bool I2CMasterSendI(I2C_TypeDef *I2Cx,uint8_t slave_addr, uint8_t len, uint8_t* data){

	uint32_t i2cTimeout = 0;

	I2C_ITConfig(I2Cx, I2C_IT_EVT|I2C_IT_BUF, DISABLE);

	i2cTimeout = I2C_LONG_TIMEOUT;
	while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY)) {
		if ((i2cTimeout--) == 0)
			return I2CTimeoutCallback(I2Cx);
	}

	if(I2Cx == I2C1)
		I2C1_Master_Flag = 1;
	else if(I2Cx == I2C2)
		I2C2_Master_Flag = 1;
	else if(I2Cx == I2C3)
		I2C3_Master_Flag = 1;

	I2C_GenerateSTART(I2Cx, ENABLE);

	i2cTimeout = I2C_SHORT_TIMEOUT;
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)) {
		if ((i2cTimeout--) == 0)
			return I2CTimeoutCallback(I2Cx);
	}

	I2C_Send7bitAddress(I2Cx, slave_addr << 1, I2C_Direction_Transmitter);

	i2cTimeout = I2C_SHORT_TIMEOUT;
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
		if ((i2cTimeout--) == 0)
			return I2CTimeoutCallback(I2Cx);
	}

	while(len--){
		I2C_SendData(I2Cx, *data++);

		i2cTimeout = I2C_SHORT_TIMEOUT;
		while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING)) {
			if ((i2cTimeout--) == 0)
				return I2CTimeoutCallback(I2Cx);
		}

	}

	i2cTimeout = I2C_SHORT_TIMEOUT;
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
		if ((i2cTimeout--) == 0)
			return I2CTimeoutCallback(I2Cx);
	}

	I2C_GenerateSTOP(I2Cx, ENABLE);

	i2cTimeout = I2C_LONG_TIMEOUT;
	while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY)) {
		if ((i2cTimeout--) == 0)
			return I2CTimeoutCallback(I2Cx);
	}

	if(I2Cx == I2C1)
		I2C1_Master_Flag = 0;
	else if(I2Cx == I2C2)
		I2C2_Master_Flag = 0;
	else if(I2Cx == I2C3)
		I2C3_Master_Flag = 0;

	I2C_ITConfig(I2Cx, I2C_IT_EVT, ENABLE);
	I2C_ITConfig(I2Cx, I2C_IT_BUF, ENABLE);

	return true;
}

/*
 * Function Name		: I2CMasterReadI
 * Function Description : read an array of data from target device.
 * Function Remarks		: The data received is handle inside the master receive callback function.
 * Function Arguments	: I2Cx			select I2C peripheral (I2C1 or I2C2 or I2C3)
 *						  slave_addr	sender’s or target’s address
 * 						  len			length of data to be received.
 * 						  *data			pointer to buffer to store the data
 * Function Return		: BOOLEAN
 * Function Example		: I2CMasterReadI(I2C3, 9, 5,&buff[0);
 */
bool I2CMasterReadI (I2C_TypeDef *I2Cx,uint8_t slave_addr, uint8_t len, uint8_t *data){

	uint32_t i2cTimeout = 0;

	I2C_ITConfig(I2Cx, I2C_IT_EVT|I2C_IT_BUF, DISABLE);

    i2cTimeout = I2C_SHORT_TIMEOUT;
    while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY)) {
        if ((i2cTimeout--) == 0)
            return I2CTimeoutCallback(I2Cx);
    }

    uint8_t i = len;

    if(I2Cx == I2C1)
		I2C1_Master_Flag = 1;
	else if(I2Cx == I2C2)
		I2C2_Master_Flag = 1;
	else if(I2Cx == I2C3)
		I2C3_Master_Flag = 1;

	I2C_GenerateSTART(I2Cx, ENABLE);

	i2cTimeout = I2C_SHORT_TIMEOUT;
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)) {
		 if ((i2cTimeout--) == 0)
		      return I2CTimeoutCallback(I2Cx);
    }

	I2C_Send7bitAddress(I2Cx, slave_addr << 1, I2C_Direction_Receiver);

	if(len==1) I2C_AcknowledgeConfig(I2Cx, DISABLE);

	i2cTimeout = I2C_SHORT_TIMEOUT;
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) {
		 if ((i2cTimeout--) == 0)
		     return I2CTimeoutCallback(I2Cx);
    }

	while(len){
		if (len==1) {
			I2C_AcknowledgeConfig(I2Cx, DISABLE);
			I2C_GenerateSTOP(I2Cx, ENABLE);
			i2cTimeout = I2C_SHORT_TIMEOUT;
			while( !I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) ){
				if ((i2cTimeout--) == 0)
					 return I2CTimeoutCallback(I2Cx);
			}
			*data = I2C_ReceiveData(I2Cx);
		} else {
			I2C_AcknowledgeConfig(I2Cx, ENABLE);
			i2cTimeout = I2C_SHORT_TIMEOUT;
			while( !I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) ){
				if ((i2cTimeout--) == 0)
					 return I2CTimeoutCallback(I2Cx);
			}
			*data = I2C_ReceiveData(I2Cx);
		}
		data++;
		len--;
	}

    I2C_AcknowledgeConfig(I2Cx, ENABLE);

    i2cTimeout = I2C_SHORT_TIMEOUT;
    while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY)) {
	   if ((i2cTimeout--) == 0) {
		   return I2CTimeoutCallback(I2Cx);
	   }
    }

    if(I2Cx == I2C1)
		I2C1_Master_Flag = 0;
	else if(I2Cx == I2C2)
		I2C2_Master_Flag = 0;
	else if(I2Cx == I2C3)
		I2C3_Master_Flag = 0;

    I2C_ITConfig(I2Cx, I2C_IT_EVT|I2C_IT_BUF, ENABLE);
    return true;

}

/*
 * Function Name		: I2CMasterSendII
 * Function Description : send an array of data to target device
 * Function Remarks		: None
 * Function Arguments	: I2Cx			select I2C peripheral (I2C1 or I2C2 or I2C3)
 *						  slave_addr	receiver’s or target’s address
 *						  len 			length of data to be send
 *						  data			data to be sent
 * Function Return		: BOOLEAN
 * Function Example		: unsigned char buffer[3] = { ‘a’,’b’,’c’ };
 *						  I2CMasterSend(I2C2, main_board_2, sizeof(buffer), &buffer);
 */
bool I2CMasterSendII(I2C_TypeDef *I2Cx,uint8_t slave_addr, uint8_t len, uint8_t* data){

	uint32_t i2cTimeout = 0;
	uint16_t index = 0;

	i2cTimeout = I2C_LONG_TIMEOUT;
	while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY)) {
		if ((i2cTimeout--) == 0)
			return I2CTimeoutCallback(I2Cx);
	}

	if(I2Cx == I2C1)
		I2C1_Master_Flag = 1;
	else if(I2Cx == I2C2)
		I2C2_Master_Flag = 1;
	else if(I2Cx == I2C3)
		I2C3_Master_Flag = 1;

	if(I2Cx == I2C1){
		I2C1txlen = len;
		I2C1slaveadress = slave_addr;
		I2C1_direction = I2C_Direction_Transmitter;
		while(len--) I2C1_master_snd_buf[index++] = *data++;
	}else if(I2Cx == I2C2){
		I2C2txlen = len;
		I2C2slaveadress = slave_addr;
		I2C2_direction = I2C_Direction_Transmitter;
		while(len--) I2C2_master_snd_buf[index++] = *data++;
	}else if(I2Cx == I2C3){
		I2C3txlen = len;
		I2C3slaveadress = slave_addr;
		I2C3_direction = I2C_Direction_Transmitter;
		while(len--) I2C3_master_snd_buf[index++] = *data++;
	}

	I2C_GenerateSTART(I2Cx, ENABLE);

	return true;

}

/*
 * Function Name		: I2CMasterReadII
 * Function Description : read an array of data from target device.
 * Function Remarks		: The data received is handle inside the master receive callback function.
 * Function Arguments	: I2Cx			select I2C peripheral (I2C1 or I2C2 or I2C3)
 *						  slave_addr	sender’s or target’s address
 * 						  len			length of data to be received.
 * Function Return		: BOOLEAN
 * Function Example		: I2CMasterReadII(I2C3, 9, 5);
 */
bool I2CMasterReadII(I2C_TypeDef *I2Cx,uint8_t slave_addr, uint8_t len){


	uint32_t i2cTimeout = 0;

	i2cTimeout = I2C_LONG_TIMEOUT;
	while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY)) {
		if ((i2cTimeout--) == 0)
			return I2CTimeoutCallback(I2Cx);
	}

	if(I2Cx == I2C1)
		I2C1_Master_Flag = 1;
	else if(I2Cx == I2C2)
		I2C2_Master_Flag = 1;
	else if(I2Cx == I2C3)
		I2C3_Master_Flag = 1;

	if(I2Cx == I2C1){
		I2C1rxlen = len;
		I2C1slaveadress = slave_addr;
		I2C1_direction = I2C_Direction_Receiver;
	}else if(I2Cx == I2C2){
		I2C2rxlen = len;
		I2C2slaveadress = slave_addr;
		I2C2_direction = I2C_Direction_Receiver;
	}else if(I2Cx == I2C3){
		I2C3rxlen = len;
		I2C3slaveadress = slave_addr;
		I2C3_direction = I2C_Direction_Receiver;
	}

	I2C_GenerateSTART(I2Cx, ENABLE);

	return true;
}

/*
 * Function Name		: I2CSend
 * Function Description : Send data in the packet defined by the RBC protocol.
 * Function Remarks		: The data pointed by data can be char, int, long, float, struct, union, array, etc.
 * Function Arguments	: I2Cx 			select I2C peripheral (I2C1 or I2C2 or I2C3)
 *						  slave_addr 	receiver’s or target’s address
 *						  len 	 		length of data to be sent
 *						  data 			pointer to data to be sent
 * Function Return		: BOOLEAN
 * Function Example		: float speed = 123.45;
 *						  I2CSend(I2C1, main_board_1, sizeof(speed), &speed);
 */
bool I2CSend(I2C_TypeDef *I2Cx, uint32_t slave_addr, uint8_t len, const void *data){

	uint8_t master_snd_buf[256];
	uint8_t checksum = 0;
	uint8_t index = 0;
	uint8_t tmp_len = len;

	if(I2Cx == I2C1)
		master_snd_buf[index++] = I2C1_InitStructure.I2C_OwnAddress1 >> 1;
	if(I2Cx == I2C2)
		master_snd_buf[index++] = I2C2_InitStructure.I2C_OwnAddress1 >> 1;
	if(I2Cx == I2C3)
		master_snd_buf[index++] = I2C3_InitStructure.I2C_OwnAddress1 >> 1;

	master_snd_buf[index++] = len;

	while (tmp_len--) {
		checksum += master_snd_buf[index++] = *(uint8_t *)data++;
	}

	master_snd_buf[index++] = ~checksum + 1;
	master_snd_buf[index++] = 0;
	master_snd_buf[index] = 0;

	return I2CMasterSendI(I2Cx,slave_addr, len + 5, &master_snd_buf[0]);
}

/*
 * Function Name		: I2CSendV
 * Function Description : Send data in the packet defined by the RBC protocol.
 * Function Remarks		: The data in the argument list must have the size of a single char.
 * 						  Longer data must be separated into byte-sized argument as input to the function,
 * 						  and its length must correspond to the second parameter len.
 * Function Arguments	: I2Cx 		select I2C peripheral (I2C1 or I2C2 or I2C3)
 *						  addr 		receiver’s or target’s address
 *						  len 		length of data to be sent
 *						  ... 		data to be sent
 * Function Return		: BOOLEAN
 * Function Example		: long output = 0x90ABCDEF;
 *						  I2CSendV (I2C1, main_board_1, sizeof(long),
 *						  (char)(output&0xff), (char)(output >> 8 & 0xff), (char)(output >> 16 & 0xff), (char)(output >> 24 & 0xff));
 */
bool I2CSendV(I2C_TypeDef *I2Cx, uint8_t slave_addr, uint8_t len, ...){

	uint8_t master_snd_buf[256];
	uint8_t tmp_len = len;
	uint8_t index = 0;
	uint8_t checksum = 0;
	va_list vdata;

	if(I2Cx == I2C1)
		master_snd_buf[index++] = I2C1_InitStructure.I2C_OwnAddress1 >> 1;
	if(I2Cx == I2C2)
		master_snd_buf[index++] = I2C3_InitStructure.I2C_OwnAddress1 >> 1;
	if(I2Cx == I2C3)
		master_snd_buf[index++] = I2C3_InitStructure.I2C_OwnAddress1 >> 1;

	master_snd_buf[index++] = len;

	va_start(vdata, len);
	while (tmp_len--) {

		checksum += master_snd_buf[index++] =  (uint8_t)va_arg(vdata, int);
	}
	va_end(vdata);

	master_snd_buf[index++] = ~checksum + 1;
	master_snd_buf[index++] = 0;
	master_snd_buf[index] = 0;

	return I2CMasterSendI(I2Cx,slave_addr, len + 5, &master_snd_buf[0]);

}

/*
 * Function Name		: I2CTimeoutCallback
 * Function Description : This function is called during timeout of the I2C sending and receiving operation and count the number of timeout.
 * Function Remarks		: This function is called automatically within the library during timeout.
 * 						  User can read the number of timeout via function i2cGetErrorCounter.
 * Function Arguments	: I2Cx 		select I2C peripheral (I2C1 or I2C2 or I2C3)
 * Function Return		: BOOLEAN
 * Function Example		: None
 */
uint32_t I2CTimeoutCallback(I2C_TypeDef *I2Cx){

	if (I2Cx == I2C1){
		i2c1ErrorCount++;
	}else if (I2Cx == I2C2) {
		i2c2ErrorCount++;
	}else if (I2Cx == I2C3) {
		i2c2ErrorCount++;
	}

	return false;
}

/*
 * Function Name		: I2CGetErrorCounter
 * Function Description : return number of timeout.
 * Function Remarks		: This function is used for debugging purpose.
 * Function Arguments	: I2Cx 		select I2C peripheral (I2C1 or I2C2 or I2C3)
 * Function Return		: BOOLEAN
 * Function Example		: uint16_t i2c1timeoutcount;
 *						  i2c1timeoutcount = i2cGetErrorCounter(I2C1);
 */
uint16_t I2CGetErrorCounter(I2C_TypeDef *I2Cx){

	uint16_t errorcount = 0;

    if (I2Cx == I2C1)
    	errorcount = i2c1ErrorCount;
    else if(I2Cx == I2C2)
    	errorcount = i2c2ErrorCount;
    else if(I2Cx == I2C2)
    	errorcount = i2c3ErrorCount;

    return errorcount;

}

