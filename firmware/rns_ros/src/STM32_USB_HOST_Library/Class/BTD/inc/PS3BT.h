
#ifndef STM32_USB_HOST_LIBRARY_CLASS_BTD_INC_PS3BT_H_
#define STM32_USB_HOST_LIBRARY_CLASS_BTD_INC_PS3BT_H_

#include "usbh_btd_core.h"
#include "PS3Enums.h"

#define HID_BUFFERSIZE 	50 // Size of the buffer for the Playstation Motion Controller


#define 	ps3_low_Lx					115.0
#define 	ps3_high_Lx					140.0

#define 	ps3_low_Ly					115.0
#define 	ps3_high_Ly					140.0

#define 	ps3_low_Rx					115.0
#define 	ps3_high_Rx					140.0

#define 	ps3_low_Ry					115.0
#define 	ps3_high_Ry					140.0

#define 	joyL_up						(float)PS3BT->leftjoy_y < ps3_low_Ly
#define 	joyL_down					(float)PS3BT->leftjoy_y > ps3_high_Ly
#define 	joyL_left					(float)PS3BT->leftjoy_x < ps3_low_Lx
#define 	joyL_right					(float)PS3BT->leftjoy_x > ps3_high_Lx

#define 	joyR_up						(float)PS3BT->rightjoy_y < ps3_low_Ry
#define 	joyR_down					(float)PS3BT->rightjoy_y > ps3_high_Ry
#define 	joyR_left					(float)PS3BT->rightjoy_x < ps3_low_Rx
#define 	joyR_right					(float)PS3BT->rightjoy_x > ps3_high_Rx



typedef struct{

	/** Variable used to indicate if the normal Playstation controller is successfully connected. */
	bool PS3Connected;
	/** Variable used to indicate if the Move controller is successfully connected. */
	bool PS3MoveConnected;
	/** Variable used to indicate if the Navigation controller is successfully connected. */
	bool PS3NavigationConnected;

	 /* Variables filled from HCI event management */
	char remote_name_first; // First letter in remote name
	bool activeConnection; // Used to indicate if it's already has established a connection

	/* Variables used by high level L2CAP task */
	uint8_t l2cap_state;

	uint32_t lastMessageTime; // Variable used to store the millis value of the last message.

	union{
		uint32_t ButtonState;
		struct {
			uint8_t  SELECT 	:1 ;
			uint8_t  L3     	:1 ;
			uint8_t  R3     	:1 ;
			uint8_t  START 		:1 ;
			uint8_t  UP 		:1 ;
			uint8_t  RIGHT  	:1 ;
			uint8_t  DOWN    	:1 ;
			uint8_t  LEFT 		:1 ;
			uint8_t  L2 		:1 ;
			uint8_t  R2   		:1 ;
			uint8_t  L1   		:1 ;
			uint8_t  R1	 		:1 ;
			uint8_t  TRIANGLE	:1 ;
			uint8_t  CIRCLE   	:1 ;
			uint8_t  CROSS   	:1 ;
			uint8_t  SQUARE	 	:1 ;
			uint8_t  PS			:1 ;
			uint8_t  UNUSED1  	:2 ;
			uint8_t  MOVE	 	:1 ;
			uint8_t  T		 	:1 ;
			uint16_t UNUSED2    :11;
		};
	};
	uint32_t OldButtonState;
	uint32_t ButtonClickState;

	uint32_t timer; // Timer used to limit time between messages and also used to continuously set PS3 Move controller Bulb and rumble values
	uint32_t timerHID; // Timer used see if there has to be a delay before a new HID command

	uint8_t l2capinbuf[BULK_MAXPKTSIZE]; // General purpose buffer for L2CAP in data
	uint8_t HIDBuffer[HID_BUFFERSIZE]; // Used to store HID commands
	uint8_t HIDMoveBuffer[HID_BUFFERSIZE]; // Used to store HID commands for the Move controller

	/* L2CAP Channels */
	uint8_t control_scid[2]; // L2CAP source CID for HID_Control
	uint8_t control_dcid[2]; // 0x0040
	uint8_t interrupt_scid[2]; // L2CAP source CID for HID_Interrupt
	uint8_t interrupt_dcid[2]; // 0x0041

	BluetoothService PS3BTservice;
	uint8_t PS3BTserviceID;

#define leftjoy_x	l2capinbuf[15]
#define leftjoy_y	l2capinbuf[16]
#define rightjoy_x  l2capinbuf[17]
#define rightjoy_y	l2capinbuf[18]
#define an_L2		l2capinbuf[27]
#define an_R2		l2capinbuf[28]

	float joyR_y;
	float joyR_x;
	float joyL_y;
	float joyL_x;
	float joyR_2;
	float joyL_2;

}PS3BT_t;

/****************************************************************************************************************************************/
void PS3BTInit(PS3BT_t *PS3BT, uint8_t btadr5, uint8_t btadr4, uint8_t btadr3, uint8_t btadr2, uint8_t btadr1, uint8_t btadr0);

void L2CAP_task(PS3BT_t *PS3BT); // L2CAP state machine

/* HID Commands */
void HID_Command(PS3BT_t *PS3BT, uint8_t* data, uint8_t nbytes);
void HIDMove_Command(PS3BT_t *PS3BT, uint8_t* data, uint8_t nbytes);
void enable_sixaxis(PS3BT_t *PS3BT); // Command used to enable the Dualshock 3 and Navigation controller to send data via Bluetooth

/****************************************************************************************************************************************/
/** @name BluetoothService implementation */
/** Used this to disconnect any of the controllers. */
void Disconnect(uint32_t pointer_address);
/**
* Used to pass acldata to the services.
* @param ACLData Incoming acldata.
*/
void ACLData(uint8_t* ACLData, uint32_t pointer_address);
/** Used to run part of the state machine. */
void Run(uint32_t pointer_address);
/** Use this to reset the service. */
void Reset(uint32_t pointer_address);
/**
* Called when the controller is successfully initialized.
* Use attachOnInit(void (*funcOnInit)(void)) to call your own function.
* This is useful for instance if you want to set the LEDs in a specific way.
*/
void onInit();
/**@}*/

/****************************************************************************************************************************************/
/** @name PS3 Controller functions */
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
bool PS3BTgetButtonPress(PS3BT_t *PS3BT, ButtonEnum b);
bool PS3BTgetButtonClick(PS3BT_t *PS3BT, ButtonEnum b);
/**@}*/
/** @name PS3 Controller functions */
/**
 * Used to get the analog value from button presses.
 * @param  a The ::ButtonEnum to read.
 * The supported buttons are:
 * ::UP, ::RIGHT, ::DOWN, ::LEFT, ::L1, ::L2, ::R1, ::R2,
 * ::TRIANGLE, ::CIRCLE, ::CROSS, ::SQUARE, and ::T.
 * @return   Analog value in the range of 0-255.
 */
uint8_t PS3BTgetAnalogButton(PS3BT_t *PS3BT, ButtonEnum a);
/**
 * Used to read the analog joystick.
 * @param  a ::LeftHatX, ::LeftHatY, ::RightHatX, and ::RightHatY.
 * @return   Return the analog value in the range of 0-255.
 */
uint8_t PS3BTgetAnalogHat(PS3BT_t *PS3BT, AnalogHatEnum a);
/**
 * Used to read the sensors inside the Dualshock 3 and Move controller.
 * @param  a
 * The Dualshock 3 has a 3-axis accelerometer and a 1-axis gyro inside.
 * The Move controller has a 3-axis accelerometer, a 3-axis gyro, a 3-axis magnetometer
 * and a temperature sensor inside.
 * @return   Return the raw sensor value.
 */
int16_t PS3BTgetSensor(PS3BT_t *PS3BT, SensorEnum a);
/**
 * Use this to get ::Pitch and ::Roll calculated using the accelerometer.
 * @param  a Either ::Pitch or ::Roll.
 * @return   Return the angle in the range of 0-360.
 */
float PS3BTgetAngle(PS3BT_t *PS3BT, AngleEnum a);
/**
 * Read the sensors inside the Move controller.
 * @param  a ::aXmove, ::aYmove, ::aZmove, ::gXmove, ::gYmove, ::gZmove, ::mXmove, ::mYmove, and ::mXmove.
 * @return   The value in SI units.
 */
float PS3BTget9DOFValues(PS3BT_t *PS3BT, SensorEnum a);
/**
 * Get the status from the controller.
 * @param  c The ::StatusEnum you want to read.
 * @return   True if correct and false if not.
 */
bool PS3BTgetStatus(PS3BT_t *PS3BT, StatusEnum c);

/** Used to set all LEDs and rumble off. */
void PS3BTsetAllOff();
/** Turn off rumble. */
void PS3BTsetRumbleOff();
/**
 * Turn on rumble.
 * @param mode Either ::RumbleHigh or ::RumbleLow.
 */
void PS3BTsetRumbleOn(PS3BT_t *PS3BT,RumbleEnum mode);
/**
 * Turn on rumble using custom duration and power.
 * @param rightDuration The duration of the right/low rumble effect.
 * @param rightPower The intensity of the right/low rumble effect.
 * @param leftDuration The duration of the left/high rumble effect.
 * @param leftPower The intensity of the left/high rumble effect.
 */
void setRumbleOn(PS3BT_t *PS3BT,uint8_t rightDuration, uint8_t rightPower, uint8_t leftDuration, uint8_t leftPower);

/**
 * Set LED value without using ::LEDEnum.
 * @param value See: ::LEDEnum.
 */
void PS3BTsetLedRaw(PS3BT_t *PS3BT,uint8_t value);

/** Turn all LEDs off. */
void PS3BTsetAllLedOff(PS3BT_t *PS3BT) ;

/**
 * Turn the specific LED off.
 * @param a The ::LEDEnum to turn off.
 */
void PS3BTsetLedOff(PS3BT_t *PS3BT,LEDEnum a);
/**
 * Turn the specific LED on.
 * @param a The ::LEDEnum to turn on.
 */
void PS3BTsetLedOn(PS3BT_t *PS3BT, LEDEnum a);
/**
 * Toggle the specific LED.
 * @param a The ::LEDEnum to toggle.
 */
void PS3BTsetLedToggle(PS3BT_t *PS3BT, LEDEnum a);

/**
 * Use this to set the Color using RGB values.
 * @param r,g,b RGB value.
 */
void PS3BTmoveSetBulb(PS3BT_t *PS3BT, uint8_t r, uint8_t g, uint8_t b);
/**
 * Use this to set the color using the predefined colors in ::ColorsEnum.
 * @param color The desired color.
 */
void moveSetBulb(PS3BT_t *PS3BT, ColorsEnum color);
/**
 * Set the rumble value inside the Move controller.
 * @param rumble The desired value in the range from 64-255.
 */
void PS3BTmoveSetRumble(PS3BT_t *PS3BT, uint8_t rumble);

/** Used to get the millis() of the last message */
uint32_t PS3BTgetLastMessageTime(PS3BT_t *PS3BT) ;
/**@}*/

/****************************************************************************************************************************************/
#endif /* STM32_USB_HOST_LIBRARY_CLASS_BTD_INC_PS3BT_H_ */
