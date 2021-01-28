//
// Created by Mehrad on 1/27/2021.
//
#include "huffman_functions.h"
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

tree_pqueue createTree(tree_pqueue queue) {
    while (queue.size() != 1) {
        TreeNode* node1 = queue.top();
        queue.pop();
        TreeNode* node2 = queue.top();
        queue.pop();
        auto newNode =
        new TreeNode{node1->data + node2->data,node1->quantity + node2->quantity,node1,node2};
        queue.push(newNode);
        node1->parent = newNode;
        node2->parent = newNode;
    }
    return queue;
}

map<char, pair<string,int>> character_map(tree_pqueue pqueue, TreeNode *root) {
    map<char, pair<string,int>> charmap;
    while (!pqueue.empty()) {
        auto node = pqueue.top();
        string code;
        int bit = 0;
        auto temp = node;
        while (temp != root) {
            ++bit;
            if(temp->parent->right == temp)
                code.push_back('1');
            else if(temp->parent->left == temp)
                code.push_back('0');
            temp = temp->parent;
        }
        pair<string,int> pair{code,bit};
        charmap.insert({node->data[0],pair});
        pqueue.pop();
    }
    return charmap;
}

void make_compressed_file(const string& fileName) {
    auto str = file_to_string(fileName);
    auto huffmanString = huffman_code_string(str);

}

string file_to_string(const string& fileName) {
    ifstream file(fileName);
    string lines;
    string line;
    getline(file,line);
    lines.append(line);
    while (file.good()) {
        lines.push_back('\n');
        getline(file,line);
        lines.append(line);
    }
    return lines;
}

std::string huffman_code_string(const string &str) {
    auto pqueue = make_queue(str);
    auto root = createTree(pqueue).top();
    auto charMap = character_map(pqueue,root);
    auto str01 = str_to_str01(str,charMap);
    auto cmpStr = str01_to_cmpStr(str);
    string huffmanString;
    huffmanString += to_string(str.size());
    huffmanString.push_back('\n');
    huffmanString += to_string(charMap.size());
    huffmanString.push_back('\n');
    for (const auto& el : charMap) {
        huffmanString.push_back(el.first);
        huffmanString += " " + el.second.first;
        huffmanString.push_back('\n');
    }
    huffmanString += cmpStr;
    return cmpStr;
}

tree_pqueue make_queue(const string &str) {
    string temp = str;
    sort(temp.begin(),temp.end());
    auto it = unique(temp.begin(),temp.end());
    temp.erase(it,temp.end());
    tree_pqueue pqueue;
    for (int i = 0; i < temp.size(); ++i) {
        int count = std::count(str.begin(),str.end(),temp[i]);
        string str = "";
        str.push_back(temp[i]);
        pqueue.push(new TreeNode{str,count});
    }
    return pqueue;
}

std::string str_to_str01(const string &str,const char_map& map) {
    string str01;
    for (const auto& el : str) {
        str01 += map.at(el).first;
    }
    unsigned int num = str01.size() % 8;
    unsigned int need = (num == 0 ? 0 : (8 - num));
    str01.append(need,'0');
    return str01;
}

std::string str01_to_cmpStr(const string &str) {
    string cmpStr;
    for (int i = 0; i < str.length(); i+=8) {
        cmpStr.push_back(str8bit_to_char(str.substr(i,8)));
    }
    return cmpStr;
}

char str8bit_to_char(const string &str8bit) {
    char a = 0;
    for (int i = 0; i < 8; ++i) {
        a <<= 1;
        if(str8bit[i] == '1'){
            a |= 1;
        }
        else {
            a &= -2;
        }
    }
    return a;
}


