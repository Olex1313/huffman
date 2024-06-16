
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
  if (!ifs.is_open()) {
    throw std::runtime_error("unable to open file: " + inFile);
  }

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

  symbolTree.Dump(ofs);
  while (ifs.get(symbol)) {
    std::string code = symbolTree.GetCode(symbol);
    for (char c : code) {
      ofs.write(&c, 1);
    }
  }

  ofs.close();
  ifs.close();
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
  TreeNode *curNode = symbolTree.GetRoot();

  while (ifs.get(symbol)) {
    if (curNode == nullptr) {
      throw std::runtime_error(
          "decode failed, probably due to corrupted symbol sequence");
    }

    if (symbol == '0') {
      curNode = curNode->left.get();
    } else {
      curNode = curNode->right.get();
    }

    if (curNode->IsLeaf()) {
      char c = curNode->symbol;
      ofs.write((const char *)&c, sizeof(c));
      curNode = symbolTree.GetRoot();
    }
  }

  ifs.close();
  ofs.close();
}

}; // namespace huffman