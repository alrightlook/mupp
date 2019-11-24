#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <memory>
#include <unordered_map>
#include "cleanup.h"
#include "file.h"
#include "serialization.h"


int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cout << "Usage: " << argv[0] << " <input_dir> <output_file>\n";
    return EXIT_FAILURE;
  }

  std::unordered_map<std::string, std::vector<unsigned char>> assets;

  std::vector<unsigned char> assets_buf;
  data::SerializeData(assets, &assets_buf);
  data::FileSetBinaryContents(argv[2], assets_buf);

  return EXIT_SUCCESS;
}
