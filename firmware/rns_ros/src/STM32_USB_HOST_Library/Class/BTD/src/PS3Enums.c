
#include "STM32_USB_HOST_Library/Class/BTD/inc/PS3Enums.h"

const uint8_t PS3_ANALOG_BUTTONS[]  = {
        23, // UP_ANALOG
        24, // RIGHT_ANALOG
        25, // DOWN_ANALOG
        26, // LEFT_ANALOG
        0, 0, 0, 0, // Skip SELECT, L3, R3 and START

        27, // L2_ANALOG
        28, // R2_ANALOG
        29, // L1_ANALOG
        30, // R1_ANALOG
        31, // TRIANGLE_ANALOG
        32, // CIRCLE_ANALOG
        33, // CROSS_ANALOG
        34, // SQUARE_ANALOG
        0, 0, // Skip PS and MOVE

        // Playstation Move Controller
        15 // T_ANALOG - Both at byte 14 (last reading) and byte 15 (current reading)
};

const uint32_t PS3_BUTTONS[]  = {
        0x10, // UP
        0x20, // RIGHT
        0x40, // DOWN
        0x80, // LEFT

        0x01, // SELECT
        0x08, // START
        0x02, // L3
        0x04, // R3

        0x0100, // L2
        0x0200, // R2
        0x0400, // L1
        0x0800, // R1

        0x1000, // TRIANGLE
        0x2000, // CIRCLE
        0x4000, // CROSS
        0x8000, // SQUARE

        0x010000, // PS
        0x080000, // MOVE - covers 12 bits - we only need to read the top 8
        0x100000, // T - covers 12 bits - we only need to read the top 8
};

const uint8_t PS3_LEDS[]  = {
        0x00, // OFF
        0x01, // LED1
        0x02, // LED2
        0x04, // LED3
        0x08, // LED4

        0x09, // LED5
        0x0A, // LED6
        0x0C, // LED7
        0x0D, // LED8
        0x0E, // LED9
        0x0F, // LED10
};

const uint8_t PS3_REPORT_BUFFER[PS3_REPORT_BUFFER_SIZE]  = {
        0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00,
        0xff, 0x27, 0x10, 0x00, 0x32,
        0xff, 0x27, 0x10, 0x00, 0x32,
        0xff, 0x27, 0x10, 0x00, 0x32,
        0xff, 0x27, 0x10, 0x00, 0x32,
        0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
