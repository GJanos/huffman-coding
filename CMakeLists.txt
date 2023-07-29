cmake_minimum_required(VERSION 3.24)
project(glzTest)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
        glaze
        GIT_REPOSITORY https://github.com/stephenberry/glaze.git
        GIT_TAG main
        GIT_SHALLOW TRUE
)

FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 8.0.1
)
FetchContent_MakeAvailable(glaze fmt)

find_path(LZ4_INCLUDE_DIR NAMES lz4.h)
find_library(LZ4_LIBRARY NAMES lz4)

if (NOT LZ4_INCLUDE_DIR OR NOT LZ4_LIBRARY)
    message(FATAL_ERROR "Could not find LZ4")
endif()

find_path(SNAPPY_INCLUDE_DIR NAMES snappy.h)
find_library(SNAPPY_LIBRARY NAMES snappy)

if(NOT SNAPPY_INCLUDE_DIR OR NOT SNAPPY_LIBRARY)
    message(FATAL_ERROR "Could not find Snappy")
endif()

find_package(Boost REQUIRED)

add_executable(glzTest main.cpp Huffman.cpp HuffmanTable.cpp HuffmanTable.h HuffmanTree.cpp HuffmanTree.h)

target_include_directories(glzTest
        PRIVATE ${LZ4_INCLUDE_DIR}
        PRIVATE ${SNAPPY_INCLUDE_DIR}
        ${Boost_INCLUDE_DIRS})
target_link_libraries(glzTest
        glaze::glaze
        fmt::fmt
        ${LZ4_LIBRARY}
        ${SNAPPY_LIBRARY}
        ${Boost_LIBRARIES})
