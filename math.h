#include <array>
#include <utility>

#pragma once

namespace math {

template <typename T, size_t size>
class Vec final {
 public:
  constexpr Vec() : data_({}) {}
  constexpr Vec(const Vec<T, size> &rhs) = default;
  constexpr Vec(Vec<T, size> &&rhs) = default;
  constexpr Vec<T, size> &operator=(const Vec<T, size> &rhs) = default;
  constexpr Vec<T, size> &operator=(Vec<T, size> &&rhs) = default;

  template <typename... Args>
  constexpr Vec(Args... data) : data_({{std::forward<Args>(data)...}}) {}

  T &operator[](size_t i) { return data_[i]; }
  const T &operator[](size_t i) const { return data_[i]; }

  Vec<T, size> operator+(const Vec<T, size> &rhs) {
    Vec<T, size> result;
    for (size_t i = 0; i < size; ++i) {
      result[i] = (*this)[i] + rhs[i];
    }
  }
  Vec<T, size> operator-(const Vec<T, size> &rhs) {
    Vec<T, size> result;
    for (size_t i = 0; i < size; ++i) {
      result[i] = (*this)[i] - rhs[i];
    }
  }

  T operator*(const Vec<T, size> &rhs) {
    T result{};
    for (size_t i = 0; i < size; ++i) {
      result += (*this)[i] * rhs[i];
    }
    return result;
  }

 private:
  std::array<T, size> data_;
};

template <typename T, size_t size>
std::ostream &operator<<(std::ostream &os, const Vec<T, size> &v) {
  os << "{";
  for (size_t i = 0; i < size; ++i) {
    if (i != 0) {
      os << ", ";
    }
    os << v[i];
  }
  os << "}";
  return os;
}

template <typename T, size_t size>
bool operator==(const Vec<T, size> &lhs, const Vec<T, size> &rhs) {
  for (size_t i = 0; i < size; ++i) {
    if (lhs[i] != rhs[i]) {
      return false;
    }
  }
  return true;
}

using Vec2f = Vec<float, 2>;
using Vec3i = Vec<int32_t, 3>;
using Vec3f = Vec<float, 3>;
using Vec4f = Vec<float, 4>;

template <typename T, size_t size>
class Mat final {
 public:
  Mat() : data_() {}

  template <typename... Args>
  constexpr Mat(Args... data) : data_({{std::forward<Args>(data)...}}) {}

  T &operator[](std::pair<size_t, size_t> position) {
    return data_[position.first][position.second];
  }

  Mat<T, size> operator*(const Mat<T, size> &rhs) {
    Mat<T, size> result;
    Mat<T, size> transposed_rhs = rhs.Transpose();
    for (size_t row = 0; row < size; ++row) {
      for (size_t col = 0; col < size; ++col) {
        result[{row, col}] = data_[row] * transposed_rhs.data_[col];
      }
    }
    return result;
  }

  Vec<T, size> operator*(const Vec<T, size> &rhs) {
    Vec<T, size> result;
    for (size_t i = 0; i < size; ++i) {
      result[i] = data_[i] * rhs;
    }
    return result;
  }

  Mat<T, size> Transpose() const {
    Mat<T, size> result;
    for (size_t row = 0; row < size; ++row) {
      for (size_t col = 0; col < size; ++col) {
        result.data_[row][col] = data_[col][row];
      }
    }
    return result;
  }

  static Mat<T, size> Identity(const T &identity) {
    Mat<T, size> result;
    for (size_t i = 0; i < size; ++i) {
      result.data_[i][i] = identity;
    }
    return result;
  }

 private:
  std::array<Vec<T, size>, size> data_;
};

using Mat2f = Mat<float, 2>;
using Mat3f = Mat<float, 3>;
using Mat4f = Mat<float, 4>;

template <typename T, size_t size>
std::ostream &operator<<(std::ostream &os, const Mat<T, size> &m) {
  os << "{";
  for (size_t row = 0; row < size; ++row) {
    if (row != 0) {
      os << ", "
    }
    os << "{";
    for (size_t col = 0; col < size; ++col) {
      if (col != 0) {
        os << ", ";
      }
      os << m[{row, col}];
    }
    os << "}";
  }
  os << "}";
  return os;
}

}  // namespace math

namespace std {

template <typename T, size_t size>
struct hash<math::Vec<T, size>> {
  size_t operator()(const ::math::Vec<T, size> &v) const {
    size_t result = 0;
    for (size_t i = 0; i < size; ++i) {
      result ^= v[i];
    }
    return result;
  }
};

}  // namespace std
