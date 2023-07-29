#include "HuffmanTree.h"

HuffmanTree::Node *HuffmanTree::build(const std::map<char, int> &freq_map) {
    for (auto pair: freq_map) {
        prioQueue.push(new Node(pair.first, pair.second, nullptr, nullptr));
    }
    while (prioQueue.size() != 1) {
        Node *left = prioQueue.top();
        prioQueue.pop();
        Node *right = prioQueue.top();
        prioQueue.pop();

        int sum = left->freq + right->freq;
        prioQueue.push(new Node('\0', sum, left, right));
    }
    return prioQueue.top();
}
