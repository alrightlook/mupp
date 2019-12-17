#include "serialization.h"

#include <cstring>
#include "error.h"

namespace data {

void SerializeData(std::string_view str, std::vector<unsigned char> *out) {
  SerializeData(str.size(), out);
  for (char c : str) {
    out->push_back(c);
  }
}

absl::Span<const unsigned char> DeserializeData(
    absl::Span<const unsigned char> data, std::string *out) {
  size_t size;
  data = DeserializeData(data, &size);
  out->resize(size);
  for (size_t i = 0; i < size; ++i) {
    out->at(i) = data[i];
  }
  return data.subspan(size);
}

}  // namespace data

namespace glm {

void SerializeData(const glm::vec2 &vec, std::vector<unsigned char> *out) {
  THROW_UNIMPLEMENTED_FUNCTION();
}

void SerializeData(const glm::vec3 &vec, std::vector<unsigned char> *out) {
  THROW_UNIMPLEMENTED_FUNCTION();
}

void SerializeData(const glm::ivec3 &vec, std::vector<unsigned char> *out) {
  THROW_UNIMPLEMENTED_FUNCTION();
}

absl::Span<const unsigned char> DeserializeData(
    absl::Span<const unsigned char> data, const glm::vec2 &vec) {
  THROW_UNIMPLEMENTED_FUNCTION();
}

absl::Span<const unsigned char> DeserializeData(
    absl::Span<const unsigned char> data, const glm::vec3 &vec) {
  THROW_UNIMPLEMENTED_FUNCTION();
}

absl::Span<const unsigned char> DeserializeData(
    absl::Span<const unsigned char> data, const glm::ivec3 &vec) {
  THROW_UNIMPLEMENTED_FUNCTION();
}

}  // namespace glm
