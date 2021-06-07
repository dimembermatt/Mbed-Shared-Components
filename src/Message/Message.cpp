/**
 * File: message.cpp
 * Author: Matthew Yu (2021).
 * Organization: UT Solar Vehicles Team
 * Created on: May 24th, 2021.
 * Last Modified: 05/24/21
 * 
 * File Description: This implementation file defines a message class, which can
 * be translated into other types of messages used for communication.
 * 
 * Message <-----> CANMessage
 *            |
 *            |--> Serial Message ("id:<ID>;data:<DATA>;")
 *            |
 *            | ... other message types
 */
#include <src/Message/Message.h>
#include <stdlib.h>

Message::Message(void) {
    mId = 0;
    mData.uint64 = 0;
    mDatatype = CHAR;
}

Message::Message(const uint16_t id, const uint64_t data) {
    mId = id;
    mData.uint64 = data;
    mDatatype = UINT64;
}

Message::Message(const uint16_t id, const int64_t data) {
    mId = id;
    mData.int64 = data;
    mDatatype = INT64;
}

Message::Message(const uint16_t id, const char* data, const uint32_t len) {
    mId = id;
    uint32_t width = (MAX_BYTES < len) ? MAX_BYTES : len;
    for (uint8_t i = 0; i < width; ++i) {
        mData.charArr[i] = data[i];
    }
    mDatatype = CHAR;
}

uint16_t Message::getMessageID(void)    const { return mId; }

uint64_t Message::getMessageDataU(void) const { return mData.uint64; }

int64_t Message::getMessageDataS(void)  const { return mData.int64; }

void Message::getMessageDataC(char* data, const uint32_t len) const {
    uint32_t width = (MAX_BYTES < len) ? MAX_BYTES : len;
    for (uint8_t i = 0; i < width; ++i) {
        data[i] = mData.charArr[i];
    }
}

enum Message::MessageDataType Message::getMessageDataType(void) const { 
    return mDatatype; 
}

void Message::setMessageID(const uint16_t id) { 
    mId = id;
}

void Message::setMessageDataU(const uint64_t data) { 
    mData.uint64 = data;
}

void Message::setMessageDataS(const int64_t data) { 
    mData.int64 = data; 
}

void Message::setMessageDataC(const char* data, const uint32_t len) {
    uint32_t width = (MAX_BYTES < len) ? MAX_BYTES : len;
    for (uint32_t i = 0; i < width; ++i) {
        mData.charArr[i] = data[i];
    }
}

bool Message::toString(char* data, const uint32_t len) const {
    /* TODO: Encode in the format "id:<ID>;data:<DATA>;". */
    uint32_t idx = 0;
    while (idx < len) {
        /* Insert chars... If complete return true. */
    }
    return false;
}

bool Message::encode(char* data, const uint32_t len) const {
    /* Encode in the format [ID][DATA] where ID is 4 bytes and DATA is 8 bytes. */
    #define DATA_BYTE_SIZE 8
    bool partiallyFilled = len < DATA_BYTE_SIZE;

    for (uint32_t i = 0; i < (partiallyFilled ? len : DATA_BYTE_SIZE); ++i) {
        // TODO:
    }

    return partiallyFilled;

    #undef DATA_BYTE_SIZE
}
