#include "serialization.h"

#include <cstring>

namespace data {

void SerializeData(std::string_view str, std::vector<unsigned char> *out) {
  SerializeData(str.size(), out);
  for (char c : str) {
    out->push_back(c);
  }
}

}  // namespace data
