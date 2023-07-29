//#include <vector>
//#include <glaze/glaze.hpp>
//#include <fmt/core.h>
//
//int main() {
////    size_t cnt = 0;
////    glz::obj o5{"count", size_t{cnt}};
////    std::vector<std::decay_t<decltype(o5)>> vec;
////    for (; cnt < 10; ++cnt) {
////        vec.emplace_back(glz::obj{"count", size_t{cnt}});
////    }
////    for( const auto& i: vec ) {
////        fmt::print("count value:{}\n", glz::tuplet::get<1>(i.value));
////    }
//
//    glz::obj o {"not", "important"};
//    glz::obj o2 {"map", glz::obj{"a", 1, "b", 2, "c", 3, "d", glz::obj{"e", 4, "f", 5, "g", 6}}, "baby", "shark"};
//    std::string s = "{\"m\":{\"a\":1,\"b\":2,\"c\":3}}";
//    std::string s2 = "{\"key1\":42,\"key2\":\"hello world\",\"v\":[1,2,3],\"m\":{\"a\":1,\"b\":2,\"c\":3}}";
//    glz::json_t j;
//    glz::read_json(j,s);
//    auto test1 = glz::write_json(j);
//    auto merged = glz::merge{o, j};
//    auto test2 = glz::write_json(merged);
//    glz::read_json(j,s2);
//    auto test4 = glz::write_json(glz::merge{o, j});
//    glz::obj o3 {100, "eee", -2, "eeee", 3.3, "eeee"};
//    fmt::print("{}\n", test1);
//    fmt::print("{}\n", test2);
//    fmt::print("{}\n", test4);
//    fmt::print("{}\n", glz::write_json(o3));
//    fmt::print("{}\n", glz::write_json(glz::merge{o, o3}));
//
//
//    glz::json_t json;
//    glz::read_json(json,"{\"key1\":42,\"key2\":\"hello world\",\"v\":[1,2,3],\"m\":{\"a\":1,\"b\":2,\"c\":3}}");
//    glz::obj obj {"not", "important"};
//    fmt::print("{}\n", glz::write_json(glz::merge{obj, json}));
//    //{"not":"important","key1":42,"key2":"hello world","m":"a":1,"b":2,"c":3,"v":[1,2,3]}
//    // -- map did not get properly serialized
//
//    glz::json_t json2 {{"object", {
//            {"currency", "USD"},
//            {"value", 42.99}
//    }}};
//    fmt::print("{}\n", glz::write_json(json2));
//    //["object",{"currency":"USD","value":42.99}]
//    // -- opening, closing brackets missing and [ ] were used
//
//    glz::json_t json3 = {
//            {"pi", 3.141},
//            {"happy", true},
//            {"name", "Stephen"},
//            {"nothing", nullptr},
//            {"answer", {{"everything", 42.0}}},
//            {"list", {1.0, 0.0, 2.0}},
//            {"object", {
//                        {"currency", "USD"},
//                           {"value", 42.99}
//                   }
//            }
//    };
//
//    fmt::print("{}\n", glz::write_json(json3));
//
////    {"m":{"a":1,"b":2,"c":3}}
////    {"not":"important","m":"a":1,"b":2,"c":3}
////    {"not":"important","key1":42,"key2":"hello world","m":"a":1,"b":2,"c":3,"v":[1,2,3]}
////    {""eee"":"eee",""eeee"":"eeee",""eeee"":"eeee"}
//    return 0;
//}

#include <iostream>
#include <vector>
#include <fmt/core.h>
#include <string>


#include "lz4.h"
#include <snappy.h>
#include "Huffman.h"


void lz4_compression_test();
void snappy_compression_test();
void huffman_compression_test();


int main() {
    //lz4_compression_test();
    //snappy_compression_test();
    huffman_compression_test();
    return 0;
}

void lz4_compression_test(){
    // Original data
    std::string src = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. "
                      "Sed non risus. Suspendisse lectus tortor, dignissim sit amet, "
                      "adipiscing nec, ultricies sed, dolor. Cras elementum ultrices diam. "
                      "Maecenas ligula massa, varius a, semper congue, euismod non, mi";
    int srcSize = src.size() + 1;  // +1 for the null-terminator

    // Prepare the compressed data buffer
    int maxDstSize = LZ4_compressBound(srcSize);
    std::vector<char> compressedData(maxDstSize);

    // Compress the data
    int compressedDataSize = LZ4_compress_default(src.c_str(), compressedData.data(), srcSize, maxDstSize);
    if (compressedDataSize <= 0) {
        std::cerr << "Failed to compress data." << std::endl;
        return;
    }
    compressedData.resize(compressedDataSize);  // Shrink to fit the actual size
    fmt::print("Compression ratio: {}\n", (float) compressedDataSize / srcSize);
    // Prepare the decompressed data buffer
    std::vector<char> decompressedData(srcSize);

    // Decompress the data
    int decompressedDataSize = LZ4_decompress_safe(compressedData.data(), decompressedData.data(), compressedDataSize, srcSize);
    if (decompressedDataSize < 0) {
        std::cerr << "Failed to decompress data." << std::endl;
        return;
    }

    // Check if the decompressed data is identical to the original data
    if (src != decompressedData.data()) {
        std::cerr << "Decompressed data is different from original." << std::endl;
        return;
    }

    std::cout << "Success! The decompressed data is:\n" << decompressedData.data() << std::endl;
}

void snappy_compression_test(){
    std::string input = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. "
                      "Sed non risus. Suspendisse lectus tortor, dignissim sit amet, "
                      "adipiscing nec, ultricies sed, dolor. Cras elementum ultrices diam. "
                      "Maecenas ligula massa, varius a, semper congue, euismod non, mi";
    std::string output;
    std::string decompressed;

    // Compress the input
    snappy::Compress(input.data(), input.size(), &output);

    // Decompress the output
    if (snappy::Uncompress(output.data(), output.size(), &decompressed)) {
        if (input == decompressed) {
            std::cout << "Decompressed string matches the original string!" << std::endl;
            fmt::print("Compression ratio: {}\n", (float)output.size() / input.size());
            fmt::print("decompressed {}\n", output);
        } else {
            std::cout << "Decompressed string does not match the original string!" << std::endl;
        }
    } else {
        std::cout << "Failed to decompress string!" << std::endl;
    }
}

void huffman_compression_test(){

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

    Huffman huffman;
    huffman.buildHuffmanTable(logMessageFrequencies);
    huffman.printHuffmanTable();
    std::string testString = "SUCCESS";
    std::string encoded_str = huffman.compress(testString);
    fmt::print("Encoded string: {}, encoding ratio {}\n", encoded_str, (float) encoded_str.size() / testString.size());
    std::string decoded_str = huffman.decompress(encoded_str);
    fmt::print("Decoded string: {}\n", decoded_str);

}