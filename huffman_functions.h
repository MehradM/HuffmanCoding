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

class compare{
public:
    bool operator() (TreeNode* x, TreeNode* y ) {
        return x->quantity > y->quantity;
    }
};
typedef std::priority_queue<TreeNode*, std::vector<TreeNode*>, compare> tree_pqueue;
tree_pqueue createTree(tree_pqueue);
std::map<char, std::pair<std::string,int>> character_map(tree_pqueue pqueue,TreeNode * root);
void make_compressed_file(const std::string& fileName);
std::string file_to_string(const std::string& fileName);
#endif //HUFFMANPROJECT_HUFFMAN_FUNCTIONS_H
