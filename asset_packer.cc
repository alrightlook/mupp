#include <ftw.h>

#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "absl/strings/ascii.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/strip.h"
#include "cleanup.h"
#include "error.h"
#include "file.h"
#include "logging.h"
#include "serialization.h"

namespace {

std::vector<std::string> tree_walk_result;

int TreeWalkCallback(const char* fpath, const struct stat* sb, int tflag) {
  if (tflag == FTW_D || tflag == FTW_DNR) {
    return 0;
  }
  tree_walk_result.emplace_back(fpath);
  return 0;
}

std::vector<std::string> RecursivelyWalkDir(const std::string& base_path) {
  ftw(base_path.c_str(), &TreeWalkCallback, 20);
  for (std::string& path : tree_walk_result) {
    path = absl::AsciiStrToLower(
        absl::StripPrefix(path, absl::StrCat(base_path, "/")));
  }
  return tree_walk_result;
}

}  // namespace

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cout << "Usage: " << argv[0] << " <input_dir> <output_file>\n";
    return EXIT_FAILURE;
  }

  std::unordered_map<std::string, std::vector<unsigned char>> assets;
  std::vector<std::string> paths = RecursivelyWalkDir(argv[1]);
  for (const std::string& path : paths) {
    LOG(INFO) << "Loading " << path << '\n';
    assets.emplace(
        path, data::FileGetBinaryContents(absl::StrCat(argv[1], "/", path)));
  }

  std::vector<unsigned char> assets_buf;
  data::SerializeData(assets, &assets_buf);
  data::FileSetBinaryContents(argv[2], assets_buf);

  return EXIT_SUCCESS;
}
