/**
 * File: Message.h
 * Author: Matthew Yu (2021).
 * Organization: UT Solar Vehicles Team
 * Created on: May 24th, 2021.
 * Last Modified: 05/24/21
 * 
 * File Description: This header file defines a message class, which can be
 * translated into other types of messages used for communication.
 * 
 * Message <-----> CANMessage
 *            |
 *            |--> Serial Message ("id:<ID>;data:<DATA>;")
 *            |
 *            | ... other message types
 */
#pragma once
#include <stdint.h>

#define MAX_BYTES 8

/**
 * A Message class instance is a translatable message which acts as a middle man
 * between message types like CANMessages and Serial messages. It has the
 * following traits:
 * - A message ID field, identifying the source and/or type of message.
 * - A message DATA field, which contains the actual data.
 * - A message DATATYPE field, which describes the type of data inside.
 * It is the responsibility of the translating classes (i.e., CANMessages) to
 *   implement the methods to convert to and from a Message instance.
 */
class Message {
    public:
        enum MessageDataType {UINT64, INT64, CHAR};

    public:
        /** Message constructor for unsigned 64 bit integer data. */
        explicit Message(const uint16_t id, const uint64_t data);

        /** Message constructor for signed 64 bit integer data. */
        explicit Message(const uint16_t id, const int64_t data);

        /** Message constructor for 8 byte character string data. */
        explicit Message(const uint16_t id, char* data, const uint32_t len);

        /** Getters. */
        uint16_t getMessageID(void) const;
        uint64_t getMessageDataU(void) const;
        int64_t getMessageDataS(void) const;
        void getMessageDataC(char* data) const;
        enum MessageDataType getMessageDataType(void) const;

        /* Setters. */
        void setMessageID(const uint16_t id);
        void setMessageDataU(const uint64_t data);
        void setMessageDataS(const int64_t data);
        void setMessageDataC(char* data, const uint32_t len);

        /**
         * toString Stringifies the ID and DATA into a human readable encoding.
         * 
         * @param[out] data Pointer to a char array to fill.
         * @param[in] len Length of the char array to fill.
         * @return True if the array was filled successfully, false if overflow.
         */
        bool toString(char* data, const uint32_t len) const;

        /**
         * encode encodes the ID and DATA into a machine readable encoding.
         * 
         * @param[out] data Pointer to a char array to fill.
         * @param[in] len Length of the char array to fill.
         * @return True if the array was filled successfully, false if overflow.
         */
        bool encode(char* data, const uint32_t len) const;

    private:
        /** Message ID. */
        uint16_t id;

        /** Message DATA. */
        union {
            uint64_t uint64;
            int64_t int64;
            char charArr[MAX_BYTES];
        } data;

        /** Expected message type. */
        enum MessageDataType datatype;
};
