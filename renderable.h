#pragma once

#include <array>
#include "absl/strings/str_cat.h"
#include "bmd.h"
#include "glad.h"
#include "glm/glm.hpp"
#include "shader.h"
#include "texture.h"

namespace gl {

class Renderable {
 public:
  Renderable(const data::Mesh& mesh);
  Renderable() = delete;
  Renderable(const Renderable&) = delete;
  Renderable(Renderable&& rhs);
  ~Renderable();

  void Render(const ShaderProgram& shader, const Texture& texture,
              const glm::mat4& transform) const;

  void Bind() const;
  void Unbind() const;

  std::string DebugString() const { return absl::StrCat("VAO ", vao_); }

 private:
  GLuint vao_;
  std::array<GLuint, 5> vbos_;
  size_t num_vertices_;
};

}  // namespace gl