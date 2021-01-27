//
// Created by Mehrad on 1/27/2021.
//

#ifndef HUFFMANPROJECT_HUFFMAN_FUNCTIONS_H
#define HUFFMANPROJECT_HUFFMAN_FUNCTIONS_H
#include <string>
#include <vector>
#include <map>
#include <queue>
struct TreeNode {
    std::string data;
    int quantity;
    TreeNode* left = nullptr;
    TreeNode* right = nullptr;
    TreeNode* parent = nullptr;
};

#endif //HUFFMANPROJECT_HUFFMAN_FUNCTIONS_H
