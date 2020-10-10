
#ifndef _PS3ENUMS_H_
#define _PS3ENUMS_H_

#include <stdint.h>
#include "controllerEnums.h"
/** Size of the output report buffer for the Dualshock and Navigation controllers */
#define PS3_REPORT_BUFFER_SIZE  48

/** Report buffer for all PS3 commands */
extern const uint8_t PS3_REPORT_BUFFER[PS3_REPORT_BUFFER_SIZE];

/** Size of the output report buffer for the Move Controller */
#define MOVE_REPORT_BUFFER_SIZE 7

/** Used to set the LEDs on the controllers */
extern const uint8_t PS3_LEDS[];

/**
 * Buttons on the controllers.
 * <B>Note:</B> that the location is shifted 9 when it's connected via USB.
 */
extern const uint32_t PS3_BUTTONS[] ;

/**
 * Analog buttons on the controllers.
 * <B>Note:</B> that the location is shifted 9 when it's connected via USB.
 */
extern const uint8_t PS3_ANALOG_BUTTONS[];

typedef enum  {
        // Note that the location is shifted 9 when it's connected via USB
        // Byte location | bit location
        Plugged = (38 << 8) | 0x02,
        Unplugged = (38 << 8) | 0x03,

        Charging = (39 << 8) | 0xEE,
        NotCharging = (39 << 8) | 0xF1,
        Shutdown = (39 << 8) | 0x01,
        Dying = (39 << 8) | 0x02,
        Low = (39 << 8) | 0x03,
        High = (39 << 8) | 0x04,
        Full = (39 << 8) | 0x05,

        MoveCharging = (21 << 8) | 0xEE,
        MoveNotCharging = (21 << 8) | 0xF1,
        MoveShutdown = (21 << 8) | 0x01,
        MoveDying = (21 << 8) | 0x02,
        MoveLow = (21 << 8) | 0x03,
        MoveHigh = (21 << 8) | 0x04,
        MoveFull = (21 << 8) | 0x05,

        CableRumble = (40 << 8) | 0x10, // Operating by USB and rumble is turned on
        Cable = (40 << 8) | 0x12, // Operating by USB and rumble is turned off
        BluetoothRumble = (40 << 8) | 0x14, // Operating by Bluetooth and rumble is turned on
        Bluetooth = (40 << 8) | 0x16, // Operating by Bluetooth and rumble is turned off
}StatusEnum;


#endif /* __PS3ENUMS_H_ */
