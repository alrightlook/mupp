#pragma once

#include <string>
#include <string_view>
#include <vector>
#include "absl/strings/str_join.h"

namespace data {

std::string FileGetTextContents(std::string_view path);

std::vector<unsigned char> FileGetBinaryContents(std::string_view path);

void FileSetBinaryContents(std::string_view path,
                           const std::vector<unsigned char>& buf);

std::string_view DirName(std::string_view path);

template <typename... Strings>
std::string JoinPath(Strings... parts) {
  std::vector<std::string_view> parts_vec({parts...});
  return absl::StrJoin(parts_vec, "/");
}

}  // namespace data
