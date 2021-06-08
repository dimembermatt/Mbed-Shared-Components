/**
 * PV Curve Tracer Project
 * 
 * File: SerialDevice.cpp
 * Author: Matthew Yu
 * Organization: UT Solar Vehicles Team
 * Created on: September 27th, 2020
 * Last Modified: 06/08/21
 * 
 * File Description: This implementation file implements the SerialDevice class,
 * which is a concrete class that defines a clear read/write API for handling
 * communication across UART SerialDevice lines.
 */
#include "SerialDevice.h"
#include <stdlib.h>

#define T2MSG_BYTES_IN_MESSAGE      13
#define T2MSG_NUM_ID_BYTES          4
#define T2MSG_NUM_DATA_BYTES        8

/** Public Methods. */

SerialDevice::SerialDevice(
    const PinName txPin, 
    const PinName rxPin, 
    const uint16_t bufferSize,
    const uint16_t baudRate) : mSerialPort(txPin, rxPin) {
    mSerialPort.set_baud(baudRate);
    mSerialPort.set_format(8, BufferedSerial::None, 1);
    mBuffer = new char[bufferSize];
    mTotalCapacity = bufferSize;
    mUsedCapacity = 0;
    mWriteIdx = 0;
    mReadIdx = 0;
}

bool SerialDevice::sendMessage(Message* message) {
    char data[T2MSG_BYTES_IN_MESSAGE] = {0};
    if (message->encode(data, T2MSG_BYTES_IN_MESSAGE)) {
        mSerialPort.write(data, T2MSG_BYTES_IN_MESSAGE);
        return true;
    }
    return false;
}

bool SerialDevice::getMessage(Message* message) {
    mBufferSem.acquire();

    /* The baseline message is in DeSeCa type 2 encoding, which consists of an
       ID field that is up to 16 bits wide (4 bytes) and DATA field that is 64
       bits wide (8 bytes). The ID width is an extension of CAN messages, which
       are up to 11 bits wide (3 bytes). Overall, a message will have a fixed 12
       byte width in the format [ID:4;DATA:8].

       To see if we've received a message, we simply we need to check whether
       there are 12 bytes in the buffer that can be read. If there are, we
       decode the ID and DATA fields from it and insert into the message. */

    if (mUsedCapacity < T2MSG_BYTES_IN_MESSAGE) {
        mBufferSem.release();
        return false;
    } else {
        uint16_t id;
        char idBuf[T2MSG_NUM_ID_BYTES] = {'\0'};
        for (uint16_t i = 0; i < T2MSG_NUM_ID_BYTES; ++i) {
            idBuf[i] = mBuffer[mReadIdx];
            mReadIdx = (mReadIdx + 1) % mTotalCapacity;
        }
        id = (uint16_t) strtoul (idBuf, NULL, 16);
        message->setMessageID(id);

        char buf[T2MSG_NUM_DATA_BYTES];
        for (uint16_t i = 0; i < T2MSG_NUM_DATA_BYTES; ++i) {
            buf[i] = mBuffer[mReadIdx];
            mReadIdx = (mReadIdx + 1) % mTotalCapacity;
        }
        message->setMessageDataC(buf, T2MSG_NUM_DATA_BYTES);

        mBufferSem.release();
        return true;
    }
}

void SerialDevice::purgeBuffer() {
    mBufferSem.acquire();
    mUsedCapacity = 0; 
    mWriteIdx = 0; 
    mReadIdx = 0;
    mBufferSem.release();
}

SerialDevice::~SerialDevice() { delete[] mBuffer; }

/** Private Methods. */

void SerialDevice::handler() {
    /* Early exit if we can't acquire the buffer or the buffer is full. */
    if (!mBufferSem.try_acquire()) return;
    if (isBufferFull(mReadIdx, mWriteIdx)) return;

    /* Create a temporary buffer the size of the space that is left, 
       and read into it. */
    uint16_t freeCapacity = mTotalCapacity - mUsedCapacity;
    char *buf = new char[freeCapacity];
    ssize_t bytesRead = mSerialPort.read(buf, freeCapacity);

    /* Smartly put in the bytes into our actual buffer. */
    if (bytesRead > 0) {
        for (ssize_t i = 0; i < bytesRead; ++i) {
            mBuffer[mWriteIdx] = buf[i];
            mWriteIdx = (mWriteIdx + 1) % mTotalCapacity;
        }
        mUsedCapacity += bytesRead;
    }
    mBufferSem.release();
    delete[] buf;
}

bool SerialDevice::isBufferFull(uint16_t readIdx, uint16_t writeIdx) {
    if (readIdx == (writeIdx + 1) % mTotalCapacity) return true;
    else return false;
}

bool SerialDevice::isBufferEmpty(uint16_t readIdx, uint16_t writeIdx) {
    if (readIdx == writeIdx) return true;
    else return false;
}

#undef T2MSG_BYTES_IN_MESSAGE
#undef T2MSG_NUM_ID_BYTES
#undef T2MSG_NUM_DATA_BYTES