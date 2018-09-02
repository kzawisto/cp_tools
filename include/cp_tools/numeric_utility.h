/*
 * numeric_utility.h
 *
 *  Created on: Aug 17, 2018
 *      Author: krystian
 */

#pragma once


#include<type_traits>

namespace cp_tools {

template<typename T>
auto get_mean(T begin, T end) -> typename std::remove_reference_t<decltype(*begin)> {
	typename std::remove_reference_t<decltype(*begin)> sum = 0;
	unsigned count = 0;
	for(auto it = begin; it != end; ++it) {
		sum += *it;
		count++;
	}
	return sum / count;
}

template<typename T>
struct MeanAndVariance {
	T mean, variance, count;
};

template<typename T>
auto get_mean_and_variance(T begin, T end) -> MeanAndVariance<typename std::remove_reference_t<decltype(*begin)>> {
	typedef std::remove_reference_t<decltype(*begin)> Value;
	Value mean_value = get_mean(begin, end), variance = 0;
	int count = 0;
	for(auto it = begin; it != end; ++it) {
		variance += (*it - mean_value) * (*it - mean_value);
		count++;
	}
	return MeanAndVariance<Value>{mean_value, variance / (count - 1), count};
}

}

