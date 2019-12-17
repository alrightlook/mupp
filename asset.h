#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include "absl/strings/ascii.h"
#include "absl/strings/str_cat.h"
#include "error.h"

namespace data {

class AssetStore {
 public:
  static AssetStore LoadFromBinary(std::string_view path);

  const std::vector<unsigned char>& GetBinaryData(std::string_view path) const {
    std::string key = absl::AsciiStrToLower(path);
    if (auto it = assets_.find(key); it == assets_.end()) {
      throw util::NotFoundException(
          absl::StrCat("Could not found asset: ", path));
    } else {
      return it->second;
    }
  }

  const std::vector<std::string>& Names() const { return names_; }

 private:
  AssetStore(std::unordered_map<std::string, std::vector<unsigned char>> assets)
      : assets_(std::move(assets)) {
    for (const auto& [k, v] : assets_) {
      names_.push_back(k);
    }
    std::sort(names_.begin(), names_.end());
  }

  std::unordered_map<std::string, std::vector<unsigned char>> assets_;
  std::vector<std::string> names_;
};

}  // namespace data
