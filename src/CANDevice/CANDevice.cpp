/**
 * Maximum Power Point Tracker Project
 * 
 * File: CANDevice.cpp
 * Author: Matthew Yu
 * Organization: UT Solar Vehicles Team
 * Created on: September 10th, 2020
 * Last Modified: 04/10/21
 * 
 * File Description: This file manages the CAN class, abstracting away
 * implementation logic to send and receive messages via the CAN lines and
 * providing an easy interface.
 */
#include "CANDevice.h"


/** Public Methods. */

CANDevice::CANDevice(
    const PinName pinRx, 
    const PinName pinTx) : can(pinRx, pinTx, CAN_BUS_BAUD) 
{
    mailboxLock = false;
    getIdx = 0;
    putIdx = 0;

    /* Initialize the default invalid CAN ID to the whitelist. */
    whitelistLen = CAN_ID_WHITELIST_LENGTH;
    for (uint32_t i = 0; i < whitelistLen; i++) {
        whitelist[i] = INVALID_CAN_ID_DEFAULT;
    }
}

result_t CANDevice::addFilter(uint32_t *ids, uint32_t len) {
    if (len > CAN_ID_WHITELIST_LENGTH) return ERROR;

    whitelistLen = len;

    /* Cycle through the list and update the IDs. */
    for (uint32_t i = 0; i < len; i++) {
        whitelist[i] = ids[i];
    }
    return OK;
}

result_t CANDevice::sendMessage(uint32_t id, char *message, uint8_t len) {
    return can.write(CANMessage(id, message, len));
}

result_t CANDevice::getMessage(char *message, int messageSize) {
    /* Stall until CANDevice is ready. */
    while (mailboxLock);

    /* If there is no message, return false. */
    if (getIdx == putIdx) return NO_MESSAGE;

    mailboxLock = true;

    /* Extract the first message from the mailbox. */
    CANMessage canMsg = mailbox[getIdx];

    /* Increment forward in the mailbox. */
    getIdx = (getIdx + 1) % CAN_BUS_SIZE;

    mailboxLock = false;

    /* Write the message into the message parameter. */
    // TODO: this may break during testing if messages are in reverse.
    uint8_t length = (len < canMsg.len) ? len : canMsg.len;
    for (uint8_t i = 0; i < length; i++) {
        message[i] = canMsg.data[i];
    }

    return OK;
}

void CANDevice::start(const int interval) {
    tick.attach(callback(this, &CANDevice::handler), 
        std::chrono::microseconds(interval));
}

void CANDevice::stop() {
    tick.detach();
}

void CANDevice::clear() {
    getIdx = 0;
    putIdx = 0;
    whitelistLen = 0;
}

/** Private Methods. */

void CANDevice::handler() {
    /* We throw out the message if we're currently locked. */
    // TODO: in the future, we may decide that the extra couple cycles 
    // waiting for L39 - L47 to occur is worth getting all messages.
    if (mailboxLock) return;
    mailboxLock = true;

    /* If mailbox is not full, put a new message into the buffer. */
    if (((putIdx + 1) % CAN_BUS_SIZE) != getIdx) {
        CANMessage msg;
        if (can.read(msg)) {
            /* Check to see if id matches our accepted list. */
            if (checkID(msg.id)) {
                /* If we have a valid message, add to the mailbox and move on. */
                mailbox[putIdx] = msg;
                putIdx = (putIdx + 1) % CAN_BUS_SIZE;
            }
        }
    }

    mailboxLock = false;
}

bool CANDevice::checkID(const int id) const {
    /* Loop through the CAN ID whitelist and see if we have a whitelisted ID. */
    for (uint32_t i = 0; i < whitelistLen; i++) {
        if (whitelist[i] == id) return true;
    }
    return false;
}