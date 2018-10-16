/*
 * cpp_utility.h
 *
 *  Created on: Aug 17, 2018
 *      Author: krystian
 */

#pragma once

#include <set>
#include <sstream>
#include <unordered_map>
#include <vector>

namespace cp_tools {

template <typename T> int sgn(T val) { return (T(0) < val) - (val < T(0)); }

template <typename T>
std::ostream &operator<<(std::ostream &op, const std::vector<T> &vec) {
  op << "[";
  for (const auto &t : vec) {
    op << t << ",";
  }
  op << "]";
  return op;
}

template <typename T, typename K>
std::ostream &operator<<(std::ostream &op,
                         const std::unordered_map<T, K> &vec) {
  op << "<";
  for (const auto &t : vec) {
    op << t.first << "->" << t.second << ", ";
  }
  op << ">";
  return op;
}
inline std::string token_at(const std::string &line, int i, char sep = '\001') {
  int j = i;
  for (; j < i + 20; ++j) {
    if (line.at(j) == sep) {
      break;
    }
  }
  return line.substr(i, j - i);
}

template <typename T> std::string stringify(T arg) {
  std::stringstream ss;
  ss << arg;
  return ss.str();
}

template <typename... Args, typename T>
std::string stringify(T arg, Args... args) {
  return stringify(arg) + stringify(args...);
}

template <typename T> std::string stringifySep(std::string sep, T arg) {
  return stringify(arg);
}

template <typename... Args, typename T>
std::string stringifySep(std::string sep, T arg, Args... args) {
  return stringify(arg) + sep + stringifySep(sep, args...);
}

template <typename T, typename K>
std::ostream &operator<<(std::ostream &os, const std::multiset<T, K> &s) {
  os << "{{";
  if (!s.empty()) {
    os << *s.begin();
  }
  for (auto it = ++s.begin(); it != s.end(); ++it) {
    os << "," << *it;
  }
  os << "}}";
  return os;
}

template <typename T> void cumsum_inplace(T begin, T end) {
  auto it = begin;
  auto next_it = std::next(begin);
  while (next_it != end) {
    *next_it = *next_it + *it;
    it++;
    next_it++;
  }
}

template <typename T> std::string stringify_collection(T begin, T end) {
  std::stringstream ss;
  auto it = begin;
  ss << "{";
  if (it != end) {
    ss << *it;
    it++;
    for (; it != end; it++) {
      ss << "," << *it;
    }
  }
  ss << "}";
  return ss.str();
}
}
