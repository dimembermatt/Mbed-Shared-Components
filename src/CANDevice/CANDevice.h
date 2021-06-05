/**
 * Maximum Power Point Tracker Project
 * 
 * File: CANDevice.h
 * Author: Matthew Yu
 * Organization: UT Solar Vehicles Team
 * Created on: September 12th, 2020
 * Last Modified: 05/25/21
 * 
 * File Description: This file manages the CAN class, abstracting away
 * implementation logic to send and receive messages via the CAN lines and
 * providing an easy interface.
 */
#pragma once
#include "mbed.h"
#include <src/InterruptDevice/InterruptDevice.h>
#include <src/Message/Message.h>
#include <set>

#define CAN_BUS_SIZE 200
#define CAN_BUS_BAUD 25000
#define MAX_CAN_IDS 10

/**
 * Definition of an implementation for the CAN abstraction layer.
 * 
 * The CANDevice implements methods to control the transmission of messages 
 * between the device and other CAN lines. It receives and sends messages 
 * given to it.
 */
class CANDevice: public InterruptDevice {
    public:
        /**
         * Constructor for a CANDevice object.
         * 
         * @param[in] pinRx RX pin to attach CAN (pin) to.
         * @param[in] pinTx TX pin to attach cAN (pin) to.
         */
        explicit CANDevice(const PinName pinRx, const PinName pinTx);

        /** sendMessage Broadcasts a message over CAN to the network. */
        bool sendMessage(Message* message);
        
        /**
         * getMessage Grabs a CAN message from the internal buffer, if any.
         * 
         * @param[out] message Pointer to a CAN Message. 
         * @return Whether the message was retrieved successfully or not.
         */
        bool getMessage(CANMessage* message);

        /**
         * getMessage Grabs a Message object from the internal buffer, if any.
         * 
         * @param[out] message Pointer to a Message instance.
         * @return Whether the message was retrieved successfully or not.
         */
        bool getMessage(Message* message);

        /**
         * getMessage Grabs a human readable string representing a message
         * from the internal buffer, if any.
         * 
         * @param[out] message Pointer to a string array.
         * @param[in] length Length of the string array.
         * @return Whether the message was retrieved successfully or not.
         */
        bool getMessage(char* message, uint8_t length);

        /**
         * Add and remove CAN IDs to a list for filtering. Messages with IDs on
         * the list are retained.
         */
        void addCANIDFilter(uint16_t id);
        void removeCANIDFilter(uint16_t id);

    private:
        /** Reads a CANmessage and puts it into the mailbox. */
        void handler();

        /**
         * Checks the ID against a list of CAN ids. If it matches we return success.
         * 
         * @param[in] ID ID of a CANMessage to check.
         * @return True if ID matches our list, False otherwise.
         */
        bool checkID(const uint16_t id); 

    private:
        CAN can;
        CANMessage msg;
        CANMessage mailbox[CAN_BUS_SIZE];

        /** Lock for the mailbox. I hope you have a key. */
        Semaphore mailboxSem;

        /** Indices for traversing the mailbox. */
        int getIdx;
        int putIdx;

        /** List of CAN IDs to keep. */
        std::set<uint16_t> filterList;
};
