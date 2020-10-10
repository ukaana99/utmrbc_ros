

#ifndef STM32_USB_HOST_LIBRARY_CLASS_BTD_INC_PS4BT_H_
#define STM32_USB_HOST_LIBRARY_CLASS_BTD_INC_PS4BT_H_

#include "usbh_btd_core.h"
#include "controllerEnums.h"

#define 	USB_HID_BOOT_PROTOCOL       0x00

#define 	ps4_low_Lx					115.0
#define 	ps4_high_Lx					140.0

#define 	ps4_low_Ly					115.0
#define 	ps4_high_Ly					140.0

#define 	ps4_low_Rx					115.0
#define 	ps4_high_Rx					140.0

#define 	ps4_low_Ry					115.0
#define 	ps4_high_Ry					140.0

#define 	ps4_leftjoy_x				ps4Data.hatValue[0]
#define 	ps4_leftjoy_y				ps4Data.hatValue[1]
#define 	ps4_rightjoy_x 				ps4Data.hatValue[2]
#define 	ps4_rightjoy_y				ps4Data.hatValue[3]
#define 	ps4_an_L2					ps4Data.trigger[1]
#define 	ps4_an_R2					ps4Data.trigger[0]

#define 	ps4_joyL_up					(float)PS4BT->ps4_leftjoy_y < ps4_low_Ly
#define 	ps4_joyL_down				(float)PS4BT->ps4_leftjoy_y > ps4_high_Ly
#define 	ps4_joyL_left				(float)PS4BT->ps4_leftjoy_x < ps4_low_Lx
#define 	ps4_joyL_right				(float)PS4BT->ps4_leftjoy_x > ps4_high_Lx

#define 	ps4_joyR_up					(float)PS4BT->ps4_rightjoy_y < ps4_low_Ry
#define 	ps4_joyR_down				(float)PS4BT->ps4_rightjoy_y > ps4_high_Ry
#define 	ps4_joyR_left				(float)PS4BT->ps4_rightjoy_x < ps4_low_Rx
#define 	ps4_joyR_right				(float)PS4BT->ps4_rightjoy_x > ps4_high_Rx

/** Buttons on the controller */
extern const uint8_t PS4_BUTTONS[];

typedef union {
	struct {
		uint8_t dpad : 4;
		uint8_t square : 1;
		uint8_t cross : 1;
		uint8_t circle : 1;
		uint8_t triangle : 1;

		uint8_t l1 : 1;
		uint8_t r1 : 1;
		uint8_t l2 : 1;
		uint8_t r2 : 1;
		uint8_t share : 1;
		uint8_t options : 1;
		uint8_t l3 : 1;
		uint8_t r3 : 1;

		uint8_t ps : 1;
		uint8_t touchpad : 1;
		uint8_t reportCounter : 6;
	} __attribute__((packed));
	uint32_t val:24 ;
} __attribute__((packed))PS4Buttons ;

typedef struct {
	uint8_t dummy; // I can not figure out what this data is for, it seems to change randomly, maybe a timestamp?
	struct {
		uint8_t counter : 7; // Increments every time a finger is touching the touchpad
		uint8_t touching : 1; // The top bit is cleared if the finger is touching the touchpad
		uint16_t x : 12;
		uint16_t y : 12;
	} __attribute__((packed)) finger[2]; // 0 = first finger, 1 = second finger
} __attribute__((packed)) touchpadXY ;

typedef struct  {
	uint8_t battery : 4;
	uint8_t usb : 1;
	uint8_t audio : 1;
	uint8_t mic : 1;
	uint8_t unknown : 1; // Extension port?
} __attribute__((packed)) PS4Status;

typedef struct {
	/* Button and joystick values */
	uint8_t hatValue[4];
	PS4Buttons btn;
	uint8_t trigger[2];

	/* Gyro and accelerometer values */
	uint8_t dummy[3]; // First two looks random, while the third one might be some kind of status - it increments once in a while
	int16_t gyroY, gyroZ, gyroX;
	int16_t accX, accZ, accY;

	uint8_t dummy2[5];
	PS4Status status;
	uint8_t dummy3[3];

	/* The rest is data for the touchpad */
	touchpadXY xy[3]; // It looks like it sends out three coordinates each time, this might be because the microcontroller inside the PS4 controller is much faster than the Bluetooth connection.
					  // The last data is read from the last position in the array while the oldest measurement is from the first position.
					  // The first position will also keep it's value after the finger is released, while the other two will set them to zero.
					  // Note that if you read fast enough from the device, then only the first one will contain any data.

	// The last three bytes are always: 0x00, 0x80, 0x00
} __attribute__((packed)) PS4Data;

typedef struct  {
	uint8_t bigRumble, smallRumble; // Rumble
	uint8_t r, g, b; // RGB
	uint8_t flashOn, flashOff; // Time to flash bright/dark (255 = 2.5 seconds)
	bool reportChanged; // The data is send when data is received from the controller
} __attribute__((packed))PS4Output ;

typedef struct {

	BluetoothService PS4BTservice;
	uint8_t PS4BTserviceID;

	/** L2CAP source CID for HID_Control */
	uint8_t control_scid[2];

	/** L2CAP source CID for HID_Interrupt */
	uint8_t interrupt_scid[2];

	bool activeConnection; // Used to indicate if it already has established a connection

	/* Variables used for L2CAP communication */
	uint8_t control_dcid[2]; // L2CAP device CID for HID_Control - Always 0x0070
	uint8_t interrupt_dcid[2]; // L2CAP device CID for HID_Interrupt - Always 0x0071
	uint8_t l2cap_state;

	/** True if a device is connected */
	bool connected;

	PS4Data ps4Data;
	PS4Buttons oldButtonState, buttonClickState;
	PS4Output ps4Output;
	uint8_t oldDpad;

	float joyR_y;
	float joyR_x;
	float joyL_y;
	float joyL_x;
	float joyR_2;
	float joyL_2;

}PS4BT_t;

/****************************************************************************************************************************************/
/*BluetoothService implementation */

void PS4BTReset(uint32_t pointer_address);
void PS4BTdisconnect(uint32_t pointer_address);
void PS4BTACLData(uint8_t* l2capinbuf, uint32_t pointer_address);
void PS4BTRun(uint32_t pointer_address);

/****************************************************************************************************************************************/

void PS4BTInit(PS4BT_t *PS4BT, bool pair);
void PS4BTL2CAP_task(PS4BT_t *PS4BT);
void setProtocol(PS4BT_t *PS4BT);
void setLeds(PS4BT_t *PS4BT, uint8_t data);
void ParseBTHIDData(PS4BT_t *PS4BT, uint8_t len, uint8_t *buf);
bool checkDpad(PS4BT_t *PS4BT, ButtonEnum b);
void OnInit(PS4BT_t *PS4BT);
void sendOutputReport(PS4BT_t *PS4BT, PS4Output *output);
void PS4_enable_sixaxis(PS4BT_t *PS4BT);

/****************************************************************************************************************************************/

/** @name PS4 Controller functions */
/**
* getButtonPress(ButtonEnum b) will return true as long as the button is held down.
*
* While getButtonClick(ButtonEnum b) will only return it once.
*
* So you instance if you need to increase a variable once you would use getButtonClick(ButtonEnum b),
* but if you need to drive a robot forward you would use getButtonPress(ButtonEnum b).
* @param  b          ::ButtonEnum to read.
* @return            getButtonPress(ButtonEnum b) will return a true as long as a button is held down, while getButtonClick(ButtonEnum b) will return true once for each button press.
*/
bool PS4BTgetButtonPress(PS4BT_t *PS4BT, ButtonEnum b);
bool PS4BTgetButtonClick(PS4BT_t *PS4BT, ButtonEnum b);
/**@}*/
/** @name PS4 Controller functions */
/**
* Used to get the analog value from button presses.
* @param  b The ::ButtonEnum to read.
* The supported buttons are:
* ::UP, ::RIGHT, ::DOWN, ::LEFT, ::L1, ::L2, ::R1, ::R2,
* ::TRIANGLE, ::CIRCLE, ::CROSS, ::SQUARE, and ::T.
* @return   Analog value in the range of 0-255.
*/
uint8_t PS4BTgetAnalogButton(PS4BT_t *PS4BT, ButtonEnum b) ;

/**
* Used to read the analog joystick.
* @param  a ::LeftHatX, ::LeftHatY, ::RightHatX, and ::RightHatY.
* @return   Return the analog value in the range of 0-255.
*/
uint8_t PS4BTgetAnalogHat(PS4BT_t *PS4BT, AnalogHatEnum a) ;

/**
* Get the x-coordinate of the touchpad. Position 0 is in the top left.
* @param  finger 0 = first finger, 1 = second finger. If omitted, then 0 will be used.
* @param  xyId   The controller sends out three packets with the same structure.
*                The third one will contain the last measure, but if you read from the controller then there is only be data in the first one.
*                For that reason it will be set to 0 if the argument is omitted.
* @return        Returns the x-coordinate of the finger.
*/
uint16_t PS4BTgetX(PS4BT_t *PS4BT, uint8_t finger, uint8_t xyId);

/**
* Get the y-coordinate of the touchpad. Position 0 is in the top left.
* @param  finger 0 = first finger, 1 = second finger. If omitted, then 0 will be used.
* @param  xyId   The controller sends out three packets with the same structure.
*                The third one will contain the last measure, but if you read from the controller then there is only be data in the first one.
*                For that reason it will be set to 0 if the argument is omitted.
* @return        Returns the y-coordinate of the finger.
*/
uint16_t PS4BTgetY(PS4BT_t *PS4BT, uint8_t finger, uint8_t xyId);

/**
* Returns whenever the user is toucing the touchpad.
* @param  finger 0 = first finger, 1 = second finger. If omitted, then 0 will be used.
* @param  xyId   The controller sends out three packets with the same structure.
*                The third one will contain the last measure, but if you read from the controller then there is only be data in the first one.
*                For that reason it will be set to 0 if the argument is omitted.
* @return        Returns true if the specific finger is touching the touchpad.
*/
bool PS4BTisTouching(PS4BT_t *PS4BT, uint8_t finger, uint8_t xyId);

/**
* This counter increments every time a finger touches the touchpad.
* @param  finger 0 = first finger, 1 = second finger. If omitted, then 0 will be used.
* @param  xyId   The controller sends out three packets with the same structure.
*                The third one will contain the last measure, but if you read from the controller then there is only be data in the first one.
*                For that reason it will be set to 0 if the argument is omitted.
* @return        Return the value of the counter, note that it is only a 7-bit value.
*/
uint8_t PS4BTgetTouchCounter(PS4BT_t *PS4BT, uint8_t finger, uint8_t xyId);

/**
* Get the angle of the controller calculated using the accelerometer.
* @param  a Either ::Pitch or ::Roll.
* @return   Return the angle in the range of 0-360.
*/
float PS4BTgetAngle(PS4BT_t *PS4BT, AngleEnum a);

/**
* Used to get the raw values from the 3-axis gyroscope and 3-axis accelerometer inside the PS4 controller.
* @param  s The sensor to read.
* @return   Returns the raw sensor reading.
*/
int16_t PS4BTgetSensor(PS4BT_t *PS4BT, SensorEnum s) ;

/**
* Return the battery level of the PS4 controller.
* @return The battery level in the range 0-15.
*/
uint8_t PS4BTgetBatteryLevel(PS4BT_t *PS4BT);

/**
* Use this to check if an USB cable is connected to the PS4 controller.
* @return Returns true if an USB cable is connected.
*/
bool PS4BTgetUsbStatus(PS4BT_t *PS4BT);

/**
* Use this to check if an audio jack cable is connected to the PS4 controller.
* @return Returns true if an audio jack cable is connected.
*/
bool PS4BTgetAudioStatus(PS4BT_t *PS4BT);

/**
* Use this to check if a microphone is connected to the PS4 controller.
* @return Returns true if a microphone is connected.
*/
bool PS4BTgetMicStatus(PS4BT_t *PS4BT);

/** Turn both rumble and the LEDs off. */
void PS4BTsetAllOff(PS4BT_t *PS4BT);

/** Set rumble off. */
void PS4BTsetRumbleOff(PS4BT_t *PS4BT);

/**
* Turn on rumble.
* @param mode Either ::RumbleHigh or ::RumbleLow.
*/
void PS4BTsetRumbleMode(PS4BT_t *PS4BT, RumbleEnum mode);

/**
* Turn on rumble.
* @param bigRumble   Value for big motor.
* @param smallRumble Value for small motor.
*/
void PS4BTsetRumbleOn(PS4BT_t *PS4BT, uint8_t bigRumble, uint8_t smallRumble);

/** Turn all LEDs off. */
void PS4BTsetLedOff(PS4BT_t *PS4BT);

/**
* Use this to set the color using RGB values.
* @param r,g,b RGB value.
*/
void PS4BTsetLed(PS4BT_t *PS4BT, uint8_t r, uint8_t g, uint8_t b);

/**
* Use this to set the color using the predefined colors in ::ColorsEnum.
* @param color The desired color.
*/
void PS4BTsetLedColour(PS4BT_t *PS4BT, ColorsEnum color);

/**
* Set the LEDs flash time.
* @param flashOn  Time to flash bright (255 = 2.5 seconds).
* @param flashOff Time to flash dark (255 = 2.5 seconds).
*/
void PS4BTsetLedFlash(PS4BT_t *PS4BT, uint8_t flashOn, uint8_t flashOff);
/**@}*/



#endif /* STM32_USB_HOST_LIBRARY_CLASS_BTD_INC_PS4BT_H_ */
