#ifndef GLZTEST_HUFFMAN_H
#define GLZTEST_HUFFMAN_H

#include "HuffmanTree.h"
#include "HuffmanTable.h"

#include <iostream>
#include <queue>
#include <map>
#include <unordered_map>
#include <boost/dynamic_bitset.hpp>

class Huffman {
public:
    Huffman() = default;
    std::string compress(const std::string& text);
    std::string decompress(const std::string& text);

    bool buildHuffmanTable(const std::string& text_to_compress);
    bool buildHuffmanTable(const std::map<std::string, int>& freq_of_logs);

    void setTable(std::unordered_map<char, boost::dynamic_bitset<>> huffmanTable);
    std::string huffmanTableToString();
    void huffmanTableFromString(const std::string& table);
    void printHuffmanTable();

private:

    HuffmanTree tree;
    HuffmanTable table;

    std::map<char, int> buildFrequencyMap(const std::string& text);
    std::string buildLogString(const std::map<std::string, int> &map);
};

#endif //GLZTEST_HUFFMAN_H
