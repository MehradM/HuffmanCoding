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
typedef std::map<char, std::pair<std::string,int>> char_map;
tree_pqueue createTree(tree_pqueue);
std::map<char, std::pair<std::string,int>> character_map(tree_pqueue pqueue,TreeNode * root);
void make_compressed_file(const std::string& fileName);
std::string file_to_string(const std::string& fileName);
std::string huffman_code_string(const std::string& str);
tree_pqueue make_queue(const std::string& str);
std::string str_to_str01(const std::string& str,const char_map& map);
std::string str01_to_cmpStr(const std::string& str);
char str8bit_to_char(const std::string& str8bit);
#endif //HUFFMANPROJECT_HUFFMAN_FUNCTIONS_H
