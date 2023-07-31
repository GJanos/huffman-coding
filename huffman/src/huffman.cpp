#include <functional>
#include "huffman.h"
#include "fmt/core.h"

/// PUBLIC

std::string huffman::compress(const std::string &text) {
    std::string result;
    // +1 is for the carry over char that stores in which bit the encoded string ended inside the last byte
    // 111 was useful info, but had to reserve a full 8 bits for it (char) 11100000
    //last char would be 3 in this case
    result.reserve(table.predicDecodedLength(text));

    char carry_over_char = 0;
    int carry_over_bits = 0;

    for (auto text_it = text.begin(); text_it != text.end(); ++text_it) {
        const boost::dynamic_bitset<> &bits = table.getTable().at(*text_it);
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

std::string huffman::decompress(const std::string &text) {
    std::string result;
    result.reserve(table.predictEncodedLength(text));

    int last_char_used_bits = text[text.size() - 1];

    boost::dynamic_bitset<> bit_buffer;
    int buffer_bits_cnt = 0;
    std::bitset<8> bits;

    for (size_t idx = 0; idx < text.size() - 1; ++idx) {
        bits = text[idx];

        for (int i = 0; i < 8; ++i) {

            bit_buffer.push_back(bits[i] << buffer_bits_cnt++);
            auto table_it = table.getReversedTable().find(bit_buffer);
            if (table_it != table.getReversedTable().end()) {
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

bool huffman::buildHuffmanTable(const std::string &text_to_compress) {
    if (text_to_compress.empty()) {
        std::cout << "Empty string" << std::endl;
        return true;
    }

    std::map<char, int> freq_map = buildFrequencyMap(text_to_compress);
    huffman_tree::Node *root = tree.build(freq_map);
    table.build(root);
    return true;
}

bool huffman::buildHuffmanTable(const std::map<std::string, int> &freq_of_logs) {
    if (freq_of_logs.empty()) {
        std::cout << "Empty string" << std::endl;
        return true;
    }

    std::string text_to_compress = buildLogString(freq_of_logs);
    std::map<char, int> freq_map = buildFrequencyMap(text_to_compress);
    huffman_tree::Node *root = tree.build(freq_map);
    table.build(root);
    return true;
}

void huffman::setTable(std::unordered_map<char, boost::dynamic_bitset<>> huffmanTable) {
    table.setTable(huffmanTable);
    table.setReversedTable();
}

std::string huffman::huffmanTableToString() {
    return table.toString();
}

void huffman::huffmanTableFromString(const std::string &table_str) {
    table.fromString(table_str);
}

void huffman::printHuffmanTable() {
    table.print();
}


/// PRIVATE

std::map<char, int> huffman::buildFrequencyMap(const std::string &text) {
    std::map<char, int> freq_map;
    for (char ch: text) {
        freq_map[ch]++;
    }
    return freq_map;
}

std::string huffman::buildLogString(const std::map<std::string, int> &log_message_frequencies) {
    std::string result;
    for (const auto &[char_key, freq]: log_message_frequencies) {
        for (int i = 0; i < freq; ++i) {
            result += char_key;
        }
    }
    return result;
}



