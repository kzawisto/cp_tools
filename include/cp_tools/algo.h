/*
 * algo.h
 *
 *  Created on: Sep 15, 2018
 *      Author: krystian
 */

#ifndef INCLUDE_CP_TOOLS_ALGO_H_
#define INCLUDE_CP_TOOLS_ALGO_H_
#include<type_traits>
#include<iterator>
#include<iostream>
namespace cp_tools {
template<typename T>
struct is_random_access_iterator {
	constexpr static bool value = std::is_same<
			typename std::iterator_traits<T>::iterator_category,
			std::random_access_iterator_tag>::value;
};
template<typename T>
//typename std::enable_if<typename is_random_access_iterator<T>::value, T>::type
auto lower_bound_local(T begin, T end, typename std::iterator_traits<T>::value_type value, T initial_guess)
-> typename std::enable_if<is_random_access_iterator<T>::value, T>::type
{
	T guess = initial_guess;
	typename std::iterator_traits<T>::difference_type stride = 1;

	while(true) {

		if(stride > 0) {
			if(*guess >= value && (guess == begin || *(guess - 1) < value)) {
				return guess;
			}
			else if(*guess < value){
				if(guess == end - 1) {
					return end;
				}
				guess = std::min(guess + stride, end - 1l);
				stride = stride * 2;
			}
			else {
				stride = std::min(-stride / 2, -1l);
				guess = std::max(guess + stride, begin);
			}
		}
		else if(stride < 0) {
			if(*guess >= value && (guess == begin || *(guess - 1) < value)) {
				return guess;
			}
			else if ( *guess < value){
				stride = std::max(-stride / 2, 1l);
				guess = std::min(guess + stride, end - 1l);
			}
			else {
				guess = std::max(guess + stride, begin);
				stride = stride * 2;
			}
		}
	}

	return guess;
}
}


#endif /* INCLUDE_CP_TOOLS_ALGO_H_ */
