/**
 * PV Curve Tracer Project
 * 
 * File: SerialDevice.h
 * Author: Matthew Yu
 * Organization: UT Solar Vehicles Team
 * Created on: September 27th, 2020
 * Last Modified: 06/08/21
 * 
 * File Description: This header file describes the SerialDevice class, which is
 * a concrete class that defines a clear read/write API for handling
 * communication across UART serial lines.
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
class SerialDevice final : public InterruptDevice {
    public:
        /** 
         * Constructor for a serial object.
         * 
         * @param[in] txPin Transceiver pin.
         * @param[in] rxPin Receiver pin.
         * @param[in] bufferSize Size of the buffer (num chars stored).
         * @param[in] baudRate Baudrate of the connection.
         * @note bufferSize is ideally a multiple of 12 bytes (1 T2 message).
         */
        explicit SerialDevice(
            const PinName txPin, 
            const PinName rxPin, 
            const uint16_t bufferSize, 
            const uint16_t baudRate
        );

        /** 
         * sendMessage Sends a message over serial to a dedicated receiver.
         * Uses message encode function (type 2 encoding). 
         * 
         * @param[in] message Pointer to a message instance to send.
         * @return Whether the message was sent successfully or not.
         */
        bool sendMessage(Message* message);

        /**
         * getMessage Grabs a Message object from the internal buffer, if any.
         * Incoming messages are type 2 encoding.
         * 
         * @note Since the serial read() function uses mutexes, we can't
         * actually perform this in an isr context. Therefore it is the user's
         * responsibility to call this function in the main loop to make sure
         * messages are captured.
         * 
         * @param[out] message Pointer to a Message instance to receive.
         * @return Whether the message was retrieved successfully or not.
         */
        bool getMessage(Message* message);

        /**
         * readData reads from the serial buffer and loads the internal buffer.

         */
        void readData(void);

        /**
         * Purges the buffer for more serial input. Happens in case we get
         * corrupted data in the buffer and it gets filled up.
         */
        void purgeBuffer(void);

        /** Deallocates relevant structures. */
        ~SerialDevice(void);

    private:
        /** Reads the serial buffer and pushes it into the secondary buffer. */
        void handler(void) override;

        inline bool isBufferFull(uint16_t readIdx, uint16_t writeIdx);
        inline bool isBufferEmpty(uint16_t readIdx, uint16_t writeIdx);

    private:
        BufferedSerial mSerialPort;

        /** Pointer to a buffer object that holds data. */
        char *mBuffer;

        /** Indices for writing/reading from a circular char buffer. */
        uint16_t mWriteIdx;
        uint16_t mReadIdx;

        /** Capacity variables. */
        uint16_t mTotalCapacity;
        uint16_t mUsedCapacity;
        
        /** mBufferSem should only be captured on the following: mUsedCapacity,
            mWriteIdx, mReadIdx, mBuffer modification. */
        Semaphore *mBufferSem;

        bool readActivity;
};
