#include "huffman.h"

#include <fmt/core.h>
#include <string>

int main() {

    std::map<std::string, int> logMessageFrequencies = {
            {"INFO", 20},
            {"DEBUG", 10},
            {"WARNING", 5},
            {"ERROR", 2},
            {"CRITICAL", 1},
            {"FATAL", 1},
            {"SUCCESS", 15},
            {"VALIDATION", 7},
            {"AUTHENTICATION", 8},
            {"USER_ACTION", 10},
            {"DATABASE", 5}
    };

    huffman huffman;
    huffman.buildHuffmanTable(logMessageFrequencies);
    huffman.printHuffmanTable();
    std::string testString = "SUCCESS";
    std::string encoded_str = huffman.compress(testString);
    fmt::print("Encoded string: {}, encoding ratio {}\n", encoded_str, (float) encoded_str.size() / testString.size());
    std::string decoded_str = huffman.decompress(encoded_str);
    fmt::print("Decoded string: {}\n", decoded_str);

    return 0;
}
