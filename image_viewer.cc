#include <algorithm>
#include <cstdlib>
#include <string_view>
#include "SDL2/SDL.h"
#include "asset.h"
#include "bmd.h"
#include "cleanup.h"
#include "gl.h"
#include "logging.h"
#include "math.h"
#include "renderable.h"
#include "shader.h"
#include "string.h"
#include "texture.h"

inline constexpr std::string_view kVertexShaderCode = R"(
#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texture_coord;
layout(location = 3) in int bone_id;
out vec3 frag_position;
out vec2 frag_texture_coord;

void main() {
  gl_Position = vec4(position, 1.0);
  frag_texture_coord = texture_coord;
}
)";

inline constexpr std::string_view kFragmentShaderCode = R"(
#version 330

in vec3 frag_position;
in vec2 frag_texture_coord;
out vec4 color;

uniform sampler2D texture_sampler;

void main() {
  color = texture(texture_sampler, frag_texture_coord);
}
)";

// TODO(paulherman): Disable for release.
extern "C" {
_declspec(dllexport) uint32_t NvOptimusEnablement = 0x00000001;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    LOG(FATAL) << "Usage: " << argv[0] << " <path>";
  }

  int sdl_init_err = SDL_Init(SDL_INIT_EVERYTHING);
  LOG_IF(FATAL, sdl_init_err != 0)
      << "failed to initialize SDL2: " << sdl_init_err;
  util::Cleanup cleanup_sdl([]() { SDL_Quit(); });

  SDL_Window *wnd = SDL_CreateWindow("Image Viewer", SDL_WINDOWPOS_CENTERED,
                                     SDL_WINDOWPOS_CENTERED, 800, 600,
                                     SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
  LOG_IF(FATAL, !wnd) << "failed to create SDL2 window";
  util::Cleanup cleanup_wnd([&]() { SDL_DestroyWindow(wnd); });

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GLContext gl = SDL_GL_CreateContext(wnd);
  LOG_IF(FATAL, !gl) << "failed to create OpenGL context: " << SDL_GetError();
  util::Cleanup cleanup_gl([&]() { SDL_GL_DeleteContext(gl); });

  gl::Gl mygl;
  gl::ShaderProgram shader(kVertexShaderCode, kFragmentShaderCode);

  std::string_view image_path = argv[1];
  data::Image image = data::LoadImage(image_path);
  gl::Texture texture(image);

  data::Mesh rectangle_mesh = {
      {
          {-1.0f, 1.0f, 0.0f},
          {-1.0f, -1.0f, 0.0f},
          {1.0f, 1.0f, 0.0f},
          {1.0f, -1.0f, 0.0f},
      },
      {
          {0.0f, 0.0f, 0.0f},
          {0.0f, 0.0f, 0.0f},
          {0.0f, 0.0f, 0.0f},
          {0.0f, 0.0f, 0.0f},
      },
      {
          {-1.0f, -1.0f},
          {-1.0f, 1.0f},
          {1.0f, -1.0f},
          {1.0f, 1.0f},
      },
      {-1, -1, -1, -1},
      {
          {1, 3, 2},
          {1, 2, 0},
      },
      "texture.fake",
  };
  gl::Renderable rectangle(rectangle_mesh);

  size_t window_width = std::max(static_cast<size_t>(256), image.width);
  size_t window_height = std::max(static_cast<size_t>(256), image.height);
  SDL_SetWindowSize(wnd, window_width, window_height);
  size_t delta_width = window_width - image.width;
  size_t delta_height = window_height - image.height;
  glViewport(delta_width / 2, delta_height / 2, delta_width / 2 + image.width,
             delta_height / 2 + image.height);

  while (true) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          return EXIT_SUCCESS;
        default:
          break;
      }
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    rectangle.Render(shader, texture, (1.0f));
    SDL_GL_SwapWindow(wnd);
  }
  return EXIT_SUCCESS;
}
