/**
 * File: InterruptDevice.h
 * Author: Matthew Yu (2021).
 * Organization: UT Solar Vehicles Team
 * Created on: June 5th, 2021.
 * Last Modified: 06/06/21
 * 
 * File Description: This header file defines a base concrete InterruptDevice
 * class, which exposes a common API for devices that need to run on reoccurring
 * timelines. All devices using this class (or the Ticker API) have the same
 * priority.
 */
#pragma once
#include <mbed.h>

class InterruptDevice {
    public:
        /** Constructor for an InterruptDevice. */
        InterruptDevice(void);

        /**
         * Starts interrupt execution of the private handler function given the
         * interval.
         * 
         * @param[in] interval Time, in microseconds, between each function
         *                     call.
         */
        void startUs(const int interval);

        /**
         * Starts interrupt execution of the private handler function given the
         * interval.
         * 
         * @param[in] interval Time, in milliseconds, between each function
         *                     call.
         */
        void startMs(const int interval);

        /**
         * Stops interrupt execution of the private handler function given the
         * interval.
         */
        void stop();

    protected:
        /** Performs an action defined by the child class. */
        void handler(void);

    protected:
        Ticker ticker;
};
