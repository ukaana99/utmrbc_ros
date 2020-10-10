
#ifndef STM32_USB_HOST_LIBRARY_CLASS_BTD_INC_SPP_H_
#define STM32_USB_HOST_LIBRARY_CLASS_BTD_INC_SPP_H_

#include "usbh_btd_core.h"

/* Used for SDP */
#define SDP_SERVICE_SEARCH_ATTRIBUTE_REQUEST_PDU    0x06 // See the RFCOMM specs
#define SDP_SERVICE_SEARCH_ATTRIBUTE_RESPONSE_PDU   0x07 // See the RFCOMM specs
#define SERIALPORT_UUID     0x1101 // See http://www.bluetooth.org/Technical/AssignedNumbers/service_discovery.htm
#define L2CAP_UUID          0x0100

/* Used for RFCOMM */
#define RFCOMM_SABM     0x2F
#define RFCOMM_UA       0x63
#define RFCOMM_UIH      0xEF
//#define RFCOMM_DM       0x0F
#define RFCOMM_DISC     0x43

#define extendAddress   0x01 // Always 1

// Multiplexer message types
#define BT_RFCOMM_PN_CMD     0x83
#define BT_RFCOMM_PN_RSP     0x81
#define BT_RFCOMM_MSC_CMD    0xE3
#define BT_RFCOMM_MSC_RSP    0xE1
#define BT_RFCOMM_RPN_CMD    0x93
#define BT_RFCOMM_RPN_RSP    0x91

typedef struct{

	BluetoothService SPPservice;
	uint8_t SPPserviceID;

	/** Variable used to indicate if the connection is established. */
	bool connected;

	/* Set true when a channel is created */
	bool SDPConnected;
	bool RFCOMMConnected;

	/* Variables used by L2CAP state machines */
	uint8_t l2cap_sdp_state;
	uint8_t l2cap_rfcomm_state;

	uint8_t l2capoutbuf[BULK_MAXPKTSIZE]; // General purpose buffer for l2cap out data
	uint8_t rfcommbuf[10]; // Buffer for RFCOMM Commands

	/* L2CAP Channels */
	uint8_t sdp_scid[2]; // L2CAP source CID for SDP
	uint8_t sdp_dcid[2]; // 0x0050
	uint8_t rfcomm_scid[2]; // L2CAP source CID for RFCOMM
	uint8_t rfcomm_dcid[2]; // 0x0051

	/* RFCOMM Variables */
	uint8_t rfcommChannel;
	uint8_t rfcommChannelConnection; // This is the channel the SPP channel will be running at
	uint8_t rfcommDirection;
	uint8_t rfcommCommandResponse;
	uint8_t rfcommChannelType;
	uint8_t rfcommPfBit;

	uint32_t timer;
	bool waitForLastCommand;
	bool creditSent;

	uint8_t rfcommDataBuffer[100]; // Create a 100 sized buffer for incoming data
	uint8_t sppOutputBuffer[100]; // Create a 100 sized buffer for outgoing SPP data
	uint8_t sppIndex;
	uint8_t rfcommAvailable;

	bool firstMessage; // Used to see if it's the first SDP request received
	uint8_t bytesRead; // Counter to see when it's time to send more credit

}SPP_t;

void SPP(SPP_t* SPP, const char* name, const char* pin);

/*BluetoothService implementation */
void SPPReset(uint32_t pointer_address);
void SPPdisconnect(uint32_t pointer_address);
void SPPACLData(uint8_t* l2capinbuf,uint32_t pointer_address);
void SPPRun(uint32_t pointer_address);
void SPPonInit(uint32_t pointer_address);

/* State machines */
void SPPSDP_task(SPP_t* SPP);
void SPPRFCOMM_task(SPP_t* SPP);

/* SDP Commands */
void SPPSDP_Command(SPP_t* SPP, uint8_t* data, uint8_t nbytes);
void SPPserviceNotSupported(SPP_t* SPP, uint8_t transactionIDHigh, uint8_t transactionIDLow);
void SPPserialPortResponse1(SPP_t* SPP, uint8_t transactionIDHigh, uint8_t transactionIDLow);
void SPPserialPortResponse2(SPP_t* SPP,uint8_t transactionIDHigh, uint8_t transactionIDLow);
void SPPl2capResponse1(SPP_t* SPP,uint8_t transactionIDHigh, uint8_t transactionIDLow);
void SPPl2capResponse2(SPP_t* SPP,uint8_t transactionIDHigh, uint8_t transactionIDLow);

/* RFCOMM Commands */
void SPPRFCOMM_Command(SPP_t* SPP, uint8_t* data, uint8_t nbytes);
void SPPsendRfcomm(SPP_t* SPP, uint8_t channel, uint8_t direction, uint8_t CR, uint8_t channelType, uint8_t pfBit, uint8_t* data, uint8_t length);
void SPPsendRfcommCredit(SPP_t* SPP, uint8_t channel, uint8_t direction, uint8_t CR, uint8_t channelType, uint8_t pfBit, uint8_t credit);
uint8_t SPPcrc(uint8_t *data);
uint8_t SPPcalcFcs(uint8_t *data);
uint8_t SPPcalcFcs(uint8_t *data);
bool SPPcheckFcs(uint8_t *data, uint8_t fcs);

/** @name Serial port profile (SPP) Print functions */

/**
 * Writes the byte to send to a buffer. The message is send when either send() or after Usb.Task() is called.
 * @param  data The byte to write.
 */
void SPPwrite(SPP_t* SPP,uint8_t data);

/**
 * Writes the bytes to send to a buffer. The message is send when either send() or after Usb.Task() is called.
 * @param data The data array to send.
 * @param size Size of the data.
 */
void write(SPP_t* SPP, const uint8_t *data, size_t size);

/**
 * This will send all the bytes in the buffer.
 */
void SPPsend(SPP_t* SPP);

/**
 * Get number of bytes waiting to be read.
 * @return Return the number of bytes ready to be read.
 */
int SPPavailable(SPP_t* SPP);

/** Discard all the bytes in the buffer. */
void SPPdiscard(SPP_t* SPP);

/**
 * Used to read the next value in the buffer without advancing to the next one.
 * @return Return the byte. Will return -1 if no bytes are available.
 */
int SPPpeek(SPP_t* SPP);

/**
 * Used to read the buffer.
 * @return Return the byte. Will return -1 if no bytes are available.
 */
int SPPread(SPP_t* SPP);

/** Send out all bytes in the buffer. */
void SPPflush(SPP_t* SPP);

#endif /* STM32_USB_HOST_LIBRARY_CLASS_BTD_INC_SPP_H_ */
