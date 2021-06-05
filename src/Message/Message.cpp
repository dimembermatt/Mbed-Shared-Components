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

Message::Message(void) {
    id = 0;
    data.uint64 = 0;
    datatype = CHAR;
}

Message::Message(const uint16_t _id, const uint64_t _data) {
    id = _id;
    data.uint64 = _data;
    datatype = UINT64;
}

Message::Message(const uint16_t _id, const int64_t _data) {
    id = _id;
    data.int64 = _data;
    datatype = INT64;
}

Message::Message(const uint16_t _id, char* _data, uint32_t _len) {
    id = _id;
    uint8_t len = MAX_BYTES < _len ? MAX_BYTES : _len;
    for (uint8_t i = 0; i < len; ++i)
        data.charArr[i] = _data[i];
    datatype = CHAR;
}

uint16_t Message::getMessageID(void)    const { return id; }

uint64_t Message::getMessageDataU(void) const { return data.uint64; }

int64_t Message::getMessageDataS(void)  const { return data.int64; }

void Message::getMessageDataC(char* _data) const {
    for (uint8_t i = 0; i < 8; ++i)
        _data[i] = data.charArr[i];
}

enum Message::MessageDataType Message::getMessageDataType(void) const { return datatype; }


void Message::setMessageID(const uint16_t _id) { 
    id = _id;
}

void Message::setMessageDataU(const uint64_t _data) { 
    data.uint64 = _data;
}

void Message::setMessageDataS(const int64_t _data) { 
    data.int64 = _data; 
}

void Message::setMessageDataC(char* _data, const uint32_t len) {
    uint32_t width = (MAX_BYTES < len) ? MAX_BYTES : len;
    for (uint32_t i = 0; i < width; ++i) {
        data.charArr[i] = _data[i];
    }
}

bool Message::toString(char* _data, const uint32_t len) const {
    /* TODO: Encode in the format "id:<ID>;data:<DATA>;". */
    return false;
}

bool Message::encode(char* data, const uint32_t len) const {
    /* TODO: Encode in the format [ID][DATA] where ID is 3 bytes and 
       DATA is 8 bytes. */
    return false;
}
