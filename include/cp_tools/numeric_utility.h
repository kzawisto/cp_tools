#pragma once

#include<deque>
#include<type_traits>
#include<vector>
#include<stdexcept>
namespace cp_tools {
inline
double get_mean(const std::vector<double> & array) {
	double sum = 0;
	for(unsigned i = 0;i < array.size(); i++) sum += array[i];
	return sum / array.size();
}
template<typename T>
struct deref {
	typedef std::remove_const_t<std::remove_reference_t<T>> type;
};

template< class T >
using deref_t = typename deref<T>::type;

template<typename T>
auto get_mean(T begin, T end) -> deref_t<decltype(*begin)> {
	deref_t<decltype(*begin)> sum = 0;
	unsigned count = 0;
	for(auto it = begin; it != end; ++it) {
		sum += *it;
		count++;
	}
	return sum / count;
}

template<typename T>
struct MeanAndVariance {
	T mean = T(0), variance= T(0);
	int count = T(0);
};

template<typename T>
auto get_mean_and_variance(T begin, T end) -> MeanAndVariance<
deref_t<decltype(*begin)>> {
	typedef deref_t<decltype(*begin)> Value;
	Value mean_value = get_mean(begin, end), variance = 0;
	int count = 0;
	for(auto it = begin; it != end; ++it) {
		variance += (*it - mean_value) * (*it - mean_value);
		count++;
	}
	return MeanAndVariance<Value>{mean_value, variance / (count - 1), count};
}

template<typename T>
class RollingMeanAndVariance {
public:
	MeanAndVariance<T> mean_var;
	std::deque<T> array;
	RollingMeanAndVariance() {

	}
	RollingMeanAndVariance(std::initializer_list<T> initial_values) {
		mean_var = get_mean_and_variance(initial_values.begin(), initial_values.end());
	}
private:
	void add_value(T a) {
		if(mean_var.count ==0) {
			mean_var.mean  =a;
			mean_var.count = 1;
		}
		else if(mean_var.count == 1) {
			mean_var = get_mean_and_variance(array.begin(), array.end());
		}
		else {
			mean_var = with_added_value(a);
		}
	}
public:
	void push_back(T a) {
		array.push_back(a);
		add_value(a);
	}
	void push_front(T a) {
		array.push_front(a);
		add_value(a);
	}

public:
	void pop_front() {
		double a = array.front();
		array.pop_front();
		mean_var = with_removed_value(a);
	}
	void pop_back() {
		double a = array.back();
		array.pop_back();
		mean_var = with_removed_value(a);
	}
	 MeanAndVariance<T> with_removed_value(T a) const{
		if(mean_var.count <=2) {
			if(mean_var.count == 0)
				throw std::logic_error("cant pop from empty");
			else if(mean_var.count == 1) {
					return MeanAndVariance<T>{(T)0,(T)0,0};
			}
			else {
					return MeanAndVariance<T>{
						(T)2*mean_var.mean - a
						,(T)0,1};
			}
		} else {
			double new_mean = (mean_var.count * mean_var.mean - a) / (mean_var.count - (T)1.0);
			double new_variance = ((mean_var.count - T(1)) * mean_var.variance -  (a - new_mean) * (a - mean_var.mean)
					) / (mean_var.count - (T)2) ;
			return MeanAndVariance<T> {new_mean, new_variance, mean_var.count-1};
		}


	}
	MeanAndVariance<T> with_added_value(T a) const {
		MeanAndVariance<T> new_mean_var{mean_var};
		new_mean_var.mean = mean_var.mean + (a - mean_var.mean) / (mean_var.count + (T)1);
		new_mean_var.variance = mean_var.variance + (
				(a - mean_var.mean) * (a - new_mean_var.mean) - mean_var.variance
		) / (mean_var.count );
		new_mean_var.count = mean_var.count + 1;
		return new_mean_var;
	}


};

}
