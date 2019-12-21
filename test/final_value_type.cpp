#include "AutoDiffCpp/final_value_type.hpp"
#include "AutoDiffCpp/ad.hpp"  // CAVEAT: must be included otherwise
                               // tests fail (incomplete type)

#include <gtest/gtest.h>

using namespace AutoDiffCpp;

TEST(Final_Value_Type, basic)
{
  using T = double;

  ASSERT_TRUE((std::is_same_v<T, AD_Final_Value_Type_t<T>>));
  ASSERT_TRUE((std::is_same_v<T, AD_Final_Value_Type_t<AD<T>>>));
  ASSERT_TRUE((std::is_same_v<T, AD_Final_Value_Type_t<AD_Function<T, 4>>>));
  ASSERT_TRUE((std::is_same_v<T, AD_Final_Value_Type_t<AD_Crtp<T, AD<T>>>>));

  ASSERT_TRUE((std::is_same_v<T, AD_Final_Value_Type_t<AD<AD<T>>>>));
  ASSERT_TRUE((std::is_same_v<T, AD_Final_Value_Type_t<AD_Function<AD<T>, 4>>>));
  ASSERT_TRUE((std::is_same_v<T, AD_Final_Value_Type_t<AD_Crtp<T, AD<AD<T>>>>>));

  ASSERT_TRUE((std::is_same_v<T, AD_Final_Value_Type_t<AD<AD<AD<T>>>>>));
  ASSERT_TRUE((std::is_same_v<T, AD_Final_Value_Type_t<AD_Function<AD<AD<T>>, 4>>>));
  ASSERT_TRUE((std::is_same_v<T, AD_Final_Value_Type_t<AD_Crtp<T, AD<AD<AD<T>>>>>>));
}
