/**
 * Maximum Power Point Tracker Project
 * 
 * File: VoltageAdcSensor.h
 * Author: Matthew Yu
 * Organization: UT Solar Vehicles Team
 * Created on: September 10th, 2020
 * Last Modified: 06/06/21
 * 
 * File Description: This header file implements the VoltageAdcSensor class,
 * which is derived from the AdcSensor class.
 */
#pragma once
#include <src/AdcSensor/AdcSensor.h>

class VoltageAdcSensor: public AdcSensor{
    public:
        /**
         * Constructor for a voltage sensor object.
         * 
         * @param[in] pin Pin to attach AnalogIn (sensor ADC pin) to.
         */
        explicit VoltageAdcSensor(const PinName pin) : AdcSensor(pin) {}

    private:
        void handler(void) {
            if (!mSensorSem.try_acquire()) return;
            float tempData = mSensor.read_voltage();
            /* TODO: insert calibration function here. */
            mFilter->addSample(tempData);
            mSensorValue = mFilter->getResult();
            mSensorSem.release();
        }
};