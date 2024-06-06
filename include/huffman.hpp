#pragma once

namespace huffman {

struct TreeNode {
public:
  TreeNode(char symbol, int weight, bool isLeaf = false);

public:
  TreeNode *left;
  TreeNode *right;
  char symbol;
  int weight;
  bool isLeaf;
};

} // namespace huffman