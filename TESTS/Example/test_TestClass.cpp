#include "gtest/gtest.h"
#include "TestClass.h"

TEST(exampleTest, sayHello) {
    EXPECT_EQ (TestClass::hi(),  2);
}