#include "gl.h"

#include "error.h"
#include "logging.h"

namespace gl {

Gl::Gl() {
  if (!gladLoadGL()) {
    LOG(FATAL) << "GLAD failed to load GL";
  }
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
