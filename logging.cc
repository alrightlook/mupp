#include "logging.h"

#include <iostream>

#include "absl/flags/flag.h"

ABSL_FLAG(bool, debug_logging, false, "Whether debug logging is enabled.");

namespace util {

std::ostream &Log::Debug(std::string_view file, size_t line) {
  static NullStream null_stream;
  if (absl::GetFlag(FLAGS_debug_logging)) {
    return (std::cout << "\nD" << TimeNow() << ":" << FileBasename(file) << ":"
                      << line << " ");
  } else {
    return null_stream;
  }
}

}  // namespace util