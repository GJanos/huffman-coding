#ifndef GLZTEST_HUFFMAN_TABLE_H
#define GLZTEST_HUFFMAN_TABLE_H

#include "huffman_tree.h"

#include <unordered_map>
#include <boost/dynamic_bitset.hpp>

class huffman_table {
public:
    void build(huffman_tree::Node *root);

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


#endif //GLZTEST_HUFFMAN_TABLE_H
