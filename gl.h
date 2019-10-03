#include <string_view>
#include <unordered_map>
#include "glad.h"

#pragma once

namespace gl {

void MessageCalback(GLenum source, GLenum type, GLuint id, GLenum severity,
                    GLsizei length, GLchar const* message, void const*);

template <typename T>
class Bind final {
 public:
  Bind(const T* resource) : resource_(resource) { resource_->Bind(); }
  ~Bind() { resource_->Unbind(); }

 private:
  const T* resource_;
};

class Gl {
 public:
  Gl();
};

float AspectRatio();

}  // namespace gl
