
#include <algorithm>
#include <cstdlib>
#include <string_view>

#include "SDL2/SDL.h"
#include "bmd.h"
#include "cleanup.h"
#include "file.h"
#include "gl.h"
#include "logging.h"
#include "math.h"
#include "renderable.h"
#include "shader.h"
#include "glm/gtc/matrix_transform.hpp"
#include "string.h"
#include "texture.h"

int main(int argc, char *argv[]) {
  if (argc != 3) {
    LOG(FATAL) << "Usage: " << argv[0] << " <assets_path> <model>";
  }
  data::AssetStore asset_store = data::AssetStore::LoadFromBinary(argv[1]);

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
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GLContext gl = SDL_GL_CreateContext(wnd);
  LOG_IF(FATAL, !gl) << "Failed to create OpenGL context: " << SDL_GetError();
  util::Cleanup cleanup_gl([&]() { SDL_GL_DeleteContext(gl); });

  gl::Gl mygl;
  gl::ShaderProgram shader(gl::kVertexShaderCode, gl::kFragmentShaderCode);

  data::Model model = data::LoadModel(asset_store, argv[2]);

  std::vector<gl::Renderable> renderables;
  std::vector<gl::Texture> textures;
  for (const data::Mesh &mesh : model.meshes) {
    renderables.emplace_back(mesh);
    textures.emplace_back(data::LoadImage(
        asset_store,
        data::JoinPath(data::DirName(argv[2]), mesh.texture_path)));
  }

  glm::mat4 projection = glm::perspective(60.0f, gl::AspectRatio(), 0.1f, 1000.0f);

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
