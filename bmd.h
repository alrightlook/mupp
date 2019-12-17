#pragma once

#include <string>
#include <string_view>
#include <vector>
#include "asset.h"
#include "glm/glm.hpp"
#include "math.h"
#include "struct.h"

namespace data {

struct Mesh {
  PP_TUPLE_DEFINE_STRUCT(                    //
      (std::vector<glm::vec3>, positions),   //
      (std::vector<glm::vec3>, normals),     //
      (std::vector<glm::vec2>, uvs),         //
      (std::vector<int32_t>, bones),         //
      (std::vector<int32_t>, triangles),  //
      (std::string, texture_path)            //
  );
};
PP_TUPLE_DEFINE_OP(Mesh, ostream);
PP_TUPLE_DEFINE_OP(Mesh, serialize);

struct Model {
  PP_TUPLE_DEFINE_STRUCT(          //
      (std::string, name),         //
      (std::vector<Mesh>, meshes)  //
  );
};
PP_TUPLE_DEFINE_OP(Model, ostream);
PP_TUPLE_DEFINE_OP(Model, serialize);

Model LoadModel(const AssetStore& assets, std::string_view file_path);

};  // namespace data
