//
// Created by Mehrad on 1/27/2021.
//
#include "huffman_functions.h"
#include <iostream>
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
