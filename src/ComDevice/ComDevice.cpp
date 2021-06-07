/**
 * Maximum Power Point Tracker Project
 * 
 * File: ComDevice.cpp
 * Author: Matthew Yu
 * Organization: UT Solar Vehicles Team
 * Created on: June 5th, 2021
 * Last Modified: 06/05/21
 * 
 * File Description: This implementation file describes the ComDevice class,
 * which is an abstraction layer over various communication devices like
 * CanDevice and SerialDevice.
 */
#include "ComDevice.h"

ComDevice::ComDevice(const enum ComDevice deviceType, const PinName pinTx, const PinName pinRx) {
    switch (deviceType) {
    case CAN:
        mComDevice = new CanDevice(pinTx, pinRx);
        break;
    case SERIAL:
        mComDevice = new SerialDevice(pinTx, pinRx, 32, 115200);
        break;
    default:
        break;
    }
}

bool ComDevice::sendMessage(Message* message) { mComDevice->sendMessage(message); }
bool ComDevice::getMessage(Message* message) { mComDevice->getMessage(message); }

void ComDevice::purgeSerialBuffer(void) { 
    static_cast<SerialDevice*>(mComDevice)->purgeBuffer(); 
}

void ComDevice::addCanIdFilter(uint16_t id) {
    static_cast<CanDevice*>(mComDevice)->addCanIdFilter(id);
}

void ComDevice::removeCanIdFilter(uint16_t id) {
    static_cast<CanDevice*>(mComDevice)->removeCanIdFilter(id);
}

ComDevice::~ComDevice(void) { delete mComDevice; }