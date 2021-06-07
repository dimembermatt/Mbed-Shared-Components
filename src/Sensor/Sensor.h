/**
 * Maximum Power Point Tracker Project
 * 
 * File: Sensor.h
 * Author: Matthew Yu
 * Organization: UT Solar Vehicles Team
 * Created on: September 10th, 2020
 * Last Modified: 06/06/21
 * 
 * File Description: Describes the Sensor class, which is an InterruptDevice
 * that reads, filters, and calibrates ADC values for various applications.
 */
#pragma once
#include "mbed.h"
#include <chrono>
#include <src/Filter/Filter.h>
#include <src/InterruptDevice/InterruptDevice.h>

class Sensor: public InterruptDevice {
    public:
        enum FilterType {NONE, EMA, SMA, MEDIAN, KALMAN};

    public:
        /** Constructor for a sensor object. */
        Sensor();
        
        /**
         * Sets the internal filter for the handler operation.
         * 
         * @param[in] filterType The filter being used.
         * @param[in] filter Upcast reference to the filter.
         * @note Deleting the filter will break the sensor if the FilterType is
         * not NONE.
         */
        void setFilter(const enum FilterType filterType, Filter * filter);

        /**
         * Returns the latest value of the sensor, scaled appropriately.
         * 
         * @note This method may stall until the lock on the variable is released, which
         * means the sensor has uploaded the new value into it.
         * @return Sensor value.
         */
        double getValue(void);

        /** Resets the internal filter history and current sensor value. */
        virtual void clearHistory(void) = 0;

    protected:
        /** Reads the sensor value and converts it into something usable. */
        virtual void handler(void) = 0;

    protected:
        /** Reference to the filter to insert data into. */
        Filter * mFilter;
        enum FilterType mFilterType;

        /** Lock to prevent read/modification of shared resources. */
        Semaphore mSensorSem;

        /** Sensor output result value. */
        float mSensorValue;
};
