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