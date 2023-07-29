#include "HuffmanTable.h"
#include "fmt/format.h"

///PUBLIC
void HuffmanTable::build(HuffmanTree::Node *root) {
    boost::dynamic_bitset<> currentCode;

    std::function<void(HuffmanTree::Node *, boost::dynamic_bitset<> &)> traverse = [&](HuffmanTree::Node *node,
                                                                                       boost::dynamic_bitset<> &currentCode) {
        if (node == nullptr) {
            return;
        }
        if (!node->left && !node->right) {
            table[node->ch] = currentCode;
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
    setReversedTable();
}

std::string HuffmanTable::toString() {
    std::string result;
    int predicted_huff_table_length = ceil((table.size() * (getAvgCodeLenght() + 1)) / 8.0);
    result.reserve(predicted_huff_table_length);

    for (const auto &[char_key, bitset]: table) {
        std::string bitset_string;
        to_string(bitset, bitset_string);
        result.append(fmt::format("{}{}{}", char_key, bitset_string, delimiter));
    }

    result.shrink_to_fit();
    return result;
}

void HuffmanTable::fromString(const std::string &table_str) {
    std::string::size_type pos = 0;
    std::string::size_type prev = 0;

    table.clear();

    while ((pos = table_str.find(delimiter, prev)) != std::string::npos) {
        std::string token = table_str.substr(prev, pos - prev);

        if (!token.empty()) {
            char ch = token[0];
            boost::dynamic_bitset<> bits(token.substr(1));
            table[ch] = bits;
        }
        prev = pos + 1;
    }
    setReversedTable();
}

void HuffmanTable::print() {
    for (const auto &pair: table) {
        fmt::print("{}: ", pair.first);
        for (int i = 0; i < pair.second.size(); i++) {
            fmt::print("{}", pair.second[i] ? 1 : 0);
        }
        fmt::print("\n");
    }
}

int HuffmanTable::getAvgCodeLenght() {
    int sum = 0;
    for (const auto &pair: table) {
        sum += pair.second.size();
    }
    return sum / table.size();
}

int HuffmanTable::getLongestCodeLength() {
    int longest = 0;
    for (const auto &pair: table) {
        if (pair.second.size() > longest) {
            longest = pair.second.size();
        }
    }
    return longest;
}

int HuffmanTable::predictEncodedLength(const std::string& text){
    int total_bits = (text.size() - 1) * 8;
    double avg_code_length = static_cast<double>(getAvgCodeLenght());
    return std::ceil(total_bits / avg_code_length);
}

int HuffmanTable::predicDecodedLength(const std::string &text) {
    double longest_code_length = static_cast<double>(getLongestCodeLength());
    double total_encoded_length = text.size() * longest_code_length;
    int total_encoded_bytes = std::ceil(total_encoded_length / 8.0);
    return total_encoded_bytes + 1;
}

const std::unordered_map<char, boost::dynamic_bitset<>> &HuffmanTable::getTable() {
    return table;
}

const std::unordered_map<boost::dynamic_bitset<>, char> &HuffmanTable::getReversedTable() {
    return reversedTable;
}

void HuffmanTable::setTable(const std::unordered_map<char, boost::dynamic_bitset<>> &table) {
    this->table = table;
    setReversedTable();
}

void HuffmanTable::setReversedTable() {
    for (auto &[huffman_key, huffman_value]: table) {
        reversedTable[huffman_value] = huffman_key;
    }
}

