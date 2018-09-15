/*
 * test_algo.cxx
 *
 *  Created on: Sep 15, 2018
 *      Author: krystian
 */



#include <cp_tools/algo.h>
#include <gtest/gtest.h>

#include<cp_tools/cpp_utility.h>
#include<iostream>
#include<numeric>
#include<algorithm>
using namespace cp_tools;
template<typename Collection, typename T, typename K>
void test_lower_bound_cache_loc_against_stl(Collection coll, T value, K guess ) {
		auto actual = lower_bound_local(coll.begin(),
				coll.end(), value, coll.begin() + guess) - coll.begin();
		auto expected = lower_bound(coll.begin(), coll.end(), value) - coll.begin();
		ASSERT_EQ(actual,expected) << "collection:" << stringify(coll) << " value: " \
				<< value << " guess:" << guess;
}

TEST(algo, cache_local_lower_bound_correction_no_elem_any_guess) {
	std::vector<int> v{1, 2,4,5,6,8,9};
	for(int j = -3; j < 11; ++j) {
		for(unsigned i = 0; i < v.size(); ++i) {
			test_lower_bound_cache_loc_against_stl(v, j, i);
		}
	}
}

TEST(algo, cache_local_lower_bound_correction_many_elems_any_guess) {
	std::vector<int> v{1, 3, 3, 3, 4, 4,5, 5 ,5 ,6, 6, 6 ,7,7 ,8,9};
	for(int j = -3; j < 11; ++j) {
		for(unsigned i = 0; i < v.size(); ++i) {
			test_lower_bound_cache_loc_against_stl(v, j, i);
		}
	}
}

TEST(algo, cache_local_works_on_c_arr) {

	std::vector<int> coll{1, 3, 3, 3, 4, 4,5, 5 ,5 ,6, 6, 6 ,7,7 ,8,9};
	auto value = 5;
	auto actual = lower_bound_local(coll.data(),
			coll.data() + coll.size(), value, coll.data() + 4) - coll.data();
	auto expected = std::lower_bound(coll.data(), coll.data() + coll.size(), value) - coll.data();
	ASSERT_EQ(actual,expected);
}

