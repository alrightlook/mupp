﻿cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/third_party")

find_package(glew REQUIRED)
find_package(SDL2 CONFIG REQUIRED)

add_library(util STATIC error.cc file.cc file.h cleanup.h error.h logging.h macro.h math.h string.h struct.h stb_image.h transformation.cc transformation.h)

add_executable(client client.cc gl.cc bmd.cc glad.c glad.h bmd.h renderable.cc renderable.h texture.cc texture.h shader.cc shader.h)
target_link_libraries(client PRIVATE util)
target_link_libraries(client PRIVATE SDL2::SDL2 SDL2::SDL2main)
target_link_libraries(client PRIVATE GLEW::GLEW)

add_executable(image_viewer image_viewer.cc gl.cc glad.c glad.h renderable.cc renderable.h texture.cc texture.h shader.cc shader.h)
target_link_libraries(image_viewer PRIVATE util)
target_link_libraries(image_viewer PRIVATE SDL2::SDL2 SDL2::SDL2main)
target_link_libraries(image_viewer PRIVATE GLEW::GLEW)