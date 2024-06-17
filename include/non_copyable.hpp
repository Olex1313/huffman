#pragma once
#include <stdint.h>

namespace huffman {
using byte = std::uint8_t;

class NonCopyable {
protected:
  NonCopyable() {}
  ~NonCopyable() {}

private:
  NonCopyable(const NonCopyable &);
  const NonCopyable &operator=(const NonCopyable &);
};
} // namespace huffman