/**
 * Maximum Power Point Tracker Project
 * 
 * File: I2cSensor.h
 * Author: Matthew Yu
 * Organization: UT Solar Vehicles Team
 * Created on: June 6th, 2021
 * Last Modified: 06/08/21
 * 
 * File Description: Describes the I2cSensor class, which is a derivative of the
 * Sensor class. It utilizes I2C.
 */
#pragma once
#include "mbed.h"
#include <src/Sensor/Sensor.h>

class I2cSensor : public Sensor {
    public:
        /**
         * Constructor for a I2cSensor object.
         * 
         * @param[in] sda Serial Data pin.
         * @param[in] scl Serial Clock pin.
         */
        explicit I2cSensor(const PinName sda, const PinName scl);

        void clearHistory(void) override;

    protected:
        /** Communicates with the I2C device to extract a sensor value. */
        virtual void handler(void) override = 0;

    protected:
        I2C mI2cSensor;
};
