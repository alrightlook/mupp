#include <ostream>
#include <streambuf>

namespace util {

class NullStream : public std::ostream {
 public:
  NullStream() : std::ostream(&buf_) {}

 private:
  class NullBuffer : public std::streambuf {
   public:
    int overflow(int c) { return c; }
  };
  NullBuffer buf_;
};

}  // namespace util