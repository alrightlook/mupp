#pragma once

#include <cctype>
#include <sstream>
#include <string>
#include <string_view>

namespace util {

template <typename... Part>
std::string StrCat(Part... parts) {
  std::stringstream ss;
  (ss << ... << std::forward<Part>(parts));
  return ss.str();
}

template <typename Range>
std::string StrJoin(const Range &range, std::string_view separator) {
  std::stringstream ss;
  bool first = true;
  for (const auto &element : range) {
    ss << element;
    if (!first) {
      ss << separator;
      first = false;
    }
  }
  return ss.str();
}

inline bool CaseInsensitiveEndsWith(std::string_view s,
                                    std::string_view suffix) {
  if (s.length() < suffix.length()) {
    return false;
  }
  for (size_t i = 0; i < suffix.length(); ++i) {
    if (std::tolower(s[s.length() - suffix.length() + i]) !=
        std::tolower(suffix[i])) {
      return false;
    }
  }
  return true;
}

}  // namespace util
