
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
  uintmax_t outSize = 0;
  uintmax_t inSize = 0;
  std::ifstream ifs(inFile, std::ios::binary);
  if (!ifs.is_open()) {
    throw std::runtime_error("unable to open file: " + inFile);
  }
  std::filesystem::path p(inFile);

  char symbol;
  std::map<byte, uint64_t> symbolFreqMap;
  while (ifs.get(symbol)) {
    inSize++;
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
      outSize++;
    }
  }

  // NOTE should flush buffer, due to non-half byte remaining
  if (outByteBuff.length() > 0) {
    for (size_t i = outByteBuff.length(); i < 8; i++) {
      outByteBuff += "0";
    }
    std::bitset<8> bset(outByteBuff.substr(0, 8));
    ofs.write((const char *)&bset, 1);
    outSize++;
  }

  ofs.close();
  ifs.close();

  std::cout << inSize << std::endl;
  std::cout << outSize << std::endl;
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

  uintmax_t inSize = 0;
  uintmax_t outSize = 0;

  char symbol;
  unsigned char mask = 0x80;
  unsigned char code = 0;
  TreeNode *curNode = symbolTree.GetRoot();

  uintmax_t totalBytes = symbolTree.GetTotalCodedBits() / 8;
  unsigned char extraBits = symbolTree.GetTotalCodedBits() % 8;

  for (uintmax_t i = 0; i < totalBytes; i++) {
    ifs.get(symbol);
    inSize++;

    for (unsigned char offset = 0; offset < 8; offset++) {
      code = (symbol & mask) ? 1 : 0;
      curNode = (code) ? curNode->right.get() : curNode->left.get();
      if (curNode->IsLeaf()) {
        char c = curNode->symbol;
        ofs.write((const char *)&c, 1);
        outSize++;
        curNode = symbolTree.GetRoot();
      }
      symbol = symbol << 1;
    }
  }

  if (extraBits > 0) {
    ifs.get(symbol);
    inSize++;
    for (unsigned char offset = 0; offset < extraBits; offset++) {
      code = (symbol & mask) ? 1 : 0;
      curNode = (code) ? curNode->right.get() : curNode->left.get();
      if (curNode->IsLeaf()) {
        char c = curNode->symbol;
        ofs.write((const char *)&c, 1);
        outSize++;
        curNode = symbolTree.GetRoot();
      }
      symbol = symbol << 1;
    }
  }
  inSize++; // for null symbol

  ifs.close();
  ofs.close();

  std::cout << inSize << std::endl;
  std::cout << outSize << std::endl;
  std::cout << symbolTree.GetCompressedHeaderBytes() << std::endl;
}

}; // namespace huffman