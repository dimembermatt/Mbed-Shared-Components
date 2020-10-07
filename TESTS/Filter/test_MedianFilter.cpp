#include "gtest/gtest.h"
#include "Filter/MedianFilter.h"

TEST(MedianFilterTest, initialize) {
    MedianFilter m = MedianFilter();
    EXPECT_EQ(m.getResult(),  0);
    m.shutdown();
}

TEST(MedianFilterTest, read) {
    MedianFilter m = MedianFilter();
    m.addSample(10.0);
    EXPECT_EQ(m.getResult(), 10.0);
    m.shutdown();
}

TEST(MedianFilterTest, readSeries) {
    int expected_res[20] = {
        100,
        55,
        20,
        25,
        30,
        30,
        40,
        60,
        70,
        80,
        80,
        90,
        100,
        110,
        120,
        120,
        130,
        140,
        160,
        170
    };

    MedianFilter m = MedianFilter(5); // 5 sample buffer
    // add 20 samples, increasing linearly by 10, and then some noisy 100s every 5 cycles.
    for (int i = 0; i < 20; i++) {
        if (i%5 == 0) { m.addSample(100); }
        else { m.addSample(i*10.0); }

        // assert the expected filter output at every point
        EXPECT_EQ(m.getResult(), expected_res[i]);
    }

    // shutdown
    m.shutdown();
}
