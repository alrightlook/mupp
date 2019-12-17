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

}  // namespace data

namespace gl {

class Texture {
 public:
  Texture(const data::Image& image);
  Texture() = delete;
  Texture(const Texture& rhs) = delete;
  Texture(Texture&& rhs) = default;

  ~Texture();

  void Bind() const;
  void Unbind() const;

 private:
  GLuint id_;
};

}  // namespace gl
