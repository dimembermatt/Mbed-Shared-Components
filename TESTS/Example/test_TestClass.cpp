#include "gtest/gtest.h"
#include "Example/TestClass.h"

TEST(exampleTest, sayHello) {
    EXPECT_EQ (TestClass::hi(),  2);
}