#include "bmd.h"

#include <cstring>
#include <unordered_map>

#include "error.h"
#include "file.h"
#include "string.h"
#include "glm/gtx/hash.hpp"

namespace data {
namespace {

struct RawInput {
  std::vector<unsigned char> raw;
  size_t pos;
};

template <typename T>
T Read(RawInput* input) {
  static_assert(std::is_standard_layout<T>::value);
  if (input->pos + sizeof(T) > input->raw.size()) {
    throw util::InternalException("cannot read past end of buffer");
  }
  T result{};
  std::memcpy(&result, &input->raw[input->pos], sizeof(T));
  input->pos += sizeof(T);
  return result;
}

template <typename T>
std::vector<T> Read(RawInput* input, size_t num) {
  std::vector<T> result;
  result.reserve(num);
  for (size_t i = 0; i < num; ++i) {
    result.push_back(Read<T>(input));
  }
  return result;
}

void Skip(RawInput* input, size_t num) { input->pos += num; }

void XorDecode(RawInput* input) {
  constexpr std::array<unsigned char, 16> kIncrementalKeys = {
      0xd1, 0x73, 0x52, 0xf6, 0xd2, 0x9a, 0xcb, 0x27,
      0x3e, 0xaf, 0x59, 0x31, 0x37, 0xb3, 0xe7, 0xa2};
  char key = 0x5e;
  for (size_t i = input->pos; i < input->raw.size(); ++i) {
    const size_t key_index = (i - input->pos) % kIncrementalKeys.size();
    unsigned char b = input->raw[i];
    input->raw[i] = (b ^ kIncrementalKeys[key_index]) - key;
    key = b + 0x3d;
  }
}

struct RawPosition {
  int32_t bone;
  glm::vec3 v;
};
static_assert(sizeof(RawPosition) == 16);

struct RawNormal {
  int32_t bone;
  glm::vec3 v;
  unsigned char pad[4];
};
static_assert(sizeof(RawNormal) == 20);

struct RawTriangle {
  unsigned char pad_0[2];
  std::array<int16_t, 3> position_indices;
  unsigned char pad_1[2];
  std::array<int16_t, 3> normal_indices;
  unsigned char pad_2[2];
  std::array<int16_t, 3> uv_indices;
  unsigned char pad_3[40];
};
static_assert(sizeof(RawTriangle) == 64);

Mesh LoadMesh(RawInput* raw) {
  Mesh res;

  auto num_positions = Read<uint16_t>(raw);
  auto num_normals = Read<uint16_t>(raw);
  auto num_uvs = Read<uint16_t>(raw);
  auto num_triangles = Read<uint16_t>(raw);
  Skip(raw, 2);

  std::vector<RawPosition> raw_positions =
      Read<RawPosition>(raw, num_positions);
  std::vector<RawNormal> raw_normals = Read<RawNormal>(raw, num_normals);
  std::vector<glm::vec2> raw_uvs = Read<glm::vec2>(raw, num_uvs);
  std::vector<RawTriangle> raw_triangles =
      Read<RawTriangle>(raw, num_triangles);

  std::unordered_map<glm::ivec3, size_t> indices;
  for (const RawTriangle& raw_triangle : raw_triangles) {
    for (size_t i = 0; i < 3; ++i) {
      glm::ivec3 component_index(raw_triangle.position_indices[i],
                                 raw_triangle.normal_indices[i],
                                 raw_triangle.uv_indices[i]);
      auto it = indices.find(component_index);
      if (it == indices.end()) {
        indices[component_index] = res.positions.size();
        res.positions.push_back(
            raw_positions[raw_triangle.position_indices[i]].v);
        res.bones.push_back(
            raw_positions[raw_triangle.position_indices[i]].bone);
        res.normals.push_back(raw_normals[raw_triangle.normal_indices[i]].v);
        res.uvs.push_back(raw_uvs[raw_triangle.uv_indices[i]]);
      }
      res.triangles.push_back(static_cast<int32_t>(indices[component_index]));
    }
  }

  std::vector<char> raw_texture = Read<char>(raw, 32);
  res.texture_path =
      std::string(raw_texture.begin(),
                  std::find(raw_texture.begin(), raw_texture.end(), '\0'));

  return res;
}

}  // namespace

Model LoadModel(const AssetStore& assets, std::string_view file_path) {
  Model m;
  RawInput raw = {assets.GetBinaryData(file_path), 0};

  auto magic = Read<uint32_t>(&raw);
  switch (magic) {
    case 0x0a444d42:
      break;
    case 0x0c444d42:
      Skip(&raw, 4);
      XorDecode(&raw);
      break;
    default:
      throw util::InternalException(
          util::StrCat("unknown BMD magic header: ", magic));
  }

  std::vector<char> raw_texture = Read<char>(&raw, 32);
  m.name = std::string(raw_texture.begin(),
                       std::find(raw_texture.begin(), raw_texture.end(), '\0'));

  auto num_meshes = Read<uint16_t>(&raw);
  m.meshes.reserve(num_meshes);
  auto num_bones = Read<uint16_t>(&raw);
  auto num_frames = Read<uint16_t>(&raw);

  for (uint16_t i = 0; i < num_meshes; ++i) {
    m.meshes.push_back(LoadMesh(&raw));
  }

  return m;
}

}  // namespace data
