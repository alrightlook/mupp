#pragma once

#include <chrono>
#include <exception>
#include <iostream>
#include <string_view>

#include "absl/time/clock.h"
#include "absl/time/time.h"
#include "macro.h"
#include "null_stream.h"

namespace util {

template <typename T>
class HasDebugString {
  using Yes = char[2];
  using No = char[1];

  template <typename C>
  static Yes &Test(decltype(&C::DebugString));
  template <typename C>
  static No &Test(...);

 public:
  enum { Value = sizeof(Test<T>(0)) == sizeof(Yes) };
};

template <typename T>
std::string DebugStringOrEmpty(const T &data) {
  if constexpr (HasDebugString<T>::Value) {
    return data.DebugString();
  } else {
    return "<no debug info>";
  }
}

class Log {
 public:
  static std::ostream &Info(std::string_view file, size_t line) {
    return (std::cout << "\nI" << TimeNow() << ":" << FileBasename(file) << ":"
                      << line << " ");
  }
  static std::ostream &Error(std::string_view file, size_t line) {
    return (std::cerr << "\nE" << TimeNow() << ":" << FileBasename(file) << ":"
                      << line << " ");
  }
  static std::ostream &Warning(std::string_view file, size_t line) {
    return (std::clog << "\nW" << TimeNow() << ":" << FileBasename(file) << ":"
                      << line << " ");
  }
  static std::ostream &Debug(std::string_view file, size_t line);

 private:
  static int64_t TimeNow() { return absl::ToUnixSeconds(absl::Now()); }
  static std::string_view FileBasename(std::string_view file) {
    auto pos = file.find_last_of("\\/");
    if (pos == std::string_view::npos) {
      return file;
    }
    return file.substr(pos + 1);
  }
};

#define LOG_DEBUG ::util::Log::Debug
#define LOG_INFO ::util::Log::Info
#define LOG_ERROR ::util::Log::Error
#define LOG_WARNING ::util::Log::Warning
#define LOG_FATAL \
  for (;; exit(1)) ::util::Log::Error
#define LOG(sev) PP_CAT(LOG_, sev)(__FILE__, __LINE__)
#define LOG_IF(sev, expr) \
  if (!(expr)) {          \
  } else                  \
    LOG(sev)

}  // namespace util
