//HFMTree.h
#ifndef HFMCODE_HFMTREE_H
#define HFMCODE_HFMTREE_H

#include <vector>
#include <string>
#include <queue>

#include "HFMNode.h"
#include <map>

class HFMTree {
public:
    HFMNode *root;

//用一个map来储存字符及其编码的关系，查找某个字符对应编码的复杂度是O(logn)
    std::map<char, std::string> encode;

//由一个频度表和一个字符集创建哈夫曼树
    void init(std::vector<double> v, std::string s);

//给定一个文件指针，将其写入该文件中
    void write_to_file(FILE *);

//给定一个文件指针，从这个文件中创哈夫曼树
    void build_from_file(FILE *);

//编码
    void encoding();

};


#endif //HFMCODE_HFMTREE_H
