/**
 * Project: Mbed-Shared-Components
 * File: main.cpp
 * Author: Matthew Yu (2021).
 * Created on: 06/08/21
 * Last Modified: 06/08/21
 * File Description: This program acts as a main tester for 
 * various components of the Mbed-Shared-Components repository.
 * Modify the preprocessor definition to swap between tester programs.
 * L432KC Pinout:
 * https://os.mbed.com/media/uploads/bcostm/nucleo_l432kc_2017_10_09.png
 */
static const enum TEST_ID {
    SERIAL_COM_DEVICE,
    CAN_COM_DEVICE
} TestId = SERIAL_COM_DEVICE;


/** Library Imports. */
#include "mbed.h"
#include <chrono> 

/** Custom Imports. */
#if TestId == SERIAL_COM_DEVICE
#include <TESTS/ComDevice/test_SerialComDevice.cpp>
#elif TestId == CAN_COM_DEVICE
#include <TESTS/ComDevice/test_CanComDevice.cpp>
#endif


#define HEARTBEAT_LED D3

/** Variable Declarations. */

/** Heartbeat LED. */
DigitalOut ledHeartbeat(HEARTBEAT_LED);
LowPowerTicker tickHeartbeat;
std::chrono::milliseconds periodHeartbeat(500);

/** Function declaration and implementations. */
void heartbeat(void) { ledHeartbeat = !ledHeartbeat; }

int main(void) {
    /* Initialize heartbeat to toggle at 0.5 s. */
    tickHeartbeat.attach(&heartbeat, std::chrono::microseconds(periodHeartbeat));

    return testMain();
}
