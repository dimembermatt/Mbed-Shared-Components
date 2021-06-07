/**
 * Maximum Power Point Tracker Project
 * 
 * File: Sensor.cpp
 * Author: Matthew Yu
 * Organization: UT Solar Vehicles Team
 * Created on: September 10th, 2020
 * Last Modified: 06/06/21
 * 
 * File Description: This file implements functions defined for the Sensor
 * class.
 */
#include "Sensor.h"

Sensor::Sensor() {
    mSensorValue = 0.0;
    mFilterType = FilterType::NONE;
}

void Sensor::setFilter(const enum FilterType filterType, Filter * filter) {
    mFilterType = filterType;
    mFilter = filter;
}

double Sensor::getValue(void) {
    mSensorSem.acquire();
    double sensorValue = mSensorValue;
    mSensorSem.release();
    return sensorValue;
}
