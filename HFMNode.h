//HFMNode.h

#ifndef HFMCODE_HFMNODE_H
#define HFMCODE_HFMNODE_H


#include <cstdio>
#include <string>
#include <map>

class HFMNode {
public:
//频度
    double value;

//字符，如果它是某两个节点的父节点，则默认为'N'（在初始化中体现），实际上并没有意义
    char ch;

//它的标号，每个节点的标号都不同
    int index;

//它的深度，用于广度优先遍历
    int depth;

//两个指针，指向它的左右子树，为空则表示这个点是叶子节点
    HFMNode *left, *right;

//初始化，传入的参数是它的标号
    void init(int);

//重载小于号，用于优先队列
//优先队列必须提供小于号运算符
    friend bool operator<(HFMNode, HFMNode);

//用深度优先搜索的方式将节点写入文件，使用的是后序遍历
//即按左子树、右子树、根节点的方式输出
    void dfs_write(FILE *);

//编码，第一个字符串是是从根节点开始到当前节点得到的字符串
//第二个map是用于储存某个字符和它对应编码的关系的结构
    void dfs_encoding(std::string,
                      std::map<char, std::string> &);
};


#endif //HFMCODE_HFMNODE_H
