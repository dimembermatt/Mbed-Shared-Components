/**
 * Maximum Power Point Tracker Project
 *
 * File: CanDevice.cpp
 * Author: Matthew Yu
 * Organization: UT Solar Vehicles Team
 * Created on: September 10th, 2020
 * Last Modified: 06/05/21
 *
 * File Description: This file manages the CAN class, abstracting away
 * implementation logic to send and receive messages via the CAN lines and
 * providing an easy interface.
 */
#include "CanDevice.h"
#include <src/CanIds/CanIdList.h>

/** Public Methods. */

CanDevice::CanDevice(
    const PinName pinTx, 
    const PinName pinRx) : mCan(pinRx, pinTx, CAN_BUS_BAUD_RATE) {
    mGetIdx = 0;
    mPutIdx = 0;
}

bool CanDevice::sendMessage(Message* message) {
    #define CAN_BYTES_IN_MESSAGE 8
    char data[CAN_BYTES_IN_MESSAGE] = {'\0'};
    message->getMessageDataC(data, CAN_BYTES_IN_MESSAGE);
    if (mCan.write(CANMessage(message->getMessageID(), data))) return true;
    return false;
    #undef CAN_BYTES_IN_MESSAGE
}

bool CanDevice::getMessage(Message* message) {
    mMailboxSem.acquire();

    if (isBufferEmpty(mGetIdx, mPutIdx)) {
        mMailboxSem.release();
        return false;
    } else {
        /* Copy each field over. We assume the data is in chars. */
        message->setMessageID(mMailbox[mGetIdx].id);
        unsigned char* data = &(mMailbox[mGetIdx].data[0]);
        message->setMessageDataC(
            reinterpret_cast<const char*>(data), 
            mMailbox[mGetIdx].len);
        mGetIdx = (mGetIdx + 1) % CAN_BUS_SIZE;

        mMailboxSem.release();
        return true;
    }
}

void CanDevice::addCanIdFilter(uint16_t id) { mFilterList.insert(id); }

void CanDevice::removeCanIdFilter(uint16_t id) { mFilterList.erase(id); }

/** Private Methods. */

void CanDevice::handler() {
    if (!mMailboxSem.try_acquire()) return;
    if (isBufferFull(mGetIdx, mPutIdx)) return;

    /* If bus buffer is free, read a new byte. */
    mCan.read(mMailbox[mPutIdx]);
    /* Ignore msg IDs that don't match our accept list. */
    if (checkId(mMailbox[mPutIdx].id)) {
        mPutIdx = (mPutIdx + 1) % CAN_BUS_SIZE;
    }

    mMailboxSem.release();
}

bool CanDevice::checkId(const uint16_t id) const {
    std::set<uint16_t>::iterator i;
    for (i = mFilterList.begin(); i != mFilterList.end(); ++i) {
        if (*i == id) return true;
    }
    return false;
}

bool CanDevice::isBufferFull(const uint16_t readIdx, const uint16_t writeIdx) const {
    if (readIdx == (writeIdx + 1) % CAN_BUS_SIZE) return true;
    else return false;
}

bool CanDevice::isBufferEmpty(const uint16_t readIdx, const uint16_t writeIdx) const {
    if (readIdx == writeIdx) return true;
    else return false;
}
