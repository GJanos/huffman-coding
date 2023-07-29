#include <functional>
#include "Huffman.h"
#include <fmt/core.h>

/// PUBLIC
std::string Huffman::compress(const std::string &text) {
    std::string result;
    // +1 is for the carry over char that stores in which bit the encoded string ended inside the last byte
    // 111 was useful info, but had to reserve a full 8 bits for it (char) 11100000
    //last char would be 3 in this case
    int predicted_encoded_length = (((int) text.size() * getLongestHuffmanCode()) / 8) + 1;
    result.reserve(predicted_encoded_length);

    char carry_over_char = 0;
    int carry_over_bits = 0;

    for (auto text_it = text.begin(); text_it != text.end(); ++text_it) {
        boost::dynamic_bitset<> &bits = huffmanTable[*text_it];
        for (int i = 0; i < bits.size(); ++i) {
            carry_over_char |= (bits[i] << carry_over_bits++);
            if (carry_over_bits == 8) {
                result.push_back(carry_over_char);
                carry_over_char = 0;
                // if it's the last char and the last bit of the last char, then we need to store the number of bits
                // otherwise we just reset
                carry_over_bits = (std::next(text_it) == text.end()) && i == bits.size() - 1 ? 8 : 0;
            }
            if (std::next(text_it) == text.end() && i == bits.size() - 1) {
                carry_over_char << (8 - carry_over_bits);
                result.push_back(carry_over_char);
            }
        }
    }
    result.push_back(carry_over_bits);
    result.shrink_to_fit();
    return result;
}

std::string Huffman::decompress(const std::string &text) {
    std::string result;
    int predicted_encoded_length = (((int) text.size() - 1) * 8) / getAverageHuffmanCode(); ///kerekites
    result.reserve(predicted_encoded_length);

    int last_char_used_bits = text[text.size() - 1];

    boost::dynamic_bitset<> bit_buffer;
    int buffer_bits_cnt = 0;
    std::bitset<8> bits;
//    printHuffmanTable();
//    printBitsOfString(text);

    for (size_t idx = 0; idx < text.size() - 1; ++idx) {
        bits = text[idx];

        for (int i = 0; i < 8; ++i) {

            bit_buffer.push_back(bits[i] << buffer_bits_cnt++);
            auto table_it = reversedHuffmanTable.find(bit_buffer);
            if (table_it != reversedHuffmanTable.end()) {
                result.push_back(table_it->second);
                bit_buffer.clear();
                buffer_bits_cnt = 0;
            }
            if (idx == text.size() - 2 && i == last_char_used_bits - 1) {
                break;
            }
        }
    }
    return result;
}

bool Huffman::buildHuffmanTree(const std::string &text_to_compress) {
    if (text_to_compress.empty()) {
        std::cout << "Empty string" << std::endl;
        return true;
    }

    std::map<char, int> freq_map = buildFrequencyMap(text_to_compress);

    Node *root = buildPriorityQueue(freq_map);
    buildHuffmanTable(root);
    return true;
}

bool Huffman::buildHuffmanTree(const std::map<std::string, int> &freq_of_logs) {
    if (freq_of_logs.empty()) {
        std::cout << "Empty string" << std::endl;
        return true;
    }

    std::string text_to_compress = buildLogString(freq_of_logs);
    std::map<char, int> freq_map = buildFrequencyMap(text_to_compress);
    Node *root = buildPriorityQueue(freq_map);
    buildHuffmanTable(root);
    return true;
}

void Huffman::setTable(std::unordered_map<char, boost::dynamic_bitset<>> huffmanTable) {
    this->huffmanTable = huffmanTable;
    setReversedHuffmanTable();
}

std::string Huffman::huffmanTableToString() {
    std::string result;
    int predicted_huff_table_length = ceil((huffmanTable.size() * (getAverageHuffmanCode() + 1)) / 8.0);
    result.reserve(predicted_huff_table_length);

    for (const auto &[char_key, bitset]: huffmanTable) {
        std::string bitset_string;
        to_string(bitset, bitset_string);
        result.append(fmt::format("{}{}{}", char_key, bitset_string, huffmanTableDelimiter));
    }

    result.shrink_to_fit();
    return result;
}

void Huffman::huffmanTableFromString(const std::string &table) {
    std::string::size_type pos = 0;
    std::string::size_type prev = 0;

    huffmanTable.clear();

    while ((pos = table.find(huffmanTableDelimiter, prev)) != std::string::npos) {
        std::string token = table.substr(prev, pos - prev);

        if (!token.empty()) {
            char ch = token[0];
            boost::dynamic_bitset<> bits(token.substr(1));
            huffmanTable[ch] = bits;
        }
        prev = pos + 1;
    }
    setReversedHuffmanTable();
}

void Huffman::printHuffmanTable() {
    for (const auto &pair: huffmanTable) {
        std::cout << pair.first << ": ";
        for (int i = 0; i < pair.second.size(); i++) {
            std::cout << pair.second[i];
        }
        std::cout << std::endl;
    }
}


/// PRIVATE
void Huffman::buildHuffmanTable(Node *root) {
    boost::dynamic_bitset<> currentCode;

    std::function<void(Node *, boost::dynamic_bitset<> &)> traverse = [&](Node *node,
                                                                          boost::dynamic_bitset<> &currentCode) {
        if (node == nullptr) {
            return;
        }
        if (!node->left && !node->right) {
            huffmanTable[node->ch] = currentCode;
        }
        if (node->left) {
            currentCode.push_back(0);
            traverse(node->left, currentCode);
            currentCode.pop_back();
        }
        if (node->right) {
            currentCode.push_back(1);
            traverse(node->right, currentCode);
            currentCode.pop_back();
        }
    };
    traverse(root, currentCode);
    setReversedHuffmanTable();
}

std::map<char, int> Huffman::buildFrequencyMap(const std::string &text) {
    std::map<char, int> freq_map;
    for (char ch: text) {
        freq_map[ch]++;
    }
    return freq_map;
}

Huffman::Node *Huffman::buildPriorityQueue(std::map<char, int> freq_map) {
    std::priority_queue<Node *, std::vector<Node *>, Comparision> pq;
    for (auto pair: freq_map) {
        pq.push(new Node(pair.first, pair.second, nullptr, nullptr));
    }

    while (pq.size() != 1) {
        Node *left = pq.top();
        pq.pop();
        Node *right = pq.top();
        pq.pop();

        int sum = left->freq + right->freq;
        pq.push(new Node('\0', sum, left, right));
    }

    return pq.top();
}

void Huffman::setReversedHuffmanTable() {
    for (const auto &[huffman_key, huffman_value]: huffmanTable) {
        reversedHuffmanTable[huffman_value] = huffman_key;
    }
}

int Huffman::getLongestHuffmanCode() {
    int longest = 0;
    for (const auto &pair: huffmanTable) {
        if (pair.second.size() > longest) {
            longest = pair.second.size();
        }
    }
    return longest;
}

int Huffman::getAverageHuffmanCode() {
    int sum = 0;
    for (const auto &pair: huffmanTable) {
        sum += pair.second.size();
    }
    return sum / huffmanTable.size();
}

std::string Huffman::buildLogString(const std::map<std::string, int> &log_message_frequencies) {
    std::string result;
    for (const auto &[char_key, freq]: log_message_frequencies) {
        for (int i = 0; i < freq; ++i) {
            result += char_key;
        }
    }
    return result;
}



