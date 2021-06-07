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

ComDevice::ComDevice(const enum ComDeviceType deviceType, const PinName pinTx, const PinName pinRx) {
    mDeviceType = deviceType;
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

bool ComDevice::sendMessage(Message* message) { 
    switch (mDeviceType) {
        case CAN:
            return static_cast<CanDevice*>(mComDevice)->sendMessage(message); 
        case SERIAL:
            return static_cast<SerialDevice*>(mComDevice)->sendMessage(message);
    }
}
bool ComDevice::getMessage(Message* message) {
    switch (mDeviceType) {
        case CAN:
            return static_cast<CanDevice*>(mComDevice)->getMessage(message); 
        case SERIAL:
            return static_cast<SerialDevice*>(mComDevice)->getMessage(message);
    }
}

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