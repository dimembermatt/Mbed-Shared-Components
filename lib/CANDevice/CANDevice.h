/**
 * Maximum Power Point Tracker Project
 * 
 * File: CANDevice.h
 * Author: Matthew Yu
 * Organization: UT Solar Vehicles Team
 * Created on: September 12th, 2020
 * Last Modified: 04/10/21
 * 
 * File Description: This file manages the CAN class, abstracting away
 * implementation logic to send and receive messages via the CAN lines and
 * providing an easy interface.
 */
#pragma once
#include "mbed.h"
#include <chrono>
#include <string.h>
#include <src/Definitions/Definitions.hpp>


#define CAN_ID_WHITELIST_LENGTH 20
#define CAN_BUS_SIZE 200
#define CAN_BUS_BAUD 25000
#define INVALID_CAN_ID_DEFAULT 0x7FF /* This may be allocated to someone. Make sure it isn't. */


/**
 * Definition of an implementation for the CAN abstraction layer.
 * 
 * The CANDevice implements methods to control the transmission of messages 
 * between the device and other CAN lines. It receives and sends messages 
 * given to it.
 */
class CANDevice {
    public:
        /**
         * Constructor for a CANDevice object.
         * 
         * @param[in] pinRx RX pin to attach CAN (pin) to.
         * @param[in] pinTx TX pin to attach cAN (pin) to.
         */
        CANDevice(const PinName pinRx, const PinName pinTx);

        /**
         * Filters what messages can be added to the mailbox.
         * 
         * @param[in] ids A pointer to a list of IDs to capture.
         * @param[in] len The number of IDs in the list.
         * @return Whether the list was applied correctly. The applicable return
         *         codes are ERROR (0) and OK (1).
         * @note An error event can occur when len > CAN_ID_WHITELIST_LENGTH.
         *       Subsequent calls of addFilter overwrites the previously set filter.
         *       Call this prior to calling start() or after calling stop() for
         *       best results.
         */
        result_t addFilter(uint32_t *ids, uint32_t len);

        /**
         * Sends a message over can.
         * 
         * @param[in] id The CAN ID assigned to the message.
         * @param[in] message The CAN message to send.
         * @param[in] len The number of bytes of the message.
         * @return Whether a message was extracted from the mailbox. The
         *         applicable return codes are ERROR (0) and OK (1).
         */
        result_t sendMessage(uint32_t id, char *message, uint8_t len);

        /**
         * Grabs a CAN message from the internal buffer, if any. The message is
         * loaded into a pointer and then subsequently deleted from the internal
         * buffer. Stalls if the CAN is currently handling a message.
         * 
         * @param[out] message    A pointer to an array to populate with the
         *                        message.
         * @param[in] messageSize How large the buffer is. Any output value
         *                        larger than this is truncated.
         * @return Whether a message was extracted from the mailbox. The
         *         applicable return codes are OK (1) and NO_MESSAGE (2).
         */
        result_t getMessage(char *message, int messageSize);

        /**
         * Starts interrupt execution of the private handler function given the  
         * interval.
         * 
         * @param[in] interval Time, in microseconds, between each function
         *                     call.
         */
        void start(const int interval);
        
        /**
         * Stops interrupt execution of the private handler function given the
         * interval. 
         */
        void stop();

        /** Clears out the mailbox. */
        void clear();

    private:
        /**
         * Reads a CANmessage and puts it into the mailbox.
         */
        void handler();

        /**
         * Checks the ID against a list of CAN ids. If it matches we return
         * success. 
         * 
         * @return True if ID matches our list, False otherwise.
         */
        bool checkID(const int id) const; 

    private:

        /** The CAN device used for sending and receiving messages. */
        CAN can;

        /** 
         * The mailbox for receiving messages. They can be picked up
         * asynchronously. This is a circular FIFO.
         */ 
        CANMessage mailbox[CAN_BUS_SIZE];

        /** Lock for the mailbox. I hope you have a key. */
        bool mailboxLock;

        /** Indices for traversing the mailbox. */
        int getIdx;
        int putIdx;

        /** Ticker used to capture messages. */
        Ticker tick;

        /** Whitelist and number of valid IDs in the whitelist. */
        uint32_t whitelistLen;
        uint32_t whitelist[CAN_ID_WHITELIST_LENGTH];
};

