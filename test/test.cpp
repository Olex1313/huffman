#include <cstdint>
#include <gtest/gtest.h>
#include <huffman.hpp>
#include <symbol_tree.hpp>

std::string getTmpFile(std::string suffix = "") {
  return testing::TempDir() + std::to_string(std::rand()) + "." + suffix;
}

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

  std::string filename = getTmpFile();

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

const std::string loremIpsum =
    "Lorem ipsum dolor sit amet, "
    "consectetur adipiscing elit, sed do eiusmod tempor incididunt ut "
    "labore et"
    " dolore magna aliqua. Ut enim ad minim veniam, quis nostrud "
    "exercitation ullamco"
    " laboris nisi ut aliquip ex ea commodo consequat. Duis aute "
    "irure dolor in reprehenderit in voluptate velit esse cillum "
    "dolore eu fugiat nulla pariatur. Excepteur sint occaecat "
    "cupidatat non proident, sunt in culpa qui officia deserunt "
    "mollit anim id est laborum.";

const std::string simpleAbba = "abbapeppa";
const std::string nonAscii = "хаффман";

class HuffmanEncodingTest : public testing::TestWithParam<std::string> {};

TEST_P(HuffmanEncodingTest, EncodeAndDecodeTextFile) {
  const std::string testText = GetParam();
  int testId = std::rand();
  std::string inFileName = getTmpFile("in." + std::to_string(testId));
  std::string outFileName = getTmpFile("out." + std::to_string(testId));
  std::string decodedFileName = getTmpFile("decoded." + std::to_string(testId));

  std::ofstream inFile(inFileName);
  ASSERT_TRUE(inFile.good());
  inFile << testText;
  inFile.close();

  huffman::Huffman huffman;
  huffman.Encode(inFileName, outFileName);

  std::ifstream decoded(outFileName);
  ASSERT_TRUE(decoded.good());
  decoded.close();

  huffman.Decode(outFileName, decodedFileName);
  std::stringstream sstr;
  std::ifstream input(decodedFileName);
  while (input >> sstr.rdbuf())
    ;

  std::string decodedTxt = sstr.str();

  ASSERT_EQ(testText, decodedTxt);
}

INSTANTIATE_TEST_SUITE_P(Huffman, HuffmanEncodingTest,
                         testing::Values(loremIpsum, simpleAbba, nonAscii));
