#include <cstdint>
#include <gtest/gtest.h>
#include <huffman.hpp>
#include <symbol_tree.hpp>

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

TEST(SymbolTreeSuite, DumpShouldRestoreToSameTableOnLoad) {
  std::map<huffman::byte, uint64_t> frequency = {
      {'l', 4}, {'w', 1}, {'a', 2}, {'s', 2}, {'e', 2}};
  std::vector<std::pair<huffman::byte, std::string>> expectedCodes = {
      std::make_pair('l', "11"),  std::make_pair('w', "100"),
      std::make_pair('a', "101"), std::make_pair('s', "00"),
      std::make_pair('e', "01"),
  };
  huffman::SymbolTree symbolTree(frequency);

  const testing::TestInfo *const testInfo =
      testing::UnitTest::GetInstance()->current_test_info();

  std::string filename = testing::TempDir() +
                         std::string(testInfo->test_suite_name()) + "." +
                         std::string(testInfo->test_case_name());
  std::ofstream outStream(filename, std::ios::binary | std::ios::out);
  symbolTree.Dump(outStream);
  outStream.close();

  std::ifstream inStream(filename, std::ios::binary | std::ios::in);
  huffman::SymbolTree second(inStream);

  for (const auto &[symbol, expectedCode] : expectedCodes) {
    auto actualCode = second.GetCode(symbol);
    EXPECT_EQ(actualCode, expectedCode);
  }
}
