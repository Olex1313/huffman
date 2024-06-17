#pragma once

namespace huffman {
using byte = uint8_t;

class NonCopyable {
protected:
  NonCopyable() {}
  ~NonCopyable() {}

private:
  NonCopyable(const NonCopyable &);
  const NonCopyable &operator=(const NonCopyable &);
};
} // namespace huffman