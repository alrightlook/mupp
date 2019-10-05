#pragma once

#include <algorithm>
#include <ostream>
#include <tuple>
#include <type_traits>
#include <vector>
#include "macro.h"
#include "serialization.h"

// Define a member.
#define PP_TUPLE_DEFINE_STRUCT_MEMBER_IMPL(type, name) \
  PP_UNPARENTHESIZE(type) name;
#define PP_TUPLE_DEFINE_STRUCT_MEMBER(p) PP_TUPLE_DEFINE_STRUCT_MEMBER_IMPL p

// Apply visitor to a member.
#define PP_TUPLE_DEFINE_STRUCT_APPLY_IMPL(type, name) \
  f(PP_STR(name), self.name);
#define PP_TUPLE_DEFINE_STRUCT_APPLY(p) PP_TUPLE_DEFINE_STRUCT_APPLY_IMPL p

// Declare the list of types for the members.
#define PP_TUPLE_DEFINE_STRUCT_TYPES_ONE_IMPL(t, n) , PP_UNPARENTHESIZE(t)
#define PP_TUPLE_DEFINE_STRUCT_TYPES_ONE(p) \
  PP_TUPLE_DEFINE_STRUCT_TYPES_ONE_IMPL p
#define PP_TUPLE_DEFINE_STRUCT_TYPES(...) \
  PP_TAIL(PP_FOR_EACH(PP_TUPLE_DEFINE_STRUCT_TYPES_ONE, __VA_ARGS__))

// Declare accessors for the members.
#define PP_TUPLE_DEFINE_STRUCT_SELF_MEMBERS_ONE_IMPL(t, n) , self.n
#define PP_TUPLE_DEFINE_STRUCT_SELF_MEMBERS_ONE(p) \
  PP_TUPLE_DEFINE_STRUCT_SELF_MEMBERS_ONE_IMPL p
#define PP_TUPLE_DEFINE_STRUCT_SELF_MEMBERS(...) \
  PP_TAIL(PP_FOR_EACH(PP_TUPLE_DEFINE_STRUCT_SELF_MEMBERS_ONE, __VA_ARGS__))

// Define the struct together with the helper.
#define PP_TUPLE_DEFINE_STRUCT(...)                          \
  PP_FOR_EACH(PP_TUPLE_DEFINE_STRUCT_MEMBER, __VA_ARGS__)    \
  struct Unpack {                                            \
    template <typename T, typename F>                        \
    static void ForEach(T &&self, F &&f) {                   \
      PP_FOR_EACH(PP_TUPLE_DEFINE_STRUCT_APPLY, __VA_ARGS__) \
    }                                                        \
  }

#define PP_TUPLE_DEFINE_OP(name, op) PP_CAT(PP_TUPLE_DEFINE_OP_, op)(name)

#define PP_TUPLE_DEFINE_OP_ostream(name)                               \
  inline std::ostream &operator<<(std::ostream &os, const name &val) { \
    bool first = true;                                                 \
    os << #name << " { ";                                              \
    auto f = [&](std::string_view member, auto val) {                  \
      if (!first) {                                                    \
        os << ", ";                                                    \
      }                                                                \
      first = false;                                                   \
      using ::tuple_define_struct::operator<<;                         \
      os << member << " = " << val;                                    \
    };                                                                 \
    name::Unpack::ForEach(val, f);                                     \
    os << " }";                                                        \
    return os;                                                         \
  }

#define PP_TUPLE_DEFINE_OP_serialize(name)                     \
  inline void SerializeData(const name &val,                   \
                            std::vector<unsigned char> *out) { \
    std::vector<unsigned char> result;                         \
    auto f = [&](std::string_view, auto val) {                 \
      using ::data::SerializeData;                             \
      SerializeData(val, out);                                 \
    };                                                         \
    name::Unpack::ForEach(val, f);                             \
  }

namespace tuple_define_struct {

template <typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &v) {
  bool first = true;
  os << "[";
  for (const T &elem : v) {
    if (!first) {
      os << ", ";
    }
    first = false;
    os << elem;
  }
  os << "]";
  return os;
}

}  // namespace tuple_define_struct
