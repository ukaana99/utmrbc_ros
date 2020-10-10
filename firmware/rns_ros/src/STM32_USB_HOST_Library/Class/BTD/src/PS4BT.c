	#include "STM32_USB_HOST_Library/Class/BTD/inc/PS4BT.h"
#include <stdbool.h>

/* Macros for L2CAP event flag tests */
#define l2cap_check_flag(flag) (PS4BT->PS4BTservice.l2cap_event_flag & (flag))
#define l2cap_set_flag(flag) (PS4BT->PS4BTservice.l2cap_event_flag |= (flag))
#define l2cap_clear_flag(flag) (PS4BT->PS4BTservice.l2cap_event_flag &= ~(flag))

enum DPADEnum {
	DPAD_UP = 0x0,
	DPAD_UP_RIGHT = 0x1,
	DPAD_RIGHT = 0x2,
	DPAD_RIGHT_DOWN = 0x3,
	DPAD_DOWN = 0x4,
	DPAD_DOWN_LEFT = 0x5,
	DPAD_LEFT = 0x6,
	DPAD_LEFT_UP = 0x7,
	DPAD_OFF = 0x8,
};

/** Buttons on the controller */
const uint8_t PS4_BUTTONS[] = {
	UP, // UP
	RIGHT, // RIGHT
	DOWN, // DOWN
	LEFT, // LEFT

	0x0C, // SHARE
	0x0D, // OPTIONS
	0x0E, // L3
	0x0F, // R3

	0x0A, // L2
	0x0B, // R2
	0x08, // L1
	0x09, // R1

	0x07, // TRIANGLE
	0x06, // CIRCLE
	0x05, // CROSS
	0x04, // SQUARE

	0x10, // PS
	0x11, // TOUCHPAD
};

void PS4BTInit(PS4BT_t *PS4BT, bool pair) {

	BTD_Machine.pairWithHIDDevice = pair;
	BTD_Machine.btdPin = "0000";

	/* Set device cid for the control and intterrupt channelse - LSB */
	PS4BT->control_dcid[0] = 0x70; // 0x0070
	PS4BT->control_dcid[1] = 0x00;
	PS4BT->interrupt_dcid[0] = 0x71; // 0x0071
	PS4BT->interrupt_dcid[1] = 0x00;

	PS4BT->PS4BTservice.ACLData = PS4BTACLData;
	PS4BT->PS4BTservice.Reset = PS4BTReset;
	PS4BT->PS4BTservice.Run = PS4BTRun;
	PS4BT->PS4BTservice.disconnect = PS4BTdisconnect;
	PS4BT->PS4BTservice.onInit = OnInit;

	PS4BT->PS4BTserviceID = registerBluetoothService((uint32_t)PS4BT, &PS4BT->PS4BTservice);
	PS4BT->PS4BTservice.Reset(BTD_Machine.btservive_pointer_address[PS4BT->PS4BTserviceID]);

}

void PS4BTReset(uint32_t pointer_address) {

	PS4BT_t *PS4BT = (PS4BT_t*)pointer_address;

	PS4BT->connected = false;
	PS4BT->activeConnection = false;
	PS4BT->PS4BTservice.l2cap_event_flag = 0; // Reset flags
	PS4BT->l2cap_state = L2CAP_WAIT;

	uint8_t i;
	for (i = 0; i < sizeof(PS4BT->ps4Data.hatValue); i++)
		PS4BT->ps4Data.hatValue[i] = 127; // Center value
	PS4BT->ps4Data.btn.val = 0;
	PS4BT->oldButtonState.val = 0;
	for (i = 0; i < sizeof(PS4BT->ps4Data.trigger); i++)
		PS4BT->ps4Data.trigger[i] = 0;
	for (i = 0; i < sizeof(PS4BT->ps4Data.xy)/sizeof(PS4BT->ps4Data.xy[0]); i++) {
		for (uint8_t j = 0; j < sizeof(PS4BT->ps4Data.xy[0].finger)/sizeof(PS4BT->ps4Data.xy[0].finger[0]); j++)
			PS4BT->ps4Data.xy[i].finger[j].touching = 1; // The bit is cleared if the finger is touching the touchpad
	}

	PS4BT->ps4Data.btn.dpad = DPAD_OFF;
	PS4BT->oldButtonState.dpad = DPAD_OFF;
	PS4BT->buttonClickState.dpad = 0;
	PS4BT->oldDpad = 0;

	PS4BT->ps4Output.bigRumble = PS4BT->ps4Output.smallRumble = 0;
	PS4BT->ps4Output.r = PS4BT->ps4Output.g = PS4BT->ps4Output.b = 0;
	PS4BT->ps4Output.flashOn = PS4BT->ps4Output.flashOff = 0;
	PS4BT->ps4Output.reportChanged = false;
}

void PS4BTdisconnect(uint32_t pointer_address) { // Use this void to disconnect the device

	PS4BT_t *PS4BT = (PS4BT_t*)pointer_address;
	// First the HID interrupt channel has to be disconnected, then the HID control channel and finally the HCI connection
	l2cap_disconnection_request(PS4BT->PS4BTservice.hci_handle, ++PS4BT->PS4BTservice.identifier, PS4BT->interrupt_scid, PS4BT->interrupt_dcid);
	PS4BTReset(pointer_address);
	PS4BT->l2cap_state = L2CAP_INTERRUPT_DISCONNECT;

}

void PS4BTACLData(uint8_t* l2capinbuf, uint32_t pointer_address) {

	PS4BT_t *PS4BT = (PS4BT_t*)pointer_address;

	if(!BTD_Machine.l2capConnectionClaimed && BTD_Machine.incomingHIDDevice && !PS4BT->connected && !PS4BT->activeConnection) {
		if(l2capinbuf[8] == L2CAP_CMD_CONNECTION_REQUEST) {
			if((l2capinbuf[12] | (l2capinbuf[13] << 8)) == HID_CTRL_PSM) {
				BTD_Machine.incomingHIDDevice = false;
				BTD_Machine.l2capConnectionClaimed = true; // Claim that the incoming connection belongs to this service
				PS4BT->activeConnection = true;
				PS4BT->PS4BTservice.hci_handle = BTD_Machine.hci_handle; // Store the HCI Handle for the connection
				PS4BT->l2cap_state = L2CAP_WAIT;
			}
		}
	}

	if(checkHciHandle(l2capinbuf, PS4BT->PS4BTservice.hci_handle)) { // acl_handle_ok
		if((l2capinbuf[6] | (l2capinbuf[7] << 8)) == 0x0001U) { // l2cap_control - Channel ID for ACL-U
			if(l2capinbuf[8] == L2CAP_CMD_COMMAND_REJECT) {

#ifdef DEBUG_USB_HOST
				sprintf(btduartbuff,"\r\nL2CAP Command Rejected - Reason: %X: %X: %X: %X: %X: %X ",l2capinbuf[13],l2capinbuf[12],l2capinbuf[17],l2capinbuf[16],l2capinbuf[15],l2capinbuf[14]);
				UARTPrintString(PRINTING_USART, btduartbuff);
#endif
			} else if(l2capinbuf[8] == L2CAP_CMD_CONNECTION_RESPONSE) {
				if(((l2capinbuf[16] | (l2capinbuf[17] << 8)) == 0x0000) && ((l2capinbuf[18] | (l2capinbuf[19] << 8)) == SUCCESSFUL)) { // Success
					if(l2capinbuf[14] == PS4BT->control_dcid[0] && l2capinbuf[15] == PS4BT->control_dcid[1]) {
						//Notify(PSTR("\r\nHID Control Connection Complete"), 0x80);
						PS4BT->PS4BTservice.identifier = l2capinbuf[9];
						PS4BT->control_scid[0] = l2capinbuf[12];
						PS4BT-> control_scid[1] = l2capinbuf[13];
						l2cap_set_flag(L2CAP_FLAG_CONTROL_CONNECTED);
					} else if(l2capinbuf[14] == PS4BT->interrupt_dcid[0] && l2capinbuf[15] == PS4BT->interrupt_dcid[1]) {
						//Notify(PSTR("\r\nHID Interrupt Connection Complete"), 0x80);
						PS4BT->PS4BTservice.identifier = l2capinbuf[9];
						PS4BT->interrupt_scid[0] = l2capinbuf[12];
						PS4BT->interrupt_scid[1] = l2capinbuf[13];
						l2cap_set_flag(L2CAP_FLAG_INTERRUPT_CONNECTED);
					}
				}
			} else if(l2capinbuf[8] == L2CAP_CMD_CONNECTION_REQUEST) {
				if((l2capinbuf[12] | (l2capinbuf[13] << 8)) == HID_CTRL_PSM) {
					PS4BT->PS4BTservice.identifier = l2capinbuf[9];
					PS4BT->control_scid[0] = l2capinbuf[14];
					PS4BT->control_scid[1] = l2capinbuf[15];
					l2cap_set_flag(L2CAP_FLAG_CONNECTION_CONTROL_REQUEST);
				} else if((l2capinbuf[12] | (l2capinbuf[13] << 8)) == HID_INTR_PSM) {
					PS4BT->PS4BTservice.identifier = l2capinbuf[9];
					PS4BT->interrupt_scid[0] = l2capinbuf[14];
					PS4BT->interrupt_scid[1] = l2capinbuf[15];
					l2cap_set_flag(L2CAP_FLAG_CONNECTION_INTERRUPT_REQUEST);
				}

			} else if(l2capinbuf[8] == L2CAP_CMD_CONFIG_RESPONSE) {
				if((l2capinbuf[16] | (l2capinbuf[17] << 8)) == 0x0000) { // Success
					if(l2capinbuf[12] == PS4BT->control_dcid[0] && l2capinbuf[13] == PS4BT->control_dcid[1]) {
						//Notify(PSTR("\r\nHID Control Configuration Complete"), 0x80);
						PS4BT->PS4BTservice.identifier = l2capinbuf[9];
						l2cap_set_flag(L2CAP_FLAG_CONFIG_CONTROL_SUCCESS);
					} else if(l2capinbuf[12] == PS4BT->interrupt_dcid[0] && l2capinbuf[13] == PS4BT->interrupt_dcid[1]) {
						//Notify(PSTR("\r\nHID Interrupt Configuration Complete"), 0x80);
						PS4BT->PS4BTservice.identifier = l2capinbuf[9];
						l2cap_set_flag(L2CAP_FLAG_CONFIG_INTERRUPT_SUCCESS);
					}
				}
			} else if(l2capinbuf[8] == L2CAP_CMD_CONFIG_REQUEST) {

				if(l2capinbuf[12] == PS4BT->control_dcid[0] && l2capinbuf[13] == PS4BT->control_dcid[1]) {
					//Notify(PSTR("\r\nHID Control Configuration Request"), 0x80);
					l2cap_config_response(PS4BT->PS4BTservice.hci_handle, l2capinbuf[9], PS4BT->control_scid);
				} else if(l2capinbuf[12] == PS4BT->interrupt_dcid[0] && l2capinbuf[13] == PS4BT->interrupt_dcid[1]) {
					//Notify(PSTR("\r\nHID Interrupt Configuration Request"), 0x80);
					l2cap_config_response(PS4BT->PS4BTservice.hci_handle, l2capinbuf[9], PS4BT->interrupt_scid);
				}

			} else if(l2capinbuf[8] == L2CAP_CMD_DISCONNECT_REQUEST) {

				if(l2capinbuf[12] == PS4BT->control_dcid[0] && l2capinbuf[13] == PS4BT->control_dcid[1]) {
#ifdef DEBUG_USB_HOST
					sprintf(btduartbuff,"\r\nDisconnect Request: Control Channel");
					UARTPrintString(PRINTING_USART, btduartbuff);
#endif
					PS4BT->PS4BTservice.identifier = l2capinbuf[9];
					l2cap_disconnection_response(PS4BT->PS4BTservice.hci_handle, PS4BT->PS4BTservice.identifier, PS4BT->control_dcid, PS4BT->control_scid);
					PS4BTReset(pointer_address);
				} else if(l2capinbuf[12] == PS4BT->interrupt_dcid[0] && l2capinbuf[13] == PS4BT->interrupt_dcid[1]) {
#ifdef DEBUG_USB_HOST
					sprintf(btduartbuff,"\r\nDisconnect Request: Interrupt Channel");
					UARTPrintString(PRINTING_USART, btduartbuff);
#endif
					PS4BT->PS4BTservice.identifier = l2capinbuf[9];
					l2cap_disconnection_response(PS4BT->PS4BTservice.hci_handle, PS4BT->PS4BTservice.identifier, PS4BT->interrupt_dcid, PS4BT->interrupt_dcid);
					PS4BTReset(pointer_address);
				}

			} else if(l2capinbuf[8] == L2CAP_CMD_DISCONNECT_RESPONSE) {

				if(l2capinbuf[12] == PS4BT->control_scid[0] && l2capinbuf[13] == PS4BT->control_scid[1]) {
					//Notify(PSTR("\r\nDisconnect Response: Control Channel"), 0x80);
					PS4BT->PS4BTservice.identifier = l2capinbuf[9];
					l2cap_set_flag(L2CAP_FLAG_DISCONNECT_CONTROL_RESPONSE);
				} else if(l2capinbuf[12] == PS4BT->interrupt_scid[0] && l2capinbuf[13] == PS4BT->interrupt_scid[1]) {
					//Notify(PSTR("\r\nDisconnect Response: Interrupt Channel"), 0x80);
					PS4BT->PS4BTservice.identifier = l2capinbuf[9];
					l2cap_set_flag(L2CAP_FLAG_DISCONNECT_INTERRUPT_RESPONSE);
				}

			}

		} else if(l2capinbuf[6] == PS4BT->interrupt_dcid[0] && l2capinbuf[7] == PS4BT->interrupt_dcid[1]) { // l2cap_interrupt

			if(l2capinbuf[8] == 0xA1) { // HID_THDR_DATA_INPUT
				uint16_t length = ((uint16_t)l2capinbuf[5] << 8 | l2capinbuf[4]);
				ParseBTHIDData(PS4BT, (uint8_t)(length - 1), &l2capinbuf[9]);

				if(ps4_joyR_up){
					PS4BT->joyR_y  = ((ps4_low_Ry - ((float)PS4BT->ps4_rightjoy_y))/100.0)/1.00;
					if(PS4BT->joyR_y < 0.0){
						PS4BT->joyR_y = 0.0;
					}else if(PS4BT->joyR_y > 1.0){
						PS4BT->joyR_y = 1.0;
					}
				}else if(ps4_joyR_down){
					PS4BT->joyR_y = ((ps4_high_Ry - ((float)PS4BT->ps4_rightjoy_y))/100.0)/1.00;
					if(PS4BT->joyR_y > 0.0){
						PS4BT->joyR_y = 0.0;
					}else if(PS4BT->joyR_y < -1.0){
						PS4BT->joyR_y = -1.0;
					}
				}else{
					PS4BT->joyR_y = 0.0;
				}

				if(ps4_joyR_left){
					PS4BT->joyR_x = ((ps4_low_Rx - ((float)PS4BT->ps4_rightjoy_x))/100.0)/1.00;
					if(PS4BT->joyR_x < 0.0){
						PS4BT->joyR_x = 0.0;
					}else if(PS4BT->joyR_x > 1.0){
						PS4BT->joyR_x = 1.0;
					}
				}else if(ps4_joyR_right){
					PS4BT->joyR_x = ((ps4_high_Rx - ((float)PS4BT->ps4_rightjoy_x))/100.0)/1.00;
					if(PS4BT->joyR_x > 0.0){
						PS4BT->joyR_x = 0.0;
					}else if(PS4BT->joyR_x < -1.0){
						PS4BT->joyR_x = -1.0;
					}
				}else{
					PS4BT->joyR_x = 0.0;
				}

				if(ps4_joyL_up){
					PS4BT->joyL_y = ((ps4_low_Ly - ((float)PS4BT->ps4_leftjoy_y))/100.0)/1.00;
					if(PS4BT->joyL_y < 0.0){
						PS4BT->joyL_y = 0.0;
					}else if(PS4BT->joyL_y > 1.0){
						PS4BT->joyL_y = 1.0;
					}
				}else if(ps4_joyL_down){
					PS4BT->joyL_y = ((ps4_high_Ly - ((float)PS4BT->ps4_leftjoy_y))/100.0)/1.00;
					if(PS4BT->joyL_y > 0.0){
						PS4BT->joyL_y = 0.0;
					}else if(PS4BT->joyL_y < -1.0){
						PS4BT->joyL_y = -1.0;
					}
				}else{
					PS4BT->joyL_y = 0.0;
				}

				if(ps4_joyL_left){
					PS4BT->joyL_x = ((ps4_low_Lx - ((float)PS4BT->ps4_leftjoy_x))/100.0)/1.00;
					if(PS4BT->joyL_x < 0.0){
						PS4BT->joyL_x = 0.0;
					}else if(PS4BT->joyL_x > 1.0){
						PS4BT->joyL_x = 1.0;
					}
				}else if(ps4_joyL_right){
					PS4BT->joyL_x = ((ps4_high_Lx - ((float)PS4BT->ps4_leftjoy_x))/100.0)/1.00;
					if(PS4BT->joyL_x > 0.0){
						PS4BT->joyL_x = 0.0;
					}else if(PS4BT->joyL_x < -1.0){
						PS4BT->joyL_x = -1.0;
					}
				}else{
					PS4BT->joyL_x = 0.0;
				}

				PS4BT->joyR_2 = (PS4BT->ps4_an_R2/255.0);
				PS4BT->joyL_2 = (PS4BT->ps4_an_L2/255.0);
			}
		} else if(l2capinbuf[6] == PS4BT->control_dcid[0] && l2capinbuf[7] == PS4BT->control_dcid[1]) { // l2cap_control

		}

		PS4BTL2CAP_task(PS4BT);
	}
}

void PS4BTL2CAP_task(PS4BT_t *PS4BT) {

	switch(PS4BT->l2cap_state) {

	/* These states are used if the HID device is the host */
	case L2CAP_CONTROL_SUCCESS:

		if(l2cap_check_flag(L2CAP_FLAG_CONFIG_CONTROL_SUCCESS)) {
#ifdef DEBUG_USB_HOST
			sprintf(btduartbuff,"\r\nHID Control Successfully Configured");
			UARTPrintString(PRINTING_USART, btduartbuff);
#endif
			setProtocol(PS4BT); // Set protocol before establishing HID interrupt channel
			PS4BT->l2cap_state = L2CAP_INTERRUPT_SETUP;
		}
		break;

	case L2CAP_INTERRUPT_SETUP:

		if(l2cap_check_flag(L2CAP_FLAG_CONNECTION_INTERRUPT_REQUEST)) {
#ifdef DEBUG_USB_HOST
			sprintf(btduartbuff,"\r\nHID Interrupt Incoming Connection Request");
			UARTPrintString(PRINTING_USART, btduartbuff);
#endif
			l2cap_connection_response(PS4BT->PS4BTservice.hci_handle, PS4BT->PS4BTservice.identifier, PS4BT->interrupt_dcid, PS4BT->interrupt_scid, PENDING);
			USB_OTG_BSP_mDelay(1);
			l2cap_connection_response(PS4BT->PS4BTservice.hci_handle, PS4BT->PS4BTservice.identifier, PS4BT->interrupt_dcid, PS4BT->interrupt_scid, SUCCESSFUL);
			PS4BT->PS4BTservice.identifier++;
			USB_OTG_BSP_mDelay(1);
			l2cap_config_request(PS4BT->PS4BTservice.hci_handle, PS4BT->PS4BTservice.identifier,  PS4BT->interrupt_scid);

			PS4BT->l2cap_state = L2CAP_INTERRUPT_CONFIG_REQUEST;
		}
		break;

		/* These states are used if the Arduino is the host */
	case L2CAP_CONTROL_CONNECT_REQUEST:

		if(l2cap_check_flag(L2CAP_FLAG_CONTROL_CONNECTED)) {
#ifdef DEBUG_USB_HOST
			sprintf(btduartbuff,"\r\nSend HID Control Config Request");
			UARTPrintString(PRINTING_USART, btduartbuff);
#endif
			PS4BT->PS4BTservice.identifier++;
			l2cap_config_request(PS4BT->PS4BTservice.hci_handle, PS4BT->PS4BTservice.identifier, PS4BT->control_scid);
			PS4BT->l2cap_state = L2CAP_CONTROL_CONFIG_REQUEST;
		}
		break;

	case L2CAP_CONTROL_CONFIG_REQUEST:

		if(l2cap_check_flag(L2CAP_FLAG_CONFIG_CONTROL_SUCCESS)) {
			setProtocol(PS4BT); // Set protocol before establishing HID interrupt channel
			USB_OTG_BSP_mDelay(1); // Short delay between commands - just to be sure
#ifdef DEBUG_USB_HOST
			sprintf(btduartbuff,"\r\nSend HID Interrupt Connection Request");
			UARTPrintString(PRINTING_USART, btduartbuff);
#endif
			PS4BT->PS4BTservice.identifier++;
			l2cap_connection_request(PS4BT->PS4BTservice.hci_handle, PS4BT->PS4BTservice.identifier, PS4BT->interrupt_dcid, HID_INTR_PSM);
			PS4BT->l2cap_state = L2CAP_INTERRUPT_CONNECT_REQUEST;
		}
		break;

	case L2CAP_INTERRUPT_CONNECT_REQUEST:

		if(l2cap_check_flag(L2CAP_FLAG_INTERRUPT_CONNECTED)) {
#ifdef DEBUG_USB_HOST
			sprintf(btduartbuff,"\r\nSend HID Interrupt Config Request");
			UARTPrintString(PRINTING_USART, btduartbuff);
#endif
			PS4BT->PS4BTservice.identifier++;
			l2cap_config_request(PS4BT->PS4BTservice.hci_handle, PS4BT->PS4BTservice.identifier, PS4BT->interrupt_scid);
			PS4BT->l2cap_state = L2CAP_INTERRUPT_CONFIG_REQUEST;
		}
		break;

	case L2CAP_INTERRUPT_CONFIG_REQUEST:

		if(l2cap_check_flag(L2CAP_FLAG_CONFIG_INTERRUPT_SUCCESS)) { // Now the HID channels is established

#ifdef DEBUG_USB_HOST
			sprintf(btduartbuff,"\r\nHID Channels Established");
			UARTPrintString(PRINTING_USART, btduartbuff);
#endif
			BTD_Machine.connectToHIDDevice = false;
			BTD_Machine.pairWithHIDDevice = false;
			PS4BT->connected = true;
			OnInit(PS4BT);
			PS4BT->l2cap_state = L2CAP_DONE;
		}
		break;

	case L2CAP_DONE:
		break;

	case L2CAP_INTERRUPT_DISCONNECT:

		if(l2cap_check_flag(L2CAP_FLAG_DISCONNECT_INTERRUPT_RESPONSE)) {
#ifdef DEBUG_USB_HOST
			sprintf(btduartbuff,"\r\nDisconnected Interrupt Channel");
			UARTPrintString(PRINTING_USART, btduartbuff);
#endif
			PS4BT->PS4BTservice.identifier++;
			l2cap_disconnection_request(PS4BT->PS4BTservice.hci_handle, PS4BT->PS4BTservice.identifier, PS4BT->control_scid, PS4BT->control_dcid);
			PS4BT->l2cap_state = L2CAP_CONTROL_DISCONNECT;
		}
		break;

	case L2CAP_CONTROL_DISCONNECT:

		if(l2cap_check_flag(L2CAP_FLAG_DISCONNECT_CONTROL_RESPONSE)) {
#ifdef DEBUG_USB_HOST
			sprintf(btduartbuff,"\r\nDisconnected Control Channel");
			UARTPrintString(PRINTING_USART, btduartbuff);
#endif
			if(hci_disconnect(PS4BT->PS4BTservice.hci_handle) == true) {
				PS4BT->PS4BTservice.hci_handle = -1; // Reset handle
				PS4BT->PS4BTservice.l2cap_event_flag = 0; // Reset flags
				PS4BT->l2cap_state = L2CAP_WAIT;
			}
		}
		break;
	}
}

void PS4BTRun(uint32_t pointer_address) {

	PS4BT_t *PS4BT = (PS4BT_t*)pointer_address;

	switch(PS4BT->l2cap_state) {

	case L2CAP_WAIT:

		if(BTD_Machine.connectToHIDDevice && !BTD_Machine.l2capConnectionClaimed && !PS4BT->connected && !PS4BT->activeConnection) {

			BTD_Machine.l2capConnectionClaimed = true;
			PS4BT->activeConnection = true;
#ifdef DEBUG_USB_HOST
			sprintf(btduartbuff,"\r\nSend HID Control Connection Request");
			UARTPrintString(PRINTING_USART, btduartbuff);
#endif
			PS4BT->PS4BTservice.hci_handle = BTD_Machine.hci_handle; // Store the HCI Handle for the connection
			PS4BT->PS4BTservice.l2cap_event_flag = 0; // Reset flags
			PS4BT->PS4BTservice.identifier = 0;
			l2cap_connection_request(PS4BT->PS4BTservice.hci_handle, PS4BT->PS4BTservice.identifier, PS4BT->control_dcid, HID_CTRL_PSM);
			PS4BT->l2cap_state = L2CAP_CONTROL_CONNECT_REQUEST;

		} else if(l2cap_check_flag(L2CAP_FLAG_CONNECTION_CONTROL_REQUEST)) {
#ifdef DEBUG_USB_HOST
			sprintf(btduartbuff,"\r\nHID Control Incoming Connection Request");
			UARTPrintString(PRINTING_USART, btduartbuff);
#endif
			l2cap_connection_response(PS4BT->PS4BTservice.hci_handle, PS4BT->PS4BTservice.identifier, PS4BT->control_dcid, PS4BT->control_scid, PENDING);
			USB_OTG_BSP_mDelay(1);
			l2cap_connection_response(PS4BT->PS4BTservice.hci_handle, PS4BT->PS4BTservice.identifier, PS4BT->control_dcid, PS4BT->control_scid, SUCCESSFUL);
			PS4BT->PS4BTservice.identifier++;
			USB_OTG_BSP_mDelay(1);
			l2cap_config_request(PS4BT->PS4BTservice.hci_handle, PS4BT->PS4BTservice.identifier, PS4BT->control_scid);
			PS4BT->l2cap_state = L2CAP_CONTROL_SUCCESS;
		}
		break;
	}
}

/************************************************************/
/*                    HID Commands                          */

/************************************************************/
void setProtocol(PS4BT_t *PS4BT) {

#ifdef DEBUG_USB_HOST

	sprintf(btduartbuff,"\r\nSet protocol mode: USB_HID_BOOT_PROTOCOL");
	UARTPrintString(PRINTING_USART, btduartbuff);
#endif

	uint8_t command = 0x70 | USB_HID_BOOT_PROTOCOL; // Set Protocol, see Bluetooth HID specs page 33
	L2CAP_Command(PS4BT->PS4BTservice.hci_handle, &command, 1, PS4BT->control_scid[0], PS4BT->control_scid[1]);
}

void setLeds(PS4BT_t *PS4BT, uint8_t data) {
	uint8_t buf[3];
	buf[0] = 0xA2; // HID BT DATA_request (0xA0) | Report Type (Output 0x02)
	buf[1] = 0x01; // Report ID
	buf[2] = data;
	L2CAP_Command(PS4BT->PS4BTservice.hci_handle, buf, 3, PS4BT->interrupt_scid[0], PS4BT->interrupt_scid[1]);
}
/****************************************************************************************************************************************/

void ParseBTHIDData(PS4BT_t *PS4BT, uint8_t len, uint8_t *buf) {

	if (len > 1 && buf)  {

		if (buf[0] == 0x01) // Check report ID
			memcpy(&PS4BT->ps4Data, buf + 1, MIN((uint8_t)(len - 1), (uint8_t) sizeof(PS4BT->ps4Data)));
		else if (buf[0] == 0x11) { // This report is send via Bluetooth, it has an offset of 2 compared to the USB data
			if (len < 4) {
				return;
			}
			memcpy(&PS4BT->ps4Data, buf + 3, MIN((uint8_t)(len - 3), (uint8_t) sizeof(PS4BT->ps4Data)));
		} else {
			return;
		}

		if (PS4BT->ps4Data.btn.val != PS4BT->oldButtonState.val) { // Check if anything has changed
			PS4BT->buttonClickState.val = PS4BT->ps4Data.btn.val & ~PS4BT->oldButtonState.val; // Update click state variable
			PS4BT->oldButtonState.val = PS4BT->ps4Data.btn.val;

			// The DPAD buttons does not set the different bits, but set a value corresponding to the buttons pressed, we will simply set the bits ourself
			uint8_t newDpad = 0;
			if (checkDpad(PS4BT, UP))
				newDpad |= 1 << UP;
			if (checkDpad(PS4BT, RIGHT))
				newDpad |= 1 << RIGHT;
			if (checkDpad(PS4BT, DOWN))
				newDpad |= 1 << DOWN;
			if (checkDpad(PS4BT, LEFT))
				newDpad |= 1 << LEFT;
			if (newDpad != PS4BT->oldDpad) {
				PS4BT->buttonClickState.dpad = newDpad & ~PS4BT->oldDpad; // Override values
				PS4BT->oldDpad = newDpad;
			}
		}
	}

	if (PS4BT->ps4Output.reportChanged)
		sendOutputReport(PS4BT, &PS4BT->ps4Output); // Send output report
}

bool checkDpad(PS4BT_t *PS4BT, ButtonEnum b) {
	switch (b) {
	case UP:
		return PS4BT->ps4Data.btn.dpad == DPAD_LEFT_UP || PS4BT->ps4Data.btn.dpad == DPAD_UP || PS4BT->ps4Data.btn.dpad == DPAD_UP_RIGHT;
	case RIGHT:
		return PS4BT->ps4Data.btn.dpad == DPAD_UP_RIGHT || PS4BT->ps4Data.btn.dpad == DPAD_RIGHT || PS4BT->ps4Data.btn.dpad == DPAD_RIGHT_DOWN;
	case DOWN:
		return PS4BT->ps4Data.btn.dpad == DPAD_RIGHT_DOWN || PS4BT->ps4Data.btn.dpad == DPAD_DOWN || PS4BT->ps4Data.btn.dpad == DPAD_DOWN_LEFT;
	case LEFT:
		return PS4BT->ps4Data.btn.dpad == DPAD_DOWN_LEFT || PS4BT->ps4Data.btn.dpad == DPAD_LEFT || PS4BT->ps4Data.btn.dpad == DPAD_LEFT_UP;
	default:
		return false;
	}
}

void OnInit(PS4BT_t *PS4BT) {

	uint8_t i;
	for (i = 0; i < sizeof(PS4BT->ps4Data.hatValue); i++)
		PS4BT->ps4Data.hatValue[i] = 127; // Center value
	PS4BT->ps4Data.btn.val = 0;
	PS4BT->oldButtonState.val = 0;
	for (i = 0; i < sizeof(PS4BT->ps4Data.trigger); i++)
		PS4BT->ps4Data.trigger[i] = 0;
	for (i = 0; i < sizeof(PS4BT->ps4Data.xy)/sizeof(PS4BT->ps4Data.xy[0]); i++) {
		for (uint8_t j = 0; j < sizeof(PS4BT->ps4Data.xy[0].finger)/sizeof(PS4BT->ps4Data.xy[0].finger[0]); j++)
			PS4BT->ps4Data.xy[i].finger[j].touching = 1; // The bit is cleared if the finger is touching the touchpad
	}

	PS4BT->ps4Data.btn.dpad = DPAD_OFF;
	PS4BT->oldButtonState.dpad = DPAD_OFF;
	PS4BT->buttonClickState.dpad = 0;
	PS4BT->oldDpad = 0;

	PS4BT->ps4Output.bigRumble = PS4BT->ps4Output.smallRumble = 0;
	PS4BT->ps4Output.r = PS4BT->ps4Output.g = PS4BT->ps4Output.b = 0;
	PS4BT->ps4Output.flashOn = PS4BT->ps4Output.flashOff = 0;
	PS4BT->ps4Output.reportChanged = false;

	PS4_enable_sixaxis(PS4BT); // Make the controller send out the entire output report
	PS4BTsetLedColour(PS4BT, Blue);
};

void sendOutputReport(PS4BT_t *PS4BT, PS4Output *output) { // Source: https://github.com/chrippa/ds4drv

	uint8_t buf[79];
	memset(buf, 0, sizeof(buf));

	buf[0] = 0x52; // HID BT Set_report (0x50) | Report Type (Output 0x02)
	buf[1] = 0x11; // Report ID
	buf[2] = 0x80;
	buf[4]= 0xFF;

	buf[7] = output->smallRumble; // Small Rumble
	buf[8] = output->bigRumble; // Big rumble

	buf[9] = output->r; // Red
	buf[10] = output->g; // Green
	buf[11] = output->b; // Blue

	buf[12] = output->flashOn; // Time to flash bright (255 = 2.5 seconds)
	buf[13] = output->flashOff; // Time to flash dark (255 = 2.5 seconds)

	output->reportChanged = false;

	// The PS4 console actually set the four last bytes to a CRC32 checksum, but it seems like it is actually not needed

	L2CAP_Command(PS4BT->PS4BTservice.hci_handle, buf, sizeof(buf), PS4BT->control_scid[0], PS4BT->control_scid[1]);
};

void PS4_enable_sixaxis(PS4BT_t *PS4BT) { // Command used to make the PS4 controller send out the entire output report
	uint8_t buf[2];
	buf[0] = 0x43; // HID BT Get_report (0x40) | Report Type (Feature 0x03)
	buf[1] = 0x02; // Report ID

	L2CAP_Command(PS4BT->PS4BTservice.hci_handle, buf, 2, PS4BT->control_scid[0], PS4BT->control_scid[1]);
};

/****************************************************************************************************************************************/

bool PS4BTgetButtonPress(PS4BT_t *PS4BT, ButtonEnum b) {

	if (b <= LEFT) // Dpad
		return checkDpad(PS4BT, b);
	else
		return PS4BT->ps4Data.btn.val & (1UL <<PS4_BUTTONS[(uint8_t)b]);

}

bool PS4BTgetButtonClick(PS4BT_t *PS4BT, ButtonEnum b) {
	uint32_t mask = 1UL << PS4_BUTTONS[(uint8_t)b];
	bool click = PS4BT->buttonClickState.val & mask;
	PS4BT->buttonClickState.val &= ~mask; // Clear "click" event
	return click;
}

uint8_t PS4BTgetAnalogButton(PS4BT_t *PS4BT, ButtonEnum b) {
	if (b == L2) // These are the only analog buttons on the controller
		return PS4BT->ps4Data.trigger[0];
	else if (b == R2)
		return PS4BT->ps4Data.trigger[1];
	return 0;
}

uint8_t PS4BTgetAnalogHat(PS4BT_t *PS4BT, AnalogHatEnum a) {
	return PS4BT->ps4Data.hatValue[(uint8_t)a];
}

uint16_t PS4BTgetX(PS4BT_t *PS4BT, uint8_t finger, uint8_t xyId) {
	return PS4BT->ps4Data.xy[xyId].finger[finger].x;
};

uint16_t PS4BTgetY(PS4BT_t *PS4BT, uint8_t finger, uint8_t xyId) {
	return PS4BT->ps4Data.xy[xyId].finger[finger].y;
};

bool PS4BTisTouching(PS4BT_t *PS4BT, uint8_t finger, uint8_t xyId) {
	return !(PS4BT->ps4Data.xy[xyId].finger[finger].touching); // The bit is cleared when a finger is touching the touchpad
};

uint8_t PS4BTgetTouchCounter(PS4BT_t *PS4BT, uint8_t finger, uint8_t xyId) {
	return PS4BT->ps4Data.xy[xyId].finger[finger].counter;
};

float PS4BTgetAngle(PS4BT_t *PS4BT, AngleEnum a) {
	if (a == Pitch)
		return (atan2f(PS4BT->ps4Data.accY, PS4BT->ps4Data.accZ) + PI) * RAD_TO_DEG;
	else
		return (atan2f(PS4BT->ps4Data.accX, PS4BT->ps4Data.accZ) + PI) * RAD_TO_DEG;
};

int16_t PS4BTgetSensor(PS4BT_t *PS4BT, SensorEnum s) {
	switch(s) {
	case gX:
		return PS4BT->ps4Data.gyroX;
	case gY:
		return PS4BT->ps4Data.gyroY;
	case gZ:
		return PS4BT->ps4Data.gyroZ;
	case aX:
		return PS4BT->ps4Data.accX;
	case aY:
		return PS4BT->ps4Data.accY;
	case aZ:
		return PS4BT->ps4Data.accZ;
	default:
		return 0;
	}
};

uint8_t PS4BTgetBatteryLevel(PS4BT_t *PS4BT) {
	return PS4BT->ps4Data.status.battery;
};


bool PS4BTgetUsbStatus(PS4BT_t *PS4BT) {
	return PS4BT->ps4Data.status.usb;
};

bool PS4BTgetAudioStatus(PS4BT_t *PS4BT) {
	return PS4BT->ps4Data.status.audio;
};

bool PS4BTgetMicStatus(PS4BT_t *PS4BT) {
	return PS4BT->ps4Data.status.mic;
};

/** Turn both rumble and the LEDs off. */
void PS4BTsetAllOff(PS4BT_t *PS4BT) {
	PS4BTsetRumbleOff(PS4BT);
	PS4BTsetLedOff(PS4BT);
};

/** Set rumble off. */
void PS4BTsetRumbleOff(PS4BT_t *PS4BT) {
	PS4BTsetRumbleOn(PS4BT, 0, 0);
};

void PS4BTsetRumbleMode(PS4BT_t *PS4BT, RumbleEnum mode) {
	if (mode == RumbleLow)
		PS4BTsetRumbleOn(PS4BT, 0x00, 0xFF);
	else
		PS4BTsetRumbleOn(PS4BT, 0xFF, 0x00);
};

void PS4BTsetRumbleOn(PS4BT_t *PS4BT, uint8_t bigRumble, uint8_t smallRumble) {
	PS4BT->ps4Output.bigRumble = bigRumble;
	PS4BT->ps4Output.smallRumble = smallRumble;
	PS4BT->ps4Output.reportChanged = true;
};

void PS4BTsetLedOff(PS4BT_t *PS4BT) {
	PS4BTsetLed(PS4BT, 0, 0, 0);
};

void PS4BTsetLed(PS4BT_t *PS4BT, uint8_t r, uint8_t g, uint8_t b) {
	PS4BT->ps4Output.r = r;
	PS4BT->ps4Output.g = g;
	PS4BT-> ps4Output.b = b;
	PS4BT->ps4Output.reportChanged = true;
};

void PS4BTsetLedColour(PS4BT_t *PS4BT, ColorsEnum color) {
	PS4BTsetLed(PS4BT, (uint8_t)(color >> 16), (uint8_t)(color >> 8), (uint8_t)(color));
};


void PS4BTsetLedFlash(PS4BT_t *PS4BT, uint8_t flashOn, uint8_t flashOff) {
	PS4BT->ps4Output.flashOn = flashOn;
	PS4BT->ps4Output.flashOff = flashOff;
	PS4BT->ps4Output.reportChanged = true;
};


