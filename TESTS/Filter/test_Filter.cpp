#include "gtest/gtest.h"
#include "Filter/Filter.h"

TEST(FilterTest, initialize) {
    Filter f = Filter();
    EXPECT_EQ(f.getResult(),  0);
}

TEST(FilterTest, read) {
    Filter f = Filter();
    f.addSample(10.0);
    EXPECT_EQ(f.getResult(), 10.0);
}

TEST(FilterTest, readSeries) {
    Filter f = Filter();

    f.addSample(10.0);
    EXPECT_EQ(f.getResult(), 10.0);

    f.addSample(11.0);
    EXPECT_EQ(f.getResult(), 11.0);

    f.addSample(12.0);
    EXPECT_EQ(f.getResult(), 12.0);

    f.addSample(13.0);
    EXPECT_EQ(f.getResult(), 13.0);

    f.addSample(14.0);
    EXPECT_EQ(f.getResult(), 14.0);
}
