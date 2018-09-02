/*
 * eigen_utility.h
 *
 *  Created on: Aug 17, 2018
 *      Author: krystian
 */


#pragma once
#include<fstream>
#include<eigen3/Eigen/Dense>
#include<boost/random.hpp>


namespace cp_tools {

namespace Ei=Eigen;

inline
void cumsum_inplace (Ei::VectorXd & v){
	for(int i = 1; i < v.size(); ++i){
		v[i] += v(i-1);
	}
}
void binary_serialize_vecd(Ei::VectorXd vec, std::string file) {
	std::fstream f(file, std::ios::out| std::ios::binary);
	if(f.good()) {
		f.write(
		(const char *)vec.data(), vec.size() * 8);
		f.close();
	}
}

void binary_deserialize_vecd(std::string file) {
	std::fstream f(file, std::ios::out| std::ios::binary);
}


inline
Ei::VectorXd random_normal(int size, int seed = 1) {
	Ei::VectorXd result(size);
	boost::random::mt19937 rng(seed);
	boost::random::normal_distribution<> nd;
	boost::variate_generator<boost::mt19937&, boost::normal_distribution<> > var_nor(rng, nd);
	for(int i =0; i < size;++i) {
		result[i] = var_nor();
	}
	return result;
}

inline
Ei::VectorXd cumsum(const Ei::VectorXd & v) {
	Ei::VectorXd result(v);
	cumsum_inplace(result);
	return result;
}


}
