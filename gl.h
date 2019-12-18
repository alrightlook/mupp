#include <string>
#include <string_view>
#include <unordered_map>

#include "absl/strings/str_cat.h"
#include "error.h"
#include "glad.h"
#include "logging.h"

#pragma once

namespace gl {
namespace internal {

#define GL_CHECK_ERROR() ::gl::internal::CheckError("", __FILE__, __LINE__)
#define GL_CHECK_ERROR_MSG(msg) \
  ::gl::internal::CheckError((msg), __FILE__, __LINE__)

inline void CheckError(std::string_view message, std::string_view file,
                       size_t line) {
  if (GLenum error = glGetError(); error != GL_NO_ERROR) {
    throw util::InternalException(absl::StrCat("GL error (", message, ") ",
                                               error, " at ", file, ":", line));
  }
}

}  // namespace internal

template <typename T>
class Bind final {
 public:
  Bind(const T* resource) : resource_(resource) {
    LOG(DEBUG) << "Binding " << util::DebugStringOrEmpty(*resource_);
    resource_->Bind();
    GL_CHECK_ERROR_MSG(util::DebugStringOrEmpty(*resource_));
  }
  ~Bind() {
    LOG(DEBUG) << "Unbinding " << util::DebugStringOrEmpty(*resource_);
    resource_->Unbind();
    GL_CHECK_ERROR_MSG(util::DebugStringOrEmpty(*resource_));
  }

 private:
  const T* resource_;
};

class Gl {
 public:
  Gl();
};

float AspectRatio();

}  // namespace gl
