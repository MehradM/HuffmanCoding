//
// Created by Mehrad on 1/27/2021.
//
#include "huffman_functions.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
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
                code.insert(0,1,'1');
            else if(temp->parent->left == temp)
                code.insert(0,1,'0');
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
    string compressedFileName = fileName;
    compressedFileName.erase(compressedFileName.end() - 3,compressedFileName.end());
    compressedFileName += "cmp";
    write(compressedFileName,huffmanString);
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
    printTree(root);
    auto charMap = character_map(pqueue,root);
    auto str01 = str_to_str01(str,charMap);
    auto cmpStr = str01_to_cmpStr(str01);
    string huffmanString;
    huffmanString += to_string(str.size());
    huffmanString.push_back('\n');
    huffmanString += to_string(charMap.size());
    huffmanString.push_back('\n');
    for (const auto& el : charMap) {
        huffmanString += to_string((int) el.first)+ " " + el.second.first;
        huffmanString.push_back('\n');
    }
    huffmanString += cmpStr;
    return huffmanString;
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
        char a = str8bit_to_char(str.substr(i,8));
        cmpStr.push_back(a);
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

void write(const string &fileName, const string &str) {
    ofstream file(fileName);
    file << str;
    file.close();
}

void decompress_file(const string &fileName) {
    ifstream file(fileName);
    int charCount, mapCount;
    file >> charCount >> mapCount;
    rchar_map map;
    for (int i = 0; i < mapCount; ++i) {
        int character; string bin;
        file >> character >> bin;
        map.insert({bin,character});
    }
    string line;
    string lines;
    getline(file,line);
    lines.append(line);
    while (file.good()) {
        getline(file,line);
        lines.append(line);
        lines.push_back('\n');
    }
    auto str = decode(lines,map,charCount);
    string decompressed = fileName;
    decompressed.erase(decompressed.end() - 4,decompressed.end());
    decompressed += "_decomp.txt";
    ofstream filewriter(decompressed);
    filewriter << str;
}

std::string decode(const string &lines,const rchar_map& map, int charCount) {
    auto str01 = chars_to_01(lines);
    int min = INT32_MAX;
    int max = INT32_MIN;
    for (const auto& el : map) {
        int size = el.first.size();
        if (min > size) {
            min = size;
        }
        if (max < size) {
            max = size;
        }
    }
    string decodedString;
    int index = 0;
    for (int i = 0; i < charCount; ++i) {
        char character;
        int added;
        for (int j = min; j <= max; ++j) {
            auto subStr = str01.substr(index,j);
            if(map.count(subStr) != 0) {
                character = map.at(subStr);
                added = j;
            }
        }
        decodedString.push_back(character);
        index+= added;
    }
    return decodedString;
}

std::string char_to_01(char character) {
    unsigned char u_char = character;
    string binary;
    for (unsigned char i = 128; i > 0 ; i /= 2) {
        if(u_char/i == 1) {
            binary.push_back('1');
            u_char -= i;
        } else {
            binary.push_back('0');
        }
    }
    return binary;
}

std::string chars_to_01(const std::string& chars) {
    string binary;
    for (const auto& el : chars) {
        binary += char_to_01(el);
    }
    return binary;
}

void printTree(TreeNode *root) {
    auto lines = new vector<vector<string*>*>;

    auto level = new vector<TreeNode*>();
    auto next = new vector<TreeNode*>();

    level->push_back(root);
    int nn = 1;

    int widest = 0;

    while (nn != 0) {
        auto line = new vector<string*>;
        nn = 0;

        for (const auto& n : *level) {
            if (n == nullptr) {
                line->push_back(nullptr);

                next->push_back(nullptr);
                next->push_back(nullptr);
            } else {
                auto aa = new string(n->toString());
                line->push_back(aa);
                if (aa->length() > widest) widest = aa->length();

                next->push_back(n->left);
                next->push_back(n->right);

                if (n->left != nullptr) nn++;
                if (n->right != nullptr) nn++;
            }
        }

        if (widest % 2 == 1) widest++;

        lines->push_back(line);

        vector<TreeNode*>* tmp = level;
        level = next;
        next = tmp;
        next->clear();
    }

    int perpiece = lines->at(lines->size() - 1)->size() * (widest + 4);
    for (int i = 0; i < lines->size(); i++) {
        auto line = lines->at(i);
        int hpw = (int) floor((double)perpiece / 2.0) - 1;

        if (i > 0) {
            for (int j = 0; j < line->size(); j++) {

                // split node
                string c;
                if (j % 2 == 1) {
                    if (line->at(j - 1) != nullptr) {
                        c = (line->at(j) != nullptr) ? "┴" : "┘";
                    } else {
                        if (j < line->size() && line->at(j) != nullptr) c = ""
                                                                            "└";
                    }
                }
                cout << c;

                // lines and spaces
                if (line->at(j) == nullptr) {
                    for (int k = 0; k < perpiece - 1; k++) {
                        cout << " ";
                    }
                } else {

                    for (int k = 0; k < hpw; k++) {
                        cout << (j % 2 == 0 ? " " : "─");
                    }
                    cout << (j % 2 == 0 ? "┌" : "┐");
                    for (int k = 0; k < hpw; k++) {
                        cout << (j % 2 == 0 ? "─" : " ");
                    }
                }
            }
            cout << endl;
        }

        // print line of numbers
        for (int j = 0; j < line->size(); j++) {

            auto f = line->at(j);
            if (f == nullptr) f = new string("");
            int gap1 = (int) ceil((double)perpiece / 2.0 - (double)f->length() / 2.0);
            int gap2 = (int) floor((double)perpiece / 2.0 - (double)f->length() / 2.0);

            // a number
            for (int k = 0; k < gap1; k++) {
                cout << (" ");
            }
            cout << (*f);
            for (int k = 0; k < gap2; k++) {
                cout << " ";
            }
        }
        cout << endl;

        perpiece /= 2;
    }
}


