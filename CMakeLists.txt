cmake_minimum_required(VERSION 3.24)
project(huffman)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 8.0.1
)
FetchContent_MakeAvailable(fmt)

find_package(Boost REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/huffman/header)

add_executable(huffman
        main.cpp
        huffman/src/huffman.cpp
        huffman/src/huffman_table.cpp
        huffman/src/huffman_tree.cpp)

target_include_directories(huffman
        PRIVATE ${Boost_INCLUDE_DIRS})

target_link_libraries(huffman
        fmt::fmt
        ${Boost_LIBRARIES})
