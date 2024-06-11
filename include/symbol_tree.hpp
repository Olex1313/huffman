#pragma once

#include "non_copyable.hpp"
#include <fstream>
#include <map>
#include <string>

namespace huffman {

const char FILETAG[] = "HUF";

struct TreeNode {
public:
  TreeNode(byte symbol, int64_t weight);

  TreeNode(const std::shared_ptr<TreeNode> &left,
           const std::shared_ptr<TreeNode> &right);

  bool IsLeaf() const;

  bool operator<(const TreeNode &other) const;
  bool operator>(const TreeNode &other) const;

public:
  std::shared_ptr<TreeNode> left;
  std::shared_ptr<TreeNode> right;
  byte symbol;
  int64_t weight;
};

class SymbolTree : public NonCopyable {
public:
  SymbolTree(const std::map<byte, uint64_t> &symbolFreqMap);

public:
  std::string GetCode(byte symbol) const;

  void Dump(std::ofstream &ofs) const;

  void Load(std::ifstream &ifs);

private:
  void BuildCodeTable(TreeNode *node, std::string code);

private:
  std::map<byte, uint64_t> alphabet;
  std::shared_ptr<TreeNode> root;
  std::map<byte, std::string> prefixCodes;
};
} // namespace huffman