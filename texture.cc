#include "texture.h"

#include <map>
#include <memory>

#include "error.h"
#include "file.h"
#include "gl.h"
#include "logging.h"
#define STB_IMAGE_IMPLEMENTATION
#include "absl/strings/str_replace.h"
#include "glad.h"
#include "stb_image.h"
#include "string.h"

namespace data {
namespace {

Image LoadImage(const AssetStore& assets, std::string_view path,
                size_t skip_bytes) {
  const std::vector<unsigned char>& raw = assets.GetBinaryData(path);
  int width, height, file_components;
  stbi_set_flip_vertically_on_load(1);
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

Image LoadOZJ(const AssetStore& assets, std::string_view path) {
  return LoadImage(assets, path, 24);
}

Image LoadOZB(const AssetStore& assets, std::string_view path) {
  return LoadImage(assets, path, 4);
}

Image LoadOZT(const AssetStore& assets, std::string_view path) {
  return LoadImage(assets, path, 4);
}

Image LoadImage(const AssetStore& assets, std::string_view path) {
  std::map<std::string, std::string> extension_replacements = {
      {".jpg", ".ozj"},
      {".bmp", ".ozb"},
      {".tga", ".ozt"},
  };
  std::string converted_path =
      absl::StrReplaceAll(path, extension_replacements);
  if (util::CaseInsensitiveEndsWith(path, ".ozj") ||
      util::CaseInsensitiveEndsWith(path, ".jpg")) {
    return LoadOZJ(assets, converted_path);
  } else if (util::CaseInsensitiveEndsWith(path, ".ozt") ||
             util::CaseInsensitiveEndsWith(path, ".tga")) {
    return LoadOZT(assets, converted_path);
  } else if (util::CaseInsensitiveEndsWith(path, ".ozb") ||
             util::CaseInsensitiveEndsWith(path, ".bmp")) {
    return LoadOZB(assets, converted_path);
  }
  return LoadImage(assets, path, 0);
}

}  // namespace data

namespace gl {

Texture::Texture(const data::Image& image) {
  glGenTextures(1, &id_);
  gl::Bind bind(this);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, image.pixels.data());
  LOG(INFO) << "Created texture " << id_;
}

Texture::Texture(Texture&& rhs) : id_(rhs.id_) {
  rhs.id_ = 0;
}

Texture::~Texture() {
  if (id_ == 0) {
    return;
  }
  glDeleteTextures(1, &id_);
  LOG(INFO) << "Deleted texture " << id_;
 }

void Texture::Bind() const { glBindTexture(GL_TEXTURE_2D, id_); }

void Texture::Unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }

}  // namespace gl
