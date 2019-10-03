#include "transformation.h"

namespace math {

math::Mat4f ProjectionMatrix(float aspect_ratio, float fov, float clip_near,
                             float clip_far) {
  float y_scale = aspect_ratio / tanf(fov / 2.0);
  float x_scale = y_scale / aspect_ratio;
  float length = clip_far - clip_near;
  return math::Mat4f(
      math::Vec4f{x_scale, 0.0f, 0.0f, 0.0f},                               //
      math::Vec4f{0.0f, y_scale, 0.0f, 0.0f},                               //
      math::Vec4f{0.0f, 0.0f, -(clip_near + clip_far) / length, -1.0f},     //
      math::Vec4f{0.0f, 0.0f, -2.0f * clip_near * clip_far / length, 0.0f}  //
  );
}

math::Mat4f TranslationMatrix(const math::Vec3f& translation) {
  return math::Mat4f(math::Vec4f{1.0f, 0.0f, 0.0f, translation[0]},  //
                     math::Vec4f{0.0f, 1.0f, 0.0f, translation[1]},  //
                     math::Vec4f{0.0f, 0.0f, 1.0f, translation[2]},  //
                     math::Vec4f{0.0f, 0.0f, 0.0f, 1.0f}             //
  );
}

math::Mat4f ScalingMatrix(float scale) {
  return math::Mat4f(math::Vec4f{scale, 0.0f, 0.0f, 0.0f},  //
                     math::Vec4f{0.0f, scale, 0.0f, 0.0f},  //
                     math::Vec4f{0.0f, 0.0f, scale, 0.0f},  //
                     math::Vec4f{0.0f, 0.0f, 0.0f, 1.0f}    //
  );
}

math::Mat4f RotationMatrix(const math::Vec3f& rotation) {
  math::Mat4f rotation_x(
      math::Vec4f{1.0f, 0.0f, 0.0f, 0.0f},                           //
      math::Vec4f{0.0f, cos(rotation[0]), -sin(rotation[0]), 0.0f},  //
      math::Vec4f{0.0f, sin(rotation[0]), cos(rotation[0]), 0.0f},   //
      math::Vec4f{0.0f, 0.0f, 0.0f, 1.0f}                            //
  );
  math::Mat4f rotation_y(
      math::Vec4f{cos(rotation[1]), 0.0f, sin(rotation[1]), 0.0f},   //
      math::Vec4f{0.0f, 1.0f, 0.0f, 0.0f},                           //
      math::Vec4f{-sin(rotation[1]), 0.0f, cos(rotation[1]), 0.0f},  //
      math::Vec4f{0.0f, 0.0f, 0.0f, 1.0f}                            //
  );
  math::Mat4f rotation_z(
      math::Vec4f{cos(rotation[2]), -sin(rotation[2]), 0.0f, 0.0f},  //
      math::Vec4f{sin(rotation[2]), cos(rotation[2]), 0.0f, 0.0f},   //
      math::Vec4f{0.0f, 0.0f, 1.0f, 0.0f},                           //
      math::Vec4f{0.0f, 0.0f, 0.0f, 1.0f}                            //
  );
  return rotation_x * rotation_y * rotation_z;
}

}  // namespace math
