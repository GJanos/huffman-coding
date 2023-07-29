#ifndef GLZTEST_HUFFMAN_H
#define GLZTEST_HUFFMAN_H

#include <iostream>
#include <queue>
#include <map>
#include <unordered_map>
#include <boost/dynamic_bitset.hpp>

class Huffman {
public:
    std::string compress(const std::string& text);
    std::string decompress(const std::string& text);

    bool buildHuffmanTree(const std::string& text);
    bool buildHuffmanTree(const std::map<std::string, int>& freq_of_logs);

    void setTable(std::unordered_map<char, boost::dynamic_bitset<>> huffmanTable);
    std::string huffmanTableToString();
    void huffmanTableFromString(const std::string& table);
    void printHuffmanTable();

private:
    struct Node {
        char ch;
        int freq;
        Node* left;
        Node* right;

        Node(char ch, int freq, Node* left, Node* right) {
            this->ch = ch;
            this->freq = freq;
            this->left = left;
            this->right = right;
        }
    };

    class Comparision
    {
    public:
        bool operator () (const Node* n1, const Node* n2) {
            return n1->freq > n2->freq;
        }
    };

    std::unordered_map<char, boost::dynamic_bitset<>> huffmanTable;
    std::unordered_map<boost::dynamic_bitset<>, char> reversedHuffmanTable;
    char huffmanTableDelimiter = '|';


    void buildHuffmanTable(Huffman::Node* root);
    std::map<char, int> buildFrequencyMap(const std::string& text);
    Node * buildPriorityQueue(std::map<char, int> freq_map);
    void setReversedHuffmanTable();
    int getLongestHuffmanCode();
    int getAverageHuffmanCode();
    std::string buildLogString(const std::map<std::string, int> &map);
};

#endif //GLZTEST_HUFFMAN_H
