/*
 * test_numeric.cxx
 *
 *  Created on: Aug 22, 2018
 *      Author: krystian
 */

#include <cp_tools/numeric_utility.h>
#include <gtest/gtest.h>

using namespace cp_tools;
TEST(rolling_mean_and_variance, should_compute_mean_and_variance_in_init) {
  RollingMeanAndVariance<double> mean_var({1.0, 2.0, 3.0});
  ASSERT_DOUBLE_EQ(mean_var.mean_var.mean, 2.);
  ASSERT_NEAR(mean_var.mean_var.variance, 1.0, 1e-6);
}

TEST(rolling_mean_and_variance,
     should_compute_mean_and_variance_for_added_value) {
  RollingMeanAndVariance<double> mean_var({1.0, 2.0, 3.0});
  auto new_mv = mean_var.with_added_value(4.0);
  ASSERT_DOUBLE_EQ(new_mv.mean, 2.5);
  ASSERT_NEAR(new_mv.variance, 1.0 + 2.0 / 3.0, 1e-6);
}

TEST(rolling_mean_and_variance,
     should_compute_mean_and_variance_for_removed_value) {
  RollingMeanAndVariance<double> mean_var({1.0, 2.0, 3.0, 4.0});
  auto new_mv = mean_var.with_removed_value(4.0);
  ASSERT_DOUBLE_EQ(new_mv.mean, 2.0);
  ASSERT_NEAR(new_mv.variance, 1.0, 1e-6);
}

TEST(rolling_mean_and_variance,
     should_produce_same_result_as_two_pass_when_appending) {
  std::vector<double> vec;
  RollingMeanAndVariance<double> mean_var;
  srand(4);
  for (int i = 0; i < 1000; i++) {
    vec.push_back(rand() % 10000 / 10000.0);
    mean_var.push_back(vec.back());
    if (i > 1) {
      auto expected = get_mean_and_variance(vec.begin(), vec.end());
      auto actual = mean_var.mean_var;
      ASSERT_NEAR(actual.mean, expected.mean, 1e-6);
      ASSERT_NEAR(actual.variance, expected.variance, 1e-6);
    }
  }
}
TEST(rolling_mean_and_variance,
     should_produce_same_result_as_two_pass_for_rolling_forward) {
  std::deque<double> vec;
  RollingMeanAndVariance<double> mean_var;
  srand(7);
  for (int i = 0; i < 1000; i++) {
    vec.push_back(rand() % 10000 / 10000.0);
    mean_var.push_back(vec.back());
    if (i > 5) {
      auto expected = get_mean_and_variance(vec.begin(), vec.end());
      auto actual = mean_var.mean_var;
      ASSERT_NEAR(actual.mean, expected.mean, 1e-6);
      ASSERT_NEAR(actual.variance, expected.variance, 1e-6);
      vec.pop_front();
      mean_var.pop_front();
    }
  }
}
TEST(rolling_mean_and_variance,
     should_produce_same_result_as_two_pass_for_rolling_backward) {
  std::deque<double> vec;
  RollingMeanAndVariance<double> mean_var;
  srand(7);
  for (int i = 0; i < 1000; i++) {
    vec.push_front(rand() % 10000 / 10000.0);
    mean_var.push_front(vec.front());
    if (i > 1) {
      auto expected = get_mean_and_variance(vec.begin(), vec.end());
      auto actual = mean_var.mean_var;
      ASSERT_NEAR(actual.mean, expected.mean, 1e-6);
      ASSERT_NEAR(actual.variance, expected.variance, 1e-6);
      vec.pop_back();
      mean_var.pop_back();
    }
  }
}
