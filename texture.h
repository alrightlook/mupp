#pragma once

#include <string_view>
#include "absl/strings/str_cat.h"
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
  Texture(const Texture&) = delete;
  Texture(Texture&& rhs);

  ~Texture();

  void Bind() const;
  void Unbind() const;

  std::string DebugString() const { return absl::StrCat("Texture ", id_); }

 private:
  GLuint id_;
};

}  // namespace gl
