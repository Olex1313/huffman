#pragma once

#include <non_copyable.hpp>
#include <string>

namespace huffman {

class Huffman : public NonCopyable {
public:
  Huffman() = default;

public:
  void Encode(std::string inFile, std::string outFile);

  void Decode(std::string inFile, std::string outFile);
};
} // namespace huffman