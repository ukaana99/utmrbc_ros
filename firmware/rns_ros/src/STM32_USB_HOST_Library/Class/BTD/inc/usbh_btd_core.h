
/***************************************************************************************
 * Title   : Bluetooth Dongle
 * Author  : Kai Sheng
 * Version : 3.00
 * Date    : 21 OCT 2018
 * *************************************************************************************
 * Descriptions:
 *		This library is use to interface with bluetooth dongle via USB.
 *
 * Version History:
 * 		v3.00 Add SPP into this Library.
 * 		v2.00 Make this library support PS4 controller.
 * 		v1.00 Library was converted from USB_Host_Shield_Library_2_0-1.3.2 by
 *			  Oleg Mazurov (Circuits@Home). This version of library supports only
 *			  PS3 controller.
 *
 * Bugs:
 *
 ****************************************************************************************/


/* Define to prevent recursive  ----------------------------------------------*/
#ifndef __USBH_BTD_CORE_H_
#define __USBH_BTD_CORE_H_

/* Includes ------------------------------------------------------------------*/
#include "STM32_USB_HOST_Library/Core/inc/usbh_core.h"
#include "STM32_USB_HOST_Library/Core/inc/usbh_stdreq.h"
#include "STM32_USB_OTG_Driver/inc/usb_bsp.h"
#include "STM32_USB_HOST_Library/Core/inc/usbh_ioreq.h"
#include "STM32_USB_HOST_Library/Core/inc/usbh_hcs.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "BIOS/bios.h"


//here to change USART for PRINTING
#define PRINTING_USART			UART5

#define PI				3.14159265358979f
#define RAD_TO_DEG		180/PI

//PID and VID of the Sony PS3 devices
#define PS3_VID                 0x054C  // Sony Corporation
#define PS3_PID                 0x0268  // PS3 Controller DualShock 3
#define PS3NAVIGATION_PID       0x042F  // Navigation controller
#define PS3MOVE_PID             0x03D5  // Motion controller

// These dongles do not present themselves correctly, so we have to check for them manually
#define IOGEAR_GBU521_VID       0x0A5C
#define IOGEAR_GBU521_PID       0x21E8
#define BELKIN_F8T065BF_VID     0x050D
#define BELKIN_F8T065BF_PID     0x065A

/* Bluetooth dongle data taken from descriptors */
#define BULK_MAXPKTSIZE         64 // Max size for ACL data

// Used in control endpoint header for HCI Commands
#define bmREQ_HCI_OUT USB_H2D|USB_REQ_TYPE_CLASS|USB_REQ_RECIPIENT_DEVICE
#define bmREQ_HID_OUT USB_H2D|USB_REQ_TYPE_CLASS|USB_REQ_RECIPIENT_INTERFACE
#define HID_REQUEST_SET_REPORT      0x09

/* Bluetooth HCI states for hci_task() */
#define HCI_INIT_STATE                  0
#define HCI_RESET_STATE                 1
#define HCI_CLASS_STATE                 2
#define HCI_BDADDR_STATE                3
#define HCI_LOCAL_VERSION_STATE         4
#define HCI_SET_NAME_STATE              5
#define HCI_CHECK_DEVICE_SERVICE        6

#define HCI_INQUIRY_STATE               7 // These three states are only used if it should pair and connect to a device
#define HCI_CONNECT_DEVICE_STATE        8
#define HCI_CONNECTED_DEVICE_STATE      9

#define HCI_SCANNING_STATE              10
#define HCI_CONNECT_IN_STATE            11
#define HCI_REMOTE_NAME_STATE           12
#define HCI_CONNECTED_STATE             13
#define HCI_DISABLE_SCAN_STATE          14
#define HCI_DONE_STATE                  15
#define HCI_DISCONNECT_STATE            16

/* HCI event flags*/
#define HCI_FLAG_CMD_COMPLETE           (1UL << 0)
#define HCI_FLAG_CONNECT_COMPLETE       (1UL << 1)
#define HCI_FLAG_DISCONNECT_COMPLETE    (1UL << 2)
#define HCI_FLAG_REMOTE_NAME_COMPLETE   (1UL << 3)
#define HCI_FLAG_INCOMING_REQUEST       (1UL << 4)
#define HCI_FLAG_READ_BDADDR            (1UL << 5)
#define HCI_FLAG_READ_VERSION           (1UL << 6)
#define HCI_FLAG_DEVICE_FOUND           (1UL << 7)
#define HCI_FLAG_CONNECT_EVENT          (1UL << 8)

/* Macros for HCI event flag tests */
#define hci_check_flag(flag) (BTD_Machine.hci_event_flag & (flag))
#define hci_set_flag(flag) (BTD_Machine.hci_event_flag |= (flag))
#define hci_clear_flag(flag) (BTD_Machine.hci_event_flag &= ~(flag))

/* HCI Events managed */
#define EV_INQUIRY_COMPLETE                             0x01
#define EV_INQUIRY_RESULT                               0x02
#define EV_CONNECT_COMPLETE                             0x03
#define EV_INCOMING_CONNECT                             0x04
#define EV_DISCONNECT_COMPLETE                          0x05
#define EV_AUTHENTICATION_COMPLETE                      0x06
#define EV_REMOTE_NAME_COMPLETE                         0x07
#define EV_ENCRYPTION_CHANGE                            0x08
#define EV_CHANGE_CONNECTION_LINK                       0x09
#define EV_ROLE_CHANGED                                 0x12
#define EV_NUM_COMPLETE_PKT                             0x13
#define EV_PIN_CODE_REQUEST                             0x16
#define EV_LINK_KEY_REQUEST                             0x17
#define EV_LINK_KEY_NOTIFICATION                        0x18
#define EV_DATA_BUFFER_OVERFLOW                         0x1A
#define EV_MAX_SLOTS_CHANGE                             0x1B
#define EV_READ_REMOTE_VERSION_INFORMATION_COMPLETE     0x0C
#define EV_QOS_SETUP_COMPLETE                           0x0D
#define EV_COMMAND_COMPLETE                             0x0E
#define EV_COMMAND_STATUS                               0x0F
#define EV_LOOPBACK_COMMAND                             0x19
#define EV_PAGE_SCAN_REP_MODE                           0x20

/* Bluetooth states for the different Bluetooth drivers */
#define L2CAP_WAIT                      0
#define L2CAP_DONE                      1

/* Used for HID Control channel */
#define L2CAP_CONTROL_CONNECT_REQUEST   2
#define L2CAP_CONTROL_CONFIG_REQUEST    3
#define L2CAP_CONTROL_SUCCESS           4
#define L2CAP_CONTROL_DISCONNECT        5

/* Used for HID Interrupt channel */
#define L2CAP_INTERRUPT_SETUP           6
#define L2CAP_INTERRUPT_CONNECT_REQUEST 7
#define L2CAP_INTERRUPT_CONFIG_REQUEST  8
#define L2CAP_INTERRUPT_DISCONNECT      9

/* Used for SDP channel */
#define L2CAP_SDP_WAIT                  10
#define L2CAP_SDP_SUCCESS               11

/* Used for RFCOMM channel */
#define L2CAP_RFCOMM_WAIT               12
#define L2CAP_RFCOMM_SUCCESS            13

#define L2CAP_DISCONNECT_RESPONSE       14 // Used for both SDP and RFCOMM channel

/* Bluetooth states used by some drivers */
#define TURN_ON_LED                     17
#define PS3_ENABLE_SIXAXIS              18
#define WII_CHECK_MOTION_PLUS_STATE     19
#define WII_CHECK_EXTENSION_STATE       20
#define WII_INIT_MOTION_PLUS_STATE      21

/* L2CAP event flags for HID Control channel */
#define L2CAP_FLAG_CONNECTION_CONTROL_REQUEST           (1UL << 0)
#define L2CAP_FLAG_CONFIG_CONTROL_SUCCESS               (1UL << 1)
#define L2CAP_FLAG_CONTROL_CONNECTED                    (1UL << 2)
#define L2CAP_FLAG_DISCONNECT_CONTROL_RESPONSE          (1UL << 3)

/* L2CAP event flags for HID Interrupt channel */
#define L2CAP_FLAG_CONNECTION_INTERRUPT_REQUEST         (1UL << 4)
#define L2CAP_FLAG_CONFIG_INTERRUPT_SUCCESS             (1UL << 5)
#define L2CAP_FLAG_INTERRUPT_CONNECTED                  (1UL << 6)
#define L2CAP_FLAG_DISCONNECT_INTERRUPT_RESPONSE        (1UL << 7)

/* L2CAP event flags for SDP channel */
#define L2CAP_FLAG_CONNECTION_SDP_REQUEST               (1UL << 8)
#define L2CAP_FLAG_CONFIG_SDP_SUCCESS                   (1UL << 9)
#define L2CAP_FLAG_DISCONNECT_SDP_REQUEST               (1UL << 10)

/* L2CAP event flags for RFCOMM channel */
#define L2CAP_FLAG_CONNECTION_RFCOMM_REQUEST            (1UL << 11)
#define L2CAP_FLAG_CONFIG_RFCOMM_SUCCESS                (1UL << 12)
#define L2CAP_FLAG_DISCONNECT_RFCOMM_REQUEST            (1UL << 13)

#define L2CAP_FLAG_DISCONNECT_RESPONSE                  (1UL << 14)

/* L2CAP signaling commands */
#define L2CAP_CMD_COMMAND_REJECT        0x01
#define L2CAP_CMD_CONNECTION_REQUEST    0x02
#define L2CAP_CMD_CONNECTION_RESPONSE   0x03
#define L2CAP_CMD_CONFIG_REQUEST        0x04
#define L2CAP_CMD_CONFIG_RESPONSE       0x05
#define L2CAP_CMD_DISCONNECT_REQUEST    0x06
#define L2CAP_CMD_DISCONNECT_RESPONSE   0x07
#define L2CAP_CMD_INFORMATION_REQUEST   0x0A
#define L2CAP_CMD_INFORMATION_RESPONSE  0x0B

// Used For Connection Response - Remember to Include High Byte
#define PENDING     0x01
#define SUCCESSFUL  0x00

/* Bluetooth L2CAP PSM - see http://www.bluetooth.org/Technical/AssignedNumbers/logical_link.htm */
#define SDP_PSM         0x01 // Service Discovery Protocol PSM Value
#define RFCOMM_PSM      0x03 // RFCOMM PSM Value
#define HID_CTRL_PSM    0x11 // HID_Control PSM Value
#define HID_INTR_PSM    0x13 // HID_Interrupt PSM Value

// Used to determine if it is a Bluetooth dongle
#define WI_SUBCLASS_RF      0x01 // RF Controller
#define WI_PROTOCOL_BT      0x01 // Bluetooth Programming Interface

#define BTD_MAX_ENDPOINTS   4
#define BTD_NUM_SERVICES    4 // Max number of Bluetooth services - if you need more than 4 simply increase this number

#define PAIR    1

char btduartbuff[200];
extern USBH_Class_cb_TypeDef  USBH_BTD_cb;

/***************************************************Bluetooth Service**********************************************************/

typedef struct BTD_Service
{
	void (*ACLData)(uint8_t* ACLData, uint32_t pointer_address);	// Used to pass acldata to the services
	void (*Run)(uint32_t pointer_address);					// Used to run the different state machines
	void (*Reset)(uint32_t pointer_address); 				// Used to reset the services
	void (*disconnect)(uint32_t pointer_address);			// Used to disconnect both the L2CAP Channel and the HCI Connection

	/**
	  * Called when a device is successfully initialized.
	  */
	void (*onInit)(void);

	 /** The HCI Handle for the connection. */
	uint16_t hci_handle;

	/** L2CAP flags of received Bluetooth events. */
	uint32_t l2cap_event_flag;

	/** Identifier for L2CAP commands. */
	uint8_t identifier;

} BluetoothService;

bool checkHciHandle(uint8_t *buf, uint16_t handle);

/*******************************************************************************************************************************/

/***********************************************************BTD*****************************************************************/

typedef struct _BTD_Process
{
	USB_OTG_CORE_HANDLE 	*pdev;
	USBH_HOST           	*phost;

	//Endpoints
	uint8_t					hc_num_it_in;
	uint8_t              	hc_num_bulk_in;
	uint8_t              	hc_num_bulk_out;
	uint8_t              	BTDIntInEp;
	uint8_t             	BTDBulkInEp;
	uint8_t              	BTDBulkOutEp;
	uint16_t             	BTDIntInEpSize;
	uint16_t            	BTDBulkInEpSize;
	uint16_t             	BTDBulkOutEpSize;

	uint8_t					int_state;
	uint8_t					acli_enable;
	uint32_t				timer;
	uint8_t					aclo_state;
	uint8_t					acli_state;

	/** Use this to see if it is waiting for a incoming connection. */
	bool waitingForConnection;
	/** This is used by the service to know when to store the device information. */
	bool l2capConnectionClaimed;
	/** This is used by the SPP library to claim the current SDP incoming request. */
	bool sdpConnectionClaimed;
	/** This is used by the SPP library to claim the current RFCOMM incoming request. */
	bool rfcommConnectionClaimed;

	/** The name you wish to make the dongle show up as. It is set automatically by the SPP library. */
	const char* btdName;
	/** The pin you wish to make the dongle use for authentication. It is set automatically by the SPP and BTHID library. */
	const char* btdPin;

	/** The bluetooth dongles Bluetooth address. */
	uint8_t my_bdaddr[6];
	/** HCI handle for the last connection. */
	uint16_t hci_handle;
	/** Last incoming devices Bluetooth address. */
	uint8_t disc_bdaddr[6];
	/** First 30 chars of last remote name. */
	char remote_name[30];
	/**
	* The supported HCI Version read from the Bluetooth dongle.
	* Used by the PS3BT library to check the HCI Version of the Bluetooth dongle,
	* it should be at least 3 to work properly with the library.
	*/
	uint8_t hci_version;

	/** Used to only send the ACL data to the HID device. */
	bool connectToHIDDevice;
	/** True if a HID device is connecting. */
	bool incomingHIDDevice;
	/** True when it should pair with a device like a mouse or keyboard. */
	bool pairWithHIDDevice;

	BluetoothService *btService[BTD_NUM_SERVICES];
	uint32_t btservive_pointer_address[BTD_NUM_SERVICES];

	bool checkRemoteName; // Used to check remote device's name before connecting.
	bool incomingPS4; // True if a PS4 controller is connecting
	uint8_t classOfDevice[3]; // Class of device of last device

	/* Variables used by high level HCI task */
	uint8_t hci_state; // Current state of Bluetooth HCI connection
	uint32_t hci_counter; // Counter used for Bluetooth HCI reset loops
	uint32_t hci_num_reset_loops; // This value indicate how many times it should read before trying to reset
	uint16_t hci_event_flag; // HCI flags of received Bluetooth events
	uint32_t inquiry_counter;

	uint8_t hcibuf[64]; // General purpose buffer for HCI data
	uint8_t hcibuf_cmd[64]; //HCI command buffer

	uint8_t l2capinbuf[BULK_MAXPKTSIZE]; // General purpose buffer for L2CAP in data
	uint8_t l2capoutbuf[14]; // General purpose buffer for L2CAP out data

	uint32_t qNextPollTime;

}BTD_Machine_TypeDef;


extern BTD_Machine_TypeDef    BTD_Machine;
extern uint8_t BTDErrorCount;

void InitializeBTD(); // Set all variables, endpoint structs etc. to default values
void disconnect();
/**
  * Register Bluetooth dongle members/services.
  * @param  pService Pointer to BluetoothService class instance.
  * @return          The service ID on success or -1 on fail.
  */
int8_t registerBluetoothService(uint32_t pointer_address,BluetoothService *pService);


/** Call this function to pair with a HID device */
void pairWithHID();

/****************************************************************************************************************************************/
/** @name HCI Commands */
/**
 * Used to send a HCI Command.
 * @param data   Data to send.
 * @param nbytes Number of bytes to send.
 */
bool HCI_Command(uint8_t* data, uint16_t nbytes);
/** Reset the Bluetooth dongle. */
bool hci_reset();
/** Read the Bluetooth address of the dongle. */
bool hci_read_bdaddr();
/** Read the HCI Version of the Bluetooth dongle. */
bool hci_read_local_version_information();
/**
 * Set the local name of the Bluetooth dongle.
 * @param name Desired name.
 */
bool hci_set_local_name(const char* name);
/** Enable visibility to other Bluetooth devices. */
bool hci_write_scan_enable();
/** Disable visibility to other Bluetooth devices. */
bool hci_write_scan_disable();
/** Read the remote devices name. */
bool hci_remote_name();
/** Accept the connection with the Bluetooth device. */
bool hci_accept_connection();
/**
 * Disconnect the HCI connection.
 * @param handle The HCI Handle for the connection.
 */
bool hci_disconnect(uint16_t handle);
/**
 * Respond with the pin for the connection.
 * The pin is automatically set for the Wii library,
 * but can be customized for the SPP library.
 */
bool hci_pin_code_request_reply();
/** Respons when no pin was set. */
bool hci_pin_code_negative_request_reply();
/**
 * Command is used to reply to a Link Key Request event from the BR/EDR Controller
 * if the Host does not have a stored Link Key for the connection.
 */
bool hci_link_key_request_negative_reply();
/** Used to try to authenticate with the remote device. */
bool hci_authentication_request();
/** Start a HCI inquiry. */
bool hci_inquiry();
/** Cancel a HCI inquiry. */
bool hci_inquiry_cancel();
/** Connect to last device communicated with. */
bool hci_connect_last_discovered();
/**
 * Connect to device.
 * @param bdaddr Bluetooth address of the device.
 */
bool hci_connect(uint8_t *bdaddr);
/** Used to a set the class of the device. */
bool hci_write_class_of_device();
/**@}*/

/****************************************************************************************************************************************/

/** @name L2CAP Commands */
/**
 * Used to send L2CAP Commands.
 * @param handle      HCI Handle.
 * @param data        Data to send.
 * @param nbytes      Number of bytes to send.
 * @param channelLow,channelHigh  Low and high byte of channel to send to.
 * If argument is omitted then the Standard L2CAP header: Channel ID (0x01) for ACL-U will be used.
 */
bool L2CAP_Command(uint16_t handle, uint8_t* data, uint8_t nbytes, uint8_t channelLow , uint8_t channelHigh);
/**
 * L2CAP Connection Request.
 * @param handle HCI handle.
 * @param rxid   Identifier.
 * @param scid   Source Channel Identifier.
 * @param psm    Protocol/Service Multiplexer - see: https://www.bluetooth.org/Technical/AssignedNumbers/logical_link.htm.
 */
bool l2cap_connection_request(uint16_t handle, uint8_t rxid, uint8_t* scid, uint16_t psm);
/**
 * L2CAP Connection Response.
 * @param handle HCI handle.
 * @param rxid   Identifier.
 * @param dcid   Destination Channel Identifier.
 * @param scid   Source Channel Identifier.
 * @param result Result - First send ::PENDING and then ::SUCCESSFUL.
 */
bool l2cap_connection_response(uint16_t handle, uint8_t rxid, uint8_t* dcid, uint8_t* scid, uint8_t result);
/**
 * L2CAP Config Request.
 * @param handle HCI Handle.
 * @param rxid   Identifier.
 * @param dcid   Destination Channel Identifier.
 */
bool l2cap_config_request(uint16_t handle, uint8_t rxid, uint8_t* dcid);
/**
 * L2CAP Config Response.
 * @param handle HCI Handle.
 * @param rxid   Identifier.
 * @param scid   Source Channel Identifier.
 */
bool l2cap_config_response(uint16_t handle, uint8_t rxid, uint8_t* scid);
/**
 * L2CAP Disconnection Request.
 * @param handle HCI Handle.
 * @param rxid   Identifier.
 * @param dcid   Device Channel Identifier.
 * @param scid   Source Channel Identifier.
 */
bool l2cap_disconnection_request(uint16_t handle, uint8_t rxid, uint8_t* dcid, uint8_t* scid);
/**
 * L2CAP Disconnection Response.
 * @param handle HCI Handle.
 * @param rxid   Identifier.
 * @param dcid   Device Channel Identifier.
 * @param scid   Source Channel Identifier.
 */
bool l2cap_disconnection_response(uint16_t handle, uint8_t rxid, uint8_t* dcid, uint8_t* scid);
/**
 * L2CAP Information Response.
 * @param handle       HCI Handle.
 * @param rxid         Identifier.
 * @param infoTypeLow,infoTypeHigh  Infotype.
 */
bool l2cap_information_response(uint16_t handle, uint8_t rxid, uint8_t infoTypeLow, uint8_t infoTypeHigh);
/**@}*/

/****************************************************************************************************************************************/

/* State machines */
void HCI_event_task(); // Poll the HCI event pipe
void HCI_task(); // HCI state machine
void ACL_event_task(); // ACL input pipe

/* Used to set the Bluetooth Address internally to the PS3 Controllers */
bool setBdaddr(uint8_t* BDADDR);

/***************************************************************************************************************************************/

#endif /* __USBH_BTD_CORE_H_ */
