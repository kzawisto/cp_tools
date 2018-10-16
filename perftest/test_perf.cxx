/*
 * test_perf.cxx
 *
 *  Created on: Sep 15, 2018
 *      Author: krystian
 */

#include<gtest/gtest.h>
#include<cp_tools/algo.h>
#include<vector>
#include<algorithm>
#include<cstdlib>
#include<numeric>
TEST(perf_test_lower_bound_rand_lookup, actual) {
	int K  = 100000;
	std::vector<int> data(K);
	std::iota(data.begin(), data.end(), 0);
	for(int i = 0; i < 1000000; ++i) {
		auto result_whatever = cp_tools::lower_bound_local(
				data.begin(), data.end(), rand() % K, data.begin() + rand() % K);
	}
}

TEST(perf_test_lower_bound_rand_lookup, control) {
	int K  = 100000;
	std::vector<int> data(K);
	std::iota(data.begin(), data.end(), 0);
	for(int i = 0; i < 1000000; ++i) {
		auto result_whatever = std::lower_bound(
				data.begin(), data.end(), rand() % K);
	}
}

TEST(perf_test_lower_bound_iterative_lookup, actual) {
	int K  = 1000000;
	std::vector<int> data(K);
	std::iota(data.begin(), data.end(), 0);
	for(int i = 0; i < 1000000; ++i) {
		auto result_whatever = cp_tools::lower_bound_local(
				data.begin(), data.end(), i, data.begin() + std::max(0, std::min(K,i
						+ rand() % 5 - 2)));
	}
}

TEST(perf_test_lower_bound_iterative_lookup, control) {
	int K  = 1000000;
	std::vector<int> data(K);
	std::iota(data.begin(), data.end(), 0);
	for(int i = 0; i < 1000000; ++i) {
		auto result_whatever = std::lower_bound(
				data.begin(), data.end(), i);
	}
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
