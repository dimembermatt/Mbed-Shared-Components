/**
 * PV Curve Tracer Project
 * 
 * File: Serial.cpp
 * Author: Matthew Yu
 * Organization: UT Solar Vehicles Team
 * Created on: September 27th, 2020
 * Last Modified: 10/07/20
 * 
 * File Discription: This implementation file implements the serial class, which is an
 * concrete class that defines a clear read/write API for handling communication
 * across UART serial lines.
 */
#include "Serial.h"
#define MAXIMUM_BUFFER_SIZE 32


Serial::Serial(
    const PinName txPin, 
    const PinName rxPin, 
    const uint32_t _bufferSize,
    const uint32_t baudRate) : serialPort(txPin, rxPin) {
    serialPort.set_baud(baudRate);
    serialPort.set_format(8, BufferedSerial::None, 1);

    buffer = new char[_bufferSize];
    totalCapacity = _bufferSize;
    usedCapacity = 0;
    writeIdx = 0;
    readIdx = 0;
}

bool Serial::sendMessage(Message* message) {
    char data[MAXIMUM_BUFFER_SIZE] = {0};
    if (message->encode(data, MAXIMUM_BUFFER_SIZE)) {
        if (serialPort.write(data, MAXIMUM_BUFFER_SIZE) >= 0) return true; 
    }
    return false;
}

bool Serial::getMessage(Message* message) {
    bufferSem.acquire();

    /* TODO: type 2 encoding parsing. */
    bool found = false;
    uint32_t total = 0;
    uint32_t currIdx = readIdx;

    /* Walk around the buffer until we find the next ';'. */
    while (!isBufferEmpty(currIdx , writeIdx) && !found) {
        if (buffer[currIdx] == ';') {
            found = true;
        }
        currIdx = (currIdx + 1) % totalCapacity;
        total++;
    }

    /* If we can't find the next ';', early exit since there is no complete
        message to read. */
    if (!found) {
        bufferSem.release();
        return false;
    }

    /* TODO: If we found the next ';', retrieve the substring from the buffer.
    */
    uint32_t idx = readIdx;
    total--; // go back a step to the ';'.
    usedCapacity -= total;
    char *msg = new char[total];
    for (uint32_t i = 0; i < total; i++) {
        msg[i] = buffer[idx];
        idx = (idx + 1) % totalCapacity;
    }
    // update read pointer
    readIdx = currIdx;
    // append null
    msg[total] = '\0';
    return msg;


    /* Attempt to parse it into a message packet. */
    /* Invalid ID parameter. */
    if (false) {
        bufferSem.release();
        return false;
    }

    /* Invalid DATA parameter. */
    if (false) {
        bufferSem.release();
        return false;
    }

    bufferSem.release();
    return true;
}

void Serial::purgeBuffer() {
    bufferSem.acquire();
    usedCapacity = 0; 
    writeIdx = 0; 
    readIdx = 0;
    bufferSem.release();
}

Serial::~Serial() { delete[] buffer; }

/** Private Methods. */

void Serial::handler() {
    if (!bufferSem.try_acquire()) return;

    if (isBufferFull(readIdx, writeIdx)) return;

    uint32_t freeCapacity = totalCapacity - usedCapacity;

    /** Read into a temporary buffer. */
    char *buf = new char[freeCapacity];
    ssize_t bytesRead = serialPort.read(buf, freeCapacity);

    /** Smartly put in the bytes into our actual buffer. */
    if (bytesRead >= 0) {
        
        for (ssize_t i = 0; i < bytesRead; i++) {
            buffer[writeIdx] = buf[i];
            writeIdx = (writeIdx + 1) % totalCapacity;
        }
        usedCapacity += bytesRead;
        
    }
    delete[] buf;

    bufferSem.release();
}

bool Serial::isBufferFull(uint32_t _readIdx, uint32_t _writeIdx) {
    if (_readIdx == (_writeIdx + 1) % totalCapacity) return true;
    else return false;
}

bool Serial::isBufferEmpty(uint32_t _readIdx, uint32_t _writeIdx) {
    if (_readIdx == _writeIdx) return true;
    else return false;
}