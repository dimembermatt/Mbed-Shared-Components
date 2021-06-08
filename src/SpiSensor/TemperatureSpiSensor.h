/**
 * Maximum Power Point Tracker Project
 * 
 * File: TemperatureSpiSensor.h
 * Author: Matthew Yu
 * Organization: UT Solar Vehicles Team
 * Created on: September 10th, 2020
 * Last Modified: 06/08/21
 * 
 * File Description: This header file implements the TemperatureSpiSensor class,
 * which is derived from the SpiSensor class.
 */
#pragma once
#include "SpiSensor.h"

class TemperatureSpiSensor final : public SpiSensor {
    public:
        /**
         * Constructor for a Temperature SPI sensor object.
         * 
         * @param[in] mosi Master Output Slave Input pin.
         * @param[in] miso Master Input Slave Output pin.
         * @param[in] sclk Clock pin.
         */
        explicit TemperatureSpiSensor(
            const PinName mosi, 
            const PinName miso, 
            const PinName sclk) : SpiSensor(mosi, miso, sclk) {}

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
