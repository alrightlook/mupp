#include "serialization.h"

namespace data {

void SerializeData(uint64_t num, std::vector<unsigned char>* out) {
  unsigned char buf[4];
  memcpy(buf, &num, sizeof(buf));
  for (size_t i = 0; i < 4; ++i) {
    out->push_back(buf[i]);
  }
}

}  // namespace data