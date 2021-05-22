/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include <chrono>


// Blinking rate in milliseconds
#define BLINKING_RATE     500ms

#define CAN_Rx D10
#define CAN_Tx D2

CAN can1(CAN_Rx, CAN_Tx);
Ticker ticker;
DigitalOut led(LED1);

char counter = 0;
void send() {
    if (can1.write(CANMessage(1337, &counter, 1))) {
        counter++;
    }
}

int main()
{
    // Initialise the digital pin LED1 as an output
    ticker.attach(&send, std::chrono::seconds(5));

    while (true) {
        led = !led;
        ThisThread::sleep_for(BLINKING_RATE);
    }
}
