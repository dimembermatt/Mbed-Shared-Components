/**
 * Maximum Power Point Tracker Project
 * 
 * File: ComDevice.h
 * Author: Matthew Yu
 * Organization: UT Solar Vehicles Team
 * Created on: June 5th, 2021
 * Last Modified: 06/05/21
 * 
 * File Description: This header file describes the ComDevice class, which is an
 * abstraction layer over various communication devices like CanDevice and
 * SerialDevice.
 */
#pragma once
#include <src/CanDevice/CanDevice.h>
#include <src/SerialDevice/SerialDevice.h>
#include <src/Message/Message.h>

class ComDevice {
    public:
        enum ComDeviceType {CAN, SERIAL};

    public:
        /**
         * Constructor for a ComDevice.
         * 
         * @param[in] deviceType Device that should be instantiated.
         */
        explicit ComDevice(const enum ComDeviceType deviceType, const PinName pinTx, const PinName pinRx);

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
         * @param[out] message Pointer to a Message instance to receive.
         * @return Whether the message was retrieved successfully or not.
         */
        bool getMessage(Message* message);

        /** SerialDevice passthrough. */
        void purgeSerialBuffer(void);

        /** CanDevice passthrough. */
        void addCanIdFilter(uint16_t id);
        void removeCanIdFilter(uint16_t id);

        /** Deallocates relevant structures. */
        ~ComDevice(void);

    private:
        InterruptDevice * mComDevice;
        enum ComDeviceType mDeviceType;
};