/**
 * Project: Mbed-Shared-Components
 * File: test_ComDevice.cpp
 * Author: Matthew Yu (2021).
 * Created on: 06/08/21
 * Last Modified: 06/08/21
 * File Description: This program tests the serial 
 * communication device drivers.
 * L432KC Pinout:
 * https://os.mbed.com/media/uploads/bcostm/nucleo_l432kc_2017_10_09.png
 */
/** Custom Imports. */
#include <src/Message/Message.h>
#include <src/ComDevice/ComDevice.h>

#define USB_TX USBTX
#define USB_RX USBRX
#define COM_RATE_MS 50

/** Communication Device. */
ComDevice transceiver(ComDevice::SERIAL, USB_TX, USB_RX);

int testMain(void) {
    /* Initialize transceiver to begin serial communication. */
    transceiver.startMs(COM_RATE_MS);

    uint64_t dataVal = 0;
    while (true) {
        Message helloWorld = Message(0x21, dataVal);

        // char data[30];
        // char data2[30];
        // for (uint32_t i = 0; i < 30; ++i) {
        //     data[i] = '\0';
        //     data2[i] = '\0';
        // }

        // helloWorld.toString(data, 30);
        // printf("toString: %s\n", data);

        // helloWorld.encode(data2, 30);
        // printf("encode: %s\n", data2);

        transceiver.sendMessage(&helloWorld);
        ++dataVal;
    }
}
