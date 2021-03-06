#include <cstdlib>
#include <string_view>

#include "SDL2/SDL.h"
#include "asset.h"
#include "bmd.h"
#include "cleanup.h"
#include "gl.h"
#include "logging.h"
#include "renderable.h"
#include "shader.h"
#include "string.h"
#include "texture.h"
#include "glm/glm.hpp"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    LOG(FATAL) << "Usage: " << argv[0] << " <assets_path>";
  }
  data::AssetStore asset_store = data::AssetStore::LoadFromBinary(argv[1]);

  int sdl_init_err = SDL_Init(SDL_INIT_EVERYTHING);
  LOG_IF(FATAL, sdl_init_err != 0)
      << "failed to initialize SDL2: " << sdl_init_err;
  util::Cleanup cleanup_sdl([]() { SDL_Quit(); });

  SDL_Window *wnd =
      SDL_CreateWindow("MU", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       1600, 900, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
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
  gl::ShaderProgram shader(gl::kVertexShaderCode, gl::kFragmentShaderCode);

  data::Model m =
      data::LoadModel(asset_store, "Object1/Beer01.bmd");
  std::vector<gl::Renderable> m_renderables;
  for (const data::Mesh &mesh : m.meshes) {
    m_renderables.emplace_back(mesh);
  }

  data::Image i = data::LoadOZT(asset_store, "Object1/bottle.ozt");
  gl::Texture t(i);

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
    for (const gl::Renderable &r : m_renderables) {
      r.Render(shader, t, glm::mat3());
    }
    SDL_GL_SwapWindow(wnd);
  }
  return EXIT_SUCCESS;
}
