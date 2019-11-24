#include "error.h"

namespace util {

std::string_view CanonicalErrorCodeName(CanonicalErrorCode code) {
  switch (code) {
    case CanonicalErrorCode::kUnknown:
      return "Unknown";
    case CanonicalErrorCode::kInternal:
      return "Internal";
    case CanonicalErrorCode::kAborted:
      return "Aborted";
    case CanonicalErrorCode::kNotFound:
      return "NotFound";
    case CanonicalErrorCode::kInvalidArgument:
      return "InvalidArgument";
    case CanonicalErrorCode::kUnimplemented:
      return "Unimplemented";
  }
  return "<unknown error code>";
};

}  // namespace util
