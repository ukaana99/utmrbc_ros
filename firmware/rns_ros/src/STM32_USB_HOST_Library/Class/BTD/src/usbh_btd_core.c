
#include "STM32_USB_HOST_Library/Class/BTD/inc/usbh_btd_core.h"

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma data_alignment=4
#endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN BTD_Machine_TypeDef        BTD_Machine __ALIGN_END ;

int flagbtd = 0;
int btdcount1 = 0 , btdcount2 = 0;

typedef enum
{
	HCI_IDLE = 0,
	HCI_SEND_DATA,
	HCI_BUSY,
	HCI_GET_DATA,
	HCI_SYNC,
	HCI_POLL,
	HCI_ERROR,
}
HCI_Event_State;

static USBH_Status USBH_BTD_InterfaceInit (USB_OTG_CORE_HANDLE *pdev,void *phost);
static void USBH_BTD_InterfaceDeInit (USB_OTG_CORE_HANDLE *pdev ,void *phost);
static USBH_Status USBH_BTD_ClassRequest (USB_OTG_CORE_HANDLE *pdev ,void *phost);
static USBH_Status USBH_BTD_Handle (USB_OTG_CORE_HANDLE *pdev , void *phost);

USBH_Class_cb_TypeDef  USBH_BTD_cb =
{
	USBH_BTD_InterfaceInit,
	USBH_BTD_InterfaceDeInit,
	USBH_BTD_ClassRequest,
	USBH_BTD_Handle
};

static USBH_Status USBH_BTD_InterfaceInit  (USB_OTG_CORE_HANDLE *pdev,void *phost)
{
	USBH_Status status = USBH_BUSY ;
	USBH_HOST *pphost = phost;

	BTD_Machine.pdev = pdev;
	BTD_Machine.phost = phost;
	InitializeBTD();

//	static uint8_t PS3CONNECTED = 0;
//
//
//#define PID		pphost->device_prop.Dev_Desc.idProduct
//#define VID		pphost->device_prop.Dev_Desc.idVendor
//
//	if(VID == PS3_VID && (PID == PS3_PID || PID == PS3NAVIGATION_PID || PID == PS3MOVE_PID)){
//
//		switch (PS3CONNECTED){
//
//		case 0 :
//
//#ifdef DEBUG_USB_HOST
//		if(PID == PS3_PID || PID == PS3NAVIGATION_PID) {
//			if(PID == PS3_PID) {
//				sprintf(btduartbuff,"\r\nDualshock 3 Controller Connected");
//				UARTPrintString(PRINTING_USART, btduartbuff);
//			} else { // It must be a navigation controller
//				sprintf(btduartbuff,"\r\nNavigation Controller Connected");
//				UARTPrintString(PRINTING_USART, btduartbuff);
//			}
//		} else { // It must be a Motion controller
//			sprintf(btduartbuff,"\r\nMotion Controller Connected");
//			UARTPrintString(PRINTING_USART, btduartbuff);
//		}
//#endif
//		if(BTD_Machine.my_bdaddr[0] == 0x00 && BTD_Machine.my_bdaddr[1] == 0x00 && BTD_Machine.my_bdaddr[2] == 0x00
//				 && BTD_Machine.my_bdaddr[3] == 0x00 && BTD_Machine.my_bdaddr[4] == 0x00 && BTD_Machine.my_bdaddr[5] == 0x00) {
//
//#ifdef DEBUG_USB_HOST
//			sprintf(btduartbuff,"\r\nPlease plug in the dongle before trying to pair with the PS3 Controller\r\nor set the Bluetooth address in the constructor of the PS3BT class");
//			UARTPrintString(PRINTING_USART, btduartbuff);
//#endif
//			status = USBH_NOT_SUPPORTED;
//
//		} else {
//
//			if(PID == PS3_PID || PID == PS3NAVIGATION_PID){
//				if( setBdaddr(BTD_Machine.my_bdaddr) == USBH_OK  ){// Set internal Bluetooth address
//					PS3CONNECTED = 1;
//				}
//			}
//		}
//		break;
//
//		case 1:
//
//#ifdef DEBUG_USB_HOST
//			sprintf(btduartbuff,"\r\nBluetooth Address was set to: %X:%X:%X:%X:%X:%X",BTD_Machine.my_bdaddr[5],BTD_Machine.my_bdaddr[4],BTD_Machine.my_bdaddr[3],BTD_Machine.my_bdaddr[2],BTD_Machine.my_bdaddr[1],BTD_Machine.my_bdaddr[0]);
//			UARTPrintString(PRINTING_USART, btduartbuff);
//#endif
//			if( USBH_SetCfg(pdev, phost, 0) == USBH_OK)
//				PS3CONNECTED = 2;
//			break;
//
//		case 2:
//
//			if( USBH_SetAddress(pdev, phost,0) == USBH_OK ){
//
//				status = USBH_NOT_SUPPORTED;
//			}
//			break;
//		}
//
//	}else

	if((pphost->device_prop.Itf_Desc[0].bInterfaceClass == USB_WIRELESS_CONTROLLER) && \
			(pphost->device_prop.Itf_Desc[0].bInterfaceProtocol == WI_PROTOCOL_BT)&&\
			(pphost->device_prop.Itf_Desc[0].bInterfaceSubClass == WI_SUBCLASS_RF)){

		for(int i = 0; i < 3; i++){

			if (pphost->device_prop.Ep_Desc[0][i].bEndpointAddress == 0x81 && pphost->device_prop.Ep_Desc[0][i].bmAttributes == 0x03) {
				/*end point 1 - interrupt In*/
				BTD_Machine.BTDIntInEp = (pphost->device_prop.Ep_Desc[0][i].bEndpointAddress);
				BTD_Machine.BTDIntInEpSize  = pphost->device_prop.Ep_Desc[0][i].wMaxPacketSize;
				BTD_Machine.hc_num_it_in  = USBH_Alloc_Channel(pdev,pphost->device_prop.Ep_Desc[0][i].bEndpointAddress);
				USBH_Open_Channel  (pdev, BTD_Machine.hc_num_it_in, pphost->device_prop.address, pphost->device_prop.speed, EP_TYPE_INTR, BTD_Machine.BTDIntInEpSize);
			} else if (pphost->device_prop.Ep_Desc[0][i].bEndpointAddress == 0x02) {
				/*end point 2 - bulk OUT*/
				BTD_Machine.BTDBulkOutEp = (pphost->device_prop.Ep_Desc[0][i].bEndpointAddress && pphost->device_prop.Ep_Desc[0][i].bmAttributes == 0x02);
				BTD_Machine.BTDBulkOutEpSize  = pphost->device_prop.Ep_Desc[0][i].wMaxPacketSize;
				BTD_Machine.hc_num_bulk_out  = USBH_Alloc_Channel(pdev,pphost->device_prop.Ep_Desc[0][i].bEndpointAddress);
				USBH_Open_Channel  (pdev, BTD_Machine.hc_num_bulk_out, pphost->device_prop.address, pphost->device_prop.speed, EP_TYPE_BULK, BTD_Machine.BTDBulkOutEpSize);
			} else if (pphost->device_prop.Ep_Desc[0][i].bEndpointAddress == 0x82 && pphost->device_prop.Ep_Desc[0][i].bmAttributes == 0x02) {
				/*end point 3 - bulk IN*/
				BTD_Machine.BTDBulkInEp = (pphost->device_prop.Ep_Desc[0][i].bEndpointAddress);
				BTD_Machine.BTDBulkInEpSize  = pphost->device_prop.Ep_Desc[0][i].wMaxPacketSize;
				BTD_Machine.hc_num_bulk_in = USBH_Alloc_Channel(pdev,pphost->device_prop.Ep_Desc[0][i].bEndpointAddress);
				USBH_Open_Channel  (pdev, BTD_Machine.hc_num_bulk_in, pphost->device_prop.address, pphost->device_prop.speed, EP_TYPE_BULK, BTD_Machine.BTDBulkInEpSize);
			}

		}

		BTD_Machine.hci_num_reset_loops = 100; // only loop 50000 times before trying to send the hci reset command
		BTD_Machine.hci_counter = 0;
		BTD_Machine.hci_state = HCI_INIT_STATE;
		BTD_Machine.waitingForConnection = false;
		BTD_Machine.acli_enable = 0;

		sprintf(btduartbuff,"\r\nBluetooth Dongle Initialized");
		UARTPrintString(PRINTING_USART, btduartbuff);

		status = USBH_OK;

	} else {

		pphost->usr_cb->DeviceNotSupported();
		status =  USBH_NOT_SUPPORTED;
	}


	return status;
}

static void USBH_BTD_InterfaceDeInit  (USB_OTG_CORE_HANDLE *pdev ,void *phost){

	if(BTD_Machine.hc_num_it_in != 0x00){
		USB_OTG_HC_Halt(pdev, BTD_Machine.hc_num_it_in);
		USBH_Free_Channel  (pdev, BTD_Machine.hc_num_it_in);
		BTD_Machine.hc_num_it_in = 0;     /* Reset the Channel as Free */
	}

	if(BTD_Machine.hc_num_bulk_in != 0x00){
		USB_OTG_HC_Halt(pdev, BTD_Machine.hc_num_bulk_in);
		USBH_Free_Channel  (pdev, BTD_Machine.hc_num_bulk_in);
		BTD_Machine.hc_num_bulk_in = 0;     /* Reset the Channel as Free */
	}

	if(BTD_Machine.hc_num_bulk_out != 0x00){
		USB_OTG_HC_Halt(pdev, BTD_Machine.hc_num_bulk_out);
		USBH_Free_Channel  (pdev, BTD_Machine.hc_num_bulk_out);
		BTD_Machine.hc_num_bulk_out = 0;     /* Reset the Channel as Free */
	}

	InitializeBTD();
	BTD_Machine.pairWithHIDDevice = false;

	for(uint8_t i = 0; i < BTD_NUM_SERVICES; i++)
		if(BTD_Machine.btService[i])
			BTD_Machine.btService[i]->Reset(BTD_Machine.btservive_pointer_address[i]);
}

static USBH_Status USBH_BTD_ClassRequest(USB_OTG_CORE_HANDLE *pdev ,void *phost){

	USBH_Status status = USBH_BUSY ;
	USBH_HOST *pphost = phost;
	status = USBH_OK ;
	return status;

}

static USBH_Status USBH_BTD_Handle(USB_OTG_CORE_HANDLE *pdev , void *phost){

	USBH_HOST *pphost = phost;
	USBH_Status status = USBH_BUSY ;

	switch(BTD_Machine.int_state){

	case HCI_IDLE:

		BTD_Machine.int_state = HCI_SYNC;

	case HCI_SYNC:

		if(USB_OTG_IsEvenFrame(pdev) == TRUE)
			BTD_Machine.int_state = HCI_GET_DATA;
		break;

	case HCI_GET_DATA:

		USBH_InterruptReceiveData(pdev, BTD_Machine.hcibuf, BTD_Machine.BTDIntInEpSize, BTD_Machine.hc_num_it_in);
		BTD_Machine.int_state = HCI_POLL;
		BTD_Machine.timer = HCD_GetCurrentFrame(pdev);
		break;

	case HCI_POLL:

		if(( HCD_GetCurrentFrame(pdev) - BTD_Machine.timer) >= 2)
			BTD_Machine.int_state = HCI_GET_DATA;

		if(HCD_GetURB_State(pdev , BTD_Machine.hc_num_it_in) == URB_DONE ||HCD_GetHCState(pdev , BTD_Machine.hc_num_it_in) == HC_NAK){
			HCI_event_task();
		}else if(HCD_GetURB_State(pdev, BTD_Machine.hc_num_it_in) == URB_STALL || HCD_GetURB_State(pdev, BTD_Machine.hc_num_it_in) == URB_ERROR) {

			if( (USBH_ClrFeature(pdev, phost,BTD_Machine.BTDIntInEp,BTD_Machine.hc_num_it_in)) == USBH_OK)
				BTD_Machine.int_state = HCI_GET_DATA;

		}
		break;

	default:break;

	}

	if((int32_t)((uint32_t)HCD_GetCurrentFrame(pdev) -  BTD_Machine.qNextPollTime) >= 0L) { // Don't poll if shorter than polling interval
		BTD_Machine.qNextPollTime = (uint32_t)HCD_GetCurrentFrame(pdev) +  2; // Set new poll time
		HCI_task();
	}

	//	HCI_task();
	ACL_event_task();
	//	pphost->usr_cb->UserApplication();

	status = USBH_OK ;

	return status;
}

void InitializeBTD() {

	for( uint8_t i = 0; i < BTD_NUM_SERVICES; i++) {
		if(BTD_Machine.btService[i])
			BTD_Machine.btService[i]->Reset(BTD_Machine.btservive_pointer_address[i]); // Reset all Bluetooth services
	}

	BTD_Machine.connectToHIDDevice = false;
	BTD_Machine.incomingHIDDevice = false;
	BTD_Machine.incomingPS4 = false;
	BTD_Machine.acli_enable = 0;
}

void disconnect() {
	for(uint8_t i = 0; i < BTD_NUM_SERVICES; i++)
		if(BTD_Machine.btService[i])
			BTD_Machine.btService[i]->disconnect(BTD_Machine.btservive_pointer_address[i]);
};

int8_t registerBluetoothService(uint32_t pointer_address,BluetoothService *pService) {
	for(uint8_t i = 0; i < BTD_NUM_SERVICES; i++) {
		if(!BTD_Machine.btService[i]) {
			BTD_Machine.btService[i] = pService;
			BTD_Machine.btservive_pointer_address[i] = pointer_address;
			return i; // Return ID
		}
	}
	return -1; // Error registering BluetoothService
};


/** Call this function to pair with a HID device */
void pairWithHID() {
	BTD_Machine.waitingForConnection = false;
	BTD_Machine.pairWithHIDDevice = true;
	BTD_Machine.hci_state = HCI_CHECK_DEVICE_SERVICE;
};

bool checkHciHandle(uint8_t *buf, uint16_t handle) {
	return (buf[0] == (handle & 0xFF)) && (buf[1] == ((handle >> 8) | 0x20));
}

void HCI_event_task() {

	switch(BTD_Machine.hcibuf[0]) { // Switch on event type

	case EV_COMMAND_COMPLETE:

		if(!BTD_Machine.hcibuf[5]) { // Check if command succeeded

			hci_set_flag(HCI_FLAG_CMD_COMPLETE); // Set command complete flag

			if((BTD_Machine.hcibuf[3] == 0x01) && (BTD_Machine.hcibuf[4] == 0x10)) { // Parameters from read local version information

				BTD_Machine.hci_version = BTD_Machine.hcibuf[6]; // Used to check if it supports 2.0+EDR - see http://www.bluetooth.org/Technical/AssignedNumbers/hci.htm
				hci_set_flag(HCI_FLAG_READ_VERSION);

			} else if((BTD_Machine.hcibuf[3] == 0x09) && (BTD_Machine.hcibuf[4] == 0x10)) { // Parameters from read local bluetooth address

				for(uint8_t i = 0; i < 6; i++)
					BTD_Machine.my_bdaddr[i] = BTD_Machine.hcibuf[6 + i];
				hci_set_flag(HCI_FLAG_READ_BDADDR);

			}

		}


		break;

	case EV_COMMAND_STATUS:

#ifdef DEBUG_USB_HOST
		if(BTD_Machine.hcibuf[2]) { // Show status on serial if not OK
			sprintf(btduartbuff,"\r\nHCI Command Failed: %X", BTD_Machine.hcibuf[2]);
			UARTPrintString(PRINTING_USART, btduartbuff);
		}
#endif
		break;

	case EV_INQUIRY_COMPLETE:

		if(BTD_Machine.inquiry_counter >= 70000 &&  BTD_Machine.pairWithHIDDevice) {
			BTD_Machine.inquiry_counter = 0;

#ifdef DEBUG_USB_HOST
			sprintf(btduartbuff,"\r\nCouldn't find HID device");
			UARTPrintString(PRINTING_USART, btduartbuff);
#endif

			BTD_Machine.connectToHIDDevice = false;
			BTD_Machine.pairWithHIDDevice = false;
			BTD_Machine.hci_state = HCI_SCANNING_STATE;
		}
		BTD_Machine.inquiry_counter++;
		break;

	case EV_INQUIRY_RESULT:

		if(BTD_Machine.hcibuf[2]) { // Check that there is more than zero responses
#ifdef EXTRADEBUG
			sprintf(btduartbuff,"\r\nNumber of responses: %X",BTD_Machine.hcibuf[2]);
			UARTPrintString(PRINTING_USART, btduartbuff);
#endif
			for(uint8_t i = 0; i < BTD_Machine.hcibuf[2]; i++) {
				uint8_t offset = 8 * BTD_Machine.hcibuf[2] + 3 * i;

				for(uint8_t j = 0; j < 3; j++)
					BTD_Machine.classOfDevice[j] = BTD_Machine.hcibuf[j + 4 + offset];

#ifdef EXTRADEBUG
				sprintf(btduartbuff,"\r\nClass of device:  \"%X\" \"%X\" \"%X\" ",BTD_Machine.classOfDevice[2],BTD_Machine.classOfDevice[1], BTD_Machine.classOfDevice[0]);
				UARTPrintString(PRINTING_USART, btduartbuff);
#endif

				if(BTD_Machine.pairWithHIDDevice && (BTD_Machine.classOfDevice[1] & 0x05) && (BTD_Machine.classOfDevice[0] & 0xC8)) { // Check if it is a mouse, keyboard or a gamepad - see: http://bluetooth-pentest.narod.ru/software/bluetooth_class_of_device-service_generator.html
#ifdef DEBUG_USB_HOST
					if(BTD_Machine.classOfDevice[0] & 0x80){
						sprintf(btduartbuff,"\r\nMouse found");
						UARTPrintString(PRINTING_USART, btduartbuff);
					}

					if(BTD_Machine.classOfDevice[0] & 0x40){
						sprintf(btduartbuff,"\r\nKeyboard found");
						UARTPrintString(PRINTING_USART, btduartbuff);
					}

					if(BTD_Machine.classOfDevice[0] & 0x08){
						sprintf(btduartbuff,"\r\nGamepad found");
						UARTPrintString(PRINTING_USART, btduartbuff);
					}
#endif

					for(uint8_t j = 0; j < 6; j++)
						BTD_Machine.disc_bdaddr[j] = BTD_Machine.hcibuf[j + 3 + 6 * i];

					sprintf(btduartbuff,"\r\ndisc_bdaddr: %X:%X:%X:%X:%X:%X",BTD_Machine.disc_bdaddr[5],BTD_Machine.disc_bdaddr[4],BTD_Machine.disc_bdaddr[3],BTD_Machine.disc_bdaddr[2],BTD_Machine.disc_bdaddr[1],BTD_Machine.disc_bdaddr[0]);
					UARTPrintString(PRINTING_USART, btduartbuff);

					hci_set_flag(HCI_FLAG_DEVICE_FOUND);
					break;
				}
			}
		}
		break;

	case EV_CONNECT_COMPLETE:
		hci_set_flag(HCI_FLAG_CONNECT_EVENT);
		if(!BTD_Machine.hcibuf[2]) { // Check if connected OK
#ifdef EXTRADEBUG
			sprintf(btduartbuff,"\r\nConnection established");
			UARTPrintString(PRINTING_USART, btduartbuff);
			sprintf(btduartbuff,"\r\nconnected_bdaddr: %X:%X:%X:%X:%X:%X",BTD_Machine.hcibuf[10],BTD_Machine.hcibuf[9],BTD_Machine.hcibuf[8],BTD_Machine.hcibuf[7],BTD_Machine.hcibuf[6],BTD_Machine.hcibuf[5]);
			UARTPrintString(PRINTING_USART, btduartbuff);
#endif
			BTD_Machine.hci_handle = BTD_Machine.hcibuf[3] | ((BTD_Machine.hcibuf[4] & 0x0F) << 8); // Store the handle for the ACL connection


			hci_set_flag(HCI_FLAG_CONNECT_COMPLETE); // Set connection complete flag
		} else {
			BTD_Machine.hci_state = HCI_CHECK_DEVICE_SERVICE;
#ifdef DEBUG_USB_HOST
			sprintf(btduartbuff,"\r\nConnection Failed: %X", BTD_Machine.hcibuf[2]);
			UARTPrintString(PRINTING_USART, btduartbuff);
#endif
		}
		break;

	case EV_DISCONNECT_COMPLETE:

		if(!BTD_Machine.hcibuf[2]) { // Check if disconnected OK
			hci_set_flag(HCI_FLAG_DISCONNECT_COMPLETE); // Set disconnect command complete flag
			hci_clear_flag(HCI_FLAG_CONNECT_COMPLETE); // Clear connection complete flag
		}
		break;

	case EV_REMOTE_NAME_COMPLETE:

		if(!BTD_Machine.hcibuf[2]) { // Check if reading is OK
			for(uint8_t i = 0; i < MIN(sizeof (BTD_Machine.remote_name), sizeof (BTD_Machine.hcibuf) - 9); i++) {

				BTD_Machine.remote_name[i] = BTD_Machine.hcibuf[9 + i];
				if(BTD_Machine.remote_name[i] == '\0') // End of string
					break;
			}
			hci_set_flag(HCI_FLAG_REMOTE_NAME_COMPLETE);
		}
		break;

	case EV_INCOMING_CONNECT:

		for(uint8_t i = 0; i < 6; i++)
			BTD_Machine.disc_bdaddr[i] = BTD_Machine.hcibuf[i + 2];

		for(uint8_t i = 0; i < 3; i++)
			BTD_Machine.classOfDevice[i] = BTD_Machine.hcibuf[i + 8];

		if((BTD_Machine.classOfDevice[1] & 0x05) && (BTD_Machine.classOfDevice[0] & 0xC8)) { // Check if it is a mouse, keyboard or a gamepad
#ifdef DEBUG_USB_HOST
			if(BTD_Machine.classOfDevice[0] & 0x80){
				sprintf(btduartbuff,"\r\nMouse is connecting");
				UARTPrintString(PRINTING_USART, btduartbuff);
			}

			if(BTD_Machine.classOfDevice[0] & 0x40){
				sprintf(btduartbuff,"\r\nKeyboard is connecting");
				UARTPrintString(PRINTING_USART, btduartbuff);
			}

			if(BTD_Machine.classOfDevice[0] & 0x08){
				sprintf(btduartbuff,"\r\nGamepad is connecting");
				UARTPrintString(PRINTING_USART, btduartbuff);
			}
#endif

			BTD_Machine.incomingHIDDevice = true;
		}

#ifdef EXTRADEBUG
		sprintf(btduartbuff,"\r\nClass of device:  \"%X\" \"%X\" \"%X\" ",BTD_Machine.classOfDevice[2],BTD_Machine.classOfDevice[1], BTD_Machine.classOfDevice[0]);
		UARTPrintString(PRINTING_USART, btduartbuff);
#endif
		hci_set_flag(HCI_FLAG_INCOMING_REQUEST);
		break;

	case EV_PIN_CODE_REQUEST:

		if(BTD_Machine.btdPin != NULL) {
#ifdef DEBUG_USB_HOST
			sprintf(btduartbuff,"\r\nBluetooth pin is set too: %s", BTD_Machine.btdPin);
			UARTPrintString(PRINTING_USART, btduartbuff);
#endif
			if(btdcount2 <2){
				hci_pin_code_request_reply();
				btdcount2 ++;
				btdcount1 = 0;
			}
		} else {
#ifdef DEBUG_USB_HOST
			sprintf(btduartbuff,"\r\nNo pin was set");
			UARTPrintString(PRINTING_USART, btduartbuff);
#endif
			hci_pin_code_negative_request_reply();
		}
		break;

	case EV_LINK_KEY_REQUEST:

#ifdef DEBUG_USB_HOST
		sprintf(btduartbuff,"\r\nReceived Key Request");
		UARTPrintString(PRINTING_USART, btduartbuff);
#endif

		if(btdcount1 < 2){
			hci_link_key_request_negative_reply();
			btdcount1 ++;
			btdcount2 = 0;
		}
		break;

	case EV_AUTHENTICATION_COMPLETE:
		if(!BTD_Machine.hcibuf[2]) { // Check if pairing was successful

			if(BTD_Machine.pairWithHIDDevice && !BTD_Machine.connectToHIDDevice) {
#ifdef DEBUG_USB_HOST
				sprintf(btduartbuff,"\r\nPairing successful with HID device");
				UARTPrintString(PRINTING_USART, btduartbuff);
#endif
				BTD_Machine.acli_enable = 1;
				BTD_Machine.connectToHIDDevice = true; // Used to indicate to the BTHID service, that it should connect to this device
			}
		} else {
#ifdef DEBUG_USB_HOST
			sprintf(btduartbuff,"\r\nPairing Failed: %X", BTD_Machine.hcibuf[2]);
			UARTPrintString(PRINTING_USART, btduartbuff);
#endif
			if(hci_disconnect(BTD_Machine.hci_handle) ==true);
			BTD_Machine.hci_state = HCI_DISCONNECT_STATE;
		}
		break;

		/* We will just ignore the following events */
	case EV_NUM_COMPLETE_PKT:
	case EV_ROLE_CHANGED:
	case EV_PAGE_SCAN_REP_MODE:
	case EV_LOOPBACK_COMMAND:
	case EV_DATA_BUFFER_OVERFLOW:
	case EV_CHANGE_CONNECTION_LINK:
	case EV_MAX_SLOTS_CHANGE:
	case EV_QOS_SETUP_COMPLETE:
	case EV_LINK_KEY_NOTIFICATION:
	case EV_ENCRYPTION_CHANGE:
	case EV_READ_REMOTE_VERSION_INFORMATION_COMPLETE:
		break;

#ifdef EXTRADEBUG
	default:

		if(BTD_Machine.hcibuf[0] != 0x00) {
			sprintf(btduartbuff,"\r\nUnmanaged HCI Event: %X", BTD_Machine.hcibuf[0]);
			UARTPrintString(PRINTING_USART, btduartbuff);
		}
		break;
#endif
	}

}


void HCI_task() {

	switch(BTD_Machine.hci_state) {

	case HCI_INIT_STATE:

		BTD_Machine.hci_counter++;
		if(BTD_Machine.hci_counter > BTD_Machine.hci_num_reset_loops) { // wait until we have looped x times to clear any old events
			if(hci_reset()== true){
				BTD_Machine.hci_state = HCI_RESET_STATE;
				BTD_Machine.hci_counter = 0;
			}
		}
		break;

	case HCI_RESET_STATE:

		BTD_Machine.hci_counter++;
		if(hci_check_flag(HCI_FLAG_CMD_COMPLETE)) {
			BTD_Machine.hci_counter = 0;

#ifdef DEBUG_USB_HOST
			sprintf(btduartbuff,"\r\nHCI Reset complete");
			UARTPrintString(PRINTING_USART, btduartbuff);
#endif
			if(hci_write_class_of_device() == true ){
				BTD_Machine.hci_state = HCI_CLASS_STATE;
			}

		}else if (BTD_Machine.hci_counter > BTD_Machine.hci_num_reset_loops) {
			BTD_Machine.hci_num_reset_loops *= 100;
			if(BTD_Machine.hci_num_reset_loops > 2000)
				BTD_Machine.hci_num_reset_loops = 2000;
#ifdef DEBUG_USB_HOST
			sprintf(btduartbuff,"\r\nNo response to HCI Reset");
			UARTPrintString(PRINTING_USART, btduartbuff);
#endif
			BTD_Machine.hci_state = HCI_INIT_STATE;
			BTD_Machine.hci_counter = 0;
		}

		break;

	case HCI_CLASS_STATE:

		if(hci_check_flag(HCI_FLAG_CMD_COMPLETE)) {
#ifdef DEBUG_USB_HOST
			sprintf(btduartbuff,"\r\nWrite class of device");
			UARTPrintString(PRINTING_USART, btduartbuff);
#endif
			if(hci_read_bdaddr()== true){
				BTD_Machine.hci_state = HCI_BDADDR_STATE;
			}

		}

		break;

	case HCI_BDADDR_STATE:

		if(hci_check_flag(HCI_FLAG_READ_BDADDR)) {
#ifdef DEBUG_USB_HOST
			sprintf(btduartbuff,"\r\nLocal Bluetooth Address: %X:%X:%X:%X:%X:%X",BTD_Machine.my_bdaddr[5],BTD_Machine.my_bdaddr[4],BTD_Machine.my_bdaddr[3],BTD_Machine.my_bdaddr[2],BTD_Machine.my_bdaddr[1],BTD_Machine.my_bdaddr[0]);
			UARTPrintString(PRINTING_USART, btduartbuff);
#endif
			if(hci_read_local_version_information()== true){
				BTD_Machine.hci_state = HCI_LOCAL_VERSION_STATE;
			}
		}
		break;

	case HCI_LOCAL_VERSION_STATE: // The local version is used by the PS3BT class

		if(hci_check_flag(HCI_FLAG_READ_VERSION)) {
			if(BTD_Machine.btdName != NULL) {
				if(hci_set_local_name(BTD_Machine.btdName) == true)
					BTD_Machine.hci_state = HCI_SET_NAME_STATE;
			} else
				BTD_Machine.hci_state = HCI_CHECK_DEVICE_SERVICE;
		}
		break;

	case HCI_SET_NAME_STATE:

		if(hci_check_flag(HCI_FLAG_CMD_COMPLETE)) {
#ifdef DEBUG_USB_HOST
			sprintf(btduartbuff,"\r\nThe name is set to: %s",BTD_Machine.btdName);
			UARTPrintString(PRINTING_USART, btduartbuff);
#endif
			BTD_Machine.hci_state = HCI_CHECK_DEVICE_SERVICE;
		}
		break;

	case HCI_CHECK_DEVICE_SERVICE:
		if(BTD_Machine.pairWithHIDDevice ) { // Check if it should try to connect to a Wiimote
#ifdef DEBUG_USB_HOST

			sprintf(btduartbuff,"\r\nPlease enable discovery of your device");
			UARTPrintString(PRINTING_USART, btduartbuff);

#endif
			if(hci_inquiry() == true ){
				BTD_Machine.hci_state = HCI_INQUIRY_STATE;
			}
		} else
			BTD_Machine.hci_state = HCI_SCANNING_STATE; // Don't try to connect to a Wiimote
		break;

	case HCI_INQUIRY_STATE:

		if(hci_check_flag(HCI_FLAG_DEVICE_FOUND)) {

			if(hci_inquiry_cancel() == true) {

#ifdef DEBUG_USB_HOST

				sprintf(btduartbuff,"\r\nHID device found");
				UARTPrintString(PRINTING_USART, btduartbuff);

				sprintf(btduartbuff,"\r\nNow just create the instance like so:");
				UARTPrintString(PRINTING_USART, btduartbuff);


				sprintf(btduartbuff,"\r\nBTHID bthid(&Btd)");
				UARTPrintString(PRINTING_USART, btduartbuff);


				sprintf(btduartbuff,"\r\nAnd then press any button on the ");
				UARTPrintString(PRINTING_USART, btduartbuff);

				sprintf(btduartbuff,"device");
				UARTPrintString(PRINTING_USART, btduartbuff);

#endif
				//				if(BTD_Machine.checkRemoteName) {
				//					if(hci_remote_name() == true)// We need to know the name to distinguish between the Wiimote, the new Wiimote with Motion Plus inside, a Wii U Pro Controller and a Wii Balance Board
				//						BTD_Machine.hci_state = HCI_REMOTE_NAME_STATE;
				//				} else
				BTD_Machine.hci_state = HCI_CONNECT_DEVICE_STATE;
			}
		}
		break;

	case HCI_CONNECT_DEVICE_STATE:

		if(hci_check_flag(HCI_FLAG_CMD_COMPLETE)) {
#ifdef DEBUG_USB_HOST

			//			sprintf(btduartbuff,"\r\nConnecting to HID device");
			//			UARTPrintString(PRINTING_USART, btduartbuff);

#endif
			BTD_Machine.checkRemoteName = false;
			flagbtd = 1;
		}

		//		USB_OTG_BSP_mDelay(3);
		//		sprintf(btduartbuff,"\r\nConnecting to HID device");
		//		UARTPrintString(PRINTING_USART, btduartbuff);

		if(flagbtd == 1){
			sprintf(btduartbuff,"\r\nConnecting to HID device");
			UARTPrintString(PRINTING_USART, btduartbuff);
			if( hci_connect_last_discovered() == true){
				BTD_Machine.hci_state = HCI_CONNECTED_DEVICE_STATE;
				flagbtd = 0;
			}
		}

		break;

	case HCI_CONNECTED_DEVICE_STATE:
		if(hci_check_flag(HCI_FLAG_CONNECT_EVENT)) {
			if(hci_check_flag(HCI_FLAG_CONNECT_COMPLETE)) {
#ifdef DEBUG_USB_HOST

				sprintf(btduartbuff,"\r\nConnected to HID device");
				UARTPrintString(PRINTING_USART, btduartbuff);

#endif
				if(hci_authentication_request() == true) // This will start the pairing with the Wiimote
					BTD_Machine.hci_state = HCI_SCANNING_STATE;
			} else {
#ifdef DEBUG_USB_HOST
				sprintf(btduartbuff,"\r\nTrying to connect one more time...");
				UARTPrintString(PRINTING_USART, btduartbuff);
#endif
				hci_connect_last_discovered(); // Try to connect one more time
			}

		}
		break;

	case HCI_SCANNING_STATE:

		if( !BTD_Machine.connectToHIDDevice && !BTD_Machine.pairWithHIDDevice) {
#ifdef DEBUG_USB_HOST
			sprintf(btduartbuff,"\r\nWait For Incoming Connection Request");
			UARTPrintString(PRINTING_USART, btduartbuff);
#endif
			if(hci_write_scan_enable()== true){
				BTD_Machine.waitingForConnection = true;
				BTD_Machine.hci_state = HCI_CONNECT_IN_STATE;
			}
		}
		break;

	case HCI_CONNECT_IN_STATE:
		if(hci_check_flag(HCI_FLAG_INCOMING_REQUEST)) {
			BTD_Machine.waitingForConnection = false;
#ifdef DEBUG_USB_HOST
			sprintf(btduartbuff,"\r\nIncoming Connection Request");
			UARTPrintString(PRINTING_USART, btduartbuff);
#endif
			if(hci_remote_name() == true)
				BTD_Machine.hci_state = HCI_REMOTE_NAME_STATE;

		} else if(hci_check_flag(HCI_FLAG_DISCONNECT_COMPLETE))
			BTD_Machine.hci_state = HCI_DISCONNECT_STATE;
		break;

	case HCI_REMOTE_NAME_STATE:
		if(hci_check_flag(HCI_FLAG_REMOTE_NAME_COMPLETE)) {
#ifdef DEBUG_USB_HOST
			sprintf(btduartbuff,"\r\nRemote Name: ");
			UARTPrintString(PRINTING_USART, btduartbuff);
			for(uint8_t i = 0; i < strlen(BTD_Machine.remote_name); i++){
				sprintf(btduartbuff,"%c",BTD_Machine.remote_name[i]);
				UARTPrintString(PRINTING_USART, btduartbuff);
			}
#endif


			//			if(BTD_Machine.classOfDevice[2] == 0 && BTD_Machine.classOfDevice[1] == 0x25 && BTD_Machine.classOfDevice[0] == 0x08 && strncmp((const char*)BTD_Machine.remote_name, "Wireless Controller", 19) == 0) {
			if(BTD_Machine.classOfDevice[2] == 0 && BTD_Machine.classOfDevice[1] == 0x25 && BTD_Machine.classOfDevice[0] == 0x08) {
#ifdef DEBUG_USB_HOST
				sprintf(btduartbuff,"\r\nPS4 controller is connecting");
				UARTPrintString(PRINTING_USART, btduartbuff);
#endif
				BTD_Machine.incomingPS4 = true;
			}


			if( hci_accept_connection() == true )
				BTD_Machine.hci_state = HCI_CONNECTED_STATE;

		}
		break;

	case HCI_CONNECTED_STATE:
		if(hci_check_flag(HCI_FLAG_CONNECT_COMPLETE)) {
#ifdef DEBUG_USB_HOST
			sprintf(btduartbuff,"\r\nConnected to Device: %X:%X:%X:%X:%X:%X",BTD_Machine.disc_bdaddr[5],BTD_Machine.disc_bdaddr[4],BTD_Machine.disc_bdaddr[3],BTD_Machine.disc_bdaddr[2],BTD_Machine.disc_bdaddr[1],BTD_Machine.disc_bdaddr[0]);
			UARTPrintString(PRINTING_USART, btduartbuff);

#endif
			if(BTD_Machine.incomingPS4)
				BTD_Machine.connectToHIDDevice = true; // We should always connect to the PS4 controller

			// Clear these flags for a new connection
			BTD_Machine.l2capConnectionClaimed = false;
			BTD_Machine.sdpConnectionClaimed = false;
			BTD_Machine.rfcommConnectionClaimed = false;

			BTD_Machine.hci_event_flag = 0;
			BTD_Machine.hci_state = HCI_DONE_STATE;
			BTD_Machine.acli_enable = 1;
			BTD_Machine.hci_counter= 0;
		}
		break;

	case HCI_DONE_STATE:

		BTD_Machine.hci_counter++;
		if(BTD_Machine.hci_counter > 50000) { // Wait until we have looped 500000 times to make sure that the L2CAP connection has been started
			BTD_Machine.hci_counter = 0;
			BTD_Machine.hci_state = HCI_SCANNING_STATE;
		}
		break;

	case HCI_DISCONNECT_STATE:
		if(hci_check_flag(HCI_FLAG_DISCONNECT_COMPLETE)) {

#ifdef DEBUG_USB_HOST
			sprintf(btduartbuff,"\r\nHCI Disconnected from Device");
			UARTPrintString(PRINTING_USART, btduartbuff);
#endif
			BTD_Machine.hci_event_flag = 0; // Clear all flags
			BTD_Machine.acli_enable = 0;
			// Reset all buffers
			memset(BTD_Machine.hcibuf, 0, BULK_MAXPKTSIZE);
			memset(BTD_Machine.l2capinbuf, 0, BULK_MAXPKTSIZE);

			BTD_Machine.connectToHIDDevice = BTD_Machine.incomingHIDDevice = BTD_Machine.pairWithHIDDevice = BTD_Machine.checkRemoteName = false;
			BTD_Machine.incomingPS4 = false;

			BTD_Machine.hci_state = HCI_SCANNING_STATE;
		}
		break;

	default:
		break;
	}
}

void ACL_event_task() {

	if(BTD_Machine.acli_enable){

		switch(BTD_Machine.acli_state){

		case 0:

			USBH_BulkReceiveData(BTD_Machine.pdev, BTD_Machine.l2capinbuf,BULK_MAXPKTSIZE,BTD_Machine.hc_num_bulk_in);
			BTD_Machine.acli_state = 1;
			break;

		case 1:

			switch(HCD_GetURB_State(BTD_Machine.pdev, BTD_Machine.hc_num_bulk_in)){

			case URB_DONE:

				for(uint8_t i = 0; i < BTD_NUM_SERVICES; i++) {
					if(BTD_Machine.btService[i])
						BTD_Machine.btService[i]->ACLData(BTD_Machine.l2capinbuf, BTD_Machine.btservive_pointer_address[i]);
				}

				//USBH_BulkReceiveData(BTD_Machine.pdev, BTD_Machine.l2capinbuf,BULK_MAXPKTSIZE,BTD_Machine.hc_num_bulk_in);
									BTD_Machine.acli_state = 0;
				break;

			case URB_NOTREADY:

				sprintf(btduartbuff,"\r\nACL URB_NOTREADY");
				UARTPrintString(PRINTING_USART, btduartbuff);

				USBH_BulkReceiveData(BTD_Machine.pdev, BTD_Machine.l2capinbuf,BULK_MAXPKTSIZE,BTD_Machine.hc_num_bulk_in);
				break;

			case URB_STALL:

				if(USBH_OK == USBH_ClrFeature(BTD_Machine.pdev, BTD_Machine.phost, BTD_Machine.BTDBulkInEp, BTD_Machine.hc_num_bulk_in))
					BTD_Machine.acli_state = 0;
				break;

			case URB_ERROR:

				if(USBH_OK == USBH_ClrFeature(BTD_Machine.pdev, BTD_Machine.phost, BTD_Machine.BTDBulkInEp, BTD_Machine.hc_num_bulk_in))
					BTD_Machine.acli_state = 0;

				break;

			default:
				break;

			}
			break;

			default:
				break;
		}

		for(uint8_t i = 0; i < BTD_NUM_SERVICES; i++)
			if(BTD_Machine.btService[i])
				BTD_Machine.btService[i]->Run(BTD_Machine.btservive_pointer_address[i]);

		if(HCD_GetURB_State(BTD_Machine.pdev, BTD_Machine.hc_num_bulk_out) == URB_STALL || HCD_GetURB_State(BTD_Machine.pdev, BTD_Machine.hc_num_bulk_out) == URB_ERROR)
			USBH_ClrFeature(BTD_Machine.pdev, BTD_Machine.phost,BTD_Machine.BTDBulkOutEp,BTD_Machine.hc_num_bulk_out);

	}

}

/************************************************************/
/*                    HCI Commands                          */
/************************************************************/

bool HCI_Command(uint8_t* data, uint16_t nbytes) {

	hci_clear_flag(HCI_FLAG_CMD_COMPLETE);
	BTD_Machine.phost->Control.setup.b.bmRequestType= bmREQ_HCI_OUT;
	BTD_Machine.phost->Control.setup.b.bRequest= 0;
	BTD_Machine.phost->Control.setup.b.wValue.w= 0;
	BTD_Machine.phost->Control.setup.b.wIndex.w= 0;
	BTD_Machine.phost->Control.setup.b.wLength.w= nbytes;

	uint8_t state = USBH_CtlReq(BTD_Machine.pdev, BTD_Machine.phost, data , nbytes );

	if (state == USBH_OK){
		return true;
	}

	return false;
}

bool hci_reset() {

	BTD_Machine.hci_event_flag = 0; // Clear all the flags
	BTD_Machine.hcibuf_cmd[0] = 0x03; // HCI OCF = 3
	BTD_Machine.hcibuf_cmd[1] = 0x03 << 2; // HCI OGF = 3
	BTD_Machine.hcibuf_cmd[2] = 0x00;

	return HCI_Command(BTD_Machine.hcibuf_cmd, 3);
}

bool hci_write_scan_enable() {

	hci_clear_flag(HCI_FLAG_INCOMING_REQUEST);

	BTD_Machine.hcibuf_cmd[0] = 0x1A; // HCI OCF = 1A
	BTD_Machine.hcibuf_cmd[1] = 0x03 << 2; // HCI OGF = 3
	BTD_Machine.hcibuf_cmd[2] = 0x01; // parameter length = 1
	if(BTD_Machine.btdName != NULL)
		BTD_Machine.hcibuf_cmd[3] = 0x03; // Inquiry Scan enabled. Page Scan enabled.
	else
		BTD_Machine.hcibuf_cmd[3] = 0x02; // Inquiry Scan disabled. Page Scan enabled.

	return HCI_Command(BTD_Machine.hcibuf_cmd, 4);

}

bool hci_write_scan_disable() {

	BTD_Machine.hcibuf_cmd[0] = 0x1A; // HCI OCF = 1A
	BTD_Machine.hcibuf_cmd[1] = 0x03 << 2; // HCI OGF = 3
	BTD_Machine.hcibuf_cmd[2] = 0x01; // parameter length = 1
	BTD_Machine.hcibuf_cmd[3] = 0x00; // Inquiry Scan disabled. Page Scan disabled.

	return HCI_Command(BTD_Machine.hcibuf_cmd, 4);
}

bool hci_read_bdaddr() {

	hci_clear_flag(HCI_FLAG_READ_BDADDR);

	BTD_Machine.hcibuf_cmd[0] = 0x09; // HCI OCF = 9
	BTD_Machine.hcibuf_cmd[1] = 0x04 << 2; // HCI OGF = 4
	BTD_Machine.hcibuf_cmd[2] = 0x00;

	return HCI_Command(BTD_Machine.hcibuf_cmd, 3);
}

bool hci_read_local_version_information() {

	hci_clear_flag(HCI_FLAG_READ_VERSION);

	BTD_Machine.hcibuf_cmd[0] = 0x01; // HCI OCF = 1
	BTD_Machine.hcibuf_cmd[1] = 0x04 << 2; // HCI OGF = 4
	BTD_Machine.hcibuf_cmd[2] = 0x00;

	return HCI_Command(BTD_Machine.hcibuf_cmd, 3);
}

bool hci_accept_connection() {

	hci_clear_flag(HCI_FLAG_CONNECT_COMPLETE);

	BTD_Machine.hcibuf_cmd[0] = 0x09; // HCI OCF = 9
	BTD_Machine.hcibuf_cmd[1] = 0x01 << 2; // HCI OGF = 1
	BTD_Machine.hcibuf_cmd[2] = 0x07; // parameter length 7
	BTD_Machine.hcibuf_cmd[3] = BTD_Machine.disc_bdaddr[0]; // 6 octet bdaddr
	BTD_Machine.hcibuf_cmd[4] = BTD_Machine.disc_bdaddr[1];
	BTD_Machine.hcibuf_cmd[5] = BTD_Machine.disc_bdaddr[2];
	BTD_Machine.hcibuf_cmd[6] = BTD_Machine.disc_bdaddr[3];
	BTD_Machine.hcibuf_cmd[7] = BTD_Machine.disc_bdaddr[4];
	BTD_Machine.hcibuf_cmd[8] = BTD_Machine.disc_bdaddr[5];
	BTD_Machine.hcibuf_cmd[9] = 0x00; // Switch role to master

	return HCI_Command(BTD_Machine.hcibuf_cmd, 10);
}

bool hci_remote_name() {

	hci_clear_flag(HCI_FLAG_REMOTE_NAME_COMPLETE);

	BTD_Machine.hcibuf_cmd[0] = 0x19; // HCI OCF = 19
	BTD_Machine.hcibuf_cmd[1] = 0x01 << 2; // HCI OGF = 1
	BTD_Machine.hcibuf_cmd[2] = 0x0A; // parameter length = 10
	BTD_Machine.hcibuf_cmd[3] = BTD_Machine.disc_bdaddr[0]; // 6 octet bdaddr
	BTD_Machine.hcibuf_cmd[4] = BTD_Machine.disc_bdaddr[1];
	BTD_Machine.hcibuf_cmd[5] = BTD_Machine.disc_bdaddr[2];
	BTD_Machine.hcibuf_cmd[6] = BTD_Machine.disc_bdaddr[3];
	BTD_Machine.hcibuf_cmd[7] = BTD_Machine.disc_bdaddr[4];
	BTD_Machine.hcibuf_cmd[8] = BTD_Machine.disc_bdaddr[5];
	BTD_Machine.hcibuf_cmd[9] = 0x01; // Page Scan Repetition Mode
	BTD_Machine.hcibuf_cmd[10] = 0x00; // Reserved
	BTD_Machine.hcibuf_cmd[11] = 0x00; // Clock offset - low byte
	BTD_Machine.hcibuf_cmd[12] = 0x00; // Clock offset - high byte

	return HCI_Command(BTD_Machine.hcibuf_cmd, 13);
}

bool hci_set_local_name(const char* name) {

	BTD_Machine.hcibuf_cmd[0] = 0x13; // HCI OCF = 13
	BTD_Machine.hcibuf_cmd[1] = 0x03 << 2; // HCI OGF = 3
	BTD_Machine.hcibuf_cmd[2] = strlen(name) + 1; // parameter length = the length of the string + end byte

	uint8_t i;
	for(i = 0; i < strlen(name); i++)
		BTD_Machine.hcibuf_cmd[i + 3] = name[i];
	BTD_Machine.hcibuf_cmd[i + 3] = 0x00; // End of string

	return HCI_Command(BTD_Machine.hcibuf_cmd, 4 + strlen(name));
}

bool hci_inquiry() {

	hci_clear_flag(HCI_FLAG_DEVICE_FOUND);

	BTD_Machine.hcibuf_cmd[0] = 0x01;
	BTD_Machine.hcibuf_cmd[1] = 0x01 << 2; // HCI OGF = 1
	BTD_Machine.hcibuf_cmd[2] = 0x05; // Parameter Total Length = 5
	BTD_Machine.hcibuf_cmd[3] = 0x33; // LAP: Genera/Unlimited Inquiry Access Code (GIAC = 0x9E8B33) - see https://www.bluetooth.org/Technical/AssignedNumbers/baseband.htm
	BTD_Machine.hcibuf_cmd[4] = 0x8B;
	BTD_Machine.hcibuf_cmd[5] = 0x9E;
	BTD_Machine.hcibuf_cmd[6] = 0x30; // Inquiry time = 61.44 sec (maximum)
	BTD_Machine.hcibuf_cmd[7] = 0x0A; // 10 number of responses

	return HCI_Command(BTD_Machine.hcibuf_cmd, 8);
}

bool hci_inquiry_cancel() {
	BTD_Machine.hcibuf_cmd[0] = 0x02;
	BTD_Machine.hcibuf_cmd[1] = 0x01 << 2; // HCI OGF = 1
	BTD_Machine.hcibuf_cmd[2] = 0x00; // Parameter Total Length = 0

	return HCI_Command(BTD_Machine.hcibuf_cmd, 3);
}

bool hci_connect_last_discovered() {
	return hci_connect(&BTD_Machine.disc_bdaddr); // Use last discovered device
}

bool hci_connect(uint8_t *bdaddr) {

	hci_clear_flag(HCI_FLAG_CONNECT_COMPLETE | HCI_FLAG_CONNECT_EVENT);

	BTD_Machine.hcibuf_cmd[0] = 0x05;
	BTD_Machine.hcibuf_cmd[1] = 0x01 << 2; // HCI OGF = 1
	BTD_Machine.hcibuf_cmd[2] = 0x0D; // parameter Total Length = 13
	BTD_Machine.hcibuf_cmd[3] = bdaddr[0]; // 6 octet bdaddr (LSB)
	BTD_Machine.hcibuf_cmd[4] = bdaddr[1];
	BTD_Machine.hcibuf_cmd[5] = bdaddr[2];
	BTD_Machine.hcibuf_cmd[6] = bdaddr[3];
	BTD_Machine.hcibuf_cmd[7] = bdaddr[4];
	BTD_Machine.hcibuf_cmd[8] = bdaddr[5];
	BTD_Machine.hcibuf_cmd[9] = 0x18; // DM1 or DH1 may be used
	BTD_Machine.hcibuf_cmd[10] = 0xCC; // DM3, DH3, DM5, DH5 may be used
	BTD_Machine.hcibuf_cmd[11] = 0x01; // Page repetition mode R1
	BTD_Machine.hcibuf_cmd[12] = 0x00; // Reserved
	BTD_Machine.hcibuf_cmd[13] = 0x00; // Clock offset
	BTD_Machine.hcibuf_cmd[14] = 0x00; // Invalid clock offset
	BTD_Machine.hcibuf_cmd[15] = 0x00; // Do not allow role switch

	return HCI_Command(BTD_Machine.hcibuf_cmd, 16);
}

bool hci_pin_code_request_reply() {

	BTD_Machine.hcibuf_cmd[0] = 0x0D; // HCI OCF = 0D
	BTD_Machine.hcibuf_cmd[1] = 0x01 << 2; // HCI OGF = 1
	BTD_Machine.hcibuf_cmd[2] = 0x17; // parameter length 23
	BTD_Machine.hcibuf_cmd[3] = BTD_Machine.disc_bdaddr[0]; // 6 octet bdaddr
	BTD_Machine.hcibuf_cmd[4] = BTD_Machine.disc_bdaddr[1];
	BTD_Machine.hcibuf_cmd[5] = BTD_Machine.disc_bdaddr[2];
	BTD_Machine.hcibuf_cmd[6] = BTD_Machine.disc_bdaddr[3];
	BTD_Machine.hcibuf_cmd[7] = BTD_Machine.disc_bdaddr[4];
	BTD_Machine.hcibuf_cmd[8] = BTD_Machine.disc_bdaddr[5];


	BTD_Machine.hcibuf_cmd[9] = strlen(BTD_Machine.btdPin); // Length of pin
	uint8_t i;
	for(i = 0; i < strlen(BTD_Machine.btdPin); i++) // The maximum size of the pin is 16
		BTD_Machine.hcibuf_cmd[i + 10] = BTD_Machine.btdPin[i];
	for(; i < 16; i++)
		BTD_Machine.hcibuf_cmd[i + 10] = 0x00; // The rest should be 0


	return HCI_Command(BTD_Machine.hcibuf_cmd, 26);
}

bool hci_pin_code_negative_request_reply() {

	BTD_Machine.hcibuf_cmd[0] = 0x0E; // HCI OCF = 0E
	BTD_Machine.hcibuf_cmd[1] = 0x01 << 2; // HCI OGF = 1
	BTD_Machine.hcibuf_cmd[2] = 0x06; // parameter length 6
	BTD_Machine.hcibuf_cmd[3] = BTD_Machine.disc_bdaddr[0]; // 6 octet bdaddr
	BTD_Machine.hcibuf_cmd[4] = BTD_Machine.disc_bdaddr[1];
	BTD_Machine.hcibuf_cmd[5] = BTD_Machine.disc_bdaddr[2];
	BTD_Machine.hcibuf_cmd[6] = BTD_Machine.disc_bdaddr[3];
	BTD_Machine.hcibuf_cmd[7] = BTD_Machine.disc_bdaddr[4];
	BTD_Machine.hcibuf_cmd[8] = BTD_Machine.disc_bdaddr[5];

	return HCI_Command(BTD_Machine.hcibuf_cmd, 9);
}

bool hci_link_key_request_negative_reply() {

	BTD_Machine.hcibuf_cmd[0] = 0x0C; // HCI OCF = 0C
	BTD_Machine.hcibuf_cmd[1] = 0x01 << 2; // HCI OGF = 1
	BTD_Machine.hcibuf_cmd[2] = 0x06; // parameter length 6
	BTD_Machine.hcibuf_cmd[3] = BTD_Machine.disc_bdaddr[0]; // 6 octet bdaddr
	BTD_Machine.hcibuf_cmd[4] = BTD_Machine.disc_bdaddr[1];
	BTD_Machine.hcibuf_cmd[5] = BTD_Machine.disc_bdaddr[2];
	BTD_Machine.hcibuf_cmd[6] = BTD_Machine.disc_bdaddr[3];
	BTD_Machine.hcibuf_cmd[7] = BTD_Machine.disc_bdaddr[4];
	BTD_Machine.hcibuf_cmd[8] = BTD_Machine.disc_bdaddr[5];

	return HCI_Command(BTD_Machine.hcibuf_cmd, 9);
}

bool hci_authentication_request() {

	BTD_Machine.hcibuf_cmd[0] = 0x11; // HCI OCF = 11
	BTD_Machine.hcibuf_cmd[1] = 0x01 << 2; // HCI OGF = 1
	BTD_Machine.hcibuf_cmd[2] = 0x02; // parameter length = 2
	BTD_Machine.hcibuf_cmd[3] = (uint8_t)(BTD_Machine.hci_handle & 0xFF); //connection handle - low byte
	BTD_Machine.hcibuf_cmd[4] = (uint8_t)((BTD_Machine.hci_handle >> 8) & 0x0F); //connection handle - high byte

	return HCI_Command(BTD_Machine.hcibuf_cmd, 5);
}

bool hci_disconnect(uint16_t handle) { // This is called by the different services

	hci_clear_flag(HCI_FLAG_DISCONNECT_COMPLETE);

	BTD_Machine.hcibuf_cmd[0] = 0x06; // HCI OCF = 6
	BTD_Machine.hcibuf_cmd[1] = 0x01 << 2; // HCI OGF = 1
	BTD_Machine.hcibuf_cmd[2] = 0x03; // parameter length = 3
	BTD_Machine.hcibuf_cmd[3] = (uint8_t)(handle & 0xFF); //connection handle - low byte
	BTD_Machine.hcibuf_cmd[4] = (uint8_t)((handle >> 8) & 0x0F); //connection handle - high byte
	BTD_Machine.hcibuf_cmd[5] = 0x13; // reason

	return HCI_Command(BTD_Machine.hcibuf_cmd, 6);
}

bool hci_write_class_of_device() { // See http://bluetooth-pentest.narod.ru/software/bluetooth_class_of_device-service_generator.html

	BTD_Machine.hcibuf_cmd[0] = 0x24; // HCI OCF = 24
	BTD_Machine.hcibuf_cmd[1] = 0x03 << 2; // HCI OGF = 3
	BTD_Machine.hcibuf_cmd[2] = 0x03; // parameter length = 3
	BTD_Machine.hcibuf_cmd[3] = 0x04; // Robot
	BTD_Machine.hcibuf_cmd[4] = 0x08; // Toy
	BTD_Machine.hcibuf_cmd[5] = 0x00;

	return HCI_Command(BTD_Machine.hcibuf_cmd, 6);
}


/*******************************************************************
 *                                                                 *
 *                        HCI ACL Data Packet                      *
 *                                                                 *
 *   buf[0]          buf[1]          buf[2]          buf[3]
 *   0       4       8    11 12      16              24            31 MSB
 *  .-+-+-+-+-+-+-+-|-+-+-+-|-+-|-+-|-+-+-+-+-+-+-+-|-+-+-+-+-+-+-+-.
 *  |      HCI Handle       |PB |BC |       Data Total Length       |   HCI ACL Data Packet
 *  .-+-+-+-+-+-+-+-|-+-+-+-|-+-|-+-|-+-+-+-+-+-+-+-|-+-+-+-+-+-+-+-.
 *
 *   buf[4]          buf[5]          buf[6]          buf[7]
 *   0               8               16                            31 MSB
 *  .-+-+-+-+-+-+-+-|-+-+-+-+-+-+-+-|-+-+-+-+-+-+-+-|-+-+-+-+-+-+-+-.
 *  |            Length             |          Channel ID           |   Basic L2CAP header
 *  .-+-+-+-+-+-+-+-|-+-+-+-+-+-+-+-|-+-+-+-+-+-+-+-|-+-+-+-+-+-+-+-.
 *
 *   buf[8]          buf[9]          buf[10]         buf[11]
 *   0               8               16                            31 MSB
 *  .-+-+-+-+-+-+-+-|-+-+-+-+-+-+-+-|-+-+-+-+-+-+-+-|-+-+-+-+-+-+-+-.
 *  |     Code      |  Identifier   |            Length             |   Control frame (C-frame)
 *  .-+-+-+-+-+-+-+-|-+-+-+-+-+-+-+-|-+-+-+-+-+-+-+-|-+-+-+-+-+-+-+-.   (signaling packet format)
 */
/************************************************************/
/*                    L2CAP Commands                        */

/************************************************************/
bool L2CAP_Command(uint16_t handle, uint8_t* data, uint8_t nbytes, uint8_t channelLow, uint8_t channelHigh) {

	uint8_t buf[8 + nbytes];
	buf[0] = (uint8_t)(handle & 0xff); // HCI handle with PB,BC flag
	buf[1] = (uint8_t)(((handle >> 8) & 0x0f) | 0x20);
	buf[2] = (uint8_t)((4 + nbytes) & 0xff); // HCI ACL total data length
	buf[3] = (uint8_t)((4 + nbytes) >> 8);
	buf[4] = (uint8_t)(nbytes & 0xff); // L2CAP header: Length
	buf[5] = (uint8_t)(nbytes >> 8);
	buf[6] = channelLow;
	buf[7] = channelHigh;

	for(uint16_t i = 0; i < nbytes; i++) // L2CAP C-frame
		buf[8 + i] = data[i];

	USBH_BulkSendData (BTD_Machine.pdev, buf, (8 + nbytes),BTD_Machine.hc_num_bulk_out);
	USB_OTG_BSP_mDelay(1);
	return true;

}

bool l2cap_connection_request(uint16_t handle, uint8_t rxid, uint8_t* scid, uint16_t psm) {

	BTD_Machine.l2capoutbuf[0] = L2CAP_CMD_CONNECTION_REQUEST; // Code
	BTD_Machine.l2capoutbuf[1] = rxid; // Identifier
	BTD_Machine.l2capoutbuf[2] = 0x04; // Length
	BTD_Machine.l2capoutbuf[3] = 0x00;
	BTD_Machine.l2capoutbuf[4] = (uint8_t)(psm & 0xff); // PSM
	BTD_Machine.l2capoutbuf[5] = (uint8_t)(psm >> 8);
	BTD_Machine.l2capoutbuf[6] = scid[0]; // Source CID
	BTD_Machine.l2capoutbuf[7] = scid[1];

	return L2CAP_Command(handle, BTD_Machine.l2capoutbuf, 8, 0x01, 0x00);
}

bool l2cap_connection_response(uint16_t handle, uint8_t rxid, uint8_t* dcid, uint8_t* scid, uint8_t result) {

	BTD_Machine.l2capoutbuf[0] = L2CAP_CMD_CONNECTION_RESPONSE; // Code
	BTD_Machine.l2capoutbuf[1] = rxid; // Identifier
	BTD_Machine.l2capoutbuf[2] = 0x08; // Length
	BTD_Machine.l2capoutbuf[3] = 0x00;
	BTD_Machine.l2capoutbuf[4] = dcid[0]; // Destination CID
	BTD_Machine.l2capoutbuf[5] = dcid[1];
	BTD_Machine.l2capoutbuf[6] = scid[0]; // Source CID
	BTD_Machine.l2capoutbuf[7] = scid[1];
	BTD_Machine.l2capoutbuf[8] = result; // Result: Pending or Success
	BTD_Machine.l2capoutbuf[9] = 0x00;
	BTD_Machine.l2capoutbuf[10] = 0x00; // No further information
	BTD_Machine.l2capoutbuf[11] = 0x00;

	return L2CAP_Command(handle, 	BTD_Machine.l2capoutbuf, 12, 0x01, 0x00);
}

bool l2cap_config_request(uint16_t handle, uint8_t rxid, uint8_t* dcid) {

	BTD_Machine.l2capoutbuf[0] = L2CAP_CMD_CONFIG_REQUEST; // Code
	BTD_Machine.l2capoutbuf[1] = rxid; // Identifier
	BTD_Machine.l2capoutbuf[2] = 0x08; // Length
	BTD_Machine.l2capoutbuf[3] = 0x00;
	BTD_Machine.l2capoutbuf[4] = dcid[0]; // Destination CID
	BTD_Machine.l2capoutbuf[5] = dcid[1];
	BTD_Machine.l2capoutbuf[6] = 0x00; // Flags
	BTD_Machine.l2capoutbuf[7] = 0x00;
	BTD_Machine.l2capoutbuf[8] = 0x01; // Config Opt: type = MTU (Maximum Transmission Unit) - Hint
	BTD_Machine.l2capoutbuf[9] = 0x02; // Config Opt: length
	BTD_Machine.l2capoutbuf[10] = 0xFF; // MTU
	BTD_Machine.l2capoutbuf[11] = 0xFF;

	return L2CAP_Command(handle, BTD_Machine.l2capoutbuf, 12, 0x01, 0x00);
}

bool l2cap_config_response(uint16_t handle, uint8_t rxid, uint8_t* scid) {

	BTD_Machine.l2capoutbuf[0] = L2CAP_CMD_CONFIG_RESPONSE; // Code
	BTD_Machine.l2capoutbuf[1] = rxid; // Identifier
	BTD_Machine.l2capoutbuf[2] = 0x0A; // Length
	BTD_Machine.l2capoutbuf[3] = 0x00;
	BTD_Machine.l2capoutbuf[4] = scid[0]; // Source CID
	BTD_Machine.l2capoutbuf[5] = scid[1];
	BTD_Machine.l2capoutbuf[6] = 0x00; // Flag
	BTD_Machine.l2capoutbuf[7] = 0x00;
	BTD_Machine.l2capoutbuf[8] = 0x00; // Result
	BTD_Machine.l2capoutbuf[9] = 0x00;
	BTD_Machine.l2capoutbuf[10] = 0x01; // Config
	BTD_Machine.l2capoutbuf[11] = 0x02;
	BTD_Machine.l2capoutbuf[12] = 0xA0;
	BTD_Machine.l2capoutbuf[13] = 0x02;

	return L2CAP_Command(handle, BTD_Machine.l2capoutbuf, 14, 0x01, 0x00);
}

bool l2cap_disconnection_request(uint16_t handle, uint8_t rxid, uint8_t* dcid, uint8_t* scid) {

	BTD_Machine.l2capoutbuf[0] = L2CAP_CMD_DISCONNECT_REQUEST; // Code
	BTD_Machine.l2capoutbuf[1] = rxid; // Identifier
	BTD_Machine.l2capoutbuf[2] = 0x04; // Length
	BTD_Machine.l2capoutbuf[3] = 0x00;
	BTD_Machine.l2capoutbuf[4] = dcid[0];
	BTD_Machine.l2capoutbuf[5] = dcid[1];
	BTD_Machine.l2capoutbuf[6] = scid[0];
	BTD_Machine.l2capoutbuf[7] = scid[1];

	return L2CAP_Command(handle, BTD_Machine.l2capoutbuf, 8, 0x01, 0x00);
}

bool l2cap_disconnection_response(uint16_t handle, uint8_t rxid, uint8_t* dcid, uint8_t* scid) {

	BTD_Machine.l2capoutbuf[0] = L2CAP_CMD_DISCONNECT_RESPONSE; // Code
	BTD_Machine.l2capoutbuf[1] = rxid; // Identifier
	BTD_Machine.l2capoutbuf[2] = 0x04; // Length
	BTD_Machine.l2capoutbuf[3] = 0x00;
	BTD_Machine.l2capoutbuf[4] = dcid[0];
	BTD_Machine.l2capoutbuf[5] = dcid[1];
	BTD_Machine.l2capoutbuf[6] = scid[0];
	BTD_Machine.l2capoutbuf[7] = scid[1];

	return L2CAP_Command(handle, BTD_Machine.l2capoutbuf, 8, 0x01, 0x00);
}

bool l2cap_information_response(uint16_t handle, uint8_t rxid, uint8_t infoTypeLow, uint8_t infoTypeHigh) {

	BTD_Machine.l2capoutbuf[0] = L2CAP_CMD_INFORMATION_RESPONSE; // Code
	BTD_Machine.l2capoutbuf[1] = rxid; // Identifier
	BTD_Machine.l2capoutbuf[2] = 0x08; // Length
	BTD_Machine.l2capoutbuf[3] = 0x00;
	BTD_Machine.l2capoutbuf[4] = infoTypeLow;
	BTD_Machine.l2capoutbuf[5] = infoTypeHigh;
	BTD_Machine.l2capoutbuf[6] = 0x00; // Result = success
	BTD_Machine.l2capoutbuf[7] = 0x00; // Result = success
	BTD_Machine.l2capoutbuf[8] = 0x00;
	BTD_Machine.l2capoutbuf[9] = 0x00;
	BTD_Machine.l2capoutbuf[10] = 0x00;
	BTD_Machine.l2capoutbuf[11] = 0x00;

	return L2CAP_Command(handle, BTD_Machine.l2capoutbuf, 12, 0x01, 0x00);
}

/* PS3 Commands - only set Bluetooth address is implemented in this library */
bool setBdaddr(uint8_t* bdaddr) {
	/* Set the internal Bluetooth address */
	uint8_t buf[8];
	buf[0] = 0x01;
	buf[1] = 0x00;

	for(uint8_t i = 0; i < 6; i++)
		buf[i + 2] = bdaddr[5 - i]; // Copy into buffer, has to be written reversed, so it is MSB first

	//        sprintf(btduartbuff,"\r\nBluetooth Address was set to: %X:%X:%X:%X:%X:%X",bdaddr[5],bdaddr[4],bdaddr[3],bdaddr[2],bdaddr[1],bdaddr[0]);
	//        UARTPrintString(USART1, btduartbuff);

	// bmRequest = Host to device (0x00) | Class (0x20) | Interface (0x01) = 0x21, bRequest = Set Report (0x09), Report ID (0xF5), Report Type (Feature 0x03), interface (0x00), datalength, datalength, data
	//        pUsb->ctrlReq(bAddress, epInfo[BTD_CONTROL_PIPE].epAddr, bmREQ_HID_OUT, HID_REQUEST_SET_REPORT, 0xF5, 0x03, 0x00, 8, 8, buf, NULL);

	//        if(BTD_Machine.cmd_state == 0){
	BTD_Machine.phost->Control.setup.b.bmRequestType= bmREQ_HID_OUT;
	BTD_Machine.phost->Control.setup.b.bRequest= HID_REQUEST_SET_REPORT;
	BTD_Machine.phost->Control.setup.b.wValue.bw.lsb= 0xF5;
	BTD_Machine.phost->Control.setup.b.wValue.bw.msb= 0x03;
	BTD_Machine.phost->Control.setup.b.wIndex.w= 0;
	BTD_Machine.phost->Control.setup.b.wLength.w= 8;
	//			BTD_Machine.cmd_state = 1;
	//		}

	uint8_t state = USBH_CtlReq(BTD_Machine.pdev, BTD_Machine.phost, &buf , 8 );

	//		if (state == USBH_OK) {
	//			BTD_Machine.cmd_state = 0;
	//			return true;
	//		} else if (state == USBH_FAIL) {
	//			BTD_Machine.cmd_state = 0;
	//		}

	//		return false;

	sprintf(btduartbuff,"\r\n%d", state);
	UARTPrintString(USART1, btduartbuff);

	//		return USBH_CtlReq(BTD_Machine.pdev, BTD_Machine.phost, buf , 8 );
	return state;
}


