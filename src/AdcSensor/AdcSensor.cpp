/**
 * Maximum Power Point Tracker Project
 * 
 * File: AdcSensor.cpp
 * Author: Matthew Yu
 * Organization: UT Solar Vehicles Team
 * Created on: June 6th, 2021
 * Last Modified: 06/06/21
 * 
 * File Description: Describes the AdcSensor class, which is a derivative of the
 * Sensor class. It utilizes AnalogIn.
 */
#include "AdcSensor.h"

AdcSensor::AdcSensor(const PinName pin) {
    mSensor = AnalogIn(pin);
}

void AdcSensor::clearHistory(void) {
    mFilter.clear();
    mSensorValue = 0;
}

void AdcSensor::handler(void) {
    if (!mSensorSem.try_acquire()) return;
    filter.addSample(mSensor.read_voltage());
    mSensorValue = filter.getResult();
    mSensorSem.release();
}
