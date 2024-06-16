
#include <bitset>
#include <filesystem>
#include <fstream>
#include <huffman.hpp>
#include <ios>
#include <iostream>
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
  if (!ifs.is_open()) {
    throw std::runtime_error("unable to open file: " + inFile);
  }
  std::filesystem::path p(inFile);
  std::cout << std::filesystem::file_size(p) << std::endl;

  byte symbol;
  std::map<byte, uint64_t> symbolFreqMap;
  while (ifs.get(symbol)) {
    symbolFreqMap[symbol]++;
  }
  ifs.clear(); // due to reuse of file stream
  ifs.seekg(0);

  SymbolTree symbolTree(symbolFreqMap);

  std::ofstream ofs(outFile, std::ios::binary);
  if (!ofs.is_open()) {
    throw std::runtime_error("unable to open file: " + outFile);
  }

  long headerSize = symbolTree.GetCompressedHeaderBytes();
  symbolTree.Dump(ofs);

  std::string outByteBuff("");
  while (ifs) {
    ifs.get(symbol);
    outByteBuff += symbolTree.GetCode(symbol);
    if (outByteBuff.length() >= 8) {
      std::bitset<8> bset(outByteBuff.substr(0, 8));
      outByteBuff.erase(0, 8);
      ofs.write((const char *)&bset, 1);
    }
  }
  ofs.close();
  ifs.close();

  std::filesystem::path o(outFile);
  long outFileSize = std::filesystem::file_size(o);

  std::cout << outFileSize - headerSize << std::endl;
  std::cout << headerSize << std::endl;
}

void Huffman::Decode(std::string sourceFileName, std::string destFileName) {
  std::ifstream ifs(sourceFileName, std::ios::binary);
  if (!ifs.is_open()) {
    throw std::runtime_error("unable to open file: " + sourceFileName);
  }
  SymbolTree symbolTree(ifs);

  std::ofstream ofs(destFileName, std::ios::out);
  if (!ofs.is_open()) {
    throw std::runtime_error("unable to open file: " + destFileName);
  }

  byte symbol;
  unsigned char mask = 0x80;
  unsigned char code = 0;
  TreeNode *curNode = symbolTree.GetRoot();

  uintmax_t totalBytes = symbolTree.GetTotalCodedBits() / 8;
  unsigned char extraBits = symbolTree.GetTotalCodedBits() % 8;

  for (uintmax_t i = 0; i < totalBytes; i++) {
    ifs.get(symbol);

    for (unsigned char offset = 0; offset < 8; offset++) {
      code = (symbol & mask) ? 1 : 0;
      curNode = (code) ? curNode->right.get() : curNode->left.get();
      if (curNode->IsLeaf()) {
        char c = curNode->symbol;
        ofs.write((const char *)&c, 1);
        curNode = symbolTree.GetRoot();
      }
      symbol = symbol << 1;
    }
  }

  if (extraBits > 0) {
    ifs.get(symbol);
    for (unsigned char offset = 0; offset < extraBits; offset++) {
      code = (symbol & mask) ? 1 : 0;
      curNode = (code) ? curNode->right.get() : curNode->left.get();
      if (curNode->IsLeaf()) {
        char c = curNode->symbol;
        ofs.write((const char *)&c, 1);
        curNode = symbolTree.GetRoot();
      }
      symbol = symbol << 1;
    }
  }

  ifs.close();
  ofs.close();
}

}; // namespace huffman