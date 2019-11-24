#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace data {

std::string FileGetTextContents(std::string_view path);

std::vector<unsigned char> FileGetBinaryContents(std::string_view path);
void FileSetBinaryContents(std::string_view path, const std::vector<unsigned char>& buf);

}  // namespace data
