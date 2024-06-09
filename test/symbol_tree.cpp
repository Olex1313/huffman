#include "huffman.hpp"
#include <cstdint>
#include <gtest/gtest.h>
#include <symbol_tree.hpp>
#include <utility>

// Demonstrate some basic assertions.
TEST(SymbolTreeSuite, Construct) {
  std::map<huffman::byte, uint64_t> frequency = {
      {'a', 5}, {'b', 9}, {'c', 12}, {'d', 13}, {'e', 16}, {'f', 45}};

  std::vector<std::pair<huffman::byte, std::string>> expectedCodes = {
      std::make_pair('a', "1100"), std::make_pair('b', "1101"),
      std::make_pair('c', "100"),  std::make_pair('d', "101"),
      std::make_pair('e', "111"),  std::make_pair('f', "0"),
  };

  huffman::SymbolTree symbolTree(frequency);

  for (const auto &[symbol, expectedCode] : expectedCodes) {
    auto actualCode = symbolTree.GetCode(symbol);
    EXPECT_EQ(actualCode, expectedCode);
  }
}