#pragma once

#include <string>
#include <string_view>
#include <vector>
#include "math.h"
#include "struct.h"

namespace data {

struct Mesh {
  PP_TUPLE_DEFINE_STRUCT(                     //
      (std::vector<math::Vec3f>, positions),  //
      (std::vector<math::Vec3f>, normals),    //
      (std::vector<math::Vec2f>, uvs),        //
      (std::vector<int32_t>, bones),          //
      (std::vector<math::Vec3i>, triangles),  //
      (std::string, texture_path)             //
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

Model LoadModel(std::string_view file_path);

};  // namespace data
