#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include "absl/strings/str_cat.h"
#include "glad.h"
#include "glm/glm.hpp"

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

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 bone_transformations[32];

void main() {
  gl_Position = projection * view * model * vec4(position, 1.0);
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
  GLenum type;
};

class ShaderProgram final {
 public:
  ShaderProgram(std::string_view vertex_shader_code,
                std::string_view fragment_shader_code);
  ShaderProgram() = delete;
  ShaderProgram(const ShaderProgram&) = delete;
  ShaderProgram(ShaderProgram&&);
  ~ShaderProgram();

  void Bind() const { glUseProgram(id_); }
  void Unbind() const { glUseProgram(0); }

  void SetUniform(std::string_view name, glm::mat4) const;

  std::string DebugString() const { return absl::StrCat("Shader ", id_); }

 private:
  std::unordered_map<std::string, UniformInfo> uniforms_;
  GLuint id_;
  GLuint vertex_shader_id_;
  GLuint fragment_shader_id_;
};

}  // namespace gl
