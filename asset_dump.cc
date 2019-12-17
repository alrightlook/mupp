
#include <algorithm>
#include <cstdlib>
#include <string_view>

#include "asset.h"
#include "cleanup.h"
#include "file.h"
#include "logging.h"
#include "string.h"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    LOG(FATAL) << "Usage: " << argv[0] << " <assets_path>";
  }

  data::AssetStore asset_store = data::AssetStore::LoadFromBinary(argv[1]);
  for (const std::string& name : asset_store.Names()) {
    LOG(INFO) << name;
  }
  return EXIT_SUCCESS;
}
