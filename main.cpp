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

/** Library Imports. */
#include "mbed.h"
#include <chrono> 

/** Custom Imports. */
// #define <TESTS/ComDevice/test_SerialComDevice.cpp>

#define CAN_Tx D2
#define CAN_Rx D10

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

    // return testMain();
}
