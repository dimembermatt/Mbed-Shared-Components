/**
 * Project: mbed-shared-components
 * File: Errors.h
 * Author: Matthew Yu (2021).
 * Created on: 06/12/21
 * Last Modified: 06/12/21
 * File Description: This file describes potential error codes for software
 * exceptions in user programs.
 */

/* Standard Error Codes 0 - 255 (2^8). */
#define ERR_NONE                0x00 /* No error has occurred. */
#define ERR_UNKNOWN             0x01 /* Unknown/Undefined error. */
#define ERR_BAD_STATE           0x02 /* Program defined state is not expected. */
#define ERR_INVALID_CAN_ID      0x20 /* CAN ID was never allocated by user. */
#define ERR_INVALID_CAN_DATA    0x21 /* CAN DATA is in a bad format. */
#define ERR_UNEXPECTED_CAN_ID   0x22 /* CAN ID was not expected in context. */

/* Extended Error Codes 256 - 65636 (2^16). */
