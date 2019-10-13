
#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <string_view>
#include "SDL2/SDL.h"
#include "asset.h"
#include "bmd.h"
#include "cleanup.h"
#include "file.h"
#include "gl.h"
#include "logging.h"
#include "math.h"
#include "renderable.h"
#include "shader.h"
#include "string.h"
#include "texture.h"
#include "transformation.h"

// TODO(paulherman): Disable for release.
extern "C" {
_declspec(dllexport) uint32_t NvOptimusEnablement = 0x00000001;
}

namespace fs = ::std::filesystem;

int main(int argc, char *argv[]) {
  if (argc != 2) {
    LOG(FATAL) << "Usage: " << argv[0] << " <path>";
  }
  fs::path model_path = argv[1];
  fs::path data_dir = fs::path(model_path).remove_filename();

  int sdl_init_err = SDL_Init(SDL_INIT_EVERYTHING);
  LOG_IF(FATAL, sdl_init_err != 0)
      << "failed to initialize SDL2: " << sdl_init_err;
  util::Cleanup cleanup_sdl([]() { SDL_Quit(); });

  SDL_Window *wnd = SDL_CreateWindow("Image Viewer", SDL_WINDOWPOS_CENTERED,
                                     SDL_WINDOWPOS_CENTERED, 800, 600,
                                     SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
  LOG_IF(FATAL, !wnd) << "Failed to create SDL2 window";
  util::Cleanup cleanup_wnd([&]() { SDL_DestroyWindow(wnd); });

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GLContext gl = SDL_GL_CreateContext(wnd);
  LOG_IF(FATAL, !gl) << "Failed to create OpenGL context: " << SDL_GetError();
  util::Cleanup cleanup_gl([&]() { SDL_GL_DeleteContext(gl); });

  gl::Gl mygl;
  gl::ShaderProgram shader(gl::kVertexShaderCode, gl::kFragmentShaderCode);

  data::Model model = data::LoadModel(model_path.string());

  std::vector<gl::Renderable> renderables;
  std::vector<gl::Texture> textures;
  for (const data::Mesh &mesh : model.meshes) {
    renderables.emplace_back(mesh);
    fs::path texture_path = data_dir / mesh.texture_path;
    texture_path.replace_extension(
        data::ImageToTextureExtension(texture_path.extension().string()));
    textures.emplace_back(data::LoadImage(texture_path.string()));
  }

  math::Vec3f rotatVion;
  math::Vec3f translation;
  math::Mat4f projection =
      math::ProjectionMatrix(gl::AspectRatio(), 1.22173f, 0.1f, 1000.0f);

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
    for (size_t i = 0; i < model.meshes.size(); ++i) {
      renderables[i].Render(shader, textures[i], projection);
    }
    SDL_GL_SwapWindow(wnd);
  }
  return EXIT_SUCCESS;
}
