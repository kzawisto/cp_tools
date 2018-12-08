#pragma once
#include <iostream>
#include <sstream>

#define ASSERT_EQUALS_STR(stuff, stuff2)                                       \
  if (stuff != stuff2)                                                         \
    throw std::logic_error(std::string(#stuff " == " #stuff2 "  ") + stuff +   \
                           " != " + stuff2);

template <typename> struct type_long {
  typedef long type;
  long value = 1;
};

#define HAS_MEMBER_HELPER(name, condition)                                     \
  template <typename T, typename type_long<decltype(condition)>::type val = 0> \
  constexpr bool has_##name##_helper(int) {                                    \
    return true;                                                               \
  };                                                                           \
  template <typename T> constexpr bool has_##name##_helper(...) {              \
    return false;                                                              \
  };                                                                           \
  template <typename T> constexpr bool has_##name() {                          \
    return has_##name##_helper<T>(0);                                          \
  }

HAS_MEMBER_HELPER(begin, std::declval<T>().begin());
HAS_MEMBER_HELPER(end, std::declval<T>().end());
HAS_MEMBER_HELPER(first, T::first);
HAS_MEMBER_HELPER(second, T::second);
// first defined goes first in overload, this avoid ambiguity with multiple matches
// and third party overloads 
HAS_MEMBER_HELPER(defined_os_already, std::declval<std::ostream &>()
                                          << std::declval<T>());

template <bool = false> struct truthy {};

template <> struct truthy<true> { typedef long type; };

template <typename T,
          typename truthy<has_first<T>() and has_second<T>() and
                          not has_defined_os_already<T>()>::type val = 0>
std::ostream &operator<<(std::ostream &op, const T &value) {
  op << "<" << value.first << "," << value.second << ">";
  return op;
}

template <typename T,
          typename truthy<has_begin<T>() and has_end<T>() and
                          not has_defined_os_already<T>()>::type val = 0>
std::ostream &operator<<(std::ostream &op, const T &value) {
  auto it = value.begin();
  op << "{";
  if (it != value.end()) {
    op << *it;
    it++;
  }
  while (it != value.end()) {
    op << "," << *it;
    it++;
  }
  op << "}";
  return op;
}

template <typename T> std::string stringify_os(const T &t) {

  std::stringstream ss;
  ss << t;
  return ss.str();
}
