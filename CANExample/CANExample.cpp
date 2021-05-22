/**
 * File name: CANExample.cpp
 * Devices: LM4F120; TM4C123
 * Description: Example programs to demonstrate the capabilities of CANDevice drivers. 
 * Two devices are hooked up to each other and ping packets to each other at 1/5 Hz.
 * Authors: Matthew Yu.
 * Last Modified: 04/10/21
 */
#include "mbed.h"
#include <chrono>
#include "./lib/CANDevice/CANDevice.h"

#define CAN_RX D10
#define CAN_TX D2

CANDevice can(CAN_RX, CAN_TX);
Ticker ticker;

typedef union {
    uint32_t uintVal;
    char charArray[8];
} message_t;

message_t data = 0x0000FFFF;
void sendMessage(void) {
    can.sendMessage(0x00, data.charArray, 8);
}


int main (void) {
    ticker.attach(&sendMessage, std::chrono::seconds(5));

    char messageBuffer[8];
    while (1) {
        printf("%i", can.getMessage(messageBuffer, 8));
    }
}
