#ifndef GLZTEST_HUFFMAN_TREE_H
#define GLZTEST_HUFFMAN_TREE_H


#include <map>
#include <vector>
#include <queue>

class huffman_tree {
public:
    struct Node {
        char ch;
        int freq;
        Node *left;
        Node *right;

        Node(char ch, int freq, Node *left, Node *right) {
            this->ch = ch;
            this->freq = freq;
            this->left = left;
            this->right = right;
        }
    };

    class Comparator {
    public:
        bool operator()(const Node *n1, const Node *n2) {
            return n1->freq > n2->freq;
        }
    };

    Node *build(const std::map<char, int> &freq_map);

private:
    std::priority_queue<Node *, std::vector<Node *>, Comparator> prioQueue;
};


#endif //GLZTEST_HUFFMAN_TREE_H
