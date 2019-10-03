#pragma once

#include <string_view>
#include <unordered_map>
#include "glad.h"

namespace gl {

inline constexpr std::string_view kVertexShaderCode = R"(
#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texture_coord;
layout(location = 3) in int bone_id;

out vec3 frag_position;
out vec3 frag_normal;
out vec2 frag_texture_coord;

uniform mat4 transformation;
uniform mat4 bone_transformations[32];

void main() {
  gl_Position = transformation * vec4(position, 1.0);
  frag_normal = normal;
  frag_texture_coord = texture_coord;
}
)";

inline constexpr std::string_view kFragmentShaderCode = R"(
#version 330

in vec3 frag_position;
in vec3 frag_normal;
in vec2 frag_texture_coord;

out vec4 color;

uniform sampler2D texture_sampler;

void main() {
  color = texture(texture_sampler, frag_texture_coord);
}
)";

struct UniformInfo {
  GLint location;
  GLsizei size;
};

class ShaderProgram final {
 public:
  ShaderProgram(std::string_view vertex_shader_code,
                std::string_view fragment_shader_code);
  ~ShaderProgram();

  void Bind() const { glUseProgram(id_); }
  void Unbind() const { glUseProgram(0); }

  const UniformInfo& GetUniform(std::string_view name) const;

 private:
  std::unordered_map<std::string, UniformInfo> uniforms_;
  GLuint id_;
  GLuint vertex_shader_id_;
  GLuint fragment_shader_id_;
};

}  // namespace gl