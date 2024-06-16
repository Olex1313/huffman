#include <cstdint>
#include <queue>
#include <symbol_tree.hpp>

namespace huffman {

TreeNode::TreeNode(byte symbol, int64_t weight)
    : symbol(symbol), weight(weight) {}

TreeNode::TreeNode(const std::shared_ptr<TreeNode> &leftAncestor,
                   const std::shared_ptr<TreeNode> &rightAncestor)
    : weight(leftAncestor->weight + rightAncestor->weight), symbol(-1),
      right(rightAncestor), left(leftAncestor) {}

bool TreeNode::operator<(const TreeNode &other) const {
  return weight < other.weight;
}

bool TreeNode::operator>(const TreeNode &other) const {
  return weight > other.weight;
}

bool TreeNode::IsLeaf() const { return right == nullptr && left == nullptr; }

SymbolTree::SymbolTree(const std::map<byte, uint64_t> &symbolFreqMap)
    : alphabet(symbolFreqMap), prefixCodes() {

  BuildHuffmanTree();
  std::string cleanCodeStr;
  cleanCodeStr.clear();
  BuildCodeTable(root.get(), cleanCodeStr);
}

SymbolTree::SymbolTree(std::ifstream &ifs) { Load(ifs); }

void SymbolTree::BuildHuffmanTree() {
  auto cmp = [](const std::shared_ptr<TreeNode> &r,
                const std::shared_ptr<TreeNode> &l) -> bool {
    return r->weight > l->weight;
  };

  std::priority_queue<std::shared_ptr<TreeNode>,
                      std::vector<std::shared_ptr<TreeNode>>, decltype(cmp)>
      pq(cmp);

  for (const auto &[symbol, frequency] : alphabet) {
    pq.push(std::make_shared<TreeNode>(symbol, frequency));
  }

  while (pq.size() > 1) {
    auto left = pq.top();
    pq.pop();
    auto right = pq.top();
    pq.pop();

    auto newEl = std::make_shared<TreeNode>(left, right);
    pq.push(newEl);
  }

  root = pq.top();
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

TreeNode *SymbolTree::GetRoot() const { return root.get(); }

bool SymbolTree::HasCode(byte symbol) const {
  return prefixCodes.count(symbol);
}

void SymbolTree::Dump(std::ofstream &ofs) const {
  if (!ofs.is_open()) {
    throw std::runtime_error("unable to open file");
  }

  uintmax_t totalChars = 0;
  uint8_t alphabetSize = alphabet.size();
  for (const auto &entry : alphabet) {
    totalChars += entry.second;
  }

  ofs.write((const char *)&FILETAG, sizeof(FILETAG));
  ofs.write((const char *)&alphabetSize, sizeof(alphabetSize));
  ofs.write((const char *)&totalChars, sizeof(totalChars));

  for (const auto &[symbol, weight] : alphabet) {
    char s = symbol;
    ofs.write((const char *)&s, sizeof(s));
    int w = weight;
    ofs.write((const char *)&w, sizeof(w));
  }
}

void SymbolTree::Load(std::ifstream &ifs) {
  if (!ifs.is_open()) {
    throw std::runtime_error("unable to open file");
  }

  int cnt = 0;
  uintmax_t totalChars = 0;
  uint8_t alphabetSize = 0;
  char temptag[sizeof(FILETAG)];

  ifs.read(const_cast<char *>(temptag), sizeof(temptag));
  if (std::string(temptag) != std::string(FILETAG)) {
    throw std::runtime_error("invalid file tag");
  }
  ifs.read(reinterpret_cast<char *>(&alphabetSize), sizeof(alphabetSize));
  ifs.read(reinterpret_cast<char *>(&totalChars), sizeof(totalChars));

  root = nullptr;
  alphabet.clear();
  prefixCodes.clear();
  char tempSymbol;
  int tempCnt;

  for (unsigned int i = 0; i < alphabetSize; i++) {
    ifs.read(reinterpret_cast<char *>(&tempSymbol), sizeof(tempSymbol));
    ifs.read(reinterpret_cast<char *>(&tempCnt), sizeof(tempCnt));
    alphabet[tempSymbol] = tempCnt;
  }

  BuildHuffmanTree();
  std::string cleanCodeStr;
  cleanCodeStr.clear();
  BuildCodeTable(root.get(), cleanCodeStr);
}
} // namespace huffman