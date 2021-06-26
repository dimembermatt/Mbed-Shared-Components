/**
 * Maximum Power Point Tracker Project
 * 
 * File: AdcSensor.h
 * Author: Matthew Yu
 * Organization: UT Solar Vehicles Team
 * Created on: June 6th, 2021
 * Last Modified: 06/08/21
 * 
 * File Description: Describes the AdcSensor class, which is a derivative of the
 * Sensor class. It utilizes AnalogIn.
 */
#pragma once
#include "mbed.h"
#include <src/Sensor/Sensor.h>

class AdcSensor : public Sensor {
    public:
        /** 
         * Constructor for an AdcSensor object. 
         * 
         * @param[in] pin Pin to attach AnalogPin (sensor ADC pin) to.
         */
        AdcSensor(const PinName pin);

        void clearHistory(void) override;

    protected:
        /** Reads the sensor ADC value and converts it into something usable. */
        void handler(void) override;

    protected:
        AnalogIn mSensor;
};
