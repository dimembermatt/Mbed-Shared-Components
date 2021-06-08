/**
 * Maximum Power Point Tracker Project
 * 
 * File: CurrentAdcSensor.h
 * Author: Matthew Yu
 * Organization: UT Solar Vehicles Team
 * Created on: September 10th, 2020
 * Last Modified: 06/08/21
 * 
 * File Description: This header file implements the CurrentAdcSensor class,
 * which is derived from the AdcSensor class.
 */
#pragma once
#include <src/AdcSensor/AdcSensor.h>

class CurrentAdcSensor final : public AdcSensor {
    public:
        /**
         * Constructor for a current sensor object.
         * 
         * @param[in] pin pin to attach AnalogIn (sensor ADC pin) to.
         */
        explicit CurrentAdcSensor(const PinName pin) : AdcSensor(pin) {}

    private:
        void handler(void) override {
            if (!mSensorSem.try_acquire()) return;
            float tempData = mSensor.read_voltage();
            /* TODO: insert calibration function here. */
            mFilter->addSample(tempData);
            mSensorValue = mFilter->getResult();
            mSensorSem.release();
        }
};
