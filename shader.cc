#include "shader.h"

#include <memory>

#include "error.h"
#include "logging.h"
#include "string.h"

namespace gl {

ShaderProgram::ShaderProgram(std::string_view vertex_shader_code,
                             std::string_view fragment_shader_code) {
  id_ = glCreateProgram();
  vertex_shader_id_ = glCreateShader(GL_VERTEX_SHADER);
  fragment_shader_id_ = glCreateShader(GL_FRAGMENT_SHADER);

  auto compile_and_attach_shader = [&](GLuint shader_id,
                                       std::string_view code) {
    GLint len = static_cast<GLint>(code.length());
    const char* data = code.data();
    glShaderSource(shader_id, 1, &data, &len);
    glCompileShader(shader_id);
    if (GLint compile_ok = GL_FALSE;
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compile_ok),
        compile_ok == GL_FALSE) {
      throw util::InternalException("failed to compile shader");
    }
    glAttachShader(id_, shader_id);
  };
  compile_and_attach_shader(vertex_shader_id_, vertex_shader_code);
  compile_and_attach_shader(fragment_shader_id_, fragment_shader_code);
  glLinkProgram(id_);
  if (GLint link_ok = GL_FALSE;
      glGetProgramiv(id_, GL_LINK_STATUS, &link_ok), link_ok == GL_FALSE) {
    throw util::InternalException("failed to link shader");
  }
  glValidateProgram(id_);
  if (GLint validate_ok = GL_FALSE;
      glGetProgramiv(id_, GL_LINK_STATUS, &validate_ok),
      validate_ok == GL_FALSE) {
    throw util::InternalException("failed to validate shader");
  }
  LOG(INFO) << "Created shader program " << id_ << " with vertex shader "
            << vertex_shader_id_ << " and fragment shader "
            << fragment_shader_id_;

  if (GLint num_uniforms = 0;
      glGetProgramiv(id_, GL_ACTIVE_UNIFORMS, &num_uniforms),
      num_uniforms > 0) {
    GLint max_name_len = 0;
    glGetProgramiv(id_, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_name_len);
    auto raw_name = std::make_unique<char[]>(max_name_len);

    for (GLint i = 0; i < num_uniforms; ++i) {
      UniformInfo uniform = {};
      GLsizei name_length = 0;
      glGetActiveUniform(id_, i, max_name_len, &name_length, &uniform.size,
                         &uniform.type, raw_name.get());
      uniform.location = glGetUniformLocation(id_, raw_name.get());
      std::string name(raw_name.get(), name_length);
      uniforms_[name] = uniform;
      LOG(INFO) << "Shader " << id_ << " has uniform " << name
                << " at location " << uniform.location;
    }
  }
}

ShaderProgram::ShaderProgram(ShaderProgram&& rhs) : id_(rhs.id_),vertex_shader_id_(rhs.vertex_shader_id_), fragment_shader_id_(rhs.fragment_shader_id_) {
  rhs.id_ = 0;
  rhs.vertex_shader_id_ = 0;
  rhs.fragment_shader_id_ = 0;
}

ShaderProgram::~ShaderProgram() {
  if (id_ == 0) {
    return;
  }
  glDetachShader(id_, fragment_shader_id_);
  glDetachShader(id_, vertex_shader_id_);
  glDeleteShader(fragment_shader_id_);
  glDeleteShader(vertex_shader_id_);
  glDeleteProgram(id_);
  LOG(INFO) << "Deleting shader " << id_;
}

const UniformInfo& ShaderProgram::GetUniform(std::string_view name) const {
  auto it = uniforms_.find(std::string(name));
  if (it == uniforms_.end()) {
    throw util::NotFoundException(util::StrCat("uniform not found: ", name));
  }
  return it->second;
}

}  // namespace gl
