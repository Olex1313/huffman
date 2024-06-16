
#include <huffman.hpp>
#include <stdexcept>

int main(int argc, char const *argv[]) {
  if (argc != 6) {
    throw std::invalid_argument("invalid number of arguments provided");
  }
  bool encodingFlag = false;
  bool decodingFlag = false;
  std::string inFile("");
  std::string outFile("");

  for (int i = 1; i < 6; i++) {
    auto arg = std::string(argv[i]);
    if (arg == "-c") {
      encodingFlag = true;
    } else if (arg == "-u") {
      decodingFlag = true;
    } else if (arg == "-f") {
      i++;
      inFile = std::string(argv[i]);
    } else if (arg == "-o") {
      i++;
      outFile = std::string(argv[i]);
    }
  }

  if (encodingFlag && decodingFlag) {
    throw std::invalid_argument("invalid arguments provided");
  }

  huffman::Huffman huff;
  if (encodingFlag) {
    huff.Encode(inFile, outFile);
  } else {
    huff.Encode(inFile, outFile);
  }
}
