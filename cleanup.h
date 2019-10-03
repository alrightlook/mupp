#pragma once

#include <functional>

namespace util {

void NoCleanup() {}

class Cleanup final {
 public:
  Cleanup() : f_(&NoCleanup) {}
  Cleanup(std::function<void()> f) : f_(std::move(f)) {}
  ~Cleanup() { f_(); }

 private:
  std::function<void()> f_;
};

}  // namespace util
