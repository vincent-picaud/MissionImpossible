#include "AutoDiffCpp/hello.hpp"
#include <gtest/gtest.h>

using namespace AutoDiffCpp;

TEST(Hello, demo)
{
  ASSERT_EQ(hello(),"Hello AutoDiffCpp");
}
