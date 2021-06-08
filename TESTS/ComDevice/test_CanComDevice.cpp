/**
 * Project: Mbed-Shared-Components
 * File: test_CanComDevice.cpp
 * Author: Matthew Yu (2021).
 * Created on: 06/08/21
 * Last Modified: 06/08/21
 * File Description: This program tests the Can communication device 
 * drivers. A CAN message of ID 0x21 and variable data is sent to a 
 * receiver device.
 * L432KC Pinout:
 * https://os.mbed.com/media/uploads/bcostm/nucleo_l432kc_2017_10_09.png
 */
/** Custom Imports. */
#include <src/Message/Message.h>
#include <src/ComDevice/ComDevice.h>

#define CAN_TX D2
#define CAN_RX D10
#define COM_RATE_MS 50

/** Communication Device. */
ComDevice transceiver(ComDevice::CAN, CAN_TX, CAN_RX);

int testMain(void) {
    /* Initialize transceiver to begin serial communication. */
    transceiver.startMs(COM_RATE_MS);

    uint64_t dataVal = 0;
    while (true) {
        Message helloWorld = Message(0x21, dataVal);
        transceiver.sendMessage(&helloWorld);
        ++dataVal;
    }
}
