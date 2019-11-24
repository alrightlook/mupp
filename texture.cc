#include "texture.h"

#include <memory>

#include "error.h"
#include "file.h"
#define STB_IMAGE_IMPLEMENTATION
#include "glad.h"
#include "stb_image.h"
#include "string.h"

namespace data {
namespace {

Image LoadImage(std::string_view path, size_t skip_bytes) {
  std::vector<unsigned char> raw = data::FileGetBinaryContents(path);
  int width, height, file_components;
  std::unique_ptr<unsigned char> pixels(
      stbi_load_from_memory(&raw[skip_bytes], raw.size() - skip_bytes, &width,
                            &height, &file_components, 4));
  if (!pixels) {
    throw util::InternalException(util::StrCat(
        "failed to read image skipping ", skip_bytes, " bytes from ", path));
  }
  size_t num_bytes = 4 * width * height * sizeof(unsigned char);
  return {
      std::vector<unsigned char>(pixels.get(), pixels.get() + num_bytes),
      static_cast<size_t>(width),
      static_cast<size_t>(height),
  };
}

}  // namespace

Image LoadOZJ(std::string_view path) { return LoadImage(path, 24); }

Image LoadOZB(std::string_view path) { return LoadImage(path, 4); }

Image LoadOZT(std::string_view path) { return LoadImage(path, 4); }

Image LoadImage(std::string_view path) {
  if (util::CaseInsensitiveEndsWith(path, ".ozj")) {
    return LoadOZJ(path);
  } else if (util::CaseInsensitiveEndsWith(path, ".ozt")) {
    return LoadOZT(path);
  } else if (util::CaseInsensitiveEndsWith(path, ".ozb")) {
    return LoadOZB(path);
  }
  return LoadImage(path, 0);
}

}  // namespace data

namespace gl {

Texture::Texture(const data::Image& image) {
  glCreateTextures(GL_TEXTURE_2D, 1, &id_);

  glTextureParameteri(id_, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTextureParameteri(id_, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTextureParameteri(id_, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTextureParameteri(id_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTextureStorage2D(id_, 1, GL_RGBA8, image.width, image.height);
  glTextureSubImage2D(id_, 0, 0, 0, image.width, image.height, GL_RGBA,
                      GL_UNSIGNED_BYTE, image.pixels.data());
}

Texture::~Texture() { glDeleteTextures(1, &id_); }

void Texture::Bind() const { glBindTextureUnit(0, id_); }

void Texture::Unbind() const { glBindTextureUnit(0, 0); }

}  // namespace gl
