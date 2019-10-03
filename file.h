#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace data {

std::string FileGetTextContents(std::string_view path);

std::vector<unsigned char> FileGetBinaryContents(std::string_view path);

}  // namespace data