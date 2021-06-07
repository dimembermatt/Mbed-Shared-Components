/**
 * PV Curve Tracer Project
 * 
 * File: SerialDevice.cpp
 * Author: Matthew Yu
 * Organization: UT Solar Vehicles Team
 * Created on: September 27th, 2020
 * Last Modified: 06/06/21
 * 
 * File Description: This implementation file implements the SerialDevice class,
 * which is a concrete class that defines a clear read/write API for handling
 * communication across UART SerialDevice lines.
 */
#include "SerialDevice.h"
#include <stdlib.h>

#define NUM_BYTES_IN_MESSAGE        12
#define MAX_MESSAGES_IN_BUFFER      5
#define MAXIMUM_BUFFER_SIZE         12*5 // TODO: Get preprocessor to play nice with chained defines
#define NUM_ID_BYTES_IN_MESSAGE     4
#define NUM_DATA_BYTES_IN_MESSAGE   8

/** Public Methods. */

SerialDevice::SerialDevice(
    const PinName txPin, 
    const PinName rxPin, 
    const uint32_t bufferSize,
    const uint32_t baudRate) {
    mSerialPort = BufferedSerial(txPin, rxPin);
    mSerialPort.set_baud(baudRate);
    mSerialPort.set_format(8, BufferedSerialDevice::None, 1);
    mBuffer = new char[bufferSize];
    mTotalCapacity = bufferSize;
    mUsedCapacity = 0;
    mWriteIdx = 0;
    mReadIdx = 0;
}

bool SerialDevice::sendMessage(Message* message) {
    #define NUM_BYTES_IN_MESSAGE 8
    char data[NUM_BYTES_IN_MESSAGE] = {0};
    if (message->encode(data, NUM_BYTES_IN_MESSAGE)) {
        serialPort.write(data, NUM_BYTES_IN_MESSAGE);
        return true;
    }
    return false;
    #undef NUM_BYTES_IN_MESSAGE
}

bool SerialDevice::getMessage(Message* message) {
    bufferSem.acquire();

    /* The baseline message is in DeSeCa type 2 encoding, which consists of an
       ID field that is up to 16 bits wide (4 bytes) and DATA field that is 64
       bits wide (8 bytes). The ID width is an extension of CAN messages, which
       are up to 11 bits wide (3 bytes). Overall, a message will have a fixed 12
       byte width in the format [ID:4;DATA:8].

       To see if we've received a message, we simply we need to check whether
       there are 12 bytes in the buffer that can be read. If there are, we
       decode the ID and DATA fields from it and insert into the message. */
    if (mUsedCapacity < NUM_BYTES_IN_MESSAGE) {
        bufferSem.release();
        return false;
    } else {
        uint16_t id;
        char idBuf[NUM_ID_BYTES_IN_MESSAGE] = {'\0'};
        for (uint32_t i = 0; i < NUM_ID_BYTES_IN_MESSAGE; ++i) {
            idBuf[i] = buffer[mReadIdx];
            mReadIdx = (mReadIdx + 1) % mTotalCapacity;
        }
        id = (uint16_t) strtoul (idBuf, NULL, 16);
        message->setMessageID(id);

        char buf[NUM_DATA_BYTES_IN_MESSAGE];
        for (uint32_t i = 0; i < NUM_DATA_BYTES_IN_MESSAGE; ++i) {
            buf[i] = buffer[mReadIdx];
            mReadIdx = (mReadIdx + 1) % mTotalCapacity;
        }
        message->setMessageDataC(buf, NUM_DATA_BYTES_IN_MESSAGE)''

        bufferSem.release();
        return true;
    }
}

void SerialDevice::purgeBuffer() {
    bufferSem.acquire();
    mUsedCapacity = 0; 
    mWriteIdx = 0; 
    mReadIdx = 0;
    bufferSem.release();
}

SerialDevice::~SerialDevice() { delete[] mBuffer; }

/** Private Methods. */

void SerialDevice::handler() {
    /* Early exit if we can't acquire the buffer or the buffer is full. */
    if (!bufferSem.try_acquire()) return;
    if (isBufferFull(mReadIdx, mWriteIdx)) return;

    /* Create a temporary buffer the size of the space that is left, 
       and read into it. */
    uint32_t freeCapacity = mTotalCapacity - mUsedCapacity;
    char *buf = new char[freeCapacity];
    ssize_t bytesRead = serialPort.read(buf, freeCapacity);

    /* Smartly put in the bytes into our actual buffer. */
    if (bytesRead > 0) {
        for (ssize_t i = 0; i < bytesRead; ++i) {
            buffer[mWriteIdx] = buf[i];
            mWriteIdx = (mWriteIdx + 1) % mTotalCapacity;
        }
        mUsedCapacity += bytesRead;
    }
    bufferSem.release();
    delete[] buf;
}

bool SerialDevice::isBufferFull(uint32_t readIdx, uint32_t writeIdx) {
    if (readIdx == (writeIdx + 1) % mTotalCapacity) return true;
    else return false;
}

bool SerialDevice::isBufferEmpty(uint32_t readIdx, uint32_t writeIdx) {
    if (readIdx == writeIdx) return true;
    else return false;
}