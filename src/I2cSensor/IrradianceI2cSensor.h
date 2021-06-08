/**
 * Maximum Power Point Tracker Project
 * 
 * File: IrradianceI2cSensor.h
 * Author: Matthew Yu
 * Organization: UT Solar Vehicles Team
 * Created on: September 10th, 2020
 * Last Modified: 06/08/21
 * 
 * File Description: This header file implements the IrradianceI2cSensor class,
 * which is derived from the I2cSensor class.
 */
#pragma once
#include "I2cSensor.h"

class IrradianceI2cSensor final : public I2cSensor {
    public:
        /**
         * Constructor for a Irradiance I2C sensor object.
         * 
         * @param[in] sda Serial Data pin.
         * @param[in] scl Serial Clock pin.
         */
        explicit IrradianceI2cSensor(
            const PinName sda, 
            const PinName scl) : I2cSensor(sda, scl) {}

    private:
        void handler(void) override {
            if (!mSensorSem.try_acquire()) return;

            /** TODO: Send request to device to ask for data. */

            /** TODO: Capture response and translate. */
            float tempData = 0.0;

            mFilter->addSample(tempData);
            mSensorValue = mFilter->getResult();
            mSensorSem.release();
        }
};
