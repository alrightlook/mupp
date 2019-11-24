#include "gl.h"

#include "error.h"
#include "logging.h"

namespace gl {

void MessageCalback(GLenum source, GLenum type, GLuint id, GLenum severity,
                    GLsizei length, GLchar const* message, void const*) {
  switch (severity) {
    case GL_DEBUG_SEVERITY_NOTIFICATION:
      LOG(INFO) << message;
      break;
    case GL_DEBUG_SEVERITY_LOW:
      LOG(WARNING) << message;
      break;
    case GL_DEBUG_SEVERITY_MEDIUM:
    case GL_DEBUG_SEVERITY_HIGH:
      LOG(ERROR) << message;
      break;
  }
}

Gl::Gl() {
  if (!gladLoadGL()) {
    LOG(FATAL) << "GLAD failed to load GL";
  }
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(&MessageCalback, nullptr);
  glEnable(GL_DEPTH_TEST);
}

float AspectRatio() {
  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);
  float width = viewport[2];
  float height = viewport[3];
  return width / height;
}

}  // namespace gl