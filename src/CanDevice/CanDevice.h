/**
 * Maximum Power Point Tracker Project
 * 
 * File: CanDevice.h
 * Author: Matthew Yu
 * Organization: UT Solar Vehicles Team
 * Created on: September 12th, 2020
 * Last Modified: 06/08/21
 * 
 * File Description: This header file describes the CanDevice class, which is a
 * concrete class that defines a clear read/write API for handling communication
 * across CAN lines.
 */
#pragma once
#include "mbed.h"
#include <src/InterruptDevice/InterruptDevice.h>
#include <src/Message/Message.h>
#include <set>

#define CAN_BUS_SIZE 50
#define CAN_BUS_BAUD_RATE 500000

class CanDevice final : public InterruptDevice {
    public:
        /**
         * Constructor for a CanDevice object.
         * 
         * @param[in] pinTx TX pin to attach CAN (pin) to.
         * @param[in] pinRx RX pin to attach CAN (pin) to.
         */
        explicit CanDevice(const PinName pinTx, const PinName pinRx);

        /** 
         * sendMessage Broadcasts a message over CAN to the network.
         * 
         * @param[in] message Pointer to a message instance to send.
         * @return Whether the message was sent successfully or not.
         */
        bool sendMessage(Message* message);

        /**
         * getMessage Grabs a Message object from the internal buffer, if any.
         * 
         * @param[out] message Pointer to a Message instance to receive.
         * @return Whether the message was retrieved successfully or not.
         */
        bool getMessage(Message* message);

        /**
         * Add and remove CAN IDs to a set for filtering. Messages with IDs on
         * the list are retained.
         */
        void addCanIdFilter(uint16_t id);
        void removeCanIdFilter(uint16_t id);

    private:
        /** Reads a CANmessage and puts it into the mailbox. */
        void handler() override;

        bool isBufferFull(const uint16_t readIdx, const uint16_t writeIdx) const;
        bool isBufferEmpty(const uint16_t readIdx, const uint16_t writeIdx) const;

        /**
         * Checks the ID against a list of CAN ids. If it matches we return
         * success.
         * 
         * @param[in] id ID of a CANMessage to check.
         * @return True if ID matches our list, False otherwise.
         */
        bool checkId(const uint16_t id) const; 

    private:
        /* Can object and buffer for messages. */
        CAN mCan;
        CANMessage mMailbox[CAN_BUS_SIZE];

        /** Lock for the mailbox. I hope you have a key. */
        Semaphore mMailboxSem;

        /** Indices for traversing the mailbox. */
        uint16_t mGetIdx;
        uint16_t mPutIdx;

        /** Set of CAN IDs to retain. */
        std::set<uint16_t> mFilterList;
};
