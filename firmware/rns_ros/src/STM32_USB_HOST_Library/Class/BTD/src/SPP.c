
#include "STM32_USB_HOST_Library/Class/BTD/inc/SPP.h"

/* Macros for L2CAP event flag tests */
#define l2cap_check_flag(flag) (SPP->SPPservice.l2cap_event_flag & (flag))
#define l2cap_set_flag(flag) (SPP->SPPservice.l2cap_event_flag |= (flag))
#define l2cap_clear_flag(flag) (SPP->SPPservice.l2cap_event_flag &= ~(flag))

/*
 * CRC (reversed crc) lookup table as calculated by the table generator in ETSI TS 101 369 V6.3.0.
 */
const uint8_t rfcomm_crc_table[256] = {/* reversed, 8-bit, poly=0x07 */
	0x00, 0x91, 0xE3, 0x72, 0x07, 0x96, 0xE4, 0x75, 0x0E, 0x9F, 0xED, 0x7C, 0x09, 0x98, 0xEA, 0x7B,
	0x1C, 0x8D, 0xFF, 0x6E, 0x1B, 0x8A, 0xF8, 0x69, 0x12, 0x83, 0xF1, 0x60, 0x15, 0x84, 0xF6, 0x67,
	0x38, 0xA9, 0xDB, 0x4A, 0x3F, 0xAE, 0xDC, 0x4D, 0x36, 0xA7, 0xD5, 0x44, 0x31, 0xA0, 0xD2, 0x43,
	0x24, 0xB5, 0xC7, 0x56, 0x23, 0xB2, 0xC0, 0x51, 0x2A, 0xBB, 0xC9, 0x58, 0x2D, 0xBC, 0xCE, 0x5F,
	0x70, 0xE1, 0x93, 0x02, 0x77, 0xE6, 0x94, 0x05, 0x7E, 0xEF, 0x9D, 0x0C, 0x79, 0xE8, 0x9A, 0x0B,
	0x6C, 0xFD, 0x8F, 0x1E, 0x6B, 0xFA, 0x88, 0x19, 0x62, 0xF3, 0x81, 0x10, 0x65, 0xF4, 0x86, 0x17,
	0x48, 0xD9, 0xAB, 0x3A, 0x4F, 0xDE, 0xAC, 0x3D, 0x46, 0xD7, 0xA5, 0x34, 0x41, 0xD0, 0xA2, 0x33,
	0x54, 0xC5, 0xB7, 0x26, 0x53, 0xC2, 0xB0, 0x21, 0x5A, 0xCB, 0xB9, 0x28, 0x5D, 0xCC, 0xBE, 0x2F,
	0xE0, 0x71, 0x03, 0x92, 0xE7, 0x76, 0x04, 0x95, 0xEE, 0x7F, 0x0D, 0x9C, 0xE9, 0x78, 0x0A, 0x9B,
	0xFC, 0x6D, 0x1F, 0x8E, 0xFB, 0x6A, 0x18, 0x89, 0xF2, 0x63, 0x11, 0x80, 0xF5, 0x64, 0x16, 0x87,
	0xD8, 0x49, 0x3B, 0xAA, 0xDF, 0x4E, 0x3C, 0xAD, 0xD6, 0x47, 0x35, 0xA4, 0xD1, 0x40, 0x32, 0xA3,
	0xC4, 0x55, 0x27, 0xB6, 0xC3, 0x52, 0x20, 0xB1, 0xCA, 0x5B, 0x29, 0xB8, 0xCD, 0x5C, 0x2E, 0xBF,
	0x90, 0x01, 0x73, 0xE2, 0x97, 0x06, 0x74, 0xE5, 0x9E, 0x0F, 0x7D, 0xEC, 0x99, 0x08, 0x7A, 0xEB,
	0x8C, 0x1D, 0x6F, 0xFE, 0x8B, 0x1A, 0x68, 0xF9, 0x82, 0x13, 0x61, 0xF0, 0x85, 0x14, 0x66, 0xF7,
	0xA8, 0x39, 0x4B, 0xDA, 0xAF, 0x3E, 0x4C, 0xDD, 0xA6, 0x37, 0x45, 0xD4, 0xA1, 0x30, 0x42, 0xD3,
	0xB4, 0x25, 0x57, 0xC6, 0xB3, 0x22, 0x50, 0xC1, 0xBA, 0x2B, 0x59, 0xC8, 0xBD, 0x2C, 0x5E, 0xCF
};

void SPP(SPP_t* SPP, const char* name, const char* pin)
{
	BTD_Machine.btdName = name;
	BTD_Machine.btdPin = pin;

	/* Set device cid for the SDP and RFCOMM channelse */
	SPP->sdp_dcid[0] = 0x50; // 0x0050
	SPP->sdp_dcid[1] = 0x00;
	SPP->rfcomm_dcid[0] = 0x51; // 0x0051
	SPP->rfcomm_dcid[1] = 0x00;

	SPP->SPPservice.ACLData = SPPACLData;
	SPP->SPPservice.Reset = SPPReset;
	SPP->SPPservice.Run = SPPRun;
	SPP->SPPservice.disconnect = SPPdisconnect;
	SPP->SPPservice.onInit = SPPonInit;

	SPP->SPPserviceID = registerBluetoothService((uint32_t)SPP, &SPP->SPPservice);
	SPP->SPPservice.Reset(BTD_Machine.btservive_pointer_address[SPP->SPPserviceID]);
}

void SPPReset(uint32_t pointer_address)
{
	SPP_t*SPP = (SPP_t*) pointer_address;
	SPP->connected = false;
	SPP->RFCOMMConnected = false;
	SPP->SDPConnected = false;
	SPP->waitForLastCommand = false;
	SPP->l2cap_sdp_state = L2CAP_SDP_WAIT;
	SPP->l2cap_rfcomm_state = L2CAP_RFCOMM_WAIT;
	SPP->SPPservice.l2cap_event_flag = 0;
	SPP->sppIndex = 0;
	SPP->creditSent = false;
}

void SPPdisconnect(uint32_t pointer_address)
{
	SPP_t*SPP = (SPP_t*) pointer_address;

	SPP->connected = false;
	// First the two L2CAP channels has to be disconnected and then the HCI connection
	if(SPP->RFCOMMConnected)
		l2cap_disconnection_request	(SPP->SPPservice.hci_handle, ++SPP->SPPservice.identifier, SPP->rfcomm_scid, SPP->rfcomm_dcid);
	if(SPP->RFCOMMConnected && SPP->SDPConnected)
		USB_OTG_BSP_mDelay(1); // Add delay between commands
	if(SPP->SDPConnected)
		l2cap_disconnection_request(SPP->SPPservice.hci_handle, ++SPP->SPPservice.identifier, SPP->sdp_scid, SPP->sdp_dcid);
	SPP->l2cap_sdp_state = L2CAP_DISCONNECT_RESPONSE;
}

void SPPACLData(uint8_t* l2capinbuf,uint32_t pointer_address) {

	SPP_t*SPP = (SPP_t*) pointer_address;

	if(!SPP->connected) {
		if(l2capinbuf[8] == L2CAP_CMD_CONNECTION_REQUEST) {
			if((l2capinbuf[12] | (l2capinbuf[13] << 8)) == SDP_PSM && !BTD_Machine.sdpConnectionClaimed) {
				BTD_Machine.sdpConnectionClaimed = true;
				SPP->SPPservice.hci_handle = BTD_Machine.hci_handle; // Store the HCI Handle for the connection
				SPP->l2cap_sdp_state = L2CAP_SDP_WAIT; // Reset state
			} else if((l2capinbuf[12] | (l2capinbuf[13] << 8)) == RFCOMM_PSM && !BTD_Machine.rfcommConnectionClaimed) {
				BTD_Machine.rfcommConnectionClaimed = true;
				SPP->SPPservice.hci_handle = BTD_Machine.hci_handle; // Store the HCI Handle for the connection
				SPP->l2cap_rfcomm_state = L2CAP_RFCOMM_WAIT; // Reset state
			}
		}
	}

	if(checkHciHandle(l2capinbuf, SPP->SPPservice.hci_handle)) { // acl_handle_ok
		if((l2capinbuf[6] | (l2capinbuf[7] << 8)) == 0x0001U) { // l2cap_control - Channel ID for ACL-U
			if(l2capinbuf[8] == L2CAP_CMD_COMMAND_REJECT) {
#ifdef DEBUG_USB_HOST
				sprintf(btduartbuff,"\r\nL2CAP Command Rejected - Reason: %X: %X: %X: %X: %X: %X ",l2capinbuf[13],l2capinbuf[12],l2capinbuf[17],l2capinbuf[16],l2capinbuf[15],l2capinbuf[14]);
				UARTPrintString(PRINTING_USART, btduartbuff);
#endif
			} else if(l2capinbuf[8] == L2CAP_CMD_CONNECTION_REQUEST) {
				if((l2capinbuf[12] | (l2capinbuf[13] << 8)) == SDP_PSM) { // It doesn't matter if it receives another reqeust, since it waits for the channel to disconnect in the L2CAP_SDP_DONE state, and the l2cap_event_flag will be cleared if so
					SPP->SPPservice.identifier = l2capinbuf[9];
					SPP->sdp_scid[0] = l2capinbuf[14];
					SPP->sdp_scid[1] = l2capinbuf[15];
					l2cap_set_flag(L2CAP_FLAG_CONNECTION_SDP_REQUEST);
				} else if((l2capinbuf[12] | (l2capinbuf[13] << 8)) == RFCOMM_PSM) { // ----- || -----
					SPP->SPPservice.identifier = l2capinbuf[9];
					SPP->rfcomm_scid[0] = l2capinbuf[14];
					SPP->rfcomm_scid[1] = l2capinbuf[15];
						l2cap_set_flag(L2CAP_FLAG_CONNECTION_RFCOMM_REQUEST);
				}
			} else if(l2capinbuf[8] == L2CAP_CMD_CONFIG_RESPONSE) {
				if((l2capinbuf[16] | (l2capinbuf[17] << 8)) == 0x0000) { // Success
					if(l2capinbuf[12] == SPP->sdp_dcid[0] && l2capinbuf[13] == SPP->sdp_dcid[1]) {
						//Notify(PSTR("\r\nSDP Configuration Complete"), 0x80);
						l2cap_set_flag(L2CAP_FLAG_CONFIG_SDP_SUCCESS);
					} else if(l2capinbuf[12] == SPP->rfcomm_dcid[0] && l2capinbuf[13] == SPP->rfcomm_dcid[1]) {
						//Notify(PSTR("\r\nRFCOMM Configuration Complete"), 0x80);
						l2cap_set_flag(L2CAP_FLAG_CONFIG_RFCOMM_SUCCESS);
					}
				}
			} else if(l2capinbuf[8] == L2CAP_CMD_CONFIG_REQUEST) {
				if(l2capinbuf[12] == SPP->sdp_dcid[0] && l2capinbuf[13] == SPP->sdp_dcid[1]) {
					//Notify(PSTR("\r\nSDP Configuration Request"), 0x80);
					l2cap_config_response(SPP->SPPservice.hci_handle, l2capinbuf[9], SPP->sdp_scid);
				} else if(l2capinbuf[12] == SPP->rfcomm_dcid[0] && l2capinbuf[13] == SPP->rfcomm_dcid[1]) {
					//Notify(PSTR("\r\nRFCOMM Configuration Request"), 0x80);
					l2cap_config_response(SPP->SPPservice.hci_handle, l2capinbuf[9], SPP->rfcomm_scid);
				}
			} else if(l2capinbuf[8] == L2CAP_CMD_DISCONNECT_REQUEST) {
				if(l2capinbuf[12] == SPP->sdp_dcid[0] && l2capinbuf[13] == SPP->sdp_dcid[1]) {
					//Notify(PSTR("\r\nDisconnect Request: SDP Channel"), 0x80);
					SPP->SPPservice.identifier = l2capinbuf[9];
					l2cap_set_flag(L2CAP_FLAG_DISCONNECT_SDP_REQUEST);
				} else if(l2capinbuf[12] == SPP->rfcomm_dcid[0] && l2capinbuf[13] == SPP->rfcomm_dcid[1]) {
					//Notify(PSTR("\r\nDisconnect Request: RFCOMM Channel"), 0x80);
					SPP->SPPservice.identifier = l2capinbuf[9];
					l2cap_set_flag(L2CAP_FLAG_DISCONNECT_RFCOMM_REQUEST);
				}
			} else if(l2capinbuf[8] == L2CAP_CMD_DISCONNECT_RESPONSE) {
				if(l2capinbuf[12] == SPP->sdp_scid[0] && l2capinbuf[13] == SPP->sdp_scid[1]) {
					//Notify(PSTR("\r\nDisconnect Response: SDP Channel"), 0x80);
					SPP->SPPservice.identifier = l2capinbuf[9];
					l2cap_set_flag(L2CAP_FLAG_DISCONNECT_RESPONSE);
				} else if(l2capinbuf[12] == SPP->rfcomm_scid[0] && l2capinbuf[13] == SPP->rfcomm_scid[1]) {
					//Notify(PSTR("\r\nDisconnect Response: RFCOMM Channel"), 0x80);
					SPP->SPPservice.identifier = l2capinbuf[9];
					l2cap_set_flag(L2CAP_FLAG_DISCONNECT_RESPONSE);
				}
			} else if(l2capinbuf[8] == L2CAP_CMD_INFORMATION_REQUEST) {
#ifdef DEBUG_USB_HOST
				sprintf(btduartbuff,"\r\nInformation request");
				UARTPrintString(PRINTING_USART, btduartbuff);
#endif
				SPP->SPPservice.identifier = l2capinbuf[9];
				l2cap_information_response(SPP->SPPservice.hci_handle, SPP->SPPservice.identifier, l2capinbuf[12], l2capinbuf[13]);
			}

		} else if(l2capinbuf[6] == SPP->sdp_dcid[0] && l2capinbuf[7] == SPP->sdp_dcid[1]) { // SDP
			if(l2capinbuf[8] == SDP_SERVICE_SEARCH_ATTRIBUTE_REQUEST_PDU) {
				if(((l2capinbuf[16] << 8 | l2capinbuf[17]) == SERIALPORT_UUID) || ((l2capinbuf[16] << 8 | l2capinbuf[17]) == 0x0000 && (l2capinbuf[18] << 8 | l2capinbuf[19]) == SERIALPORT_UUID)) { // Check if it's sending the full UUID, see: https://www.bluetooth.org/Technical/AssignedNumbers/service_discovery.htm, we will just check the first four bytes
					if(SPP->firstMessage) {
						SPPserialPortResponse1(SPP,l2capinbuf[9], l2capinbuf[10]);
						SPP->firstMessage = false;
					} else {
						SPPserialPortResponse2(SPP, l2capinbuf[9], l2capinbuf[10]); // Serialport continuation state
						SPP->firstMessage = true;
					}
				} else if(((l2capinbuf[16] << 8 | l2capinbuf[17]) == L2CAP_UUID) || ((l2capinbuf[16] << 8 | l2capinbuf[17]) == 0x0000 && (l2capinbuf[18] << 8 | l2capinbuf[19]) == L2CAP_UUID)) {
					if(SPP->firstMessage) {
						SPPl2capResponse1(SPP, l2capinbuf[9], l2capinbuf[10]);
						SPP->firstMessage = false;
					} else {
						SPPl2capResponse2(SPP, l2capinbuf[9], l2capinbuf[10]); // L2CAP continuation state
						SPP->firstMessage = true;
					}
				} else
					SPPserviceNotSupported(SPP,l2capinbuf[9], l2capinbuf[10]); // The service is not supported
			}

		} else if(l2capinbuf[6] == SPP->rfcomm_dcid[0] && l2capinbuf[7] == SPP->rfcomm_dcid[1]) { // RFCOMM
			SPP->rfcommChannel = l2capinbuf[8] & 0xF8;
			SPP->rfcommDirection = l2capinbuf[8] & 0x04;
			SPP->rfcommCommandResponse = l2capinbuf[8] & 0x02;
			SPP->rfcommChannelType = l2capinbuf[9] & 0xEF;
			SPP->rfcommPfBit = l2capinbuf[9] & 0x10;

			if(SPP->rfcommChannel >> 3 != 0x00)
				SPP->rfcommChannelConnection = SPP->rfcommChannel;

			if(SPP->rfcommChannelType == RFCOMM_DISC) {
#ifdef DEBUG_USB_HOST
				sprintf(btduartbuff,"\r\nReceived Disconnect RFCOMM Command on channel: %x", SPP->rfcommChannel >> 3);
				UARTPrintString(PRINTING_USART, btduartbuff);
#endif
				SPP->connected = false;
				SPPsendRfcomm(SPP, SPP->rfcommChannel, SPP->rfcommDirection, SPP->rfcommCommandResponse, RFCOMM_UA, SPP->rfcommPfBit, SPP->rfcommbuf, 0x00); // UA Command
			}

			if(SPP->connected) {
				/* Read the incoming message */
				if(SPP->rfcommChannelType == RFCOMM_UIH && SPP->rfcommChannel == SPP->rfcommChannelConnection) {
					uint8_t length = l2capinbuf[10] >> 1; // Get length
					uint8_t offset = l2capinbuf[4] - length - 4; // Check if there is credit
					if(SPPcheckFcs(&l2capinbuf[8], l2capinbuf[11 + length + offset])) {
						uint8_t i = 0;
						for(; i < length; i++) {
							if(SPP->rfcommAvailable + i >= sizeof (SPP->rfcommDataBuffer)) {
#ifdef DEBUG_USB_HOST
								sprintf(btduartbuff,"\r\nWarning: Buffer is full!");
								UARTPrintString(PRINTING_USART, btduartbuff);
#endif
								break;
							}
							SPP->rfcommDataBuffer[SPP->rfcommAvailable + i] = l2capinbuf[11 + i + offset];
						}
						SPP->rfcommAvailable += i;
					}
#ifdef DEBUG_USB_HOST
					else{
						sprintf(btduartbuff,"\r\nError in FCS checksum!");
						UARTPrintString(PRINTING_USART, btduartbuff);
					}
#endif
				} else if(SPP->rfcommChannelType == RFCOMM_UIH && l2capinbuf[11] == BT_RFCOMM_RPN_CMD) { // UIH Remote Port Negotiation Command
#ifdef DEBUG_USB_HOST
					sprintf(btduartbuff,"\r\nReceived UIH Remote Port Negotiation Command");
					UARTPrintString(PRINTING_USART, btduartbuff);
#endif
					SPP->rfcommbuf[0] = BT_RFCOMM_RPN_RSP; // Command
					SPP->rfcommbuf[1] = l2capinbuf[12]; // Length and shiftet like so: length << 1 | 1
					SPP->rfcommbuf[2] = l2capinbuf[13]; // Channel: channel << 1 | 1
					SPP->rfcommbuf[3] = l2capinbuf[14]; // Pre difined for Bluetooth, see 5.5.3 of TS 07.10 Adaption for RFCOMM
					SPP->rfcommbuf[4] = l2capinbuf[15]; // Priority
					SPP->rfcommbuf[5] = l2capinbuf[16]; // Timer
					SPP->rfcommbuf[6] = l2capinbuf[17]; // Max Fram Size LSB
					SPP->rfcommbuf[7] = l2capinbuf[18]; // Max Fram Size MSB
					SPP->rfcommbuf[8] = l2capinbuf[19]; // MaxRatransm.
					SPP->rfcommbuf[9] = l2capinbuf[20]; // Number of Frames
					SPPsendRfcomm(SPP, SPP->rfcommChannel, SPP->rfcommDirection, 0, RFCOMM_UIH, SPP->rfcommPfBit, SPP->rfcommbuf, 0x0A); // UIH Remote Port Negotiation Response
				} else if(SPP->rfcommChannelType == RFCOMM_UIH && l2capinbuf[11] == BT_RFCOMM_MSC_CMD) { // UIH Modem Status Command
#ifdef DEBUG_USB_HOST
					sprintf(btduartbuff,"\r\nSend UIH Modem Status Response");
					UARTPrintString(PRINTING_USART, btduartbuff);
#endif
					SPP->rfcommbuf[0] = BT_RFCOMM_MSC_RSP; // UIH Modem Status Response
					SPP->rfcommbuf[1] = 2 << 1 | 1; // Length and shiftet like so: length << 1 | 1
					SPP->rfcommbuf[2] = l2capinbuf[13]; // Channel: (1 << 0) | (1 << 1) | (0 << 2) | (channel << 3)
					SPP->rfcommbuf[3] = l2capinbuf[14];
					SPPsendRfcomm(SPP,SPP->rfcommChannel, SPP->rfcommDirection, 0, RFCOMM_UIH, SPP->rfcommPfBit, SPP->rfcommbuf, 0x04);
				}
			} else {
				if(SPP->rfcommChannelType == RFCOMM_SABM) { // SABM Command - this is sent twice: once for channel 0 and then for the channel to establish
#ifdef DEBUG_USB_HOST
					sprintf(btduartbuff,"\r\nReceived SABM Command");
					UARTPrintString(PRINTING_USART, btduartbuff);
#endif
					SPPsendRfcomm(SPP,SPP->rfcommChannel, SPP->rfcommDirection, SPP->rfcommCommandResponse, RFCOMM_UA, SPP->rfcommPfBit, SPP->rfcommbuf, 0x00); // UA Command
				} else if(SPP->rfcommChannelType == RFCOMM_UIH && l2capinbuf[11] == BT_RFCOMM_PN_CMD) { // UIH Parameter Negotiation Command
#ifdef DEBUG_USB_HOST
					sprintf(btduartbuff,"\r\nReceived UIH Parameter Negotiation Command");
					UARTPrintString(PRINTING_USART, btduartbuff);
#endif
					SPP->rfcommbuf[0] = BT_RFCOMM_PN_RSP; // UIH Parameter Negotiation Response
					SPP->rfcommbuf[1] = l2capinbuf[12]; // Length and shiftet like so: length << 1 | 1
					SPP->rfcommbuf[2] = l2capinbuf[13]; // Channel: channel << 1 | 1
					SPP->rfcommbuf[3] = 0xE0; // Pre difined for Bluetooth, see 5.5.3 of TS 07.10 Adaption for RFCOMM
					SPP->rfcommbuf[4] = 0x00; // Priority
					SPP->rfcommbuf[5] = 0x00; // Timer
					SPP->rfcommbuf[6] = BULK_MAXPKTSIZE - 14; // Max Fram Size LSB - set to the size of received data (50)
					SPP->rfcommbuf[7] = 0x00; // Max Fram Size MSB
					SPP->rfcommbuf[8] = 0x00; // MaxRatransm.
					SPP->rfcommbuf[9] = 0x00; // Number of Frames
					SPPsendRfcomm(SPP,SPP->rfcommChannel, SPP->rfcommDirection, 0, RFCOMM_UIH, SPP->rfcommPfBit, SPP->rfcommbuf, 0x0A);
				} else if(SPP->rfcommChannelType == RFCOMM_UIH && l2capinbuf[11] == BT_RFCOMM_MSC_CMD) { // UIH Modem Status Command
#ifdef DEBUG_USB_HOST
					sprintf(btduartbuff,"\r\nSend UIH Modem Status Response");
					UARTPrintString(PRINTING_USART, btduartbuff);
#endif
					SPP->rfcommbuf[0] = BT_RFCOMM_MSC_RSP; // UIH Modem Status Response
					SPP->rfcommbuf[1] = 2 << 1 | 1; // Length and shiftet like so: length << 1 | 1
					SPP->rfcommbuf[2] = l2capinbuf[13]; // Channel: (1 << 0) | (1 << 1) | (0 << 2) | (channel << 3)
					SPP->rfcommbuf[3] = l2capinbuf[14];
					SPPsendRfcomm(SPP,SPP->rfcommChannel, SPP->rfcommDirection, 0, RFCOMM_UIH, SPP->rfcommPfBit, SPP->rfcommbuf, 0x04);

					USB_OTG_BSP_mDelay(1);
#ifdef DEBUG_USB_HOST
					sprintf(btduartbuff,"\r\nSend UIH Modem Status Command");
					UARTPrintString(PRINTING_USART, btduartbuff);
#endif
					SPP->rfcommbuf[0] = BT_RFCOMM_MSC_CMD; // UIH Modem Status Command
					SPP->rfcommbuf[1] = 2 << 1 | 1; // Length and shiftet like so: length << 1 | 1
					SPP->rfcommbuf[2] = l2capinbuf[13]; // Channel: (1 << 0) | (1 << 1) | (0 << 2) | (channel << 3)
					SPP->rfcommbuf[3] = 0x8D; // Can receive frames (YES), Ready to Communicate (YES), Ready to Receive (YES), Incomig Call (NO), Data is Value (YES)

					SPPsendRfcomm(SPP,SPP->rfcommChannel, SPP->rfcommDirection, 0, RFCOMM_UIH, SPP->rfcommPfBit, SPP->rfcommbuf, 0x04);
				} else if(SPP->rfcommChannelType == RFCOMM_UIH && l2capinbuf[11] == BT_RFCOMM_MSC_RSP) { // UIH Modem Status Response
					if(!SPP->creditSent) {
#ifdef DEBUG_USB_HOST
						sprintf(btduartbuff,"\r\nSend UIH Command with credit");
						UARTPrintString(PRINTING_USART, btduartbuff);
#endif
						SPPsendRfcommCredit(SPP,SPP->rfcommChannelConnection, SPP->rfcommDirection, 0, RFCOMM_UIH, 0x10, sizeof (SPP->rfcommDataBuffer)); // Send credit
						SPP->creditSent = true;
						SPP->timer = (uint32_t)HCD_GetCurrentFrame(BTD_Machine.pdev);
						SPP->waitForLastCommand = true;
					}
				} else if(SPP->rfcommChannelType == RFCOMM_UIH && l2capinbuf[10] == 0x01) { // UIH Command with credit
#ifdef DEBUG_USB_HOST
					sprintf(btduartbuff,"\r\nReceived UIH Command with credit");
					UARTPrintString(PRINTING_USART, btduartbuff);
#endif
				} else if(SPP->rfcommChannelType == RFCOMM_UIH && l2capinbuf[11] == BT_RFCOMM_RPN_CMD) { // UIH Remote Port Negotiation Command
#ifdef DEBUG_USB_HOST
					sprintf(btduartbuff,"\r\nReceived UIH Remote Port Negotiation Command");
					UARTPrintString(PRINTING_USART, btduartbuff);
#endif
					SPP->rfcommbuf[0] = BT_RFCOMM_RPN_RSP; // Command
					SPP->rfcommbuf[1] = l2capinbuf[12]; // Length and shiftet like so: length << 1 | 1
					SPP->rfcommbuf[2] = l2capinbuf[13]; // Channel: channel << 1 | 1
					SPP->rfcommbuf[3] = l2capinbuf[14]; // Pre difined for Bluetooth, see 5.5.3 of TS 07.10 Adaption for RFCOMM
					SPP->rfcommbuf[4] = l2capinbuf[15]; // Priority
					SPP->rfcommbuf[5] = l2capinbuf[16]; // Timer
					SPP->rfcommbuf[6] = l2capinbuf[17]; // Max Fram Size LSB
					SPP->rfcommbuf[7] = l2capinbuf[18]; // Max Fram Size MSB
					SPP->rfcommbuf[8] = l2capinbuf[19]; // MaxRatransm.
					SPP->rfcommbuf[9] = l2capinbuf[20]; // Number of Frames
					SPPsendRfcomm(SPP,SPP->rfcommChannel, SPP->rfcommDirection, 0, RFCOMM_UIH, SPP->rfcommPfBit, SPP->rfcommbuf, 0x0A); // UIH Remote Port Negotiation Response
#ifdef DEBUG_USB_HOST
					sprintf(btduartbuff,"\r\nRFCOMM Connection is now established\r\n");
					UARTPrintString(PRINTING_USART, btduartbuff);
#endif
					SPPonInit((uint32_t)SPP);
				}
			}
		}

		SPPSDP_task(SPP);
		SPPRFCOMM_task(SPP);
	}
}

void SPPRun(uint32_t pointer_address)
{

	SPP_t*SPP = (SPP_t*) pointer_address;

	if(SPP->waitForLastCommand && (int32_t)((uint32_t)HCD_GetCurrentFrame(BTD_Machine.pdev) - SPP->timer) > 100) { // We will only wait 100ms and see if the UIH Remote Port Negotiation Command is send, as some deviced don't send it
#ifdef DEBUG_USB_HOST
		sprintf(btduartbuff,"\r\nRFCOMM Connection is now established - Automatic\r\n");
		UARTPrintString(PRINTING_USART, btduartbuff);
#endif
		SPPonInit((uint32_t)SPP);
	}
	SPPsend(SPP); // Send all bytes currently in the buffer
}

void SPPonInit(uint32_t pointer_address)
{
	SPP_t*SPP = (SPP_t*) pointer_address;
	SPP->creditSent = false;
	SPP->waitForLastCommand = false;
	SPP->connected = true; // The RFCOMM channel is now established
	SPP->sppIndex = 0;

};

void SPPSDP_task(SPP_t* SPP)
{

	switch(SPP->l2cap_sdp_state) {

	case L2CAP_SDP_WAIT:
		if(l2cap_check_flag(L2CAP_FLAG_CONNECTION_SDP_REQUEST)) {
			l2cap_clear_flag(L2CAP_FLAG_CONNECTION_SDP_REQUEST); // Clear flag
#ifdef DEBUG_USB_HOST
			sprintf(btduartbuff,"\r\nSDP Incoming Connection Request");
			UARTPrintString(PRINTING_USART, btduartbuff);
#endif
			l2cap_connection_response(SPP->SPPservice.hci_handle, SPP->SPPservice.identifier, SPP->sdp_dcid, SPP->sdp_scid, PENDING);
			USB_OTG_BSP_mDelay(1);
			l2cap_connection_response(SPP->SPPservice.hci_handle, SPP->SPPservice.identifier, SPP->sdp_dcid, SPP->sdp_scid, SUCCESSFUL);
			SPP->SPPservice.identifier++;
			USB_OTG_BSP_mDelay(1);
			l2cap_config_request(SPP->SPPservice.hci_handle, SPP->SPPservice.identifier, SPP->sdp_scid);
			SPP->l2cap_sdp_state = L2CAP_SDP_SUCCESS;
		} else if(l2cap_check_flag(L2CAP_FLAG_DISCONNECT_SDP_REQUEST)) {
			l2cap_clear_flag(L2CAP_FLAG_DISCONNECT_SDP_REQUEST); // Clear flag
			SPP->SDPConnected = false;
#ifdef DEBUG_USB_HOST
			sprintf(btduartbuff,"\r\nDisconnected SDP Channel");
			UARTPrintString(PRINTING_USART, btduartbuff);
#endif
			l2cap_disconnection_response(SPP->SPPservice.hci_handle, SPP->SPPservice.identifier,  SPP->sdp_dcid, SPP->sdp_scid);
		}
		break;
	case L2CAP_SDP_SUCCESS:
		if(l2cap_check_flag(L2CAP_FLAG_CONFIG_SDP_SUCCESS)) {
			l2cap_clear_flag(L2CAP_FLAG_CONFIG_SDP_SUCCESS); // Clear flag
#ifdef DEBUG_USB_HOST
			sprintf(btduartbuff,"\r\nSDP Successfully Configured");
			UARTPrintString(PRINTING_USART, btduartbuff);
#endif
			SPP->firstMessage = true; // Reset bool
			SPP->SDPConnected = true;
			SPP->l2cap_sdp_state = L2CAP_SDP_WAIT;
		}
		break;

	case L2CAP_DISCONNECT_RESPONSE: // This is for both disconnection response from the RFCOMM and SDP channel if they were connected
		if(l2cap_check_flag(L2CAP_FLAG_DISCONNECT_RESPONSE)) {
#ifdef DEBUG_USB_HOST
			sprintf(btduartbuff,"\r\nDisconnected L2CAP Connection");
			UARTPrintString(PRINTING_USART, btduartbuff);
#endif
			hci_disconnect(SPP->SPPservice.hci_handle);
			SPP->SPPservice.hci_handle = -1; // Reset handle
			SPP->SPPservice.Reset((uint32_t)SPP);
		}
		break;
	}
}

void SPPRFCOMM_task(SPP_t* SPP) {

	switch(SPP->l2cap_rfcomm_state) {
	case L2CAP_RFCOMM_WAIT:
		if(l2cap_check_flag(L2CAP_FLAG_CONNECTION_RFCOMM_REQUEST)) {
			l2cap_clear_flag(L2CAP_FLAG_CONNECTION_RFCOMM_REQUEST); // Clear flag
#ifdef DEBUG_USB_HOST
			sprintf(btduartbuff,"\r\nRFCOMM Incoming Connection Request");
			UARTPrintString(PRINTING_USART, btduartbuff);
#endif
			l2cap_connection_response(SPP->SPPservice.hci_handle, SPP->SPPservice.identifier, SPP->rfcomm_dcid, SPP->rfcomm_scid, PENDING);
			USB_OTG_BSP_mDelay(1);
			l2cap_connection_response(SPP->SPPservice.hci_handle, SPP->SPPservice.identifier, SPP->rfcomm_dcid, SPP->rfcomm_scid, SUCCESSFUL);
			SPP->SPPservice.identifier++;
			USB_OTG_BSP_mDelay(1);
			l2cap_config_request(SPP->SPPservice.hci_handle, SPP->SPPservice.identifier, SPP->rfcomm_scid);
			SPP->l2cap_rfcomm_state = L2CAP_RFCOMM_SUCCESS;
		} else if(l2cap_check_flag(L2CAP_FLAG_DISCONNECT_RFCOMM_REQUEST)) {
			l2cap_clear_flag(L2CAP_FLAG_DISCONNECT_RFCOMM_REQUEST); // Clear flag
			SPP->RFCOMMConnected = false;
			SPP->connected = false;
#ifdef DEBUG_USB_HOST
			sprintf(btduartbuff,"\r\nDisconnected RFCOMM Channel");
			UARTPrintString(PRINTING_USART, btduartbuff);
#endif
			l2cap_disconnection_response(SPP->SPPservice.hci_handle, SPP->SPPservice.identifier, SPP->rfcomm_dcid, SPP->rfcomm_scid);
		}
		break;
	case L2CAP_RFCOMM_SUCCESS:
		if(l2cap_check_flag(L2CAP_FLAG_CONFIG_RFCOMM_SUCCESS)) {
			l2cap_clear_flag(L2CAP_FLAG_CONFIG_RFCOMM_SUCCESS); // Clear flag
#ifdef DEBUG_USB_HOST
			sprintf(btduartbuff,"\r\nRFCOMM Successfully Configured");
			UARTPrintString(PRINTING_USART, btduartbuff);
#endif
			SPP->rfcommAvailable = 0; // Reset number of bytes available
			SPP->bytesRead = 0; // Reset number of bytes received
			SPP->RFCOMMConnected = true;
			SPP->l2cap_rfcomm_state = L2CAP_RFCOMM_WAIT;
		}
		break;
	}
}

/************************************************************/
/*                    SDP Commands                          */
/************************************************************/
void SPPSDP_Command(SPP_t* SPP, uint8_t* data, uint8_t nbytes) // See page 223 in the Bluetooth specs
{
    L2CAP_Command(SPP->SPPservice.hci_handle, data, nbytes, SPP->sdp_scid[0], SPP->sdp_scid[1]);
}

void SPPserviceNotSupported(SPP_t* SPP, uint8_t transactionIDHigh, uint8_t transactionIDLow) // See page 235 in the Bluetooth specs
{
	SPP->l2capoutbuf[0] = SDP_SERVICE_SEARCH_ATTRIBUTE_RESPONSE_PDU;
	SPP->l2capoutbuf[1] = transactionIDHigh;
	SPP->l2capoutbuf[2] = transactionIDLow;
	SPP->l2capoutbuf[3] = 0x00; // MSB Parameter Length
	SPP->l2capoutbuf[4] = 0x05; // LSB Parameter Length = 5
	SPP->l2capoutbuf[5] = 0x00; // MSB AttributeListsByteCount
	SPP->l2capoutbuf[6] = 0x02; // LSB AttributeListsByteCount = 2

    /* Attribute ID/Value Sequence: */
	SPP->l2capoutbuf[7] = 0x35; // Data element sequence - length in next byte
	SPP->l2capoutbuf[8] = 0x00; // Length = 0
	SPP->l2capoutbuf[9] = 0x00; // No continuation state

    SPPSDP_Command(SPP, SPP->l2capoutbuf, 10);
}

void SPPserialPortResponse1(SPP_t* SPP, uint8_t transactionIDHigh, uint8_t transactionIDLow)
{
	SPP->l2capoutbuf[0] = SDP_SERVICE_SEARCH_ATTRIBUTE_RESPONSE_PDU;
	SPP->l2capoutbuf[1] = transactionIDHigh;
	SPP->l2capoutbuf[2] = transactionIDLow;
	SPP->l2capoutbuf[3] = 0x00; // MSB Parameter Length
	SPP->l2capoutbuf[4] = 0x2B; // LSB Parameter Length = 43
	SPP->l2capoutbuf[5] = 0x00; // MSB AttributeListsByteCount
	SPP->l2capoutbuf[6] = 0x26; // LSB AttributeListsByteCount = 38

        /* Attribute ID/Value Sequence: */
	SPP->l2capoutbuf[7] = 0x36; // Data element sequence - length in next two bytes
	SPP->l2capoutbuf[8] = 0x00; // MSB Length
	SPP->l2capoutbuf[9] = 0x3C; // LSB Length = 60

	SPP->l2capoutbuf[10] = 0x36; // Data element sequence - length in next two bytes
	SPP->l2capoutbuf[11] = 0x00; // MSB Length
	SPP->l2capoutbuf[12] = 0x39; // LSB Length = 57

	SPP->l2capoutbuf[13] = 0x09; // Unsigned Integer - length 2 bytes
	SPP->l2capoutbuf[14] = 0x00; // MSB ServiceRecordHandle
	SPP->l2capoutbuf[15] = 0x00; // LSB ServiceRecordHandle
	SPP->l2capoutbuf[16] = 0x0A; // Unsigned int - length 4 bytes
	SPP->l2capoutbuf[17] = 0x00; // ServiceRecordHandle value - TODO: Is this related to HCI_Handle?
	SPP->l2capoutbuf[18] = 0x01;
	SPP->l2capoutbuf[19] = 0x00;
	SPP->l2capoutbuf[20] = 0x06;

	SPP->l2capoutbuf[21] = 0x09; // Unsigned Integer - length 2 bytes
	SPP->l2capoutbuf[22] = 0x00; // MSB ServiceClassIDList
	SPP->l2capoutbuf[23] = 0x01; // LSB ServiceClassIDList
	SPP->l2capoutbuf[24] = 0x35; // Data element sequence - length in next byte
	SPP->l2capoutbuf[25] = 0x03; // Length = 3
	SPP->l2capoutbuf[26] = 0x19; // UUID (universally unique identifier) - length = 2 bytes
	SPP->l2capoutbuf[27] = 0x11; // MSB SerialPort
	SPP->l2capoutbuf[28] = 0x01; // LSB SerialPort

	SPP->l2capoutbuf[29] = 0x09; // Unsigned Integer - length 2 bytes
	SPP->l2capoutbuf[30] = 0x00; // MSB ProtocolDescriptorList
	SPP->l2capoutbuf[31] = 0x04; // LSB ProtocolDescriptorList
	SPP->l2capoutbuf[32] = 0x35; // Data element sequence - length in next byte
	SPP->l2capoutbuf[33] = 0x0C; // Length = 12

	SPP->l2capoutbuf[34] = 0x35; // Data element sequence - length in next byte
	SPP->l2capoutbuf[35] = 0x03; // Length = 3
	SPP->l2capoutbuf[36] = 0x19; // UUID (universally unique identifier) - length = 2 bytes
	SPP->l2capoutbuf[37] = 0x01; // MSB L2CAP
	SPP->l2capoutbuf[38] = 0x00; // LSB L2CAP

	SPP->l2capoutbuf[39] = 0x35; // Data element sequence - length in next byte
	SPP->l2capoutbuf[40] = 0x05; // Length = 5
	SPP->l2capoutbuf[41] = 0x19; // UUID (universally unique identifier) - length = 2 bytes
	SPP->l2capoutbuf[42] = 0x00; // MSB RFCOMM
	SPP->l2capoutbuf[43] = 0x03; // LSB RFCOMM
	SPP->l2capoutbuf[44] = 0x08; // Unsigned Integer - length 1 byte

	SPP->l2capoutbuf[45] = 0x02; // ContinuationState - Two more bytes
	SPP->l2capoutbuf[46] = 0x00; // MSB length
	SPP->l2capoutbuf[47] = 0x19; // LSB length = 25 more bytes to come

    SPPSDP_Command(SPP,SPP->l2capoutbuf, 48);
}

void SPPserialPortResponse2(SPP_t* SPP,uint8_t transactionIDHigh, uint8_t transactionIDLow)
{
	SPP->l2capoutbuf[0] = SDP_SERVICE_SEARCH_ATTRIBUTE_RESPONSE_PDU;
	SPP->l2capoutbuf[1] = transactionIDHigh;
	SPP->l2capoutbuf[2] = transactionIDLow;
	SPP->l2capoutbuf[3] = 0x00; // MSB Parameter Length
	SPP->l2capoutbuf[4] = 0x1C; // LSB Parameter Length = 28
	SPP->l2capoutbuf[5] = 0x00; // MSB AttributeListsByteCount
	SPP->l2capoutbuf[6] = 0x19; // LSB AttributeListsByteCount = 25

	/* Attribute ID/Value Sequence: */
	SPP->l2capoutbuf[7] = 0x01; // Channel 1 - TODO: Try different values, so multiple servers can be used at once

	SPP->l2capoutbuf[8] = 0x09; // Unsigned Integer - length 2 bytes
	SPP->l2capoutbuf[9] = 0x00; // MSB LanguageBaseAttributeIDList
	SPP->l2capoutbuf[10] = 0x06; // LSB LanguageBaseAttributeIDList
	SPP->l2capoutbuf[11] = 0x35; // Data element sequence - length in next byte
	SPP->l2capoutbuf[12] = 0x09; // Length = 9

	// Identifier representing the natural language = en = English - see: "ISO 639:1988"
	SPP->l2capoutbuf[13] = 0x09; // Unsigned Integer - length 2 bytes
	SPP->l2capoutbuf[14] = 0x65; // 'e'
	SPP->l2capoutbuf[15] = 0x6E; // 'n'

	// "The second element of each triplet contains an identifier that specifies a character encoding used for the language"
	// Encoding is set to 106 (UTF-8) - see: http://www.iana.org/assignments/character-sets/character-sets.xhtml
	SPP->l2capoutbuf[16] = 0x09; // Unsigned Integer - length 2 bytes
	SPP->l2capoutbuf[17] = 0x00; // MSB of character encoding
	SPP->l2capoutbuf[18] = 0x6A; // LSB of character encoding (106)

	// Attribute ID that serves as the base attribute ID for the natural language in the service record
	// "To facilitate the retrieval of human-readable universal attributes in a principal language, the base attribute ID value for the primary language supported by a service record shall be 0x0100"
	SPP->l2capoutbuf[19] = 0x09; // Unsigned Integer - length 2 bytes
	SPP->l2capoutbuf[20] = 0x01;
	SPP->l2capoutbuf[21] = 0x00;

	SPP->l2capoutbuf[22] = 0x09; // Unsigned Integer - length 2 bytes
	SPP->l2capoutbuf[23] = 0x01; // MSB ServiceDescription
	SPP->l2capoutbuf[24] = 0x00; // LSB ServiceDescription

	SPP->l2capoutbuf[25] = 0x25; // Text string - length in next byte
	SPP->l2capoutbuf[26] = 0x05; // Name length
	SPP->l2capoutbuf[27] = 'T';
	SPP->l2capoutbuf[28] = 'K';
	SPP->l2capoutbuf[29] = 'J';
	SPP->l2capoutbuf[30] = 'S';
	SPP->l2capoutbuf[31] = 'P';
	SPP->l2capoutbuf[32] = 0x00; // No continuation state

    SPPSDP_Command(SPP, SPP->l2capoutbuf, 33);
}

void SPPl2capResponse1(SPP_t* SPP,uint8_t transactionIDHigh, uint8_t transactionIDLow)
{
    SPPserialPortResponse1(SPP, transactionIDHigh, transactionIDLow); // These has to send all the supported functions, since it only supports virtual serialport it just sends the message again
}

void SPPl2capResponse2(SPP_t* SPP,uint8_t transactionIDHigh, uint8_t transactionIDLow)
{
    SPPserialPortResponse2(SPP, transactionIDHigh, transactionIDLow); // Same data as serialPortResponse2
}

/************************************************************/
/*                    RFCOMM Commands                       */

/************************************************************/
void SPPRFCOMM_Command(SPP_t* SPP, uint8_t* data, uint8_t nbytes)
{
	L2CAP_Command(SPP->SPPservice.hci_handle, data, nbytes, SPP->rfcomm_scid[0], SPP->rfcomm_scid[1]);
}

void SPPsendRfcomm(SPP_t* SPP, uint8_t channel, uint8_t direction, uint8_t CR, uint8_t channelType, uint8_t pfBit, uint8_t* data, uint8_t length)
{
	SPP->l2capoutbuf[0] = channel | direction | CR | extendAddress; // RFCOMM Address
	SPP->l2capoutbuf[1] = channelType | pfBit; // RFCOMM Control
	SPP->l2capoutbuf[2] = length << 1 | 0x01; // Length and format (always 0x01 bytes format)
	uint8_t i = 0;
	for(; i < length; i++)
		SPP->l2capoutbuf[i + 3] = data[i];
	SPP->l2capoutbuf[i + 3] = SPPcalcFcs(SPP->l2capoutbuf);
	SPPRFCOMM_Command(SPP, SPP->l2capoutbuf, length + 4);
}

void SPPsendRfcommCredit(SPP_t* SPP, uint8_t channel, uint8_t direction, uint8_t CR, uint8_t channelType, uint8_t pfBit, uint8_t credit) {
	SPP->l2capoutbuf[0] = channel | direction | CR | extendAddress; // RFCOMM Address
	SPP->l2capoutbuf[1] = channelType | pfBit; // RFCOMM Control
	SPP->l2capoutbuf[2] = 0x01; // Length = 0
	SPP->l2capoutbuf[3] = credit; // Credit
	SPP->l2capoutbuf[4] = SPPcalcFcs(SPP->l2capoutbuf);
	SPPRFCOMM_Command(SPP,SPP->l2capoutbuf, 5);
}

/* CRC on 2 bytes */
uint8_t SPPcrc(uint8_t *data) {
	return (rfcomm_crc_table[rfcomm_crc_table[0xFF ^ data[0]] ^ data[1]]);
}

/* Calculate FCS */
uint8_t SPPcalcFcs(uint8_t *data) {
	uint8_t temp = SPPcrc(data);
	if((data[1] & 0xEF) == RFCOMM_UIH)
		return (0xFF - temp); // FCS on 2 bytes
	else
		return (0xFF - rfcomm_crc_table[temp ^ data[2]]); // FCS on 3 bytes
}

/* Check FCS */
bool SPPcheckFcs(uint8_t *data, uint8_t fcs) {
	uint8_t temp = SPPcrc(data);
	if((data[1] & 0xEF) != RFCOMM_UIH)
		temp = rfcomm_crc_table[temp ^ data[2]]; // FCS on 3 bytes
	return (rfcomm_crc_table[temp ^ fcs] == 0xCF);
}

void SPPwrite(SPP_t* SPP,uint8_t data)
{
	write(SPP, &data, 1);
}

void write(SPP_t* SPP, const uint8_t *data, size_t size)
{

	for(uint8_t i = 0; i < size; i++) {
		if(SPP->sppIndex >= sizeof (SPP->sppOutputBuffer) / sizeof (SPP->sppOutputBuffer[0]))
			SPPsend(SPP); // Send the current data in the buffer
		SPP->sppOutputBuffer[SPP->sppIndex++] = data[i]; // All the bytes are put into a buffer and then send using the send() function
	}

}

void SPPsend(SPP_t* SPP)
{

	if(!SPP->connected || !SPP->sppIndex)
		return;
	uint8_t length; // This is the length of the string we are sending
	uint8_t offset = 0; // This is used to keep track of where we are in the string

	 SPP->l2capoutbuf[0] = SPP->rfcommChannelConnection | 0 | 0 | extendAddress; // RFCOMM Address
	 SPP->l2capoutbuf[1] = RFCOMM_UIH; // RFCOMM Control

	while( SPP->sppIndex) { // We will run this while loop until this variable is 0
		if( SPP->sppIndex > (sizeof ( SPP->l2capoutbuf) - 4)) // Check if the string is larger than the outgoing buffer
			length = sizeof ( SPP->l2capoutbuf) - 4;
		else
			length =  SPP->sppIndex;

		 SPP->l2capoutbuf[2] = length << 1 | 1; // Length
		uint8_t i = 0;
		for(; i < length; i++)
			 SPP->l2capoutbuf[i + 3] =  SPP->sppOutputBuffer[i + offset];
		 SPP->l2capoutbuf[i + 3] = SPPcalcFcs( SPP->l2capoutbuf); // Calculate checksum

		SPPRFCOMM_Command(SPP, SPP->l2capoutbuf, length + 4);

		SPP->sppIndex -= length;
		offset += length; // Increment the offset
	}
}

int SPPavailable(SPP_t* SPP)
{
	return SPP->rfcommAvailable;
}

void SPPdiscard(SPP_t* SPP) {
	SPP->rfcommAvailable = 0;
}

int SPPpeek(SPP_t* SPP) {
	if(SPP->rfcommAvailable == 0) // Don't read if there is nothing in the buffer
		return -1;
	return SPP->rfcommDataBuffer[0];
}

int SPPread(SPP_t* SPP) {
	if(SPP->rfcommAvailable == 0) // Don't read if there is nothing in the buffer
		return -1;
	uint8_t output = SPP->rfcommDataBuffer[0];
	for(uint8_t i = 1; i < SPP->rfcommAvailable; i++)
		SPP->rfcommDataBuffer[i - 1] =SPP-> rfcommDataBuffer[i]; // Shift the buffer one left
	SPP->rfcommAvailable--;
	SPP->bytesRead++;
	if(SPP->bytesRead > (sizeof (SPP->rfcommDataBuffer) - 5)) { // We will send the command just before it runs out of credit
		SPP->bytesRead = 0;
		SPPsendRfcommCredit(SPP,SPP->rfcommChannelConnection, SPP->rfcommDirection, 0, RFCOMM_UIH, 0x10, sizeof (SPP->rfcommDataBuffer)); // Send more credit
	}
	return output;
}

/** Send out all bytes in the buffer. */
void SPPflush(SPP_t* SPP) {
	   SPPsend(SPP);
};
