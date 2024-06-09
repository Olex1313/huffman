#include "huffman.hpp"
#include <queue>
#include <symbol_tree.hpp>

namespace huffman {

TreeNode::TreeNode(const TreeNode &other)
    : weight(other.weight), symbol(other.symbol), right(other.right),
      left(other.left) {}

TreeNode::TreeNode(const TreeNode &left, const TreeNode &right)
    : weight(left.weight + left.weight), symbol(-1),
      right(std::make_shared<TreeNode>(right)),
      left(std::make_shared<TreeNode>(left)) {}

bool TreeNode::operator<(const TreeNode &other) const {
  return weight > other.weight;
}

bool TreeNode::IsLeaf() const { return right == nullptr && left == nullptr; }

SymbolTree::SymbolTree(const std::map<byte, uint64_t> &symbolFreqMap)
    : prefixCodes() {
  std::priority_queue<TreeNode> pq;

  for (const auto &[symbol, frequency] : symbolFreqMap) {
    pq.push(TreeNode(symbol, frequency));
  }

  while (pq.size() > 1) {
    TreeNode left = std::move(pq.top());
    pq.pop();

    TreeNode right = std::move(pq.top());
    pq.pop();

    pq.push(TreeNode(left, right));
  }

  root = std::make_unique<TreeNode>(pq.top());
  pq.pop();

  std::string cleanCodeStr;
  cleanCodeStr.clear();

  BuildCodeTable(root.get(), cleanCodeStr);
}

void SymbolTree::BuildCodeTable(TreeNode *node, std::string accumulatedCode) {
  if (node->IsLeaf() && node->symbol >= 0) {
    prefixCodes[node->symbol] = accumulatedCode;
    return;
  }

  if (node->left != nullptr) {
    BuildCodeTable(node->left.get(), accumulatedCode + '0');
  }

  if (node->right != nullptr) {
    BuildCodeTable(node->right.get(), accumulatedCode + '1');
  }
}

std::string SymbolTree::GetCode(byte symbol) const {
  return prefixCodes.at(symbol);
}
} // namespace huffman