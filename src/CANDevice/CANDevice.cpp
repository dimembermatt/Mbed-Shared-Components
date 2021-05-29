/**
 * Maximum Power Point Tracker Project
 *
 * File: CANDevice.cpp
 * Author: Matthew Yu
 * Organization: UT Solar Vehicles Team
 * Created on: September 10th, 2020
 * Last Modified: 05/25/21
 *
 * File Description: This file manages the CAN class, abstracting away
 * implementation logic to send and receive messages via the CAN lines and
 * providing an easy interface.
 */
#include "CANDevice.h"
#include <src/Message/Message.h>
#include <src/CANIDs/CANIDList.h>

/** Public Methods. */

CANDevice::CANDevice(
    const PinName pinRx,
    const PinName pinTx) : can(pinRx, pinTx, CAN_BUS_BAUD)
{
    getIdx = 0;
    putIdx = 0;
}

bool CANDevice::getMessage(CANMessage* message) {
    if (!mailboxSem.try_acquire()) return false;

    /* Get the first message from the mailbox FIFO. */
    CANMessage canMsg = mailbox[getIdx];

    /* Copy each field over. */
    message->id = canMsg.id;
    message->len = canMsg.len;
    for (uint8_t i = 0; i < canMsg.len; ++i) {
        message->data[i] = canMsg.data[i];
    }
    message->format = canMsg.format;
    message->type = canMsg.type;

    getIdx = (getIdx + 1) % CAN_BUS_SIZE;
    mailboxSem.release();

    return true;
}

bool CANDevice::sendMessage(Message* message) {
    char data[8];
    message->getMessageDataC(data);
    CANMessage msg = CANMessage(message->getMessageID(), data);
    if (can.write(msg)) return true;
    return false;
}


bool CANDevice::getMessage(Message* message) {
    if (!mailboxSem.try_acquire()) return false;

    /* Get the first message from the mailbox FIFO. */
    CANMessage canMsg = mailbox[getIdx];

    /* Copy each field over. We assume the data is in chars. */
    message->setMessageID(canMsg.id);
    // message->setMessageDataC(canMsg.data, canMsg.len);
    
    getIdx = (getIdx + 1) % CAN_BUS_SIZE;
    mailboxSem.release();

    return true;
}

bool CANDevice::getMessage(char* _message, uint8_t length) {
    if (!mailboxSem.try_acquire()) return false;

    /* Get the first message from the mailbox FIFO. */
    CANMessage canMsg = mailbox[getIdx];

    /* Copy each field over. */
    // Message message = Message(canMsg.id, canMsg.data, canMsg.len);
    // message.toString(_message, length);

    getIdx = (getIdx + 1) % CAN_BUS_SIZE;
    mailboxSem.release();

    return true;
}

void CANDevice::addCANIDFilter(uint16_t id) { filterList.insert(id); }

void CANDevice::removeCANIDFilter(uint16_t id) { filterList.erase(id); }

/** Private Methods. */

void CANDevice::handler() {
    if (!mailboxSem.try_acquire()) return;

    /* If bus buffer is free, read a new byte. */
    if (((putIdx + 1) % CAN_BUS_SIZE) != getIdx) {
        can.read(mailbox[putIdx]);
        /* Ignore msg IDs that don't match our accept list. */
        if (checkID(mailbox[putIdx].id)) {
            putIdx = (putIdx + 1) % CAN_BUS_SIZE;
        }
    }

    mailboxSem.release();
}

bool CANDevice::checkID(const uint16_t id) {
    std::set<uint16_t>::iterator i;
    for (i = filterList.begin(); i != filterList.end(); ++i) {
        if (*i == id) return true;
    }
    return false;
}
