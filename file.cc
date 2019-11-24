#include "file.h"

#include <fstream>
#include <ios>
#include <iterator>

#include "error.h"

namespace data {
namespace {}  // namespace

std::string FileGetTextContents(std::string_view path) {
  std::ifstream f(path.data(), std::ios_base::in);
  if (!f.is_open()) {
    throw util::NotFoundException(util::StrCat("file not found: ", path));
  }
  std::string contents((std::istreambuf_iterator<char>(f)),
                       std::istreambuf_iterator<char>());
  return contents;
}

std::vector<unsigned char> FileGetBinaryContents(std::string_view path) {
  std::ifstream f(path.data(), std::ios_base::in | std::ios_base::binary);
  if (!f.is_open()) {
    throw util::NotFoundException(util::StrCat("file not found: ", path));
  }
  std::vector<char> raw_contents((std::istreambuf_iterator<char>(f)),
                                 std::istreambuf_iterator<char>());
  std::vector<unsigned char> result(raw_contents.begin(), raw_contents.end());
  return result;
}

void FileSetBinaryContents(std::string_view path,
                           const std::vector<unsigned char>& buf) {
  std::ofstream f(path.data(), std::ios_base::out | std::ios_base::binary);
  if (!f.is_open()) {
    throw util::NotFoundException(util::StrCat("file not found: ", path));
  }
  for (unsigned char c : buf) {
    f << c;
  }
}

}  // namespace data
