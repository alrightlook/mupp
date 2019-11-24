#pragma once

#include <cstring>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <vector>
#include "absl/types/span.h"

namespace data {

void SerializeData(std::string_view str, std::vector<unsigned char> *out);
absl::Span<const unsigned char> DeserializeData(absl::Span<const unsigned char> data, std::string* out);

template <typename Int, std::enable_if_t<std::is_integral_v<Int>, int> = 0>
void SerializeData(Int num, std::vector<unsigned char> *out) {
  unsigned char buf[sizeof(num)];
  memcpy(buf, &num, sizeof(num));
  for (size_t i = 0; i < sizeof(num); ++i) {
    out->push_back(buf[i]);
  }
}

template <typename Int, std::enable_if_t<std::is_integral_v<Int>, int> = 0>
absl::Span<const unsigned char> DeserializeData(absl::Span<const unsigned char> data, Int* out) {
  memcpy(out, data.data(), sizeof(Int));
  return data.subspan(sizeof(Int));
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

template <typename T>
void SerializeData(const std::vector<T> &vec, std::vector<unsigned char> *out) {
  SerializeData(vec.size(), out);
  for (const T &elem : vec) {
    SerializeData(elem, out);
  }
}

template <typename T>
absl::Span<const unsigned char> DeserializeData(absl::Span<const unsigned char> data, std::vector<T>* out) {
  size_t size;
  data = DeserializeData(data, &size);
  for (size_t i = 0; i < size; ++i) {
    T elem{};
    data = DeserializeData(data, &elem);
    out->push_back(elem);
  }
  return data;
}

template <typename K, typename V>
void SerializeData(const std::unordered_map<K, V> &map, std::vector<unsigned char> *out) {
  SerializeData(map.size(), out);
  for (const auto &[k, v] : map) {
    SerializeData(k, out);
    SerializeData(v, out);
  }
}

template <typename K, typename V>
absl::Span<const unsigned char> DeserializeData(absl::Span<const unsigned char> data, std::unordered_map<K, V>* out) {
  size_t size;
  data = DeserializeData(data, &size);
  for (size_t i = 0; i < size; ++i) {
    K key{};
    data = DeserializeData(data, &key);
    V value{};
    data = DeserializeData(data, &value);
    out->emplace(key, value);
  }
  return data;
}

}  // namespace data
