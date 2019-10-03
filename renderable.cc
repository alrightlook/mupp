#include "renderable.h"
#include "gl.h"
#include "logging.h"
#include "math.h"
#include "texture.h"

namespace gl {
namespace {

enum class Vbo : int {
  kPosition = 0,
  kNormal,
  kTextureCoord,
  kBone,
  kIndex,
};

}

Renderable::Renderable(const data::Mesh& mesh)
    : vao_(0),
      vbos_({0, 0, 0, 0, 0}),
      num_vertices_(mesh.triangles.size() * 3) {
  glCreateVertexArrays(1, &vao_);
  for (size_t i = 0; i < vbos_.size(); ++i) {
    glEnableVertexArrayAttrib(vao_, i);
    glVertexArrayAttribBinding(vao_, i, i);
  }
  glCreateBuffers(static_cast<GLsizei>(vbos_.size()), vbos_.data());

  glNamedBufferStorage(vbos_[static_cast<int>(Vbo::kPosition)],
                       sizeof(math::Vec3f) * mesh.positions.size(),
                       mesh.positions.data(), GL_DYNAMIC_STORAGE_BIT);
  glVertexArrayAttribFormat(vao_, 0, 3, GL_FLOAT, GL_FALSE, 0);
  glVertexArrayVertexBuffer(vao_, 0, vbos_[static_cast<int>(Vbo::kPosition)], 0,
                            sizeof(math::Vec3f));

  glNamedBufferStorage(vbos_[static_cast<int>(Vbo::kNormal)],
                       sizeof(math::Vec3f) * mesh.normals.size(),
                       mesh.normals.data(), GL_DYNAMIC_STORAGE_BIT);
  glVertexArrayAttribFormat(vao_, 1, 3, GL_FLOAT, GL_FALSE, 0);
  glVertexArrayVertexBuffer(vao_, 1, vbos_[static_cast<int>(Vbo::kNormal)], 0,
                            sizeof(math::Vec3f));

  glNamedBufferStorage(vbos_[static_cast<int>(Vbo::kTextureCoord)],
                       sizeof(math::Vec2f) * mesh.uvs.size(), mesh.uvs.data(),
                       GL_DYNAMIC_STORAGE_BIT);
  glVertexArrayAttribFormat(vao_, 2, 2, GL_FLOAT, GL_FALSE, 0);
  glVertexArrayVertexBuffer(vao_, 2,
                            vbos_[static_cast<int>(Vbo::kTextureCoord)], 0,
                            sizeof(math::Vec2f));

  glNamedBufferStorage(vbos_[static_cast<int>(Vbo::kBone)],
                       sizeof(int32_t) * mesh.bones.size(), mesh.bones.data(),
                       GL_DYNAMIC_STORAGE_BIT);
  glVertexArrayAttribFormat(vao_, 3, 1, GL_INT, GL_FALSE, 0);
  glVertexArrayVertexBuffer(vao_, 3, vbos_[static_cast<int>(Vbo::kBone)], 0,
                            sizeof(int32_t));

  glNamedBufferStorage(vbos_[static_cast<int>(Vbo::kIndex)],
                       sizeof(math::Vec3i) * mesh.triangles.size(),
                       mesh.triangles.data(), GL_DYNAMIC_STORAGE_BIT);
  glVertexArrayElementBuffer(vao_, vbos_[static_cast<int>(Vbo::kIndex)]);
}

Renderable::~Renderable() {}

void Renderable::Render(const ShaderProgram& shader, const Texture& texture,
                        const math::Mat4f& transform) const {
  Bind bind_shader(&shader);
  Bind bind_texture(&texture);
  glBindVertexArray(vao_);
  glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(num_vertices_),
                 GL_UNSIGNED_INT, 0);
}

}  // namespace gl