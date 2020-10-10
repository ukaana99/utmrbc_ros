#include "STM32_USB_HOST_Library/Class/BTD/inc/PS3BT.h"

/* Macros for L2CAP event flag tests */
#define l2cap_check_flag(flag) (PS3BT->PS3BTservice.l2cap_event_flag & (flag))
#define l2cap_set_flag(flag) (PS3BT->PS3BTservice.l2cap_event_flag |= (flag))
#define l2cap_clear_flag(flag) (PS3BT->PS3BTservice.l2cap_event_flag &= ~(flag))

void PS3BTInit(PS3BT_t *PS3BT, uint8_t btadr5, uint8_t btadr4, uint8_t btadr3, uint8_t btadr2, uint8_t btadr1, uint8_t btadr0)
{
	BTD_Machine.my_bdaddr[5] = btadr5; // Change to your dongle's Bluetooth address instead
	BTD_Machine.my_bdaddr[4] = btadr4;
	BTD_Machine.my_bdaddr[3] = btadr3;
	BTD_Machine.my_bdaddr[2] = btadr2;
	BTD_Machine.my_bdaddr[1] = btadr1;
	BTD_Machine.my_bdaddr[0] = btadr0;

	PS3BT->HIDBuffer[0] = 0x52; // HID BT Set_report (0x50) | Report Type (Output 0x02)
	PS3BT->HIDBuffer[1] = 0x01; // Report ID

	// Needed for PS3 Move Controller commands to work via bluetooth
	PS3BT->HIDMoveBuffer[0] = 0xA2; // HID BT DATA_request (0xA0) | Report Type (Output 0x02)
	PS3BT->HIDMoveBuffer[1] = 0x02; // Report ID

	/* Set device cid for the control and intterrupt channelse - LSB */
	PS3BT->control_dcid[0] = 0x40; // 0x0040
	PS3BT->control_dcid[1] = 0x00;
	PS3BT->interrupt_dcid[0] = 0x41; // 0x0041
	PS3BT->interrupt_dcid[1] = 0x00;

	PS3BT->PS3BTservice.ACLData = ACLData;
	PS3BT->PS3BTservice.Reset = Reset;
	PS3BT->PS3BTservice.Run = Run;
	PS3BT->PS3BTservice.disconnect = Disconnect;
	PS3BT->PS3BTservice.onInit = onInit;

	PS3BT->PS3BTserviceID = registerBluetoothService((uint32_t)PS3BT, &PS3BT->PS3BTservice);

	PS3BT->PS3BTservice.Reset(BTD_Machine.btservive_pointer_address[PS3BT->PS3BTserviceID]);

	sprintf(btduartbuff,"\r\nPS3 Bluetooth Library Started");
	UARTPrintString(PRINTING_USART, btduartbuff);
}

/****************************************************************************************************************************************/

bool PS3BTgetButtonPress(PS3BT_t *PS3BT,ButtonEnum b) {
	return (PS3BT->ButtonState & PS3_BUTTONS[(uint8_t)b]);
}

bool PS3BTgetButtonClick(PS3BT_t *PS3BT,ButtonEnum b) {
	uint32_t button = PS3_BUTTONS[(uint8_t)b];
	bool click = (PS3BT->ButtonClickState & button);
	PS3BT->ButtonClickState &= ~button; // Clear "click" event
	return click;
}

uint8_t PS3BTgetAnalogButton(PS3BT_t *PS3BT,ButtonEnum a) {
	return (uint8_t)(PS3BT->l2capinbuf[PS3_ANALOG_BUTTONS[(uint8_t)a]]);
}

uint8_t PS3BTgetAnalogHat(PS3BT_t *PS3BT,AnalogHatEnum a) {
	return (uint8_t)(PS3BT->l2capinbuf[(uint8_t)a + 15]);
}

int16_t PS3BTgetSensor(PS3BT_t *PS3BT, SensorEnum a) {
	if(PS3BT->PS3Connected) {
		if(a == aX || a == aY || a == aZ || a == gZ)
			return ((PS3BT->l2capinbuf[(uint16_t)a] << 8) | PS3BT->l2capinbuf[(uint16_t)a + 1]);
		else
			return 0;
	} else if(PS3BT->PS3MoveConnected) {
		if(a == mXmove || a == mYmove) // These are all 12-bits long
			return (((PS3BT->l2capinbuf[(uint16_t)a] & 0x0F) << 8) | (PS3BT->l2capinbuf[(uint16_t)a + 1]));
		else if(a == mZmove || a == tempMove) // The tempearature is also 12 bits long
			return ((PS3BT->l2capinbuf[(uint16_t)a] << 4) | ((PS3BT->l2capinbuf[(uint16_t)a + 1] & 0xF0) >> 4));
		else // aXmove, aYmove, aZmove, gXmove, gYmove and gZmove
			return (PS3BT->l2capinbuf[(uint16_t)a] | (PS3BT->l2capinbuf[(uint16_t)a + 1] << 8));
	} else
		return 0;
}

float PS3BTgetAngle(PS3BT_t *PS3BT,AngleEnum a) {

	float accXval, accYval, accZval;

	if(PS3BT->PS3Connected) {
		// Data for the Kionix KXPC4 used in the DualShock 3
		const float zeroG = 511.5f; // 1.65/3.3*1023 (1.65V)
		accXval = -((float)PS3BTgetSensor(PS3BT, aX) - zeroG);
		accYval = -((float)PS3BTgetSensor(PS3BT, aY) - zeroG);
		accZval = -((float)PS3BTgetSensor(PS3BT, aZ) - zeroG);
	} else if(PS3BT->PS3MoveConnected) {
		// It's a Kionix KXSC4 inside the Motion controller
		const uint16_t zeroG = 0x8000;
		accXval = -(int16_t)(PS3BTgetSensor(PS3BT, aXmove) - zeroG);
		accYval = (int16_t)(PS3BTgetSensor(PS3BT,aYmove) - zeroG);
		accZval = (int16_t)(PS3BTgetSensor(PS3BT,aZmove) - zeroG);
	} else
		return 0;

	// Convert to 360 degrees resolution
	// atan2 outputs the value of -π to π (radians)
	// We are then converting it to 0 to 2π and then to degrees
	if(a == Pitch)
		return (atan2f(accYval, accZval) + PI) * RAD_TO_DEG;
	else
		return (atan2f(accXval, accZval) + PI) * RAD_TO_DEG;

}

float PS3BTget9DOFValues(PS3BT_t *PS3BT,SensorEnum a) { // Thanks to Manfred Piendl

	if(!PS3BT->PS3MoveConnected)
		return 0;
	int16_t value = PS3BTgetSensor(PS3BT, a);
	if(a == mXmove || a == mYmove || a == mZmove) {
		if(value > 2047)
			value -= 0x1000;
		return (float)value / 3.2f; // unit: muT = 10^(-6) Tesla
	} else if(a == aXmove || a == aYmove || a == aZmove) {
		if(value < 0)
			value += 0x8000;
		else
			value -= 0x8000;
		return (float)value / 442.0f; // unit: m/(s^2)
	} else if(a == gXmove || a == gYmove || a == gZmove) {
		if(value < 0)
			value += 0x8000;
		else
			value -= 0x8000;
		if(a == gXmove)
			return (float)value / 11.6f; // unit: deg/s
		else if(a == gYmove)
			return (float)value / 11.2f; // unit: deg/s
		else // gZmove
			return (float)value / 9.6f; // unit: deg/s
	} else
		return 0;

}

bool PS3BTgetStatus(PS3BT_t *PS3BT,StatusEnum c) {
	return (PS3BT->l2capinbuf[(uint16_t)c >> 8] == ((uint8_t)c & 0xff));
}

uint32_t PS3BTgetLastMessageTime(PS3BT_t *PS3BT) {
	return PS3BT->lastMessageTime;
};

/****************************************************************************************************************************************/

/** @name BluetoothService implementation */

void Reset(uint32_t pointer_address) {

	PS3BT_t *PS3BT = (PS3BT_t*)pointer_address;

	PS3BT->PS3Connected = false;
	PS3BT->PS3MoveConnected = false;
	PS3BT->PS3NavigationConnected = false;
	PS3BT->activeConnection = false;
	PS3BT->PS3BTservice.l2cap_event_flag = 0; // Reset flags
	PS3BT->ButtonState = 0;
	PS3BT->leftjoy_x = 128;
	PS3BT->leftjoy_y = 128;
	PS3BT->rightjoy_x = 128;
	PS3BT->rightjoy_y = 128;
	PS3BT->an_L2 = 0;
	PS3BT->an_R2 = 0;
	PS3BT->joyL_2 = 0.0;
	PS3BT->joyL_x = 0.0;
	PS3BT->joyL_y = 0.0;
	PS3BT->joyR_2 = 0.0;
	PS3BT->joyR_x = 0.0;
	PS3BT->joyR_y = 0.0;
	PS3BT->l2cap_state = L2CAP_WAIT;

	// Needed for PS3 Dualshock Controller commands to work via Bluetooth
	for(uint8_t i = 0; i < PS3_REPORT_BUFFER_SIZE; i++)
		PS3BT-> HIDBuffer[i + 2] = PS3_REPORT_BUFFER[i]; // First two bytes reserved for report type and ID
}

void Disconnect(uint32_t pointer_address) { // Use this void to disconnect any of the controllers
	// First the HID interrupt channel has to be disconnected, then the HID control channel and finally the HCI connection

	PS3BT_t *PS3BT = (PS3BT_t*)pointer_address;
	l2cap_disconnection_request(PS3BT->PS3BTservice.hci_handle, ++PS3BT->PS3BTservice.identifier, PS3BT->interrupt_scid, PS3BT->interrupt_dcid);
	Reset(pointer_address);
	PS3BT->l2cap_state = L2CAP_INTERRUPT_DISCONNECT;
}

void ACLData(uint8_t* ACLData, uint32_t pointer_address) {

	PS3BT_t *PS3BT = (PS3BT_t*)pointer_address;

	if(!BTD_Machine.l2capConnectionClaimed && !PS3BT->PS3Connected && !PS3BT->PS3MoveConnected && !PS3BT->PS3NavigationConnected && !PS3BT->activeConnection) {
		if(ACLData[8] == L2CAP_CMD_CONNECTION_REQUEST) {
			if((ACLData[12] | (ACLData[13] << 8)) == HID_CTRL_PSM) {
				BTD_Machine.l2capConnectionClaimed = true; // Claim that the incoming connection belongs to this service
				PS3BT->activeConnection = true;
				PS3BT->PS3BTservice.hci_handle = BTD_Machine.hci_handle; // Store the HCI Handle for the connection!!!!!!!!
				PS3BT->l2cap_state = L2CAP_WAIT;
				PS3BT->remote_name_first = BTD_Machine.remote_name[0]; // Store the first letter in remote name for the connection

#ifdef DEBUG_USB_HOST
				if(BTD_Machine.hci_version < 3) { // Check the HCI Version of the Bluetooth dongle
					sprintf(btduartbuff,"\r\nYour dongle may not support reading the analog buttons, sensors and status\r\nYour HCI Version is:  %X", BTD_Machine.hci_version);
					UARTPrintString(PRINTING_USART, btduartbuff);
					sprintf(btduartbuff,"\r\nBut should be at least 3\r\nThis means that it doesn't support Bluetooth Version 2.0+EDR");
					UARTPrintString(PRINTING_USART, btduartbuff);
				}
#endif
			}
		}
	}

	if(checkHciHandle(ACLData,  PS3BT->PS3BTservice.hci_handle)) { // acl_handle_ok
		memcpy(PS3BT->l2capinbuf, ACLData, BULK_MAXPKTSIZE);                 						//i2capinbuff crash !!!!!!!
		if((PS3BT->l2capinbuf[6] | (PS3BT->l2capinbuf[7] << 8)) == 0x0001U) { // l2cap_control - Channel ID for ACL-U
			if(PS3BT->l2capinbuf[8] == L2CAP_CMD_COMMAND_REJECT) {
#ifdef DEBUG_USB_HOST
				sprintf(btduartbuff,"\r\nL2CAP Command Rejected - Reason: %X: %X: %X: %X: %X: %X ",PS3BT->l2capinbuf[13],PS3BT->l2capinbuf[12],PS3BT->l2capinbuf[17],PS3BT->l2capinbuf[16],PS3BT->l2capinbuf[15],PS3BT->l2capinbuf[14]);
				UARTPrintString(PRINTING_USART, btduartbuff);
#endif
			} else if(PS3BT->l2capinbuf[8] == L2CAP_CMD_CONNECTION_REQUEST) {
				if((PS3BT->l2capinbuf[12] | (PS3BT->l2capinbuf[13] << 8)) == HID_CTRL_PSM) {
					PS3BT->PS3BTservice.identifier = PS3BT->l2capinbuf[9];
					PS3BT->control_scid[0] = PS3BT->l2capinbuf[14];
					PS3BT->control_scid[1] = PS3BT->l2capinbuf[15];
					l2cap_set_flag(L2CAP_FLAG_CONNECTION_CONTROL_REQUEST);
				} else if((PS3BT->l2capinbuf[12] | (PS3BT->l2capinbuf[13] << 8)) == HID_INTR_PSM) {
					PS3BT->PS3BTservice.identifier = PS3BT->l2capinbuf[9];
					PS3BT->interrupt_scid[0] = PS3BT->l2capinbuf[14];
					PS3BT->interrupt_scid[1] = PS3BT->l2capinbuf[15];
					l2cap_set_flag(L2CAP_FLAG_CONNECTION_INTERRUPT_REQUEST);
				}
			} else if(PS3BT->l2capinbuf[8] == L2CAP_CMD_CONFIG_RESPONSE) {
				if((PS3BT->l2capinbuf[16] | (PS3BT->l2capinbuf[17] << 8)) == 0x0000) { // Success
					if(PS3BT->l2capinbuf[12] == PS3BT->control_dcid[0] && PS3BT->l2capinbuf[13] == PS3BT->control_dcid[1]) {
						l2cap_set_flag(L2CAP_FLAG_CONFIG_CONTROL_SUCCESS);
					} else if(PS3BT->l2capinbuf[12] ==PS3BT->interrupt_dcid[0] && PS3BT->l2capinbuf[13] == PS3BT->interrupt_dcid[1]) {
						l2cap_set_flag(L2CAP_FLAG_CONFIG_INTERRUPT_SUCCESS);
					}
				}
			} else if(PS3BT->l2capinbuf[8] == L2CAP_CMD_CONFIG_REQUEST) {
				if(PS3BT->l2capinbuf[12] == PS3BT->control_dcid[0] && PS3BT->l2capinbuf[13] == PS3BT->control_dcid[1]) {
					l2cap_config_response(PS3BT->PS3BTservice.hci_handle, PS3BT->l2capinbuf[9], PS3BT->control_scid);
				} else if(PS3BT->l2capinbuf[12] == PS3BT->interrupt_dcid[0] && PS3BT->l2capinbuf[13] == PS3BT->interrupt_dcid[1]) {
					l2cap_config_response(PS3BT->PS3BTservice.hci_handle, PS3BT->l2capinbuf[9], PS3BT->interrupt_scid);
				}
			} else if(PS3BT->l2capinbuf[8] == L2CAP_CMD_DISCONNECT_REQUEST) {
				if(PS3BT->l2capinbuf[12] == PS3BT->control_dcid[0] && PS3BT->l2capinbuf[13] == PS3BT->control_dcid[1]) {
#ifdef DEBUG_USB_HOST
					sprintf(btduartbuff,"\r\nDisconnect Request: Control Channel");
					UARTPrintString(PRINTING_USART, btduartbuff);
#endif
					PS3BT->PS3BTservice.identifier = PS3BT->l2capinbuf[9];
					l2cap_disconnection_response(PS3BT->PS3BTservice.hci_handle, PS3BT->PS3BTservice.identifier, PS3BT->control_dcid, PS3BT->control_scid);
					Reset(pointer_address);
				} else if(PS3BT->l2capinbuf[12] == PS3BT->interrupt_dcid[0] && PS3BT->l2capinbuf[13] == PS3BT->interrupt_dcid[1]) {
#ifdef DEBUG_USB_HOST
					sprintf(btduartbuff,"\r\nDisconnect Request: Interrupt Channel");
					UARTPrintString(PRINTING_USART, btduartbuff);
#endif
					PS3BT->PS3BTservice.identifier =  PS3BT->l2capinbuf[9];
					l2cap_disconnection_response(PS3BT->PS3BTservice.hci_handle, PS3BT->PS3BTservice.identifier, PS3BT->interrupt_dcid, PS3BT->interrupt_scid);
					Reset(pointer_address);
				}
			} else if(PS3BT->l2capinbuf[8] == L2CAP_CMD_DISCONNECT_RESPONSE) {
				if(PS3BT->l2capinbuf[12] == PS3BT->control_scid[0] && PS3BT->l2capinbuf[13] == PS3BT->control_scid[1]) {
					PS3BT->PS3BTservice.identifier = PS3BT->l2capinbuf[9];
					l2cap_set_flag(L2CAP_FLAG_DISCONNECT_CONTROL_RESPONSE);
				} else if(PS3BT->l2capinbuf[12] == PS3BT->interrupt_scid[0] && PS3BT->l2capinbuf[13] == PS3BT->interrupt_scid[1]) {
					PS3BT->PS3BTservice.identifier = PS3BT->l2capinbuf[9];
					l2cap_set_flag(L2CAP_FLAG_DISCONNECT_INTERRUPT_RESPONSE);
				}
			}
		} else if(PS3BT->l2capinbuf[6] == PS3BT->interrupt_dcid[0] && PS3BT->l2capinbuf[7] == PS3BT->interrupt_dcid[1]) { // l2cap_interrupt

			if(PS3BT->PS3Connected || PS3BT->PS3MoveConnected || PS3BT->PS3NavigationConnected) {
				/* Read Report */
				if(PS3BT->l2capinbuf[8] == 0xA1) { // HID_THDR_DATA_INPUT
					PS3BT->lastMessageTime = (uint32_t)HCD_GetCurrentFrame(BTD_Machine.pdev); // Store the last message time

					if(PS3BT->PS3Connected || PS3BT->PS3NavigationConnected)
						PS3BT-> ButtonState = (uint32_t)(PS3BT->l2capinbuf[11] | ((uint16_t)PS3BT->l2capinbuf[12] << 8) | ((uint32_t)PS3BT->l2capinbuf[13] << 16));
					else if(PS3BT->PS3MoveConnected)
						PS3BT->ButtonState = (uint32_t)(PS3BT->l2capinbuf[10] | ((uint16_t)PS3BT->l2capinbuf[11] << 8) | ((uint32_t)PS3BT->l2capinbuf[12] << 16));

					if(PS3BT->ButtonState != PS3BT->OldButtonState) {
						PS3BT->ButtonClickState = PS3BT->ButtonState & ~PS3BT->OldButtonState; // Update click state variable
						PS3BT->OldButtonState = PS3BT->ButtonState;
					}

					if(joyR_up){
						PS3BT->joyR_y  = ((ps3_low_Ry - ((float)PS3BT->rightjoy_y))/100.0)/1.00;
						if(PS3BT->joyR_y < 0.0){
							PS3BT->joyR_y = 0.0;
						}else if(PS3BT->joyR_y > 1.0){
							PS3BT->joyR_y = 1.0;
						}
					}else if(joyR_down){
						PS3BT->joyR_y = ((ps3_high_Ry - ((float)PS3BT->rightjoy_y))/100.0)/1.00;
						if(PS3BT->joyR_y > 0.0){
							PS3BT->joyR_y = 0.0;
						}else if(PS3BT->joyR_y < -1.0){
							PS3BT->joyR_y = -1.0;
						}
					}else{
						PS3BT->joyR_y = 0.0;
					}

					if(joyR_left){
						PS3BT->joyR_x = ((ps3_low_Rx - ((float)PS3BT->rightjoy_x))/100.0)/1.00;
						if(PS3BT->joyR_x < 0.0){
							PS3BT->joyR_x = 0.0;
						}else if(PS3BT->joyR_x > 1.0){
							PS3BT->joyR_x = 1.0;
						}
					}else if(joyR_right){
						PS3BT->joyR_x = ((ps3_high_Rx - ((float)PS3BT->rightjoy_x))/100.0)/1.00;
						if(PS3BT->joyR_x > 0.0){
							PS3BT->joyR_x = 0.0;
						}else if(PS3BT->joyR_x < -1.0){
							PS3BT->joyR_x = -1.0;
						}
					}else{
						PS3BT->joyR_x = 0.0;
					}

					if(joyL_up){
						PS3BT->joyL_y = ((ps3_low_Ly - ((float)PS3BT->leftjoy_y))/100.0)/1.00;
						if(PS3BT->joyL_y < 0.0){
							PS3BT->joyL_y = 0.0;
						}else if(PS3BT->joyL_y > 1.0){
							PS3BT->joyL_y = 1.0;
						}
					}else if(joyL_down){
						PS3BT->joyL_y = ((ps3_high_Ly - ((float)PS3BT->leftjoy_y))/100.0)/1.00;
						if(PS3BT->joyL_y > 0.0){
							PS3BT->joyL_y = 0.0;
						}else if(PS3BT->joyL_y < -1.0){
							PS3BT->joyL_y = -1.0;
						}
					}else{
						PS3BT->joyL_y = 0.0;
					}

					if(joyL_left){
						PS3BT->joyL_x = ((ps3_low_Lx - ((float)PS3BT->leftjoy_x))/100.0)/1.00;
						if(PS3BT->joyL_x < 0.0){
							PS3BT->joyL_x = 0.0;
						}else if(PS3BT->joyL_x > 1.0){
							PS3BT->joyL_x = 1.0;
						}
					}else if(joyL_right){
						PS3BT->joyL_x = ((ps3_high_Lx - ((float)PS3BT->leftjoy_x))/100.0)/1.00;
						if(PS3BT->joyL_x > 0.0){
							PS3BT->joyL_x = 0.0;
						}else if(PS3BT->joyL_x < -1.0){
							PS3BT->joyL_x = -1.0;
						}
					}else{
						PS3BT->joyL_x = 0.0;
					}

					PS3BT->joyR_2 = (PS3BT->an_R2/255.0);
					PS3BT->joyL_2 = (PS3BT->an_L2/255.0);
				}
			}
		}

		L2CAP_task(PS3BT);
	}
}

void L2CAP_task(PS3BT_t *PS3BT) {

	switch(PS3BT->l2cap_state) {

	case L2CAP_WAIT:
		if(l2cap_check_flag(L2CAP_FLAG_CONNECTION_CONTROL_REQUEST)) {
#ifdef DEBUG_USB_HOST
			sprintf(btduartbuff,"\r\nHID Control Incoming Connection Request");
			UARTPrintString(PRINTING_USART, btduartbuff);
#endif
			l2cap_connection_response(PS3BT->PS3BTservice.hci_handle, PS3BT->PS3BTservice.identifier, PS3BT->control_dcid, PS3BT->control_scid, PENDING);
			USB_OTG_BSP_mDelay(1);
			l2cap_connection_response(PS3BT->PS3BTservice.hci_handle, PS3BT->PS3BTservice.identifier, PS3BT->control_dcid, PS3BT->control_scid, SUCCESSFUL);
			PS3BT->PS3BTservice.identifier++;
			USB_OTG_BSP_mDelay(1);
			l2cap_config_request(PS3BT->PS3BTservice.hci_handle, PS3BT->PS3BTservice.identifier, PS3BT->control_scid);
			PS3BT->l2cap_state = L2CAP_CONTROL_SUCCESS;
		}
		break;

	case L2CAP_CONTROL_SUCCESS:

		if(l2cap_check_flag(L2CAP_FLAG_CONFIG_CONTROL_SUCCESS)) {
#ifdef DEBUG_USB_HOST
			sprintf(btduartbuff,"\r\nHID Control Successfully Configured");
			UARTPrintString(PRINTING_USART, btduartbuff);
#endif
			PS3BT->l2cap_state = L2CAP_INTERRUPT_SETUP;
		}
		break;

	case L2CAP_INTERRUPT_SETUP:

		if(l2cap_check_flag(L2CAP_FLAG_CONNECTION_INTERRUPT_REQUEST)) {
#ifdef DEBUG_USB_HOST
			sprintf(btduartbuff,"\r\nHID Interrupt Incoming Connection Request");
			UARTPrintString(PRINTING_USART, btduartbuff);
#endif
			l2cap_connection_response(PS3BT->PS3BTservice.hci_handle, PS3BT->PS3BTservice.identifier, PS3BT->interrupt_dcid, PS3BT->interrupt_scid, PENDING);
			USB_OTG_BSP_mDelay(1);
			l2cap_connection_response(PS3BT->PS3BTservice.hci_handle, PS3BT->PS3BTservice.identifier, PS3BT->interrupt_dcid, PS3BT->interrupt_scid, SUCCESSFUL);
			PS3BT->PS3BTservice.identifier++;
			USB_OTG_BSP_mDelay(1);
			l2cap_config_request(PS3BT->PS3BTservice.hci_handle, PS3BT->PS3BTservice.identifier, PS3BT->interrupt_scid);

			PS3BT->l2cap_state = L2CAP_INTERRUPT_CONFIG_REQUEST;
		}
		break;

	case L2CAP_INTERRUPT_CONFIG_REQUEST:

		if(l2cap_check_flag(L2CAP_FLAG_CONFIG_INTERRUPT_SUCCESS)) { // Now the HID channels is established
#ifdef DEBUG_USB_HOST
			sprintf(btduartbuff,"\r\nHID Interrupt Successfully Configured");
			UARTPrintString(PRINTING_USART, btduartbuff);
#endif
			if(PS3BT->remote_name_first == 'M') { // First letter in Motion Controller ('M')
				memset(PS3BT->l2capinbuf, 0, BULK_MAXPKTSIZE); // Reset l2cap in buffer as it sometimes read it as a button has been pressed
				PS3BT->l2cap_state = TURN_ON_LED;
			} else
				PS3BT->l2cap_state = PS3_ENABLE_SIXAXIS;
			PS3BT->timer = (uint32_t)HCD_GetCurrentFrame(BTD_Machine.pdev);
		}
		break;

		/* These states are handled in Run() */

	case L2CAP_INTERRUPT_DISCONNECT:

		if(l2cap_check_flag(L2CAP_FLAG_DISCONNECT_INTERRUPT_RESPONSE)) {
#ifdef DEBUG_USB_HOST
			sprintf(btduartbuff,"\r\nDisconnected Interrupt Channel");
			UARTPrintString(PRINTING_USART, btduartbuff);
#endif
			PS3BT->PS3BTservice.identifier++;
			l2cap_disconnection_request(PS3BT->PS3BTservice.hci_handle,PS3BT->PS3BTservice. identifier, PS3BT->control_scid, PS3BT->control_dcid);
			PS3BT->l2cap_state = L2CAP_CONTROL_DISCONNECT;
		}
		break;

	case L2CAP_CONTROL_DISCONNECT:

		if(l2cap_check_flag(L2CAP_FLAG_DISCONNECT_CONTROL_RESPONSE)) {
#ifdef DEBUG_USB_HOST
			sprintf(btduartbuff,"\r\nDisconnected Control Channel");
			UARTPrintString(PRINTING_USART, btduartbuff);
#endif
			hci_disconnect(PS3BT->PS3BTservice.hci_handle);
			PS3BT->PS3BTservice.hci_handle = -1; // Reset handle
			PS3BT->PS3BTservice.l2cap_event_flag = 0; // Reset flags
			PS3BT->l2cap_state = L2CAP_WAIT;
		}
		break;
	}
}

void Run(uint32_t pointer_address) {

	PS3BT_t *PS3BT = (PS3BT_t*)pointer_address;

	switch(PS3BT->l2cap_state) {

	case PS3_ENABLE_SIXAXIS:

		if((int32_t)((uint32_t)HCD_GetCurrentFrame(BTD_Machine.pdev) - PS3BT->timer) > 1000) { // loop 1 second before sending the command
			memset(PS3BT->l2capinbuf, 0, BULK_MAXPKTSIZE); // Reset l2cap in buffer as it sometimes read it as a button has been pressed
			for(uint8_t i = 15; i < 19; i++)
				PS3BT->l2capinbuf[i] = 0x7F; // Set the analog joystick values to center position
			enable_sixaxis(PS3BT);
			PS3BT->l2cap_state = TURN_ON_LED;
			PS3BT->timer = (uint32_t)HCD_GetCurrentFrame(BTD_Machine.pdev);
		}
		break;

	case TURN_ON_LED:

		if((int32_t)((uint32_t)HCD_GetCurrentFrame(BTD_Machine.pdev) - PS3BT->timer) > 1000) { // loop 1 second before sending the command

			if(PS3BT->remote_name_first == 'P') { // First letter in PLAYSTATION(R)3 Controller ('P')
#ifdef DEBUG_USB_HOST
				sprintf(btduartbuff,"\r\nDualshock 3 Controller Enabled\r\n");
				UARTPrintString(PRINTING_USART, btduartbuff);
#endif
				PS3BT->PS3Connected = true;

			} else if(PS3BT->remote_name_first == 'N') { // First letter in Navigation Controller ('N')
#ifdef DEBUG_USB_HOST
				sprintf(btduartbuff,"\r\nNavigation Controller Enabled\r\n");
				UARTPrintString(PRINTING_USART, btduartbuff);
#endif
				PS3BT->PS3NavigationConnected = true;

			} else if(PS3BT->remote_name_first == 'M') { // First letter in Motion Controller ('M')

				PS3BT->timer = (uint32_t)HCD_GetCurrentFrame(BTD_Machine.pdev);
#ifdef DEBUG_USB_HOST
				sprintf(btduartbuff,"\r\nMotion Controller Enabled\r\n");
				UARTPrintString(PRINTING_USART, btduartbuff);
#endif
				PS3BT->PS3MoveConnected = true;
			}
			PS3BT->ButtonState = 0; // Clear all values
			PS3BT->OldButtonState = 0;
			PS3BT->ButtonClickState = 0;
			onInit(PS3BT); // Turn on the LED on the controller
			PS3BT->l2cap_state = L2CAP_DONE;
		}
		break;

	case L2CAP_DONE:
		if(PS3BT->PS3MoveConnected) { // The Bulb and rumble values, has to be send at approximately every 5th second for it to stay on
			if((int32_t)((uint32_t)HCD_GetCurrentFrame(BTD_Machine.pdev) - PS3BT->timer) > 4000) { // Send at least every 4th second
				HIDMove_Command(PS3BT, PS3BT->HIDMoveBuffer, HID_BUFFERSIZE); // The Bulb and rumble values, has to be written again and again, for it to stay turned on
				PS3BT->timer = (uint32_t)HCD_GetCurrentFrame(BTD_Machine.pdev);
			}
		}
		break;
	}
}

/************************************************************/
/*                    HID Commands                          */
/************************************************************/

// Playstation Sixaxis Dualshock and Navigation Controller commands

void HID_Command(PS3BT_t *PS3BT, uint8_t* data, uint8_t nbytes) {
	if((int32_t)((uint32_t)HCD_GetCurrentFrame(BTD_Machine.pdev) - PS3BT->timerHID) <= 150) // Check if is has been more than 150ms since last command
		USB_OTG_BSP_mDelay((uint32_t)(150 - ((uint32_t)HCD_GetCurrentFrame(BTD_Machine.pdev) - PS3BT->timerHID))); // There have to be a delay between commands
	L2CAP_Command(PS3BT->PS3BTservice.hci_handle, data, nbytes, PS3BT->control_scid[0], PS3BT->control_scid[1]); // Both the Navigation and Dualshock controller sends data via the control channel
	PS3BT->timerHID = (uint32_t)HCD_GetCurrentFrame(BTD_Machine.pdev);
}

void PS3BTsetAllOff(PS3BT_t *PS3BT) {
	PS3BT->HIDBuffer[3] = 0x00; // Rumble bytes
	PS3BT->HIDBuffer[4] = 0x00;
	PS3BT->HIDBuffer[5] = 0x00;
	PS3BT->HIDBuffer[6] = 0x00;

	PS3BT->HIDBuffer[11] = 0x00; // LED byte

	HID_Command(PS3BT, PS3BT->HIDBuffer, HID_BUFFERSIZE);
}

void PS3BTsetRumbleOff(PS3BT_t *PS3BT) {
	uint8_t rumbleBuf[HID_BUFFERSIZE];
	memcpy(rumbleBuf, PS3BT->HIDBuffer, HID_BUFFERSIZE);
	rumbleBuf[3] = 0x00;
	rumbleBuf[4] = 0x00;
	rumbleBuf[5] = 0x00;
	rumbleBuf[6] = 0x00;
	HID_Command(PS3BT, rumbleBuf, HID_BUFFERSIZE);
}

void PS3BTsetRumbleOn(PS3BT_t *PS3BT, RumbleEnum mode) {
	uint8_t power[2] = {0xff, 0x00}; // Defaults to RumbleLow
	if(mode == RumbleHigh) {
		power[0] = 0x00;
		power[1] = 0xff;
	}
	setRumbleOn(PS3BT, 0xfe, power[0], 0xfe, power[1]);
}

void setRumbleOn(PS3BT_t *PS3BT, uint8_t rightDuration, uint8_t rightPower, uint8_t leftDuration, uint8_t leftPower) {
	uint8_t rumbleBuf[HID_BUFFERSIZE];
	memcpy(rumbleBuf, PS3BT->HIDBuffer, HID_BUFFERSIZE);
	rumbleBuf[3] = rightDuration;
	rumbleBuf[4] = rightPower;
	rumbleBuf[5] = leftDuration;
	rumbleBuf[6] = leftPower;
	HID_Command(PS3BT, rumbleBuf, HID_BUFFERSIZE);
}

void PS3BTsetLedRaw(PS3BT_t *PS3BT, uint8_t value) {
	PS3BT->HIDBuffer[11] = value << 1;
	HID_Command(PS3BT, PS3BT->HIDBuffer, HID_BUFFERSIZE);
}

void PS3BTsetAllLedOff(PS3BT_t *PS3BT) {
	PS3BTsetLedRaw(PS3BT, 0);
};

void PS3BTsetLedOff(PS3BT_t *PS3BT,LEDEnum a) {
	PS3BT->HIDBuffer[11] &= ~((uint8_t)((PS3_LEDS[(uint8_t)a] & 0x0f) << 1));
	HID_Command(PS3BT, PS3BT->HIDBuffer, HID_BUFFERSIZE);
}

void PS3BTsetLedOn(PS3BT_t *PS3BT, LEDEnum a) {
	if(a == OFF)
		PS3BTsetLedRaw(PS3BT, 0);
	else {
		PS3BT->HIDBuffer[11] |= (uint8_t)((PS3_LEDS[(uint8_t)a] & 0x0f) << 1);
		HID_Command(PS3BT, PS3BT->HIDBuffer, HID_BUFFERSIZE);
	}
}

void PS3BTsetLedToggle(PS3BT_t *PS3BT, LEDEnum a) {
	PS3BT->HIDBuffer[11] ^= (uint8_t)((PS3_LEDS[(uint8_t)a] & 0x0f) << 1);
	HID_Command(PS3BT, PS3BT->HIDBuffer, HID_BUFFERSIZE);
}

void enable_sixaxis(PS3BT_t *PS3BT) { // Command used to enable the Dualshock 3 and Navigation controller to send data via Bluetooth
	uint8_t cmd_buf[6];
	cmd_buf[0] = 0x53; // HID BT Set_report (0x50) | Report Type (Feature 0x03)
	cmd_buf[1] = 0xF4; // Report ID
	cmd_buf[2] = 0x42; // Special PS3 Controller enable commands
	cmd_buf[3] = 0x03;
	cmd_buf[4] = 0x00;
	cmd_buf[5] = 0x00;

	HID_Command(PS3BT,cmd_buf, 6);
}

// Playstation Move Controller commands

void HIDMove_Command(PS3BT_t *PS3BT, uint8_t* data, uint8_t nbytes) {
//	if((int32_t)((uint32_t)HCD_GetCurrentFrame(BTD_Machine.pdev) - PS3BT->timerHID) <= 150)// Check if is has been less than 150ms since last command
//		USB_OTG_BSP_mDelay((uint32_t)(150 - ((uint32_t)HCD_GetCurrentFrame(BTD_Machine.pdev) - PS3BT->timerHID))); // There have to be a delay between commands
	USB_OTG_BSP_mDelay(150);
	L2CAP_Command(PS3BT->PS3BTservice.hci_handle, data, nbytes, PS3BT->interrupt_scid[0], PS3BT->interrupt_scid[1]); // The Move controller sends it's data via the intterrupt channel
	PS3BT->timerHID = (uint32_t)HCD_GetCurrentFrame(BTD_Machine.pdev);
}

void PS3BTmoveSetBulb(PS3BT_t *PS3BT, uint8_t r, uint8_t g, uint8_t b) { // Use this to set the Color using RGB values
	// Set the Bulb's values into the write buffer
	PS3BT->HIDMoveBuffer[3] = r;
	PS3BT->HIDMoveBuffer[4] = g;
	PS3BT->HIDMoveBuffer[5] = b;

	HIDMove_Command(PS3BT, PS3BT->HIDMoveBuffer, HID_BUFFERSIZE);
}

void moveSetBulb(PS3BT_t *PS3BT, ColorsEnum color) { // Use this to set the Color using the predefined colors in enum
	PS3BTmoveSetBulb(PS3BT, (uint8_t)(color >> 16), (uint8_t)(color >> 8), (uint8_t)(color));
}

void PS3BTmoveSetRumble(PS3BT_t *PS3BT, uint8_t rumble) {
#ifdef DEBUG_USB_HOST
	if(rumble < 64 && rumble != 0){ // The rumble value has to at least 64, or approximately 25% (64/255*100)
		sprintf(btduartbuff,"\r\nThe rumble value has to at least 64, or approximately 25%");
		UARTPrintString(PRINTING_USART, btduartbuff);
	}
#endif
	// Set the rumble value into the write buffer
	PS3BT->HIDMoveBuffer[7] = rumble;

	HIDMove_Command(PS3BT, PS3BT->HIDMoveBuffer, HID_BUFFERSIZE);
}

void onInit(PS3BT_t *PS3BT) {

	if(PS3BT->PS3MoveConnected)
		moveSetBulb(PS3BT, Red);
	else{ // Dualshock 3 or Navigation controller
		PS3BTsetLedOn(PS3BT, LED1);
		PS3BTsetLedOn(PS3BT, LED2);
		PS3BTsetLedOn(PS3BT, LED3);
		PS3BTsetLedOn(PS3BT, LED4);
	}

}

