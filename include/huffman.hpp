#pragma once

#include <fstream>

#include <non_copyable.hpp>

namespace huffman {
using byte = char;

class Huffman : public NonCopyable {
public:
  Huffman();

public:
  void Encode(std::string inFile, std::string outFile);

  void Decode(std::ifstream in, std::ofstream out);
};
} // namespace huffman