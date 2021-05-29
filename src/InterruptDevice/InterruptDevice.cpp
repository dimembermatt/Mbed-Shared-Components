/**
 * File: InterruptDevice.cpp
 * Author: Matthew Yu (2021).
 * Organization: UT Solar Vehicles Team
 * Created on: May 24th, 2021.
 * Last Modified: 05/24/21
 * 
 * File Description: This implementation file defines a base concrete InterruptDevice
 * class, which exposes a common API for devices that need to run on reoccurring
 * timelines. All devices using this class (or the Ticker API) have the same
 * priority.
 */
#include <chrono>
#include "InterruptDevice.h"


InterruptDevice::InterruptDevice(void) {}

void InterruptDevice::start_us(const int interval) {
    ticker.attach(callback(this, &InterruptDevice::handler),
        std::chrono::microseconds(interval));
}

void InterruptDevice::start_ms(const int interval) {
    ticker.attach(callback(this, &InterruptDevice::handler),
        std::chrono::milliseconds(interval));
}

void InterruptDevice::stop(void) { ticker.detach(); }
