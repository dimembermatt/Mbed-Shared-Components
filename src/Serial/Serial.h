/**
 * PV Curve Tracer Project
 * 
 * File: Serial.h
 * Author: Matthew Yu
 * Organization: UT Solar Vehicles Team
 * Created on: September 27th, 2020
 * Last Modified: 05/25/21
 * 
 * File Description: This header file describes the serial class, which is an
 * concrete class that defines a clear read/write API for handling communication
 * across UART serial lines.
 */
#pragma once
#include "mbed.h"
#include <src/InterruptDevice/InterruptDevice.h>
#include <src/Message/Message.h>


/**
 * Definition of an implementation of serial communication using the mbed
 * BufferedSerial class.
 * 
 * The Serial class sets up a the serial communication and manages a buffer to
 * read and translate messages from the PC via USB. Data is captured on
 * interrupt from a BufferedSerial instance and stashed into a string buffer;
 * this secondary buffer concatenates data into a single stream.
 * 
 * The caller can then asynchronously extract messages from the stream in order.
 */
class Serial: public InterruptDevice {
    public:
        /** 
         * Constructor for a serial object.
         * 
         * @param[in] txPin Transceiver pin.
         * @param[in] rxPin Receiver pin.
         * @param[in] bufferSize Size of the buffer (num chars stored).
         * @param[in] baudRate Baudrate of the connection.
         */
        explicit Serial(
            const PinName txPin, 
            const PinName rxPin, 
            const uint32_t bufferSize, 
            const uint32_t baudRate
        );

        /** 
         * sendMessage Sends a message over serial to a dedicated receiver.
         * Uses message toString (type 1 encoding). 
         * TODO: Outgoing messages are type 1 encoding, convert to type 2.
         * 
         * @param[in] message Pointer to a message instance to send.
         * @return Whether the message was sent successfully or not.
         */
        bool sendMessage(Message* message);

        /**
         * getMessage Grabs a Message object from the internal buffer, if any.
         * TODO: Incoming messages are type 2 encoding.
         * 
         * @param[out] message Pointer to a Message instance to receive.
         * @return Whether the message was retrieved successfully or not.
         */
        bool getMessage(Message* message);

        /**
         * Purges the buffer for more serial input. Happens in case we get
         * corrupted data in the buffer and it gets filled up.
         */
        void purgeBuffer();

        /**
         * Deallocates relevant structures.
         */
        ~Serial();

    private:
        /** Reads the serial buffer and pushes it into the secondary buffer. */
        void handler();

        bool isBufferFull(uint32_t readIdx, uint32_t writeIdx);
        bool isBufferEmpty(uint32_t readIdx, uint32_t writeIdx);

    private:
        BufferedSerial serialPort;

        /** Pointer to a buffer object that holds data. */
        char *buffer;

        /** Indices for writing/reading from a circular char buffer. */
        uint32_t writeIdx;
        uint32_t readIdx;

        /** Capacity variables. */
        uint32_t totalCapacity;
        uint32_t usedCapacity;
        
        /** bufferSem should only be captured on the following: usedCapacity,
            writeIdx, readIdx, buffer modification. */
        Semaphore bufferSem;
};
