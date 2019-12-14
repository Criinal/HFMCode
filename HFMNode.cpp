//HFMNode.cpp

#include "HFMNode.h"

//初始化节点
//频度(value)和深度(depth)都初始化为0
//index是它的标号
//ch默认为'N'
//左右节点初始化为空指针(nullptr)
void HFMNode::init(int ind) {
    value = 0;
    ch = 'N';
    left = right = nullptr;
    index = ind;
    depth = 0;
}

//重载<符号，用于优先队列/堆
bool operator<(HFMNode a, HFMNode b) {
    return a.value > b.value;
}

//通过递归的方式将节点写入文件，遍历的方式是后序遍历
void HFMNode::dfs_write(FILE *fp) {
//    如果该节点的左子节点不为空，说明它不是叶子节点，那么继续搜索。
    if (left != nullptr) {
        left->depth = depth + 1;
        left->dfs_write(fp);
    }
//    如果该节点的右子节点不为空，说明这个不是叶子节点，那么继续搜索。
    if (right != nullptr) {
        right->depth = depth + 1;
        right->dfs_write(fp);
    }
//将信息输出到文件，用逗号隔开
    fprintf(fp, "%f,%c,%d,%d,", value, ch, index, depth);

//如果左子节点指针是空，则将它的左子节点标号记为1，否则记为它左子节点的标号
//右子节点指针同
    fprintf(fp, "%d,", left == nullptr ? -1 : left->index);
    fprintf(fp, "%d\n", right == nullptr ? -1 : right->index);

}

//递归编码
void HFMNode::dfs_encoding(std::string code, std::map<char, std::string> &encode) {
//如果当前还不是叶子节点，则分别往左子树和右子树走
//往左子树走, 在code的末尾追加一个字符'0', 并遍历左子树
//往右子树走, 在code的末尾追加一个字符'1', 并遍历右子树
    if (left != nullptr && right != nullptr) {
        left->dfs_encoding(code + "0", encode);
        right->dfs_encoding(code + "1", encode);
        return;
    }

//运行到此处表示走到叶子节点，此时将ch和code构成的pair放入map中
//例如此时的字符为'a', code为"0011", 则encode['a'] = "0011"
    encode.insert(make_pair(ch, code));
}

