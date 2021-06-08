/**
 * File: message.cpp
 * Author: Matthew Yu (2021).
 * Organization: UT Solar Vehicles Team
 * Created on: May 24th, 2021.
 * Last Modified: 06/08/21
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
#include <stdio.h>

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

Message::Message(const uint16_t id, const char* data, const uint16_t len) {
    mId = id;
    uint16_t width = (MESSAGE_MAX_BYTES < len) ? MESSAGE_MAX_BYTES : len;
    for (uint8_t i = 0; i < width; ++i) {
        mData.charArr[i] = data[i];
    }
    mDatatype = CHAR;
}

uint16_t Message::getMessageID(void)    const { return mId; }

uint64_t Message::getMessageDataU(void) const { return mData.uint64; }

int64_t Message::getMessageDataS(void)  const { return mData.int64; }

void Message::getMessageDataC(char* data, const uint16_t len) const {
    uint16_t width = (MESSAGE_MAX_BYTES < len) ? MESSAGE_MAX_BYTES : len;
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

void Message::setMessageDataC(const char* data, const uint16_t len) {
    uint16_t width = (MESSAGE_MAX_BYTES < len) ? MESSAGE_MAX_BYTES : len;
    for (uint16_t i = 0; i < width; ++i) {
        mData.charArr[i] = data[i];
    }
}

bool Message::toString(char* data, const uint16_t len) const {
    /* toString in the format id:<ID>;data:<DATA>;. */
    #define MAX_BUFFER_SIZE 30
    char buffer[MAX_BUFFER_SIZE];

    /* Needs to be 0xXX, 0xYY format. */
    /* TODO: replace '0x' with more pretty '#' without sprintf ignoring it. */
    uint16_t length = sprintf(buffer, "id:0x%x;data:0x%llx;", mId, mData.uint64);
    if (length < 0 || length > len) return false;
    else {
        for (uint16_t i = 0; i < length; ++i) {
            data[i] = buffer[i];
        }
        return true;
    }
    #undef MAX_BUFFER_SIZE
}

bool Message::encode(char* data, const uint16_t len) const {
    /* Encode in the format <ID><DATA> where ID is 4 bytes and DATA is 8 bytes. */
    #define MESSAGE_ENCODE_SIZE 12
    #define ID_BYTE_SIZE 4
    #define DATA_BYTE_SIZE 8

    char buffer[MESSAGE_ENCODE_SIZE+1];
    /* Needs to be in XXXX_YYYY_YYYY format. */
    /* TODO: figure out how to use preprocessor definitions without making the result
     * disregard symbols and throw out something like: '%0*x%0*llx'. */
    uint16_t length = sprintf(
        buffer, 
        "%04x%08llx", 
        mId, 
        mData.uint64);
    if (length < 0 || length > len) return false;
    else {
        for (uint16_t i = 0; i < length; ++i) {
            data[i] = buffer[i];
        }
        return true;
    }

    #undef MESSAGE_ENCODE_SIZE
    #undef ID_BYTE_SIZE
    #undef DATA_BYTE_SIZE
}
