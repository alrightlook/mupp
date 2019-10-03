#pragma once

#include <cmath>
#include "math.h"

namespace math {

math::Mat4f ProjectionMatrix(float aspect_ratio, float fov, float clip_near,
                             float clip_far);
math::Mat4f TranslationMatrix(const math::Vec3f& translation);
math::Mat4f ScalingMatrix(float scale);
math::Mat4f RotationMatrix(const math::Vec3f& rotation);

}  // namespace math