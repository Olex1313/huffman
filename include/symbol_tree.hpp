#include "huffman.hpp"
#include <fstream>
#include <map>

namespace huffman {
struct TreeNode {
public:
  TreeNode(byte symbol, int64_t weight);

  TreeNode(const TreeNode &left, const TreeNode &right);

  TreeNode(const TreeNode &other);

  bool IsLeaf() const;

  bool operator<(const TreeNode &other) const;

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

private:
  void BuildCodeTable(TreeNode *node, std::string code);

private:
  std::unique_ptr<TreeNode> root;
  std::map<byte, std::string> prefixCodes;
};
} // namespace huffman