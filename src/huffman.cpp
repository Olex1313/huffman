
#include <fstream>
#include <huffman.hpp>
#include <ios>
#include <symbol_tree.hpp>

namespace huffman {
/*
First pass:
- read file by bytes
- build symbol frequency map
- build huffman tree
- build huffman codes for each char
Second pass:
- write header for out file
- read in file by bytes
  - get code for char
  - write code to out file
- end
*/
void Huffman::Encode(std::string inFile, std::string outFile) {
  std::ifstream ifs(inFile, std::ios::binary);
  byte symbol;
  std::map<byte, uint64_t> symbolFreqMap;

  while (ifs.get(symbol)) {
    symbolFreqMap[symbol]++;
  }
  ifs.clear(); // due to reuse of file stream
  ifs.seekg(0);

  SymbolTree symbolTree(symbolFreqMap);

  std::ofstream ofs(outFile, std::ios::binary);

  // symbolTree.WriteHeader(ofs);

  while (ifs.get(symbol)) {
    std::string code = symbolTree.GetCode(symbol);
    for (char c : code) {
      ofs.write(&c, 1);
    }
  }

  ofs.close();
  ifs.close();
}
}; // namespace huffman