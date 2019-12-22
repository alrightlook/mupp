
#include <algorithm>
#include <cstdlib>
#include <string_view>

#include "SDL2/SDL.h"
#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/time/time.h"
#include "bmd.h"
#include "cleanup.h"
#include "file.h"
#include "gl.h"
#include "glm/gtc/matrix_transform.hpp"
#include "logging.h"
#include "math.h"
#include "renderable.h"
#include "shader.h"
#include "string.h"
#include "texture.h"
#include "ticker.h"

ABSL_FLAG(std::string, assets_path, "", "Path to the assets store.");
ABSL_FLAG(std::string, model, "", "Which model to render.");

constexpr glm::vec3 kUpDirection = glm::vec3(0.0f, 1.0f, 0.0f);
constexpr glm::vec3 kFrontDirection = glm::vec3(0.0f, 0.0f, 1.0f);

bool HandleMovement(absl::Duration frame, glm::vec3 *pos, glm::vec3 *target) {
  constexpr float kSpeed = 100.0f;
  float frame_s = absl::ToDoubleSeconds(frame);
  bool moved = false;

  const uint8_t *keyboard_state = SDL_GetKeyboardState(NULL);
  if (keyboard_state[SDL_SCANCODE_W]) {
    *pos += frame_s * kSpeed * kFrontDirection;
    moved = true;
  }
  if (keyboard_state[SDL_SCANCODE_S]) {
    *pos -= frame_s * kSpeed * kFrontDirection;
    moved = true;
  }
  if (keyboard_state[SDL_SCANCODE_A]) {
    *pos -= frame_s * kSpeed *
            glm::normalize(glm::cross(kFrontDirection, kUpDirection));
    moved = true;
  }
  if (keyboard_state[SDL_SCANCODE_D]) {
    *pos -= frame_s * kSpeed *
            glm::normalize(glm::cross(kFrontDirection, kUpDirection));
    moved = true;
  }
  if (keyboard_state[SDL_SCANCODE_Q]) {
    pos->y -= frame_s * kSpeed;
    moved = true;
  }
  if (keyboard_state[SDL_SCANCODE_E]) {
    pos->y += frame_s * kSpeed;
    moved = true;
  }

  if (keyboard_state[SDL_SCANCODE_UP]) {
    moved = true;
  }
  if (keyboard_state[SDL_SCANCODE_DOWN]) {
    moved = true;
  }
  if (keyboard_state[SDL_SCANCODE_LEFT]) {
    moved = true;
  }
  if (keyboard_state[SDL_SCANCODE_RIGHT]) {
    moved = true;
  }
  LOG_IF(DEBUG, moved) << "New position " << *pos << " " << *target;
  return moved;
}

int main(int argc, char *argv[]) {
  absl::ParseCommandLine(argc, argv);
  LOG(DEBUG) << "Assets: " << absl::GetFlag(FLAGS_assets_path);
  LOG(DEBUG) << "Model: " << absl::GetFlag(FLAGS_model);

  data::AssetStore asset_store =
      data::AssetStore::LoadFromBinary(absl::GetFlag(FLAGS_assets_path));

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

  data::Model model = data::LoadModel(asset_store, absl::GetFlag(FLAGS_model));

  std::vector<gl::Renderable> renderables;
  std::vector<gl::Texture> textures;
  for (const data::Mesh &mesh : model.meshes) {
    renderables.emplace_back(mesh);
    textures.emplace_back(data::LoadImage(
        asset_store, data::JoinPath(data::DirName(absl::GetFlag(FLAGS_model)),
                                    mesh.texture_path)));
  }

  glm::mat4 projection =
      glm::perspective(glm::radians(60.0f), gl::AspectRatio(), 0.1f, 1000.0f);
  glm::vec3 camera_pos(0.0f, 0.0f, -1000.0f);
  glm::vec3 camera_target;
  {
    gl::Bind bind_shader(&shader);
    shader.SetUniform("projection", projection);
    shader.SetUniform("model", glm::mat4(1.0f));
    glm::mat4 view = glm::lookAt(camera_pos, camera_target, kUpDirection);
    shader.SetUniform("view", view);
  }

  util::Ticker ticker;
  while (true) {
    absl::Duration frame_time = ticker.Tick();
    LOG(DEBUG) << "Frame " << frame_time;

    if (HandleMovement(frame_time, &camera_pos, &camera_target)) {
      gl::Bind bind_shader(&shader);
      glm::mat4 view =
          glm::lookAt(camera_pos, camera_target, glm::vec3(0.0f, 1.0f, 0.0f));
      shader.SetUniform("view", view);
    }

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
