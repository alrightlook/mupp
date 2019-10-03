#pragma once

#include <string_view>
#include "glad.h"
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

Image LoadOZJ(std::string_view path);
Image LoadOZB(std::string_view path);
Image LoadOZT(std::string_view path);

}  // namespace data

namespace gl {

class Texture {
 public:
  Texture(const data::Image& image);
  Texture(const Texture& rhs) = delete;

  ~Texture();

  void Bind() const;
  void Unbind() const;

 private:
  GLuint id_;
};

}  // namespace gl