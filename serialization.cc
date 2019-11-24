#include "serialization.h"

#include <cstring>

namespace data {

void SerializeData(std::string_view str, std::vector<unsigned char> *out) {
  SerializeData(str.size(), out);
  for (char c : str) {
    out->push_back(c);
  }
}

absl::Span<const unsigned char> DeserializeData(absl::Span<const unsigned char> data, std::string* out) {
  size_t size;
  data = DeserializeData(data, &size);
  out->resize(size);
  for (size_t i = 0; i < size; ++i) {
    out->at(i) = data[i];
  }
  return data.subspan(size);
}

}  // namespace data
