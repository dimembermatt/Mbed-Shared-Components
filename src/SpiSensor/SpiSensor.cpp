/**
 * Maximum Power Point Tracker Project
 * 
 * File: SpiSensor.cpp
 * Author: Matthew Yu
 * Organization: UT Solar Vehicles Team
 * Created on: June 6th, 2021
 * Last Modified: 06/06/21
 * 
 * File Description: Describes the SpiSensor class, which is a derivative of the
 * Sensor class. It utilizes SPI.
 */
#include "SpiSensor.h"

SpiSensor::SpiSensor(
    const PinName mosi, 
    const PinName miso, 
    const PinName sclk) : mSpiSensor(mosi, miso, sclk) {}

void SpiSensor::clearHistory(void) {
    mFilter->clear();
    mSensorValue = 0;
}
