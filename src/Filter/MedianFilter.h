/**
 * Maximum Power Point Tracker Project
 * 
 * File: MedianFilter.h
 * Author: Matthew Yu
 * Organization: UT Solar Vehicles Team
 * Created on: September 19th, 2020
 * Last Modified: 06/06/21
 * 
 * File Description: This header file implements the MedianFilter class, which
 * is a derived class from the parent Filter class.
 */
#pragma once
#include "Filter.h"
#include <new>
#include <limits>
#include <bits/stdc++.h>
#include <cmath>
#include <stdio.h>

class MedianFilter: public Filter {
    public:
        /** Default constructor for a MedianFilter object. 10 sample size. */
        MedianFilter(void) : Filter(10) {
            mDataBuffer = new double[mMaxSamples];
            mIdx = 0;
            mNumSamples = 0;
        }

        /**
         * Constructor for a MedianFilter object.
         * 
         * @param[in] maxSamples Number of samples that the filter should 
         *      hold at maximum at any one time.
         * @precondition maxSamples is a positive number.
         */
        MedianFilter(const int maxSamples) : Filter(maxSamples) {
            mDataBuffer = new double[mMaxSamples];
            mIdx = 0;
            mNumSamples = 0;
        }

        /**
         * Adds a sample to the filter and updates calculations.
         * 
         * @param[in] val Input value to calculate filter with.
         */
        void addSample(const double sample) { 
            // check for exception
            if (mDataBuffer == nullptr) { return; }
            
            // saturate counter at max samples
            if (mNumSamples < mMaxSamples) {
                mNumSamples ++;
            }
        
            mDataBuffer[mIdx] = sample;
            mIdx = (mIdx + 1) % mMaxSamples;
        }

        /**
         * Returns the filtered result of the input data.
         * 
         * @return Filter output.
         */
        float getResult() const { 
            /* Check for exception. */
            if (mDataBuffer == nullptr) { return 0.0; }

            /* Get the range window. */
            int startIdx = (mIdx - mNumSamples + mMaxSamples) % mMaxSamples;

            /* Find the median from that range window. */
            return getMedian(startIdx);
        }

        void clear(void) {
            mNumSamples = 0;
            mIdx = 0;
        }

        /**
         * Deallocates constructs in the filter for shutdown.
         */
        void shutdown() { delete[] mDataBuffer; }

    private:
        /**
         * Returns the median of the data buffer.
         * 
         * @param[in] startIdx Start index of the data buffer.
         * @return Median of the data buffer.
         */
        float getMedian(const int startIdx) const {
            /* Naive solution is to sort the data and pick the n/2 index. */
            double * tempBuffer = new  float[mNumSamples];
            if (tempBuffer != nullptr) {
                for (int i = 0; i < mNumSamples; i++) {
                    tempBuffer[i] = mDataBuffer[(i + startIdx) % mMaxSamples];
                }
                
                /* Sort the buffer. */
                std::sort(tempBuffer, tempBuffer + mNumSamples);
                
                /* Get the correct index value. */
                float val = 0.0;
                if (mNumSamples == 0) { 
                    return 0.0;
                } else if (mNumSamples%2 == 0) {
                    /* Even, split the median between two values. */
                    val = (tempBuffer[mNumSamples/2] + tempBuffer[mNumSamples/2 - 1]) / 2.0;
                } else {
                    val = tempBuffer[(int) floor(mNumSamples/2)];
                }
                
                delete[] tempBuffer;
                return val;
            } else {
                return 0.0;
            }
        }

    private:
        /** Data Buffer.  */
        double * mDataBuffer;

        /** Number of samples in the buffer. */
        int mNumSamples;

        /** Current index in the buffer. */
        int mIdx;
};
