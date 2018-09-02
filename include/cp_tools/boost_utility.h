/*
 * boost_utility.h
 *
 *  Created on: Aug 17, 2018
 *      Author: krystian
 */


#pragma once

#include<boost/property_tree/ptree.hpp>
#include<boost/property_tree/json_parser.hpp>
#include<boost/algorithm/string.hpp>

#include<sstream>
namespace cp_tools {

typedef boost::property_tree::ptree PropTree;
namespace pt = boost::property_tree;

inline
PropTree ptree_from_json(std::string json) {
	PropTree tree;
	std::stringstream ss;
	ss<< json;
	pt::read_json(ss, tree);
	return tree;
}


inline
std::vector<std::string> split_str(std::string text, std::string char_separators) {
	std::vector<std::string> result;
	boost::split(result, text, boost::is_any_of(char_separators.c_str()));
	return result;
}



}

