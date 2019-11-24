#include "asset.h"
#include "logging.h"
#include "file.h"
#include "serialization.h"

namespace data {

AssetStore AssetStore::LoadFromBinary(std::string_view path) {
  LOG(INFO) << "Loading binary assets from" << path;
  std::vector<unsigned char> assets_bin = data::FileGetBinaryContents(path);
  LOG(INFO) << "Loaded binary assets " << assets_bin.size() << " bytes.";
  std::unordered_map<std::string, std::vector<unsigned char>> assets;
  data::DeserializeData(assets_bin, &assets);
  LOG(INFO) << "Deserialized assets " << assets.size() << " entries.";
  return AssetStore(std::move(assets));
}

}  // namespace data
