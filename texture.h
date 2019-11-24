#pragma once

#include <string_view>
#include "asset.h"
#include "error.h"
#include "glad.h"
#include "string.h"
#include "struct.h"

namespace data {

struct Image {
  PP_TUPLE_DEFINE_STRUCT(                    //
      (std::vector<unsigned char>, pixels),  //
      (size_t, width),                       //
      (size_t, height)                       //
  );
};
PP_TUPLE_DEFINE_OP(Image, ostream);

Image LoadOZJ(const AssetStore& assets, std::string_view path);
Image LoadOZB(const AssetStore& assets, std::string_view path);
Image LoadOZT(const AssetStore& assets, std::string_view path);
Image LoadImage(const AssetStore& assets, std::string_view path);

inline std::string ImageToTextureExtension(std::string_view image_extension) {
  if (util::CaseInsensitiveEndsWith(image_extension, ".jpg")) {
    return ".ozj";
  }
  if (util::CaseInsensitiveEndsWith(image_extension, ".bmp")) {
    return ".ozb";
  }
  if (util::CaseInsensitiveEndsWith(image_extension, ".tga")) {
    return ".ozt";
  }
  throw util::InvalidArgumentException("unknown MU image extension ");
}

}  // namespace data

namespace gl {

class Texture {
 public:
  Texture(const data::Image& image);
  Texture(const Texture& rhs) = delete;
  Texture(Texture&& rhs) = default;

  ~Texture();

  void Bind() const;
  void Unbind() const;

 private:
  GLuint id_;
};

}  // namespace gl
