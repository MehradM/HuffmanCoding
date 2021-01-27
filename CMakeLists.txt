cmake_minimum_required(VERSION 3.17)
project(huffmanproject)

set(CMAKE_CXX_STANDARD 14)

add_executable(huffmanproject main.cpp huffman_functions.h huffman_functions.cpp)