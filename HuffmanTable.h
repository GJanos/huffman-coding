#ifndef GLZTEST_HUFFMANTABLE_H
#define GLZTEST_HUFFMANTABLE_H

#include "HuffmanTree.h"

#include <unordered_map>
#include <boost/dynamic_bitset.hpp>

class HuffmanTable {
public:
    void build(HuffmanTree::Node *root);

    std::string toString();
    void fromString(const std::string &table);
    void print();

    int getAvgCodeLenght();
    int getLongestCodeLength();

    int predictEncodedLength(const std::string &text);
    int predicDecodedLength(const std::string &text);

    const std::unordered_map<char, boost::dynamic_bitset<>> &getTable();
    const std::unordered_map<boost::dynamic_bitset<>, char> &getReversedTable();
    void setTable(const std::unordered_map<char, boost::dynamic_bitset<>> &table);
    void setReversedTable();

private:
    std::unordered_map<char, boost::dynamic_bitset<>> table;
    std::unordered_map<boost::dynamic_bitset<>, char> reversedTable;
    char delimiter = '|';
};


#endif //GLZTEST_HUFFMANTABLE_H
