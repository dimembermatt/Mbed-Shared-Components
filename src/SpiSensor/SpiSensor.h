/**
 * Maximum Power Point Tracker Project
 * 
 * File: SpiSensor.h
 * Author: Matthew Yu
 * Organization: UT Solar Vehicles Team
 * Created on: June 6th, 2021
 * Last Modified: 06/06/21
 * 
 * File Description: Describes the SpiSensor class, which is a derivative of the
 * Sensor class. It utilizes SPI.
 */
#pragma once
#include "mbed.h"
#include <src/Sensor/Sensor.h>

class SpiSensor : public Sensor {
    public:
        /**
         * Constructor for a SpiSensor object.
         * 
         * @param[in] mosi Master Output Slave Input pin.
         * @param[in] miso Master Input Slave Output pin.
         * @param[in] sclk Clock pin.
         */
        explicit SpiSensor(const PinName mosi, const PinName miso, const PinName sclk);

        void clearHistory(void) override;

    private:
        /** Communicates with the SPI device to extract a sensor value. */
        virtual void handler(void) override = 0;

    private:
        SPI mSpiSensor;
};
