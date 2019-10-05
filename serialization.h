#pragma once

#include <string_view>
#include <type_traits>
#include <vector>

namespace data {

template <typename T>
void SerializeData(const std::vector<T> &vec, std::vector<unsigned char> *out) {
  SerializeData(vec.size(), out);
  for (const T &elem : vec) {
    SerializeData(elem, out);
  }
}

void SerializeData(std::string_view str, std::vector<unsigned char> *out);

template <typename Int, std::enable_if_t<std::is_integral_v<Int>, int> = 0>
void SerializeData(Int num, std::vector<unsigned char> *out) {
  unsigned char buf[sizeof(num)];
  memcpy(buf, &num, sizeof(num));
  for (size_t i = 0; i < sizeof(num); ++i) {
    out->push_back(buf[i]);
  }
}

template <typename Float,
          std::enable_if_t<std::is_floating_point_v<Float>, int> = 0>
void SerializeData(Float num, std::vector<unsigned char> *out) {
  unsigned char buf[sizeof(num)];
  memcpy(buf, &num, sizeof(num));
  for (size_t i = 0; i < sizeof(num); ++i) {
    out->push_back(buf[i]);
  }
}

}  // namespace data
