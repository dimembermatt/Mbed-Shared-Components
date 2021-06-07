/**
 * Maximum Power Point Tracker Project
 * 
 * File: I2cSensor.cpp
 * Author: Matthew Yu
 * Organization: UT Solar Vehicles Team
 * Created on: June 6th, 2021
 * Last Modified: 06/06/21
 * 
 * File Description: Describes the I2cSensor class, which is a derivative of the
 * Sensor class. It utilizes I2C.
 */
#include "I2cSensor.h"

I2cSensor::SeI2cSensornsor(const PinName sda, const PinName scl) {
    mI2cSensor = I2C(sda, scl);
}

void I2cSensor::clearHistory(void) {
    mFilter.clear();
    mSensorValue = 0;
}
