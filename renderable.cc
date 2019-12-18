#include "renderable.h"

#include "absl/strings/str_join.h"
#include "gl.h"
#include "glm/glm.hpp"
#include "logging.h"
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
    : vao_(0), vbos_({0, 0, 0, 0, 0}), num_vertices_(mesh.triangles.size()) {
  glGenVertexArrays(1, &vao_);
  GL_CHECK_ERROR();

  gl::Bind bind(this);
  GL_CHECK_ERROR();
  glGenBuffers(static_cast<GLsizei>(vbos_.size()), vbos_.data());

  glEnableVertexAttribArray(static_cast<int>(Vbo::kPosition));
  glBindBuffer(GL_ARRAY_BUFFER, vbos_[static_cast<int>(Vbo::kPosition)]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh.positions.size(),
               mesh.positions.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(static_cast<int>(Vbo::kPosition), 3, GL_FLOAT, GL_FALSE,
                        0, 0);
  GL_CHECK_ERROR();

  glEnableVertexAttribArray(static_cast<int>(Vbo::kNormal));
  glBindBuffer(GL_ARRAY_BUFFER, vbos_[static_cast<int>(Vbo::kNormal)]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh.normals.size(),
               mesh.normals.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(static_cast<int>(Vbo::kNormal), 3, GL_FLOAT,
                        /*normalized=*/GL_FALSE, 0, 0);
  GL_CHECK_ERROR();

  glEnableVertexAttribArray(static_cast<int>(Vbo::kTextureCoord));
  glBindBuffer(GL_ARRAY_BUFFER, vbos_[static_cast<int>(Vbo::kTextureCoord)]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * mesh.uvs.size(),
               mesh.uvs.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(static_cast<int>(Vbo::kTextureCoord), 2, GL_FLOAT,
                        GL_FALSE, 0, 0);
  GL_CHECK_ERROR();

  glBindBuffer(GL_ARRAY_BUFFER, vbos_[static_cast<int>(Vbo::kBone)]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(int32_t) * mesh.bones.size(),
               mesh.bones.data(), GL_STATIC_DRAW);
  glVertexAttribIPointer(static_cast<int>(Vbo::kBone), 1, GL_INT, 0, 0);
  GL_CHECK_ERROR();

  // The indices buffer is stored in the VAO so we don't unbind.
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos_[static_cast<int>(Vbo::kIndex)]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               sizeof(glm::ivec3) * mesh.triangles.size(),
               mesh.triangles.data(), GL_STATIC_DRAW);
  GL_CHECK_ERROR();

  LOG(INFO) << "Created VAO " << vao_ << " with VBOs "
            << absl::StrJoin(vbos_, ", ");
}

  Renderable::Renderable(Renderable&& rhs) : vao_(std::move(rhs.vao_)), vbos_(rhs.vbos_), num_vertices_(rhs.num_vertices_) {
    rhs.vao_ = 0;
    for (GLuint& vbo : rhs.vbos_) {
      vbo = 0;
    }
    rhs.num_vertices_ = 0;
  }

Renderable::~Renderable() {
  if (vao_ == 0) {
    return;
  }
  glDeleteBuffers(static_cast<GLsizei>(vbos_.size()), vbos_.data());
  glDeleteVertexArrays(1, &vao_);
  GL_CHECK_ERROR();
  LOG(INFO) << "Deleted VAO " << vao_;
}

void Renderable::Render(const ShaderProgram& shader, const Texture& texture,
                        const glm::mat4& transform) const {
  gl::Bind bind_shader(&shader);
  gl::Bind bind_texture(&texture);
  gl::Bind bind_vao(this);
  GL_CHECK_ERROR();
  glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(num_vertices_),
                 GL_UNSIGNED_INT, 0);
  GL_CHECK_ERROR();
}

void Renderable::Bind() const { glBindVertexArray(vao_); }

void Renderable::Unbind() const { glBindVertexArray(0); }

}  // namespace gl