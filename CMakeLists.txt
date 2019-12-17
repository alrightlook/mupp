cmake_minimum_required (VERSION 3.8)

project(mupp)

set(CMAKE_CXX_STANDARD 17)

find_package(glew REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(absl CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_library(util STATIC error.cc file.cc file.h cleanup.h error.h logging.h macro.h math.h string.h struct.h stb_image.h transformation.cc transformation.h serialization.cc serialization.h asset.cc asset.h)
target_link_libraries(util PUBLIC absl::base absl::time absl::span absl::strings)

add_executable(client client.cc gl.cc bmd.cc glad.c glad.h bmd.h renderable.cc renderable.h texture.cc texture.h shader.cc shader.h)
target_link_libraries(client PRIVATE absl::strings util SDL2::SDL2 SDL2::SDL2main GLEW::GLEW)

add_executable(image_viewer image_viewer.cc gl.cc glad.c glad.h renderable.cc renderable.h texture.cc texture.h shader.cc shader.h)
target_link_libraries(image_viewer PRIVATE absl::strings util SDL2::SDL2 SDL2::SDL2main GLEW::GLEW)
# target_link_libraries(main PRIVATE glm)

add_executable(model_viewer model_viewer.cc gl.cc glad.c glad.h renderable.cc renderable.h texture.cc texture.h shader.cc shader.h bmd.cc bmd.h)
target_link_libraries(model_viewer PRIVATE absl::strings util SDL2::SDL2 SDL2::SDL2main GLEW::GLEW)

add_executable(asset_packer asset_packer.cc)
target_link_libraries(asset_packer PRIVATE util absl::strings)

add_executable(asset_dump asset_dump.cc)
target_link_libraries(asset_dump PRIVATE util absl::strings)