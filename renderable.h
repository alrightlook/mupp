#pragma once

#include <array>
#include "bmd.h"
#include "glad.h"
#include "math.h"
#include "shader.h"
#include "texture.h"

namespace gl {

class Renderable {
 public:
  Renderable(const data::Mesh& mesh);
  ~Renderable();

  void Render(const ShaderProgram& shader, const Texture& texture,
              const math::Mat4f& transform) const;

 private:
  GLuint vao_;
  std::array<GLuint, 5> vbos_;
  size_t num_vertices_;
};

}  // namespace gl