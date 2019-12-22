#include "absl/time/time.h"

namespace util {

class Ticker final {
 public:
  Ticker() : last_(absl::Now()) {}

  absl::Duration Tick() {
    absl::Time now = absl::Now();
    absl::Duration delta = now - last_;
    last_ = now;
    return delta;
  }

 private:
  absl::Time last_;
};

}  // namespace util