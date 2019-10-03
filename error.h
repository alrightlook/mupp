#pragma once

#include <exception>
#include <string>
#include <string_view>
#include "string.h"

namespace util {

enum class CanonicalErrorCode {
  kUnknown,
  kInternal,
  kAborted,
  kNotFound,
  kUnimplemented,
};

std::string_view CanonicalErrorCodeName(CanonicalErrorCode code);

class AnyCanonicalException : public std::runtime_error {
 public:
  AnyCanonicalException(CanonicalErrorCode code, std::string_view message)
      : runtime_error(util::StrCat("[", CanonicalErrorCodeName(code), "] ",
                                   message.data())) {}
};

template <CanonicalErrorCode code>
class CanonicalException : public AnyCanonicalException {
 public:
  CanonicalException(std::string_view message)
      : AnyCanonicalException(code, message) {}
};

using UnknownException = CanonicalException<CanonicalErrorCode::kUnknown>;
using InternalException = CanonicalException<CanonicalErrorCode::kInternal>;
using AbortedException = CanonicalException<CanonicalErrorCode::kAborted>;
using NotFoundException = CanonicalException<CanonicalErrorCode::kNotFound>;
using UnimplementedException =
    CanonicalException<CanonicalErrorCode::kUnimplemented>;

}  // namespace util
