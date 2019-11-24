#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include "absl/strings/ascii.h"

namespace data {

class AssetStore {
 public:
  static AssetStore LoadFromBinary(std::string_view path);

  const std::vector<unsigned char>& GetBinaryData(std::string_view path) const {
    return assets_.at(absl::AsciiStrToLower(path));
  }

 private:
  AssetStore(std::unordered_map<std::string, std::vector<unsigned char>> assets)
      : assets_(std::move(assets)) {}

  std::unordered_map<std::string, std::vector<unsigned char>> assets_;
};

}  // namespace data
