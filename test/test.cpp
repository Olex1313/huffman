#include <cstdint>
#include <gtest/gtest.h>
#include <huffman.hpp>
#include <symbol_tree.hpp>

// Demonstrate some basic assertions.
TEST(SymbolTreeSuite, Construct) {
  std::map<huffman::byte, uint64_t> frequency = {
      {'l', 4}, {'w', 1}, {'a', 2}, {'s', 2}, {'e', 2}};

  std::vector<std::pair<huffman::byte, std::string>> expectedCodes = {
      std::make_pair('l', "11"),  std::make_pair('w', "100"),
      std::make_pair('a', "101"), std::make_pair('s', "00"),
      std::make_pair('e', "01"),
  };

  huffman::SymbolTree symbolTree(frequency);

  for (const auto &[symbol, expectedCode] : expectedCodes) {
    auto actualCode = symbolTree.GetCode(symbol);
    EXPECT_EQ(actualCode, expectedCode);
  }
}